#ifndef CALCLOADJOINTITEM_H
#define CALCLOADJOINTITEM_H

#include "calcloadgraphicsitem.h"
namespace post3dapp{
class CalcLoadJointItem : public CalcLoadGraphicsItem
{

public:

    CalcLoadJointItem( DATATYPE dtype, const QUuid &uid, const CalculationLoad2DViewSettings &setting,int inum );

    QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:

};
} // namespace post3dapp
#endif
