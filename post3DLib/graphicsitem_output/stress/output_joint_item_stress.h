#ifndef OUTPUTJOINTITEMSTRESS_H
#define OUTPUTJOINTITEMSTRESS_H
#include "output_jointitem.h"

namespace post3dapp{

enum class OUTFIGTYPE;
class OutputJointItemStress : public OutputJointItem
{
public:
    OutputJointItemStress(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, int inum, VEC3D mypvec, VEC3D myxvec,
                          QUuid myframe, OUTFIGTYPE );

    void setDrawingStatusDirect() override;
    QPainterPath shape() const override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override{};
};

} // namespace post3dapp

#endif // OUTPUTJOINTITEMSTRESS_H
