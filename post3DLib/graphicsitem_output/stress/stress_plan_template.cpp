#include "stress_plan_template.h"

#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "output_joint_item_stress.h"
#include "output_line_item_stress.h"
#include "output_lineplaneitem.h"
#include "output_plane_item_stress.h"
#include "planOT_grscene.h"
#include "unified_analysisdata.h"
#include "unified_data.h"
#include "unified_output.h"

namespace post3dapp{

StressPlanTemplate::StressPlanTemplate(const Analysis2DViewSettings& _settings, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    : FigureCommonTemplatePlan(_settings, type, uuid)
{
}


void StressPlanTemplate::createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)
{
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();
    QUuid myFrameID = scene->getFrameFloorId();

    qDebug() << "makeJointItem";
    OutputGraphicsItem *item = NULL;

    //        bool isFace = (getOutFigType() == OUTFIGTYPE::FSTRESS) ? true : false;
    qreal fr_angle = uad->globalXYZToFrameAngle(myFrameID, jd->xyz());
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

}

void StressPlanTemplate::createLineItem(OutputGrScene* scene, MemberData *md, QList<QPointF> points)
{
    qDebug() << "makeLineItem";
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();
    UnifiedOutput* uo = UnifiedFixedData::getInstance()->getPstnData();

    OutputGraphicsItem *item = nullptr;

    if (getOutFigType() == OUTFIGTYPE::NSTRESS || getOutFigType() == OUTFIGTYPE::FSTRESS|| getOutFigType() == OUTFIGTYPE::RSTRESS
            || getOutFigType() == OUTFIGTYPE::CMQ||getOutFigType() == OUTFIGTYPE::RANK  ) {
        VEC3D y_v = md->getVVector();
        VEC3D z_v = md->getWVector();
        qreal yt1 = CalcVector3D::dot(y_v, VEC3D(0.0, 0.0, 1.0));
        qreal zt1 = CalcVector3D::dot(z_v, VEC3D(0.0, 0.0, 1.0));
        bool isZin = ( qAbs(yt1) > qAbs(zt1) ); // 部材z軸を面内とする
        bool isClockWize = (yt1 > 0);



        item = new OutputLineItemStress( this,md->dataType(),md->structureType(*uad), md->getUuid(), md->sectionName(),
                                         points, md->getTotalLength(), isZin, isClockWize,
                                         uo->resultOfMember(md->getUuid() ), VEC3D(0, 0, 1), false);
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

    scene->addItem(item);
}

void StressPlanTemplate::createPlaneItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points,bool isNormalDirection )
{
    //平面応力要素の描画未実装
    OutputGraphicsItem *item;
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();
    UnifiedOutput* uo = UnifiedFixedData::getInstance()->getPstnData();

    if(md->getPlaneModel()==PLANEMODEL::ZWALL_MODEL)return;
    if ( (getOutFigType() == OUTFIGTYPE::NSTRESS || getOutFigType() == OUTFIGTYPE::FSTRESS|| getOutFigType() == OUTFIGTYPE::RSTRESS) && !isNormalDirection) {
        item = new OutputPlaneItemStress(this,md->dataType(), md->structureType(*uad),md->getUuid(), points,
                                         uo->resultOfMember(md->getUuid() ));
        item->setDrawingStatusDirect();
    } else if ( !isNormalDirection ) {
        item = new OutputPlaneItem(this,md->dataType(), md->structureType(*uad),md->getUuid(),points);
    }else{return;}

    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setPos(points.first());
    item->setZValue(-200.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}

qreal StressPlanTemplate::diagramLoadPerMeter()
{
    return getViewSettings()->getStressScale();
}
QColor StressPlanTemplate::diagramColor()
{
    return getViewSettings()->getStressColor();
}

QColor StressPlanTemplate::valueTextColor(DATATYPE dataType,ATTENTION_STATUS status) const
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

bool StressPlanTemplate::figureVisible(DATATYPE type) const
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

} // namespace post3dapp
