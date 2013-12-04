#! /bin/sh
gcc -Wall -o testAnalyzer testAnalyzer.c analyzer.c lib.c interpreter.c
./testAnalyzer
gcc -Wall -o testPrescan testPrescan.c analyzer.c lib.c interpreter.c
./testPrescan
gcc -Wall -o testArrays testArrays.c analyzer.c lib.c interpreter.c
./testArrays
gcc -Wall -o smallC smallC.c analyzer.c lib.c interpreter.c
./smallC
