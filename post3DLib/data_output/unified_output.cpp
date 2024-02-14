#include "unified_output.h"

#include <QDateTime>
#include <QDebug>

#include "calc_any_point_stress.h"
#include "define_draw_figure.h"
#include "unified_data.h"

namespace post3dapp{
UnifiedOutput::UnifiedOutput()
{
}


void UnifiedOutput::setReadStatus(QHash<OUTPUT_FILE_TYPE,READING_STATUS> _pstnReadStatus)
{
    pstnReadStatus = _pstnReadStatus;
}
READING_STATUS UnifiedOutput::getReadStatus(OUTPUT_FILE_TYPE filetype)const
{
    return pstnReadStatus.value(filetype);
}

void UnifiedOutput::setCurrentFile(const QString &filename){
    currentFileInfo = ( filename.isEmpty() ) ? QFileInfo() : QFileInfo(filename) ;
    emit fileChanged();
}

QString UnifiedOutput::currentFilePath() const
{
    return ( currentFileInfo.exists() ) ? currentFileInfo.absoluteFilePath() : QString() ;
}

QString UnifiedOutput::currentFileBaseName() const
{
    return ( currentFileInfo.exists() ) ? currentFileInfo.baseName() : QString() ;
}

QString UnifiedOutput::currentFileDir() const
{
    if ( !currentFileInfo.exists() ) return QString();
    return currentFileInfo.absolutePath();
}


/*ibaraki defined*/
void UnifiedOutput::setFaceMemberForce(const QUuid &uid, const QString &loadcase,
                                       const QList<MemberForce> &st)
{
    MRESULT *result;
    if (memberResult.contains(uid))result = memberResult[uid];
    else result = new MRESULT;
    result->faceSectionalForce[loadcase] = st;
    memberResult[uid] = result;
}
MemberForce UnifiedOutput::getFaceMemberForce(const QUuid &uid, const QString &loadcase,
                                              const int &step)const
{
    //stepがjointdisp[loadcase].countより大のとき記述
    qDebug()<<"UnifiedOutput::getFaceMemberForce in"<<loadcase<<step;
    if(!memberResult.contains(uid)){
        qDebug()<<"UnifiedOutput::getFaceMemberForce memberResult not contains uid";
        return MemberForce();
    }

    if (loadcase.isEmpty() || step == 0) {
        return MemberForce();
    } else if (!memberResult[uid]->faceSectionalForce.contains(loadcase)) {
        return MemberForce();
    } else if (step > memberResult[uid]->faceSectionalForce[loadcase].count() || step < 0) {
        return memberResult[uid]->faceSectionalForce[loadcase].last();
    } else {
        return memberResult[uid]->faceSectionalForce[loadcase][step - 1];
    }
}
MemberForce UnifiedOutput::getCombinationFaceMemberForce(const QUuid &uid, const QList<STRESS> &stress)const
{
    qDebug()<<"UnifiedOutput::getCombinationFaceMemberForce in";
    QStringList loadcase;
    QList<qreal> coef;
    QList<int> step;
    for(int i=0;i<stress.count();i++){
        loadcase << stress.at(i).rst;
        coef <<stress.at(i).coef;
        step << stress.at(i).getStep();
    }
    qDebug()<<"UnifiedOutput::getCombinationFaceMemberForce set case,coef,step";

    int numcase = qMin(coef.count(), loadcase.count());
    MemberForce combiforce;
    for (int i = 0; i < numcase; i++) {
        combiforce = combiforce + getFaceMemberForce(uid, loadcase.at(i), step.at(i)) * coef.at(i);
    }
    qDebug()<<"UnifiedOutput::getCombinationFaceMemberForce out";
    return combiforce;
}
void UnifiedOutput::setNodeMemberForce(const QUuid &uid, const QString &loadcase,
                                       const QList<MemberForce> &st)
{
    MRESULT *result;
    if (memberResult.contains(uid))result = memberResult[uid];
    else result = new MRESULT;
    result->nodeSectionalForce[loadcase] = st;
    memberResult[uid] = result;
}
MemberForce UnifiedOutput::getNodeMemberForce(const QUuid &uid, const QString &loadcase,
                                              const int &step)const
{
    //stepがjointdisp[loadcase].countより大のとき記述
    if (!memberResult.contains(uid))return MemberForce();

    if (loadcase.isEmpty() || step == 0) {
        return MemberForce();
    } else if (!memberResult[uid]->nodeSectionalForce.contains(loadcase)) {
        return MemberForce();
    } else if (step > memberResult[uid]->nodeSectionalForce[loadcase].count() || step < 0) {
        return memberResult[uid]->nodeSectionalForce[loadcase].last();
    } else {
        return memberResult[uid]->nodeSectionalForce[loadcase][step - 1];
    }
}
MemberForce UnifiedOutput::getCombinationNodeMemberForce(const QUuid &uid, const QList<STRESS> &stress)const
{
    QStringList loadcase;
    QList<qreal> coef;
    QList<int> step;
    for(int i=0;i<stress.count();i++){
        loadcase << stress.at(i).rst;
        coef <<stress.at(i).coef;
        step << stress.at(i).getStep();
    }

    int numcase = qMin(coef.count(), loadcase.count());
    MemberForce combiforce;
    for (int i = 0; i < numcase; i++) {
        combiforce = combiforce + getNodeMemberForce(uid, loadcase.at(i), step.at(i)) * coef.at(i);
    }
    return combiforce;
}

MemberForceByIL UnifiedOutput::getAnyPointMemberForce(const QUuid &uid,  const QList<STRESS> &stress,const int iPos,const qreal pos)const
{
    //faceのとき、iPos=1 or 3、中央でiPos=2,節点の時、iPos=0かつpos=0 or 1？確認
    QVector<qreal> total(6,0.0);
    QVector<qreal> tmp(6,0.0);


//    QString modelname = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstData().value(stress.).nameam;
    for(int i=0;i<stress.count();i++){
        if(stress.at(i).rst.isEmpty())continue;
        if(stress.at(i).getStep()==0)continue;
        tmp = calcAnyPointStress::calcMemberForce(uid,iPos,pos,stress.at(i).rst,stress.at(i).getStep());
        for(int j=0;j<6;j++){
            total[j] += tmp[j] * stress.at(i).coef;
        }
    }
    return MemberForceByIL(total);
}
DataNBLD3D_RSTStep UnifiedOutput::getRSTStep(const QString &loadcase, const int &step)const
{
    getNbldRstData().value(loadcase);
    if (loadcase.isEmpty() || step == 0) {
        return DataNBLD3D_RSTStep();
    } else if (!getNbldRstData().contains(loadcase)) {
        return DataNBLD3D_RSTStep();
    } else if (step > getNbldRstData().value(loadcase).stressSteps.count() || step < 0) {
        return getNbldRstData().value(loadcase).stressSteps.last();
    } else {
        return getNbldRstData().value(loadcase).stressSteps.at(step-1);
    }
}
XYZ UnifiedOutput::getJointDisp(const QUuid &uid, const QString &loadcase, const int &step)const
{
    //stepがjointdisp[loadcase].countより大のとき記述
    if (loadcase.isEmpty() || step == 0) {
        return XYZ(0.0, 0.0, 0.0);
    } else if (!jointResult[uid]->jointDisp.contains(loadcase)) {
        return XYZ(0.0, 0.0, 0.0);
    } else if (step > jointResult[uid]->jointDisp[loadcase].count() || step < 0) {
        return jointResult[uid]->jointDisp[loadcase].last();
    } else {
        return jointResult[uid]->jointDisp[loadcase][step - 1];
    }
}
XYZ UnifiedOutput::getCombinationJointDisp(const QUuid &uid, const QList<STRESS> &stress)const
{
    QStringList loadcase;
    QList<qreal> coef;
    QList<int> step;
    for(int i=0;i<stress.count();i++){
        loadcase << stress.at(i).rst;
        coef <<stress.at(i).coef;
        step << stress.at(i).getStep();
    }

    XYZ disp;
    int numcase = qMin(loadcase.count(), coef.count());
    for (int i = 0; i < numcase; i++) {
        if (loadcase.at(i).isEmpty())continue;
        disp = disp + getJointDisp(uid, loadcase.at(i), step.at(i)) * coef.at(i);
    }
    return disp;
}
XYZ UnifiedOutput::getJointRotate(const QUuid &uid, const QString &loadcase, const int &step) const
{
    //stepがjointdisp[loadcase].countより大のとき記述
    if (loadcase.isEmpty() || step == 0) {
        return XYZ(0.0, 0.0, 0.0);
    } else if (!jointResult[uid]->jointRotate.contains(loadcase)) {
        return XYZ(0.0, 0.0, 0.0);
    } else if (step > jointResult[uid]->jointRotate[loadcase].count() || step < 0) {
        return jointResult[uid]->jointRotate[loadcase].last();
    } else {
        return jointResult[uid]->jointRotate[loadcase][step - 1];
    }
}
QList<qreal> UnifiedOutput::getReaction(const QUuid &uid, const QString &loadcase,
                                        const int &step)const
{
    if (loadcase.isEmpty() || !jointResult[uid]->react.contains(loadcase) || step == 0) {
        QList<qreal> zerolist;
        zerolist << 0.0 << 0.0 << 0.0 << 0.0 << 0.0 << 0.0;
        return zerolist;
    } else if (step > jointResult[uid]->react[loadcase].count() || step < 0) {
        return jointResult[uid]->react[loadcase].last();
    } else {
        return  jointResult[uid]->react[loadcase][step - 1];
    }
}
QList<qreal> UnifiedOutput::getCombinationReaction(const QUuid &uid, const QList<STRESS> &stress)const
{
    QStringList loadcase;
    QList<qreal> coef;
    QList<int> step;
    for(int i=0;i<stress.count();i++){
        loadcase << stress.at(i).rst;
        coef <<stress.at(i).coef;
        step << stress.at(i).getStep();
    }
    int mxdof = 6;
    int numcase = qMin(loadcase.count(), coef.count());
    QList<qreal> combiReaction = getReaction(uid, QString());
    for (int i = 0; i < numcase; i++) {
        if (loadcase.at(i).isEmpty())continue;
        QList<qreal> reaction = getReaction(uid, loadcase.at(i), step.at(i));
        for (int dof = 0; dof < mxdof; dof++) {
            combiReaction[dof] += coef.at(i) * reaction.at(dof);
        }
    }
    return combiReaction;
}
QList<MRESULT::YIELD_STATE> UnifiedOutput::getYieldState(const QUuid &uid, const QString &loadcase,
                                                         const int &step)const
{
    if (!memberResult[uid]->yieldstate.contains(loadcase) || step == 0) {
        QList<MRESULT::YIELD_STATE> ystate;
        ystate << MRESULT::YIELD_STATE::NOT << MRESULT::YIELD_STATE::NOT << MRESULT::YIELD_STATE::NOT;
        return ystate;
    } else if (step > memberResult[uid]->yieldstate[loadcase].count() || step < 0) {
        return memberResult[uid]->yieldstate[loadcase].last();
    } else {
        return memberResult[uid]->yieldstate[loadcase][step - 1];
    }

}
void UnifiedOutput::setModeData(const QHash<QUuid, QVector<XYZ> > &_mode,
                                const QVector<QVector<qreal>> &_beta,
                                const QVector<qreal> &_naturalPeriod, const QVector<qreal> &_totalMass)
{
    modeData.mode = _mode;
    modeData.beta = _beta;
    modeData.totalMass = _totalMass;
    modeData.naturalPeriod = _naturalPeriod;
    modeData.resetStandardizedMode();
}
XYZ UnifiedOutput::getModeDisp(const MODE_TYPE &type, const QUuid &uid, const int &nmode,
                               const qreal &dispratio, const qreal &time, const qreal &timespeedratio) const
{
    if (nmode <= 0 || nmode > modeData.nummode)return XYZ(); //nmodeは正の整数
    XYZ basedisp;
    if (type == MODE) {
        basedisp = modeData.standardizedMode[uid].at(nmode - 1);
    } else if (type == BETAX_U) {
        basedisp = modeData.mode[uid].at(nmode - 1) * modeData.beta.at(nmode - 1).at(0);
    } else if (type == BETAY_U) {
        basedisp = modeData.mode[uid].at(nmode - 1) * modeData.beta.at(nmode - 1).at(1);
    }
    return basedisp * dispratio * qCos(2.0 * M_PI * time * timespeedratio / modeData.naturalPeriod.at(
                                           nmode - 1));
}
void UnifiedOutput::setSpringValue(const QUuid &uid, const QString &loadcase,
                                   const QVector<qreal> &values)
{
    SRESULT *result;
    if (springResult.contains(uid))result = springResult[uid];
    else result = new SRESULT;
    result->springValue[loadcase] = values;
    springResult[uid] = result;
}
qreal UnifiedOutput::getSpringValue(const QUuid &uid, const QString &loadcase, const int &step)const
{
    //stepがjointdisp[loadcase].countより大のとき記述
    if (!springResult.contains(uid))return 0.0;

    if (loadcase.isEmpty() || step == 0) {
        return 0.0;
    } else if (!springResult[uid]->springValue.contains(loadcase)) {
        return 0.0;
    } else if (step > springResult[uid]->springValue[loadcase].count() || step < 0) {
        return springResult[uid]->springValue[loadcase].last();
    } else {
        return springResult[uid]->springValue[loadcase][step - 1];
    }
}

/*uuidに対応する構造形式によって返す値をコントロールすること*/

//N-M余裕率
qreal UnifiedOutput::getAllowanceRateBending(const QUuid &uid, const int &numch, const int &icj)const
{
    return static_cast<double>(chckRatioData.allowanceRateBending.value(uid).at(numch).at(icj));
}
//N-M検定比
qreal UnifiedOutput::getVerificationRatioBending(const QUuid &uid, const int &numch, const int &icj)const
{
    return static_cast<double>(chckRatioData.verificationRatioBending.value(uid).at(numch).at(icj));
}

//せん断検定比
qreal UnifiedOutput::getVerificationRatioShear1(const QUuid &uid, const int &numch, const int &icj)const
{
    return static_cast<double>(chckRatioData.verificationRatioShear.value(uid).at(numch).at(icj));
}
//せん断検定比2
qreal UnifiedOutput::getVerificationRatioShear2(const QUuid &uid, const int &numch, const int &icj)const
{
    return static_cast<double>(chckRatioData.verificationRatioShear2.value(uid).at(numch).at(icj));
}

QList<STRESS> UnifiedOutput::askCombiStress(const QString& combi) const{
    qDebug()<<"askCombiStress init";
    int ich = nbld_chckData.namech.indexOf(combi);
    qDebug()<<"askCombiStress index" << ich;
    QList<STRESS> list;
    if(ich == -1)return list; //データがなかったら空のリストを返す

    for(int i=0;i<nbld_chckData.mxrsch;i++){
        qDebug()<<"askCombiStress loop" << i;
        STRESS st;
        st.rst = nbld_chckData.nmrsch.at(ich).at(i);
        st.coef = nbld_chckData.corsch.at(ich).at(i);
        list<<st;
    }
    return list;
}
//qreal UnifiedOutput::calcMemberForce(){
//DataNBLD3D_LOAD load;
//DataNBLD3D_RST rst;
///*
// *　今→icjの値を計算して保持、二次関数近似
// * 任意点の値が欲しい。indleのような中間荷重のデータを整形して同様にMemberforceのメンバとして持たせる？
// * →中間荷重のデータは、STEPにかかわらないため、MemberForceのメンバーではない。
// *　UnifiedOutputに整形データと計算用static関数を用意する？中間荷重分だけ拾うルーチンをUnifiedOutput、短期と合算したものであればMemberForceの領分かな。
// * /

//}



} // namespace post3dapp
