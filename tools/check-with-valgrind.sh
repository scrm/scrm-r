#!/bin/bash

script="$(tempfile)"
echo "devtools::test()" > $script
R -d valgrind --debugger-args='--leak-check=full' --vanilla < $script 2>&1 | less
rm $script
