#ifndef JOINTITEM_H
#define JOINTITEM_H

#include "customdraw_item.h"
namespace post3dapp{
class JointItem : public CustomDrawItem
{
public:
    JointItem(const Input2DViewSettings &setting, int inum, bool isFirst = true );

    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setNodeNumber(int);
    void setInteriorOn(bool);
    bool isCopyedJoint() const
    {
        return isCopyedItem;
    }

protected:

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event)}

private:

    bool isInterior;
    const bool isCopyedItem;

};
} // namespace post3dapp
#endif
