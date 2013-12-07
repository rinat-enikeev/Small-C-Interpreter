@echo off
cl /MT /TP smallC.c analyzer.c lib.c interpreter.c
pushd .
cd tests
call make_tests.bat
popd
call clear.bat
