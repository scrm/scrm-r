#ifndef scrm_macros
#define scrm_macros

#include "Rcpp.h"

// Assure that assertions are deactived
#ifndef NDEBUG
#define NDEBUG
#endif

#define dout Rcpp::Rcout

// from Knuths "The art of computer programming"
inline bool areSame(const double a, const double b, 
                    const double epsilon = std::numeric_limits<double>::epsilon()) {
  return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

#endif
