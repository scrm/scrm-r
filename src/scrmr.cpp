#include <Rcpp.h>

// #include "scrm/param.h"
// #include "scrm/forest.h"

#include "r_random_generator.h"

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]

//' Function to call scrm
//' @export
//' @examples
//' scrm('')
// [[Rcpp::export]]
List scrm(std::string args) {
  RRandomGenerator rrg;
  Rprintf("%f\n", rrg.sample());
  return List::create("blub", "BLA");
}
