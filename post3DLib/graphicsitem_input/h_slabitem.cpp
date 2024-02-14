#include "h_slabitem.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <qmath.h>

#include "calc_vector2d.h"
namespace post3dapp{
SlabHPlaneItem::SlabHPlaneItem( const Input2DViewSettings& setting, const QList<QPointF> &plist,
                                const QString &str, bool onplane,qreal thickness)
    : CustomDrawHPlaneItem(setting,ELSLAB, plist, str, onplane, thickness)
{
    createShapePath();
    update(boundRect);
}

void SlabHPlaneItem::createShapePath()
{
    if ( myPointList.count() < 3 ) return;

    /* 自己交差チェック */
    bool isError = ( myThickness < 1.0e-3 ) ;
    for ( int i = 0; i < myPointList.count() - 1; i++ ) {
        if ( isError ) break;
        QLineF l1 = QLineF(myPointList.at(i), myPointList.at(i + 1));
        for ( int j = i + 1; j < myPointList.count(); j++ ) {
            QLineF l2 = ( j == myPointList.count() - 1 ) ? QLineF(myPointList.at(j), myPointList.first())
                        : QLineF(myPointList.at(j), myPointList.at(j + 1));
            if ( CalcVector2D::checkSegmentIntersection(l1, l2) ) {
                isError = true;
                break;
            }
        }
    }

    /* 時計回り・反時計回り、図心位置の算出 */
    qreal area = 0.0, cx = 0.0, cy = 0.0;
    for ( int i = 0; i < myPointList.count(); i++ ) {
        QPointF p1 = myPointList.at(i);
        QPointF p2 = ( i == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(i + 1) ;
        area += p1.x() * p2.y() - p2.x() * p1.y();
        cx += (p1.x() + p2.x()) * (p1.x() * p2.y() - p2.x() * p1.y());
        cy += (p1.y() + p2.y()) * (p1.x() * p2.y() - p2.x() * p1.y());
    }
    area = area / 2.0;

    if ( isError || qAbs(area) < 1.0e-5 ) {
        cx = 0.0;
        cy = 0.0;
        Q_FOREACH ( QPointF pp, myPointList ) {
            cx += pp.x();
            cy += pp.y();
        }
        cx = cx / qreal( myPointList.count() );
        cy = cy / qreal( myPointList.count() );
    } else {
        cx = cx / 6.0 / area;
        cy = cy / 6.0 / area;
    }

    /* 描画パスの作成 */
    drawShapePath = QPainterPath();
    drawShapePath.addPolygon( QPolygonF( myPointList.toVector() ) );
    drawShapePath.closeSubpath();

    qreal clen = 800.0;
    for ( int i = 0; i < myPointList.count(); i++ ) {

        QPointF p1 = ( i == 0 ) ? myPointList.last() : myPointList.at(i - 1);
        QPointF p2 = myPointList.at(i) ;
        QPointF p3 = ( i == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(i + 1) ;

        qreal th1 = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());
        qreal th2 = qAtan2(p3.y() - p2.y(), p3.x() - p2.x());
        qreal a1 = (th1 + th2 - M_PI) / 2.0;
        qreal a2 = a1 + M_PI;

        QPointF q1 = p2 + QPointF(clen * qCos(a1), clen * qSin(a1));

        int side = CalcVector2D::sideToSegment(p1, p2, q1);
        if ( area < 0.0 && side < 0 ) {
            q1 = p2 + QPointF(clen * qCos(a2), clen * qSin(a2));
        } else if ( area > 0.0 && side > 0 ) {
            q1 = p2 + QPointF(clen * qCos(a2), clen * qSin(a2));
        }

        drawShapePath.moveTo(p2);
        drawShapePath.lineTo(q1);
    }

    shapePath = QPainterPath();
    shapePath.addPolygon( QPolygonF( myPointList.toVector() ) );
    shapePath.closeSubpath();
    boundRect = shapePath.boundingRect();

    textPoint = QPointF(cx, cy);
    if ( sectionName.isEmpty() ) {
        myElementState = ELDUMMY;
        textName = "(dummyS)";
    } else if ( !isOnPlane || isError ) {
        myElementState = ELERROR;
        textName = "ERROR(" + sectionName + ")";
    } else {
        myElementState = ELNORMAL;
        textName = sectionName;
    }
}
} // namespace post3dapp
