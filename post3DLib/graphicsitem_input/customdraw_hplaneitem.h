#ifndef CUSTOMDRAWHPLANEITEM_H
#define CUSTOMDRAWHPLANEITEM_H

#include "customdraw_item.h"
namespace post3dapp{
class CustomDrawHPlaneItem : public CustomDrawItem
{
public:

    CustomDrawHPlaneItem( const Input2DViewSettings& setting,ELEMENTTYPE, const QList<QPointF> &, const QString &, bool,
                          qreal thickness);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool approximatelyContains(QPointF pp) const;
    bool isOnLinePoint(QPointF pp, bool exceptPoint = false) const;

protected:

    virtual void createShapePath() = 0 ;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event)}

protected:

    QList<QPointF> myPointList;
    QRectF appRect;

    bool isOnPlane;
    const qreal myThickness;

};
} // namespace post3dapp
#endif
