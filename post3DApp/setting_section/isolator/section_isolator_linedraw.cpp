#include "section_isolator_linedraw.h"
#include <QtGui>
namespace post3dapp{
SectionIsolatorLineDraw::SectionIsolatorLineDraw(const SectionDataType &type, QWidget *parent)
    :SectionLineDraw(type, parent)
{

}

void SectionIsolatorLineDraw::paintEvent( QPaintEvent * )
{
    qDebug()<<"section line draw paint";
    QPainter painter( this );
    painter.setPen(QPen(Qt::darkGray, 1, Qt::SolidLine));
    QFont serifFont("Meiryo UI", 10);
    painter.setFont(serifFont);
    painter.drawRect(0, 0, width() - 1, height() - 1);
    if ( myValues.count() < 1 )
        return;

    drawOuterFrame(&painter);
    drawIsolatorGraph(&painter);
}


void SectionIsolatorLineDraw::drawOuterFrame(QPainter *pt)
{
    pt->drawLine(QPoint(0, 25), QPoint(width(), 25));

    drawflag = drawFlag::IJ;
    pt->drawLine(QPoint(width() / 2, 0), QPoint(width() / 2, height()));
    pt->setPen(Qt::black);
    pt->drawText(QRect(0, 0, width() / 2, 25), Qt::AlignCenter, u8"水平ばね");
    pt->drawText(QRect(width() / 2, 0, width() / 2, 25), Qt::AlignCenter, u8"鉛直ばね");
    pt->setPen(Qt::lightGray);
    pt->drawLine(QPoint(0, 10 + height() / 2), QPoint(width(), 10 + height() / 2));
    pt->drawLine(QPoint(width() * 0.25, 25), QPoint(width() * 0.25, height()));
    pt->drawLine(QPoint(width() * 0.75, 25), QPoint(width() * 0.75, height()));
    pt->drawText(QRect(width() * 0.25 - 20,     25, 20, 20), Qt::AlignCenter, "P");
    pt->drawText(QRect(width() * 0.5 - 20, 13 + height() / 2, 20, 20), Qt::AlignCenter, u8"δ");
    pt->drawText(QRect(width() * 0.75 - 20,     25, 20, 20), Qt::AlignCenter, "P");
    pt->drawText(QRect(width() - 20, 13 + height() / 2, 20, 20), Qt::AlignCenter, u8"δ");
}

void SectionIsolatorLineDraw::drawOneIsolatorGraph(QPainter* pt, const QPointF& origin, qreal maxX, qreal maxY, const SpringParameter& param) const
{
    switch (param.hysteresis) {
    case SpringType::Type::LINEAR:
        drawLinearGraph(param, origin, maxX, maxY, pt);
        break;
    case SpringType::Type::BILINEAR:
        drawBilinearGraph(param, origin, maxX, maxY, pt);
        break;
    case SpringType::Type::REVERSIBLE_BILINEAR:
        drawReversibleBilinearGraph(param, origin, maxX, maxY, pt);
        break;
    case SpringType::Type::TRILINEAR:
        drawTrilinearGraph(param, origin, maxX, maxY, pt);
        break;
    case SpringType::Type::TENSILE_REVERSIBLE_BILINEAR:
        drawTensileReversibleBilinearGraph(param, origin, maxX, maxY, pt);
        break;
    case SpringType::Type::TENSILE_REVERSIBLE_TRILINEAR:
        drawTensileReversibleTrilinearGraph(param, origin, maxX, maxY, pt);
        break;
    default:
        break;
    }

}


void SectionIsolatorLineDraw::drawIsolatorGraph(QPainter *pt)
{
    scaleFactor = calcScaleFactor(2, 2, 0., 0., 0);
    qreal oy = 10.0 + height() / 2.0;
    pt->setClipping(true);
    pt->setClipRect(0, 25, width() / 2.0, height() - 25);
    drawOneIsolatorGraph(pt, QPointF(width() * 0.25, oy), 1., 1000., mySection.hspring_param);
    pt->setClipRect(width() / 2., 25, width() / 2., height() - 25);
    drawOneIsolatorGraph(pt, QPointF(width() * 0.75, oy), 0.01, 10000., mySection.vspring_param);
    pt->setClipping(false);
}

void SectionIsolatorLineDraw::drawLinearGraph(const SpringParameter &spring,
                                      const QPointF& origin, qreal maxx, qreal maxy, QPainter *pt) const
{
    //maxx 1 m
    //maxy 1000 kN
    qreal K1 = spring.K1;
    QPointF p1 = origin;
    QPointF p2 = origin + QPointF(2 * scaleFactor / maxx, - (2 * K1) / maxy * scaleFactor);
    drawCrossPoint(origin, p2, maxx, maxy, scaleFactor, pt);

    QPointF p3 = origin + QPointF(- 2 * scaleFactor / maxx, (2 * K1) / maxy * scaleFactor);
    pt->setPen(Qt::SolidLine);
    pt->drawLine(p1, p2);
    pt->drawLine(p1, p3);
}

void SectionIsolatorLineDraw::drawBilinearGraph(const SpringParameter &spring,
                                        const QPointF& origin, qreal maxx, qreal maxy, QPainter *pt) const
{
    qreal K1 = spring.K1;
    qreal K2 = spring.K2;
    qreal fy1_plus = spring.Fy1p;
    qreal fy1_minus = spring.Fy1m;

    if (K1 < 1.0e-9)return;
//    if(K1<1.0e-5 || K2<-1.0e-5)return;
//    if(fy1_plus<1.0e-5 || fy1_minus>1.0e-5)return;
//    if(K1<K2)return;

    QPointF p1 = origin;
    QPointF p2 = origin + QPointF(fy1_plus / K1 / maxx * scaleFactor, - fy1_plus / maxy * scaleFactor);
    QPointF p3 = origin + QPointF(qMax(2 * fy1_plus / K1, 1.) / maxx * scaleFactor,
                          - (fy1_plus + (qMax(2 * fy1_plus / K1, 1.) - fy1_plus / K1) * K2) / maxy * scaleFactor);
    drawCrossPoint(origin, p2, maxx, maxy, scaleFactor, pt);
    drawCrossPoint(origin, p3, maxx, maxy, scaleFactor, pt);

    QPointF p4 = origin + QPointF(fy1_minus / K1 / maxx * scaleFactor, - fy1_minus / maxy * scaleFactor);
    QPointF p5 = origin + QPointF(qMin(2 * fy1_minus / K1, -1.) / maxx * scaleFactor,
                         - (fy1_minus + (qMin(2 * fy1_minus / K1, -1.) - fy1_minus / K1) * K2) / maxy * scaleFactor);
    QPointF p6 = p3 + p4 - p2;
    QPointF p7 = p5 + p2 - p4;


    pt->setPen(Qt::SolidLine);
    pt->drawLine(p1, p2);
    pt->drawLine(p2, p3);
    pt->drawLine(p3, p6);
    pt->drawLine(p6, p4);
    pt->drawLine(p4, p5);
    pt->drawLine(p5, p7);
    pt->drawLine(p7, p2);
    pt->drawLine(p1, p4);
    drawArrow(p2, p3, 10, pt);
    drawArrow(p3, p6, 10, pt);
    drawArrow(p6, p4, 10, pt);
    drawArrow(p4, p5, 10, pt);
    drawArrow(p5, p7, 10, pt);
    drawArrow(p7, p2, 10, pt);

}

void SectionIsolatorLineDraw::drawTrilinearGraph(const SpringParameter &spring,
                                        const QPointF& origin, qreal maxx, qreal maxy, QPainter *pt) const
{
    qreal K1 = spring.K1;
    qreal K2 = spring.K2;
    qreal K3 = spring.K3;
    qreal fy1_plus = spring.Fy1p;
    qreal fy2_plus = spring.Fy2p;

    if (K1 < 1.0e-9 || K2 < 1.0e-9) return;

    QPointF p1 = origin;
    QPointF p2 = p1 + QPointF(fy1_plus / K1 / maxx * scaleFactor, -fy1_plus / maxy * scaleFactor);
    QPointF p3 = p2 + QPointF((fy2_plus - fy1_plus) / K2 / maxx * scaleFactor,
                              -(fy2_plus - fy1_plus) / maxy * scaleFactor);
    drawCrossPoint(origin, p2, maxx, maxy, scaleFactor, pt);
    drawCrossPoint(origin, p3, maxx, maxy, scaleFactor, pt);

    qreal max = qMax(p3.x() - p2.x(), 1.5 / maxx * scaleFactor + p1.x() - p3.x());
    QPointF p4 = p3 + QPointF(max, -max * K3 * maxx / maxy);
    QPointF p5 = p4 - 2 * (p2 - p1);
    QPointF p6 = p5 - 2 * (p3 - p2);

    QPointF p7 = p1 - (p4 - p1);
    QPointF p8 = p7 + 2 * (p2 - p1);
    QPointF p9 = p8 + 2 * (p3 - p2);

    pt->setPen(Qt::SolidLine);
    pt->drawLine(p1, p2);
    drawArrow(p1, p2, 10, pt);
    pt->drawLine(p2, p3);
    drawArrow(p2, p3, 10, pt);
    pt->drawLine(p3, p4);
    drawArrow(p3, p4, 10, pt);
    pt->drawLine(p4, p5);
    drawArrow(p4, p5, 10, pt);
    pt->drawLine(p5, p6);
    drawArrow(p5, p6, 10, pt);
    pt->drawLine(p6, p7);
    drawArrow(p6, p7, 10, pt);
    pt->drawLine(p7, p8);
    drawArrow(p7, p8, 10, pt);
    pt->drawLine(p8, p9);
    drawArrow(p8, p9, 10, pt);
    pt->drawLine(p9, p3);
    drawArrow(p9, p3, 10, pt);
}

void SectionIsolatorLineDraw::drawTensileReversibleBilinearGraph(const SpringParameter &spring,
                                                                     const QPointF& origin, qreal maxx, qreal maxy, QPainter *pt) const
{
    drawReversibleBilinearGraph(spring.K1, spring.K2, spring.Fy1p, -1000000000., origin, scaleFactor, maxx, maxy, pt);
}

void SectionIsolatorLineDraw::drawReversibleBilinearGraph(const SpringParameter &spring,
                                                  const QPointF& origin, qreal maxx, qreal maxy, QPainter *pt) const
{
    drawReversibleBilinearGraph(spring.K1, spring.K2, spring.Fy1p, spring.Fy1m, origin, scaleFactor, maxx, maxy, pt);
}

void SectionIsolatorLineDraw::drawReversibleBilinearGraph(qreal k1, qreal k2, qreal fy1_plus, qreal fy1_minus,
                                                  const QPointF& origin, qreal scaleFactor, qreal maxx, qreal maxy, QPainter *pt)
{
    if (k1 < 1.0e-9)return;

    QPointF p1 = origin;
    QPointF p2 = p1 + QPointF(fy1_plus / k1 / maxx * scaleFactor, - fy1_plus / maxy * scaleFactor);
    drawCrossPoint(origin, p2, maxx, maxy, scaleFactor, pt);

    qreal max = qMax(2 * fy1_plus / k1, 3.);
    QPointF p3 = p1 + QPointF(max / maxx * scaleFactor, - (fy1_plus + (max - fy1_plus / k1) * k2) / maxy * scaleFactor);

    QPointF p4 = p1 + QPointF(fy1_minus / k1 / maxx * scaleFactor, - fy1_minus / maxy * scaleFactor);
    qreal min = qMin(2 * fy1_minus / k1, -3.);
    QPointF p5 = p1 + QPointF(min / maxx * scaleFactor, - (fy1_minus + (min - fy1_minus / k1) * k2) / maxy * scaleFactor);
    QString unitx, unity;


    pt->setPen(Qt::SolidLine);
    pt->drawLine(p1, p2);
    pt->drawLine(p2, p3);
    pt->drawLine(p4, p5);
    pt->drawLine(p1, p4);
}


void SectionIsolatorLineDraw::drawTensileReversibleTrilinearGraph(const SpringParameter &spring,
                                                                     const QPointF& origin, qreal maxx, qreal maxy, QPainter *pt) const
{
    drawReversibleTrilinearGraph(spring.K1, spring.K2, spring.K3, spring.Fy1p, -100000000., spring.Fy2p, -200000000., origin, scaleFactor, maxx, maxy, pt);
}

void SectionIsolatorLineDraw::drawReversibleTrilinearGraph(const SpringParameter &spring, const QPointF& origin, qreal maxx, qreal maxy, QPainter *pt) const
{
    drawReversibleTrilinearGraph(spring.K1, spring.K2, spring.K3, spring.Fy1p, spring.Fy1m, spring.Fy2p, spring.Fy2m, origin, scaleFactor, maxx, maxy, pt);
}


void SectionIsolatorLineDraw::drawReversibleTrilinearGraph(qreal k1, qreal k2, qreal k3, qreal fy1_plus, qreal fy1_minus, qreal fy2_plus, qreal fy2_minus,
                                                   const QPointF& origin, qreal scaleFactor, qreal maxx, qreal maxy, QPainter *pt)
{
    if (k1 < 1.0e-9 || k2 < 1.0e-9) return;

    QPointF p1 = origin;
    QPointF p2 = p1 + QPointF(fy1_plus / k1 / maxx * scaleFactor,
                              -fy1_plus / maxy * scaleFactor);
    QPointF p3 = p2 + QPointF((fy2_plus - fy1_plus) / k2 / maxx * scaleFactor,
                              -(fy2_plus - fy1_plus) / maxy * scaleFactor);
    drawCrossPoint(origin, p2, maxx, maxy, scaleFactor, pt);
    drawCrossPoint(origin, p3, maxx, maxy, scaleFactor, pt);

    QPointF p4 = p3 + QPointF(3 / maxx * scaleFactor + p1.x() - p3.x(),
                              -(3 / maxx * scaleFactor + p1.x() - p3.x()) * k3 * maxx / maxy);
    QPointF p5 = p1 + QPointF(fy1_minus / k1 / maxx * scaleFactor,
                              -fy1_minus / maxy * scaleFactor);
    QPointF p6 = p5 + QPointF((fy2_minus - fy1_minus) / k2 / maxx * scaleFactor,
                              -(fy2_minus - fy1_minus) / maxy * scaleFactor);
    QPointF p7 = p6 + QPointF(-3 / maxx * scaleFactor + p1.x() - p6.x(),
                              -(-3 / maxx * scaleFactor + p1.x() - p6.x()) * k3 * maxx / maxy);


    pt->setPen(Qt::SolidLine);
    pt->drawLine(p1, p2);
    pt->drawLine(p2, p3);
    pt->drawLine(p3, p4);
    pt->drawLine(p1, p5);
    pt->drawLine(p5, p6);
    pt->drawLine(p6, p7);
}

void SectionIsolatorLineDraw::drawOriginOrientedTrilinearGraph(const SpringParameter &spring,
                                                       const QPointF& origin, qreal maxx, qreal maxy, QPainter *pt) const
{
    qreal K1 = spring.K1;
    qreal K2 = spring.K2;
    qreal K3 = spring.K3;
    qreal fy1_plus = spring.Fy1p;
    qreal fy1_minus = spring.Fy1m;
    qreal fy2_plus = spring.Fy2p;
    qreal fy2_minus = spring.Fy2m;

    if (K1 < 1.0e-9 || K2 < 1.0e-9) return;

    QPointF p1 = origin;
    QPointF p2 = p1 + QPointF(fy1_plus / K1 / maxx * scaleFactor,
                              -fy1_plus / maxy * scaleFactor);
    QPointF p3 = p2 + QPointF((fy2_plus - fy1_plus) / K2 / maxx * scaleFactor,
                              -(fy2_plus - fy1_plus) / maxy * scaleFactor);
    QPointF p4 = p3 + QPointF(qMax(p3.x() - p2.x(), 1.5 / maxx * scaleFactor + p1.x() - p3.x()),
                              -qMax(p3.x() - p2.x(), 1.5 / maxx * scaleFactor + p1.x() - p3.x()) * K3 * maxx / maxy);
    QPointF p5 = p1 + QPointF(fy1_minus / K1 / maxx * scaleFactor,
                              -fy1_minus / maxy * scaleFactor);
    QPointF p6 = p5 + QPointF((fy2_minus - fy1_minus) / K2 / maxx * scaleFactor,
                              -(fy2_minus - fy1_minus) / maxy * scaleFactor);
    QPointF p7 = p6 + QPointF((p5.x() - p4.x()),
                              -(p5.x() - p4.x()) * maxx / maxy * K3);
    QPointF p8 = (p2 + p3) / 2;
    QPointF p9 = (p3 + p4) / 2; //a:p5.x b:p5.y c:p6.x d:p6.y e:p8.x f:p8.y g:p1.x h:p1.y
    QPointF p10 = QPointF(((p8.y() * p1.x() - p8.x() * p1.y()) * (p6.x() - p5.x()) -
                           (p5.y() * p6.x() - p5.x() * p6.y()) * (p1.x() - p8.x()))
                          / ((p6.y() - p5.y()) * (p1.x() - p8.x()) - (p6.x() - p5.x()) * (p1.y() - p8.y())),
                          ((p8.y() * p1.x() - p8.x() * p1.y()) * (p6.y() - p5.y()) - (p5.y() * p6.x() - p5.x() * p6.y()) *
                           (p1.y() - p8.y()))
                          / ((p6.y() - p5.y()) * (p1.x() - p8.x()) - (p6.x() - p5.x()) * (p1.y() -
                                                                                          p8.y()))); //p5p6とp8p1の交点
    QPointF p11 = QPointF(((p8.y() * p1.x() - p8.x() * p1.y()) * (p6.x() - p7.x()) -
                           (p7.y() * p6.x() - p7.x() * p6.y()) * (p1.x() - p8.x()))
                          / ((p6.y() - p7.y()) * (p1.x() - p8.x()) - (p6.x() - p7.x()) * (p1.y() - p8.y())),
                          ((p8.y() * p1.x() - p8.x() * p1.y()) * (p6.y() - p7.y()) - (p7.y() * p6.x() - p7.x() * p6.y()) *
                           (p1.y() - p8.y()))
                          / ((p6.y() - p7.y()) * (p1.x() - p8.x()) - (p6.x() - p7.x()) * (p1.y() -
                                                                                          p8.y()))); //p6p7とp8p1の交点→場合分け
    QPointF p12 = QPointF(((p9.y() * p1.x() - p9.x() * p1.y()) * (p6.x() - p7.x()) -
                           (p7.y() * p6.x() - p7.x() * p6.y()) * (p1.x() - p9.x()))
                          / ((p6.y() - p7.y()) * (p1.x() - p9.x()) - (p6.x() - p7.x()) * (p1.y() - p9.y())),
                          ((p9.y() * p1.x() - p9.x() * p1.y()) * (p6.y() - p7.y()) - (p7.y() * p6.x() - p7.x() * p6.y()) *
                           (p1.y() - p9.y()))
                          / ((p6.y() - p7.y()) * (p1.x() - p9.x()) - (p6.x() - p7.x()) * (p1.y() -
                                                                                          p9.y()))); //p6p7とp9p1の交点
    QPointF p13 = QPointF(((p9.y() * p1.x() - p9.x() * p1.y()) * (p6.x() - p5.x()) -
                           (p5.y() * p6.x() - p5.x() * p6.y()) * (p1.x() - p9.x()))
                          / ((p6.y() - p5.y()) * (p1.x() - p9.x()) - (p6.x() - p5.x()) * (p1.y() - p9.y())),
                          ((p9.y() * p1.x() - p9.x() * p1.y()) * (p6.y() - p5.y()) - (p5.y() * p6.x() - p5.x() * p6.y()) *
                           (p1.y() - p9.y()))
                          / ((p6.y() - p5.y()) * (p1.x() - p9.x()) - (p6.x() - p5.x()) * (p1.y() -
                                                                                          p9.y()))); //p5p6とp9p1の交点

    drawCrossPoint(origin, p2, maxx, maxy, scaleFactor, pt);
    drawCrossPoint(origin, p3, maxx, maxy, scaleFactor, pt);

    pt->setPen(Qt::SolidLine);
    pt->drawLine(p1, p2);
    pt->drawLine(p2, p8);
    pt->drawLine(p8, p1);
    if (p12.x() < p1.x() && (p12.x() > p13.x() || p13.x() > p1.x())) {
        pt->drawLine(p12, p1);
        drawArrow(p12, p1, 10, pt);
    } else if (p13.x() < p1.x()) {
        pt->drawLine(p13, p1);
        drawArrow(p13, p1, 10, pt);
    }
    pt->drawLine(p1, p9);
    pt->drawLine(p9, p4);
    if (p10.x() < p1.x() && (p10.x() > p11.x() || p11.x() > p1.x())) {
        pt->drawLine(p1, p10);
        pt->drawLine(p10, p6);
        if (p12.x() < p1.x() && (p12.x() > p13.x() || p13.x() > p1.x())) {
            pt->drawLine(p6, p12);
            drawArrow(p6, p12, 10, pt);
        } else if (p13.x() < p1.x()) {
            pt->drawLine(p6, p13);
            drawArrow(p6, p13, 10, pt);
        }
        drawArrow(p1, p10, 10, pt);
        drawArrow(p10, p6, 10, pt);
    } else if (p11.x() < p1.x()) {
        pt->drawLine(p1, p11);
        if (p12.x() < p1.x() && (p12.x() > p13.x() || p13.x() > p1.x())) {
            pt->drawLine(p11, p12);
            drawArrow(p11, p12, 10, pt);
        } else if (p13.x() < p1.x()) {
            pt->drawLine(p11, p13);
            drawArrow(p11, p13, 10, pt);
        }
        drawArrow(p1, p11, 10, pt);
    }
    drawArrow(p1, p2, 10, pt);
    drawArrow(p2, p8, 10, pt);
    drawArrow(p8, p1, 10, pt);
    drawArrow(p1, p9, 10, pt);
    drawArrow(p9, p4, 10, pt);


    pt->setPen(Qt::DotLine);
    pt->drawLine(p1, p5);
    if (p10.x() > p11.x()) {
        pt->drawLine(p5, p10);
    } else {
        pt->drawLine(p5, p6);
        pt->drawLine(p6, p11);
    }
    pt->drawLine(p8, p3);
    pt->drawLine(p3, p9);
}
} // namespace post3dapp
