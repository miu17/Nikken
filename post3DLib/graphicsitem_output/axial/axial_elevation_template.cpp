#include "axial_elevation_template.h"

#include "fixed_data.h"
#include "manage_memberdata.h"
#include "output_grscene.h"
#include "output_line_item_axial.h"
#include "unified_analysisdata.h"

namespace post3dapp{

AxialElevationTemplate::AxialElevationTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    : FigureCommonTemplateElevation(_setting, type, uuid)
{
}

void AxialElevationTemplate::createLineItem(OutputGrScene* scene, MemberData *md, QList<QPointF> points)
{
    qDebug() << "axial_makeLineItem";
    OutputGraphicsItem *item = nullptr;

    if ( getOutFigType() == OUTFIGTYPE::AXIAL ) {
        item = new OutputLineItemAxial(this,md->dataType(),md->structureType(*UnifiedFixedData::getInstance()->getAnalysisData()), md->getUuid(), points, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setDrawingStatusDirect();
    }

    if ( item == nullptr ) {
        item = new OutputLineItem(this,md->dataType(), md->structureType(*UnifiedFixedData::getInstance()->getAnalysisData()),md->getUuid(), points);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }


    item->setPos(points.first());
    item->setZValue(-200.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}
} // namespace post3dapp
