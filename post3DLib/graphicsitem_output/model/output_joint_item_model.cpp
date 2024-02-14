#include "output_joint_item_model.h"

#include <QBrush>
#include <QDebug>
#include <QPaintEngine>
#include <QPainter>
#include <QStyleOption>
#include <QRectF>

#include "define_npstn.h"
#include "fixed_data.h"
#include "figure_template.h"
#include "manage_jointdata.h"
#include "model_elevation_template.h"
#include "output_jointitem.h"
#include "unified_output.h"

namespace post3dapp {
OutputJointItemModel::OutputJointItemModel( FigureTemplate* fig, DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid , VEC3D mypvec, VEC3D myxvec,bool _shape_on)
    : OutputJointItem( fig, dtype,stype, uid,mypvec,myxvec,_shape_on)
{
}
void OutputJointItemModel::setDrawingStatusDirect(){
    createPanelPath();
    //if(static_cast<ModelElevationTemplate*>(myFigureTemplate)->boundaryVisible())
        createBoundaryString();
    if(myFigureTemplate->nameVisible(myDataType))createJointNumber();
}
QPainterPath OutputJointItemModel::shape() const
{
    QPainterPath path;
    path.addRect(boundRect);
    return path;
}


void OutputJointItemModel::createPanelPath()
{
    ModelElevationTemplate* tp = static_cast<ModelElevationTemplate*>(myFigureTemplate);
    QColor color = tp->panelColor();

    //パネルをモデル化している場合、□描く
    DataNBLD3D_CMDL cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int jno = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    int jntpp = cmdl.jntpp.at(jno)-1;
    if(jntpp < 0)return;
    //フレームのベクトルと比較して、XYZの適切なものを選択
    //描画面の法線と、パネル座標各軸の内積をとって、一番大きいもの
    VEC3D panelXvec(cmdl.evpp.at(jntpp).at(0).at(0),cmdl.evpp.at(jntpp).at(0).at(1),cmdl.evpp.at(jntpp).at(0).at(2));
    VEC3D panelYvec(cmdl.evpp.at(jntpp).at(1).at(0),cmdl.evpp.at(jntpp).at(1).at(1),cmdl.evpp.at(jntpp).at(1).at(2));
    VEC3D panelZvec = CalcVector3D::crossNormalized(panelXvec,panelYvec);
    qreal dotx = CalcVector3D::dot(myPlaneVector,panelXvec);
    qreal doty = CalcVector3D::dot(myPlaneVector,panelYvec);
    qreal dotz = CalcVector3D::dot(myPlaneVector,panelZvec);
    int ipp = 0;
    if(qAbs(dotx) >= qAbs(doty) && qAbs(dotx) >= qAbs(dotz)){
        ipp = cmdl.ipp.at(jntpp).at(1)-1;
    }else if(qAbs(doty) >= qAbs(dotx) && qAbs(doty) >= qAbs(dotz)){
        ipp = cmdl.ipp.at(jntpp).at(2)-1;
    }else{
        ipp = cmdl.ipp.at(jntpp).at(3)-1;
    }
    if(ipp < 0)return;
    qreal dd=cmdl.dsp.at(ipp).at(0)*1000.0;
    qreal bb=cmdl.dsp.at(ipp).at(1)*1000.0;

    QPainterPath pat;
    qreal eps = 100;
    if(bb < eps || dd < eps)return;

    QRect rect=QRect(-bb/2.0,-dd/2.0,bb,dd);
    pat.addRect(rect);
    PATH_AND_PEN pap(pat,QPen(color,0));
    OTpathes.append(pap);
    return;


}
void OutputJointItemModel::createBoundaryString()
{
    ModelElevationTemplate* tp = static_cast<ModelElevationTemplate*>(myFigureTemplate);
    if( !tp->boundaryTextVisible())return;
    QColor color = tp->boundaryTextColor();
    int size = tp->boundaryTextSize();
    //境界条件の数字
    QVector<int> boundary = UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(myUuid)->getBoundary();
    int fix = 0;
    if (boundary.at(0) == IFFIX)fix += 100000;
    if (boundary.at(1) == IFFIX)fix += 10000;
    if (boundary.at(2) == IFFIX)fix += 1000;
    if (boundary.at(3) == IFFIX)fix += 100;
    if (boundary.at(4) == IFFIX)fix += 10;
    if (boundary.at(5) == IFFIX)fix += 1;
    qDebug()<<"OutputJointItemModel::boundary ="<<fix;
    if(fix==0)return;
    //
    POINT_AND_TEXT pat(QPointF(0,0),0.0,QString::number(fix,10).rightJustified(6,'0'),color,OFFSETPOINT::OFFSET_RIGHTBOTTOM,999999.9,size);
    OTvaluetext.append(pat);

}
void OutputJointItemModel::createJointNumber()
{
    ModelElevationTemplate* tp = static_cast<ModelElevationTemplate*>(myFigureTemplate);
    if( !tp->nameVisible(DATATYPE::TPJOINT))return;
    QColor color = tp->nameColor(DATATYPE::TPJOINT,STRUCTTYPE::NOSTRUCTTYPE,ATTENTION_STATUS::STATUS_NORMAL);
    int size = tp->nameTextSize(DATATYPE::TPJOINT);

    //節点番号・自由度のインデックス
    DataNBLD3D_CMDL cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int jno = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    int jntnm = cmdl.jntnm.at(jno);//節点名（番号）
    int jrgdf=cmdl.jrgdf.at(jno);//所属剛床番号（０のとき非剛床）
    POINT_AND_TEXT pat(QPointF(0,0),0.0,QString::number(jntnm,10),color,OFFSETPOINT::OFFSET_RIGHTTOP,999999.9,size);
    OTvaluetext.append(pat);

    if(jrgdf > 0){
        POINT_AND_TEXT pat(QPointF(0,0),0.0,u8"("+QString::number(jrgdf,10)+u8")",color,OFFSETPOINT::OFFSET_RIGHTBOTTOM,999999.9,size);
        OTvaluetext.append(pat);
    }


}
//void OutputJointItemModel::createBoundaryPath()
//{
//    OutputJointItem::createBoundaryPath（）；
//}
//void OutputJointItemModel::createSpringPath(){
//    OutputJointItem::createSpringPath();

//}
}
