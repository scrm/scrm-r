#include <Rcpp.h>

#include <iostream>

#include "scrm/param.h"
#include "scrm/forest.h"
#include "scrm/model.h"

#include "r_random_generator.h"

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

//' Function to call scrm
//' @export
//' @examples
//' scrm('5 1 -r 10 100 -t 5')
// [[Rcpp::export]]
List scrm(std::string args) {
  std::ostream *output = &Rcout;
  
  Param param = Param("scrm " + args);
  Rcout << param << std::endl;
  Model model;
  param.parse(model);
  RRandomGenerator rrg;
  
  Forest forest = Forest(&model, &rrg);
  
  // Loop over the independent loci/chromosomes
  for (size_t rep_i=0; rep_i < model.loci_number(); ++rep_i) {
    
    // Mark the start of a new independent sample
    *output << std::endl << "//" << std::endl;
    
    // Now set up the ARG, and sample the initial tree
    forest.buildInitialTree();
    forest.printSegmentSumStats(*output);
    
    while (forest.next_base() < model.loci_length()) { 
      forest.sampleNextGenealogy();
      forest.printSegmentSumStats(*output);
    }
    
    forest.printLocusSumStats(*output);
    forest.clear();
  }
    
  return List::create();
}
