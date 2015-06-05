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

#include "newick_tree.h"

void NewickTree::calculate(const Forest &forest) {
  if (forest.calcSegmentLength() == 0.0) return;
  output_buffer_.push_back(generateTree(forest.local_root(), forest)); 
  segment_length_.push_back(forest.calcSegmentLength());
}

void NewickTree::printLocusOutput(std::ostream &output) const {
  assert( output_buffer_.size() == segment_length_.size() );
  if (!has_rec_) {
    assert( output_buffer_.size() == 1 );
    output << *(output_buffer_.begin()) << ";" << std::endl;
  } else {
    size_t i = 0;
    for (auto tree : output_buffer_) {
      output << "[" << segment_length_[i++] << "]" << tree << ";" << std::endl;
    }
  }
}

/**
 * @brief Prints a part of the tree in newick format
 *
 * @param node The root of the subtree that will be printed
 *
 * @return A part of the tree in newick format
 */
std::string NewickTree::generateTree(Node *node, const Forest &forest, const bool use_buffer) {
  // Use tree from buffer if possible
  std::map<Node const*, NewickBuffer>::iterator it = buffer_.find(node);
  if (use_buffer && it != buffer_.end()) {
    if (it->second.recombination > node->last_change()) {
      // Check that the buffered tree is correct.
      assert(it->second.tree.compare(generateTree(node, forest, false)) == 0);
      return it->second.tree;
    }
  }

  // Generate a new tree
  std::stringstream tree;
  tree.precision(this->precision_);
  tree.exceptions(std::ios::failbit); 
  if (node->in_sample()) tree << node->label();
  else { 
    Node *left = node->getLocalChild1();
    Node *right = node->getLocalChild2();

    tree << "(" << generateTree(left, forest, use_buffer) << ":" <<
           (node->height() - left->height()) * forest.model().scaling_factor() <<
           "," << generateTree(right, forest, use_buffer) << ":" <<
           (node->height() - right->height()) * forest.model().scaling_factor() << ")";
  }

  // And add to to the buffer
  if (use_buffer) {
    NewickBuffer buf = {forest.current_rec(), tree.str()};
    buffer_[node] = buf; 
  }

  return tree.str();
}
