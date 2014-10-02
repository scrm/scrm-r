context("scrm")

test_that("Parsing arguments works", {
  expect_error(scrm('10 -t 5'))
  expect_error(scrm('10 1 -t'))
  
  scrm('5 2 -t 5')
  scrm('5 1 -t 5 -r 10 100')
  scrm('5 1 -T')
  scrm('5 1 -I 2 2 3 1.5 -r 5 100')
})

test_that("Warning is given when using -seed", {
  expect_warning(scrm('10 1 -seed 17'))
  expect_warning(scrm('10 1 -seed 1 2 3'))
})