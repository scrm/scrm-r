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

#ifndef scrmr_summary_statistics
#define scrmr_summary_statistics

#include <Rcpp.h>
#include <vector>
#include <string>
#include "scrm/summary_statistics/summary_statistic.h"
#include "scrm/summary_statistics/seg_sites.h"
#include "scrm/summary_statistics/tmrca.h"
#include "scrm/summary_statistics/newick_tree.h"
#include "scrm/summary_statistics/oriented_forest.h"
#include "scrm/summary_statistics/frequency_spectrum.h"

using namespace Rcpp;


void addLocusSumStats(const Forest &forest, size_t locus,
                      List &sumstat_list_, CharacterVector newick_trees);

template <typename T>
T* getSumStat(const Forest &forest) {
  for (size_t i = 0; i < forest.model().countSummaryStatistics(); ++i) {
    SummaryStatistic* sum_stat = forest.model().getSummaryStatistic(i);
    if (typeid(*sum_stat) == typeid(T)) {
      return dynamic_cast<T*>(sum_stat);
    }
  }
  return NULL;
}

std::string readSegmentTree(SummaryStatistic* sum_stat);

class SumStatStore {
 public:
  SumStatStore (const Forest &forest) {
    this->sumstat_list_ = List(forest.model().countSummaryStatistics());
    CharacterVector names(forest.model().countSummaryStatistics(), "");
    nt_ = NULL;
    of_ = NULL;

    for (size_t i = 0; i < forest.model().countSummaryStatistics(); ++i) {
      SummaryStatistic* sum_stat = forest.model().getSummaryStatistic(i);

      if (typeid(*sum_stat) == typeid(SegSites)) {
        sumstat_list_(i) = List(forest.model().loci_number());
        names(i) = "seg_sites";
      }

      else if (typeid(*sum_stat) == typeid(TMRCA)) {
        sumstat_list_(i) = List(forest.model().loci_number());
        names(i) = "tmrca";
      }

      else if (typeid(*sum_stat) == typeid(NewickTree)) {
        sumstat_list_(i) = List(forest.model().loci_number());
        names(i) = "trees";
        nt_ = dynamic_cast<NewickTree*>(sum_stat);
      }

      else if (typeid(*sum_stat) == typeid(OrientedForest)) {
        sumstat_list_(i) = List(forest.model().loci_number());
        names(i) = "oriented_forest";
        of_ = dynamic_cast<OrientedForest*>(sum_stat);
      }

      else if (typeid(*sum_stat) == typeid(FrequencySpectrum)) {
        sumstat_list_(i) = NumericMatrix(forest.model().loci_number(),
                  forest.model().sample_size() - 1);
        names(i) = "sfs";
      }

      //else stop("Failed to parse unknown Summary Statistic.");
    }

    sumstat_list_.names() = names;
  }

  void addSegmentStats(const Forest &forest) {
    std::string tmp_tree;

    if (nt_ != NULL) {
      tmp_tree = readSegmentTree(nt_);
      if (tmp_tree.size() > 0) nt_trees_.push_back(tmp_tree);
    }

    if (of_ != NULL) {
      tmp_tree = readSegmentTree(of_);
      if (tmp_tree.size() > 0) of_trees_.push_back(tmp_tree);
    }
  }

  void addLocusStats(const Forest &forest, size_t locus) {
    for (size_t i = 0; i < forest.model().countSummaryStatistics(); ++i) {
      SummaryStatistic* sum_stat = forest.model().getSummaryStatistic(i);

      if (typeid(*sum_stat) == typeid(SegSites)) {
        SegSites* ss = dynamic_cast<SegSites*>(sum_stat);
        NumericMatrix seg_sites(forest.model().sample_size(), ss->countMutations());
        for (size_t col = 0; col < ss->countMutations(); ++col) {
          for (size_t row = 0; row < forest.model().sample_size(); ++row) {
            seg_sites(row,col) = (*(ss->getHaplotype(col)))[row];
          }
        }
        seg_sites.attr("dimnames") = List::create(R_NilValue, *(ss->positions()));
        as<List>(sumstat_list_[i])[locus] = seg_sites;
      }

      else if (typeid(*sum_stat) == typeid(TMRCA)) {
        TMRCA* tmrca = dynamic_cast<TMRCA*>(sum_stat);
        as<List>(sumstat_list_[i])[locus] = DataFrame::create(
          _["tmrca"] = tmrca->tmrca(),
          _["tree_length"] = tmrca->tree_length()
        );
      }

      else if (typeid(*sum_stat) == typeid(NewickTree)) {
        CharacterVector trees(no_init(nt_trees_.size()));
        std::copy(nt_trees_.begin(), nt_trees_.end(), trees.begin());
        as<List>(sumstat_list_[i])[locus] = trees;
        nt_trees_.clear();
      }

      else if (typeid(*sum_stat) == typeid(OrientedForest)) {
        as<List>(sumstat_list_[i])[locus] = of_trees_;
        of_trees_ = CharacterVector(0);
      }

      else if (typeid(*sum_stat) == typeid(FrequencySpectrum)) {
        FrequencySpectrum* sfs = dynamic_cast<FrequencySpectrum*>(sum_stat);
        std::vector<size_t> sfs_vec = sfs->sfs();
        for (size_t j = 0; j < sfs_vec.size(); ++j) {
          as<NumericMatrix>(sumstat_list_[i])(locus,j) = sfs_vec.at(j);
        }
      }
    }
  };

  List getStats() { return sumstat_list_; };

 private:
  List sumstat_list_;
  NewickTree* nt_;
  std::list<std::string> nt_trees_;
  OrientedForest* of_;
  CharacterVector of_trees_;

  std::string readSegmentTree(SummaryStatistic* sum_stat) {
    std::stringstream ss;
    sum_stat->printSegmentOutput(ss);
    std::string tree = ss.str();
    if (tree.size() > 2) tree.erase(tree.size() - 1);
    return tree;
  }
};

#endif
