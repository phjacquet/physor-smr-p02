#!/bin/bash
cd ~/projects/physor-smr-p02/problem1_TcycleOptim
lpwd=$PWD

ALPHAVAR=`echo $1|sed s#e#*10^#`
HCVAR=`echo $2|sed s#e#*10^#`
PNUCLVAR=`echo $3|sed s#e#*10^#`

#***********************************************************************************************
#DEFINITION DU RANGE DES PARAMETRES
Range_APLHA=(0.3 1.1)
Range_HC=(70. 200.)
Range_PNUCL=(10. 400.)


#***********************************************************************************************
#DEFINITION DU JEU DE PARAMETRES
ALPHA=` echo "${Range_APLHA[0]} + $ALPHAVAR*(${Range_APLHA[1]}-${Range_APLHA[0]})" | bc  `
HC=` echo "${Range_HC[0]} + $HCVAR*(${Range_HC[1]}-${Range_HC[0]})" | bc  `
PNUCL=` echo "${Range_PNUCL[0]} + $PNUCLVAR*(${Range_PNUCL[1]}-${Range_PNUCL[0]})" | bc  `
#echo $ALPHA $HC $PNUCL

#***********************************************************************************************
#GENERATION DES JDD
cat CoreParameters.templ | sed "s/VAR.CORE.HC/$HC/" | sed "s/VAR.CORE.PNUCL/$PNUCL/" | sed "s/VAR.Comb.ALPHA/$ALPHA/" > CoreParameters.proc1

cp CoreParameters.proc1  CoreParameters.d
rm CoreParameters.proc*


#***********************************************************************************************
#REALISATION DES CALCULS DANS LA FOULEE
rdir="$HOME/projects/physor-smr-p02/problem1_TcycleOptim/DragonJddGen"
CalculationScript="./Evol.sh"
mv CoreParameters.d $rdir
cd $rdir
$CalculationScript > $CalculationScript.out 2>&1
cp results $lpwd

#***********************************************************************************************
#RETOUR DES RESULTATS
cd $lpwd

if [[ `grep "nan" results` == "" ]] 
then
MC=`awk '{print $1}' results` ;
E=`awk '{print $2}' results` ;
TCYCLE=`awk '{print $3}' results` ;

MCtarget=10.     # 10 t  
Etarget=8.      # 10 %
TCYCLEtarget=20. # 20 ans
PNucltarget=400. # 400 MWth
BUtarget=200. # 200 GWth.d/t

BUreached=`echo "($TCYCLE*$PNUCL/$MC/1000.)"|bc -l `

f1=`echo "($MC-$MCtarget)^2"|bc -l `
f2=`echo "($E-$Etarget)^2"|bc -l `
#f3=`echo "($TCYCLE/365.-$TCYCLEtarget)^2"|bc -l `
#f4=`echo "($PNUCL-$PNucltarget)^2"|bc -l `
f3=`echo "($BUtarget-$BUreached)^2"|bc -l `

comment="$ALPHAVAR $HCVAR $PNUCLVAR $f1 $f2 $f3 { $ALPHA $HC $PNUCL }  { `cat results`	$BUreached }"

echo "$f1 ; $f2 ; $f3 ; COMMENT= $comment"
echo "$comment" >> $lpwd/script.out
else
echo "Calcul non converge : $comment" >&2
echo "Calcul non converge : $comment" >> $lpwd/script.out
fi

