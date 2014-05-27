my ($E, $vPu, $file) = @ARGV;

print "TrisoCompact.pl \n" ;
if ( scalar( @ARGV ) != 3 ) {
	print "ERROR: incorrect number of arguments : found ", scalar( @ARGV ) ," arguments \n";
}

print join(" ",@ARGV), "\n";

#vecteurPu : ratio des isotopes du Pu, dans l'ordre : NomDuVecteur Pu-238 Pu-239 Pu-240 Pu-241 Pu-242 Am241
my @vecteurPu ;
my $MassPu ;
open(FP,'<','./src/vecteursPu.dat');
while(<FP> )   { if ($_ =~ "$vPu*" ) { @vecteurPu=split(' ', $_); }}
close(FP);
print "Vecteur Pu ",$vPu," : ",join(" ",@vecteurPu), "\n" ;
$MassPu = ($vecteurPu[1]*238. +           
           $vecteurPu[2]*239. +           
	   $vecteurPu[3]*240. +           
	   $vecteurPu[4]*241. +           
	   $vecteurPu[5]*242. +           
	   $vecteurPu[6]*241.) / 100. ;   
print "Masse Pu = ",$MassPu, "\n" ;

#compoNucl : tableau des noyaux du milieu compact
my @compoNucl = ("U238","Pu0","Pu238","Pu239","Pu240","Pu241","Pu242","Am241") ;

my $U238 = 1.9282E-02/2.* (1. - $E/100.) ;
my $Pu   = 1.9282E-02/2.* (     $E/100.) ;
my $Pu238   = $vecteurPu[1]/100.*$Pu ;
my $Pu239   = $vecteurPu[2]/100.*$Pu ;
my $Pu240   = $vecteurPu[3]/100.*$Pu ;
my $Pu241   = $vecteurPu[4]/100.*$Pu ;
my $Am241   = $vecteurPu[5]/100.*$Pu ;

my @layerCompo = (
[$U238                 ],
[$Pu                   ],
[$vecteurPu[1]/100.*$Pu],
[$vecteurPu[2]/100.*$Pu],
[$vecteurPu[3]/100.*$Pu],
[$vecteurPu[4]/100.*$Pu],
[$vecteurPu[5]/100.*$Pu],
[$vecteurPu[6]/100.*$Pu]);


$ifile=$file ;
$ofile="$file.fuel_proc" ;
open(FILE_IN,$ifile);
my @contenu = <FILE_IN>;
close(FILE_IN);
 
open(FILE_OUT,">$ofile");
foreach my $ligne (@contenu)
{
for ($i = 0; $i < 8 ; $i++) {
    $VarTag = "VAR.C.$compoNucl[$i]" ;
    $VarVal = sprintf("%E", $layerCompo[$i][0]) ;
    $ligne =~ s/$VarTag/$VarVal/g;
}
    chomp $ligne;
    print FILE_OUT "$ligne\n";
}
close(FILE_OUT);
