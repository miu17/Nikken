#include "section_base_linedraw.h"
#include <QtGui>
namespace post3dapp{
SectionBaseLineDraw::SectionBaseLineDraw(const SectionDataType &type, QWidget *parent )
    : SectionLineDraw(type, parent)
{
}

void SectionBaseLineDraw::paintEvent( QPaintEvent * )
{
    qDebug()<<"section line draw paint";

    QPainter painter( this );

    painter.setPen(QPen(Qt::darkGray, 1, Qt::SolidLine));
    QFont serifFont("Meiryo UI", 10);
    painter.setFont(serifFont);

    painter.drawRect(0, 0, width() - 1, height() - 1);

    if ( myValues.count() < 1 )return;
    drawOuterFrame(&painter);
    drawBasePlate(&painter);
}

void SectionBaseLineDraw::drawBasePlate(QPainter *pt)
{
    qreal B = mySection.base_param.sizeB;
    qreal D = mySection.base_param.sizeD;
    qreal SML = mySection.base_param.SML;

    qreal ox = width() * 0.5;
    qreal oy = 10 + height() / 2;
    drawflag = drawFlag::I;
    scaleFactor = calcScaleFactor(B, D, 0., 0., 0);
    drawScale(-B / 2., B / 2., -D / 2., D / 2., ox, oy, SCALEDW::D3N2, pt, scaleFactor);
    if (mySection.base_param.boltType != BaseBoltType::Type::CIRCLE) {
        drawScale(-B / 2, -B / 2 + mySection.base_param.edgehy1, -D / 2, -D / 2 + mySection.base_param.edgehz1, ox, oy, SCALEDW::D3N1, pt, scaleFactor); //edx1edy1
        drawScale(B / 2 - mySection.base_param.edgehy2, B / 2, D / 2 - mySection.base_param.edgehz2, D / 2, ox, oy, SCALEDW::D3N1, pt, scaleFactor); //edx2edy2
    }
    QPainterPath myPath;
    myPath.setFillRule(Qt::OddEvenFill);
    //ベースプレート形状
    QList<QPointF> bplpoints;
    switch (mySection.base_param.basePlateType) {
    case BasePlateType::Type::RECTANGLE:
        bplpoints << QPointF(B / 2, D / 2)
                  << QPointF(-B / 2, D / 2)
                  << QPointF(-B / 2, -D / 2)
                  << QPointF(B / 2, -D / 2)
                  << QPointF(B / 2, D / 2);
        break;
    case BasePlateType::Type::OCTAGON:
        bplpoints << QPointF(B / 2, D / 2 - SML)
                  << QPointF(B / 2 - SML, D / 2)
                  << QPointF(-B / 2 + SML, D / 2)
                  << QPointF(-B / 2, D / 2 - SML)
                  << QPointF(-B / 2, -D / 2 + SML)
                  << QPointF(-B / 2 + SML, -D / 2)
                  << QPointF(B / 2 - SML, -D / 2)
                  << QPointF(B / 2, -D / 2 + SML)
                  << QPointF(B / 2, D / 2 - SML);
        break;
    case BasePlateType::Type::DTYPE_L:
        bplpoints << QPointF(B / 2, D / 2 - SML)
                  << QPointF(B / 2 - SML, D / 2)
                  << QPointF(-B / 2, D / 2)
                  << QPointF(-B / 2, -D / 2)
                  << QPointF(B / 2 - SML, -D / 2)
                  << QPointF(B / 2, -D / 2 + SML)
                  << QPointF(B / 2, D / 2 - SML);
        break;
    case BasePlateType::Type::DTYPE_R:
        bplpoints << QPointF(B / 2, D / 2)
                  << QPointF(-B / 2 + SML, D / 2)
                  << QPointF(-B / 2, D / 2 - SML)
                  << QPointF(-B / 2, -D / 2 + SML)
                  << QPointF(-B / 2 + SML, -D / 2)
                  << QPointF(B / 2, -D / 2)
                  << QPointF(B / 2, D / 2);
        break;
    case BasePlateType::Type::DTYPE_U:
        bplpoints << QPointF(B / 2, D / 2)
                  << QPointF(-B / 2, D / 2)
                  << QPointF(-B / 2, -D / 2 + SML)
                  << QPointF(-B / 2 + SML, -D / 2)
                  << QPointF(B / 2 - SML, -D / 2)
                  << QPointF(B / 2, -D / 2 + SML)
                  << QPointF(B / 2, D / 2);
        break;
    case BasePlateType::Type::DTYPE_D:
        bplpoints << QPointF(B / 2, D / 2 - SML)
                  << QPointF(B / 2 - SML, D / 2)
                  << QPointF(-B / 2 + SML, D / 2)
                  << QPointF(-B / 2, D / 2 - SML)
                  << QPointF(-B / 2, -D / 2)
                  << QPointF(B / 2, -D / 2)
                  << QPointF(B / 2, D / 2 - SML);
        break;
    case BasePlateType::Type::CIRCLE:
        break;
    default:
        return;
    }

    if (mySection.base_param.basePlateType == BasePlateType::Type::CIRCLE) {
        myPath.addEllipse(QPointF(ox, oy), B / 2 * scaleFactor, B / 2 * scaleFactor);
    } else {
        for (int i = 0; i < bplpoints.size(); ++i) {
            bplpoints[i] = QPointF(bplpoints[i].x() * scaleFactor + ox, -bplpoints[i].y() * scaleFactor + oy);
        }
        myPath.moveTo(bplpoints[0]);
        for (int i = 1; i < bplpoints.count(); ++i) {
            myPath.lineTo(bplpoints[i]);
        }
        myPath.closeSubpath();
    }
    //ボルト穴

    QList<QPointF> boltlist = mySection.base_param.boltPointList();
    for (int i = 0; i < boltlist.size(); ++i) {
        boltlist[i] = QPointF(boltlist[i].x() * scaleFactor + ox, -boltlist[i].y() * scaleFactor + oy);
    }
    Q_FOREACH (QPointF pp, boltlist) {
        myPath.addEllipse(pp, mySection.base_param.boltHolePhi / 2 * scaleFactor, mySection.base_param.boltHolePhi / 2 * scaleFactor);
    }

    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);
    pt->setBrush(Qt::NoBrush);
    pt->setRenderHint(QPainter::Antialiasing, false);

}

void SectionBaseLineDraw::drawOuterFrame(QPainter *pt)
{
    pt->drawLine(QPoint(0, 25), QPoint(width(), 25));
    pt->setPen(Qt::black);
    pt->drawText(QRect(0, 0, width(), 25), Qt::AlignCenter, u8"ベースプレート形状");
    pt->setPen(Qt::lightGray);
    pt->drawLine(QPoint(0, 10 + height() / 2), QPoint(width(), 10 + height() / 2));
    pt->drawLine(QPoint(width() * 0.5, 25), QPoint(width() * 0.5, height()));
    pt->drawText(QRect(width() * 0.5 - 20,     25, 20, 20), Qt::AlignCenter, "z");
    pt->drawText(QRect(width() - 20, 10 + height() / 2, 20, 20), Qt::AlignCenter, "y");
}
} // namespace post3dapp
