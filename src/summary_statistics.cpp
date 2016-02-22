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


void addLocusSumStats(const Forest &forest, size_t locus, List &sum_stats) {
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

      // These two are only references.
      DataFrame tmrca_data = DataFrame::create(
        _["tmrca"] = tmrca->tmrca(),
        _["tree_length"] = tmrca->tree_length()
      );

      as<List>(sum_stats[i])[locus] = tmrca_data;
    }

    else if (typeid(*sum_stat) == typeid(NewickTree)) {
      NewickTree* nt = dynamic_cast<NewickTree*>(sum_stat);
      CharacterVector stat(1);
      std::stringstream ss;
      nt->printLocusOutput(ss);
      stat[0] = ss.str();
      as<List>(sum_stats[i])[locus] = stat;
    }

    else if (typeid(*sum_stat) == typeid(OrientedForest)) {
      OrientedForest* of = dynamic_cast<OrientedForest*>(sum_stat);
      as<List>(sum_stats[i])[locus] = of->getTrees();
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
