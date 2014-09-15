#!/bin/bash
#
# %title%
# %description%
# 
# Author:   Paul R. Staab 
# Email:    staab (at) bio.lmu.de
# Date:     2014-09-15
# Licence:  GPLv3 or later
#

rsync -rvcA --exclude='.*' --exclude='*.o' ../scrm/src/* src/scrm/
