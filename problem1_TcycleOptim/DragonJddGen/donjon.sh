#!/bin/sh
#
# author : A. Hebert
# use    : donjon <file.x2m> [-w]
# note   : <file.x2m> must be located on directory ./data/
#          If <file.access> exists, it is executed.
#          -w to execute in console
#
if [ $# = 0 ]
   then
   echo "usage: $0 file" 1>&2
   exit 1
fi

xxx=0
term=0
log=0

for param in $*
do
   case $param in
      -w) echo Execute in terminal
          term=1
          ;;
      *) if [ $log = 0 ]; then
            xxx=`basename $param .x2m`
            log=1
            data=$param
            typ='custom'
         else
            typ=$param
         fi ;;
   esac
done
Code="/home/physor/bin/POLYMTL/VERSION4/Version4_lgpl/Donjon/bin/Linux*/Donjon"
echo 'execute' $xxx 'with' $Code

RUN="run"

if [ -d "$RUN" ]
  then
  echo 'use the existing directory' $RUN
else
  echo 'creation of directory' $RUN
  mkdir "$RUN"
fi
CodeDir=$PWD

Tmpdir=/tmp
inum=1
while [ -d $Tmpdir/rundir$inum ]
  do 
  inum=`expr $inum + 1 `
done
Rundir=$Tmpdir/rundir$inum
mkdir $Rundir
echo "RunDirectory:" $Rundir
cd $Rundir
pwd
cp $Code ./code
cp "$CodeDir"/data/$data ./mydata
if [ -d "$CodeDir"/data/`echo $xxx`_proc ]
  then
  cp "$CodeDir"/data/`echo $xxx`_proc/*.c2m .
fi
if [ -f "$CodeDir"/data/$xxx.access ]
  then
  "$CodeDir"/data/$xxx.access "$CodeDir"
fi
if [ -f "$CodeDir"/data/tdraglib.access ]
  then
  "$CodeDir"/data/tdraglib.access "$CodeDir"
fi
if [ -f "$CodeDir"/data/twlup.access ]
  then
  "$CodeDir"/data/twlup.access "$CodeDir"
fi
if [ $term = 0 ]
   then
   time ./code <mydata >$xxx.result
else
   time ./code <mydata
fi
if [ -f "$CodeDir"/data/$xxx.save ]
  then
  "$CodeDir"/data/$xxx.save "$CodeDir"
fi
echo 'end of execution'
mv * "$CodeDir"/"$RUN"
echo 'the listing is located on ./'$RUN

cd "$CodeDir"/"$RUN"
if [ $term = 0 ]
   then
   tail -15 $xxx.result
fi
chmod -R 777 $Rundir
/bin/rm -r -f $Rundir
cd ..
