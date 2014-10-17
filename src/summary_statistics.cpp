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
      NumericMatrix mat(forest.segment_count(), 2);

      mat.attr("dimnames") = 
        List::create(R_NilValue, CharacterVector::create("tmrca", "tree_length"));
      as<List>(sum_stats[i])[locus] = mat;
    }
  }
}
