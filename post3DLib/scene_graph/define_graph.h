#ifndef DEFINE_GRAPH_H
#define DEFINE_GRAPH_H

#include <QtCore>
#include <QtWidgets>
#include <QFont>
#include <QPainter>

#include "define_draw_figure.h"
#include "define_output.h"
#include "draw_figure.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class Graph; // ファイル内で循環参照　あまりよくなさそう
enum class DotType{
    None,
    Triangle,
    Rectangle,
    Circle,
    BlackTriangle,
    BlackRectangle,
    BlackCircle,
};

struct Dot {
    Dot(QPointF pt, DotType type = DotType::None, QString str = QString())
        :  point(pt), label(str), markType(type)
    {
        font = QFont("Meiryo", 8);
        symbolSize=4;
        symbolColor =Qt::black;
    }
//    QString mark; // obsolete
    QPointF point;
    QString label;
    QFont font;
    DotType markType;
    QColor symbolColor;
    int symbolSize;
    void draw(QPainter *p, const Graph &parentGraph);
    static Dot makeDot(GRAPH_SUBTYPE_PROPERTY::NMTYPE nmtype,
                            GRAPH_SUBTYPE_PROPERTY::ICJ icj,
                            QString label,
                            QUuid memberId, QList<STRESS> stress);
};

struct Series {
    Series() {}
    Series(QList<QPointF> pts, QList<Dot> dotlist, QPen pn = QPen(Qt::black), QString nm = QString())
    {
        plotPoints = pts;
        dots = dotlist;
        pen = pn;
        name = nm;
        font = QFont("Meiryo", 4);
    }
    QList<QPointF> plotPoints;
    QList<Dot> dots;
    QFont font;
    QString name;
    QPen pen;
    void draw(QPainter *p, const Graph &parentGraph);
};

class POST3D_CLASS_DLLSPEC Graph
{
public:
    Graph()
    {
        size = GRAPH_SIZE_OPTION();

        title = "title";
        labelX = "labelx";
        labelY = "labely";

        //pixcel系
//        width = 700;
//        height = 500;

        xmin = 0;
        xmax = 10000;
        ymin = 0;
        ymax = 500;

//        setMargin(50,100);
        subAxisX = calcSubAxis(xmin , xmax,false,false,1);
        subAxisY = calcSubAxis(ymin , ymax,false,false,2);

        axisvaluefont = QFont("Meiryo", 8);
        axisTitleFont = QFont("Meiryo", 8);
        titlefont = QFont("Meiryo", 12);
        rulerPen = QPen(Qt::black);
        titleVisible = true;
        axisColor=Qt::black;
    }
    Graph(const QList<Series>&, const GRAPH_SIZE_OPTION&,  const QString&, const QString&, const QString&);
    void draw(QPainter* p);
    QRectF graphArea(qreal dpi)const {return size.graphArea(dpi);}
    QRectF plotArea(qreal dpi)const {return size.plotArea(dpi);}


    //プロット値。スケールはプロット値と座標値の変換
    qreal xPos(qreal xVal,qreal dpi)const{

        QRectF plotArea = size.plotArea(dpi);
        if(xmax <= xmin){qDebug()<< "xmax<=xmin";return 0;}
        if(size.logScaleX){
            if(xmin < 0){qDebug()<<"log:: min is negative";return 0;}
            if(xVal < 0){qDebug()<<"log:: val is negative";return 0;}
            qreal logmax = log10(xmax/xmin);
            qreal logval = log10(xVal/xmin);
            return plotArea.x() + plotArea.width()*logval/logmax;

        }else{
            return plotArea.x() + plotArea.width()*(xVal-xmin)/(xmax-xmin);
        }
    }
    qreal yPos(qreal yVal,qreal dpi)const{
        QRectF plotArea = size.plotArea(dpi);
        if(ymax <= ymin){qDebug()<< "ymax<=ymin";return 0;}
        if(size.logScaleY){
            if(ymin < 0){qDebug()<<"log:: min is negative";return 0;}
            if(yVal < 0){qDebug()<<"log:: val is negative";return 0;}
            qreal logmax = log10(ymax/ymin);
            qreal logval = log10(yVal/ymin);
            return plotArea.y() +plotArea.height()- plotArea.height()*logval/logmax;

        }else{
        return plotArea.y()+plotArea.height() - plotArea.height()*(yVal-ymin)/(ymax-ymin);
        }
    }

    void setTitleFont(const QFont& font){titlefont = font;}
    void setTitleVisible(bool titlevisible){titleVisible = titlevisible;}
    void setAxisTextFont(const QFont& font){axisTitleFont = font;}
    void setAxisValueFont(const QFont& font){axisvaluefont = font;}
    void setAxisColor(const QColor& color){axisColor = color;}
    void setRulerPen(const QPen& pen){rulerPen=pen;}

private:
    //複数の系列を保持する
    QList<Series> series;
    QList<qreal>  calcSubAxis(qreal ymin, qreal ymax, bool strictBorderMin=false, bool strictBorderMax=false, int direction=0);
    QList<qreal> subAxisX;
    QList<qreal> subAxisY;

    //pixcel系
//    QMarginsF margin;

    //pixcel系。座標値
    GRAPH_SIZE_OPTION size;

    //プロット値。スケールはプロット値と座標値の変換
    qreal xmin;//x軸の最小
    qreal xmax;
    qreal ymin;//y軸の最小
    qreal ymax;
    QFont titlefont;
    QFont axisTitleFont;
    QFont axisvaluefont;
    QPen rulerPen;
    bool titleVisible;
    QColor axisColor;

    QString title;
    QString labelX;
    QString labelY;


};


} // namespace post3dapp

#endif // DEFINE_GRAPH_H
