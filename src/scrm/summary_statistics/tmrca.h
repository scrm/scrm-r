/*
 * scrm is an implementation of the Sequential-Coalescent-with-Recombination Model.
 * 
 * Copyright (C) 2013, 2014 Paul R. Staab, Sha (Joe) Zhu, Dirk Metzler and Gerton Lunter
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

#ifndef scrm_src_summary_statistic_tmrca
#define scrm_src_summary_statistic_tmrca

#include <sstream>
#include <iostream>
#include <vector>

#include "summary_statistic.h"
#include "../forest.h"

class TMRCA : public SummaryStatistic
{
 public:
   TMRCA() {};
   ~TMRCA() {};

   //Virtual methods
   void calculate(const Forest &forest);
   void printLocusOutput(std::ostream &output) const;
   void clear() {
     tmrca_.clear();
     tree_length_.clear();
   }
   
   TMRCA* clone() const { return new TMRCA(); } 

   const std::vector<double> & tmrca() const { return tmrca_; }
   const std::vector<double> & tree_length() const { return tree_length_; }

 private:
   std::vector<double> tmrca_;
   std::vector<double> tree_length_;
};

#endif
