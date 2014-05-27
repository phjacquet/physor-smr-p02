my ($HC, $file) = @ARGV;
print "geometry.core.pl \n";
print join(" ",@ARGV), "\n";
if ( scalar( @ARGV ) != 2 ) {
	print "ERROR: incorrect number of arguments : found ", scalar( @ARGV ) ," arguments \n";
}

#****************************************************************#
#*** Partie Axiale                                              *#
    
    $HC1 = 1./8. * $HC ;
    $HC2 = 2./8. * $HC ;
    $HC3 = 3./8. * $HC ;
    $HC4 = 4./8. * $HC ;

$ifile=$file ;
$ofile="$file.geom_proc" ;
open(FILE_IN,$ifile);
my @contenu = <FILE_IN>;
close(FILE_IN); 
open(FILE_OUT,">$ofile");
foreach my $ligne (@contenu)
{

#****************************************************************#
#*** Partie Axiale                                              *#

    $VarTag = "VAR.G.HC1" ;
    $VarVal = sprintf("%f", $HC1) ;
    $ligne =~ s/$VarTag/$VarVal/g;

    $VarTag = "VAR.G.HC2" ;
    $VarVal = sprintf("%f", $HC2) ;
    $ligne =~ s/$VarTag/$VarVal/g;

    $VarTag = "VAR.G.HC3" ;
    $VarVal = sprintf("%f", $HC3) ;
    $ligne =~ s/$VarTag/$VarVal/g;

    $VarTag = "VAR.G.HC4" ;
    $VarVal = sprintf("%f", $HC4) ;
    $ligne =~ s/$VarTag/$VarVal/g;


    chomp $ligne;
    print FILE_OUT "$ligne\n";

#****************************************************************#


}
close(FILE_OUT);
