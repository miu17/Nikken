#ifndef UNIFIEDOUTPUT_H
#define UNIFIEDOUTPUT_H

#include <QDebug>
#include <QHash>
#include <QObject>

#include "define_npstn.h"
#include "define_output.h"
#include "define_unifieddata.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{

enum OUTPUT_FILE_TYPE;
enum class OUTFIGTYPE;
enum class READING_STATUS;
struct STRESS;
class MemberForceByIL;

class POST3D_CLASS_DLLSPEC UnifiedOutput : public QObject
{
    Q_OBJECT

public:
    static UnifiedOutput* Create(){return new UnifiedOutput();}

    //部材番号とUuidの、post3dモデルでの対応表
    void insertUuidIndexTable(QUuid uid,int index,DATATYPE type){
        uuidIndexTable.insert(uid,QPair<int,DATATYPE>(index,type));
    }
    void removeUuidIndexTable(){
        uuidIndexTable.clear();
    }
    void removeUuidIndexTableItem(QUuid uid){
        uuidIndexTable.remove(uid);
    }
    int uuidToIndex(QUuid uid) const{
        return uuidIndexTable.value(uid).first;
    }
    QUuid indexToUuid(int index,DATATYPE type) const{
        return uuidIndexTable.key(QPair<int,DATATYPE>(index,type));
    }
    //b3dモデルとの対応表
    //ただし、0からになっているのでFortranで使う時は1足す
    void insertUuidB3dIndexTable(QUuid uid,int index,B3D_DATATYPE type){
        uuidB3dIndexTable.insert(uid,QPair<int,B3D_DATATYPE>(index,type));
    }
    void removeUuidB3dIndexTable(){
        uuidB3dIndexTable.clear();
    }
    void removeUuidIndexB3dTableItem(QUuid uid){
        uuidB3dIndexTable.remove(uid);
    }
    int uuidToB3dIndex(QUuid uid) const{
        return uuidB3dIndexTable.value(uid).first;
    }
    QUuid b3dIndexToUuid(int index,B3D_DATATYPE type) const{
        return uuidB3dIndexTable.key(QPair<int,B3D_DATATYPE>(index,type));
    }
    //b3dモデルの断面名との対応表
    void insertLineSectionNameB3dIndexTable(QString name,int index){
        lineSectionNameB3dIndexTable.insert(name,index);
    }
    void removeLineSectionNameB3dIndexTable(){
        lineSectionNameB3dIndexTable.clear();
    }
    void removeLineSectionNameIndexB3dTableItem(QString name){
        lineSectionNameB3dIndexTable.remove(name);
    }
    int lineSectionNameToB3dIndex(QString name) const{
        return lineSectionNameB3dIndexTable.value(name);
    }
    QUuid b3dIndexToLineSectionName(int index) const{
        return lineSectionNameB3dIndexTable.key(index);
    }
    //b3dモデルの断面名との対応表
    void insertPlaneSectionNameB3dIndexTable(QString name,int index){
       planeSectionNameB3dIndexTable.insert(name,index);
    }
    void removePlaneSectionNameB3dIndexTable(){
        planeSectionNameB3dIndexTable.clear();
    }
    void removePlaneSectionNameIndexB3dTableItem(QString name){
        planeSectionNameB3dIndexTable.remove(name);
    }
    int planeSectionNameToB3dIndex(QString name) const{
        return planeSectionNameB3dIndexTable.value(name);
    }
    QUuid b3dIndexToPlaneSectionName(int index) const{
        return planeSectionNameB3dIndexTable.key(index);
    }

    void insertDamperSectionNameB3dIndexTable(QString name,int index){
       damperSectionNameB3dIndexTable.insert(name,index);
    }
    void insertSpringSectionNameB3dIndexTable(QString name,int index){
       springSectionNameB3dIndexTable.insert(name,index);
    }

    //一般
    void setMaxStep(const QString &stresscase, const int &step)
    {
        qDebug()<<"setMaxStep caseName="<<stresscase<<",maxStep="<<step;
        maxStep.insert(stresscase,step);
    }
    int getMaxStep(const QString &stresscase) const
    {
        qDebug()<<"getMaxStep caseName="<<stresscase<<",maxStep="<<maxStep[stresscase];
        return maxStep[stresscase];
    }
    void setStressCaseList(const QStringList &stcase)
    {
        stressCase = stcase;
    }
    QStringList getStressCaseList() const
    {
        return stressCase;
    }
    void setModelCaseList(const QStringList &mdcase)
    {
        modelCase = mdcase;
    }
    QStringList getModelCaseList() const
    {
        return modelCase;
    }
    QStringList getChckCaseList() const
    {
        return nbld_chckData.namech;
    }
    void setQuDsCaseList(const QStringList &qdcase)
    {
        qudsCase = qdcase;
    }
    QStringList getQuDsCaseList() const
    {
        return qudsCase;
    }

    MRESULT *resultOfMember(const QUuid &uid)const
    {
        if (memberResult.contains(uid)) {
            return memberResult[uid];
        } else {
            MRESULT *mr = new MRESULT;
            return mr;
        }
    }
    void setReadStatus(QHash<OUTPUT_FILE_TYPE, READING_STATUS> _pstnReadStatus);
    READING_STATUS getReadStatus(OUTPUT_FILE_TYPE filetype)const;

    //断面力
    void setFaceMemberForce(const QUuid &uid, const QString &loadcase, const QList<MemberForce> &st);
    MemberForce getFaceMemberForce(const QUuid &uid, const QString &loadcase, const int &step = 0)const;
    MemberForce getCombinationFaceMemberForce(const QUuid &uid, const QList<STRESS> &stress)const;
    void setNodeMemberForce(const QUuid &uid, const QString &loadcase, const QList<MemberForce> &st);
    MemberForce getNodeMemberForce(const QUuid &uid, const QString &loadcase, const int &step = 0)const;
    MemberForce getCombinationNodeMemberForce(const QUuid &uid,  const QList<STRESS> &stress)const;
    MemberForceByIL getAnyPointMemberForce(const QUuid &uid,  const QList<STRESS> &stress,const int iPos,const qreal pos)const;

    //節点変位
    qreal xDisp(const QUuid &uid, const QString &loadcase, const int &step = 0) const
    {
        if(step == 0) return 0.0;
        return (step < 0
                || step > jointResult[uid]->jointDisp[loadcase].count()) ?
               jointResult[uid]->jointDisp[loadcase].last().x : jointResult[uid]->jointDisp[loadcase][step - 1].x;
    }
    qreal yDisp(const QUuid &uid, const QString &loadcase, const int &step = 0) const
    {
        if(step == 0) return 0.0;
        return (step < 0
                || step > jointResult[uid]->jointDisp[loadcase].count()) ?
               jointResult[uid]->jointDisp[loadcase].last().y : jointResult[uid]->jointDisp[loadcase][step - 1].y;
    }
    qreal zDisp(const QUuid &uid, const QString &loadcase, const int &step = 0) const
    {
        if(step == 0) return 0.0;
        return (step < 0
                || step > jointResult[uid]->jointDisp[loadcase].count()) ?
               jointResult[uid]->jointDisp[loadcase].last().z : jointResult[uid]->jointDisp[loadcase][step - 1].z;
    }
    void setJointDisp(const QUuid &uid, const QString &loadcase, const QList<XYZ> &jd)
    {
        if (!jointResult.contains(uid)) jointResult[uid] = new JRESULT;
        jointResult[uid]->jointDisp[loadcase] = jd;
    }
    XYZ getJointDisp(const QUuid &uid, const QString &loadcase, const int &step = 0) const;
    void setJointRotate(const QUuid &uid, const QString &loadcase, const QList<XYZ> &jd)
    {
        if (!jointResult.contains(uid)) jointResult[uid] = new JRESULT;
        jointResult[uid]->jointRotate[loadcase] = jd;
    }
    XYZ getJointRotate(const QUuid &uid, const QString &loadcase, const int &step = 0) const;
    XYZ getCombinationJointDisp(const QUuid &uid,  const QList<STRESS> &stress)const;

    //節点反力
    void setReaction(const QUuid &uid, const QString &loadcase, const QList<QList<qreal> > &_react)
    {
        if (!jointResult.contains(uid)) jointResult[uid] = new JRESULT;
        jointResult[uid]->react[loadcase] = _react;
    }
    QList<qreal> getReaction(const QUuid &uid, const QString &loadcase, const int &step = 0)const;
    QList<qreal> getCombinationReaction(const QUuid &uid,  const QList<STRESS> &stress)const;

    //降伏状態
    void setYieldState(const QUuid &uid, const QString &loadcase,
                       const QList<QList<MRESULT::YIELD_STATE> > &_state)
    {
        if (!memberResult.contains(uid)) memberResult[uid] = new MRESULT;
        memberResult[uid]->yieldstate[loadcase] = _state;
    }
    QList<MRESULT::YIELD_STATE> getYieldState(const QUuid &uid, const QString &loadcase,
                                              const int &step = 0)const;
    //ばね
    void setSpringValue(const QUuid &uid, const QString &loadcase, const QVector<qreal> &values);
    qreal getSpringValue(const QUuid &uid, const QString &loadcase, const int &step = 0)const;

    //モード
    void setModeData(const QHash<QUuid, QVector<XYZ> > &_mode, const QVector<QVector<qreal>> &_beta,
                     const QVector<qreal> &_naturalPeriod, const QVector<qreal> &_totalMass);
    XYZ getModeDisp(const MODE_TYPE &type, const QUuid &uid, const int &nmode,
                    const qreal &dispratio = 1, const qreal &time = 0.0, const qreal &timespeedratio = 1.0) const;
    //CMDL
    void setCmdlData(const DataNBLD3D_CMDL& cmdl){
        nbld_cmdlData = cmdl;
    }

    DataNBLD3D_CMDL getCmdlData()const{return nbld_cmdlData;}

    //LOAD
    void setLoadData(DataNBLD3D_LOAD load){
        nbld_loadData = load;
    }
    DataNBLD3D_LOAD getLoadData() const{
        return nbld_loadData;
    }

    //PREA
    void setPreaData(QHash<QString, DataNBLD3D_PREA> prea){
        nbld_preaData = prea;
    }
    QHash<QString, DataNBLD3D_PREA> getPreaData() const{
        return nbld_preaData;
    }
    void setPrea(const QString model,const QUuid id, PREADATA* prea){
        QPair<QUuid,QString> pair;
        pair.first= id;
        pair.second=model;
        preaData.insert(pair,prea);
    }
    PREADATA* getPrea(const QString model , const QUuid id) const{
        QPair<QUuid,QString> pair;
        //qDebug()<<id<<model;
        pair.first= id;
        pair.second=model;
        return preaData.value(pair);
    }
    //PREM
    void setPrem(const QUuid id, PREMDATA* prem){
        premData.insert(id,prem);
    }
    PREMDATA* getPrem(const QUuid id) const{
        return premData.value(id);
    }
    //QUDS
    void setQuDsData(const QHash<QString,QUDSDATA>& quds){
        qudsData = quds; // key: XP, YP, XM, YM
        setQuDsCaseList(quds.keys());
    }
    QUDSDATA getQuDsData(const QString& casename) const{
        if(qudsData.contains(casename)){
            return qudsData.value(casename);
        }else{
            return QUDSDATA();
        }
    }
    void setNbldQuDsData(const DataNBLD3D_QUDS& quds){
        nbld_qudsData = quds;
    }
    DataNBLD3D_QUDS getNbldQuDsData() const{
        return nbld_qudsData;
    }

    //RSTQD
    void setNbldRstQDData(const QHash<QString,DataNBLD3D_RSTQD>& rstqd){
        nbld_rstQdData = rstqd;
    }
    QHash<QString,DataNBLD3D_RSTQD> getNbldRstQDData() const{
        return nbld_rstQdData;
    }
    //RST
    void setNbldRstData(const QHash<QString,DataNBLD3D_RST>& rst){
        nbld_rstData = rst;
    }
    QHash<QString,DataNBLD3D_RST> getNbldRstData() const{
        return nbld_rstData;
    }
    DataNBLD3D_RSTStep getRSTStep( const QString &loadcase, const int &step = 0) const;
    //CHCK
    void setNbldChckData(const DataNBLD3D_CHCK& chck){
        nbld_chckData = chck;
    }
    DataNBLD3D_CHCK getNbldChckData() const{
        return nbld_chckData;
    }

    //GENE
    void setNbldGeneData(const DataNBLD3D_GENE& gene){
        nbld_geneData = gene;
    }
    DataNBLD3D_GENE getNbldGeneData() const{
        return nbld_geneData;
    }

    //STEC
    void setNbldStecData(const DataNBLD3D_STEC& stec){
        nbld_stecData = stec;
    }
    DataNBLD3D_STEC getNbldStecData() const{
        return nbld_stecData;
    }

    //NMIN
    void setNbldNminData(const DataNBLD3D_NMIN& nmin){
        nbld_nminData = nmin;
    }
    DataNBLD3D_NMIN getNbldNminData() const{
        return nbld_nminData;
    }

    //余裕率・検定比
    void setNameCh(const QStringList &namech)
    {
        chckRatioData.numch = namech.count();
        chckRatioData.namech = namech;
    }
    void setAllowanceRateBending(const QHash<QUuid, QVector<QVector<float> > > &data)
    {
        chckRatioData.allowanceRateBending = data;
    }
    void setVerificationRatioShear(const QHash<QUuid, QVector<QVector<float> > > &data)
    {
        chckRatioData.verificationRatioShear = data;
    }
    void setVerificationRatioShear2(const QHash<QUuid, QVector<QVector<float> > > &data)
    {
        chckRatioData.verificationRatioShear2 = data;
    }
    void setVerificationRatioBending(const QHash<QUuid, QVector<QVector<float> > > &data)
    {
        chckRatioData.verificationRatioBending = data;
    }
    qreal getAllowanceRateBending(const QUuid &uid, const int &numch,
                                  const int &icj) const; //N-M余裕率
    qreal getVerificationRatioBending(const QUuid &uid, const int &numch,
                                      const int &icj) const; //N-M検定比
    qreal getVerificationRatioShear1(const QUuid &uid, const int &numch,
                                    const int &icj) const; //せん断検定比
    qreal getVerificationRatioShear2(const QUuid &uid, const int &numch,
                                    const int &icj) const; //せん断検定比

    //file
    void readOutputData(QDataStream &, const QString &ver = QString());
    void writeOutputData(QDataStream &, const QString &ver = QString());
    void setCurrentFile(const QString& file);
    QString currentFilePath() const;  // ファイルの絶対パス
    QString currentFileBaseName() const;  // 拡張子を除くファイル名
    QString currentFileDir() const;  // ファイルの場所（絶対パス）

    //荷重組み合わせケースの問い合わせ
    QList<STRESS> askCombiStress(const QString&) const;

//    qreal calcMemberForce(const QUuid &member_id,const int &iPos,const qreal &xPos,const QString &combi,const int &step) const;
public slots:

signals:

    void outputDataUpdated(OUTFIGTYPE);
    void fileChanged();

private:

    UnifiedOutput();
    UnifiedOutput(const UnifiedOutput &);
    QFileInfo currentFileInfo;
    UnifiedOutput &operator=(const UnifiedOutput &);

    QHash<OUTPUT_FILE_TYPE,READING_STATUS> pstnReadStatus;

    //ibaraki defined
    QHash<QUuid, MRESULT *> memberResult;
    QHash<QUuid, JRESULT *> jointResult;
    QHash<QUuid, SRESULT *> springResult;
    QHash<QPair<QUuid,QString>,PREADATA*> preaData;//uuidとmodelのペア
    QHash<QUuid,PREMDATA*> premData;
    QStringList modelCase;
    CHCKDATA chckRatioData;
    MODEDATA modeData;
    QStringList stressCase;
    QStringList qudsCase;
    QHash<QString, int> maxStep;
    QHash<QString,QUDSDATA> qudsData;

    QHash<QString, DataNBLD3D_RSTQD> nbld_rstQdData;
    QHash<QString, DataNBLD3D_RST> nbld_rstData;
    DataNBLD3D_CMDL nbld_cmdlData;
    QHash<QString, DataNBLD3D_PREA> nbld_preaData;
    DataNBLD3D_CHCK nbld_chckData;
    DataNBLD3D_LOAD nbld_loadData;
    DataNBLD3D_QUDS nbld_qudsData;
    DataNBLD3D_GENE nbld_geneData;
    DataNBLD3D_STEC nbld_stecData;
    DataNBLD3D_NMIN nbld_nminData;
  //read とwriteを書き直す
    QHash<QUuid,QPair<int,DATATYPE>> uuidIndexTable;
    QHash<QUuid,QPair<int,B3D_DATATYPE>> uuidB3dIndexTable;
    QHash<QString,int> lineSectionNameB3dIndexTable;
    QHash<QString,int> planeSectionNameB3dIndexTable;
    QHash<QString,int> damperSectionNameB3dIndexTable;
    QHash<QString,int> springSectionNameB3dIndexTable;
};
} // namespace post3dapp
#endif
