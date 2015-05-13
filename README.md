[![Linux Build Status](https://travis-ci.org/scrm/scrm-r.svg?branch=master)](https://travis-ci.org/scrm/scrm-r)
[![Windows Build Status](https://ci.appveyor.com/api/projects/status/octt2w69byiq198s/branch/master?svg=true)](https://ci.appveyor.com/project/paulstaab/scrm-r/branch/master)
[![CRAN Status](http://www.r-pkg.org/badges/version/scrm)](http://cran.r-project.org/web/packages/scrm)
[![CRAN RStudio Mirror Downloads](http://cranlogs.r-pkg.org/badges/scrm)](http://cran.r-project.org/web/packages/scrm/index.html)

R Package scrm
=======================

_scrm_ is a coalescence simulator for the evolution 
of biological sequences. It is available as a command line program at
[scrm.github.io](https://scrm.github.io).

This R package contains a copy _scrm_, packaged for convenient usage in R.


Installation
======================
__This packages requires an R version of `3.1.0` or higher.__

It is recommended to use the current CRAN version. It can be installed
from within R using

```
install.packages('scrm')
```

The development version can be installed from GitHub using
```
devtools::install_github('scrm/scrm-r')
```


Usage
======================
Use the function `scrm::scrm()` to call _scrm_:
```
library('scrm')
sum_stats <- scrm('5 1 -r 10 100 -t 5 -oSFS')
```


Help & Documentation
======================
- The basic usage of `scrm::scrm()` is explained in its R help page `help('scrm')`.
- The package contains a vignette on _scrm_'s command line arguments: `vignette('scrm-Arguments')`.
- Online documentation for the command line program is available in 
  [_scrm's Wiki_](https://github.com/paulstaab/scrm/wiki).


Bug Reports
=======================
Please report problems with _scrm_ using its 
[issue tracker](https://github.com/scrm/scrm-r/issues). 
Please include the version you are using and the exact command that 
causes the problem including seed (if applicable) in the report.
Also, feel free to suggest features you would like to see in _scrm_ there.
