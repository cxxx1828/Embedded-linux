#! /bin/bash

LOGFILE=logfile

{

date

echo
who

echo
lscpu

echo
free -h

echo
uptime


} | tee "$LOGFILE"

exit 0
