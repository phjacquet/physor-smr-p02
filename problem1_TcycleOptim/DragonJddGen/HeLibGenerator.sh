#!/bin/bash

echo "_________________________________________"
echo "Generateur de jeu de donnees pour Dragon - test"
echo "Version X.X"
echo "_________________________________________"

srcdir="./src" ;
templdir="./files" ;
outdir="./files/Lib6G-XMAS172" ;

###################################################################
#   *** Helium Cell                                               #
###################################################################
file="$templdir/HECELL.templ" ;
cp  $file $file.proc
sed 's/VAR.G.filename/HEDB.db/' $file.proc > $outdir/hecell.x2m
rm $file.proc

mv $outdir/hecell.x2m data/

rm -r run/

./donjon.sh hecell.x2m

mv run/*.db $outdir/
mv run/*.result $outdir/

