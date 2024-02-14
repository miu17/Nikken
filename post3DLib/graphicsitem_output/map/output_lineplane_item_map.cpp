#include "output_lineplane_item_map.h"

#include <QPainter>

#include "calc_vector2d.h"
#include "map_plan_template.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp {

OutputLinePlaneItemMap::OutputLinePlaneItemMap( FigureCommonTemplate* fig, DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid,
                                          const QList<QPointF> &plist )
    : OutputLinePlaneItem( fig, dtype, stype,uid, plist )
{
    createLinePlanePath();
}

void OutputLinePlaneItemMap::setDrawingStatusDirect()
{
    switch(myFigureTemplate->getOutFigType()){
    case OUTFIGTYPE::AXIALMAP:
    case OUTFIGTYPE::SHEARMAP:
    case OUTFIGTYPE::LAYERDEFORMMAP://56
    case OUTFIGTYPE::LAYERSTIFFMAP://57
        putValue();
        break;
    default:
        return;
    }
}

void OutputLinePlaneItemMap::putValue(){
    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();
    QList<STRESS> stress = type.stress.at(0);
    QUuid myId = getUuid();
    QUuid myFloorId = myFigureTemplate->getId();
    QColor stringColor = myFigureTemplate->valueTextColor(DATATYPE::TPJOINT,ATTENTION_STATUS::STATUS_NORMAL);


    QVector<qreal> tot(3,0.0);
    QVector<qreal> f(3,0.0);
    QVector<qreal> d(3,0.0);
    QVector<qreal> value(3,0.0);
    QVector<QString> valuestr(3,QString());
    MemberData* md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myId);
    qreal eps = 0.00000001;

    QList<JointData*> jdlist = md->getJointList();
    if(jdlist.count()<4)return;
    QVector<bool> isUpper(4,false);
    QVector<bool> isOnFloor(4,false);
    qreal floorheight = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(myFloorId);
    QVector<qreal> joint_height(4,0.0);
    for(int i=0;i<4;i++){
        joint_height[i]=UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(jdlist.at(i)->getFirstFloor());
        isUpper[i]=(joint_height[i] > floorheight+eps);
        isOnFloor[i]=(qAbs(joint_height[i] - floorheight)<eps);
    }


    //方向は、EVMEMWの値を保持。UVWベクトルに保管
    MemberForce force=UnifiedFixedData::getInstance()->getPstnData()->getCombinationNodeMemberForce(md->getUuid(),stress);
    QString modelName = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstData().value(stress.at(0).rst).nameam;
    qreal shear = force.getQy(0.5);
    QList<QList<qreal>> tmat = MapPlanTemplate::getTMat_Wall(md,modelName);
    QList<qreal> localForce;localForce<<shear<<force.N<<0.0;//<<force.iMy;
    QList<qreal> globalForce = MapPlanTemplate::getGlobalForce_Wall(tmat,localForce,md);
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
    f[0]= nQx1;//+Qx3;
    f[1]= nQy1;//+Qy3;
    f[2]= nQz1;//+Qy3;

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
    }else{
        d[0]=1000.0*(iDisp.x-jDisp.x);
        d[1]=1000.0*(iDisp.y-jDisp.y);
    }

    switch(myFigureTemplate->getOutFigType()){
    case OUTFIGTYPE::AXIALMAP:
        value[0]=f[2];
        valuestr[0]=QString::number(value[0], 'f', 0);
        break;
    case OUTFIGTYPE::SHEARMAP:
        value[0]=f[0];
        value[1]=f[1];
        tot = static_cast<MapPlanTemplate*>(myFigureTemplate)->getGlobalForceTotal();
        value[2] = tot[0]*tot[0] + tot[1]*tot[1];
        if(qAbs(value[2]) > eps){
            value[2] =  100.0 * qSqrt(f[0]*f[0]+f[1]*f[1])/qSqrt(value[2]);
        }
        valuestr[0]=QString::number(value[0], 'f', 0);
        valuestr[1]=QString::number(value[1], 'f', 0);
        valuestr[2]="("+QString::number(value[2], 'f', 1)+")";
        break;
    case OUTFIGTYPE::LAYERDEFORMMAP://56
            valuestr[0]=QString::number(d[0], 'f', 2);
            valuestr[1]=QString::number(d[1], 'f', 2);
        break;
    case OUTFIGTYPE::LAYERSTIFFMAP://57
        if(qAbs(d[0])>eps){
            value[0]= 10.0*f[0]/d[0];//kN/cm
            valuestr[0]=QString::number(value[0], 'f', 0);
        }
        if(qAbs(d[1])>eps){
            value[1]= 10.0*f[1]/d[1];
            valuestr[1]=QString::number(value[1], 'f', 0);
        }
        break;
    default:
        break;
    }

    QPointF center;
    for(int i=0;i<myPointList.count();i++){
        center += myPointList[i]/myPointList.count();
    }
    center.setY(-center.y());

    OTvaluetext.append( POINT_AND_TEXT(center, 0.0, valuestr[0], stringColor,
                                      OFFSETPOINT::OFFSET_RIGHTBOTTOM, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(center, 0.0, valuestr[1], stringColor,
                                      OFFSETPOINT::OFFSET_RIGHTBOTTOM2, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(center, 0.0, valuestr[2], stringColor,
                                      OFFSETPOINT::OFFSET_RIGHTBOTTOM3, 999999,valueTextSize) );

}
} // namespace post3dapp
