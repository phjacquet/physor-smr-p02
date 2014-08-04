#ifndef OPTIMISATIONENGINE_H
#define OPTIMISATIONENGINE_H

#include <QObject>
#include <vector>
#include <map>
#include <string>
#include <QMetaType>
#include "individual.h"
#include "abstractproblem.h"

class OptimisationEngine : public QObject
{
    Q_OBJECT

public:
    OptimisationEngine();
    OptimisationEngine(const OptimisationEngine &);
    virtual ~OptimisationEngine();
    unsigned getNumberOfParameters() ;
    unsigned getNumberOfObjectives() ;
    int getSizeOfPopulation() ;
    unsigned getLastIterationNumber() ;


public Q_SLOTS:
    void iterate( int );
    void setsizeOfPopulation(int) ;
    void getGlobalNonDominatedFront( ) ;


private:
    void nextIteration() ;
    void init() ;
    void instantiateProblem();
    std::vector<Individual> nonDominatedFront( std::vector<Individual> &) ;
    std::vector<Individual> decimateFront( unsigned , std::vector<Individual> &) ;

    AbstractProblem * problem ;
    std::vector< std::map<std::string,std::vector<Individual> > > historySetOfIndidual ;
    unsigned sizeOfPopulationIni ;
    unsigned sizeOfPopulationTarget ;
Q_SIGNALS:
    void updateCurves(std::map<std::string,std::vector<Individual> >);

    friend QDataStream & operator << (QDataStream & out, const OptimisationEngine & l);
    friend QDataStream & operator >> (QDataStream & in, OptimisationEngine & l);

};

Q_DECLARE_METATYPE(OptimisationEngine)
QDataStream & operator << (QDataStream & out, const OptimisationEngine & l);
QDataStream & operator >> (QDataStream & in, OptimisationEngine & l);


#endif // OPTIMISATIONENGINE_H
