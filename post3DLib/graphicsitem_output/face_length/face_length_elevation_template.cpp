#include "face_length_elevation_template.h"

#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "output_grscene.h"
#include "output_line_item_face_length.h"
#include "output_line_item_rigid_length.h"
#include "unified_analysisdata.h"

namespace post3dapp{

FaceLengthElevationTemplate::FaceLengthElevationTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid id)
    : FigureCommonTemplateElevation(_setting, type, id) {}
void FaceLengthElevationTemplate::createLineItem(OutputGrScene* scene, MemberData *md, QList<QPointF> points)
{
    qDebug() << "makeLineItem";
    OutputGraphicsItem *item = nullptr;

   if ( getOutFigType() == OUTFIGTYPE::FACELENGTH ) {
       qreal fr_angle = UnifiedFixedData::getInstance()->getAnalysisData()->globalXYZToFrameAngle(scene->getFrameFloorId(), md->iJoint()->xyz());
       VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
       VEC3D v2(0.0, 0.0, 1.0);
       VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);

        item = new OutputLineItemFaceLength(this, md->dataType(),md->structureType(*UnifiedFixedData::getInstance()->getAnalysisData()), md->getUuid(),  points,v3, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);

    }
   if ( getOutFigType() == OUTFIGTYPE::RIGIDLENGTH ) {
        item = new OutputLineItemRigidLength(this, md->dataType(),md->structureType(*UnifiedFixedData::getInstance()->getAnalysisData()), md->getUuid(),  points, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);

    }

    if ( item == nullptr ) {
        item = new OutputLineItem(this, md->dataType(), md->structureType(*UnifiedFixedData::getInstance()->getAnalysisData()),md->getUuid(), points);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

    item->setPos(points.first());
    item->setZValue(-200.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}
} // namespace post3dapp
