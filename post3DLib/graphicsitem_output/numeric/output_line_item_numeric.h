#ifndef OUTPUTLINEITEMNUMERIC_H
#define OUTPUTLINEITEMNUMERIC_H
#include "output_lineitem.h"

namespace post3dapp{

class MemberData;

class OutputLineItemNumeric : public OutputLineItem
{
public:
    OutputLineItemNumeric(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid,
                          const QList<QPointF> &plist, bool zin, bool isaroundplane, bool shape_on = true);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event);};
    void layout1();
    void layout2();
    bool isZin;
    bool isAroundPlane;

    QList<MemberData *> iRelatedMember;
    QList<MemberData *> jRelatedMember;

//    QColor textColor();
    QColor checkColor(qreal checkval);
//    qreal checkVal;

private:
};
} // namespace post3dapp

#endif // OUTPUTLINEITEMNUMERIC_H
