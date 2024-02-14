#ifndef OUTPUT_LINE_ITEM_MODEL_H
#define OUTPUT_LINE_ITEM_MODEL_H

#include "output_lineitem.h"

namespace post3dapp{

class OutputLineItemModel : public OutputLineItem
{
public:
    OutputLineItemModel(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, qreal tlen, VEC3D mypvec,
                         VEC3D mypxvec);

    void setDrawingStatusDirect() override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override{};
    qreal totalLength;

    private:
    void hatch(QPainterPath& path,QPointF p1,QPointF p2,qreal hh,qreal ang,int maxdiv);
    void baneHatch(QPainterPath& path,QPointF p1,QPointF p2,qreal hh,qreal ang,int maxdiv);
    void createWallPath();
    void createRigidPath();
    void createFacePath();
//    void createFaceBanePath();
    void createMemberBanePath();
    void createSectionName();
    void createMemberNumber();
    void createMemberDirection();

};
} // namespace post3dapp

#endif // OUTPUT_LINE_ITEM_MODEL_H
