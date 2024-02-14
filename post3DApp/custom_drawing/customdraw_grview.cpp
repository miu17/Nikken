#include "customdraw_grview.h"

#include <QDebug>
#include <QWheelEvent>
#include <QGraphicsItem>
#include "qmath.h"

#include "calc_simple.h"
#include "define_unifieddata.h"

namespace post3dapp{
CustomDrawGrView::CustomDrawGrView(UnifiedDataType isout, QWidget *parent)
    : QGraphicsView( parent ), isOutput(isout)
{
    setViewportUpdateMode(FullViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);
    setDragMode(RubberBandDrag);
    setRubberBandSelectionMode(Qt::ContainsItemShape);

    mousePressed = false;
    setCursor(Qt::OpenHandCursor);

    scale(qreal(12), qreal(12));
    setMinimumSize(500, 500);
    setAttribute(Qt::WA_DeleteOnClose);
    setTransform(QTransform(0.02, 0, 0, -0.02, 0, 0));
}

void CustomDrawGrView::drawForeground( QPainter *painter, const QRectF & )
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setTransform(QTransform(1, 0, 0, 1, 0, 0), false);
    painter->setPen(QColor(150, 150, 150));
    painter->setBrush(QColor(220, 220, 220));
    painter->setFont(QFont("Helvetica [Cronyx]", 9));

    int scaleSize = 20;

    painter->drawRect(QRect(0, 0, viewport()->width(), scaleSize));
    painter->drawRect(QRect(0, 0, scaleSize, viewport()->height()));

    QPointF mp1 = mapToScene(QPoint(0, 0));
    QPointF mp2 = mapToScene(QPoint(viewport()->width(), viewport()->height()));

    qreal tick = CalcSimple::tic(mp2.x(), mp1.x());
    // X-tick
    int ns = (mp1.x() + 0.0001) / tick - 1;
    int ne = (mp2.x() + 0.0001) / tick;
    for (int i = ns; i <= ne; ++i) {
        QPoint p1 = mapFromScene(tick * qreal(i), 0);
        QPoint p2 = mapFromScene(tick * qreal(i + 1), 0);
        int x1 = p1.x();
        qreal dx = qreal(p2.x() - x1) / 10.;
        painter->drawLine(QPoint(x1, 0), QPoint(x1, scaleSize));
        QString str = QString("%1m").arg( tick * qreal(i) / 1000., 0, 'f', 2);
        painter->drawText( QPoint(x1 + 2, scaleSize - 6), str );
        for (int j = 1; j < 10 ; ++j) {
            painter->drawLine(QPoint(x1 + dx * qreal(j), scaleSize - 3), QPoint(x1 + dx * qreal(j), scaleSize));
        }
    }
    // Y-tick
    ns = (mp2.y() + 0.0001) / tick - 1;
    ne = (mp1.y() + 0.0001) / tick;
    for (int i = ns; i <= ne; ++i) {
        QPoint p1 = mapFromScene(0, tick * qreal(i));
        QPoint p2 = mapFromScene(0, tick * qreal(i + 1));
        int y1 = p1.y();
        qreal dy = qreal(p2.y() - y1) / 10.;
        painter->drawLine(QPoint(0, y1), QPoint(scaleSize, y1));
        QString str = QString("%1m").arg( tick * qreal(i) / 1000., 0, 'f', 2 );
        painter->rotate(-90.0);
        painter->drawText(QPoint(-y1 + 2, scaleSize - 6), str);
        painter->rotate(90.0);
        for (int j = 1; j < 10 ; ++j) {
            painter->drawLine(QPoint(scaleSize - 3, y1 + dy * qreal(j)), QPoint(scaleSize, y1 + dy * qreal(j)));
        }
    }

    painter->setBrush(QColor(225, 225, 225));
    painter->drawRect(QRect(0, 0, scaleSize, scaleSize));
}

void CustomDrawGrView::drawBackground(QPainter *painter, const QRectF &)
{
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setTransform(QTransform(1, 0, 0, 1, 0, 0), true);
    painter->setPen(QColor(102, 153, 153, 150));

    QPointF p1(0., -2000000.);
    QPointF p2(0., 2000000.);
    QPointF p3(-2000000., 0.);
    QPointF p4( 2000000., 0.);
    painter->drawLine(p1, p2);
    painter->drawLine(p3, p4);
}

void CustomDrawGrView::wheelEvent(QWheelEvent *event)
{
    qDebug()<<"CustomDrawGrView::wheelEvent() : in";
    if ( event->modifiers() == Qt::NoModifier ) {
        changeViewScale( qPow(2.0, -event->delta() / 240.0) );
        qDebug()<<"CustomDrawGrView::wheelEvent() : return";
        return;
    }
    QGraphicsView::wheelEvent (event);
    qDebug()<<"CustomDrawGrView::wheelEvent() : out";
}

void CustomDrawGrView::changeViewScale(qreal factor)
{
    qreal scale = transform().m11() * factor;
    if ( scale < 1.0 / 16.0 / 100.0 || scale > 16.0 / 100.0 ) return;

    setTransform(QTransform(scale, 0, 0, -scale, 0, 0));

    emit scaleChanged(scale);
    qDebug() << "scale changed!" << scale;
}

void CustomDrawGrView::mousePressEvent (QMouseEvent *event)
{
    mousePressed = true;
    pressedPoint = mapToScene(event->pos());

    ( event->button() == Qt::LeftButton ) ?
    setCursor(Qt::ClosedHandCursor) : setCursor(Qt::OpenHandCursor) ;
}

void CustomDrawGrView::mouseReleaseEvent (QMouseEvent *event)
{
    mousePressed = false;
    setCursor(Qt::OpenHandCursor);
    QGraphicsView::mouseReleaseEvent(event);
}

void CustomDrawGrView::mouseMoveEvent (QMouseEvent *event)
{
    if ( mousePressed ) {
        qreal rmv = qreal(items().count()) / 1000.0 + 0.05;
        if ( rmv > 1.0 ) rmv = 1.0;
        QPointF pp = mapToScene(QPoint(viewport()->width() / 2, viewport()->height() / 2))
                     - (mapToScene(event->pos()) - pressedPoint) * rmv;
        centerOn(pp);
    }

    QGraphicsView::mouseMoveEvent(event);
}

void CustomDrawGrView::clearSelectedItems()
{
    mySelectedItems.clear();
    scene()->clearSelection();
}
/*
void CustomDrawGrView::setSelectItems( const QList<QGraphicsItem*> &mitems )
{
    mySelectedItems=mitems;
    Q_FOREACH( QGraphicsItem* mitem, mySelectedItems ){
        mitem->setSelected(true);
    }
}
*/
} // namespace post3dapp
