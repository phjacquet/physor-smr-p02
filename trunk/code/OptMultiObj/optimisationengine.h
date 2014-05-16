#ifndef OPTIMISATIONENGINE_H
#define OPTIMISATIONENGINE_H

#include <QObject>
#include <vector>
#include <map>
#include <string>
#include "individual.h"

class AbstractProblem ;
#include "abstractproblem.h"

class OptimisationEngine : public QObject
{
    Q_OBJECT

public:
    OptimisationEngine();
    virtual ~OptimisationEngine();
    unsigned getNumberOfParameters() ;
    unsigned getNumberOfObjectives() ;
    int getSizeOfPopulation() ;

public Q_SLOTS:
    void iterate( int );
    void setsizeOfPopulation(int) ;

private:
    void nextIteration() ;
    void init() ;
    std::vector<Individual> nonDominatedFront( std::vector<Individual> &) ;
    std::vector<Individual> decimateFront( unsigned , std::vector<Individual> &) ;

    AbstractProblem * problem ;
    std::vector< std::map<std::string,std::vector<Individual> > > historySetOfIndidual ;
    unsigned sizeOfPopulationIni ;
    unsigned sizeOfPopulationTarget ;
Q_SIGNALS:
    void updateCurves(std::map<std::string,std::vector<Individual> >);
};

#endif // OPTIMISATIONENGINE_H
