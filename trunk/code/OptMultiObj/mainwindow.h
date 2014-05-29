#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_ 1

#include <qmainwindow.h>
#include <vector>
#include <QMap>
#include <string>
#include "individual.h"

class Plot;
class QTabWidget ;
class QPushButton;
class QSpinBox ;
class OptimisationEngine ;
class QCheckBox ;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    QWidget *createComputeTab( QWidget *parent );
    QWidget *createPlotTab( QWidget *parent );
    QWidget *createFileTab( QWidget *parent ) ;
    void createPlots() ;

public Q_SLOTS:
    void getSetOfIndividual( std::map<std::string,std::vector<Individual> > );
    void iterButtonReleased(  );
    void openButtonReleased(  );
    void saveButtonReleased(  );
    void computeButtonReleased(  ) ;
    void individualSelection(Plot *, QString , QString , QRectF) ;
    void exportToPDF() ;

private:
    QMap<QString,Plot *> paramPlots;
    QMap<QString,Plot *> objPlots;
    std::map<std::string,std::vector<Individual> > currentSetOfIndiduals ;

    QWidget *plottingWidget ;
    QPushButton * d_iterButton ;
    QPushButton * d_computeButton ;
    QSpinBox    * d_iterNumberSpinBox ;
    QSpinBox    * d_computeNumberSpinBox ;
    QSpinBox    * d_NumberOfIndividuals ;
    QTabWidget * d_panel ;
    QPushButton * d_openButton ;
    QPushButton * d_saveButton ;
    QPushButton * d_pdfExportButton ;
    OptimisationEngine * d_optimisationEngine ;
};

#endif
