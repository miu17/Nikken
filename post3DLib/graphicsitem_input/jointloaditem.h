#ifndef JOINTLOADITEM_H
#define JOINTLOADITEM_H

#include "customdraw_item.h"
namespace post3dapp{
class JointLoadItem : public CustomDrawItem
{

public:

    JointLoadItem(const Input2DViewSettings& setting , const QString &);

    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event)}

private:

};
} // namespace post3dapp
#endif
