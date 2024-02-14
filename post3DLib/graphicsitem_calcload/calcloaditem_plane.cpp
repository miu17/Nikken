#include "calcloaditem_plane.h"

#include <QDebug>
#include <QPainter>

#include "calc_vector2d.h"
#include "unified_settings.h"
namespace post3dapp{
CalcLoadPlaneItem::CalcLoadPlaneItem( DATATYPE dtype, const QUuid &uid,
                                  const CalculationLoad2DViewSettings& setting,const QList<QPointF> &plist)
    : CalcLoadGraphicsItem(dtype, uid,setting)
{
    QPointF p0 = plist.first();
    myPointList.clear();
    Q_FOREACH (QPointF p1, plist) myPointList.append(p1 - p0);
    createPlanePath();
    planeColor = setting.getPlaneColor();
}

void  CalcLoadPlaneItem::createPlanePath()
{
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

    if ( qAbs(area) < 1.0e-5 ) {
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

    qreal clen = 150.0;
    QVector<QPointF> c_points;
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
        c_points.append(q1);
    }
    drawShapePath.closeSubpath();
    drawShapePath.addPolygon( c_points );
    drawShapePath.closeSubpath();

    shapePath = QPainterPath();
    shapePath.addPolygon( QPolygonF( myPointList.toVector() ) );
    shapePath.closeSubpath();
    scaledShapePath = QPainterPath();
    QVector<QPointF> points = myPointList.toVector();
    Q_FOREACH (QPointF point, points) {
        point = point * 0.001;
    }
    scaledShapePath.addPolygon( points );
    scaledShapePath.closeSubpath();
    boundRect = shapePath.boundingRect();

//    memberTextPos=QPointF(cx,cy);
//    if( sectionName.isEmpty() ){
//        memberText="(dummy)";
//    }else{
//        memberText=sectionName;
//    }
}

void CalcLoadPlaneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    setPainter(painter, option);

    planeColor = Qt::darkCyan;
    QPen orgpen = painter->pen();
    const QPen pen = QPen(planeColor,0, Qt::DashLine);
//    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(drawShapePath);
    planeColor = QColor(100, 100, 100, 50);
    const QPen pen2 = QPen(planeColor, 0, Qt::SolidLine);
    painter->setPen(pen2);
    painter->fillPath(shapePath, QBrush(planeColor, Qt::SolidPattern));
//    painter->setTransform(QTransform(1000.0,0,0,1000.0,0,0),true);
//    painter->fillPath(scaledShapePath,QBrush(planeColor,Qt::BDiagPattern));
//    painter->setTransform(QTransform(1.0/1000.0,0,0,1.0/1000.0,0,0),true);

//    if( !isSectionTextVisible ) return;

//    qreal factor=1.0/qPow(painter->transform().m11(),0.75);
//    painter->setFont(QFont("Consolas", (memberTextSize*4+20)*factor));

//    painter->setRenderHint(QPainter::Antialiasing,true);
//    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0),true);
//    painter->translate(memberTextPos.x(),-memberTextPos.y());
//    painter->drawText(QRect(-200.0*factor, -50.0*factor, 400.0*factor, 50.0*factor), Qt::AlignCenter, memberText);
//    painter->translate(-memberTextPos.x(),memberTextPos.y());
//    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0),true);
    painter->setPen(orgpen);
}

void CalcLoadPlaneItem::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
}
} // namespace post3dapp

