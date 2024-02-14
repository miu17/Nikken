#include "output_planeitem.h"

#include <QPainter>

#include "calc_vector2d.h"

namespace post3dapp {

OutputPlaneItem::OutputPlaneItem( FigureTemplate* fig, DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist)
    : OutputGraphicsItem(fig, dtype,stype, uid)
{
    QPointF p0 = plist.first();
    myPointList.clear();
    Q_FOREACH (QPointF p1, plist) myPointList.append(p1 - p0);
}

void OutputPlaneItem::drawOTShapePath(QPainter *painter){

    createPlaneShapePath();

    QColor planeLineColor = itemColor(attentionStatus);
    QPen orgpen = painter->pen();
    QColor planeFillColor = itemColor(attentionStatus);

    const QPen pen2 = QPen(planeLineColor, 0, Qt::SolidLine);
    painter->setPen(pen2);
    QBrush brush=QBrush(planeFillColor, Qt::BDiagPattern);
    brush.setTransform(QTransform(painter->worldTransform().inverted()));
    painter->fillPath(drawShapePath, brush);
    painter->strokePath(drawShapePath,pen2);

    painter->setPen(orgpen);

};
void  OutputPlaneItem::createPlaneShapePath()
{

    /* 時計回り・反時計回り、図心位置の算出 */
    qreal area = 0.0;
    for ( int i = 0; i < myPointList.count(); i++ ) {
        QPointF p1 = myPointList.at(i);
        QPointF p2 = ( i == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(i + 1) ;
        area += p1.x() * p2.y() - p2.x() * p1.y();
    }
    area = area / 2.0;


    /* 描画パスの作成 */
    drawShapePath = QPainterPath();

    qreal clen = 300.0;
    QPointF q0;
    for ( int i = 0; i < myPointList.count(); i++ ) {

        QPointF p1 = ( i == 0 ) ? myPointList.last() : myPointList.at(i - 1);
        QPointF p2 = myPointList.at(i) ;
        QPointF p3 = ( i == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(i + 1) ;

        qreal th1 = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());
        qreal th2 = qAtan2(p3.y() - p2.y(), p3.x() - p2.x());

        qreal a0 = (M_PI+th1-th2)/2.0;
        qreal a1 = th2+a0;
        qreal len = clen / qSin(a0);
        QPointF q1 = p2 + QPointF(len * qCos(a1), len * qSin(a1));

        if(i==0){
            q0 = q1;
            drawShapePath.moveTo(q1);
        }else if( i == myPointList.count() - 1 ){
            drawShapePath.lineTo(q1);
            drawShapePath.lineTo(q0);
        }else{
            drawShapePath.lineTo(q1);
        }
    }
    drawShapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();
    shapePath = drawShapePath;//クリック判定

}


} // namespace post3dapp
