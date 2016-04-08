lintr::lint_package()
covr::coveralls(line_exclusions = c("R/RcppExports.R",
                                    "src/RcppExports.cpp",
                                    list.files("src/scrm",
                                               recursive = TRUE,
                                               full.names = TRUE)))
