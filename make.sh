#! /bin/sh
cd tests
./make_tests.sh
cd ..
gcc -Wall -o smallC smallC.c analyzer.c lib.c interpreter.c
