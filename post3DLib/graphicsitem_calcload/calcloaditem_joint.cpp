#include "calcloaditem_joint.h"

#include <QDebug>
#include <QPainter>
#include <qmath.h>

#include "unified_data.h"

namespace post3dapp{
CalcLoadJointItem::CalcLoadJointItem( DATATYPE dtype, const QUuid &uid, const CalculationLoad2DViewSettings &setting, int inum )
    : CalcLoadGraphicsItem( dtype, uid,setting)
{
    Q_UNUSED(inum);
//    sectionText=QString("%1").arg(inum);

}

QPainterPath CalcLoadJointItem::shape() const
{
    QPainterPath path;
    path.addRect(boundRect);
    return path;
}

void CalcLoadJointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    setPainter(painter, option);
    boundRect = QRectF(-10.0 * DpiScale(*painter), -10.0 * DpiScale(*painter), 20.0 * DpiScale(*painter), 20.0 * DpiScale(*painter));
    painter->drawEllipse(-5.0 * DpiScale(*painter), -5.0 * DpiScale(*painter), 10.0 * DpiScale(*painter), 10.0 * DpiScale(*painter));
}

void CalcLoadJointItem::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
}
} // namespace post3dapp
