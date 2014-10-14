context("Summary Statistics")

test_that("SegSites", {
  set.seed(32)
  sum_stats <- scrm('5 2 -t 7')
  expect_is(sum_stats, "list")
  expect_equal(length(sum_stats), 2)
  expect_is(sum_stats[[1]]$seg_sites, 'matrix')
  expect_is(sum_stats[[2]]$seg_sites, 'matrix')
  expect_equal(nrow(sum_stats[[1]]$seg_sites), 5)
  expect_equal(nrow(sum_stats[[2]]$seg_sites), 5)
  
  col_names <- colnames(sum_stats[[1]]$seg_sites)
  expect_false(is.null(col_names))
  expect_true(all(0 < col_names & col_names < 1))
  
  col_names <- colnames(sum_stats[[2]]$seg_sites)
  expect_false(is.null(col_names))
  expect_true(all(0 < col_names & col_names < 1))
  
  expect_true(all(sum_stats[[1]]$seg_sites %in% c(0,1)))
  expect_true(all(sum_stats[[2]]$seg_sites %in% c(0,1)))
})