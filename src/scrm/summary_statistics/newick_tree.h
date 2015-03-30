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

#ifndef scrm_src_summary_statistic_newick_tree
#define scrm_src_summary_statistic_newick_tree

#include <sstream>
#include <iostream>
#include <string>
#include <map>

#include "summary_statistic.h"
#include "../forest.h"


/**
 * @brief Save buffered tree along with the sequence position at which
 * they where created. 
 */
struct NewickBuffer {
  double position;  ///< The sequence position at which the subtree was created.
  std::string tree; ///< The subtree itself.
};

class NewickTree : public SummaryStatistic
{
 public:
   NewickTree() { 
    precision_ = 6; 
   }

   NewickTree(size_t precision) { 
    precision_ = precision; 
   }

   //NewickTree(const NewickTree &nt) { (void)nt; }
   ~NewickTree() {}

   //Virtual methods
   void calculate(const Forest &forest);
   void printLocusOutput(std::ostream &output) const;

   NewickTree* clone() const { return new NewickTree(); };

   void clear() {
     output_buffer_.str("");
     output_buffer_.clear();
     buffer_.clear();
   }

   std::string getTrees() const { return output_buffer_.str(); }

 private:
   std::string generateTree(Node *node, const Forest &forest,
                            const bool use_buffer = true);
   std::ostringstream output_buffer_;
   size_t precision_;

   /**
    * A map to buffer already created subtrees indexed by their 
    * root.
    */
   std::map<Node const*, NewickBuffer> buffer_;
};

#endif
