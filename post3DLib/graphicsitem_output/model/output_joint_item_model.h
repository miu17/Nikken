#ifndef OUTPUT_JOINT_ITEM_MODEL_H
#define OUTPUT_JOINT_ITEM_MODEL_H

#include <QPainter>
#include "output_jointitem.h"
#include "unified_analysisdata.h"

namespace post3dapp {

class OutputJointItemModel : public OutputJointItem
{

public:

    OutputJointItemModel(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, VEC3D mypvec, VEC3D myxvec, bool _shape_on=true);
    QPainterPath shape() const override;
    virtual void setDrawingStatusDirect() override;
protected:
//    void createJointShapePath();


private:
    void createPanelPath();
    void createJointNumber();
    void createBoundaryString();

};
} // namespace post3dapp

#endif // OUTPUT_JOINT_ITEM_MODEL_H
