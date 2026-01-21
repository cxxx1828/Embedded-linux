#!/bin/sh
# try to run this script from shell on different ways:
# ./example3.sh
# source example3.sh
# also check the values of variables in shell before and after running
# try to define values in shell before running the script. try it also with export
# try to pass the value of variable to the script and check what happens in script and after execution
#		var=2 ./example3.sh

echo ${var}
echo $EXT_VAR
var=5
echo $var
export EXT_VAR=7
echo $EXT_VAR
