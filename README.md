scrm-r
=======================

[![Build Status](https://travis-ci.org/scrm/scrm-r.svg?branch=master)](https://travis-ci.org/scrm/scrm-r)

_scrm_, packaged for convenient usage in R.


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

Versioning
======================
We will stick to the sheme version `x.y-z`, where `x.y` is the version 
of _scrm_ included in the package, and `z` is the number of CRAN releases 
for this _scrm_ version, starting at `1` (`0` is used for development).
