#!/bin/bash
#
# Updates the source code files of scrm in scrm-r/src/scrm
# with files from scrm's git repo supposed to be at
# ../scrm

mkdir tmp
git clone https://github.com/scrm/scrm tmp/


rsync -rvcA --exclude '.*'\
            --include '*/'\
            --include '*.cc' --include '*.h'\
            --exclude='*'\
            tmp/src/* src/scrm/

rm -rf tmp

