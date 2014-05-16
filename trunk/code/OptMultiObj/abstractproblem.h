#ifndef ABSTRACTPROBLEM_H
#define ABSTRACTPROBLEM_H

#include "individual.h"

class AbstractProblem
{
public:
    AbstractProblem(int, int);
    int numberOfParameters;
    int numberOfObjectives;

    virtual void evaluateIndividual(Individual & r) = 0 ;
    virtual Individual generateIndividual() ;
    virtual bool individualIsAllowed(Individual & r) ;

    int getNumberOfIterations() ;

    int numberOfEvaluations ;
};

#endif // ABSTRACTPROBLEM_H
