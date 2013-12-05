#! /bin/sh
tests="test_if test_loops test_vars test_funcs"

for test in $tests; do
  if $(./$test 1>/dev/null); then
    echo $test	"SUCCESSFUL"
  else
    echo $test	"FAILED"
  fi
done