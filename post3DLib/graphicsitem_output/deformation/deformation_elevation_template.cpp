#include "deformation_elevation_template.h"

#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "output_grscene.h"
#include "output_line_item_deformation.h"
#include "unified_analysisdata.h"

namespace post3dapp{

DeformationElevationTemplate::DeformationElevationTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    : FigureCommonTemplateElevation(_setting, type, uuid)
{
}
qreal DeformationElevationTemplate::deformScale()
{
    return getViewSettings()->getDeformScale();
}
QColor DeformationElevationTemplate::figureColor(DATATYPE dataType,STRUCTTYPE material,ATTENTION_STATUS status) const
{
    Q_UNUSED(dataType);
    Q_UNUSED(material);
    if(status == ATTENTION_STATUS::STATUS_NORMAL){
        return getViewSettings()->getBeforeColor();
    }else{
        return getViewSettings()->getAfterColor();
    }
}
void DeformationElevationTemplate::createLineItem(OutputGrScene* scene, MemberData *md, QList<QPointF> points)
{
    qDebug() << "Deformation_makeLineItem";
    OutputGraphicsItem *item = nullptr;

    if ( getOutFigType() == OUTFIGTYPE::DEFORM ) {
        qreal fr_angle = UnifiedFixedData::getInstance()->getAnalysisData()->globalXYZToFrameAngle(scene->getFrameFloorId(), md->iJoint()->xyz());
        VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
        VEC3D v2(0.0, 0.0, 1.0);
        VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
        VEC3D y_v = md->getVVector();
        VEC3D z_v = md->getWVector();
        qreal yt1 = CalcVector3D::dot(y_v, v3);
        qreal zt1 = CalcVector3D::dot(z_v, v3);
        bool isZin = ( qAbs(yt1) > qAbs(zt1) );
        bool isClockWize = (yt1 > 0);

        item = new OutputLineItemDeformation(this,md->dataType(),md->structureType(*UnifiedFixedData::getInstance()->getAnalysisData()), md->getUuid(), points,isZin,isClockWize,v3,v1, true);
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
