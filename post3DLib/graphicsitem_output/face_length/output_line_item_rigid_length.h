#ifndef OUTPUTLINEITEMRIGIDLENGTH_H
#define OUTPUTLINEITEMRIGIDLENGTH_H

#include "output_lineitem.h"

namespace post3dapp{

class OutputLineItemRigidLength : public OutputLineItem
{
public:

    OutputLineItemRigidLength(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, bool shape_on = true);

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event);};

protected:

//    qreal irigid;
//    qreal jrigid;

    void createRigidPath();

};
} // namespace post3dapp
#endif // OUTPUTLINEITEMRIGIDLENGTH_H
