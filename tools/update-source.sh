#!/bin/bash
#
# Updates the source code files of scrm in scrm-r/src/scrm
# with files from scrm's git repo supposed to be at
# ../scrm

rsync -rvcA --exclude '.*'\
            --include '*/'\
            --include '*.cc' --include '*.h'\
            --exclude='*'\
            ../scrm/src/* src/scrm/
