#include "mainwindow.h"
#include "plot.h"
#include <qmath.h>
#include <QPushButton>
#include <QTabWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QVariant>
#include <QPrinter>
#include <QDateTime>
#include <QListWidget>
#include <qwt_plot_renderer.h>

#include "optimisationengine.h"
#include "optics.h"

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
    d_panel->addTab( createFileTab( this ), "File" );

    setCentralWidget( w );

    connect(d_iterButton,SIGNAL(released()),this, SLOT(iterButtonReleased())) ;
    connect(d_computeButton,SIGNAL(released()),this, SLOT(computeButtonReleased())) ;
    connect(d_openButton,SIGNAL(released()),this, SLOT(openButtonReleased())) ;
    connect(d_saveButton,SIGNAL(released()),this, SLOT(saveButtonReleased())) ;
    connect(d_iterNumberSpinBox,SIGNAL(valueChanged(int)),d_optimisationEngine, SLOT(iterate(int))) ;
    connect(d_optimisationEngine,SIGNAL(updateCurves(std::map<std::string,std::vector<Individual> >)),this,SLOT(getSetOfIndividualFromOptEng(std::map<std::string,std::vector<Individual> >))) ;
    connect(d_NumberOfIndividuals,SIGNAL(valueChanged(int)),d_optimisationEngine, SLOT(setsizeOfPopulation(int)) ) ;
    connect(d_pdfExportButton,SIGNAL(released()),this, SLOT(exportToPDF()) ) ;
    connect(d_txtExportButton,SIGNAL(released()),this, SLOT(exportToTXT()) ) ;
    for (unsigned i = 0; i < paramPlots.size(); i++) {
        connect(paramPlots[paramPlots.keys().at(i)],SIGNAL(defineSelection(Plot*,QString,QString,QRectF)),this, SLOT(individualSelection(Plot*,QString,QString,QRectF)) ) ;
        connect(paramPlots[paramPlots.keys().at(i)],SIGNAL(defineUnselection(Plot*,QString,QString,QRectF)),this, SLOT(individualUnselection(Plot*,QString,QString,QRectF)) ) ;
    }
    for (unsigned i = 0; i < objPlots.size(); i++) {
        connect(objPlots[objPlots.keys().at(i)],SIGNAL(defineSelection(Plot*,QString,QString,QRectF)),this, SLOT(individualSelection(Plot*,QString,QString,QRectF)) ) ;
        connect(objPlots[objPlots.keys().at(i)],SIGNAL(defineUnselection(Plot*,QString,QString,QRectF)),this, SLOT(individualUnselection(Plot*,QString,QString,QRectF)) ) ;
    }

    connect(d_IndivSetSelUpdateButton,SIGNAL(released()),this, SLOT(updateSelSet()) ) ;
    connect(d_IndivSetSelRemoveButton,SIGNAL(released()),this, SLOT(removeSelSet()) ) ;
    connect(d_IndivSetSelListWidget,SIGNAL(clicked(QModelIndex)),this, SLOT(clickSelSet()) ) ;
    connect(d_GlobalParetoComputationButton,SIGNAL(released()),d_optimisationEngine, SLOT(getGlobalNonDominatedFront()) ) ;
    connect(d_DBSCANComputationButton,SIGNAL(released()),this,SLOT(DBSCANcomputeButtonReleased())) ;

}

void MainWindow::exportToPDF() {
    QPrinter printer;
    QwtPlotRenderer renderer;

    QString fileName="results.pdf";
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFileName(fileName);
    QPainter painter;
    if (! painter.begin(&printer)) {
        qDebug()<<"failed to open "<<fileName<<" is it writable?";
        return ;
    }
    for (unsigned i = 0; i < paramPlots.size(); i++) {
        QString key=paramPlots.keys().at(i);
        paramPlots[key]->exportToPDF();
        renderer.render(paramPlots[key],&painter,QRectF(300,0,700,700));
        painter.drawText(10, 10, QDateTime::currentDateTime().toString());
        if (! printer.newPage()) {
            qWarning("failed in flushing page to disk, disk full?");
            return ;
        }
    }
    for (unsigned i = 0; i < objPlots.size(); i++) {
        QString key=objPlots.keys().at(i);
        objPlots[key]->exportToPDF();
        renderer.render(objPlots[key],&painter,QRectF(300,0,700,700));
        painter.drawText(10, 10, QDateTime::currentDateTime().toString());
        if (! printer.newPage()) {
            qWarning("failed in flushing page to disk, disk full?");
            return ;
        }
    }
    painter.drawText(10, 10, "END OF RESULTS FILE");
    painter.end();
}

void MainWindow::exportToTXT() {
    QString fileName="results.txt";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
    QTextStream out(&file);
    std::map<std::string,std::vector<Individual> >::iterator it ;
    for (it = currentSetOfIndiduals.begin();it!=currentSetOfIndiduals.end();it++) {
        std::string setName=it->first ;
        std::vector<Individual> set = it->second ;
        for ( unsigned i = 0; i < set.size() ; i++ )
            out<<QString::fromStdString(setName)<<"\t"<<set[i].toString()<<"\n";
    }
}


void MainWindow::openButtonReleased(  ) {
    qRegisterMetaTypeStreamOperators<OptimisationEngine>("OptimisationEngine");
    qMetaTypeId<OptimisationEngine>();

    QString fileName = QFileDialog::getOpenFileName(this,"Open File","..","Files (*.*)");
    qDebug()<<fileName;

    QVariant qvar ;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> qvar;

    qDebug()<<"d_optimisationEngine->deleteLater():"<<d_optimisationEngine;
    disconnect(d_iterNumberSpinBox,SIGNAL(valueChanged(int)),d_optimisationEngine, SLOT(iterate(int))) ;
    disconnect(d_optimisationEngine,SIGNAL(updateCurves(std::map<std::string,std::vector<Individual> >)),this,SLOT(getSetOfIndividualFromOptEng(std::map<std::string,std::vector<Individual> >))) ;
    disconnect(d_NumberOfIndividuals,SIGNAL(valueChanged(int)),d_optimisationEngine, SLOT(setsizeOfPopulation(int)) ) ;
    disconnect(d_GlobalParetoComputationButton,SIGNAL(released()),d_optimisationEngine, SLOT(getGlobalNonDominatedFront()) ) ;
    d_optimisationEngine->deleteLater();

    d_optimisationEngine = new OptimisationEngine(qvar.value<OptimisationEngine>());
    qDebug()<<"d_optimisationEngine = new : "<<d_optimisationEngine;
    connect(d_iterNumberSpinBox,SIGNAL(valueChanged(int)),d_optimisationEngine, SLOT(iterate(int))) ;
    connect(d_optimisationEngine,SIGNAL(updateCurves(std::map<std::string,std::vector<Individual> >)),this,SLOT(getSetOfIndividualFromOptEng(std::map<std::string,std::vector<Individual> >))) ;
    connect(d_NumberOfIndividuals,SIGNAL(valueChanged(int)),d_optimisationEngine, SLOT(setsizeOfPopulation(int)) ) ;
    connect(d_GlobalParetoComputationButton,SIGNAL(released()),d_optimisationEngine, SLOT(getGlobalNonDominatedFront()) ) ;

    d_iterNumberSpinBox->setMaximum(d_optimisationEngine->getLastIterationNumber()-1);
    d_iterNumberSpinBox->setValue(d_optimisationEngine->getLastIterationNumber()-1);
}

void MainWindow::saveButtonReleased(  ) {
    qRegisterMetaTypeStreamOperators<OptimisationEngine>("OptimisationEngine");
    qMetaTypeId<OptimisationEngine>();

    QString fileName =  QFileDialog::getSaveFileName(this, "Save File", "..", "Files (*.dat)");
    qDebug()<<fileName;
    QVariant qvar = QVariant::fromValue(* d_optimisationEngine);
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << qvar;
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
        repaint();
    }
}

QWidget *MainWindow::createComputeTab( QWidget *parent )
{
    QWidget *page = new QWidget( parent );
    d_iterButton = new QPushButton( page );
    d_iterButton->setText("Iterate");
    d_iterNumberSpinBox = new QSpinBox();
    d_iterNumberSpinBox->setMinimum(-1);
    d_iterNumberSpinBox->setMaximum(0);
    d_iterNumberSpinBox->setValue(-1);
    d_NumberOfIndividuals = new QSpinBox();
    d_NumberOfIndividuals->setMaximum(100000);
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
    d_pdfExportButton=new QPushButton("export",page);
    d_pdfExportButton->setChecked(false);
    d_txtExportButton=new QPushButton("export",page);
    d_txtExportButton->setChecked(false);
    d_IndivSetSelListWidget=new QListWidget( page );
    d_IndivSetSelListWidget->addItem("add") ;
    d_IndivSetSelListWidget->setCurrentRow(d_IndivSetSelListWidget->count()-1);
    d_IndivSetSelUpdateButton=new QPushButton("update",page);
    d_IndivSetSelRemoveButton=new QPushButton("remove",page);
    d_GlobalParetoComputationButton = new QPushButton("Compute",page);
    d_DBSCANComputationButton= new QPushButton("Compute",page);
    layout->addWidget( new QLabel( "Use to plot", page ), 0, 0 );
    layout->addWidget( new QLabel( "Export to pdf", page ), 1, 0 );
    layout->addWidget( new QLabel( "Export to txt", page ), 2, 0 );
    layout->addWidget( new QLabel( "Set Definition", page ), 3, 0 );
    layout->addWidget( new QLabel( "Global Pareto Front", page ), 4, 0 );
    layout->addWidget( new QLabel( "DBSCAN clustering analysis", page ), 5, 0 );
    layout->addWidget( d_pdfExportButton, 1, 1 );
    layout->addWidget( d_txtExportButton, 2, 1 );
    layout->addWidget( d_IndivSetSelListWidget, 3, 1 );
    layout->addLayout(new QVBoxLayout(),3,2);
    layout->itemAtPosition(3,2)->layout()->addWidget(d_IndivSetSelUpdateButton);
    layout->itemAtPosition(3,2)->layout()->addWidget(d_IndivSetSelRemoveButton);
    layout->addWidget( d_GlobalParetoComputationButton, 4, 1 );
    layout->addWidget( d_DBSCANComputationButton, 5, 1 );
    int lastline=layout->rowCount();
    layout->addLayout( new QHBoxLayout(), lastline, 0 );
    layout->setColumnStretch( lastline, 10 );
    layout->setRowStretch( lastline, 10 );
    return page;
}

QWidget *MainWindow::createFileTab( QWidget *parent )
{
    QWidget *page = new QWidget( parent );
    QGridLayout *layout = new QGridLayout( page );
    d_openButton= new QPushButton("Open", page );
    d_saveButton= new QPushButton("Save", page );
    layout->addWidget( new QLabel( "Open existing results", page ), 0, 0 );
    layout->addWidget(d_openButton , 1, 0 );
    layout->addWidget(d_saveButton , 1, 1 );
    layout->addLayout( new QHBoxLayout(), 2, 0 );
    layout->setColumnStretch( 2, 10 );
    layout->setRowStretch( 2, 10 );
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

    for (unsigned l = 0; l < d_optimisationEngine->getNumberOfParameters()-1; l++) {
        for (unsigned c = l+1; c < d_optimisationEngine->getNumberOfParameters(); c++) {
            QString ord="X"+QString::number(l);
            QString abs="X"+QString::number(c);
            QString key=ord+"_"+abs;

            Plot * d_plot = new Plot( key, plottingWidget );
            d_plot->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
            playout->setColumnStretch(c-1,1);
            playout->setRowStretch(l,1);

            d_plot->setAxisTitle(QwtPlot::xBottom,abs);
            d_plot->setAxisTitle(QwtPlot::yLeft,ord);
            paramPlots[key]=d_plot;
            playout->addWidget( d_plot, l, c-1 );
        }
    }

    for (unsigned l = 0; l < d_optimisationEngine->getNumberOfObjectives()-1; l++) {
        for (unsigned c = l+1; c < d_optimisationEngine->getNumberOfObjectives(); c++) {
            QString ord="F"+QString::number(l);
            QString abs="F"+QString::number(c);
            QString key=ord+"_"+abs;

            Plot * d_plot = new Plot( key, plottingWidget );
            d_plot->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
            olayout->setColumnStretch(c-1,1);
            olayout->setRowStretch(l,1);

            d_plot->setAxisTitle(QwtPlot::xBottom,abs);
            d_plot->setAxisTitle(QwtPlot::yLeft,ord);
            d_plot->setAxisAutoScale(QwtPlot::xBottom) ;
            d_plot->setAxisAutoScale(QwtPlot::yLeft) ;
            objPlots[key]=d_plot;
            olayout->addWidget( d_plot, l, c-1);
        }
    }
    qDebug()<<"ok";
}

void MainWindow::getSetOfIndividualFromOptEng( std::map<std::string,std::vector<Individual> > setOfIndiduals_l ) {
    qDebug()<<"MainWindow::getSetOfIndividualFromOptEng( )" ;
    qRegisterMetaTypeStreamOperators<OptimisationEngine>("OptimisationEngine");
    qMetaTypeId<OptimisationEngine>();
    QString fileName="./res.bak";
    qDebug()<<"save in file :"<<fileName;
    QVariant qvar = QVariant::fromValue(* d_optimisationEngine);
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << qvar;
    getSetOfIndividual(setOfIndiduals_l);
}

void MainWindow::getSetOfIndividual( std::map<std::string,std::vector<Individual> > setOfIndiduals_l ) {
    qDebug()<<"MainWindow::getSetOfIndividual( )" ;
    currentSetOfIndiduals=setOfIndiduals_l ;

    for (unsigned l = 0; l < d_optimisationEngine->getNumberOfParameters()-1; l++) {
        for (unsigned c = l+1; c < d_optimisationEngine->getNumberOfParameters(); c++) {
            QString ord="X"+QString::number(l);
            QString abs="X"+QString::number(c);
            QString key=ord+"_"+abs;
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
            QString key=ord+"_"+abs;
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
    repaint();
}

void MainWindow::individualSelection(Plot * plot, QString ordName, QString absName, QRectF rect) {
//    qDebug()<<"get signal : individualSelection(this, "<<absName<<","<<ordName<<", ["<<rect.left()<<"-"<<rect.right()<<"]x["<<rect.top()<<"-"<<rect.bottom()<<"] )";
    std::vector<Individual> set = currentSetOfIndiduals["nonDominatedFront"];
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


void MainWindow::individualUnselection(Plot * plot, QString ordName, QString absName, QRectF rect) {
    qDebug()<<"get signal : individualUnselection(this, "<<absName<<","<<ordName<<", ["<<rect.left()<<"-"<<rect.right()<<"]x["<<rect.top()<<"-"<<rect.bottom()<<"] )";
    std::vector<Individual> set = currentSetOfIndiduals["sel"];
    currentSetOfIndiduals["sel"].clear();
    int counter=0;
    for ( unsigned i = 0; i < set.size() ; i++ )      {
        bool unkeepIt=true;
        for (unsigned p = 0; p < d_optimisationEngine->getNumberOfParameters(); p++) {
            QString pName="X"+QString::number(p);
            double val=set[i].parameters[p] ;
            if (pName==absName)
                if(rect.left()>val || rect.right()<val )
                    unkeepIt=false;
            if (pName==ordName)
                if( rect.top()>val || rect.bottom()<val )
                    unkeepIt=false ;
//            qDebug()<<i<<" "<<p<<" "<<pName<<" "<<absName<<" "<<ordName<<" "<<set[i].parameters[p]<<" "<<unkeepIt;
        }
        for (unsigned o = 0; o < d_optimisationEngine->getNumberOfObjectives(); o++) {
            QString pName="F"+QString::number(o);
            double val=set[i].objectives[o] ;
            if (pName==absName)
                if(rect.left()>val || rect.right()<val )
                    unkeepIt=false;
            if (pName==ordName)
                if( rect.top()>val || rect.bottom()<val )
                    unkeepIt=false ;
//            qDebug()<<i<<" "<<o<<" "<<pName<<" "<<absName<<" "<<ordName<<" "<<set[i].objectives[o]<<" "<<unkeepIt;
        }
        if (unkeepIt==false) {currentSetOfIndiduals["sel"].push_back(set[i]) ; counter++;}
    }
    if (counter==0) currentSetOfIndiduals["sel"].clear() ;
    getSetOfIndividual( currentSetOfIndiduals ) ;
}

void MainWindow::updateSelSet() {
    //qDebug()<<"updateSelSet()" ;
    if (d_IndivSetSelListWidget->selectedItems().size()==0) return ;
    //qDebug()<<d_IndivSetSelListWidget->currentItem()->text() ;

    std::string setName("set") ;
    if (d_IndivSetSelListWidget->currentItem()->text()=="add") {
        d_IndivSetSelListWidget->addItem(QString::number(d_IndivSetSelListWidget->count()));
        d_IndivSetSelListWidget->setCurrentRow(d_IndivSetSelListWidget->count()-1);
    }
    setName=setName+d_IndivSetSelListWidget->currentItem()->text().toStdString() ;
    currentSetOfIndiduals[setName]=currentSetOfIndiduals["sel"];
    d_IndivSetSelListWidget->currentItem()->setForeground(QBrush(Qt::black));
}

void MainWindow::clickSelSet() {
    std::string setName("set") ;
    if (d_IndivSetSelListWidget->currentItem()->text()=="add") return ;

    setName=setName+d_IndivSetSelListWidget->currentItem()->text().toStdString() ;
    if (currentSetOfIndiduals.find(setName) != currentSetOfIndiduals.end() )
        currentSetOfIndiduals["sel"]=currentSetOfIndiduals[setName];
    getSetOfIndividual( currentSetOfIndiduals ) ;
}

void MainWindow::removeSelSet() {
    qDebug()<<"removeSelSet()";

    if (d_IndivSetSelListWidget->selectedItems().size()==0) return ;

    std::string setName("set") ;
    if (d_IndivSetSelListWidget->currentItem()->text()=="add") return ;

    setName=setName+d_IndivSetSelListWidget->currentItem()->text().toStdString() ;
    if (currentSetOfIndiduals.find(setName) != currentSetOfIndiduals.end() )
        currentSetOfIndiduals.erase(currentSetOfIndiduals.find(setName));

    d_IndivSetSelListWidget->currentItem()->setForeground(QBrush(Qt::gray));
}

void MainWindow::DBSCANcomputeButtonReleased() {
    qDebug()<<"DBSCANcomputeButtonReleased()";
    std::vector< std::vector<Individual> > clusters ;
    OPTICS opt;
    double eps = 0.1 ;
    int minPts=8;
    clusters=opt.DBSCAN(currentSetOfIndiduals["nonDominatedFront"],eps,minPts);
    int indexName =0 ;
    for (int i=0;i<clusters.size();i++) {
        qDebug()<<"pack "<<i<<" : "<<clusters[i].size();
        if (clusters[i].size()>=minPts)   {
            QString setName=QString("Cluster")+QString::number(indexName);
            currentSetOfIndiduals[setName.toStdString()]=clusters[i] ;
            qDebug()<<setName<<" : "<<clusters[i].size();
            indexName++ ;
        }
    }
    currentSetOfIndiduals.erase(currentSetOfIndiduals.find("nonDominatedFront"));
    currentSetOfIndiduals.erase(currentSetOfIndiduals.find("nonDominatedDecFront"));
    currentSetOfIndiduals.erase(currentSetOfIndiduals.find("all"));
    getSetOfIndividual( currentSetOfIndiduals ) ;
}
