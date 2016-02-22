#include "summary_statistics.h"

List initSumStats(const Forest &forest) {
  List sum_stats(forest.model().countSummaryStatistics());
  CharacterVector names(forest.model().countSummaryStatistics(), "");

  for (size_t i = 0; i < forest.model().countSummaryStatistics(); ++i) {
    SummaryStatistic* sum_stat = forest.model().getSummaryStatistic(i);

    if (typeid(*sum_stat) == typeid(SegSites)) {
      sum_stats(i) = List(forest.model().loci_number());
      names(i) = "seg_sites";
    }

    else if (typeid(*sum_stat) == typeid(TMRCA)) {
      sum_stats(i) = List(forest.model().loci_number());
      names(i) = "tmrca";
    }

    else if (typeid(*sum_stat) == typeid(NewickTree)) {
      sum_stats(i) = List(forest.model().loci_number());
      names(i) = "trees";
    }

    else if (typeid(*sum_stat) == typeid(OrientedForest)) {
      sum_stats(i) = List(forest.model().loci_number());
      names(i) = "oriented_forest";
    }

    else if (typeid(*sum_stat) == typeid(FrequencySpectrum)) {
      sum_stats(i) = NumericMatrix(forest.model().loci_number(),
                                   forest.model().sample_size() - 1);
      names(i) = "sfs";
    }

    else stop("Failed to parse unknown Summary Statistic.");
  }

  sum_stats.names() = names;
  return sum_stats;
}


void addLocusSumStats(const Forest &forest,
                      size_t locus,
                      List &sum_stats,
                      CharacterVector newick_trees) {

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
      as<List>(sum_stats[i])[locus] = seg_sites;
    }

    else if (typeid(*sum_stat) == typeid(TMRCA)) {
      TMRCA* tmrca = dynamic_cast<TMRCA*>(sum_stat);
      as<List>(sum_stats[i])[locus] = DataFrame::create(
        _["tmrca"] = tmrca->tmrca(),
        _["tree_length"] = tmrca->tree_length()
      );
    }

    else if (typeid(*sum_stat) == typeid(NewickTree)) {
      as<List>(sum_stats[i])[locus] = newick_trees;
    }

    else if (typeid(*sum_stat) == typeid(OrientedForest)) {
      OrientedForest* of = dynamic_cast<OrientedForest*>(sum_stat);
      //as<List>(sum_stats[i])[locus] = of->getTrees();
    }

    else if (typeid(*sum_stat) == typeid(FrequencySpectrum)) {
      FrequencySpectrum* sfs = dynamic_cast<FrequencySpectrum*>(sum_stat);
      std::vector<size_t> sfs_vec = sfs->sfs();
      for (size_t j = 0; j < sfs_vec.size(); ++j) {
        as<NumericMatrix>(sum_stats[i])(locus,j) = sfs_vec.at(j);
      }
    }
  }
}

NewickTree* getNewickTree(const Forest &forest) {
  for (size_t i = 0; i < forest.model().countSummaryStatistics(); ++i) {
    SummaryStatistic* sum_stat = forest.model().getSummaryStatistic(i);
    if (typeid(*sum_stat) == typeid(NewickTree)) {
      return dynamic_cast<NewickTree*>(sum_stat);
    }
  }
  return NULL;
};

std::string readSegmentTree(SummaryStatistic* sum_stat) {
  std::stringstream ss;
  sum_stat->printSegmentOutput(ss);
  std::string tree = ss.str();
  //tree.erase(tree.end()-2);
  return tree;
}
