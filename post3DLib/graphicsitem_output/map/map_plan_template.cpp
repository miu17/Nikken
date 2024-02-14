#include "map_plan_template.h"

#include <typeinfo>

#include "define_analysis2dsettings.h"
#include "elevationOT_grscene.h"
#include "fixed_data.h"
#include "input_grscene.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "output_grscene.h"
#include "output_linesection_item_map.h"
#include "output_lineplane_item_map.h"
#include "output_planeitem.h"
#include "output_point_item.h"
#include "output_text_item.h"
#include "planOT_grscene.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp{

MapPlanTemplate::MapPlanTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    : FigureCommonTemplatePlan(_setting, type, uuid)
{
}
void MapPlanTemplate::preCreateItems(OutputGrScene *scene)
{
    qDebug()<<"preCreateItems";


    //せん断力マップの時はせん断力の集計値をここで出す。全節点について描画
    QUuid myFloorId = scene->getFrameFloorId();
    QList<STRESS> stress = option.stress.at(0);
    qreal eps = 0.00001;

    globalForceTotal.clear();
    globalForceTotal<<0.0<<0.0<<0.0;
    globalStiffTotal.clear();
    globalStiffTotal<<0.0<<0.0<<0.0;

    Q_FOREACH ( MemberData *md, UnifiedFixedData::getInstance()->getAnalysisData()->memberListOfAll() ) {

        if ( !md->isAnalysisMember(*UnifiedFixedData::getInstance()->getAnalysisData()) ) continue;

        QList<JointData *> jdlist = md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getAnalysisData());
        // 線材の場合
        if ( md->isLineMember(false) ) {
            //TODO:ばね要素は考慮外

            //平面を横切る部材の探索
            JointData *jd0 = jdlist.first();
            JointData *jd1 = jdlist.last();
            qreal height_fl = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(uuid);
            qreal height_i = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(jd0->getFirstFloor());
            qreal height_j = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(jd1->getFirstFloor());
            if(qAbs(height_i - height_j) < 0.001){continue;}//ij端が同一平面内のとき

            bool hasLower=false;
            bool hasUpper = false;
            if(height_i<=height_fl+eps || height_j <=height_fl+eps)hasLower = true;
            if(height_fl+eps <height_j || height_fl+eps <height_i)hasUpper = true;

            if(hasUpper && hasLower){
                MemberForce force=UnifiedFixedData::getInstance()->getPstnData()->getCombinationNodeMemberForce(md->getUuid(),stress);
                qreal Qx1=CalcVector3D::dot(md->getUVector(),VEC3D(1,0,0))*force.N;
                qreal Qx2=CalcVector3D::dot(md->getVVector(),VEC3D(1,0,0))*force.getQy(0.5);
                qreal Qx3=CalcVector3D::dot(md->getWVector(),VEC3D(1,0,0))*force.getQz(0.5);
                qreal Qy1=CalcVector3D::dot(md->getUVector(),VEC3D(0,1,0))*force.N;
                qreal Qy2=CalcVector3D::dot(md->getVVector(),VEC3D(0,1,0))*force.getQy(0.5);
                qreal Qy3=CalcVector3D::dot(md->getWVector(),VEC3D(0,1,0))*force.getQz(0.5);
                qreal Qz1=CalcVector3D::dot(md->getUVector(),VEC3D(0,0,1))*force.N;
                qreal Qz2=CalcVector3D::dot(md->getVVector(),VEC3D(0,0,1))*force.getQy(0.5);
                qreal Qz3=CalcVector3D::dot(md->getWVector(),VEC3D(0,0,1))*force.getQz(0.5);
                globalForceTotal[0]+= Qx1+Qx2+Qx3;
                globalForceTotal[1]+= Qy1+Qy2+Qy3;
                globalForceTotal[2]+= Qz1+Qz2+Qz3;

                QVector<qreal> d(3,0.0);
                JointData* iJoint = md->iJoint();
                JointData* jJoint = md->jJoint();
                XYZ iDisp = UnifiedFixedData::getInstance()->getPstnData()->getCombinationJointDisp(iJoint->getUuid(),stress);
                XYZ jDisp = UnifiedFixedData::getInstance()->getPstnData()->getCombinationJointDisp(jJoint->getUuid(),stress);
                if(jJoint->zPos() >= iJoint->zPos()){
                    d[0]=1000.0*(jDisp.x-iDisp.x);
                    d[1]=1000.0*(jDisp.y-iDisp.y);
                    d[2]=1000.0*(jDisp.z-iDisp.z);
                }else{
                    d[0]=1000.0*(iDisp.x-jDisp.x);
                    d[1]=1000.0*(iDisp.y-jDisp.y);
                    d[2]=1000.0*(iDisp.z-jDisp.z);
                }
                globalStiffTotal[0]+= 10.0*(Qx1+Qx2+Qx3)/d[0];
                globalStiffTotal[1]+= 10.0*(Qy1+Qy2+Qy3)/d[1];
                globalStiffTotal[2]+= 10.0*(Qz1+Qz2+Qz3)/d[2];

            }
        }else if(md->dataType() == DATATYPE::TPWALL){
            //TODO:平板シェル要素・平面応力要素は考慮外
            if(md->getPlaneSubModel()!=PLANESUBMODEL::SH_MODEL
                   && md->getPlaneSubModel()!=PLANESUBMODEL::SI_MODEL
                    && md->getPlaneSubModel()!=PLANESUBMODEL::WH_MODEL
                    && md->getPlaneSubModel()!=PLANESUBMODEL::WI_MODEL)continue;

            QList<JointData*> jdlist = md->getJointList();
            if(jdlist.count()<4)continue;
            //                    JointData* next;
            //                    JointData* oppo;
            QVector<bool> isUpper(4,false);
            QVector<bool> isOnFloor(4,false);
            qreal floorheight = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(myFloorId);
            QVector<qreal> joint_height(4,0.0);
            for(int i=0;i<4;i++){
                joint_height[i]=UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(jdlist.at(i)->getFirstFloor());
                isUpper[i]=(joint_height[i] > floorheight+eps);
                isOnFloor[i]=(qAbs(joint_height[i] - floorheight)<eps);
            }
            if(isUpper[0] == true && isUpper[1] == true &&isUpper[2] == true &&isUpper[3] == true)continue;
            if(isUpper[0] == false && isUpper[1] == false &&isUpper[2] == false &&isUpper[3] == false)continue;


            //方向は、EVMEMWの値を保持。UVWベクトルに保管
            MemberForce force=UnifiedFixedData::getInstance()->getPstnData()->getCombinationNodeMemberForce(md->getUuid(),stress);
            QString modelName = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstData().value(stress.at(0).rst).nameam;
            qreal shear = force.getQy(0.5);
            QList<QList<qreal>> tmat = getTMat_Wall(md,modelName);
            QList<qreal> localForce;localForce<<shear<<force.N<<0.0;//<<force.iMy;
            QList<qreal> globalForce = getGlobalForce_Wall(tmat,localForce,md);
            qreal Qx1 = 0.0;
            qreal Qy1 = 0.0;
            qreal Qz1 = 0.0;
            for(int i=0;i<4;i++){
                if((isOnFloor[i] || isUpper[i]) && i>=2){
                    Qx1 += globalForce.at(3*i);
                    Qy1 += globalForce.at(3*i+1);
                    Qz1 += globalForce.at(3*i+2);
                }
            }

            //グローバル座標系との内積をとってグローバル方向での集計
            qreal nQx1 = 1*Qx1*CalcVector3D::dot(VEC3D(1,0,0),md->getVVector())
                    + Qy1*CalcVector3D::dot(VEC3D(1,0,0),md->getUVector())
                    + Qz1*CalcVector3D::dot(VEC3D(1,0,0),md->getWVector());
            qreal nQy1 = 1*Qx1*CalcVector3D::dot(VEC3D(0,1,0),md->getVVector())
                    + Qy1*CalcVector3D::dot(VEC3D(0,1,0),md->getUVector())
                    + Qz1*CalcVector3D::dot(VEC3D(0,1,0),md->getWVector());
            qreal nQz1 = -1*Qx1*CalcVector3D::dot(VEC3D(0,0,1),md->getVVector())
                    - Qy1*CalcVector3D::dot(VEC3D(0,0,1),md->getUVector())
                    - Qz1*CalcVector3D::dot(VEC3D(0,0,1),md->getWVector());
            globalForceTotal[0] += nQx1;//+Qx3;
            globalForceTotal[1] += nQy1;//+Qy3;
            globalForceTotal[2] += nQz1;//+Qy3;

            QVector<qreal> d(3,0.0);
            XYZ iDisp,jDisp;
            qreal zPos_i=0.0;
            qreal zPos_j=0.0;
            for(int i=0;i<4;i++){
                JointData* jd = jdlist.at(i);
                XYZ disp = UnifiedFixedData::getInstance()->getPstnData()->getCombinationJointDisp(jd->getUuid(),stress);
                if(i==0 || i==1){iDisp.x += 0.5*disp.x;iDisp.y += 0.5*disp.y;iDisp.z += 0.5*disp.z;zPos_i+=jd->zPos();}
                if(i==2 || i==3){jDisp.x += 0.5*disp.x;jDisp.y += 0.5*disp.y;jDisp.z += 0.5*disp.z;zPos_j+=jd->zPos();}
            }
            if(zPos_j >= zPos_i){
                d[0]=1000.0*(jDisp.x-iDisp.x);
                d[1]=1000.0*(jDisp.y-iDisp.y);
                d[2]=1000.0*(jDisp.z-iDisp.z);
            }else{
                d[0]=1000.0*(iDisp.x-jDisp.x);
                d[1]=1000.0*(iDisp.y-jDisp.y);
                d[2]=1000.0*(iDisp.z-jDisp.z);
            }
            if(qAbs(d[0])>eps)globalStiffTotal[0]+= 10.0*nQx1/d[0];
            if(qAbs(d[1])>eps)globalStiffTotal[1]+= 10.0*nQy1/d[1];
            if(qAbs(d[2])>eps)globalStiffTotal[2]+= 10.0*nQz1/d[2];

        }

    }

    return;
}

void MapPlanTemplate::createLineSectionItem(OutputGrScene *scene,MemberData *md, const QPointF &pp)
{
    qDebug() << "MapPlanTemplate::createLineSectionItem";
    VEC3D v1(1.0, 0.0, 0.0);
    VEC3D v3(0.0, 0.0, 1.0);
    OutputGraphicsItem *item = new OutputLineSectionItemMap(this,DATATYPE::NODATATYPE,STRUCTTYPE::NOSTRUCTTYPE,md->getUuid()); //,ino,v3,v1,myFrameID);
    item->setDrawingStatusDirect();
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);


    item->setPos(pp);
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}
void MapPlanTemplate::createPlaneItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points,bool isNormalDirection )
{
    qDebug()<<"FigureTemplate::createPlaneItem";

    OutputGraphicsItem *item;
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();

    if(md->getPlaneModel()==PLANEMODEL::ZWALL_MODEL)return;
    if ( !isNormalDirection ) {
        item = new OutputPlaneItem(this,md->dataType(), md->structureType(*uad),md->getUuid(),points);
        item->setDrawingStatusDirect();
    } else {
        item = new OutputLinePlaneItemMap(this,md->dataType(),md->structureType(*uad), md->getUuid(), points);
        item->setDrawingStatusDirect();
    }

    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setPos(points.first());
    item->setZValue(-300.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}


void MapPlanTemplate::createTextItems(OutputGrScene *scene)
{

    QString str;
    switch(getFigureTypeProperty().type){
    case OUTFIGTYPE::SHEARMAP:
        str = u8"TOTAL Fx="+QString::number(globalForceTotal[0],'f',0)
                +u8"(kN), TOTAL Fy=:"+QString::number(globalForceTotal[1],'f',0)+u8"(kN)";
        break;
    case OUTFIGTYPE::AXIALMAP:
        str = u8"TOTAL Fz=:"+QString::number(globalForceTotal[2],'f',0)
                +u8"(kN)";
        break;
    case OUTFIGTYPE::LAYERSTIFFMAP:
        str = u8"TOTAL STIFF-X="+QString::number(globalStiffTotal[0],'f',0)
                +u8"(kN/m), TOTAL STIFF-Y=:"+QString::number(globalStiffTotal[1],'f',0)+u8"(kN/m)";
        break;
    default:
        return;
    }

    OutputGraphicsItem *item;

    QPointF pt = scene->itemsBoundingRect().bottomLeft();
    item = new OutputTextItem(this,DATATYPE::NODATATYPE,pt,str);
    item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    item->setPos(QPointF(pt.x(),pt.y()));
    item->setZValue(-300.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}

QList<QList<qreal>> MapPlanTemplate::getTMat_Wall(MemberData* md,QString model){
    qDebug()<<"getTMat";
    QList<QList<qreal>> TMat;
    QList<qreal> row1,row2,row3;

    VEC3D uvec = md->getUVector();
    VEC3D vvec = md->getVVector();
    VEC3D wvec = CalcVector3D::cross(uvec,vvec);
    qDebug()<<uvec.z<<vvec.z<<wvec.z;
    PREADATA* prea = UnifiedFixedData::getInstance()->getPstnData()->getPrea(model,md->getUuid());
    qDebug()<<prea->EIy;
    qreal phiB=prea->phaiB;//梁の部材角
    qreal phiU=prea->phaiU;//梁の部材角
    qreal bmLB=prea->lenBeamB;//梁の長さ
    qreal bmLU=prea->lenBeamU;//梁の長さ

    row1<<0.5*uvec.x<<0.5*vvec.x<<0.5*wvec.x<<0.5*uvec.x<<0.5*vvec.x<<0.5*wvec.x;
    row1<<0.5*uvec.x<<0.5*vvec.x<<0.5*wvec.x<<0.5*uvec.x<<0.5*vvec.x<<0.5*wvec.x;
    row2<<0.5*uvec.y<<0.5*vvec.y<<0.5*wvec.y<<0.5*uvec.y<<0.5*vvec.y<<0.5*wvec.y;
    row2<<0.5*uvec.y<<0.5*vvec.y<<0.5*wvec.y<<0.5*uvec.y<<0.5*vvec.y<<0.5*wvec.y;
    qreal cc = qCos(phiB)/bmLB;
    qreal ss = qSin(phiB)/bmLB;
    row3<<cc*uvec.x-ss*uvec.y<<cc*vvec.x-ss*vvec.y<<cc*wvec.x-ss*wvec.y;
    row3<<-cc*uvec.x+ss*uvec.y<<-cc*vvec.x+ss*vvec.y<<-cc*wvec.x+ss*wvec.y;
    cc = qCos(phiU)/bmLU;
    ss = qSin(phiU)/bmLU;
    row3<<cc*uvec.x-ss*uvec.y<<cc*vvec.x-ss*vvec.y<<cc*wvec.x-ss*wvec.y;
    row3<<-cc*uvec.x+ss*uvec.y<<-cc*vvec.x+ss*vvec.y<<-cc*wvec.x+ss*wvec.y;
    TMat<<row1<<row2<<row3;

    return TMat;
}
QList<qreal> MapPlanTemplate::getGlobalForce_Wall(QList<QList<qreal>> TMat,QList<qreal>localForce,MemberData* md){

    qDebug()<<"getGlobalForce";
    QList<qreal> cornerForce;
    QList<qreal> globalForce;
    //    {globalForce} = [Tmat]-1 * {localForce}
    //UVW軸で4隅に分配
    for(int i=0;i<12;i++){
        cornerForce << TMat[0][i]*localForce[0] + TMat[1][i]*localForce[1] + TMat[2][i]*localForce[2];
    }

    VEC3D xvec = VEC3D(1,0,0);
    VEC3D yvec = VEC3D(0,1,0);
    VEC3D zvec = VEC3D(0,0,1);
    VEC3D uvec = md->getUVector();
    VEC3D vvec = md->getVVector();
    VEC3D wvec = CalcVector3D::cross(uvec,vvec);

    //globalに変換のつもり
    globalForce << cornerForce[0]*CalcVector3D::dot(uvec,xvec)+cornerForce[1]*CalcVector3D::dot(vvec,xvec)+cornerForce[2]*CalcVector3D::dot(wvec,xvec);
    globalForce << cornerForce[0]*CalcVector3D::dot(uvec,yvec)+cornerForce[1]*CalcVector3D::dot(vvec,yvec)+cornerForce[2]*CalcVector3D::dot(wvec,yvec);
    globalForce << cornerForce[0]*CalcVector3D::dot(uvec,zvec)+cornerForce[1]*CalcVector3D::dot(vvec,zvec)+cornerForce[2]*CalcVector3D::dot(wvec,zvec);

    globalForce << cornerForce[3]*CalcVector3D::dot(uvec,xvec)+cornerForce[4]*CalcVector3D::dot(vvec,xvec)+cornerForce[5]*CalcVector3D::dot(wvec,xvec);
    globalForce << cornerForce[3]*CalcVector3D::dot(uvec,yvec)+cornerForce[4]*CalcVector3D::dot(vvec,yvec)+cornerForce[5]*CalcVector3D::dot(wvec,yvec);
    globalForce << cornerForce[3]*CalcVector3D::dot(uvec,zvec)+cornerForce[4]*CalcVector3D::dot(vvec,zvec)+cornerForce[5]*CalcVector3D::dot(wvec,zvec);

    globalForce << cornerForce[6]*CalcVector3D::dot(uvec,xvec)+cornerForce[7]*CalcVector3D::dot(vvec,xvec)+cornerForce[8]*CalcVector3D::dot(wvec,xvec);
    globalForce << cornerForce[6]*CalcVector3D::dot(uvec,yvec)+cornerForce[7]*CalcVector3D::dot(vvec,yvec)+cornerForce[8]*CalcVector3D::dot(wvec,yvec);
    globalForce << cornerForce[6]*CalcVector3D::dot(uvec,zvec)+cornerForce[7]*CalcVector3D::dot(vvec,zvec)+cornerForce[8]*CalcVector3D::dot(wvec,zvec);

    globalForce << cornerForce[9]*CalcVector3D::dot(uvec,xvec)+cornerForce[10]*CalcVector3D::dot(vvec,xvec)+cornerForce[11]*CalcVector3D::dot(wvec,xvec);
    globalForce << cornerForce[9]*CalcVector3D::dot(uvec,yvec)+cornerForce[10]*CalcVector3D::dot(vvec,yvec)+cornerForce[11]*CalcVector3D::dot(wvec,yvec);
    globalForce << cornerForce[9]*CalcVector3D::dot(uvec,zvec)+cornerForce[10]*CalcVector3D::dot(vvec,zvec)+cornerForce[11]*CalcVector3D::dot(wvec,zvec);


    return globalForce;
    //    return cornerForce;
}
} // namespace post3dapp
