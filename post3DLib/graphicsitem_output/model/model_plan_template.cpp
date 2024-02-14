#include "model_plan_template.h"

#include "calc_vector2d.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "manage_springdata.h"
#include "output_jointitem.h"
#include "output_line_item_model.h"
#include "output_lineplaneitem.h"
#include "output_plane_item_model.h"
#include "output_joint_item_model.h"
#include "output_spring_item_model.h"
#include "planOT_grscene.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp{

ModelPlanTemplate::ModelPlanTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    : FigureModelTemplate(_setting, type, uuid)
{
}

void ModelPlanTemplate::createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)
{
    Q_UNUSED(ino);
    qDebug() << "NumericPlanTemplate::createJointItem";
    VEC3D v1(1.0, 0.0, 0.0);
    VEC3D v3(0.0, 0.0, 1.0);
    OutputGraphicsItem *item = new OutputJointItemModel(this, DATATYPE::TPJOINT,STRUCTTYPE::NOSTRUCTTYPE,jd->getUuid(),v3,v1); //,ino,v3,v1,myFrameID);
    item->setDrawingStatusDirect();
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);


    item->setPos(pp);
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}
void ModelPlanTemplate::createLineItem(OutputGrScene* scene, MemberData *md, QList<QPointF> points)
{
    OutputGraphicsItem *item = nullptr;
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();

    if (getOutFigType() == OUTFIGTYPE::MODEL  ) {

        item = new OutputLineItemModel( this,md->dataType(),md->structureType(*uad), md->getUuid(),
                                           points, md->getTotalLength(),  VEC3D(0, 0, 1), VEC3D(1, 0, 0));


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
void ModelPlanTemplate::createPlaneItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points,bool isNormalDirection )
{
    OutputGraphicsItem *item;
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();

    if(md->getPlaneModel()==PLANEMODEL::ZWALL_MODEL)return;
    if ( (getOutFigType() == OUTFIGTYPE::MODEL) && !isNormalDirection) {
//        item = new OutputPlaneItem(this,md->dataType(), md->structureType(*uad),md->getUuid(),points);
        item = new OutputPlaneItemModel(this,md->dataType(), md->structureType(*uad),md->getUuid(), points,
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

void ModelPlanTemplate::createSpringItem(OutputGrScene *scene, SpringData* sd,QList<QPointF> points )
{
    OutputGraphicsItem *item = nullptr;

    if ( item == nullptr ) {
        item = new OutputSpringItemModel(this, DATATYPE::TPSPRING,STRUCTTYPE::NOSTRUCTTYPE, sd->getUuid(), points);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

    item->setPos(points.first());
    item->setZValue(-200.0);
    item->setEnabled(true);
    item->setSelected(false);
    scene->addItem(item);
}

QColor ModelPlanTemplate::nameColor(DATATYPE dataType,STRUCTTYPE material,ATTENTION_STATUS status)
{
    Q_UNUSED(status);
    QColor color;

    if ( dataType == DATATYPE::TPJOINT ) {
        color = getViewSettings()->getJointNumberColor();
    } else {
        return figureColor(dataType,material,status);
    }
    return color;
}

QColor ModelPlanTemplate::zwallColor()
{
    return getViewSettings()->getZwallColor();
}
QColor ModelPlanTemplate::rigidColor()
{
    return getViewSettings()->getRigidRangeColor();
}
QColor ModelPlanTemplate::panelColor()
{
    return getViewSettings()->getPanelZoneColor();
}
QColor ModelPlanTemplate::boundaryTextColor()
{
    return getViewSettings()->getBoundaryConditionColor();
}
int ModelPlanTemplate::boundaryTextSize()
{
    return getViewSettings()->getBoundaryConditionSize();
}
bool ModelPlanTemplate::boundaryTextVisible()
{
    return getViewSettings()->getBoundaryConditionVisible();
}

void ModelPlanTemplate::createJointItems(OutputGrScene* scene){

    // TODO
    Q_FOREACH ( JointData *jd, UnifiedFixedData::getInstance()->getAnalysisData()->jointListOfFloor(uuid) ) {

        if ( !jd->isAnalysisJoint(*UnifiedFixedData::getInstance()->getAnalysisData()) ) continue;

        int ino = UnifiedFixedData::getInstance()->getAnalysisData()->indexOfJoint(jd) + 1;

        QPointF pp = QPointF(jd->xPos() * 1000., jd->yPos() * 1000.);

        createJointItem(scene, jd, ino, pp);

    }
}
void ModelPlanTemplate::createMemberItems(OutputGrScene* scene)
{
    //    Q_FOREACH ( MemberData *md, UnifiedFixedData::getInstance()->getAnalysisData()->memberListOfFloor(uuid) ) {
    qreal eps = 0.00001;
    Q_FOREACH ( MemberData *md, UnifiedFixedData::getInstance()->getAnalysisData()->memberListOfAll() ) {

        if ( !md->isAnalysisMember(*UnifiedFixedData::getInstance()->getAnalysisData()) ) continue;

        QList<JointData *> jdlist = md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getAnalysisData());


        // 線材の場合
        if ( md->isLineMember(false) ) {

            if(md->touchFloor(uuid)){
                //平面上の部材の探索
                QList<QList<QPointF> > hlinePoints;

                bool prev_on;
                bool cur_on = false;
                bool next_on = jdlist.first()->containsFloor(uuid);

                for (int i = 0; i < jdlist.count() - 1; i++) {

                    JointData *jd0 = jdlist.at(i);
                    JointData *jd1 = jdlist.at(i + 1);

                    QPointF p0 = QPointF(jd0->xPos() * 1000., jd0->yPos() * 1000.);
                    QPointF p1 = QPointF(jd1->xPos() * 1000., jd1->yPos() * 1000.);

                    prev_on = cur_on;
                    cur_on = next_on;
                    next_on = jd1->containsFloor(uuid);

                    if ( cur_on ) {
                        if ( next_on ) {
                            if ( prev_on ) {
                                QList<QPointF> poly = hlinePoints.last();
                                QPointF q1 = poly.at( poly.count() - 2 );
                                QPointF q2 = poly.last();
                                q1 = q2 - q1;
                                q2 = p1 - q2;
                                q1 = q1 / CalcVector2D::length(q1);
                                q2 = q2 / CalcVector2D::length(q2);
                                if ( CalcVector2D::dot(q1, q2) > 0.99985 ) {
                                    poly.replace(poly.count() - 1, p1);
                                } else {
                                    poly.append( p1 );
                                }
                                hlinePoints.replace(hlinePoints.count() - 1, poly);
                            } else {
                                QList<QPointF> poly;
                                poly.append( p0 );
                                poly.append( p1 );
                                hlinePoints.append(poly);
                            }
                        }
                    }
                }

                //            if ( hlinePoints.isEmpty() ) continue;
                if(!hlinePoints.isEmpty()){
                    for ( int i = 0; i < hlinePoints.count(); i++ ) {
                        createLineItem(scene, md, hlinePoints.at(i));
                    }
                }
            }

            //平面を横切る部材の探索
            JointData *jd0 = jdlist.first();
            JointData *jd1 = jdlist.last();
            QUuid id_upper = UnifiedFixedData::getInstance()->getAnalysisData()->getUpperFloorID(uuid,1);
            qreal height_fl = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(uuid);
            qreal height_upfl = id_upper == QUuid() ? height_fl : UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(id_upper);
            qreal height_i = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(jd0->getFirstFloor());
            qreal height_j = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(jd1->getFirstFloor());
            if(qAbs(height_i - height_j) < 0.001){continue;}//ij端が同一平面内のとき

            //TODO:高さではなく所属階で判定！！！！
            bool hasLower=false;
            bool hasUpper = false;
            if(height_i<=height_fl+eps || height_j <=height_fl+eps)hasLower = true;
            if(height_fl+eps <height_j || height_fl+eps <height_i)hasUpper = true;

            if(hasUpper && hasLower){
                QPointF p0 = QPointF(jd0->xPos() * 1000., jd0->yPos() * 1000.);
                QPointF p1 = QPointF(jd1->xPos() * 1000., jd1->yPos() * 1000.);
                //クロス判定は階の位置、描画位置は上の階との中点
                qreal t = (0.5*(height_fl+height_upfl) - height_i) / (height_j - height_i);
            }
        }


        // 面材の場合
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

            //面内
            QList<QList<QPointF> > planePoints;
            bool prev_on;
            bool cur_on = false;

            for (int i = 0; i < jdlist.count(); i++) {

                if ( !isOnPlane  ) break;
                JointData *jd = jdlist.at(i);
                prev_on = cur_on;
                cur_on = jd->containsFloor(uuid);

                if ( cur_on ) {
                    if ( prev_on ) {
                        QList<QPointF> poly = planePoints.last();
                        poly.append( QPointF(jd->xPos() * 1000., jd->yPos() * 1000.) );
                        planePoints.replace(planePoints.count() - 1, poly);
                    } else {
                        QList<QPointF> poly;
                        poly.append( QPointF(jd->xPos() * 1000., jd->yPos() * 1000.) );
                        planePoints.append(poly);
                    }
                }else{
                    isOnPlane = false;
                }
            }

            // 始点と終点がONで,Polygonが2つ以上存在する場合,最初と最後のPolygonを結合する
            if ( cur_on && planePoints.count() > 1 ) {
                if ( jdlist.first()->containsFloor(uuid) ) {
                    QList<QPointF> poly = planePoints.last();
                    poly.append( planePoints.first() );
                    planePoints.replace(planePoints.count() - 1, poly);
                    planePoints.removeFirst();
                }
            }


            if(isOnPlane){
                for ( int i = 0; i < planePoints.count(); i++ ) {
                    createPlaneItem(scene, md, planePoints.at(i), false);
                }
            }

            //面外

            bool isCrossPlane = false;
            bool hasUpper = false;
            bool hasLower = false;
            //クロス（含onPlane）する？→NO：continue
            //onPlane？→NO：交線を求めてcreatePlaneLineMemberを返す
            //→YES:createPlaneMemberを返す
            qreal height_fl = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(uuid);
            for(int i=0;i<jdlist.count();i++){
                qreal height = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(jdlist.at(i)->getFirstFloor());

                if(height > height_fl+eps)hasUpper = true;
                if(height <=height_fl+eps)hasLower = true;
            }
            if(hasUpper && hasLower)isCrossPlane = true;


            if(isCrossPlane){
                QUuid id_upper = UnifiedFixedData::getInstance()->getAnalysisData()->getUpperFloorID(uuid,1);
                qreal height_upfl = id_upper == QUuid() ? height_fl : UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(id_upper);
                QList<QPointF> crossPoints;
                bool prev_lower = false;
                bool curr_lower = false;
                for (int i = 0; i < jdlist.count(); i++) {
                    int j=i-1;
                    if(i==0)j=jdlist.count()-1;
                    JointData *jd0 = jdlist.at(j);
                    JointData *jd1 = jdlist.at(i);
                    qreal h0 = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(jd0->getFirstFloor());
                    qreal h1 = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(jd1->getFirstFloor());

                    prev_lower = (h0<=height_fl+eps);
                    curr_lower = (h1<=height_fl+eps);

                    if ( (curr_lower && !prev_lower) || (!curr_lower && prev_lower) ) {
                        QPointF p0 = QPointF(jd0->xPos() * 1000., jd0->yPos() * 1000.);
                        QPointF p1 = QPointF(jd1->xPos() * 1000., jd1->yPos() * 1000.);
                        qreal height_i = jd0->zPos();
                        qreal height_j = jd1->zPos();
                        //クロス判定は階の位置、描画位置は上の階との中点
                        qreal t = (0.5*(height_fl+height_upfl) - height_i) / (height_j - height_i);
                        crossPoints<<(1.0-t)*p0+t*p1;
                    }
                }
                if(crossPoints.count()==2){
                    createPlaneItem(scene, md, crossPoints, true);
                }
            }
        }


    }



}

} // namespace post3dapp
