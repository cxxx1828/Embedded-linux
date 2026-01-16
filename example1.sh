#!/bin/bash

echo "Hello from script ${0} arg1=$1"

echo;echo

# Listing digits
for digit in {a..z}
do
echo $digit
done

echo;echo

for arg in a b c d e f
do
if [ "$arg" == "b" ]
then
continue
fi
echo quiz
done

echo;echo

