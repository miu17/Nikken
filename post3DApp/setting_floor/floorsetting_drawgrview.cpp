#include "floorsetting_drawgrview.h"

#include <QtGui>

#include "calc_simple.h"
#include "unified_data.h"
namespace post3dapp{
FloorSettingDrawGrView::FloorSettingDrawGrView(QWidget *parent)
    : CustomDrawGrView(UnifiedDataType::Input,parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    minHeight = 0.0;
    maxHeight = 20000.0;
}

void FloorSettingDrawGrView::mouseMoveEvent (QMouseEvent *event)
{
    if ( mousePressed ) {
        QPointF pp = mapToScene(QPoint(viewport()->width() / 2, viewport()->height() / 2))
                     - (mapToScene(event->pos()) - pressedPoint) / 10.;
        centerOn(QPoint(0, pp.y()));
    }

    QGraphicsView::mouseMoveEvent (event);
}

void FloorSettingDrawGrView::fitWindow()
{

    QRectF bound_rect = QRectF(QPointF(-1., minHeight), QPointF(1., maxHeight));
    if ( !bound_rect.isValid() ) bound_rect = QRectF(-1., -10000., 2., 30000.);

    fitInView(bound_rect, Qt::KeepAspectRatio);
    qreal scale = transform().m11();
    emit scaleChanged(scale);

}

void FloorSettingDrawGrView::setDrawingData(const QList<FLOORVALUES> &list)
{
    allFloorList = list;
    if (!allFloorList.isEmpty()) {
        minHeight = allFloorList.first().floorHeight * 1000.0;
        maxHeight = allFloorList.last().floorHeight * 1000.0;
    } else {
        minHeight = 0.0;
        maxHeight = 20000.0;
    }
    scene()->update();
}

void FloorSettingDrawGrView::drawForeground(QPainter *painter, const QRectF &)
{

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setTransform(QTransform(1, 0, 0, 1, 0, 0), false);
    QPen pen =  QPen(QBrush(QColor(150, 150, 150)),0);
    painter->setPen(pen);
    painter->setBrush(QColor(220, 220, 220));
    painter->setFont(QFont("Helvetica [Cronyx]", 9));

    qreal vw = viewport()->width();
    qreal vh = viewport()->height();
    qreal sx = vw - 70;

    painter->drawRect(QRect(sx, 0, 70, vh));

    QPointF mp1 = mapToScene(QPoint(0, 0));
    QPointF mp2 = mapToScene(QPoint(0, vh));

    qreal tick = CalcSimple::tic(mp1.y(), mp2.y());
    int ns = (mp2.y() + 0.0001) / tick - 1;
    int ne = (mp1.y() + 0.0001) / tick;
    for (int i = ns; i <= ne; ++i) {
        QPoint p1 = mapFromScene(0, tick * qreal(i));
        QPoint p2 = mapFromScene(0, tick * qreal(i + 1));
        int y1 = p1.y();
        qreal dy = qreal(p2.y() - y1) / 10.;
        painter->drawLine(QPoint(sx, y1), QPoint(vw, y1));
        QString str = QString("%1m").arg( tick * qreal(i) / 1000., 0, 'f', 2);
        QRect txrect = QRect(sx + 10, y1 - 30, 55, 30);
        painter->drawText(txrect, Qt::AlignRight | Qt::AlignBottom, str);
        for (int j = 1; j < 10 ; ++j) {
            painter->drawLine(QPoint(sx, y1 + dy * qreal(j)), QPoint(sx + 6, y1 + dy * qreal(j)));
        }
    }

}

void FloorSettingDrawGrView::drawBackground(QPainter *painter, const QRectF &)
{

    if (allFloorList.isEmpty()) return;

    painter->setRenderHint(QPainter::Antialiasing, false);

    QTransform b_trans = painter->transform();
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    QTransform t_trans = painter->transform();
    QPointF p1, p2, p3, p4, p5, p6, p7, p8;
    QString str;
    qreal dh;

    QPen pen =  QPen(QBrush(QColor(102, 153, 153, 150)),0);
    painter->setPen(pen);
    QPointF mp1 = mapToScene(QPoint(0, 0));
    QPointF mp2 = mapToScene(QPoint(width(), 0));
    painter->drawLine(QPointF(mp1.x(), 0.), QPointF(mp2.x(), 0.));

    QPointF p02 = mapToScene((width() - 80) / 2, 0);
    qreal dx = p02.x() - 3500.;
    QFont myFont("Helvetica [Cronyx]", 500);
    painter->setFont(myFont);
    QPen blackpen =  QPen(QBrush(Qt::black),0);
    painter->setPen(blackpen);

    for (int i = 0; i < allFloorList.count() ; i++) {

        painter->setTransform(b_trans);

        p1 = QPointF(dx, allFloorList.at(i).floorHeight * 1000.0 );
        p2 = p1 + QPointF(5000., 0.);
        painter->drawLine(p1, p2);
        p3 = p1 + QPointF(2000., 0.);
        p4 = p3 + QPointF(-300., 520.);
        p5 = p4 + QPointF( 600., 0.);
        painter->drawLine(p3, p4);
        painter->drawLine(p4, p5);
        painter->drawLine(p5, p3);

        painter->setTransform(t_trans);
        str = allFloorList.at(i).floorName;
        p3 = p1 + QPointF(0., 3000.);
        p4 = p3 + QPointF(4000.0, -2480.);
        p3.setY(-p3.y());
        p4.setY(-p4.y());
        painter->drawText(QRectF(p3, p4), Qt::AlignHCenter | Qt::AlignBottom | Qt::TextDontClip, str);

        if (i == 0) {
            p6 = p2;
            continue;
        }
        painter->setTransform(b_trans);
        painter->drawLine(p2, p6);

        dh = (allFloorList.at(i).floorHeight - allFloorList.at(i - 1).floorHeight) * 1000.;

        p7 = p6 + QPointF(-2000., 0.);
        p8 = p7 + QPointF(dh, -2000.);
        p7.setY(-p7.y());
        p8.setY(-p8.y());
        str = QString("%1").arg(dh, 0, 'f', 0);

        painter->setTransform(t_trans);
        painter->translate(p7.x(), p7.y());
        painter->rotate(-90);
        painter->translate(-p7.x(), -p7.y());

        if ( painter->fontMetrics().width(str) > dh ) {
            int stretch = int(dh * 100. / qreal(painter->fontMetrics().width(str)));
            if (stretch > 0) myFont.setStretch(stretch);
            painter->setFont(myFont);
        }
        painter->drawText(QRectF(p7, p8),
                          Qt::AlignHCenter | Qt::AlignBottom, str);
        myFont.setStretch(100);
        painter->setFont(myFont);

        p6 = p2;
    }

    if (allFloorList.count() < 2) return;

    if (minHeight < 0. && maxHeight > 0.) {

        dh = -minHeight;
        painter->setTransform(b_trans);
        p1 = QPointF(dx + 5000., minHeight);
        p2 = p1 + QPointF(2000., 0.);
        p3 = p2 + QPointF(0., dh);
        p4 = p3 + QPointF(-2000., 0.);
        painter->drawLine(p1, p2);
        painter->drawLine(p2, p3);
        painter->drawLine(p3, p4);

        p2 = p1 + QPointF(dh, -2000.);
        p1.setY(-p1.y());
        p2.setY(-p2.y());
        str = QString("%1").arg(dh, 0, 'f', 0);
        painter->setTransform(t_trans);
        painter->translate(p1.x(), p1.y());
        painter->rotate(-90);
        painter->translate(-p1.x(), -p1.y());
        painter->drawText(QRectF(p1, p2),
                          Qt::AlignHCenter | Qt::AlignBottom | Qt::TextDontClip, str);

        dh = maxHeight;
        painter->setTransform(b_trans);
        p1 = QPointF(dx + 5000., 0);
        p2 = p1 + QPointF(2000., 0.);
        p3 = p2 + QPointF(0., dh);
        p4 = p3 + QPointF(-2000., 0.);
        painter->drawLine(p2, p3);
        painter->drawLine(p3, p4);

        p2 = p1 + QPointF(dh, -2000.);
        p1.setY(-p1.y());
        p2.setY(-p2.y());
        str = QString("%1").arg(dh, 0, 'f', 0);
        painter->setTransform(t_trans);
        painter->translate(p1.x(), p1.y());
        painter->rotate(-90);
        painter->translate(-p1.x(), -p1.y());
        painter->drawText(QRectF(p1, p2),
                          Qt::AlignHCenter | Qt::AlignBottom | Qt::TextDontClip, str);

    } else {

        dh = maxHeight - minHeight;
        painter->setTransform(b_trans);
        p1 = QPointF(dx + 5000., minHeight);
        p2 = p1 + QPointF(2000., 0.);
        p3 = p2 + QPointF(0., dh);
        p4 = p3 + QPointF(-2000., 0.);
        painter->drawLine(p1, p2);
        painter->drawLine(p2, p3);
        painter->drawLine(p3, p4);

        p2 = p1 + QPointF(dh, -2000.);
        p1.setY(-p1.y());
        p2.setY(-p2.y());
        str = QString("%1").arg(dh, 0, 'f', 0);
        painter->setTransform(t_trans);
        painter->translate(p1.x(), p1.y());
        painter->rotate(-90);
        painter->translate(-p1.x(), -p1.y());
        painter->drawText(QRectF(p1, p2),
                          Qt::AlignHCenter | Qt::AlignBottom | Qt::TextDontClip, str);

    }

}
} // namespace post3dapp
