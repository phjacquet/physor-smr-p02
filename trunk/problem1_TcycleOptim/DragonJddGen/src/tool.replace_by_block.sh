#!/bin/bash

if [ $# != 3 ]
   then
   echo "usage: $0 param file.dat file.templ" 1>&2
   echo "param : string to replace"
   echo "file.dat : replacement block"
   echo "file.templ : file"
   echo "only $# args found"
   exit 1
fi

awk ' BEGIN        {before=1}
      /'$1'/  {before=0} 
      before==1    {print $0}       ' $3  > /tmp/f
cat $2 >> /tmp/f
awk ' BEGIN        {before=1}
      before==0    {print $0}
      /'$1'/  {before=0}            ' $3 >> /tmp/f

cat /tmp/f 
