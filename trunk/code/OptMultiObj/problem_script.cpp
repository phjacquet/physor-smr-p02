#include "problem_script.h"
#include <QMap>
#include <QTextStream>
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QFile>


Problem_ShScriptEval::Problem_ShScriptEval(QString f) : AbstractProblem(0,0)  {
    jddFilename=f;
    QFile file(jddFilename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return ;
    }

    QTextStream ts(&file);
    QMap< QString , QString> map ;
    while (!ts.atEnd()) {
        QStringList s=ts.readLine().split("=");
        map[s.front()]=s.back() ;
    }
    numberOfParameters=map["numberOfParameters"].toInt() ;
    numberOfObjectives=map["numberOfObjectives"].toInt() ;
    scriptFilename=map["scriptFilename"];
    qDebug()<<scriptFilename;;
}


void Problem_ShScriptEval::evaluateIndividual(Individual & r) {
    if (r.getEvaluationStatus()) return;

    bool evaluationOk = true ;

    QString scriptArg="" ;
    for (unsigned i=0;i<r.parameters.size();i++)
            scriptArg.append(QString::number(r.parameters[i])+" ") ;


    QProcess process;
    process.start(scriptFilename+" "+scriptArg);
    process.waitForFinished(-1);
    QString stdout = process.readAllStandardOutput();
    QString stderr = process.readAllStandardError();

    QStringList s=stdout.split(QRegExp(";"));

    if (stderr!="" || s.size() != r.objectives.size()) {
        qDebug()<<r.toString()<<"\n"<<"stderr="<<stderr ;
        for (unsigned i=0;i<r.objectives.size() && i<s.size(); i++) {
            r.objectives[i]=0. ;
            }
        evaluationOk=false ;
    }
    else {
        for (unsigned i=0;i<r.objectives.size() && i<s.size() && evaluationOk; i++) {
            bool ok=true;
            r.objectives[i]=s.at(i).toDouble(&ok) ;
            if (ok==false) {
                qDebug()<<r.toString()<<"\n"<<"Bad value : r.objectives["<<i<<"]="<<s.at(i)<<stderr ;
                evaluationOk=false ;
            }
    //        qDebug()<<r.objectives[i] ;
        }
    }

    r.setEvaluationStatus(evaluationOk);
    numberOfEvaluations++;
}
