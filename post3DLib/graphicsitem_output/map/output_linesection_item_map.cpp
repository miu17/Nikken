#include "output_linesection_item_map.h"

#include "map_plan_template.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp{

OutputLineSectionItemMap::OutputLineSectionItemMap(FigureTemplate* fig , DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid)
    : OutputLineSectionItem(fig, dtype,stype, uid){}



void OutputLineSectionItemMap::setDrawingStatusDirect()
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

void OutputLineSectionItemMap::putValue(){
    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();
    QList<STRESS> stress = type.stress.at(0);
    QUuid myId = getUuid();
    QColor stringColor = myFigureTemplate->valueTextColor(DATATYPE::TPJOINT,ATTENTION_STATUS::STATUS_NORMAL);

    QVector<qreal> f(3,0.0);
    QVector<qreal> d(3,0.0);
    QVector<qreal> value(3,0.0);
    QVector<QString> valuestr(3,QString());
    MemberData* md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myId);
    qreal eps = 0.00001;
    QVector<qreal> tot;

    //    DVALMM(1,IM)=RSTMM( 7,IM,ISTR)
    //         &           +(CMQMM( 7,IM,ISTR)-CMQMM( 1,IM,ISTR))*0.5D0
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
    f[0] = Qx1+Qx2+Qx3;
    f[1] = Qy1+Qy2+Qy3;
    f[2] = Qz1+Qz2+Qz3;

    JointData* iJoint = md->iJoint();
    JointData* jJoint = md->jJoint();
    XYZ iDisp = UnifiedFixedData::getInstance()->getPstnData()->getCombinationJointDisp(iJoint->getUuid(),stress);
    XYZ jDisp = UnifiedFixedData::getInstance()->getPstnData()->getCombinationJointDisp(jJoint->getUuid(),stress);
    if(jJoint->zPos() >= iJoint->zPos()){
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
//        if(value[0]>=0)stringColor = static_cast<AxialMapPlanTemplate*>(myFigureTemplate)->valueTextColor(DATATYPE::TPJOINT,ATTENTION_STATUS::STATUS_NORMAL);
//        if(value[0]<0)stringColor = static_cast<AxialMapPlanTemplate*>(myFigureTemplate)->valueTextColor(DATATYPE::TPJOINT,ATTENTION_STATUS::STATUS_WARNING);

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
            value[0]= 10.0*f[0]/d[0];
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

    OTvaluetext.append( POINT_AND_TEXT(QPointF(), 0.0, valuestr[0], stringColor,
                        OFFSETPOINT::OFFSET_RIGHTBOTTOM, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(), 0.0, valuestr[1], stringColor,
                        OFFSETPOINT::OFFSET_RIGHTBOTTOM2, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(), 0.0, valuestr[2], stringColor,
                        OFFSETPOINT::OFFSET_RIGHTBOTTOM3, 999999,valueTextSize) );
    //            }

}
} // namespace post3dapp
