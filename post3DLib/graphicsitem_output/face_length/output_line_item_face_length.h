#ifndef OUTPUTLINEITEMFACELENGTH_H
#define OUTPUTLINEITEMFACELENGTH_H

#include "output_lineitem.h"

namespace post3dapp{

class OutputLineItemFaceLength : public OutputLineItem
{
public:

    OutputLineItemFaceLength(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, const VEC3D pvec,bool shape_on = true);

protected:

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event);};

protected:

    qreal iface;
    qreal jface;

    void createFacePath();

};
} // namespace post3dapp
#endif // OUTPUTLINEITEMFACELENGTH_H
