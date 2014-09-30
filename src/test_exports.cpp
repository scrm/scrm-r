#include <Rcpp.h>

#include "r_random_generator.h"

using namespace Rcpp;

RRandomGenerator rrg;

// [[Rcpp::export]]
double test_RRG_sample() {
   return rrg.sample();
}

// [[Rcpp::export]]
double test_RRG_sampleUnitExpo() {
  return rrg.sampleUnitExponential();
}

// [[Rcpp::export]]
double test_RRG_sampleExpoExpoLimit(double lambda, double b, double limit) {
  return rrg.sampleExpoExpoLimit(lambda, b, limit);
}