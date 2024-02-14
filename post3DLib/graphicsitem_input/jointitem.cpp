#include "jointitem.h"

#include <QPainter>
#include <qmath.h>
#include <QDebug>
namespace post3dapp{
JointItem::JointItem(const Input2DViewSettings& setting,int inum, bool isFirst )
    : CustomDrawItem(setting, ELJOINT, QString(), ITEMTYPE::POINTITEM), isCopyedItem(!isFirst)
{
    textName = QString("%1").arg(inum);
    isInterior = false;
}

void JointItem::setNodeNumber(int inum)
{
    textName = QString("%1").arg(inum);
    update(boundRect);
}

void JointItem::setInteriorOn(bool onoff)
{
    isInterior = onoff;
    update(boundRect);
}

QPainterPath JointItem::shape() const
{
    QPainterPath path;
    path.addRect(boundRect);
    return path;
}

void JointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    setPainter(painter, option);

    /*
        if( isCopyedItem ){
            painter->setPen(Qt::gray);
            painter->setBrush(QBrush(Qt::gray));
        }
    */

    if ( isInterior ) painter->setBrush(Qt::white);
    const qreal factor = 1 / qPow(painter->transform().m11(), 0.75);
    boundRect = QRectF(-10.0 * factor, -10.0 * factor, 20.0 * factor, 20.0 * factor);
    painter->drawEllipse(-5.0 * factor, -5.0 * factor, 10.0 * factor, 10.0 * factor);

    /*
        if( isInputState ){
            painter->setTransform(QTransform(1, 0, 0, -1, 0, 0),true);
            painter->setFont(QFont("Arial", (nameSize*3+14)*factor*1.5));
            painter->drawText(QRect(25.0*factor, -50.0*factor, 300.0*factor, 100.0*factor), Qt::AlignLeft|Qt::AlignVCenter,"<- Base" );
            if( !isNameVisible ) return;
            painter->setFont(QFont("Arial", (nameSize*3+14)*factor));
            painter->drawText(QRect(-310.0*factor, -40.0*factor, 300.0*factor, 35.0*factor), Qt::AlignRight|Qt::AlignVCenter, textName);
            return;
        }
    */

    if ( !isNameVisible ) return;

    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->setFont(QFont("Arial", FontSize(*painter) * 0.7));
    QRect boundingRect = QFontMetrics(painter->font()).boundingRect(textName);
    painter->drawText(QRect(-boundingRect.width(), -boundingRect.height(), boundingRect.width(), boundingRect.height()),
                      Qt::AlignRight | Qt::AlignVCenter, textName);
}

QVariant JointItem::itemChange(GraphicsItemChange change, const QVariant &var)
{
    switch (change) {
    case ItemSelectedHasChanged:
        if ( var.toBool() == false ) setInputState(false);
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, var);
}

} // namespace post3dapp
