#include "react_elevation_template.h"
#include "output_grscene.h"
#include "output_joint_item_react.h"
#include "fixed_data.h"

namespace post3dapp{

ReactElevationTemplate::ReactElevationTemplate(OUTFIGTYPE type, QUuid uuid,bool display):FigureTemplateElevation(type, uuid,display)
{

}
void ReactElevationTemplate::createJointItem(OutputGrScene *scene,OUTFIG_TYPE_OPTION option,JointData *jd, int ino, const QPointF &pp)
{
    qDebug() << "makeJointItem";
    qreal fr_angle = UnifiedAnalysisData::getOutInstance()->globalXYZToFrameAngle(uuid, jd->xyz());
    VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
    VEC3D v2(0.0, 0.0, 1.0);
    VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
    OutputGraphicsItem *item = new OutputJointItemReact(this, DATATYPE::TPJOINT,STRUCTTYPE::NOSTRUCTTYPE,jd->getUuid(),v3,v1); //,ino,v3,v1,myFrameID);
    item->setDrawingStatusDirect(option);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);


    item->setPos(pp);
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
    scene->createdItems.append(item);
    FRAMETYPE f_type = UnifiedFixedData::getInstance()->getAnalysisData()->getFrameTypeOfFloor(uuid);
    bool isClosedFrame = ( f_type == FRAMETYPE::CLCIRCLE || f_type == FRAMETYPE::CLVARIABLE ) ;

    if ( isClosedFrame && qAbs(pp.x()) < 1.0e-3 ) { // 閉じた通り
        qreal cx = UnifiedFixedData::getInstance()->getAnalysisData()->globalZToClosedFrameEndX(uuid, jd->zPos());

        qreal fr_angle = UnifiedAnalysisData::getOutInstance()->globalXYZToFrameAngle(uuid, jd->xyz());
        VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
        VEC3D v2(0.0, 0.0, 1.0);
        VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
        OutputGraphicsItem *item = new OutputJointItem(this, DATATYPE::TPJOINT, STRUCTTYPE::NOSTRUCTTYPE,jd->getUuid(),v3,v1);
        //item->setFlag(QGraphicsItem::ItemIsSelectable, false);
        item->setPos( QPointF(cx * 1000.0, pp.y()));
        item->setZValue(-100.0);
        item->setEnabled(true);
        item->setSelected(false);
        scene->addItem(item);
        scene->createdItems.append(item);
    }

}

} // namespace post3dapp
