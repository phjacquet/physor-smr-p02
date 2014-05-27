#!/bin/bash

echo "_________________________________________"
echo "Generateur de jeu de donnees pour Dragon - test"
echo "Version X.X"
echo "_________________________________________"

srcdir="./src" ;
templdir="./files" ;
outdir="./files/Lib6G-XMAS172" ;

###################################################################
#   *** Control Tube Cell                                         #
###################################################################
file="$templdir/CTCELL.templ" ;
cp  $file $file.proc
sed 's/VAR.G.filename/CTDB.db/' $file.proc > $outdir/ctcell.x2m
rm $file.proc

mv $outdir/ctcell.x2m data/

rm -r run/

./donjon.sh ctcell.x2m

mv run/*.db $outdir/
mv run/*.result $outdir/

