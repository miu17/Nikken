#include "output_lineplaneitem.h"

#include <QPainter>

#include "calc_vector2d.h"

namespace post3dapp {

OutputLinePlaneItem::OutputLinePlaneItem( FigureTemplate* fig, DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid,
                                          const QList<QPointF> &plist )
    : OutputLineItem( fig, dtype, stype,uid, plist, false )
{
    createLinePlanePath();
}

void  OutputLinePlaneItem::createLinePlanePath()
{
//    qreal eps = 1.0e-6;
//    qreal len_mid = ( myLenList.first() + myLenList.last() ) / 2.0;

    qreal shapeWidth = 200.0;
    QPainterPath linePlanePath;

    for ( int i = 0; i < myPointList.count() - 1 ; i++ ) {

        QPointF q1, q2, q3, q4;
        QPointF p1 = myPointList.at(i);
        QPointF p2 = myPointList.at(i + 1);
        qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());

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

        linePlanePath.addPolygon(s_polygon);
    }


    PATH_AND_PEN fc;
    fc.brush = QBrush(Qt::NoBrush);
    fc.path = linePlanePath;
    fc.pen = QPen(normalColor,0,Qt::DashLine);
    OTpathes.append(fc);

}


} // namespace post3dapp
