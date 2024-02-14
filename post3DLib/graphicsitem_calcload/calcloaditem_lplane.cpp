#include "calcloaditem_lplane.h"

#include <QDebug>
#include <QPainter>

#include "calc_vector2d.h"
#include "unified_settings.h"

namespace post3dapp{
CalcLoadLinePlaneItem::CalcLoadLinePlaneItem( DATATYPE dtype, const QUuid &uid,
                                          const CalculationLoad2DViewSettings &setting,const QList<QPointF> &plist )
    : CalcLoadLineItem( dtype, uid,setting, plist, false )
{
    createLinePath();
    planeColor = setting.getPlaneColor();
}

void  CalcLoadLinePlaneItem::createLinePath()
{
//    qreal eps = 1.0e-6;
//    qreal len_mid = ( myLenList.first() + myLenList.last() ) / 2.0;

    qreal shapeWidth = 200.0;
    drawShapePath = QPainterPath();
    shapePath = QPainterPath();

    for ( int i = 0; i < myPointList.count() - 1 ; i++ ) {

        QPointF q1, q2, q3, q4;
        QPointF p1 = myPointList.at(i);
        QPointF p2 = myPointList.at(i + 1);
        qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());

//        drawShapePath.moveTo(p1);
//        drawShapePath.lineTo(p2);
        if ( i == 0 ) {
            p1 = CalcVector2D::extendPoint(p2, p1, -150.0);
        }
        if ( i == myPointList.count() - 2 ) {
            p2 = CalcVector2D::extendPoint(p1, p2, -150.0);
        }

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

        drawShapePath.addPolygon(s_polygon);
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

}

void CalcLoadLinePlaneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *)
{
    setPainter(painter, option);

    QPen pen(planeColor);
    pen.setWidth(0);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(drawShapePath);

//    if( !isSectionTextVisible ) return;

//    qreal factor=1.0/qPow(painter->transform().m11(),0.75);
//    painter->setFont(QFont("Consolas", (sectionTextSize*4+20)*factor));

//    painter->setRenderHint(QPainter::Antialiasing,true);
//    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0),true);

//    painter->translate(sectionTextPos.x(),-sectionTextPos.y());
//    painter->rotate(-textAngle);
//    painter->drawText(QRect(-200.0*factor, 100.0, 400.0*factor, 50.0*factor),
//                      Qt::AlignCenter, sectionText);
//    painter->rotate( textAngle);
//    painter->translate(-sectionTextPos.x(),sectionTextPos.y());

//    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0),true);

}
} // namespace post3dapp
