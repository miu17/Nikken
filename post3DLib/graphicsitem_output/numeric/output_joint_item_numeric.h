#ifndef OUTPUTJOINTITEMNUMERIC_H
#define OUTPUTJOINTITEMNUMERIC_H

#include "output_jointitem.h"

namespace post3dapp{


class OutputJointItemNumeric : public OutputJointItem
{
public:
    OutputJointItemNumeric(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype,
                           const QUuid &uid, VEC3D mypvec, VEC3D myxvec);

    virtual void setDrawingStatusDirect() override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event);};
};

} // namespace post3dapp

#endif // OUTPUTJOINTITEMNUMERIC_H
