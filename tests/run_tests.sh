#! /bin/sh
echo "test_if"
if [ $(./test_if 1>/dev/null 2>/dev/null) ]; then
  echo "SUCCESSFUL"
else
  echo "FAILED"
fi
