#ifndef OUTPUT_PLANE_ITEM_NUMERIC_H
#define OUTPUT_PLANE_ITEM_NUMERIC_H

#include "output_planeitem.h"

namespace post3dapp{

class OutputPlaneItemNumeric : public OutputPlaneItem
{
public:
    OutputPlaneItemNumeric(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist);

    void setDrawingStatusDirect() override;
//    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
//    void drawOTPath(QPainter *painter) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event);};
    QColor checkColor(qreal checkval);

//    void createStracturalWallPath();
//    void createPlaneShapePath()override{}
private:
//    MRESULT *myResult;
 };
} // namespace post3dapp

#endif // OUTPUT_PLANE_ITEM_Numeric_H
