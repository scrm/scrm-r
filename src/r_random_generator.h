/*
 * scrm is an implementation of the Sequential-Coalescent-with-Recombination Model.
 * 
 * Copyright (C) 2013, 2014 Paul R. Staab, Sha (Joe) Zhu and Gerton Lunter
 * 
 * This file is part of scrm.
 * 
 * scrm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef scrmr_r_random_generator
#define scrmr_r_random_generator

#include <Rcpp.h>
#include "scrm/random/random_generator.h"

using namespace Rcpp;

class RRandomGenerator : public RandomGenerator {
 public:
  RRandomGenerator() {};
  virtual ~RRandomGenerator() {};

  void initialize() {};
  double sample() { 
    RNGScope scope;
    return R::runif(0,1); 
  }
};

#endif