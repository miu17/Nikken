#include "output_line_item_stress.h"

#include "calc_vector2d.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "stress_elevation_template.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp{


OutputLineItemStress::OutputLineItemStress(FigureCommonTemplate* fig, DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid,
                                           const QString &, const QList<QPointF> &plist, qreal tlen,
                                           bool zin, bool isCW, MRESULT *mr, VEC3D mypvec, bool isaroundplane)
    : OutputLineItem(fig, dtype, stype,uid,plist)
{
    myPlaneVector = mypvec; //描画面の法線
    totalLength = tlen;
    isZin = zin;
    isClockWize = isCW;
    isAroundPlane = isaroundplane;
    myResult = mr;
    MemberData *member = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);
    QList<JointData *> jlist = member->getJointList();

    iRelatedMember = jlist.first()->getRelatedMembers();
    jRelatedMember = jlist.last()->getRelatedMembers();


}

void OutputLineItemStress::setDrawingStatusDirect()
{
    qDebug()<<"OutputLineItemStress::setDrawingStatusDirect in";
    //    if(type.type == OUTFIG_RANK)
    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();
    bool isNormalDirection = (type.viewdirection==OUTPUTDIR::ODIR_PLOUT);

    switch(myFigureTemplate->getOutFigType()){
    case OUTFIGTYPE::NSTRESS:
        nodeDiagram(  myUuid, type.stress.at(0),  isNormalDirection );
        break;
    case OUTFIGTYPE::FSTRESS:
        faceDiagram(  myUuid, type.stress.at(0),  isNormalDirection );
        break;
    case OUTFIGTYPE::RSTRESS:
        rigidDiagram(  myUuid, type.stress.at(0),  isNormalDirection );
        break;
    case OUTFIGTYPE::CMQ:
        cmqDiagram(  myUuid, type.stress.at(0),  isNormalDirection );
        break;
    case OUTFIGTYPE::RANK:
        rankDiagram(  myUuid,   isNormalDirection );
        break;
    default:
        break;
    }
    qDebug()<<"OutputLineItemStress::setDrawingStatusDirect out";

    return;

}


void OutputLineItemStress::drawCurve(QList<qreal> vallist,qreal start,qreal end)
{
    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();
    if (CalcVector2D::length(pe - ps) < 0.1)return;

    qreal diagramLoadPerMeter=static_cast<StressElevationTemplate*>(myFigureTemplate)->diagramLoadPerMeter();
    qreal coef = 1000.0/qMax(1.0,diagramLoadPerMeter);
    QColor diagramColor = static_cast<StressElevationTemplate*>(myFigureTemplate)->diagramColor();

    //稜線をつなぐ
    QPainterPath path;

    //ps-peに対して右が正か左が正かによって回転方向を場合分けすること
    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));
    qreal t_ang;
    if (isClockWize)t_ang = p_ang - M_PI / 2.0;
    if (!isClockWize)t_ang = p_ang + M_PI / 2.0;

    ps = QPointF(qCos(p_ang) * start ,
                 qSin(p_ang) * start );
    pe = QPointF(qCos(p_ang) * end ,
                 qSin(p_ang) * end );
    qreal centerLength = (end - start);

    QPointF pm;
    path.moveTo(ps);
    for (int i = 0; i <= div; i++) {
        pm = ps + QPointF(qCos(p_ang) * centerLength * (qreal)i / (qreal)div, qSin(p_ang) * centerLength * (qreal)i / (qreal)div);

        path.lineTo(pm + QPointF(coef*vallist.at(i)*qCos(t_ang), coef*vallist.at(i)*qSin(t_ang)));
    }
    path.lineTo(pe);
    const QPen pen(QBrush(diagramColor), 0);
    OTpathes.append(PATH_AND_PEN(path,pen));


}

VALUEPLOTMAXLENGTH OutputLineItemStress::calcMaxLength()
{
    //隣の部材との距離を見て、数値幅の最大値を調整する
    qDebug() << "set path";
    VALUEPLOTMAXLENGTH ml;
    if (myDataType==DATATYPE::TPCOLUMN) {
        qreal maxdot = -999999;
        qreal mindot = 999999;
        Q_FOREACH (MemberData *anotherMember, iRelatedMember) {
            if (anotherMember->dataType() == DATATYPE::TPGIRDER) {
                //i-jとi-多端の外積と、通り面の内積が正か負かで左か右か判断
                MemberData *member = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
                VEC3D ijvec = member->getUVector();
                VEC3D mdvec;
                if (anotherMember->getJointList().first() == member->getJointList().first()) {
                    mdvec = VEC3D(anotherMember->jXYZ().x - anotherMember->iXYZ().x,
                                  anotherMember->jXYZ().y - anotherMember->iXYZ().y,
                                  anotherMember->jXYZ().z - anotherMember->iXYZ().z);
                } else {
                    mdvec = VEC3D(anotherMember->iXYZ().x - anotherMember->jXYZ().x,
                                  anotherMember->iXYZ().y - anotherMember->jXYZ().y,
                                  anotherMember->iXYZ().z - anotherMember->jXYZ().z);
                }
                VEC3D cross = CalcVector3D::crossNormalized(ijvec, mdvec);
                qreal dot = CalcVector3D::dot(cross, myPlaneVector);
                if (dot > 0 && dot > maxdot)ml.iLeftMaxLength = anotherMember->getTotalLength() / 2.0;
                if (dot < 0 && dot < mindot)ml.iRightMaxLength = anotherMember->getTotalLength() / 2.0;
            }
        }
        Q_FOREACH (MemberData *anotherMember, jRelatedMember) {
            if (anotherMember->dataType() == DATATYPE::TPGIRDER) {
                //j-iとj-他端の外積と、通り面の内積が正か負かで左か右か判断
                MemberData *member = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
                VEC3D jivec = member->getUVector();
                jivec = VEC3D(-jivec.x, -jivec.y, -jivec.z);
                VEC3D mdvec;
                if (anotherMember->getJointList().first() == member->getJointList().last()) {
                    mdvec = VEC3D(anotherMember->jXYZ().x - anotherMember->iXYZ().x,
                                  anotherMember->jXYZ().y - anotherMember->iXYZ().y,
                                  anotherMember->jXYZ().z - anotherMember->iXYZ().z);
                } else {
                    mdvec = VEC3D(anotherMember->iXYZ().x - anotherMember->jXYZ().x,
                                  anotherMember->iXYZ().y - anotherMember->jXYZ().y,
                                  anotherMember->iXYZ().z - anotherMember->jXYZ().z);
                }
                VEC3D cross = CalcVector3D::crossNormalized(jivec, mdvec);
                qreal dot = CalcVector3D::dot(cross, myPlaneVector);
                if (dot > 0 && dot > maxdot)ml.jRightMaxLength = anotherMember->getTotalLength() / 2.0;
                if (dot < 0 && dot < mindot)ml.jLeftMaxLength = anotherMember->getTotalLength() / 2.0;
            }
        }

    }
    if (myDataType==DATATYPE::TPCOLUMN) {
        ml.cRightMaxLength = (ml.iRightMaxLength + ml.jRightMaxLength) / 2.0;
        ml.cLeftMaxLength = (ml.iLeftMaxLength + ml.jLeftMaxLength) / 2.0;
    }
    if (myDataType==DATATYPE::TPGIRDER)ml.maxLength = totalLength / 2.0;
    return ml;
}

void OutputLineItemStress::plotValues(QVector<qreal> values)
{
    bool q_visible = static_cast<StressElevationTemplate*>(myFigureTemplate)->qVisible();
    bool m_visible = static_cast<StressElevationTemplate*>(myFigureTemplate)->mVisible();
    bool n_visible = static_cast<StressElevationTemplate*>(myFigureTemplate)->nVisible();

    QColor textColor=myFigureTemplate->valueTextColor(myDataType,ATTENTION_STATUS::STATUS_NORMAL);
    int stringSize=valueTextSize;
    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();
    if (CalcVector2D::length(pe - ps) < 0.1)return;
    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));

    if(values.count() < 7)return;

    if (isClockWize){
        values[0]=-values[0];
        values[1]=-values[1];
        values[2]=-values[2];
    }

    VALUEPLOTMAXLENGTH maxlen = calcMaxLength();


    qreal angle;
    QVector<QPointF> point;
    QVector<OFFSETPOINT> offsetpoint;
    QVector<qreal> len;
    QVector<int> index;
    QVector<bool> visible;

    QPointF iPoint=QPointF(ps.x(), -ps.y());
    QPointF cPoint=QPointF(0.5 * ps.x() + 0.5 * pe.x(),-0.5 * ps.y() - 0.5 * pe.y());
    QPointF jPoint=QPointF(pe.x(), -pe.y());

    if (myDataType==DATATYPE::TPCOLUMN && myFigureTemplate->figureVisible(DATATYPE::TPCOLUMN)) {
        angle = p_ang - M_PI/2.0;
        index << 0<<2<<4<<6;
        offsetpoint << OFFSETPOINT::OFFSET_RIGHTTOP  << OFFSETPOINT::OFFSET_LEFTBOTTOM<< OFFSETPOINT::OFFSET_RIGHTMIDDLE<<OFFSETPOINT::OFFSET_LEFTMIDDLE;
        len << maxlen.iRightMaxLength  << maxlen.jLeftMaxLength << maxlen.cRightMaxLength <<maxlen.cLeftMaxLength;
        point<<iPoint<<jPoint<<cPoint<<cPoint;
        visible<<m_visible<<m_visible<<q_visible<<n_visible;
    }else if(myDataType==DATATYPE::TPGIRDER && myFigureTemplate->figureVisible(DATATYPE::TPGIRDER)){
        bool wallgirderVisible = static_cast<StressElevationTemplate*>(myFigureTemplate)->wallGirderFigureVisible();
        if (isAroundPlane && !wallgirderVisible)return;
        angle = p_ang ;
        index << 0<<1<<2<<3<<5<<6;
        offsetpoint << OFFSETPOINT::OFFSET_RIGHTBOTTOM << OFFSETPOINT::OFFSET_CENTERBOTTOM << OFFSETPOINT::OFFSET_LEFTTOP
                    << OFFSETPOINT::OFFSET_RIGHTTOP2<< OFFSETPOINT::OFFSET_LEFTTOP2
                    <<OFFSETPOINT::OFFSET_CENTERTOP;
        len << maxlen.maxLength << maxlen.maxLength << maxlen.maxLength
            <<maxlen.maxLength <<  maxlen.maxLength
           <<maxlen.maxLength;
        point<<iPoint<<cPoint<<jPoint<<iPoint<<jPoint<<cPoint;
        visible<<m_visible<<m_visible<<m_visible<<q_visible<<q_visible<<n_visible;
    } else if (myDataType==DATATYPE::TPBRACE && myFigureTemplate->figureVisible(DATATYPE::TPBRACE)) {
        angle = p_ang;
        index <<6;
        offsetpoint <<OFFSETPOINT::OFFSET_CENTERTOP;
        len << maxlen.maxLength;
        point<<cPoint;
        visible<<n_visible;
    }
    for(int i=0;i<index.count();i++){
        QString str = QString::number(values[index[i]], 'f', 0);
        if(!isZero(values[index[i]]) && visible[i]) OTvaluetext.append(POINT_AND_TEXT(point[i],angle,str,textColor,offsetpoint[i],len[i],stringSize));
    }

}

void OutputLineItemStress::nodeDiagram(const QUuid &uid,
                                       const QList<STRESS> &stress,  bool isNormalDirection )
{
    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);
    MemberForce mf = UnifiedFixedData::getInstance()->getPstnData()->getCombinationNodeMemberForce(uid, stress);


    //values
    QVector<qreal> values(7,0.0);
    MemberForceByIL mfil_i = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0,0);
    MemberForceByIL mfil_c = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0,0.5*md->getTotalLength());
    MemberForceByIL mfil_j = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0,md->getTotalLength());
    if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ){
        values[0] = -mf.getMy(0.0)-mfil_i.getMy();
        values[1] = -mf.getMy(0.5)-mfil_c.getMy();
        values[2] = mf.getMy(1.0)+mfil_j.getMy();
        values[3] = -mf.getQz(0.0)-mfil_i.getQz();
        values[4] = mf.getQz(0.5)+mfil_c.getQz();
        values[5] = -mf.getQz(1.0)-mfil_j.getQz();
        values[6] =  mf.N+mfil_i.getN();
    }else{
        values[0] = -mf.getMz(0.0)-mfil_i.getMz();
        values[1] = -mf.getMz(0.5)-mfil_c.getMz();
        values[2] = +mf.getMz(1.0)+mfil_j.getMz();
        values[3] = -mf.getQy(0.0)-mfil_i.getQy();
        values[4] = mf.getQy(0.5)+mfil_c.getQy();
        values[5] = -mf.getQy(1.0)-mfil_j.getQy();
        values[6] =  mf.N+mfil_i.getN();
    }
    int sign = 1;
//    if(isClockWize){
//        for(int i=0;i<3;i++)values[i]=-values[i];
//        sign = -1;
//    }

    //curve
    QList<qreal> valuelist;
    valuelist.clear();
    for (int i = 0; i <= div; i++) {
        qreal x = (qreal)i / (qreal)div;
        qreal moment = 0;
        MemberForceByIL mfil = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0,x * md->getTotalLength());
        if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ) {
            moment = sign*(mf.getMy(x) + mfil.getMy());
        } else  {
            moment = sign*(mf.getMz(x) + mfil.getMz());
        }
        valuelist << moment;
    }

    drawCurve(valuelist,0.0,totalLength);
    plotValues(values);


}

//QVector<qreal> OutputLineItemStress::faceMemberForce(const QUuid &uid,
//                               const QList<STRESS> &stress, bool isNormalDirection ){
//    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);

//    MemberForce mf = UnifiedFixedData::getInstance()->getPstnData()->getCombinationFaceMemberForce(uid, stress);
//    MemberForceByIL mfil_i = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,1,0);
//    MemberForceByIL mfil_c = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,2,0);
//    MemberForceByIL mfil_j = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,3,0);

//    //value
//    QVector<qreal> values(7,0.0);
//    bool useMy=false;
//    if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ) useMy = true;
//    if(useMy){
//        values[0]  = -mf.getMy(0)-mfil_i.getMy();
//        values[1]  = -mf.getMy(0.5)-mfil_c.getMy();
//        values[2]  = mf.getMy(1.0)+mfil_j.getMy();
//        values[3] = -mf.getQz(0) - mfil_i.getQz();
//        values[4] = mf.getQz(0.5) + mfil_c.getQz();
//        values[5] = -mf.getQz(1.0) - mfil_j.getQz();
//        values[6] = mf.N + mfil_i.getN();
//    }else{
//        values[0]  = -mf.getMz(0)-mfil_i.getMz();
//        values[1]  = -mf.getMz(0.5)-mfil_c.getMz();
//        values[2]  = +mf.getMz(1.0)+mfil_j.getMz();
//        values[3] = -mf.getQy(0) - mfil_i.getQy();
//        values[4] = mf.getQy(0.5) + mfil_c.getQy();
//        values[5] = -mf.getQy(1.0) - mfil_j.getQy();
//        values[6] = mf.N + mfil_i.getN();
//    }
//}
void OutputLineItemStress::faceDiagram(const QUuid &uid,
                                       const QList<STRESS> &stress, bool isNormalDirection )
{
    qDebug()<<"faceDiagram in";
    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);

    MemberForce mf = UnifiedFixedData::getInstance()->getPstnData()->getCombinationFaceMemberForce(uid, stress);
    MemberForceByIL mfil_i = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,1,0);
    MemberForceByIL mfil_c = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,2,0);
    MemberForceByIL mfil_j = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,3,0);

    //value
    QVector<qreal> values(7,0.0);
    bool useMy=false;
    if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ) useMy = true;
    if(useMy){
        values[0]  = -mf.getMy(0)-mfil_i.getMy();
        values[1]  = -mf.getMy(0.5)-mfil_c.getMy();
        values[2]  = mf.getMy(1.0)+mfil_j.getMy();
        values[3] = -mf.getQz(0) - mfil_i.getQz();
        values[4] = mf.getQz(0.5) + mfil_c.getQz();
        values[5] = -mf.getQz(1.0) - mfil_j.getQz();
        values[6] = mf.N + mfil_i.getN();
    }else{
        values[0]  = -mf.getMz(0)-mfil_i.getMz();
        values[1]  = -mf.getMz(0.5)-mfil_c.getMz();
        values[2]  = +mf.getMz(1.0)+mfil_j.getMz();
        values[3] = -mf.getQy(0) - mfil_i.getQy();
        values[4] = mf.getQy(0.5) + mfil_c.getQy();
        values[5] = -mf.getQy(1.0) - mfil_j.getQy();
        values[6] = mf.N + mfil_i.getN();
    }

    //curve
    QList<qreal> valuelist;
    valuelist.clear();
    QVector<qreal> len_f_r=getOutputFaceRigidLength();
    qreal iface = len_f_r.at(0);
    qreal jface = md->getTotalLength()-len_f_r.at(1);

    for (int i = 0; i <= div; i++) {
        qreal x = (qreal)i / (qreal)div;
        qreal moment = 0;
        MemberForceByIL mfil = UnifiedFixedData::getInstance()->getPstnData()
                ->getAnyPointMemberForce(uid,stress,0,iface + x*(jface-iface));
        if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ) {
            moment = mf.getMy(x) + mfil.getMy();
        } else  {
            moment = mf.getMz(x) + mfil.getMz();
        }
        valuelist << moment;
    }
    drawCurve(valuelist,iface,jface);
    plotValues(values);
//    drawYieldMarks();
}

void OutputLineItemStress::rigidDiagram(const QUuid &uid,
                                       const QList<STRESS> &stress, bool isNormalDirection )
{
    QVector<qreal> len_r=getOutputFaceRigidLength();
    qreal len_ri=len_r.at(2);
    qreal len_rj=len_r.at(3);
    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);

    MemberForce mf = UnifiedFixedData::getInstance()->getPstnData()->getCombinationFaceMemberForce(uid, stress);
//    MemberForceByIL mfil_i = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,1,0);
    MemberForceByIL mfil_i = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0,len_ri);
    MemberForceByIL mfil_j = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0,md->getTotalLength()-len_rj);

    MemberForceByIL mfil_c = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,2,0);
//    MemberForceByIL mfil_j = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,3,0);

    //value
    QVector<qreal> values(7,0.0);
    bool useMy=false;
    if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ) useMy = true;
    if(useMy){
        values[0]  = -mf.getMy(0)-mfil_i.getMy();
        values[1]  = -mf.getMy(0.5)-mfil_c.getMy();
        values[2]  = mf.getMy(1.0)+mfil_j.getMy();
        values[3] = -mf.getQz(0) - mfil_i.getQz();
        values[4] = mf.getQz(0.5) + mfil_c.getQz();
        values[5] = -mf.getQz(1.0) - mfil_j.getQz();
        values[6] = mf.N + mfil_i.getN();
    }else{
        values[0]  = -mf.getMz(0)-mfil_i.getMz();
        values[1]  = -mf.getMz(0.5)-mfil_c.getMz();
        values[2]  = +mf.getMz(1.0)+mfil_j.getMz();
        values[3] = -mf.getQy(0) - mfil_i.getQy();
        values[4] = mf.getQy(0.5) + mfil_c.getQy();
        values[5] = -mf.getQy(1.0) - mfil_j.getQy();
        values[6] = mf.N + mfil_i.getN();
    }

    //curve
    QList<qreal> valuelist;
    valuelist.clear();
    for (int i = 0; i <= div; i++) {
        qreal x = (qreal)i / (qreal)div;
        qreal moment = 0;
        MemberForceByIL mfil = UnifiedFixedData::getInstance()->getPstnData()
                ->getAnyPointMemberForce(uid,stress,0,len_ri + x*(len_rj-len_ri));
        if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ) {
            moment = mf.getMy(x) + mfil.getMy();
        } else  {
            moment = mf.getMz(x) + mfil.getMz();
        }
        valuelist << moment;
    }
    drawCurve(valuelist,len_ri,md->getTotalLength()-len_rj);
    plotValues(values);
//    drawYieldMarks();
}

void OutputLineItemStress::cmqDiagram(const QUuid &uid,
                                      const QList<STRESS> &stress,  bool isNormalDirection )
{
    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);



    //value
    QVector<qreal> values(7,0.0);
    MemberForceByIL mfil_i = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0,0);
    MemberForceByIL mfil_c = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0,0.5 * md->getTotalLength());
    MemberForceByIL mfil_j = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0, md->getTotalLength());

    if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ){
        values[0] = -mfil_i.getMy();
        values[1] = -mfil_c.getMy();
        values[2] = mfil_j.getMy();
        values[3] = -mfil_i.getQz();
        values[4] = mfil_c.getQz();
        values[5] = -mfil_j.getQz();
        values[6] =  mfil_i.getN();

    }else{
        values[0] = -mfil_i.getMz();
        values[1] = -mfil_c.getMz();
        values[2] = +mfil_j.getMz();
        values[3] = -mfil_i.getQy();
        values[4] = mfil_c.getQy();
        values[5] = -mfil_j.getQy();
        values[6] =  mfil_i.getN();

    }
    values[1]=0.5*(values[2]-values[0])+values[1];

    //curve
    QList<qreal> valuelist;
    valuelist.clear();
    for (int i = 0; i <= div; i++) {
        qreal x = (qreal)i / (qreal)div;
        qreal moment = 0;
        MemberForceByIL mfil = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0,x * md->getTotalLength());
        if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ) {
            moment =   mfil.getMy();
        } else  {
            moment =   mfil.getMz();
        }
        valuelist << moment;

    }

    //稜線をつなぐ
    drawCurve(valuelist,0,totalLength);
    plotValues(values);

}
void OutputLineItemStress::rankDiagram(const QUuid &uid, bool isNormalDirection )
{

    QList<STRESS> stress;
    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();
 //    if(type.ds_or_qu == FIGURE_TYPE_PROPERTY::DSQU::DS){
//    QString fileName = UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(type.caseName).quFileName;
    QString fileName = u8"RSTU3QD";//UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(type.caseName).dsFileName;
    DataNBLD3D_RSTQD qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(fileName);
    stress = getQuDsStressList(fileName);
//    }
//    if(type.ds_or_qu == FIGURE_TYPE_PROPERTY::DSQU::QU){
//        stress = getQuDsStressList(fileName);
//    }

    DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(uid);
    bool iPin = false;
    if(model.ifpmm[numm][0]==1 && model.ifpmm[numm][1]==1 && model.ifpmm[numm][2]==1
            && model.ifpmm[numm][3]==1 && model.ifpmm[numm][4]==1)iPin = true;

    MemberForce mf = UnifiedFixedData::getInstance()->getPstnData()->getCombinationFaceMemberForce(uid, stress);
    MemberForceByIL mfil_i = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,1,0);
    MemberForceByIL mfil_c = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,2,0);
    MemberForceByIL mfil_j = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,3,0);

    //value
    QVector<qreal> values(7,0.0);
    bool useMy=false;
    if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ) useMy = true;


//    qreal val0=0,val2=0;
//    if(iPin){
//        if(qd.ultmm[numm][30] > 0.0){
//            values[6] = qMax(qd.ultmm[numm][42],qd.ultmm[numm][54]);
//        }else{
//            values[6] = qMin(qd.ultmm[numm][43],qd.ultmm[numm][55]);
//        }
//    }else if(useMy){
//        val0=mf.getMy(0)+mfil_i.getMy();
//        val2=mf.getMy(1.0)+mfil_j.getMy();
//        values[0]  = qd.ramem[numm][6] -val0;
//        values[1]  = -mf.getMy(0.5)-mfil_c.getMy();
//        values[2]  = qd.ramem[numm][16] +val2;
//        values[3] = -qd.ramem[numm][27]-mf.getQz(0) - mfil_i.getQz();
//        values[4] = mf.getQz(0.5) + mfil_c.getQz();
//        values[5] = -qd.ramem[numm][27] -mf.getQz(1.0) - mfil_j.getQz();
//        values[6] = qd.ultmm[numm][30] + mf.N + mfil_i.getN();
//    }else{
//        val0=mf.getMz(0)+mfil_i.getMz();
//        val2=mf.getMz(1.0)+mfil_j.getMz();
//        values[0]  = qd.ramem[numm][7] -val0;
//        values[1]  = -mf.getMz(0.5)-mfil_c.getMz();
//        values[2]  = qd.ramem[numm][17] +val2;
//        values[3] = -qd.ramem[numm][26]-mf.getQy(0) - mfil_i.getQy();
//        values[4] = mf.getQy(0.5) + mfil_c.getQy();
//        values[5] = -qd.ramem[numm][26] -mf.getQy(1.0) - mfil_j.getQy();
//        values[6] = qd.ultmm[numm][30] + mf.N + mfil_i.getN();
//    }
//    values[1]=0.5*(val2+val0)+values[1];
    //壁付帯柱の数値、M0の値、柱のMの符号
    if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ) useMy = true;
    if(useMy){
        values[0] = -mf.getMy(0)-mfil_i.getMy();// + qd.ramem[numm][6];
        values[1] = -mf.getMy(0.5)-mfil_c.getMy();
        values[2] =  mf.getMy(1.0)+mfil_j.getMy();// + qd.ramem[numm][16];
        values[3] = -mf.getQz(0) - mfil_i.getQz();//+ qd.ramem[numm][27];
        values[4] =  mf.getQz(0.5) + mfil_c.getQz();
        values[5] = -mf.getQz(1.0) - mfil_j.getQz();//+ qd.ramem[numm][27];
        values[6] =  mf.N + mfil_i.getN();
    }else{
        values[0] = -mf.getMz(0)-mfil_i.getMz() ;//+ qd.ramem[numm][7];
        values[1] = -mf.getMz(0.5)-mfil_c.getMz();
        values[2] = +mf.getMz(1.0)+mfil_j.getMz();//+ qd.ramem[numm][17];
        values[3] = -mf.getQy(0) - mfil_i.getQy();//+ qd.ramem[numm][26];
        values[4] =  mf.getQy(0.5) + mfil_c.getQy();
        values[5] = -mf.getQy(1.0) - mfil_j.getQy();//+ qd.ramem[numm][26];
        values[6] =  mf.N + mfil_i.getN();
    }
//    DFQ(1)=-ULTMM(31,IM,IULT)-POSRST(1,1)   x i
//            DFQ(2)=-RAMEM(27,IM,IULT)-POSRST(2,1) y i
//            DFQ(3)=-RAMEM(28,IM,IULT)-POSRST(3,1) z i
//            DFM(1)=                  -POSRST(4,1) rx i
//            DFM(2)= RAMEM( 7,IM,IULT)-POSRST(5,1) ry i
//            DFM(3)= RAMEM( 8,IM,IULT)-POSRST(6,1) rz i

//            DFQ(4)= ULTMM(31,IM,IULT)+POSRST(1,3)
//            DFQ(5)= RAMEM(27,IM,IULT)+POSRST(2,3)
//            DFQ(6)= RAMEM(28,IM,IULT)+POSRST(3,3)
//            DFM(4)=                  +POSRST(4,3)
//            DFM(5)= RAMEM(17,IM,IULT)+POSRST(5,3)
//            DFM(6)= RAMEM(18,IM,IULT)+POSRST(6,3)

    //curve
    QVector<qreal> len_f_r=getOutputFaceRigidLength();
    qreal iface = len_f_r.at(0);
    qreal jface = md->getTotalLength()-len_f_r.at(1);

    QList<qreal> valuelist;
    valuelist.clear();
    for (int i = 0; i <= div; i++) {
        qreal x = (qreal)i / (qreal)div;
        qreal moment = 0;
        MemberForceByIL mfil = UnifiedFixedData::getInstance()->getPstnData()
                ->getAnyPointMemberForce(uid,stress,0,iface + x*(jface-iface));
        if ((isZin && !isNormalDirection) || (!isZin && isNormalDirection) ) {
            moment = mf.getMy(x) + mfil.getMy();
        } else  {
            moment = mf.getMz(x) + mfil.getMz();
        }
        valuelist << moment;
    }
    drawCurve(valuelist,iface,jface);
    plotValues(values);

    appendRank();
}
//void OutputMemberStressItem::appendRankMark(){

//}

QList<STRESS> OutputLineItemStress::getQuDsStressList(const QString& fileName )const{
    DataNBLD3D_RSTQD rstqd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(fileName);
    QList<STRESS> stresslist;
    for(int i=0;i<rstqd.nameis.count();i++){
        STRESS st;
        st.rst = rstqd.nameis.at(i);
        st.coef = rstqd.coefis.at(i);
        stresslist.append(st);
    }
    STRESS st;
    st.rst = rstqd.nmrsu;
    st.coef = 1.0;
    stresslist.append(st);
    return stresslist;

}

void OutputLineItemStress::appendRank()
{
    //    !isAroundPlane
    //壁中部材の梁・・・スキップ
    //弾性支承梁・・・スキップ  AKY=DMM1(23,IM)    AKZ=DMM1(24,IM)　が0でない
    //平面図の柱・・・直交部材。別途考慮？
    //平面図のブレース・・・直交部材。別途考慮<A>。せん断マップなどを参考
    //iPin・・・せん断集計用ランク
    qDebug()<<"appendRank";
    QColor textColor=myFigureTemplate->valueTextColor(myDataType,ATTENTION_STATUS::STATUS_NORMAL);
    int stringSize=valueTextSize;
    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();
    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));

    qreal eps = 0.000001;
    DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();

    DataNBLD3D_RSTQD qd;
    QString name = myFigureTemplate->getFigureTypeProperty().caseName;
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    if(model.dmm1[numm][22] > eps || model.dmm1[numm][23] > eps)return;
    bool iPin = false;
    if(model.ifpmm[numm][0]==1 && model.ifpmm[numm][1]==1 && model.ifpmm[numm][2]==1
            && model.ifpmm[numm][3]==1 && model.ifpmm[numm][4]==1)iPin = true;


    //    if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
    qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
    //    }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
    //        qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
    //    }else{
    //        qDebug()<<"unknown qu ds type";
    //    }
    int iRank=qd.mamem[numm][19];
    int jRank=qd.mamem[numm][20];
    int shearRank=qd.mamem[numm][12];
    qDebug()<<"i,j,s rank"<<iRank<<jRank<<shearRank;
    QString str_i;
    QString str_j;
    if(iPin){
        str_i=u8"(A)";
        if(shearRank == 1)str_i=u8"(A)";
        if(shearRank == 2)str_i=u8"(B)";
        if(shearRank == 3)str_i=u8"(C)";
        if(shearRank == 4)str_i=u8"(D)";
        qDebug()<<"pin"<<str_i;

//        str_i=QString::number(iRank);
//        str_j=QString::number(jRank);
        OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                                                   -0.5 * ps.y() - 0.5 * pe.y()), p_ang, str_i,textColor,
                                           OFFSETPOINT::OFFSET_CENTERBOTTOM2, 99999,stringSize) );

        return;
    }

    if (myDataType==DATATYPE::TPCOLUMN && myFigureTemplate->figureVisible(DATATYPE::TPCOLUMN)) {

        str_i=u8"A";
        if(iRank == 1)str_i=u8"A";
        if(iRank == 2)str_i=u8"B";
        if(iRank == 3)str_i=u8"C";
        if(iRank == 4)str_i=u8"D";
        if(jRank == 1)str_j=u8"A";
        if(jRank == 2)str_j=u8"B";
        if(jRank == 3)str_j=u8"C";
        if(jRank == 4)str_j=u8"D";
        if(shearRank == 1)str_i+=u8"(A)";
        if(shearRank == 2)str_i+=u8"(B)";
        if(shearRank == 3)str_i+=u8"(C)";
        if(shearRank == 4)str_i+=u8"(D)";

        qDebug()<<"col"<<str_i<<str_j;
//        str_i=QString::number(iRank);
//        str_j=QString::number(jRank);

        OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                                                   -0.5 * ps.y() - 0.5 * pe.y()), p_ang - M_PI / 2.0, str_i, textColor,
                                           OFFSETPOINT::OFFSET_RIGHTMIDDLE2, 999999,stringSize) );
        OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                                                   -0.5 * ps.y() - 0.5 * pe.y()), p_ang - M_PI / 2.0, str_j, textColor,
                                           OFFSETPOINT::OFFSET_RIGHTMIDDLE3, 999999,stringSize) );
    } else if (myDataType==DATATYPE::TPGIRDER && myFigureTemplate->figureVisible(DATATYPE::TPGIRDER)) {
        bool wallgirderVisible = static_cast<StressElevationTemplate*>(myFigureTemplate)->wallGirderFigureVisible();
        if (isAroundPlane && !wallgirderVisible)return;
        if(iRank == 1)str_i=u8"A";
        if(iRank == 2)str_i=u8"B";
        if(iRank == 3)str_i=u8"C";
        if(iRank == 4)str_i=u8"D";
        if(jRank == 1)str_j=u8"A";
        if(jRank == 2)str_j=u8"B";
        if(jRank == 3)str_j=u8"C";
        if(jRank == 4)str_j=u8"D";

        qDebug()<<"gir"<<str_i<<str_j;

//        str_i=QString::number(iRank);
//        str_j=QString::number(jRank);
        OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                                                   -0.5 * ps.y() - 0.5 * pe.y()), p_ang, str_i+u8"   "+str_j,textColor,
                                           OFFSETPOINT::OFFSET_CENTERBOTTOM2, 999999,stringSize)  );
    } else if (myDataType==DATATYPE::TPBRACE && myFigureTemplate->figureVisible(DATATYPE::TPBRACE)) {
        if(iRank == 1)str_i=u8"A";
        if(iRank == 2)str_i=u8"B";
        if(iRank == 3)str_i=u8"C";
        if(iRank == 4)str_i=u8"D";
        if(jRank == 1)str_j=u8"A";
        if(jRank == 2)str_j=u8"B";
        if(jRank == 3)str_j=u8"C";
        if(jRank == 4)str_j=u8"D";
        qDebug()<<"bra"<<str_i<<str_j;

//        str_i=QString::number(iRank);
//        str_j=QString::number(jRank);
         OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                                                   -0.5 * ps.y() - 0.5 * pe.y()), p_ang, str_i + u8"   " + str_j,textColor,
                                           OFFSETPOINT::OFFSET_CENTERBOTTOM2, 999999,stringSize) );
    }
}
} // namespace post3dapp
