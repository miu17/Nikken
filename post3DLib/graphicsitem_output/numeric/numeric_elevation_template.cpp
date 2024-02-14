#include "numeric_elevation_template.h"

#include "elevationOT_grscene.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "output_joint_item_numeric.h"
#include "output_line_item_numeric.h"
#include "output_plane_item_numeric.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp
{

    NumericElevationTemplate::NumericElevationTemplate(const Analysis2DViewSettings &_setting, const FIGURE_TYPE_PROPERTY &type, QUuid uuid)
        : FigureCommonTemplateElevation(_setting, type, uuid)
    {
    }
    void NumericElevationTemplate::preCreateItems(OutputGrScene *scene)
    {
        FigureCommonTemplate::preCreateItems(scene);
        QUuid myFrameID = scene->getFrameFloorId();

        lineMembersAroundPlane.clear();
        Q_FOREACH (MemberData *md, UnifiedFixedData::getInstance()->getAnalysisData()->memberListOfFrame(myFrameID))
        {
            if (!md->isAnalysisMember(*UnifiedFixedData::getInstance()->getAnalysisData()))
                continue;
            if (md->getPlaneModel() == PLANEMODEL::ZWALL_MODEL)
                continue;
            QList<JointData *> jdlist = md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getAnalysisData());

            if (md->isClosedMember(false))
            {
                bool isOnPlane = false;
                XYZ a1 = jdlist.first()->xyz();
                VEC3D vv(0.0, 0.0, 0.0);
                for (int i = 1; i < jdlist.count() - 1; i++)
                {
                    XYZ a2 = jdlist.at(i)->xyz();
                    XYZ a3 = jdlist.at(i + 1)->xyz();
                    vv = CalcVector3D::cross(VEC3D(a2.x - a1.x, a2.y - a1.y, a2.z - a1.z),
                                             VEC3D(a3.x - a1.x, a3.y - a1.y, a3.z - a1.z));
                    if (CalcVector3D::norm(vv) > 1.0e-3)
                    {
                        isOnPlane = true;
                        break;
                    }
                }
                if (!isOnPlane)
                    continue;
                QList<MemberData *> around_member = md->getLineMembersAroundPlane();
                Q_FOREACH (MemberData *amd, around_member)
                {
                    if (!lineMembersAroundPlane.contains(amd))
                        lineMembersAroundPlane.append(amd);
                }
            }
        }
    }
    void NumericElevationTemplate::createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)
    {
        Q_UNUSED(ino);
        UnifiedAnalysisData *uad = UnifiedFixedData::getInstance()->getAnalysisData();

        qDebug() << "makeJointItem";
        qreal fr_angle = uad->globalXYZToFrameAngle(uuid, jd->xyz());
        VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
        VEC3D v2(0.0, 0.0, 1.0);
        VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
        OutputGraphicsItem *item = new OutputJointItemNumeric(this, DATATYPE::TPJOINT, STRUCTTYPE::NOSTRUCTTYPE, jd->getUuid(), v3, v1); //,ino,v3,v1,myFrameID);
        item->setDrawingStatusDirect();
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);

        item->setPos(pp);
        item->setZValue(-100.0);
        item->setEnabled(true);
        item->setSelected(false);

        scene->addItem(item);
        FrameType::FRAMETYPE f_type = UnifiedFixedData::getInstance()->getAnalysisData()->getFrameTypeOfFloor(uuid);
        bool isClosedFrame = (f_type == FrameType::FRAMETYPE::CLCIRCLE || f_type == FrameType::FRAMETYPE::CLVARIABLE);

        if (isClosedFrame && qAbs(pp.x()) < 1.0e-3)
        { // 閉じた通り
            qreal cx = UnifiedFixedData::getInstance()->getAnalysisData()->globalZToClosedFrameEndX(uuid, jd->zPos());

            qreal fr_angle = uad->globalXYZToFrameAngle(uuid, jd->xyz());
            VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
            VEC3D v2(0.0, 0.0, 1.0);
            VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
            OutputGraphicsItem *item = new OutputJointItemNumeric(this, DATATYPE::TPJOINT, STRUCTTYPE::NOSTRUCTTYPE, jd->getUuid(), v3, v1);
            // item->setFlag(QGraphicsItem::ItemIsSelectable, false);
            item->setPos(QPointF(cx * 1000.0, pp.y()));
            item->setZValue(-100.0);
            item->setEnabled(true);
            item->setSelected(false);
            scene->addItem(item);
        }
    }

    void NumericElevationTemplate::createLineItem(OutputGrScene *scene, MemberData *md, QList<QPointF> points)
    {
        qDebug() << "makeLineItem";
        //        return;
        UnifiedAnalysisData *uad = UnifiedFixedData::getInstance()->getAnalysisData();
        OutputGraphicsItem *item = nullptr;

        qreal fr_angle = UnifiedFixedData::getInstance()->getAnalysisData()->globalXYZToFrameAngle(uuid, md->iJoint()->xyz());
        VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
        VEC3D v2(0.0, 0.0, 1.0);
        VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
        VEC3D y_v = md->getVVector();
        VEC3D z_v = md->getWVector();
        qreal yt1 = CalcVector3D::dot(y_v, v3);
        qreal zt1 = CalcVector3D::dot(z_v, v3);
        bool isZin = (qAbs(yt1) > qAbs(zt1));

        if (lineMembersAroundPlane.contains(md))
        {
            item = new OutputLineItemNumeric(this, md->dataType(), md->structureType(*uad), md->getUuid(), points, isZin, true, true);
        }
        else
        {
            item = new OutputLineItemNumeric(this, md->dataType(), md->structureType(*uad), md->getUuid(), points, isZin, false, true);
        }

        item->setDrawingStatusDirect();
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);

        item->setPos(points.first());
        item->setZValue(-200.0);
        item->setEnabled(true);
        item->setSelected(false);

        scene->addItem(item);
    }
    void NumericElevationTemplate::createPlaneItem(OutputGrScene *scene, MemberData *md, QList<QPointF> points, bool isNormalDirection)
    {
        //        return;
        OutputGraphicsItem *item;
        UnifiedAnalysisData *uad = UnifiedFixedData::getInstance()->getAnalysisData();

        if (md->getPlaneModel() == PLANEMODEL::ZWALL_MODEL)
            return;

        if (!isNormalDirection)
        {
            item = new OutputPlaneItemNumeric(this, md->dataType(), md->structureType(*uad), md->getUuid(), points);
            item->setDrawingStatusDirect();
        }
        else
        {
            item = new OutputPlaneItem(this, md->dataType(), md->structureType(*uad), md->getUuid(), points);
        }

        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setPos(points.first());
        item->setZValue(-300.0);
        item->setEnabled(true);
        item->setSelected(false);

        scene->addItem(item);
    }

} // namespace post3dapp
