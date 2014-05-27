#!/bin/bash

echo "_________________________________________"
echo "Generateur de jeu de donnees pour Dragon - test"
echo "Version X.X"
echo "_________________________________________"

srcdir="./src" ;
templdir="./files" ;
outdir="./files/Lib6G-XMAS172" ;

VecteurPu=`awk '$1=="VecteurPu" {print $2}' CoreParameters.d `;
vp=`awk '$1=="'$VecteurPu'" {print $8}' $srcdir/vecteursPu.dat `;

rm -r run/
for E in  `seq 1 50`
do

echo "E=$E"

file="$templdir/FCELL.templ" ;
fuelFile="$templdir/DragonFuelCompo.templ" ;
perl $srcdir/TrisoCompact.pl $E $VecteurPu $fuelFile ;
$srcdir/tool.replace_by_block.sh VAR.C.FUEL "$fuelFile.fuel_proc" $file > $file.proc1
sed 's/VAR.G.filename/FC'$E.$vp'.db/'  $file.proc1 > $outdir/fcell$E.$vp.x2m
rm $fuelFile.fuel_proc
rm $file.proc*

mv $outdir/fcell$E.$vp.x2m data/
./donjon.sh fcell$E.$vp.x2m

done

mv run/*.db $outdir/
mv run/*.result $outdir/

