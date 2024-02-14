#include "numeric_plan_template.h"

#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "output_joint_item_numeric.h"
#include "output_line_item_numeric.h"
#include "planOT_grscene.h"
#include "unified_analysisdata.h"

namespace post3dapp{

NumericPlanTemplate::NumericPlanTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    : FigureCommonTemplatePlan(_setting, type, uuid)
{
}

void NumericPlanTemplate::createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)
{
    Q_UNUSED(ino);
    qDebug() << "NumericPlanTemplate::createJointItem";
    VEC3D v1(1.0, 0.0, 0.0);
    VEC3D v3(0.0, 0.0, 1.0);
    OutputGraphicsItem *item = new OutputJointItemNumeric(this, DATATYPE::TPJOINT,STRUCTTYPE::NOSTRUCTTYPE,jd->getUuid(),v3,v1); //,ino,v3,v1,myFrameID);
    item->setDrawingStatusDirect();
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);


    item->setPos(pp);
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}

void NumericPlanTemplate::createLineItem(OutputGrScene* scene, MemberData *md, QList<QPointF> points)
{
    qDebug() << "NumericPlanTemplate::createLineItem()";
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();
    OutputGraphicsItem *item = nullptr;

        VEC3D y_v = md->getVVector();
        VEC3D z_v = md->getWVector();
        qreal yt1 = CalcVector3D::dot(y_v, VEC3D(0.0, 0.0, 1.0));
        qreal zt1 = CalcVector3D::dot(z_v, VEC3D(0.0, 0.0, 1.0));
        bool isZin = ( qAbs(yt1) > qAbs(zt1) ); // 部材z軸を面内とする

        item = new OutputLineItemNumeric(this, md->dataType(),md->structureType(*uad), md->getUuid(), points,isZin, false,true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);


    item->setPos(points.first());
    item->setZValue(-200.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
    qDebug() << "line item created";
}
} // namespace post3dapp
