#ifndef _PLOT_H_
#define _PLOT_H_ 1

#include <qwt_plot.h>
#include <vector>


class QwtPlotCurve;
class QwtSymbol;

class Plot : public QwtPlot
{
    Q_OBJECT

public:
    Plot( QString, QWidget *parent = NULL );
    void setCurves( QMap<QString, QPolygonF > & ) ;
    void exportToPDF( ) ;

private:
    QString name ;
    QString absName;
    QString ordName;

    QVector<QString> sortingCurves;
    QVector<QwtPlotCurve*> d_curves;
    QMap<QString,QColor> colors;
    QMap<QString,QString> legendsName;

public Q_SLOTS:
    void pickerSelection(QRectF);
    void pickerUnselection(QRectF);

Q_SIGNALS:
    void defineSelection(Plot *, QString ordName, QString absName, QRectF);
    void defineUnselection(Plot *, QString ordName, QString absName, QRectF);

};

#endif // _PLOT_H_
