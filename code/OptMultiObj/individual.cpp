#include "individual.h"
#include <QtMath>
#include <QDebug>

Individual::Individual()
{
    paramDim=0;
    objDim=0;
    NonDominationOrder=0;
    nearestIndividualDistance=0.;
    hasBeenEvaluated=false;
}


Individual::Individual(int paramDim_l, int objDim_l) : parameters(paramDim_l), objectives(objDim_l)
{
    paramDim=paramDim_l;
    objDim=objDim_l;
    NonDominationOrder=0;
    nearestIndividualDistance=0.;
    hasBeenEvaluated=false;
}

int Individual::getParamDim() {
    return paramDim;
}
int Individual::getObjDim() {
    return objDim;
}

bool Individual::getEvaluationStatus() {
    return hasBeenEvaluated ;
}

void Individual::setEvaluationStatus(bool evalStat) {
    hasBeenEvaluated=evalStat ;
}

bool Individual::isDominatedBy(Individual& m) {
    if (paramDim!=m.paramDim) {qDebug()<<"bool Individual::isDominatedBy(Individual& m) bad arguments"; exit(0);}
    if (getEvaluationStatus()==false) return true ;
    if (m.getEvaluationStatus()==false) return false ;
    bool mDominate=true ;
    unsigned nEquals=0 ;
    for (unsigned oId=0;oId<objectives.size() && mDominate ;oId++) {
        if (objectives[oId]<m.objectives[oId]) mDominate=false;
        if (objectives[oId]==m.objectives[oId]) nEquals++;
    }
    if (mDominate && nEquals==objectives.size()) mDominate=false ;

    return (mDominate) ;
}

double Individual::distanceTo(Individual& m) {
    if (paramDim!=m.paramDim) {qDebug()<<"double Individual::distanceTo(Individual& m) bad arguments"; exit(0);}

    double distance = 0;
    for (unsigned pId=0;pId<parameters.size() ;pId++) {
        distance+=pow(parameters[pId]-m.parameters[pId],2.) ;
    }
    return qSqrt(distance) ;
}

Individual Individual::extrapolate(Individual& m, double position) {
    if (paramDim!=m.paramDim) {qDebug()<<"double Individual::extrapolate(Individual& m, double position) bad arguments"; exit(0);}

    Individual r(paramDim,objDim);
    for (unsigned pId=0;pId<parameters.size() ;pId++) {
        r.parameters[pId]=parameters[pId]+position*(m.parameters[pId]-parameters[pId]) ;
    }
    return r ;
}

Individual Individual::mutation(double radius) {
    Individual r(paramDim,objDim);

    for (unsigned pId=0;pId<parameters.size() ;pId++) {
        double d=qrand();
        double dmax=RAND_MAX ;
        r.parameters[pId]=radius*(d/dmax-0.5)+parameters[pId];
    }
    return r ;
}

QString Individual::toString() {
    QString r;
    r+="("+QString::number(paramDim)+");";
    for (unsigned i=0;i<parameters.size();i++)
        r+=QString::number(parameters[i])+";" ;
    r+=" " ;
    r+="("+QString::number(objDim)+");";
    for (unsigned i=0;i<objectives.size();i++)
        r+=QString::number(objectives[i])+";" ;
    r+=" " ;
    r+="(NonDominationOrder);";
        r+=QString::number(NonDominationOrder)+";" ;

    return r ;
}
