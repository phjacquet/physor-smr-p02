dKTarget=0.10000 ; #10000 pcm sous le k(DDV)

awk 'BEGIN {T1=0;T2=100000;K1=10;K2=0}
            ($1==0) {KTarget=$2-'$dKTarget';}
            ($2>KTarget && $2<K1) {T1=$1;K1=$2}
            ($2<KTarget && $2>K2) {T2=$1;K2=$2}
	    {T[NR]=$1; K[NR]=$2; }
            END {
	    if (K2==0) {K2=K[NR-1];T2=T[NR-1]}
	    print "KTARGET= " KTarget " " K1 " " K2 " " T1 " " T2 ;
	    TCYCLE=T1+(KTarget-K1)*(T2-T1)/(K2-K1); 
	    print TCYCLE
	    }'  evol.out 
