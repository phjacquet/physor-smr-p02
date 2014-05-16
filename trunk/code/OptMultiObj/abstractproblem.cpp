#include "abstractproblem.h"

static double randomValue()
{
    // a number between [ 0.0, 1.0 ]
    double d=qrand();
    double dmax=RAND_MAX ;

    return d/dmax;
}

AbstractProblem::AbstractProblem(int a, int b) : numberOfParameters(a), numberOfObjectives(b)
{
    numberOfEvaluations=0;
}

Individual AbstractProblem::generateIndividual()  {
    Individual r(numberOfParameters,numberOfObjectives) ;
    for ( unsigned i = 0 ; i < numberOfParameters ; i++ )
       r.parameters[i]=randomValue() ;
    return r;
}

bool AbstractProblem::individualIsAllowed(Individual & r) {
    return true;
}


int AbstractProblem::getNumberOfIterations() {
    return numberOfEvaluations ;
}
