#ifndef CALCLOADPLANEITEM_H
#define CALCLOADPLANEITEM_H

#include "calcloadgraphicsitem.h"
namespace post3dapp{
class CalcLoadPlaneItem : public CalcLoadGraphicsItem
{
public:

    CalcLoadPlaneItem(DATATYPE dtype, const QUuid &uid, const CalculationLoad2DViewSettings &setting,
                    const QList<QPointF> &plist );

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

protected:

    QList<QPointF> myPointList;
    QColor planeColor;
    QPainterPath scaledShapePath;
    void createPlanePath();

};
} // namespace post3dapp
#endif
