#include "mainwindow.h"
#include "plot.h"
#include <qmath.h>
#include <QPushButton>
#include <QTabWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include "optimisationengine.h"

MainWindow::MainWindow()
{
    QWidget *w = new QWidget( this );
    d_panel = new QTabWidget( w );
    d_optimisationEngine = new OptimisationEngine;
    createPlots();

    QHBoxLayout *hLayout = new QHBoxLayout( w );
    hLayout->addWidget( d_panel );
    hLayout->addWidget( plottingWidget, 10 );

    d_panel->setTabPosition( QTabWidget::West );
    d_panel->addTab( createComputeTab( this ), "Compute" );
    d_panel->addTab( createPlotTab( this ), "Plot" );

    setCentralWidget( w );

    connect(d_iterButton,SIGNAL(released()),this, SLOT(iterButtonReleased())) ;
    connect(d_computeButton,SIGNAL(released()),this, SLOT(computeButtonReleased())) ;
    connect(d_iterNumberSpinBox,SIGNAL(valueChanged(int)),d_optimisationEngine, SLOT(iterate(int))) ;
    connect(d_optimisationEngine,SIGNAL(updateCurves(std::map<std::string,std::vector<Individual> >)),this,SLOT(getSetOfIndividual(std::map<std::string,std::vector<Individual> >))) ;
    connect(d_NumberOfIndividuals,SIGNAL(valueChanged(int)),d_optimisationEngine, SLOT(setsizeOfPopulation(int)) ) ;

    d_optimisationEngine->iterate(0);
}

void MainWindow::iterButtonReleased(  ) {
    d_iterNumberSpinBox->setMaximum(d_iterNumberSpinBox->value()+1);
    d_iterNumberSpinBox->setValue(d_iterNumberSpinBox->value()+1);
}

void MainWindow::computeButtonReleased(  ) {
    d_iterNumberSpinBox->setMaximum(d_iterNumberSpinBox->value()+d_computeNumberSpinBox->value());
    while (d_iterNumberSpinBox->value()<d_iterNumberSpinBox->maximum())
    {
        d_iterNumberSpinBox->setValue(d_iterNumberSpinBox->value()+1);
        plottingWidget->repaint();
    }
}




QWidget *MainWindow::createComputeTab( QWidget *parent )
{
    QWidget *page = new QWidget( parent );
    d_iterButton = new QPushButton( page );
    d_iterButton->setText("Iterate");
    d_iterNumberSpinBox = new QSpinBox();
    d_iterNumberSpinBox->setMaximum(1);
    d_NumberOfIndividuals = new QSpinBox();
    d_NumberOfIndividuals->setMaximum(10000);
    d_NumberOfIndividuals->setValue(d_optimisationEngine->getSizeOfPopulation());
    d_computeButton= new QPushButton( page );
    d_computeButton->setText("Compute");
    d_computeNumberSpinBox=new QSpinBox();
    d_computeNumberSpinBox->setValue(10);

    QGridLayout *layout = new QGridLayout( page );
    layout->addWidget( new QLabel( "Current iteration", page ), 0, 0 );
    layout->addWidget( d_iterNumberSpinBox, 0, 1 );
    layout->addWidget( d_iterButton, 1, 0 );
    layout->addWidget( d_computeButton, 2, 0 );
    layout->addWidget( d_computeNumberSpinBox, 2, 1 );
    layout->addWidget( new QLabel( "iter.", page ), 2, 2 );
    layout->addWidget( new QLabel( "Number of individuals", page ), 3, 0 );
    layout->addWidget( d_NumberOfIndividuals, 3, 1 );

    layout->addLayout( new QHBoxLayout(), 4, 0 );
    layout->setColumnStretch( 4, 10 );
    layout->setRowStretch( 4, 10 );

    return page;
}

QWidget *MainWindow::createPlotTab( QWidget *parent )
{
    QWidget *page = new QWidget( parent );
    QGridLayout *layout = new QGridLayout( page );
    layout->addWidget( new QLabel( "Use to plot", page ), 0, 0 );
    layout->addLayout( new QHBoxLayout(), 1, 0 );
    layout->setColumnStretch( 1, 10 );
    layout->setRowStretch( 1, 10 );
    return page;
}

void MainWindow::createPlots() {
    qDebug()<<"void MainWindow::createPlots()";

    plottingWidget = new QWidget( this );

    QHBoxLayout *hLayout = new QHBoxLayout( plottingWidget );
    hLayout->setStretch(0,1);hLayout->setStretch(1,1);
    QGridLayout *playout = new QGridLayout();
    QGridLayout *olayout = new QGridLayout();
    hLayout->addLayout(playout);
    hLayout->addLayout(olayout);
    qDebug()<<"ok";

    for (unsigned l = 0; l < d_optimisationEngine->getNumberOfParameters()-1; l++) {
        for (unsigned c = l+1; c < d_optimisationEngine->getNumberOfParameters(); c++) {
            QString ord="X"+QString::number(l);
            QString abs="X"+QString::number(c);
            QString key=ord+":"+abs;

            Plot * d_plot = new Plot( key, plottingWidget );
            d_plot->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
            playout->setColumnStretch(c-1,1);
            playout->setRowStretch(l,1);

            d_plot->setAxisTitle(QwtPlot::xBottom,abs);
            d_plot->setAxisTitle(QwtPlot::yLeft,ord);
            paramPlots[key]=d_plot;
            playout->addWidget( d_plot, l, c-1 );
            connect(d_plot,SIGNAL(defineSelection(Plot*,QString,QString,QRectF)),this, SLOT(individualSelection(Plot*,QString,QString,QRectF)) ) ;
        }
    }

    for (unsigned l = 0; l < d_optimisationEngine->getNumberOfObjectives()-1; l++) {
        for (unsigned c = l+1; c < d_optimisationEngine->getNumberOfObjectives(); c++) {
            QString ord="F"+QString::number(l);
            QString abs="F"+QString::number(c);
            QString key=ord+":"+abs;

            Plot * d_plot = new Plot( key, plottingWidget );
            d_plot->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
            olayout->setColumnStretch(c-1,1);
            olayout->setRowStretch(l,1);

            d_plot->setAxisTitle(QwtPlot::xBottom,abs);
            d_plot->setAxisTitle(QwtPlot::yLeft,ord);
            d_plot->setAxisAutoScale(QwtPlot::xBottom) ;
            d_plot->setAxisAutoScale(QwtPlot::yLeft) ;
            objPlots[key]=d_plot;
            olayout->addWidget( d_plot, l, c-1);

            connect(d_plot,SIGNAL(defineSelection(Plot*,QString,QString,QRectF)),this, SLOT(individualSelection(Plot*,QString,QString,QRectF)) ) ;
        }
    }
}

void MainWindow::getSetOfIndividual( std::map<std::string,std::vector<Individual> > setOfIndiduals_l ) {
    qDebug()<<"MainWindow::getSetOfIndividual( )" ;
    currentSetOfIndiduals=setOfIndiduals_l ;

    for (unsigned l = 0; l < d_optimisationEngine->getNumberOfParameters()-1; l++) {
        for (unsigned c = l+1; c < d_optimisationEngine->getNumberOfParameters(); c++) {
            QString ord="X"+QString::number(l);
            QString abs="X"+QString::number(c);
            QString key=ord+":"+abs;
//            qDebug()<<key;

            QMap<QString, QPolygonF > pcurves ;
            for (std::map<std::string,std::vector<Individual> >::iterator it=setOfIndiduals_l.begin(); it!=setOfIndiduals_l.end(); ++it) {
//                qDebug()<<QString::fromStdString(it->first.c_str()) <<" "<< it->second.size();
                QPolygonF samples;
                for ( unsigned i = 0; i < it->second.size() ; i++ )
                {
                    const double Xl =  it->second[i].parameters[l] ;
                    const double Xc =  it->second[i].parameters[c] ;
                    samples += QPointF( Xc, Xl );
                }
                pcurves[QString::fromStdString ( it->first)]=samples;
            }
            paramPlots[key]->setCurves( pcurves );
        }
    }

    for (unsigned l = 0; l < d_optimisationEngine->getNumberOfObjectives()-1; l++) {
        for (unsigned c = l+1; c < d_optimisationEngine->getNumberOfObjectives(); c++) {
            QString ord="F"+QString::number(l);
            QString abs="F"+QString::number(c);
            QString key=ord+":"+abs;
            QMap<QString, QPolygonF > ocurves;
            for (std::map<std::string,std::vector<Individual> >::iterator it=setOfIndiduals_l.begin(); it!=setOfIndiduals_l.end(); ++it) {
                QPolygonF samples;
                for ( unsigned i = 0; i < it->second.size() ; i++ )
                {
                    const double Fl = it->second[i].objectives[l] ;
                    const double Fc = it->second[i].objectives[c] ;
                    samples += QPointF( Fc, Fl );
                }
                ocurves[QString::fromStdString ( it->first)]=samples;
            }
            objPlots[key]->setCurves(ocurves );
        }
    }
}

void MainWindow::individualSelection(Plot * plot, QString ordName, QString absName, QRectF rect) {
//    qDebug()<<"get signal : individualSelection(this, "<<absName<<","<<ordName<<", ["<<rect.left()<<"-"<<rect.right()<<"]x["<<rect.top()<<"-"<<rect.bottom()<<"] )";
    std::vector<Individual> set = currentSetOfIndiduals["nonDominatedFront"];
    //currentSetOfIndiduals["sel"].clear() ;
    int counter=0;

    for ( unsigned i = 0; i < set.size() ; i++ )      {
        bool keepIt=true;
        for (unsigned p = 0; p < d_optimisationEngine->getNumberOfParameters(); p++) {
            QString pName="X"+QString::number(p);
            double val=set[i].parameters[p] ;
            if (pName==absName)
                if(rect.left()>val || rect.right()<val )
                    keepIt=false;
            if (pName==ordName)
                if( rect.top()>val || rect.bottom()<val )
                    keepIt=false ;
//            qDebug()<<i<<" "<<p<<" "<<pName<<" "<<absName<<" "<<ordName<<" "<<set[i].parameters[p]<<" "<<keepIt;
        }
        for (unsigned o = 0; o < d_optimisationEngine->getNumberOfObjectives(); o++) {
            QString pName="F"+QString::number(o);
            double val=set[i].objectives[o] ;
            if (pName==absName)
                if(rect.left()>val || rect.right()<val )
                    keepIt=false;
            if (pName==ordName)
                if( rect.top()>val || rect.bottom()<val )
                    keepIt=false ;
//            qDebug()<<i<<" "<<p<<" "<<pName<<" "<<absName<<" "<<ordName<<" "<<set[i].parameters[p]<<" "<<keepIt;
        }
        if (keepIt) {currentSetOfIndiduals["sel"].push_back(set[i]) ; counter++;}
    }

    if (counter==0) currentSetOfIndiduals["sel"].clear() ;

    getSetOfIndividual( currentSetOfIndiduals ) ;
}


