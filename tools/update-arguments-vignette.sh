#!/bin/bash
#
# Updates the source code files of scrm in scrm-r/src/scrm
# with files from scrm's git repo supposed to be at
# ../scrm

scrm_dir=$PWD
if [ ! -d "$scrm_dir/vignettes" ]; then
  "Error: please execute this script from scrm-r's folder"
  exit 1
fi

cd $(mktemp -d)
git clone https://github.com/scrm/scrm.wiki.git || exit 1

cat << EOF > scrm-Arguments.Rmd
---
title: "Command Line Arguments"
author: "Paul Staab"
date: "\`r Sys.Date()\`"
output: rmarkdown::html_vignette
vignette: >
  %\VignetteIndexEntry{scrm-Arguments}
  %\VignetteEngine{knitr::rmarkdown}
  %\usepackage[utf8]{inputenc}
---
EOF

cat scrm.wiki/Command-Line-Options.md | tail -n +2 >> scrm-Arguments.Rmd
rsync -avP scrm-Arguments.Rmd "$scrm_dir/vignettes/"
