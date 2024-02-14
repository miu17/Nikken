#ifndef OUTPUTSPRINGITEMMODEL_H
#define OUTPUTSPRINGITEMMODEL_H
#include "output_springitem.h"

namespace post3dapp {
enum class B3D_DATATYPE;
enum class DATATYPE;
enum class STRUCTTYPE;
class FigureCommonTemplate;

class OutputSpringItemModel : public OutputSpringItem
{
public:

    OutputSpringItemModel(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, bool shape_on = true );


protected:

//    QList<QPointF> myPointList;
//    QList<qreal> myLenList;

//    void createSpringShapePath();
    void createSectionName();

};
} // namespace post3dapp


#endif // OUTPUTSPRINGITEMMODEL_H
