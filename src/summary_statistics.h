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


List initSumStats(const Forest &forest);
void addLocusSumStats(const Forest &forest, size_t locus,
                      List &sum_stats, CharacterVector newick_trees);
NewickTree* getNewickTree(const Forest &forest);
std::string readSegmentTree(SummaryStatistic* sum_stat);

#endif
