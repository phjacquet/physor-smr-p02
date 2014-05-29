#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <vector>
#include <QString>

class Individual
{
public:
    Individual();
    Individual(int paramDim_l, int objDim_l);

    int getParamDim() ;
    int getObjDim() ;
    bool getEvaluationStatus() ;
    void setEvaluationStatus(bool evalStat);
    bool isDominatedBy(Individual&) ;
    double distanceTo(Individual&) ;
    Individual extrapolate(Individual&,double) ;
    Individual mutation(double) ;
    QString toString() ;

    std::vector<double> parameters ;
    std::vector<double> objectives ;
    int NonDominationOrder ;
    double nearestIndividualDistance ;
private:
    int  paramDim ;
    int objDim ;
    bool hasBeenEvaluated ;

    friend QDataStream & operator << (QDataStream & out, const Individual & l);
    friend QDataStream & operator >> (QDataStream & in, Individual & l);

};

QDataStream & operator << (QDataStream & out, const Individual & l);
QDataStream & operator >> (QDataStream & in, Individual & l);

#endif // INDIVIDUAL_H
