#include "jointloaditem.h"

#include <QPainter>
#include <qmath.h>
#include <QDebug>
namespace post3dapp{
JointLoadItem::JointLoadItem(const Input2DViewSettings &setting,const QString &name )
    : CustomDrawItem(setting,ELJOINTLOAD, name, ITEMTYPE::POINTITEM)
{
    if ( sectionName.isEmpty() ) {
        myElementState = ELDUMMY;
        textName = "(dummyP)";
    } else {
        myElementState = ELNORMAL;
        textName = name;
    }
}

QPainterPath JointLoadItem::shape() const
{
    QPainterPath path;
    path.addRect(boundRect);
    return path;
}

void JointLoadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    setPainter(painter, option);
    const qreal factor = 1 / qPow(painter->transform().m11(), 0.75);
    boundRect = QRectF(-30.0 * factor, -30.0 * factor, 60.0 * factor, 60.0 * factor);
    painter->drawEllipse(-15.0 * factor, -15.0 * factor, 30.0 * factor, 30.0 * factor);

    if ( !isNameVisible ) return;

    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->setFont(QFont("Consolas", FontSize(*painter)));
    QRect boundingRect = QFontMetrics(painter->font()).boundingRect(textName);
    painter->drawText(QRect(0.0, -boundingRect.height(), boundingRect.width(), boundingRect.height()),
                      Qt::AlignLeft | Qt::AlignVCenter, textName);
}

} // namespace post3dapp
