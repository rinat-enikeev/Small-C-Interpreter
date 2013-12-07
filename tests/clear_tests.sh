#! /bin/sh
tests="test_analyzer test_arrays test_funcs test_if test_loops test_prescan test_vars"

for test in $tests; do
  rm $test
done
