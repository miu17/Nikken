#ifndef OUTPUT_LINEITEM_H
#define OUTPUT_LINEITEM_H
#include "output_graphicsitem.h"

namespace post3dapp {
enum class DATATYPE;
enum class STRUCTTYPE;
class FigureCommonTemplate;

class OutputLineItem : public OutputGraphicsItem
{
public:

    OutputLineItem(FigureTemplate*, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, bool shape_on = true );

    QVector<qreal> getOutputFaceRigidLength();
protected:

    QList<QPointF> myPointList;
    QList<qreal> myLenList;

    void createLineShapePath();

};
} // namespace post3dapp
#endif // OUTPUT_LINEITEM_H
