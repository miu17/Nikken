#include "output_joint_item_stress.h"

#include "fixed_data.h"
#include "manage_jointdata.h"
#include "stress_elevation_template.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp{

OutputJointItemStress::OutputJointItemStress(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype,const QUuid &uid, int inum, VEC3D mypvec, VEC3D myxvec,
                                             QUuid myframe,OUTFIGTYPE type)
    : OutputJointItem( fig, dtype,stype, uid ,mypvec,myxvec)
{

    Q_UNUSED(type);
    Q_UNUSED(myframe);
    Q_UNUSED(inum);

}



void OutputJointItemStress::setDrawingStatusDirect()
{
    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();
    QColor springColor = myFigureTemplate->valueTextColor(DATATYPE::TPSPRING,ATTENTION_STATUS::STATUS_NORMAL);
    int stringSize = myFigureTemplate->valueTextSize(dataType());
    bool springVisible = myFigureTemplate->figureVisible(DATATYPE::TPSPRING);

    QList<qreal> reaction = UnifiedFixedData::getInstance()->getPstnData()->getCombinationReaction(myUuid, type.stress.at(0));

    bool isPlan = false;
    if (qAbs(CalcVector3D::dot(myPlaneVector, VEC3D(0, 0, 1)) > 0.9))isPlan = true;

    //z-fixかつ軸組図だったらz-反力を描画
    QVector<int> boundary = UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(myUuid)->getBoundary();
    if (boundary.at(2) == IFFIX && !isPlan) {
        if(!isZero(reaction.at(2))){
        OTvaluetext.append( POINT_AND_TEXT(QPointF(-45, 0), 0, QString::number(reaction.at(2), 'f', 0),
                                           springColor, OFFSETPOINT::OFFSET_RIGHTBOTTOM2, 999999,stringSize) );
        }
    }
    //軸ばねがあり、紙面垂直でなかった場合数値を描画
    //    QList<JOINTSPRING*> springs=UnifiedAnalysisData::getOutInstance()->jointIDToPointer(myUuid)->getJointSpring();


    //    if(!reaction.isEmpty()){
    if (springVisible) {
        QList<JOINTSPRING *> springs = UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(
                    myUuid)->getJointSpring();

        for (int iii = 0; iii < springs.count(); iii++) {
            JOINTSPRING *spring = springs.at(iii);
            VEC3D vn = CalcVector3D::normalize(myPlaneVector);
            VEC3D vb = CalcVector3D::normalize(spring->springVec);
            if ((!spring->isRotation) && qAbs(CalcVector3D::dot(vn, vb)) < 0.1) {

                VEC3D v1 = myPlaneXVector;
                VEC3D v3 = myPlaneVector;
                VEC3D v2 = CalcVector3D::crossNormalized(v3, v1);
                VEC3D baneXY = VEC3D(CalcVector3D::dot(v1, spring->springVec), CalcVector3D::dot(v2,
                                                                                                 spring->springVec));

                qreal springvalue = 0.0;
                for (int load = 0; load < type.stress.at(0).count(); load++) {
                    springvalue += type.stress.at(0).at(load).coef * UnifiedFixedData::getInstance()->getPstnData()->getSpringValue(spring->uid,
                                                                                                             type.stress.at(0).at(load).rst, type.stress.at(0).at(load).getStep());
                }
                springvalue *= type.stress.at(0).first().coef;
                if(!isZero(springvalue)){
                OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), atan2(baneXY.y, baneXY.x) + M_PI,
                                                   QString::number(springvalue, 'f', 0), springColor, OFFSETPOINT::OFFSET_RIGHTBOTTOM2, 999999,stringSize) );
                }


            }

        }
    }

}
QPainterPath OutputJointItemStress::shape() const
{
    QPainterPath path;
    path.addRect(boundRect);
    return path;
}

} // namespace post3dapp
