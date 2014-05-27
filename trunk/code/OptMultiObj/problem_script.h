#ifndef PROBLEM_SCRIPT_H
#define PROBLEM_SCRIPT_H

#include "abstractproblem.h"

class Problem_ShScriptEval : public AbstractProblem
{
public:
    Problem_ShScriptEval(QString jddFilename);
    void evaluateIndividual(Individual & r)  ;

    QString jddFilename ;
    QString scriptFilename ;

};

#endif // PROBLEM_SCRIPT_H
