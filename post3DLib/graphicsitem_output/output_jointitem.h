#ifndef OUTPUT_JOINTITEM_H
#define OUTPUT_JOINTITEM_H

#include "output_graphicsitem.h"

namespace post3dapp {

class OutputJointItem : public OutputGraphicsItem
{

public:

    OutputJointItem(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, VEC3D mypvec, VEC3D myxvec, bool _shape_on=true);
    QPainterPath shape() const override;

protected:
    void createJointShapePath();


    void createBoundaryPath();
    void createSpringPath();

};
} // namespace post3dapp

#endif // OUTPUT_JOINTITEM_H
