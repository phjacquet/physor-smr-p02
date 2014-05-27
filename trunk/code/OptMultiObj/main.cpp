#include <qapplication.h>
#include "mainwindow.h"
#include <QDateTime>
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
