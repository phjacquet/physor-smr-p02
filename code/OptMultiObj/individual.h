#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <QString>

class Individual
{
public:
    Individual(int paramDim_l, int objDim_l);
    std::vector<double> parameters ;
    std::vector<double> objectives ;
    int getParamDim() ;
    int getObjDim() ;
    bool isDominatedBy(Individual&) ;
    double distanceTo(Individual&) ;
    Individual extrapolate(Individual&,double) ;
    Individual mutation(double) ;
    QString toString() ;

    int NonDominationOrder ;
    double nearestIndividualDistance ;
private:
    int  paramDim ;
    int objDim ;

};

#endif // INDIVIDUAL_H
