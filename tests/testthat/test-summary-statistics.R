context("Summary Statistics")

test_that("Warning is thrown when no sumstats are present", {
  expect_warning(scrm("2 1"))
  expect_warning(scrm("2 1 -I 2 1 1 1.0"))
})

test_that("SegSites import works", {
  sum_stats <- scrm("5 2 -t 7")
  expect_is(sum_stats, "list")
  expect_is(sum_stats$seg_sites, "list")
  expect_equal(length(sum_stats$seg_sites), 2)

  expect_is(sum_stats$seg_sites[[1]], "matrix")
  expect_is(sum_stats$seg_sites[[2]], "matrix")
  expect_equal(nrow(sum_stats$seg_sites[[1]]), 5)
  expect_equal(nrow(sum_stats$seg_sites[[2]]), 5)

  col_names <- colnames(sum_stats$seg_sites[[1]])
  expect_false(is.null(col_names))
  expect_true(all(0 < col_names & col_names < 1))

  col_names <- colnames(sum_stats$seg_sites[[2]])
  expect_false(is.null(col_names))
  expect_true(all(0 < col_names & col_names < 1))

  expect_true(all(sum_stats$seg_sites[[1]] %in% c(0, 1)))
  expect_true(all(sum_stats$seg_sites[[2]] %in% c(0, 1)))
})

test_that("TMRCA import works", {
  sum_stats <- scrm("2 3 -r 1 100 -L")
  expect_is(sum_stats, "list")
  expect_is(sum_stats$tmrca, "list")
  expect_equal(length(sum_stats$tmrca), 3)

  for (tmrca in sum_stats$tmrca) {
    expect_is(tmrca, "data.frame")
    expect_equal(ncol(tmrca), 2)
    expect_that(nrow(tmrca), is_more_than(0))
    expect_true(all(tmrca > 0))
  }
})

test_that("Newick Tree import works", {
  stats <- scrm("4 2 -r 1 100 -T")
  expect_is(stats, "list")
  expect_is(stats$trees, "list")
  expect_equal(length(stats$trees), 2)

  for (locus in stats$trees) {
    for (tree in locus) {
      expect_is(tree, "character")
      expect_that(nchar(tree), is_more_than(3))
      expect_true(grepl("^\\[[0-9]+\\]\\(.*\\);$", tree))
    }
  }

  expect_true(stats$trees[[1]][1] != stats$trees[[2]][1])
})

test_that("Oriented Forest import works", {
  stats <- scrm("4 4 -r 1 100 -O")
  expect_is(stats, "list")
  expect_is(stats$oriented_forest, "list")
  expect_equal(length(stats$oriented_forest), 4)
  for (locus in stats$oriented_forest) {
    for (tree in locus) {
      expect_is(tree, "character")
      expect_that(nchar(tree), is_more_than(3))
    }
  }
  
  expect_true(stats$oriented_forest[[1]][1] != stats$oriented_forest[[2]][1])
})

test_that("Frequency spectrum import works", {
  sum_stats <- scrm("7 5 -r 1 100 -t 5 -oSFS")
  expect_is(sum_stats, "list")
  expect_is(sum_stats$sfs, "matrix")
  expect_equal(nrow(sum_stats$sfs), 5)
  expect_equal(ncol(sum_stats$sfs), 6)
  for (i in seq_len(nrow(sum_stats$sfs))) {
    expect_equal(sum(sum_stats$sfs[i,]), ncol(sum_stats$seg_sites[[i]]))
  }
})
