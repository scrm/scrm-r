#!/bin/bash

docker run --rm -ti -v $(pwd):/mnt rocker/r-devel-ubsan-clang \
  check.r --setwd /mnt --install-deps $1
