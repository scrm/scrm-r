#include <Rcpp.h>

#include <iostream>
#include <fstream>
#include <memory>

#include "scrm/param.h"
#include "scrm/forest.h"
#include "scrm/model.h"
#include "scrm/random/fastfunc.h"

#include "summary_statistics.h"
#include "r_random_generator.h"

using namespace Rcpp;
std::ofstream fs;
bool write_file;


// [[Rcpp::plugins(cpp11)]]

//' Simulate the evolution of biological sequences
//'
//' This function provides an interface for calling \emph{scrm} from R.
//' The command line options are passed via the \code{args} argument.
//' The vignette 'scrm-Arguments' contains details about the available options.
//' Summary statistics are converted into an R format. Additionally, there
//' is an option to write the original command line output into a file.
//'
//' @section Seeding:
//' The R version of scrm uses random number from R's random generator.
//' Therefore, the '-seed' argument of the command line version will be ignored,
//' and no seed is given in the output.
//' Use the R function \code{\link{set.seed}} prior to calling this
//' function to ensure reproduciblity of results.
//'
//' @param args A string containing the command line arguments for scrm. Look at
//'             scrms vignette for a description of available arguments.
//' @param file If provided, scrm will additionally write it is output into
//'             a file with the given file, using an ms-like text output.
//' @return A named list of summary statistics. Most summary statistics are
//'         again a list, where each entry contains the value for one locus.
//'         For the site frequency spectrum, the summary statistic is a matrix,
//'         where each row contains the spectrum for one locus.
//' @export
//' @keywords datagen
//' @seealso
//' \itemize{
//'   \item \code{vignette('scrm-Arguments')} for an overview of commandline arguments and
//'   \item \code{vignette('scrm-TreesForApe')} for an example on using
//'         genealogies simulated with \emph{scrm} with package 'ape'.
//' }
//' @examples
//' set.seed(789)
//' # 5 Chromosomes with 100 bases each with recombination and mutation
//' sum_stats <- scrm('5 1 -r 3.1 100 -t 1.5 -T -L')
//' str(sum_stats)
//'
//' # Simulate the site frequency spectrum at 3 loci. For each locus
//' # 10 Chromosomes of 1Mb length are sampled from two populations with
//' # migration inbetween.
//' scrm('10 3 -r 400 1000000 -l 100000 -I 2 4 6 0.5 -t 300 -oSFS')$sfs
//'
// [[Rcpp::export]]
List scrm(std::string args, std::string file = "") {

  /** Parse args and generate the model */
  Param param(args);
  Model model = param.parse();

  // Print help or version information if user asked for it
  if (param.help()) stop("Please use '?scrm' for help");
  if (param.version()) {
    return List::create(_("version") = VERSION);
  }

  std::shared_ptr<FastFunc> ff = std::make_shared<FastFunc>();
  RRandomGenerator rrg(ff);

  /** Open a file for writing if 'file' is given */
  if (file.length() > 0) {
    fs.open(file);
    if(!fs.is_open()) stop(std::string("Failed to write the file '") + file +
                           std::string("'! Does the directory exist?"));
    write_file = true;
    fs << param << std::endl;
  } else {
    write_file = false;
  }

  /** Throw a warning if -seed argmuent is used */
  if (param.seed_is_set())
    Rf_warning("Ignoring seed argument. Set a seed in R.");

  /** Throw a warning if no summary statistics are used */
  if (model.countSummaryStatistics() == 0)
    Rf_warning("No summary statisics specified. No output will be produced.");

  Forest forest = Forest(&model, &rrg);

  SumStatStore stats_store(forest);

  // Loop over the independent loci/chromosomes
  for (size_t locus = 0; locus < model.loci_number(); ++locus) {

    // Mark the start of a new independent sample
    if (write_file) fs << std::endl << "//" << std::endl;

    // Now set up the ARG, and sample the initial tree
    forest.buildInitialTree();
    stats_store.addSegmentStats(forest);
    if (write_file) forest.printSegmentSumStats(fs);

    while (forest.next_base() < model.loci_length()) {
      forest.sampleNextGenealogy();
      stats_store.addSegmentStats(forest);
      if (write_file) forest.printSegmentSumStats(fs);
    }

    stats_store.addLocusStats(forest, locus);
    if (write_file) forest.printLocusSumStats(fs);

    forest.clear();
  }

  /** Clean up */
  if (write_file) fs.close();

  return stats_store.getStats();
}
