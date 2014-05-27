#!/bin/bash

if [[ $# != 1 ]]
   then
   echo "usage: $0 KeffTarget" 1>&2
   exit 1
fi



echo "_________________________________________"
echo "Generateur de jeu de donnees pour Dragon - test"
echo "Version X.X"
echo "_________________________________________"

KTarget=$1
echo "CIBLE keff = $KTarget"

srcdir="./src" ;
templdir="./files" ;
outdir="./out" ;
dbdir="./files/Lib6G-XMAS172" ;

#$srcdir/fillGeomConstrainedParam.sh ;

Alpha=`awk '$1=="Alpha" {print $2}' CoreParameters.d `;
HC=`awk '$1=="HauteurCoeur" {print $2}' CoreParameters.d `;
PNucleaire=`awk '$1=="PNucleaire" {print $2}' CoreParameters.d `;
VecteurPu=`awk '$1=="VecteurPu" {print $2}' CoreParameters.d `;
vp=`awk '$1=="'$VecteurPu'" {print $8}' $srcdir/vecteursPu.dat `;

echo "E	E1	E2	KEFF"> keff.out

for E in  8  20
do

echo "E=$E Alpha=$Alpha"
E1=`echo "2*$E/(1+1/$Alpha)" |bc -l `;
E2=`echo "$E1/$Alpha"      |bc -l `;
echo "enrichissement : E1=$E1 E2=$E2"
E1=`echo "$E1"|awk '{print int($1)}'`
E2=`echo "$E2"|awk '{print int($1)}'`
echo "enrichissement arrond. : E1=$E1 E2=$E2"
###################################################################
#   *** Core                                                      #
###################################################################

file="$templdir/CritSearchCore.templ" ;
geoFile="$templdir/DragonCoreGeom_TGE.templ" ;
perl $srcdir/geometry.core.pl $HC  $geoFile ;
cp $file $file.proc
$srcdir/tool.replace_by_block.sh VAR.G.GCELL$c "$geoFile.geom_proc" $file.proc       > $file.proc1
mv $file.proc1 $file.proc
rm $geoFile.geom_proc

cp  $file.proc $outdir/core.x2m
rm $file.proc*

rm -r run
mkdir run

 FCIf=$PWD/$dbdir/FC$E1.$vp.db
 FCOf=$PWD/$dbdir/FC$E2.$vp.db
 if [[ -e $FCIf ]] && [[ -e $FCOf ]] 
 then
 cp  $FCIf run/FCIDB.db
 cp  $FCOf run/FCODB.db
 cp  $PWD/$dbdir/CTDB.db    run/CTDB.db
 cp  $PWD/$dbdir/HEDB.db    run/HEDB.db
 cp  $PWD/$dbdir/REFL.db    run/REDB.db

 ./donjon.sh core.x2m
 grep KINF run/core.result
 KEFF=`grep KINF run/core.result |awk  '{print $9}'|sed s#KEFF=##`
 
 echo "$E	$E1	$E2	$KEFF">> keff.out
 fi

done



E=`awk 'BEGIN {El=0;Er=100;Kl=0;Kr=10}
     ($4<'$KTarget' && $4>Kl) {El=$1;Kl=$4}
     ($4>'$KTarget' && $4<Kr) {Er=$1;Kr=$4}
     END {E=El+('$KTarget'-Kl)*(Er-El)/(Kr-Kl); print E}' keff.out `


echo "E=$E Alpha=$Alpha"
E1=`echo "2*$E/(1+1/$Alpha)" |bc -l `;
E2=`echo "$E1/$Alpha"      |bc -l `;
echo "enrichissement : E1=$E1 E2=$E2"
E1=`echo "$E1"|awk '{print int($1)}'`
E2=`echo "$E2"|awk '{print int($1)}'`
E=`echo "($E1+$E2)/2" |bc -l `
echo "enrichissement arrond. : E1=$E1 E2=$E2 E=`echo "($E1+$E2)/2."|bc`"
###################################################################
#   *** Core                                                      #
###################################################################

file="$templdir/CritSearchCore.templ" ;
geoFile="$templdir/DragonCoreGeom_TGE.templ" ;
perl $srcdir/geometry.core.pl $HC  $geoFile ;
cp $file $file.proc
$srcdir/tool.replace_by_block.sh VAR.G.GCELL$c "$geoFile.geom_proc" $file.proc       > $file.proc1
mv $file.proc1 $file.proc
rm $geoFile.geom_proc

cp  $file.proc $outdir/core.x2m
rm $file.proc*

rm -r run
mkdir run

 FCIf=$PWD/$dbdir/FC$E1.$vp.db
 FCOf=$PWD/$dbdir/FC$E2.$vp.db
 if [[ -e $FCIf ]] && [[ -e $FCOf ]] 
 then
 cp  $FCIf run/FCIDB.db
 cp  $FCOf run/FCODB.db
 cp  $PWD/$dbdir/CTDB.db    run/CTDB.db
 cp  $PWD/$dbdir/HEDB.db    run/HEDB.db
 cp  $PWD/$dbdir/REFL.db    run/REDB.db

 ./donjon.sh core.x2m
 grep KINF run/core.result
 KEFF=`grep KINF run/core.result |awk  '{print $9}'|sed s#KEFF=##`
 
 echo "$E	$E1	$E2	$KEFF">> keff.out
 fi

E=`awk 'BEGIN {El=0;Er=100;Kl=0;Kr=10}
     ($4<'$KTarget' && $4>Kl) {El=$1;Kl=$4}
     ($4>'$KTarget' && $4<Kr) {Er=$1;Kr=$4}
     END {E=El+('$KTarget'-Kl)*(Er-El)/(Kr-Kl); print E}' keff.out `


echo "E=$E Alpha=$Alpha"
E1=`echo "2*$E/(1+1/$Alpha)" |bc -l `;
E2=`echo "$E1/$Alpha"      |bc -l `;
echo "enrichissement : E1=$E1 E2=$E2"
E1=`echo "$E1"|awk '{print int($1)}'`
E2=`echo "$E2"|awk '{print int($1)}'`
echo "enrichissement arrond. : E1=$E1 E2=$E2 E=`echo "($E1+$E2)/2."|bc`"

echo "$E	$E1	$E2	STOP">> keff.out

###################################################################
#   *** CoreParameter filling                                     #
###################################################################

sed 's/VAR.Comb.E1/'$E1'/'  CoreParameters.d | sed 's/VAR.Comb.E2/'$E2'/' > CoreParameters.proc

