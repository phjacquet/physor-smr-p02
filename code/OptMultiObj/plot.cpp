#include "plot.h"
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_renderer.h>

Plot::Plot( QString l_name, QWidget *parent ):
    QwtPlot( parent )
{
    name=l_name;
    absName=name.split("_").at(1);
    ordName=name.split("_").at(0);

    setCanvasBackground( Qt::black );
    setAxisScale( QwtPlot::yLeft, 0.0, 10.0 );
    insertLegend( new QwtLegend(), QwtPlot::TopLegend );
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(QColor(Qt::darkGray));
    grid->attach( this );

    setAxisScale(QwtPlot::xBottom,0.,1.);
    setAxisScale(QwtPlot::yLeft,0.,1.);
    //setAxisAutoScale(QwtPlot::xBottom) ;
    //setAxisAutoScale(QwtPlot::yLeft) ;

    QwtPlotPanner *panner1 = new QwtPlotPanner( canvas() );
    panner1->setMouseButton (Qt::MiddleButton) ;
    QwtPlotPanner *panner2 = new QwtPlotPanner( canvas() );
    panner2->setMouseButton (Qt::LeftButton,Qt::ControlModifier) ;
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( canvas() );
    magnifier->setMouseButton( Qt::NoButton );
    QwtPlotPicker *  picker=new QwtPlotPicker ( canvas() );
    picker->setTrackerMode( QwtPicker::AlwaysOn );
    picker->setTrackerPen(QPen(QColor(Qt::cyan))) ;
    picker->setStateMachine( new QwtPickerDragRectMachine() );
    picker->setRubberBand( QwtPlotPicker::RectRubberBand );
    picker->setRubberBandPen(QPen(QColor(Qt::white)));

    connect(picker,SIGNAL(selected(QRectF)),this, SLOT(pickerSelection(QRectF))) ;

    sortingCurves.push_back("all");
    sortingCurves.push_back("nonDominatedFront");
    sortingCurves.push_back("nonDominatedDecFront");
    sortingCurves.push_back("sel");

    colors[sortingCurves[0]]=Qt::blue;
    colors[sortingCurves[1]]=Qt::darkRed;
    colors[sortingCurves[2]]=Qt::red;
    colors[sortingCurves[3]]=Qt::white;

    legendsName[sortingCurves[0]]="all";
    legendsName[sortingCurves[1]]="nDom";
    legendsName[sortingCurves[2]]="nDomDec";
    legendsName[sortingCurves[3]]="sel";
}
void Plot::exportToPDF() {
    QwtPlotRenderer renderer;
    renderer.renderDocument(this, name+".png", QSizeF(200, 200), 100);
    renderer.renderDocument(this, name+".pdf", QSizeF(200, 200), 100);
    qDebug()<<"export plot (name:"<<name<<") to files : "<<name+".pdf/.png";
}

void Plot::setCurves( QMap<QString, QPolygonF > & curves )
{
    for (int c=0;c<d_curves.size();c++) {
        d_curves[c]->detach();
        delete d_curves[c] ;
    }
    d_curves.resize(0);

    // attach curves
    for (int c=0;c<sortingCurves.size();c++) {
        d_curves.push_back(new QwtPlotCurve(legendsName[sortingCurves[c]])) ;
        d_curves.back()->setPen( QColor( colors[sortingCurves[c]] ) );
        d_curves.back()->attach( this );
        d_curves.back()->setSymbol(new QwtSymbol( QwtSymbol::Cross,Qt::NoBrush, d_curves.back()->pen(), QSize( 2, 2 ) ) );
        d_curves.back()->setStyle( QwtPlotCurve::NoCurve );
        d_curves.back()->setSamples( curves[sortingCurves[c]] );
    }
    replot();
}

void Plot::pickerSelection(QRectF rect) {
    //qDebug()<<"void Plot::pickerSelection(QRectF)";
    emit(defineSelection(this,ordName,absName,rect));
}


