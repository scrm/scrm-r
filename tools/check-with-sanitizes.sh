#!/bin/bash

docker run --rm -ti -v $(pwd):/mnt rocker/r-devel-san \
  Rscriptdevel -e "devtools::test('/mnt')"
