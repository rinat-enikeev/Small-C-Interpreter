@echo off
pushd .
cd tests
call run_tests.bat
popd
smallC.exe
pause