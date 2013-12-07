@echo off
del /F *.obj
pushd .
cd tests
call clear_tests.bat
popd
