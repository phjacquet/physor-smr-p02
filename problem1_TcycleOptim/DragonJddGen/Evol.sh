#!/bin/bash

echo "_________________________________________"
echo "Generateur de jeu de donnees pour Dragon - test"
echo "Version X.X"
echo "_________________________________________"

srcdir="./src" ;
templdir="./files" ;
outdir="./out" ;
dbdir="./files/Lib6G-XMAS172" ;

###################################################################
#   *** Core                                                      #
###################################################################
echo "Recherche d enrichissement tel que keff (t=0) = 1.10000"
time ./CriticSearch.sh 1.1 > CriticSearch.out
cat keff.out
echo "Fin de la recherche d enrichissement"
mv CoreParameters.proc CoreParameters.final
mv run/FLUX.db FLUX.db

Alpha=`awk '$1=="Alpha" {print $2}' CoreParameters.final `;
HC=`awk '$1=="HauteurCoeur" {print $2}' CoreParameters.final `;
RC=`awk '$1=="RayonCoeur" {print $2}' CoreParameters.final `;
PNucleaire=`awk '$1=="PNucleaire" {print $2}' CoreParameters.final `;
VecteurPu=`awk '$1=="VecteurPu" {print $2}' CoreParameters.final `;
vp=`awk '$1=="'$VecteurPu'" {print $8}' $srcdir/vecteursPu.dat `;
E1=`awk '$1=="E1" {print $2}' CoreParameters.final `;
E2=`awk '$1=="E2" {print $2}' CoreParameters.final `;

VC=`echo "3.14159*$RC*$RC*$HC" | bc`                   # volume coeur en CC
FuelDens="3.814274"                                    # fuel density approx en G/CC
MFuel=`echo "$VC*$FuelDens*10^-6"   | bc -l `            # Masse de fuel dans le coeur en tonne
PNMass=`echo "$PNucleaire/($MFuel)" | bc -l |awk '{print int($1*100)/100.}'` # Puissance nucleaire du coeur en MW/tonne
E=`echo "($E1+$E2)/2" |bc -l `

echo "VC	$VC" >> CoreParameters.final 
echo "MFuel	$MFuel" >> CoreParameters.final 
echo "FuelDens	$FuelDens" >> CoreParameters.final 
echo "PNMass	$PNMass" >> CoreParameters.final 


###################################################################
#   *** Core                                                      #
###################################################################
file="$templdir/CORE.templ" ;
geoFile="$templdir/DragonCoreGeom_TGE.templ" ;
perl $srcdir/geometry.core.pl $HC  $geoFile ;
cp $file $file.proc
for c in `seq 1 6`
do
$srcdir/tool.replace_by_block.sh VAR.G.GCELL$c "$geoFile.geom_proc" $file.proc       > $file.proc1
sed 's/VAR.CORE.PNUCL/'$PNMass'/'  $file.proc1 > $file.proc2

mv $file.proc2 $file.proc
done
rm $geoFile.geom_proc

cp  $file.proc $outdir/core.x2m

rm -r run
mkdir run
cp  $PWD/$dbdir/FC$E1.$vp.db run/FCIDB.db
cp  $PWD/$dbdir/FC$E2.$vp.db run/FCODB.db
cp  $PWD/$dbdir/CTDB.db    run/CTDB.db
cp  $PWD/$dbdir/HEDB.db    run/HEDB.db
cp  $PWD/$dbdir/REFL.db    run/REDB.db
cp  $PWD/FLUX.db           run/FLUX.db

rm $file.proc*

./donjon.sh core.x2m

awk '   BEGIN {BU=0}
        /FINAL BURNUP AT TIME/ {BU=$7} 
        /KINF/ {print BU "\t" $9}'  run/core.result |sed s#KEFF=## > evol.out
awk '($2!="FINAL") {print $0}' evol.out >evol.tmp
mv evol.tmp evol.out
cat evol.out

dKTarget=0.10000 ; #10000 pcm sous le k(DDV)


TCYCLE=`awk 'BEGIN {T1=0;T2=100000;K1=10;K2=0}
            ($1==0) {KTarget=$2-'$dKTarget';}
            ($2>KTarget && $2<K1) {T1=$1;K1=$2}
            ($2<KTarget && $2>K2) {T2=$1;K2=$2}
	    {T[NR]=$1; K[NR]=$2; }
            END {
	    if (K2==0) {K2=K[NR-1];T2=T[NR-1]}
	    TCYCLE=T1+(KTarget-K1)*(T2-T1)/(K2-K1); 
	    print TCYCLE
	    }'  evol.out `

echo $MFuel	$E	$TCYCLE	> results
cat results
