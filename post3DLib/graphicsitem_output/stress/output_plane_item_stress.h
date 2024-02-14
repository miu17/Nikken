#ifndef OUTPUTPLANEITEMSTRESS_H
#define OUTPUTPLANEITEMSTRESS_H
#include "output_planeitem.h"

namespace post3dapp{
struct MRESULT;

class OutputPlaneItemStress : public OutputPlaneItem
{
public:
    OutputPlaneItemStress(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, MRESULT *mresult);

    void setDrawingStatusDirect() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override{};

private:
    MRESULT *myResult;
 };
} // namespace post3dapp

#endif // OUTPUTPLANEITEMSTRESS_H
