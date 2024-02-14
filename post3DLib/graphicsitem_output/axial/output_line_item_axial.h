#ifndef OUTPUTLINEITEMAXIAL_H
#define OUTPUTLINEITEMAXIAL_H
#include "output_lineitem.h"

namespace post3dapp{

class OutputLineItemAxial : public OutputLineItem
{
public:
    OutputLineItemAxial(FigureCommonTemplate* fig,DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, bool shape_on = true);
    void setDrawingStatusDirect() override;
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event)};
    void createAxialShapePath();
//    virtual void drawOTShapePath(QPainter*)override;
    virtual QColor itemColor(ATTENTION_STATUS)override;

private:
    qreal axialForce;
    qreal shapeWidth;

};
} // namespace post3dapp

#endif // OUTPUTLINEITEMAXIAL_H
