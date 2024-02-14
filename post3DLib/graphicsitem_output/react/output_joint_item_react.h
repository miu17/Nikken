#ifndef OUTPUTJOINTITEMREACT_H
#define OUTPUTJOINTITEMREACT_H
#include "output_jointitem.h"

namespace post3dapp{


class OutputJointItemReact : public OutputJointItem
{
public:
    OutputJointItemReact(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype,
                           const QUuid &uid,VEC3D mypvec,VEC3D myxvec);

    void setDrawingStatusDirect( const OUTFIG_TYPE_OPTION& ) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{};
};

} // namespace post3dapp

#endif // OUTPUTJOINTITEMREACT_H
