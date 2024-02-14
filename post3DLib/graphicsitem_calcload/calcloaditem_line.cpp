#include "calcloaditem_line.h"

#include <QDebug>
#include <QPainter>

#include "calc_vector2d.h"
namespace post3dapp{
CalcLoadLineItem::CalcLoadLineItem(DATATYPE dtype,
                                   const QUuid &uid,
                                   const CalculationLoad2DViewSettings &setting,
                                   const QList<QPointF> &plist,
                                   bool shape_on )
    : CalcLoadGraphicsItem(dtype, uid,setting)
{
    QPointF p0 = plist.first();
    myLenList.clear();
    myPointList.clear();
    myLenList.append(0.0);
    myPointList.append(QPointF(0.0, 0.0));
    for ( int i = 1; i < plist.count(); i++ ) {
        myLenList.append( myLenList.last() + CalcVector2D::length( plist.at(i) - plist.at(i - 1) ) );
        myPointList.append( plist.at(i) - p0 );
    }

    if ( shape_on ) createLinePath();
}

void  CalcLoadLineItem::createLinePath()
{
    qreal shapeWidth = 200.0;
    drawShapePath = QPainterPath();
    shapePath = QPainterPath();

    for ( int i = 0; i < myPointList.count() - 1 ; i++ ) {

        QPointF q1, q2, q3, q4;
        QPointF p1 = myPointList.at(i);
        QPointF p2 = myPointList.at(i + 1);
        qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());

        drawShapePath.moveTo(p1);
        drawShapePath.lineTo(p2);

        q1 = p1 + QPointF(shapeWidth / 2.0 * qCos(ag + M_PI / 2.0),
                          shapeWidth / 2.0 * qSin(ag + M_PI / 2.0));
        q2 = p2 + QPointF(shapeWidth / 2.0 * qCos(ag + M_PI / 2.0),
                          shapeWidth / 2.0 * qSin(ag + M_PI / 2.0));
        q3 = p2 + QPointF(shapeWidth / 2.0 * qCos(ag - M_PI / 2.0),
                          shapeWidth / 2.0 * qSin(ag - M_PI / 2.0));
        q4 = p1 + QPointF(shapeWidth / 2.0 * qCos(ag - M_PI / 2.0),
                          shapeWidth / 2.0 * qSin(ag - M_PI / 2.0));

        QPolygonF s_polygon;
        s_polygon << q1 << q2 << q3 << q4;
        shapePath.addPolygon(s_polygon);
    }

    drawShapePath.closeSubpath();
    shapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
//    int idx_mid=0;
//    for(int i=0; i<myLenList.count()-1; i++){
//        if( myLenList.at(i)-eps < len_mid && len_mid < myLenList.at(i+1) ){ idx_mid=i; break; }
//    }
//    textAngle=qAtan2(myPointList.at(idx_mid+1).y()-myPointList.at(idx_mid).y(),
//                     myPointList.at(idx_mid+1).x()-myPointList.at(idx_mid).x())*180.0/M_PI;
//    qreal rr=(len_mid-myLenList.at(idx_mid))/(myLenList.at(idx_mid+1)-myLenList.at(idx_mid));
//    sectionTextPos=CalcVector2D::interiorPoint(myPointList.at(idx_mid),myPointList.at(idx_mid+1),rr);

//    if( textAngle >= 92.0 || textAngle <= -88.0 ) textAngle+=180.0;
//    sectionText=sectionName;
//    if( !isLineDirVisible ) return;
//    sectionText+= ( textAngle < 92.0 && textAngle > -88.0 ) ? " >" : " <" ;
}

void CalcLoadLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    setPainter(painter, option);
    painter->setBrush(Qt::NoBrush);
//    const QPen outputpen=QPen(QBrush(),100,Qt::SolidLine);
//    painter->setPen(outputpen);

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(drawShapePath);

//    if( !isSectionTextVisible ) return;

//    qreal factor=1.0/qPow(painter->transform().m11(),0.75);
//    painter->setFont(QFont("Consolas", (sectionTextSize*4+20)*factor));

//    painter->setRenderHint(QPainter::Antialiasing,true);
//    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0),true);

//    painter->translate(sectionTextPos.x(),-sectionTextPos.y());
//    painter->rotate(-textAngle);
//    painter->drawText(QRect(-200.0*factor, -50.0*factor, 400.0*factor, 50.0*factor),
//                      Qt::AlignCenter, sectionText);
//    painter->rotate( textAngle);
//    painter->translate(-sectionTextPos.x(),sectionTextPos.y());

//    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0),true);
}

void CalcLoadLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
}
} // namespace post3dapp

