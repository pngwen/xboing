#!/bin/bash

cc=gcc $(pkg-config raylib --cflags) -c 

# test compile each file
succeed=1
echo "Compiler Output" > COMPILER.OUT
for x in $(ls *.c)
do
    echo -n ${x}...
    echo "" >> COMPILER.OUT
    echo "" >> COMPILER.OUT
    echo "" >> COMPILER.OUT
    echo "$x" >> COMPILER.OUT
    if gcc $(pkg-config raylib --cflags) -c  $x 2>>  COMPILER.OUT 
    then
      echo "PASS"
    else
      echo "FAIL"
      succeed=0
    fi
done

if [ $succeed -eq "1" ]; then
  echo "All tests succeeded"
else
  echo "Some tests failed."
  exit 1
fi
