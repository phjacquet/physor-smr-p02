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
file="$templdir/RCELL.templ" ;
$srcdir/tool.replace_by_block.sh VAR.R.ODS "$srcdir/AcierODS_1.0.dat" $file > $file.proc
sed 's/VAR.G.filename/REFL.db/' $file.proc > $outdir/reflcell.x2m
rm $file.proc

mv $outdir/reflcell.x2m data/

rm -r run/

./donjon.sh reflcell.x2m

mv run/*.db $outdir/
mv run/*.result $outdir/

