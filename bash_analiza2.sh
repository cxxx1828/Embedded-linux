#!/bin/bash

MAX=10000


  for((nr=1; nr<$MAX; nr++))
  do
    
    if (((nr % 5==3)) &&  ((nr % 7==4)) && ((nr % 9==5)))
    then
    	echo "Number = $nr"
    fi

  break  

  done

  


exit 0
