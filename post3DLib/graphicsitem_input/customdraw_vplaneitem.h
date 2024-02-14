#ifndef CUSTOMDRAWVPLANEITEM_H
#define CUSTOMDRAWVPLANEITEM_H

#include "customdraw_item.h"
namespace post3dapp{
class CustomDrawVPlaneItem : public CustomDrawItem
{
public:
    CustomDrawVPlaneItem(bool _isPlan, const Input2DViewSettings& setting,ELEMENTTYPE, const QList<QPointF> &, const QString &, bool,
                          qreal thickness, qreal shift_z);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool approximatelyContains(QPointF pp) const;
    bool isOnLinePoint(QPointF pp, bool exceptPoint = false) const;

protected:
    const bool isOnPlane;
    const qreal myThickness;
    const qreal shiftZ;
    const ELEMENTDIR myElementDir;

    QList<QPointF> myPointList;
    QList<qreal> myLenList;
    QRectF appRect;


    virtual void createShapePath() = 0;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {Q_UNUSED(event)}
    void setDrawShape(QPainterPath&, qreal, qreal);
    void setTextProperty(qreal, qreal);
};
} // namespace post3dapp
#endif
