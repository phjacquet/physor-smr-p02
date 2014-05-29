#include <qapplication.h>
#include <QDateTime>

#include "mainwindow.h"
#include "optimisationengine.h"

int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    qsrand(QDateTime::currentDateTime ().toTime_t ());

    MainWindow w;
    w.resize( 800, 600 );
    //w.showMaximized();
    w.show();
    return a.exec();
}
