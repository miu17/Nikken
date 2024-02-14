#include "output_text_item.h"

#include <QBrush>
#include <QDebug>
#include <QPaintEngine>
#include <QPainter>
#include <QStyleOption>
#include "define_npstn.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp {

OutputTextItem::OutputTextItem( FigureCommonTemplate* fig, DATATYPE dtype,QPointF point,QString str)
    : OutputGraphicsItem( fig, dtype,STRUCTTYPE::NOSTRUCTTYPE, QUuid())
{
    Q_UNUSED(point);

    text = str;
    createTextShapePath();
}

QPainterPath OutputTextItem::shape() const
{
    qDebug()<<"OutputTextItem::shape()";
    QPainterPath path;
    path.addRect(boundRect);
    return path;
}

void OutputTextItem::createTextShapePath(){

    qDebug()<<"OutputTextItem::createTextShapePath()";
    int sz = myFigureTemplate->noteSize();
    QColor color = myFigureTemplate->noteColor();

    POINT_AND_TEXT tx(QPointF(),0.0,text,color,OFFSETPOINT::OFFSET_RIGHTTOP,9999999,sz);
    OTvaluetext.append(tx);

    boundRect = QRectF(0,1000,2000,1000);

};


} // namespace post3dapp

