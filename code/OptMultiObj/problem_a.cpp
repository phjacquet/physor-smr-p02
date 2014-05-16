#include "problem_a.h"
#include <QtMath>

Problem_A::Problem_A() : AbstractProblem(2,2)
{
}

/*
void Problem_A::evaluateIndividual(Individual & r) {
    double x,y,x0,y0,x1,y1,R0,R1,RM0,RM1;
    x=r.parameters[0] ;
    y=r.parameters[1] ;
    x0=0. ;
    y0=0. ;
    R0=0.5 ;
    x1=1. ;
    y1=1. ;
    R1=0.5 ;

    RM0=qSqrt( (x-x0)*(x-x0) + (y-y0)*(y-y0) ) ;
    RM1=qSqrt( (x-x1)*(x-x1) + (y-y1)*(y-y1) ) ;

    r.objectives[0]=(RM0-R0)*(RM0-R0) ;
    r.objectives[1]=(RM1-R1)*(RM1-R1) ;
}
*/

/*
void Problem_A::evaluateIndividual(Individual & r) {
    //arbitaire : f0 est minimisé pour ||M(x,y) - M0||=R0.
    //arbitaire : f1 est minimisé pour ||M(x,y) - M1||=R1.
    double x,y,x0,y0,x1,y1,R0,R1,RM0,RM1;
    x=r.parameters[0] ;
    y=r.parameters[1] ;
    x0=0.5 ;
    y0=0.5 ;
    R0=0.1 ;
    x1=0.5 ;
    y1=0.5 ;
    R1=0.3 ;

    RM0=qSqrt( (x-x0)*(x-x0) + (y-y0)*(y-y0) ) ;
    RM1=qSqrt( (x-x1)*(x-x1) + (y-y1)*(y-y1) ) ;

    r.objectives[0]=(RM0-R0)*(RM0-R0) ;
    r.objectives[1]=(RM1-R1)*(RM1-R1) ;
}
*/

void Problem_A::evaluateIndividual(Individual & r) {
    double x,y,x0,y0,x1,y1,R0,R1,RM0,RM1;
    x=r.parameters[0] ;
    y=r.parameters[1] ;

    r.objectives[0]=4*x*x+y*y+x*y ;
    r.objectives[1]=(x-1)*(x-1)+3*(y-1)*(y-1) ;

    numberOfEvaluations++;
}

