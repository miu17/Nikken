#ifndef OUTPUTLINEPLANEITEMMAP_H
#define OUTPUTLINEPLANEITEMMAP_H

#include "output_lineplaneitem.h"

namespace post3dapp {
enum class DATATYPE;
enum class STRUCTTYPE;

class FigureCommonTemplate;
struct STRESS;
struct FIGURE_TYPE_PROPERTY;

class OutputLinePlaneItemMap : public OutputLinePlaneItem
{
public:

    OutputLinePlaneItemMap(FigureCommonTemplate*, DATATYPE, STRUCTTYPE stype, const QUuid&, const QList<QPointF>&);


protected:
    void setDrawingStatusDirect();
    void putValue();


};
} // namespace post3dapp

#endif // OUTPUTLINEPLANEITEMMAP_H
