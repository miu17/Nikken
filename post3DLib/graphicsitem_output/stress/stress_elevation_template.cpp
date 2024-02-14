#include "stress_elevation_template.h"

#include "elevationOT_grscene.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "output_joint_item_stress.h"
#include "output_line_item_stress.h"
#include "output_lineplaneitem.h"
#include "output_plane_item_stress.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp{

StressElevationTemplate::StressElevationTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    : FigureCommonTemplateElevation(_setting, type, uuid)
{
}

void StressElevationTemplate::preCreateItems(OutputGrScene* scene)
{
    FigureCommonTemplate::preCreateItems(scene);
    QUuid myFrameID = scene->getFrameFloorId();

    lineMembersAroundPlane.clear();
    Q_FOREACH ( MemberData *md, UnifiedFixedData::getInstance()->getAnalysisData()->memberListOfFrame(myFrameID) ) {
        if ( !md->isAnalysisMember(*UnifiedFixedData::getInstance()->getAnalysisData()) ) continue;
        if(md->getPlaneModel()==PLANEMODEL::ZWALL_MODEL)continue;

        QList<JointData *> jdlist = md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getAnalysisData());

        if ( md->isClosedMember(false) ) {
            bool isOnPlane = false;
            XYZ a1 = jdlist.first()->xyz();
            VEC3D vv(0.0, 0.0, 0.0);
            for ( int i = 1; i < jdlist.count() - 1; i++ ) {
                XYZ a2 = jdlist.at(i)->xyz();
                XYZ a3 = jdlist.at(i + 1)->xyz();
                vv = CalcVector3D::cross(VEC3D(a2.x - a1.x, a2.y - a1.y, a2.z - a1.z),
                                         VEC3D(a3.x - a1.x, a3.y - a1.y, a3.z - a1.z));
                if ( CalcVector3D::norm(vv) > 1.0e-3 ) {
                    isOnPlane = true;
                    break;
                }
            }
            if (!isOnPlane)continue;
            QList<MemberData *> around_member = md->getLineMembersAroundPlane();
            Q_FOREACH (MemberData *amd, around_member) {
                if (!lineMembersAroundPlane.contains(amd))lineMembersAroundPlane.append(amd);
            }
        }
    }

}
void StressElevationTemplate::createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)
{
//        return;
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();
    QUuid myFrameID = scene->getFrameFloorId();

    OutputGraphicsItem *item = NULL;

    //        bool isFace = (getOutFigType() == OUTFIGTYPE::FSTRESS) ? true : false;
    qreal fr_angle = UnifiedFixedData::getInstance()->getAnalysisData()->globalXYZToFrameAngle(myFrameID, jd->xyz());
    VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
    VEC3D v2(0.0, 0.0, 1.0);
    VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
    item = new OutputJointItemStress(this,DATATYPE::TPJOINT,STRUCTTYPE::NOSTRUCTTYPE, jd->getUuid(), ino, v3, v1, myFrameID, getOutFigType());
    item->setDrawingStatusDirect();
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);


    item->setPos(pp);
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);

    FrameType::FRAMETYPE f_type = uad->getFrameTypeOfFloor(myFrameID);
    bool isClosedFrame = ( f_type == FrameType::FRAMETYPE::CLCIRCLE || f_type == FrameType::FRAMETYPE::CLVARIABLE ) ;

    if ( isClosedFrame && qAbs(pp.x()) < 1.0e-3 ) { // 閉じた通り
        qreal cx = uad->globalZToClosedFrameEndX(myFrameID, jd->zPos());

        qreal fr_angle = uad->globalXYZToFrameAngle(uuid, jd->xyz());
        VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
        VEC3D v2(0.0, 0.0, 1.0);
        VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
        OutputJointItemStress *item = new OutputJointItemStress(this,DATATYPE::TPJOINT,STRUCTTYPE::NOSTRUCTTYPE, jd->getUuid(),ino,v3,v1,myFrameID,getOutFigType());
     //   item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        item->setPos( QPointF(cx * 1000.0, pp.y()));
        item->setZValue(-100.0);
        item->setEnabled(true);
        item->setSelected(false);
        scene->addItem(item);
    }

}

void StressElevationTemplate::createLineItem(OutputGrScene* scene, MemberData *md, QList<QPointF> points)
{
//        return;
    qDebug()<<"StressElevationTemplate::createLineItem in";
    OutputGraphicsItem *item = nullptr;
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();

    if (getOutFigType() == OUTFIGTYPE::NSTRESS || getOutFigType() == OUTFIGTYPE::FSTRESS || getOutFigType() == OUTFIGTYPE::RSTRESS
            || getOutFigType() == OUTFIGTYPE::RANK|| getOutFigType() == OUTFIGTYPE::CMQ  ) {
        qreal fr_angle = UnifiedFixedData::getInstance()->getAnalysisData()->globalXYZToFrameAngle(scene->getFrameFloorId(), md->iJoint()->xyz());
        VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
        VEC3D v2(0.0, 0.0, 1.0);
        VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
        VEC3D y_v = md->getVVector();
        VEC3D z_v = md->getWVector();
        qreal yt1 = CalcVector3D::dot(y_v, v3);
        qreal zt1 = CalcVector3D::dot(z_v, v3);
        bool isZin = ( qAbs(yt1) > qAbs(zt1) );//部材zベクトル方向が面内方向、部材yベクトルが面外方向
        bool isClockWize = (yt1 >= 0);//部材を表から見ているか裏から見ているか
//        if(isZin)isClockWize = (yt1 >= 0);
//        if(!isZin)isClockWize = (zt1 >= 0);

        //bool isFace = (myFigureType == OUTFIG_FSTRESS || myFigureType == OUTFIG_RANK) ? true : false;
        //bool isRank = myFigureType == OUTFIG_RANK;

        if (lineMembersAroundPlane.contains(md)) {
            qDebug()<<"StressElevationTemplate::createLineItem new Item";
            item = new OutputLineItemStress( this,md->dataType(),md->structureType(*uad), md->getUuid(), md->sectionName(),
                                             points, md->getTotalLength(), isZin, isClockWize,
                                             UnifiedFixedData::getInstance()->getPstnData()->resultOfMember(md->getUuid() ), v3, true);

        } else {

            qDebug()<<"StressElevationTemplate::createLineItem new Item aroundPlane";
            item = new OutputLineItemStress( this,md->dataType(),md->structureType(*uad), md->getUuid(), md->sectionName(),
                                             points, md->getTotalLength(), isZin, isClockWize,
                                             UnifiedFixedData::getInstance()->getPstnData()->resultOfMember(md->getUuid() ), v3, false);
        }
        qDebug()<<"StressElevationTemplate::createLineItem setDrawingStatusDirect";
        item->setDrawingStatusDirect();

    }

    if ( item == nullptr ) {
        item = new OutputLineItem(this,md->dataType(),md->structureType(*uad), md->getUuid(), points);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

    item->setPos(points.first());
    item->setZValue(-200.0);
    item->setEnabled(true);
    item->setSelected(false);
    qDebug()<<"StressElevationTemplate::createLineItem addItem";

    scene->addItem(item);
    qDebug()<<"StressElevationTemplate::createLineItem return";
}
void StressElevationTemplate::createPlaneItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points,bool isNormalDirection )
{
//    return;

    OutputGraphicsItem *item;
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();
    if(md->getPlaneModel()==PLANEMODEL::ZWALL_MODEL)return;

    if ( (getOutFigType() == OUTFIGTYPE::NSTRESS || getOutFigType() == OUTFIGTYPE::FSTRESS|| getOutFigType() == OUTFIGTYPE::RSTRESS) && !isNormalDirection) {
//        item = new OutputPlaneItem(this,md->dataType(), md->structureType(*uad),md->getUuid(),points);
        item = new OutputPlaneItemStress(this,md->dataType(), md->structureType(*uad),md->getUuid(), points,
                                         UnifiedFixedData::getInstance()->getPstnData()->resultOfMember(md->getUuid() ));
        item->setDrawingStatusDirect();
    } else if ( !isNormalDirection ) {
        item = new OutputPlaneItem(this,md->dataType(), md->structureType(*uad),md->getUuid(),points);
    } else {
        item = new OutputLinePlaneItem(this,md->dataType(),md->structureType(*uad), md->getUuid(), points);
    }

    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setPos(points.first());
    item->setZValue(-300.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}


qreal StressElevationTemplate::diagramLoadPerMeter()
{
    return getViewSettings()->getStressScale();
}
QColor StressElevationTemplate::diagramColor()
{
    return getViewSettings()->getStressColor();
}
QColor StressElevationTemplate::valueTextColor(DATATYPE dataType,ATTENTION_STATUS status) const
{
    Q_UNUSED(status);

    switch(dataType){
    case DATATYPE::TPCOLUMN:
        return getViewSettings()->getColumnStressValueColor();
    case DATATYPE::TPGIRDER:
        return getViewSettings()->getGirderStressValueColor();
    case DATATYPE::TPBRACE:
        return getViewSettings()->getBraceStressValueColor();
    case DATATYPE::TPSPRING:
        if(status == ATTENTION_STATUS::STATUS_WARNING)return getViewSettings()->getIsoStressValueColor();
        return getViewSettings()->getSpringStressValueColor();
    case DATATYPE::TPWALL:
        return getViewSettings()->getWallStressValueColor();
    case DATATYPE::TPSLAB:
        return getViewSettings()->getSlabStressValueColor();
    case DATATYPE::TPDAMPER:
        return getViewSettings()->getDamperStressValueColor();
    case DATATYPE::TPISO:
        return getViewSettings()->getIsoStressValueColor();
    default:
        return getViewSettings()->getValueColor();
    }

}
bool StressElevationTemplate::figureVisible(DATATYPE type) const
{
    switch(type){
    case DATATYPE::TPCOLUMN:
        return getViewSettings()->getColumnStressVisible();
    case DATATYPE::TPGIRDER:
        return getViewSettings()->getGirderStressVisible();
    case DATATYPE::TPBRACE:
        return getViewSettings()->getBraceStressVisible();
    case DATATYPE::TPSPRING:
        return getViewSettings()->getSpringStressVisible();
    case DATATYPE::TPWALL:
        return getViewSettings()->getWallStressVisible();
    case DATATYPE::TPSLAB:
        return getViewSettings()->getSlabStressVisible();
    case DATATYPE::TPDAMPER:
        return getViewSettings()->geteDamperStressVisible();
    case DATATYPE::TPISO:
        return getViewSettings()->getIsoStressVisible();
    default:
        return true;
    }
}
bool StressElevationTemplate::wallGirderFigureVisible()
{
    return false;
}

} // namespace post3dapp
