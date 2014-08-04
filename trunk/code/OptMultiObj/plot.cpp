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
#include <QGuiApplication>

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
    QwtPlotPicker *  pickerSel=new QwtPlotPicker ( canvas() );
    pickerSel->setTrackerMode( QwtPicker::AlwaysOn );
    pickerSel->setTrackerPen(QPen(QColor(Qt::cyan))) ;
    pickerSel->setStateMachine( new QwtPickerDragRectMachine() );
    pickerSel->setRubberBand( QwtPlotPicker::RectRubberBand );
    pickerSel->setRubberBandPen(QPen(QColor(Qt::white)));

    QwtPlotPicker *  pickerUnsel=new QwtPlotPicker ( canvas() );
   // pickerUnsel->setMouseTracking(false);
    pickerUnsel->setMousePattern(QwtEventPattern::MouseSelect1,Qt::RightButton);
    pickerUnsel->setStateMachine( new QwtPickerDragRectMachine() );
    pickerUnsel->setRubberBand( QwtPlotPicker::RectRubberBand );
    pickerUnsel->setRubberBandPen(QPen(QColor(Qt::blue)));


    connect(pickerSel,SIGNAL(selected(QRectF)),this, SLOT(pickerSelection(QRectF))) ;
    connect(pickerUnsel,SIGNAL(selected(QRectF)),this, SLOT(pickerUnselection(QRectF))) ;

    sortingCurves.push_back("all");
    sortingCurves.push_back("nonDominatedFront");
    sortingCurves.push_back("nonDominatedDecFront");

    QColor colorsPalette[]={
        QColor(0,0,255),
        QColor(128,0,0),
        QColor(255,0,0),
        QColor(255,0,255),
        QColor(0,255,255),
        QColor(255,215,0),
        QColor(0,255,0),
        QColor(255,255,0),
        QColor(255,105,180),
        QColor(255,165,0),
        QColor(0,0,0),
        QColor(0,0,0),
        QColor(0,0,0),
        QColor(0,0,0),
        QColor(0,0,0),
    };

    colors[sortingCurves[0]]=colorsPalette[0];
    colors[sortingCurves[1]]=colorsPalette[1];
    colors[sortingCurves[2]]=colorsPalette[2];

    legendsName[sortingCurves[0]]="all";
    legendsName[sortingCurves[1]]="nDom";
    legendsName[sortingCurves[2]]="nDomDec";

    for (int i=0;i<10;i++) {
        sortingCurves.push_back(QString("Cluster")+QString::number(i));
        colors[sortingCurves.back()]=colorsPalette[i+3];
        legendsName[sortingCurves.back()]=QString("C")+QString::number(i);
    }

    sortingCurves.push_back("sel");
    colors[sortingCurves.back()]=QColor(Qt::white);
    legendsName[sortingCurves.back()]="sel";

}
void Plot::exportToPDF() {
    int symbolSizeScreen=2;
    int symbolSizePDF=10;

    for (int c=0;c<sortingCurves.size() && c<d_curves.size();c++)
        d_curves[c]->setSymbol(new QwtSymbol( QwtSymbol::Cross,Qt::NoBrush, d_curves[c]->pen(), QSize( symbolSizePDF,symbolSizePDF) ) );
    replot();

    QwtPlotRenderer renderer;
    renderer.renderDocument(this, name+".png", QSizeF(200, 200), 100);
    renderer.renderDocument(this, name+".pdf", QSizeF(200, 200), 100);
    qDebug()<<"export plot (name:"<<name<<") to files : "<<name+".pdf/.png";

    for (int c=0;c<sortingCurves.size() && c<d_curves.size();c++)
        d_curves[c]->setSymbol(new QwtSymbol( QwtSymbol::Cross,Qt::NoBrush, d_curves[c]->pen(), QSize( symbolSizeScreen,symbolSizeScreen) ) );
    replot();

}

void Plot::setCurves( QMap<QString, QPolygonF > & curves ) {
    for (int c=0;c<d_curves.size();c++) {
        d_curves[c]->detach();
        delete d_curves[c] ;
    }
    d_curves.resize(0);

    // attach curves
    for (int c=0;c<sortingCurves.size();c++) {
        if (curves.find(sortingCurves[c])!=curves.end()) {
            d_curves.push_back(new QwtPlotCurve(legendsName[sortingCurves[c]])) ;
            d_curves.back()->setPen( colors[sortingCurves[c]] );
            d_curves.back()->attach( this );
            d_curves.back()->setSymbol(new QwtSymbol( QwtSymbol::Cross,Qt::NoBrush, d_curves.back()->pen(), QSize( 2, 2 ) ) );
            d_curves.back()->setStyle( QwtPlotCurve::NoCurve );
            d_curves.back()->setSamples( curves[sortingCurves[c]] );
//qDebug()<<sortingCurves[c]<<" "<<this->name<<" "<<legendsName[sortingCurves[c]];
//qDebug()<<colors[sortingCurves[c]].red()<<" "<<colors[sortingCurves[c]].green()<<" "<<colors[sortingCurves[c]].blue();
        }
    }
    replot();
}

void Plot::pickerSelection(QRectF rect) {
    //qDebug()<<"void Plot::pickerSelection(QRectF)";
        emit(defineSelection(this,ordName,absName,rect));
        qDebug()<<"select";
}


void Plot::pickerUnselection(QRectF rect) {
        emit(defineUnselection(this,ordName,absName,rect));
        qDebug()<<"unselect";
}
