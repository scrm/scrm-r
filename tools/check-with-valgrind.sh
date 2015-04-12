#!/bin/bash

script="$(tempfile)"
echo "devtools::test()" > $script
R -d "valgrind --tool=memcheck --leak-check=full" --vanilla < $script 2>&1 | less
rm $script
