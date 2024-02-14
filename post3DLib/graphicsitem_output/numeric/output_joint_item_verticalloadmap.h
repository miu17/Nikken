#ifndef OUTPUTJOINTITEMVERTICALLOADMAP_H
#define OUTPUTJOINTITEMVERTICALLOADMAP_H

#include "output_joint_item_numeric.h"
#include "verticalloadmap_plan_template.h"

namespace post3dapp{

//class OutputJointItemVerticalLoadMap: public OutputJointItemNumeric
class OutputJointItemVerticalLoadMap: public OutputJointItem
{
public:
    OutputJointItemVerticalLoadMap(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype,
                                   const QUuid &uid, VEC3D mypvec, VEC3D myxvec)
        :OutputJointItem(fig,dtype,stype,uid,mypvec,myxvec){};
//        :OutputJointItemNumeric(fig,dtype,stype,uid,mypvec,myxvec){};
    virtual void setDrawingStatusDirect() override;
private:
    //VerticalLoadMapPlanTemplate* myFigureTemplate;
};
}
#endif // OUTPUTJOINTITEMVERTICALLOADMAP_H
