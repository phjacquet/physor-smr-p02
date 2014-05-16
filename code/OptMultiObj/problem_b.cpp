#include "problem_b.h"
#include <QtMath>

Problem_B::Problem_B() : AbstractProblem(3,4)
{
}

/*
void Problem_B::evaluateIndividual(Individual & r) {
    //arbitaire : f0 est minimisé pour ||M(x,y) - M0||=R0.
    //arbitaire : f1 est minimisé pour ||M(x,y) - M1||=R1.
    double x,y,z,x0,y0,z0,x1,y1,z1,R0,R1,RM0,RM1;
    x=r.parameters[0] ;
    y=r.parameters[1] ;
    z=r.parameters[2] ;
    x0=0. ;
    y0=0. ;
    z0=0. ;
    R0=0.5 ;
    x1=1. ;
    y1=1. ;
    z1=1.;
    R1=0.5 ;

    RM0=qSqrt( pow(x-x0,2.) + pow(y-y0,2.) + pow(z-z0,2.) ) ;
    RM1=qSqrt( pow(x-x1,2.) + pow(y-y1,2.) + pow(z-z1,2.) ) ;

    r.objectives[0]=pow(RM0-R0,2.) ;
    r.objectives[1]=pow(RM1-R1,2.) ;

    numberOfEvaluations++ ;
}
*/


void Problem_B::evaluateIndividual(Individual & r) {
    //arbitaire : f0 est minimisé pour ||M(x,y) - M0||=R0.
    //arbitaire : f1 est minimisé pour ||M(x,y) - M1||=R1.
    ("sh script.sh");
    double x,y,z;
    x=r.parameters[0] ;
    y=r.parameters[1] ;
    z=r.parameters[2] ;

    double x0,y0,z0,R0,RM0;
    double x1,y1,z1,R1,RM1;
    double x2,y2,z2,R2,RM2;
    double x3,y3,z3,R3,RM3;

    x0=0.5 ;
    y0=0.5 ;
    z0=0.5 ;
    R0=0.2 ;

    x1=0.5 ;
    y1=0.5 ;
    z1=0.5;
    R1=0.4 ;

    x2=0. ;
    y2=0. ;
    z2=0.;
    R2=0. ;

    x3=1. ;
    y3=1. ;
    z3=1.;
    R3=0. ;

    RM0=qSqrt( pow(x-x0,2.) + pow(y-y0,2.) + pow(z-z0,2.) ) ;
    RM1=qSqrt( pow(x-x1,2.) + pow(y-y1,2.) + pow(z-z1,2.) ) ;
    RM2=qSqrt( pow(x-x2,2.) + pow(y-y2,2.) + pow(z-z2,2.) ) ;
    RM3=qSqrt( pow(x-x3,2.) + pow(y-y3,2.) + pow(z-z3,2.) ) ;

    r.objectives[0]=pow(RM0-R0,2.) ;
    r.objectives[1]=pow(RM1-R1,2.) ;
    r.objectives[2]=pow(RM2-R2,2.) ;
    r.objectives[3]=pow(RM3-R3,2.) ;

    numberOfEvaluations++ ;
}


/*
void Problem_B::evaluateIndividual(Individual & r) {
    //arbitaire : f0 est minimisé pour ||M(x,y) - M0||=R0.
    //arbitaire : f1 est minimisé pour ||M(x,y) - M1||=R1.
    double x,y,z,x0,y0,z0,x1,y1,z1,x2,y2,z2,R0,R1,R2,RM0,RM1,RM2;
    x=r.parameters[0] ;
    y=r.parameters[1] ;
    //z=r.parameters[2] ;
    x0=0. ;
    y0=0. ;
    //z0=0.5 ;
    R0=0.2 ;
    x1=0. ;
    y1=1. ;
    z1=0.;
    R1=0.2 ;
    x2=1. ;
    y2=1. ;
    //z2=0.;
    R2=0.2 ;

    RM0=qSqrt( pow(x-x0,2.) + pow(y-y0,2.)  ) ;
    RM1=qSqrt( pow(x-x1,2.) + pow(y-y1,2.)  ) ;
    RM2=qSqrt( pow(x-x2,2.) + pow(y-y2,2.)  ) ;

    r.objectives[0]=pow(RM0-R0,2.) ;
    r.objectives[1]=pow(RM1-R1,2.) ;
    r.objectives[2]=pow(RM2-R2,2.) ;

    numberOfEvaluations++ ;
}

*/
