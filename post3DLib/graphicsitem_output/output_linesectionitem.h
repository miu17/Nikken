#ifndef OUTPUTLINESECTIONITEM_H
#define OUTPUTLINESECTIONITEM_H

#include "output_graphicsitem.h"

namespace post3dapp {

class OutputLineSectionItem : public OutputGraphicsItem
{

public:

    OutputLineSectionItem(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype,const QUuid &uid);//,QVector<QVector<qreal>> mat1,QVector<QVector<qreal>> mat2);

    virtual void setDrawingStatusDirect() override{};

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *) override{};



};
} // namespace post3dapp

#endif // OUTPUTLINESECTIONITEM_H
