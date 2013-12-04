gcc -o testAnalyzer testAnalyzer.c analyzer.c lib.c interpreter.c
./testAnalyzer
gcc -o testPrescan testPrescan.c analyzer.c lib.c interpreter.c
./testPrescan
gcc -o testArrays testArrays.c analyzer.c lib.c interpreter.c
./testArrays
gcc -o smallC smallC.c analyzer.c lib.c interpreter.c
./smallC
