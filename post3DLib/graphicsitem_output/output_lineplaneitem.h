#ifndef OUTPUTLINEPLANEITEM_H
#define OUTPUTLINEPLANEITEM_H

#include "output_lineitem.h"

namespace post3dapp {
enum class DATATYPE;
enum class STRUCTTYPE;

class FigureCommonTemplate;

class OutputLinePlaneItem : public OutputLineItem
{
public:

    OutputLinePlaneItem(FigureTemplate*, DATATYPE, STRUCTTYPE stype, const QUuid&, const QList<QPointF>&);


protected:

    void createLinePlanePath();

};
} // namespace post3dapp

#endif // OUTPUTLINEPLANEITEM_H
