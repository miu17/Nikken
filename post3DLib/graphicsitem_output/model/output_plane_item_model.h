#ifndef OUTPUT_PLANE_ITEM_MODEL_H
#define OUTPUT_PLANE_ITEM_MODEL_H

#include "output_planeitem.h"

namespace post3dapp{
struct MRESULT;

class OutputPlaneItemModel : public OutputPlaneItem
{

public:
    OutputPlaneItemModel(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, MRESULT *mresult);


protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override{};

    virtual void createPlaneShapePath()override;
private:
    MRESULT *myResult;
 };
} // namespace post3dapp

#endif // OUTPUT_PLANE_ITEM_MODEL_H
