#ifndef CALCLOADLINEITEM_H
#define CALCLOADLINEITEM_H

#include "calcloadgraphicsitem.h"
namespace post3dapp{
class CalcLoadLineItem : public CalcLoadGraphicsItem
{
public:

    CalcLoadLineItem(DATATYPE dtype,
                     const QUuid &uid,
                     const CalculationLoad2DViewSettings &setting,
                     const QList<QPointF> &plist,
                     bool shape_on = true );

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

protected:

    QList<QPointF> myPointList;
    QList<qreal> myLenList;

    void createLinePath();

};
} // namespace post3dapp
#endif
