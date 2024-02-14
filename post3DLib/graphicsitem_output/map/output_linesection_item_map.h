#ifndef OUTPUT_LINESECTION_ITEM_MAP_H
#define OUTPUT_LINESECTION_ITEM_MAP_H

#include "output_linesectionitem.h"

namespace post3dapp{
struct STRESS;
struct FIGURE_TYPE_PROPERTY;

class OutputLineSectionItemMap : public OutputLineSectionItem
{
public:
    OutputLineSectionItemMap(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid);

    void setDrawingStatusDirect() override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override{};
    void putValue();

};

} // namespace post3dapp


#endif // OUTPUT_LINESECTION_ITEM_MAP_H
