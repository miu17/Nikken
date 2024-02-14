#include "output_point_item.h"

#include <QBrush>
#include <QDebug>
#include <QPaintEngine>
#include <QPainter>
#include <QStyleOption>
#include "define_npstn.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp {

OutputPointItem::OutputPointItem(FigureTemplate* fig, DATATYPE dtype, QPointF pt, QString str, QColor _color, int _size)
    : OutputGraphicsItem( fig, dtype,STRUCTTYPE::NOSTRUCTTYPE, QUuid())
{
    text = str;
    point = pt;
    color = _color;
    size = _size;
    createPointShapePath();
}

QPainterPath OutputPointItem::shape() const
{
    qDebug()<<"OutputPointItem::shape()";
    QPainterPath path;
    path.addRect(boundRect);
    return path;
}

void OutputPointItem::createPointShapePath(){

    int sz = 50 + (size-3)*10;
        QPainterPath path;
        path.addEllipse(-2*sz, -2*sz, 4*sz, 4*sz);
        //十字
        path.moveTo(-5*sz,-5*sz);
        path.lineTo(5*sz,5*sz);
        path.moveTo(5*sz,-5*sz);
        path.lineTo(-5*sz,5*sz);

        //矢印
        QPointF p1=0.1*point+QPointF(-0.05*point.y(),0.05*point.x());
        QPointF p2=0.1*point-QPointF(-0.05*point.y(),0.05*point.x());
        path.moveTo(0,0);
        path.lineTo(p1);
        path.lineTo(p2);
        path.lineTo(0,0);
        path.lineTo(point);

        OTpathes.append(PATH_AND_PEN(path,QPen(color,0)));
//        drawShapePath = path;
        boundRect = QRectF(-5*sz, -5*sz, 10*sz, 10*sz);
        POINT_AND_TEXT txt(QPoint(point.x(),-point.y())+QPointF(0,0),0.0,text,color,OFFSETPOINT::OFFSET_RIGHTMIDDLE,999999,size);
        OTvaluetext.append(txt);
        qDebug()<<"createPointShapePath";

};


} // namespace post3dapp

