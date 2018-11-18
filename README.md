[![Linux Build Status](https://travis-ci.org/scrm/scrm-r.svg?branch=master)](https://travis-ci.org/scrm/scrm-r)
[![Windows Build Status](https://ci.appveyor.com/api/projects/status/octt2w69byiq198s/branch/master?svg=true)](https://ci.appveyor.com/project/paulstaab/scrm-r/branch/master)
[![CRAN Status](http://www.r-pkg.org/badges/version/scrm)](https://cran.r-project.org/package=scrm)


scrm R Package
=======================

_scrm_ is a coalescence simulator for the evolution 
of biological sequences. It is available as a command line program at
[scrm.github.io](https://scrm.github.io).

This R package contains a copy _scrm_, packaged for convenient usage in R.


Installation
======================

It is recommended to use the current CRAN version. It can be installed
from within R using

```
install.packages('scrm')
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


Citation
======================
Please cite the following article when using _scrm_ in a publication:

> Paul R. Staab, Sha Zhu, Dirk Metzler and Gerton Lunter.
> **scrm: efficiently simulating long sequences using the approximated coalescent
> with recombination**. 
> Bioinformatics (2015) 31 (10): 1680-1682.
> [doi:10.1093/bioinformatics/btu861](http://bioinformatics.oxfordjournals.org/content/31/10/1680).


Bug Reports
=======================
Please report any problems via the [issue tracker](https://github.com/scrm/scrm-r/issues)
or via email to `develop (at) paulstaab (dot) de`.

Please include the version you are using and the exact command that 
causes the problem including seed (if applicable) in the report.
Also, feel free to suggest features there.
