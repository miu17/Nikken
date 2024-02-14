#ifndef CUSTOMDRAWVLINEITEM_H
#define CUSTOMDRAWVLINEITEM_H

#include "customdraw_item.h"
#include "lineshape.h"
namespace post3dapp{

class CustomDrawVLineItem : public CustomDrawItem
{
public:

    CustomDrawVLineItem(bool _isPlan, const Input2DViewSettings& setting, ELEMENTTYPE type, QPointF p0, const VLINESHAPE &shape,
                        ITEMVIEWTYPE view_type, ELEMENTDIR direct, bool forward = true);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    const QPointF myPosition;
    const VLINESHAPE myLineShape;
    const ITEMVIEWTYPE myItemView;
    const ELEMENTDIR myElementDir;
    const bool isForward;

    virtual void createShapePath() = 0;
    void createDummyShape();
    void createErrorShape();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event)}

};
} // namespace post3dapp
#endif
