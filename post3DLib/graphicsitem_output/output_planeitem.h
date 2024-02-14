#ifndef OUTPUTPLANEITEM_H
#define OUTPUTPLANEITEM_H

#include "output_graphicsitem.h"

namespace post3dapp {
enum class DATATYPE;
enum class STRUCTTYPE;
class FigureCommonTemplate;

class OutputPlaneItem : public OutputGraphicsItem
{
public:

    OutputPlaneItem(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist );


protected:

    QList<QPointF> myPointList;
    virtual void createPlaneShapePath();
    virtual void drawOTShapePath(QPainter *painter);

};
} // namespace post3dapp

#endif // OUTPUTPLANEITEM_H
