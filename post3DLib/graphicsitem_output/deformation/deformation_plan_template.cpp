#include "deformation_plan_template.h"

#include "fixed_data.h"
#include "manage_memberdata.h"
#include "output_grscene.h"
#include "output_line_item_deformation.h"
#include "unified_analysisdata.h"

namespace post3dapp{

DeformationPlanTemplate::DeformationPlanTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    : FigureCommonTemplatePlan(_setting, type, uuid)
{
}
QColor DeformationPlanTemplate::figureColor(DATATYPE dataType,STRUCTTYPE material,ATTENTION_STATUS status) const
{
    Q_UNUSED(dataType);
    Q_UNUSED(material);

    if(status == ATTENTION_STATUS::STATUS_NORMAL){
        return getViewSettings()->getBeforeColor();
    }else{
        return getViewSettings()->getAfterColor();
    }
}
void DeformationPlanTemplate::createLineItem(OutputGrScene* scene, MemberData *md, QList<QPointF> points)
{
    OutputGraphicsItem *item = nullptr;

    if ( getOutFigType() == OUTFIGTYPE::DEFORM ) {
        VEC3D y_v = md->getVVector();
        VEC3D z_v = md->getWVector();
        qreal yt1 = CalcVector3D::dot(y_v, VEC3D(0.0, 0.0, 1.0));
        qreal zt1 = CalcVector3D::dot(z_v, VEC3D(0.0, 0.0, 1.0));
        bool isZin = ( qAbs(yt1) > qAbs(zt1) ); // 部材z軸を面内とする
        bool isClockWize = (yt1 > 0);

        item = new OutputLineItemDeformation(this,md->dataType(),md->structureType(*UnifiedFixedData::getInstance()->getAnalysisData()), md->getUuid(), points,isZin,isClockWize,VEC3D(0.0,0.0,1.0),VEC3D(1.0,0.0,0.0), true);
        item->setDrawingStatusDirect();
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
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
