#include "optimisationengine.h"
#include <QDebug>
#include <QtMath>
#include <QTimer>
#include <QCoreApplication>

#include "problem_a.h"
#include "problem_b.h"
#include "problem_script.h"

void OptimisationEngine::instantiateProblem() {
    sizeOfPopulationIni=100 ;
    sizeOfPopulationTarget=100;
    //problem = new Problem_ShScriptEval("/home/physor/projects/physor-smr-p02/problem1_TcycleOptim/data") ;
    problem = new Problem_B() ;
}

OptimisationEngine::OptimisationEngine() {
    instantiateProblem();
}

OptimisationEngine::OptimisationEngine(const OptimisationEngine & l) {
    qDebug()<<"OptimisationEngine::OptimisationEngine(const OptimisationEngine & l)";
    instantiateProblem();
    historySetOfIndidual = l.historySetOfIndidual;
    sizeOfPopulationIni = l.sizeOfPopulationIni;
    sizeOfPopulationTarget = l.sizeOfPopulationTarget;
    qDebug()<<"historySetOfIndidual.size():"<<historySetOfIndidual.size();
}


OptimisationEngine::~OptimisationEngine()
{
    delete problem  ;
}

unsigned OptimisationEngine::getLastIterationNumber() {
    return historySetOfIndidual.size();
}


unsigned OptimisationEngine::getNumberOfParameters() {
    return problem->numberOfParameters ;
}

unsigned OptimisationEngine::getNumberOfObjectives() {
    return problem->numberOfObjectives ;
}

int OptimisationEngine::getSizeOfPopulation() {
    return sizeOfPopulationTarget ;
}

void OptimisationEngine::setsizeOfPopulation(int size)  {
    sizeOfPopulationTarget=size;
}

void OptimisationEngine::iterate( int itNumber_l ) {

    qDebug()<<"OptimisationEngine::iterate( int itNumber_l ) | itNumber_l= "<<itNumber_l;

    if (itNumber_l==0 && historySetOfIndidual.size()==0) { init() ;}
    else if (itNumber_l==historySetOfIndidual.size()) nextIteration();
    else if (itNumber_l>historySetOfIndidual.size()) {
        qDebug()<<"Error : shall compute "<<historySetOfIndidual.size()<<" first";
        qDebug()<<this;
        return;
    }
    std::map< std::string,std::vector<Individual> > &rs=historySetOfIndidual[itNumber_l];
    emit(updateCurves(rs));
    //for (unsigned i=0;i<rs["all"].size();i++) qDebug()<<"i["<<i<<"] : "<<rs["all"][i].toString();
    qDebug()<<"Current number of iterations performed "<<problem->getNumberOfIterations();

}

std::vector<Individual> OptimisationEngine::nonDominatedFront( std::vector<Individual> & setOfIndividuals ) {
    // non domination order definition
    qDebug()<<"Select non dominated individuals upon "<<setOfIndividuals.size()<<" mutated individuals";
    std::vector<Individual> rs;

    for (unsigned i=0;i<setOfIndividuals.size();i++) {
        Individual &ri = setOfIndividuals[i] ;
        ri.NonDominationOrder=0 ;
        for (unsigned j=0;( j<setOfIndividuals.size() ) && (ri.NonDominationOrder==0);j++) {
            Individual &rj = setOfIndividuals[j] ;
            if (i!=j) {
                //qDebug()<<i<<" = "<<ri.toString();
                //qDebug()<<j<<" = "<<rj.toString();
                if (ri.isDominatedBy(rj)) ri.NonDominationOrder=ri.NonDominationOrder+1 ;
            }
            //qDebug()<<i<<":"<<ri.NonDominationOrder;
        }
        if (ri.NonDominationOrder==0) rs.push_back(ri);
    }

    return rs ;
}

std::vector<Individual> OptimisationEngine::decimateFront(unsigned size, std::vector<Individual> & nonDominatedIndividuals ) {

    std::vector<Individual> rs;
    unsigned nonDominatedIndividualsNumber=nonDominatedIndividuals.size();
    qDebug()<<"New generation by decimation upon "<<nonDominatedIndividualsNumber<<" non dominated individuals";

    for (unsigned i=0;i<nonDominatedIndividualsNumber;i++) {
        unsigned nearestIndividualId = 0 ;
        double nearestIndividualDistance = 1000 ;
        Individual &ri = nonDominatedIndividuals[i] ;
        for (unsigned j=0;j<nonDominatedIndividualsNumber ; j++) {
            if (j!=i) {
                Individual &rj = nonDominatedIndividuals[j] ;
                double distance = ri.distanceTo(rj)  ;
                if (distance<nearestIndividualDistance) {
                    nearestIndividualDistance=distance;
                    nearestIndividualId=j ;
                }
            }
        }
        nonDominatedIndividuals[i].nearestIndividualDistance=nearestIndividualDistance;
//        qDebug()<<i<<" : distance "<<nearestIndividualDistance<<" from "<<nearestIndividualId ;
        }

    if (nonDominatedIndividuals.size()<=size) return nonDominatedIndividuals;
    QVector<double> distances;
    double cutDistance = 0 ;
    for (unsigned i=0;i<nonDominatedIndividuals.size();i++) {
        Individual &ri = nonDominatedIndividuals[i] ;
        distances.push_back(ri.nearestIndividualDistance) ;
    }

    qStableSort(distances.begin(),distances.end());
    cutDistance=distances[nonDominatedIndividuals.size()-size] ;

    for (unsigned i=0;i<nonDominatedIndividuals.size() && rs.size()<size ;i++) {
        Individual &ri = nonDominatedIndividuals[i] ;
        if (ri.nearestIndividualDistance >= cutDistance )
            rs.push_back(ri);
    }
    return rs ;
}

void OptimisationEngine::init(  ) {
    std::vector<Individual> rs;
    for ( unsigned i = 0; i < sizeOfPopulationIni ; i++ ) {
        rs.push_back(problem->generateIndividual());
        problem->evaluateIndividual(rs.back()) ;
    }

    std::map<std::string,std::vector<Individual> > rsm ;
    rsm["nonDominatedFront"]=nonDominatedFront(rs);
    rsm["nonDominatedDecFront"]=decimateFront(sizeOfPopulationTarget,rsm["nonDominatedFront"]);
    rsm["all"]=rs;
    historySetOfIndidual.push_back(rsm);
}

void OptimisationEngine::nextIteration(  ) {
    std::vector<Individual> rsNew = historySetOfIndidual.back()["nonDominatedDecFront"] ;

    // mutation
    unsigned nonDominatedQSelectedIndividualsNumber=rsNew.size();
    qDebug()<<"New generation by mutation upon "<<nonDominatedQSelectedIndividualsNumber<<" non dominated individuals";
    double multiplicity=double(sizeOfPopulationTarget)/double(rsNew.size()) ;
    int i_multiplicity=int(multiplicity) ;
    qDebug()<<"multiplicity = "<< multiplicity<<" - > "<<i_multiplicity;
    for (unsigned i=0;i<nonDominatedQSelectedIndividualsNumber;i++) {
            for (unsigned k=0;k<i_multiplicity || k<1; k++) {
                //double radius = rsNew[i].nearestIndividualDistance*2 ;
                //if (historySetOfIndidual.size()<10) radius=0.1 ;
                double radius = 0.2*qrand()/double(RAND_MAX) ;

                Individual m=rsNew[i].mutation(radius) ;
                rsNew.push_back(m) ;
            }
    }

    std::map<std::string,std::vector<Individual> > rsm ;

    // individual evaluation
    QTimer *timer = new QTimer(this);
    timer->start(60000);
    for ( unsigned i = 0; i < rsNew.size() ; i++ ) {
        problem->evaluateIndividual(rsNew[i]) ;
        QCoreApplication::processEvents () ;
        if (timer->remainingTime() == 0) {
            rsm["nonDominatedDecFront"]=nonDominatedFront(rsNew);
            rsm["all"]=rsNew;
            emit(updateCurves(rsm));
            timer->start(60000);
        }
    }

    rsm["nonDominatedFront"]=nonDominatedFront(rsNew);
    rsm["nonDominatedDecFront"]=decimateFront(sizeOfPopulationTarget,rsm["nonDominatedFront"]);
    rsm["all"]=rsNew;


    //keep N last individuals
    //rsm["nonDominatedFront"].insert(rsm["nonDominatedFront"].end(),historySetOfIndidual.back()["nonDominatedFront"].begin(),historySetOfIndidual.back()["nonDominatedFront"].end());
    //unsigned newHistSize=5*sizeOfPopulationTarget ;
    //if (rsm["nonDominatedFront"].size()>newHistSize) rsm["nonDominatedFront"].resize(newHistSize) ;

    historySetOfIndidual.push_back(rsm);
}



QDataStream & operator << (QDataStream & out, const OptimisationEngine & l) {
    out <<l.sizeOfPopulationIni<<l.sizeOfPopulationTarget;
    out<<l.historySetOfIndidual.size();
    for (unsigned i=0;i<l.historySetOfIndidual.size();i++) {
        const std::map<std::string,std::vector<Individual> > & m=l.historySetOfIndidual[i] ;
        std::map<std::string,std::vector<Individual> >::const_iterator it;
        out<<m.size();
        for (it=m.begin(); it != m.end(); ++it) {
            out<<QString::fromStdString(it->first);
            out<<it->second.size();
            for (unsigned k=0;k<it->second.size();k++) {
                out<<it->second[k];
            }
        }
    }
    return out ;
}

QDataStream & operator >> (QDataStream & in, OptimisationEngine & l){
    in>>l.sizeOfPopulationIni>>l.sizeOfPopulationTarget;
    unsigned hsize;
    in>>hsize;
    l.historySetOfIndidual.resize(hsize) ;
    qDebug()<<l.sizeOfPopulationIni;
    qDebug()<<l.sizeOfPopulationTarget;
    qDebug()<<hsize;

    for (unsigned i=0;i<l.historySetOfIndidual.size();i++) {
        std::map<std::string,std::vector<Individual> > & m=l.historySetOfIndidual[i] ;
        unsigned msize;
        in>>msize;
        qDebug()<<"i:"<<i;
        qDebug()<<"msize:"<<msize;
        for (unsigned j=0; j<msize ; j++) {
            QString mkey ;
            unsigned mvalSize ;
            in>>mkey ;
            in>>mvalSize ;
            qDebug()<<"j:"<<j;
            qDebug()<<"mkey:"<<mkey;
            qDebug()<<"mvalSize:"<<mvalSize;

            m[mkey.toStdString()].resize(mvalSize) ;
            for (unsigned k=0;k<mvalSize;k++) {
                in>>m[mkey.toStdString()][k];
            }
        }
    }
    return in ;
}

