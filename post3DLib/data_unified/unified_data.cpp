#include "unified_data.h"

#include <QDebug>
#include <QProgressDialog>
#include <QMessageBox>
#include <QStringList>

#include "calc_vector2d.h"
#include "manage_floordata.h"
#include "manage_framedata.h"
#include "manage_jointdata.h"
#include "manage_loaddata.h"
#include "manage_materialdata.h"
#include "manage_memberdata.h"
#include "manage_sectiondata.h"
#include "unified_calcload.h"
#include "unified_output.h"
#include "version.h"


namespace post3dapp{
UnifiedData::UnifiedData(UnifiedDataType isout)
{
    myFloorData = new ManageFloorData();
    myFrameData = new ManageFrameData(isout);
    myMaterialData = new ManageMaterialData();
    mySectionData = new ManageSectionData(isout);
    myJointData = new ManageJointData(isout);
    myMemberData = new ManageMemberData(isout);
    myLoadData = new ManageLoadData();
}

QString UnifiedData::idToName(const QUuid &id, DATATYPE type) const
{
    if ( type == DATATYPE::TPFLOOR ) return myFloorData->idToName(id);
    if ( type == DATATYPE::TPAFLOOR ) return myFloorData->idToAnalysisName(id);
    if ( type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP )
        return myFrameData->idToName(id);
    return QString();
}

QUuid UnifiedData::nameToID(const QString &str, DATATYPE type) const
{
    //断面名などは名前からマッチできない。複数の断面が同じ名前を持つ可能性があるため
    qDebug() << "init nameToID";
    if ( type == DATATYPE::TPFLOOR ) return myFloorData->nameToID(str);
    if ( type == DATATYPE::TPAFLOOR ) return myFloorData->analysisNameToID(str);
    if ( type == DATATYPE::TPFRAMEG){
        Q_FOREACH(FRAMEGROUPVALUE fg, myFrameData->getFrameGroupList()){
            if (fg.groupName == str)
                return fg.GID;
        }
    }
    if ( type == DATATYPE::TPFRAMEP ) return myFrameData->nameToID(str);
    return QUuid();
}


QStringList UnifiedData::idsToNames(const QList<QUuid> &idlist, DATATYPE type) const
{
    if ( type == DATATYPE::TPFLOOR ) return myFloorData->idsToNames(idlist);
    if ( type == DATATYPE::TPAFLOOR ) return myFloorData->idsToAnalysisNames(idlist);
    if ( type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP )
        return myFrameData->idsToNames(idlist);
    return QStringList();
}

QList<QUuid> UnifiedData::namesToIDs(const QStringList &strlist, DATATYPE type) const
{
    if ( type == DATATYPE::TPFLOOR ) return myFloorData->namesToIDs(strlist);
    if ( type == DATATYPE::TPAFLOOR ) return myFloorData->analysisNamesToIDs(strlist);
    if ( type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP )
        return myFrameData->namesToIDs(strlist);
    return QList<QUuid>();
}

bool UnifiedData::isExistedName(const QString &str, DATATYPE type) const
{
    if ( type == DATATYPE::TPFLOOR ) {
        return myFloorData->isExistedName(str);
    } else if ( type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP ) {
        return myFrameData->isExistedName(str);
    } else if ( type == DATATYPE::TPMACONC || type == DATATYPE::TPMASTEEL
                || type == DATATYPE::TPMAREIN || type == DATATYPE::TPMAREIND ) {
        return myMaterialData->isExistedName(str, type);
    } else if ( type == DATATYPE::TPCOLUMN || type == DATATYPE::TPGIRDER || type == DATATYPE::TPBRACE || type == DATATYPE::TPBEAM
                || type == DATATYPE::TPSLAB || type == DATATYPE::TPWALL || type == DATATYPE::TPUWALL
                || type == DATATYPE::TPBASE || type == DATATYPE::TPWALLOPEN || type == DATATYPE::TPDAMPER || type == DATATYPE::TPISO ) {
        return mySectionData->isExistedName(str, type);
    } else if ( type == DATATYPE::TPSLABLOAD || type == DATATYPE::TPMEMBERLOAD || type == DATATYPE::TPJOINTLOAD ) {
        return myLoadData->isExistedName(str, type);
    }else{
        qDebug()<<"UnifiedData::isExistedName() invalid datatype";
        return false;
    }
}

bool UnifiedData::isExistedID(const QUuid &id, DATATYPE type) const
{
    if ( type == DATATYPE::TPFLOOR || type == DATATYPE::TPAFLOOR ) {
        return myFloorData->isExistedID(id);
    } else if ( type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP ) {
        return myFrameData->isExistedID(id, type);
    } else if ( type == DATATYPE::TPJOINT ) {
        return myJointData->isExistedID(id);
    } else if ( type == DATATYPE::TPCOLUMN || type == DATATYPE::TPGIRDER || type == DATATYPE::TPBRACE || type == DATATYPE::TPBEAM
                || type == DATATYPE::TPSLAB || type == DATATYPE::TPWALL || type == DATATYPE::TPUWALL
                || type == DATATYPE::TPBASE || type == DATATYPE::TPWALLOPEN || type == DATATYPE::TPDAMPER || type == DATATYPE::TPISO
                || type == DATATYPE::TPSLABLOAD || type == DATATYPE::TPMEMBERLOAD || type == DATATYPE::TPJOINTLOAD ) {
        return myMemberData->isExistedID(id);
    }else{
        qDebug()<<"UnifiedData::isExistedID() invalid datatype";
        return false;
    }
}

QList<QUuid> UnifiedData::getIDList(DATATYPE type) const
{
    if ( type == DATATYPE::TPFLOOR ) return myFloorData->getIDList();
    if ( type == DATATYPE::TPAFLOOR ) return myFloorData->getAnalysisIDList();
    if ( type == DATATYPE::TPFRAMEG  ) return myFrameData->getGroupIDList();
    if (  type == DATATYPE::TPFRAMEP ) return myFrameData->getIDList();
    if(type == DATATYPE::TPJOINT) return myJointData->getIDList();
    else if ( type == DATATYPE::TPCOLUMN || type == DATATYPE::TPGIRDER || type == DATATYPE::TPBRACE || type == DATATYPE::TPBEAM ||
            type == DATATYPE::TPSLAB || type == DATATYPE::TPWALL || type == DATATYPE::TPUWALL ||
            type == DATATYPE::TPBASE || type == DATATYPE::TPWALLOPEN || type == DATATYPE::TPDAMPER || type == DATATYPE::TPISO ) {
    return myMemberData->getIDList();
    }
    return QList<QUuid>();
}
QUuid UnifiedData::noToID(const int no, DATATYPE type) const
{
    qDebug() << "init noToID";
    if ( type == DATATYPE::TPJOINT ) return myJointData->analysisJointNoToID(no);
    else qDebug()<<"noToID not defined";
    return QUuid();
}


QStringList UnifiedData::getNameList(DATATYPE type) const
{
    if ( type == DATATYPE::TPFLOOR ) {
        return myFloorData->getNameList();
    } else if ( type == DATATYPE::TPAFLOOR ) {
        return myFloorData->getAnalysisNameList();
    } else if ( type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP ) {
        return myFrameData->getNameList();
    } else if ( type == DATATYPE::TPMACONC || type == DATATYPE::TPMASTEEL || type == DATATYPE::TPMAREIN || type == DATATYPE::TPMAREIND ) {
        return myMaterialData->getNameList(type);
    } else if ( type == DATATYPE::TPCOLUMN || type == DATATYPE::TPGIRDER || type == DATATYPE::TPBRACE || type == DATATYPE::TPBEAM ||
                type == DATATYPE::TPSLAB || type == DATATYPE::TPWALL || type == DATATYPE::TPUWALL ||
                type == DATATYPE::TPBASE || type == DATATYPE::TPWALLOPEN || type == DATATYPE::TPDAMPER || type == DATATYPE::TPISO ) {
        return mySectionData->getNameList(type);
    } else if ( type == DATATYPE::TPJOINTLOAD || type == DATATYPE::TPMEMBERLOAD || type == DATATYPE::TPSLABLOAD ) {
        return myLoadData->getNameList(type);
    }
    return QStringList();
}

// 鉄筋材種　SD295...
QStringList UnifiedData::getReinforceNameList() const
{
    return myMaterialData->getReinforceNameList();
}

QString UnifiedData::dataTypeToJapanese(DATATYPE dtype)
{
    if (dtype == DATATYPE::TPFLOOR) {
        return u8"階";
    } else if (dtype == DATATYPE::TPAFLOOR) {
        return u8"解析階";
    } else if (dtype == DATATYPE::TPFRAMEG) {
        return u8"通りグループ";
    } else if (dtype == DATATYPE::TPFRAMEP) {
        return u8"通り";
    } else if (dtype == DATATYPE::TPMACONC) {
        return u8"コンクリート材種";
    } else if (dtype == DATATYPE::TPMASTEEL) {
        return u8"鉄骨材種";
    } else if (dtype == DATATYPE::TPMAREIN || dtype == DATATYPE::TPMAREIND) {
        return u8"鉄筋材種";
    } else if (dtype == DATATYPE::TPJOINT) {
        return u8"形状節点";
    } else if (dtype == DATATYPE::TPCOLUMN) {
        return u8"柱";
    } else if (dtype == DATATYPE::TPGIRDER) {
        return u8"大梁";
    } else if (dtype == DATATYPE::TPBRACE) {
        return u8"ブレース";
    } else if (dtype == DATATYPE::TPBEAM) {
        return u8"小梁";
    } else if (dtype == DATATYPE::TPSLAB) {
        return u8"スラブ";
    } else if (dtype == DATATYPE::TPWALL) {
        return u8"壁";
    } else if (dtype == DATATYPE::TPUWALL) {
        return u8"地下外壁";
    } else if (dtype == DATATYPE::TPBASE) {
        return u8"柱脚";
    } else if (dtype == DATATYPE::TPWALLOPEN) {
        return u8"壁開口";
    } else if (dtype == DATATYPE::TPDAMPER) {
        return u8"ダンパー";
    } else if (dtype == DATATYPE::TPISO) {
        return u8"免震部材";
    } else if (dtype == DATATYPE::TPSLABLOAD) {
        return u8"スラブ荷重";
    } else if (dtype == DATATYPE::TPMEMBERLOAD) {
        return u8"部材荷重";
    } else if (dtype == DATATYPE::TPJOINTLOAD) {
        return u8"節点荷重";
    } else if (dtype == DATATYPE::TPSPRING) {
        return u8"ばね";
    }
    return QString();
}

QString UnifiedData::dataTypeToEnglish(DATATYPE dtype)
{
    if (dtype == DATATYPE::TPFLOOR) {
        return "Floor";
    } else if (dtype == DATATYPE::TPAFLOOR) {
        return "AnalysisFloor";
    } else if (dtype == DATATYPE::TPFRAMEG) {
        return "FrameGroup";
    } else if (dtype == DATATYPE::TPFRAMEP) {
        return "Frame";
    } else if (dtype == DATATYPE::TPMACONC) {
        return "ConcreteMaterial";
    } else if (dtype == DATATYPE::TPMASTEEL) {
        return "SteelMaterial";
    } else if (dtype == DATATYPE::TPMAREIN || dtype == DATATYPE::TPMAREIND) {
        return "ReinforceMaterial";
    } else if (dtype == DATATYPE::TPJOINT) {
        return "Joint";
    } else if (dtype == DATATYPE::TPCOLUMN) {
        return "Column";
    } else if (dtype == DATATYPE::TPGIRDER) {
        return "Girder";
    } else if (dtype == DATATYPE::TPBRACE) {
        return "Brace";
    } else if (dtype == DATATYPE::TPBEAM) {
        return "Beam";
    } else if (dtype == DATATYPE::TPSLAB) {
        return "Slab";
    } else if (dtype == DATATYPE::TPWALL) {
        return "Wall";
    } else if (dtype == DATATYPE::TPUWALL) {
        return "UnderGroundWall";
    } else if (dtype == DATATYPE::TPBASE) {
        return "ColumnBase";
    } else if (dtype == DATATYPE::TPWALLOPEN) {
        return "WallOpen";
    } else if (dtype == DATATYPE::TPDAMPER) {
        return "DamperElement";
    } else if (dtype == DATATYPE::TPISO) {
        return "IsolationElement";
    } else if (dtype == DATATYPE::TPSLABLOAD) {
        return "SlabLoad";
    } else if (dtype == DATATYPE::TPMEMBERLOAD) {
        return "MemberLoad";
    } else if (dtype == DATATYPE::TPJOINTLOAD) {
        return "JointLoad";
    }
    return QString();
}

DATATYPE UnifiedData::stringToDataType(const QString &str)
{
    if ( str == "Floor" || str == u8"階" ) {
        return DATATYPE::TPFLOOR;
    } else if ( str == "AnalysisFloor" || str == u8"解析階" ) {
        return DATATYPE::TPAFLOOR;
    } else if ( str == "FrameGroup" || str == u8"通りグループ" ) {
        return DATATYPE::TPFRAMEG;
    } else if ( str == "Frame" || str == u8"通り" ) {
        return DATATYPE::TPFRAMEP;
    } else if ( str == "ConcreteMaterial" || str == u8"コンクリート材種") {
        return DATATYPE::TPMACONC;
    } else if ( str == "SteelMaterial" || str == u8"鉄骨材種") {
        return DATATYPE::TPMASTEEL;
    } else if ( str == "ReinforceMaterial" || str == u8"鉄骨材種") {
        return DATATYPE::TPMAREIN;
    } else if ( str == "Joint" || str == u8"形状節点") {
        return DATATYPE::TPJOINT;
    } else if ( str == "Column" || str == u8"柱") {
        return DATATYPE::TPCOLUMN;
    } else if ( str == "Girder" || str == u8"大梁") {
        return DATATYPE::TPGIRDER;
    } else if ( str == "Brace" || str == u8"ブレース") {
        return DATATYPE::TPBRACE;
    } else if ( str == "Beam" || str == u8"小梁") {
        return DATATYPE::TPBEAM;
    } else if ( str == "Slab" || str == u8"スラブ") {
        return DATATYPE::TPSLAB;
    } else if ( str == "Wall" || str == u8"壁") {
        return DATATYPE::TPWALL;
    } else if ( str == "UnderGroundWall" || str == u8"地下外壁") {
        return DATATYPE::TPUWALL;
    } else if ( str == "ColumnBase" || str == u8"柱脚") {
        return DATATYPE::TPBASE;
    } else if ( str == "WallOpen" || str == u8"壁開口") {
        return DATATYPE::TPWALLOPEN;
    } else if ( str == "DamperElement" || str == u8"ダンパー") {
        return DATATYPE::TPDAMPER;
    } else if ( str == "IsolationElement" || str == u8"免震部材") {
        return DATATYPE::TPISO;
    } else if ( str == "SlabLoad" || str == u8"スラブ荷重") {
        return DATATYPE::TPSLABLOAD;
    } else if ( str == "MemberLoad" || str == u8"部材荷重") {
        return DATATYPE::TPMEMBERLOAD;
    } else if ( str == "JointLoad" || str == u8"節点荷重") {
        return DATATYPE::TPJOINTLOAD;
    }
    return DATATYPE::NODATATYPE;
}

int UnifiedData::getDataCount(DATATYPE type, bool isList) const
{
    if ( type == DATATYPE::TPFLOOR || type == DATATYPE::TPAFLOOR ) {
        return myFloorData->dataCount(type);
    } else if ( type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP ) {
        return myFrameData->dataCount();
    } else if ( type == DATATYPE::TPMACONC || type == DATATYPE::TPMASTEEL || type == DATATYPE::TPMAREIN || type == DATATYPE::TPMAREIND ) {
        return myMaterialData->dataCount(type);
    } else if ( type == DATATYPE::TPJOINT ) {
        return myJointData->dataCount();
    } else if (type == DATATYPE::TPCOLUMN || type == DATATYPE::TPGIRDER || type == DATATYPE::TPBRACE || type == DATATYPE::TPBEAM ||
               type == DATATYPE::TPSLAB || type == DATATYPE::TPWALL ||type == DATATYPE::TPUWALL ||
               type == DATATYPE::TPBASE || type == DATATYPE::TPWALLOPEN || type == DATATYPE::TPDAMPER || type == DATATYPE::TPISO ) {
        return (isList) ? mySectionData->dataCount(type) : myMemberData->dataCount(type) ;
    } else if (type == DATATYPE::TPJOINTLOAD || type == DATATYPE::TPMEMBERLOAD || type == DATATYPE::TPSLABLOAD ) {
        return (isList) ? myLoadData->dataCount(type) : myMemberData->dataCount(type);
    }

    return 0;
}

QHash<QString, int> UnifiedData::getMemberSectionCount(DATATYPE type)
{
    return myMemberData->getMemberSectionCount(*this, type);
}


/* シグナルの強制発生 */
void UnifiedData::signalJointAppended(const QList<JointData *> &jdlist)
{
    if ( jdlist.isEmpty() ) return;
    emit jointAppended( jdlist );

    // Log Message
    QString lmsg = QString("create Joints. \"count=%1\"").arg(jdlist.count());
    emit signalSendLogMessage(lmsg);
}

void UnifiedData::signalJointChanged(const QList<JointData *> &jdlist)
{
    if ( jdlist.isEmpty() ) return;
    emit jointChanged( jdlist );

    // Log Message
    QString lmsg = QString("change Joints. \"count=%1\"").arg(jdlist.count());
    emit signalSendLogMessage(lmsg);
}

void UnifiedData::signalJointRemoved(const QList<JointData *> &jdlist)
{
    if ( jdlist.isEmpty() ) return;
    emit jointRemoved( jdlist );

    // Log Message
    QString lmsg = QString("remove Joints. \"count=%1\"").arg(jdlist.count());
    emit signalSendLogMessage(lmsg);
}

void UnifiedData::signalMemberAppended(const QList<MemberData *> &mdlist)
{
    if ( mdlist.isEmpty() ) return;
    emit memberAppended( mdlist );

    // Log Message
    QString lmsg = QString("create Members. \"count=%1\"").arg(mdlist.count());
    emit signalSendLogMessage(lmsg);
}

void UnifiedData::signalMemberChanged(const QList<MemberData *> &mdlist)
{
    if ( mdlist.isEmpty() ) return;
    emit memberChanged( mdlist );

    // Log Message
    QString lmsg = QString("change Members. \"count=%1\"").arg(mdlist.count());
    emit signalSendLogMessage(lmsg);
}

void UnifiedData::signalMemberRemoved(const QList<MemberData *> &mdlist, DATATYPE dtype)
{
    if ( mdlist.isEmpty() ) return;
    emit memberRemoved( mdlist, dtype );

    // Log Message
    QString lmsg = QString("remove Members. \"type=") + dataTypeToEnglish(dtype)
                   + QString(", count=%1\"").arg(mdlist.count());
    emit signalSendLogMessage(lmsg);
}

void UnifiedData::signalElementCountChanged()
{
    emit elementCountChanged();
}


/* 階に関する問い合わせ  */

FLOORVALUES  UnifiedData::getFloorValue(const QUuid &uid) const
{
    return myFloorData->getFloorValues(uid);
}

QList<FLOORVALUES> UnifiedData::getFloorList() const
{
    return myFloorData->getFloorList();
}

qreal UnifiedData::getAdjustedFloorLevel(const QUuid &uid) const
{
    return myFloorData->getAdjustedFloorLevel(uid);
}

bool UnifiedData::isChildFloor(const QUuid &uid) const
{
    return myFloorData->isChildFloor(uid);
}

qreal UnifiedData::getFloorHeight(const QUuid &uid) const
{
    return myFloorData->getFloorHeight(uid);
}

qreal UnifiedData::getMinimumFloorHeight() const
{
    return myFloorData->getMinimumFloorHeight();
}

qreal UnifiedData::getMaximumFloorHeight() const
{
    return myFloorData->getMaximumFloorHeight();
}

QList<QUuid> UnifiedData::getFloorListInRange(qreal hmin, qreal hmax) const
{
    return myFloorData->getFloorListInRange(hmin, hmax);
}

QList<QUuid> UnifiedData::getNearlyFloorOfHeight(qreal h) const
{
    return myFloorData->getNearlyFloorOfHeight(h);
}

QList<QUuid> UnifiedData::getNearlyFloorOfHeight(const QList<QUuid> &idlist, qreal h) const
{
    return myFloorData->getNearlyFloorOfHeight(idlist, h);
}

QList<QUuid> UnifiedData::sortFloorOrderOfHeight(const QList<QUuid> &idlist) const
{
    return myFloorData->sortFloorOrderOfHeight(idlist);
}

QUuid UnifiedData::getUpperFloorID(const QUuid &fl, int iu) const
{
    return myFloorData->getUpperFloorID(fl, iu);
}

QList<QUuid> UnifiedData::getFloorRange(const QUuid &id1, const QUuid &id2) const
{
    return myFloorData->getFloorRange(id1, id2);
}

QUuid UnifiedData::getNearlyDownFloorID(qreal hh) const
{
    return myFloorData->getNearlyDownFloorID(hh);
}

bool UnifiedData::withinFloors(const QUuid &f1, const QUuid &f2, qreal hh) const
{
    return myFloorData->withinFloors(f1, f2, hh);
}

bool UnifiedData::withinFloors(const QUuid &f1, const QUuid &f2, const QUuid &f3) const
{
    return myFloorData->withinFloors(f1, f2, f3);
}

QUuid UnifiedData::getFloorOfHeight(qreal hh) const
{
    return myFloorData->getFloorOfHeight(hh);
}

QUuid UnifiedData::getBaseLevelFloor() const
{
    return myFloorData->getBaseLevelFloor();
}

QUuid UnifiedData::getRoofTopFloor() const
{
    return myFloorData->getRoofTopFloor();
}


/* 通りに関する問い合わせ  */

QList<FRAMEPOINTSVALUE> UnifiedData::getFrameValuesOfFloor(const QUuid &fid) const
{
    return myFrameData->getFrameValuesOfFloor(fid );
}

FRAMEPOINTSVALUE UnifiedData::getFrameValueOfFloor(const QUuid &fr_id, const QUuid &fl_id ) const
{
    return myFrameData->getFrameValueOfFloor(fr_id, fl_id );
}

QList<QPointF> UnifiedData::getFramePointsOfFloor(const QUuid &fr_id, const QUuid &fl_id ) const
{
    return myFrameData->getFramePointsOfFloor(fr_id, fl_id );
}

FrameType::FRAMETYPE UnifiedData::getFrameTypeOfFloor( const QUuid &fr_id, const QUuid &fl_id ) const
{
    return myFrameData->getFrameTypeOfFloor(fr_id, fl_id);
}

QList<QUuid> UnifiedData::getFloorListOfFrame( const QUuid &fr_id) const
{
    return myFrameData->getFloorListOfFrame(fr_id );
}

QList<QUuid> UnifiedData::getFrameRange(const QUuid &id1, const QUuid &id2) const
{
    return myFrameData->getFrameRange(id1, id2);
}

/*
XYZ UnifiedData::framePointToGlobalXYZ(const QUuid &fr_id, QPointF p0) const
{
    return myFrameData->framePointToGlobalXYZ(fr_id,p0);
}

QList<XYZ> UnifiedData::framePointsToGlobalXYZ(const QUuid &fr_id, const QList<QPointF>& p0_list) const
{
    return myFrameData->framePointsToGlobalXYZ(fr_id,p0_list);
}

QPointF UnifiedData::GlobalXYZToFramePoint(const QUuid &fr_id, XYZ p0) const
{
    return myFrameData->GlobalXYZToFramePoint(fr_id,p0);
}

QList<QPointF> UnifiedData::GlobalXYZToFramePoints(const QUuid &fr_id, const QList<XYZ>& p0_list) const
{
    return myFrameData->GlobalXYZToFramePoints(fr_id,p0_list);
}
*/

QList<FRAMEGROUPVALUE> UnifiedData::getFrameGroupList()const{
    return myFrameData->getFrameGroupList();
}
QList<FRAMEPOINTSVALUE> UnifiedData::getFramePointList()const{
    return myFrameData->getFramePointList();
}

QUuid UnifiedData::getJointId(const QString& floorName, const QString& gridName1, const QString& gridName2)
{
    auto floorId = idToName(floorName, DATATYPE::TPFLOOR);
    auto grid1Id = idToName(gridName1, DATATYPE::TPFRAMEP);
    auto grid2Id = idToName(gridName2, DATATYPE::TPFRAMEP);
    auto floorJoints = jointListOfFloor(floorId);
    QList<QUuid> jList;
    foreach(auto j, floorJoints){
        if(j->containsFrame(grid1Id) && j->containsFrame(grid2Id)){
            jList.append(j->getUuid());
        }
    }
    if (jList.count() == 1)
        return jList.first();
    else
        throw;
}

XYZ UnifiedData::frameXYZToGlobalXYZ(const QUuid &fr_id, XYZ p0) const
{
    return myFrameData->frameXYZToGlobalXYZ(fr_id, p0 );
}

QList<XYZ> UnifiedData::frameXYZToGlobalXYZ(const QUuid &fr_id, const QList<XYZ> &p0_list) const
{
    return myFrameData->frameXYZToGlobalXYZ(fr_id, p0_list );
}

XYZ UnifiedData::globalXYZToFrameXYZ(const QUuid &fr_id, XYZ p0) const
{
    return myFrameData->globalXYZToFrameXYZ(fr_id, p0 );
}

QList<XYZ> UnifiedData::globalXYZToFrameXYZ(const QUuid &fr_id, const QList<XYZ> &p0_list) const
{
    return myFrameData->globalXYZToFrameXYZ(fr_id, p0_list );
}

qreal UnifiedData::globalXYZToFrameAngle(const QUuid &fr_id, XYZ p0) const
{
    return myFrameData->globalXYZToFrameAngle(fr_id, p0 );
}

qreal UnifiedData::globalZToClosedFrameEndX(const QUuid &fr_id, qreal z) const
{
    return myFrameData->globalZToClosedFrameEndX(fr_id, z );
}

/* チェックと一時データの保持  */

bool UnifiedData::checkTempData(QWidget *parent, const QList<FLOORVALUES> &list) const
{
    return ( myFloorData->checkTempData( parent, list ) );
}

bool UnifiedData::checkTempData(QWidget *parent, const QList<FRAMEGROUPVALUE> &glist,
                                const QList<FRAMEPOINTSVALUE> &flist) const
{
    return ( myFrameData->checkTempData( parent, glist, flist  ) );
}

bool UnifiedData::checkTempData(QWidget *parent, const QList<MATERIALVALUES> &list) const
{
    return ( myMaterialData->checkTempData( parent, list ) );
}

/*
bool UnifiedData::checkTempData(QWidget* parent,DATATYPE dtype,
                                   const QList<SECTIONVALUES> & slist ) const
{
    if(dtype==TPCOLUMN){ return mySectionData->checkColumnData(parent, slist); }
    else if(dtype==TPGIRDER){ return mySectionData->checkGirderData(parent, slist); }
    else if(dtype==TPBRACE){ return mySectionData->checkBraceData(parent, slist); }
    else if(dtype==TPBEAM){ return mySectionData->checkBeamData(parent, slist); }
    else if(dtype==TPSLAB){ return mySectionData->checkUWallData(parent, slist); }
    else if(dtype==TPWALL){ return mySectionData->checkWallData(parent, slist); }
    else if(dtype==TPUWALL){ return mySectionData->checkSlabData(parent, slist); }
    else if(dtype==TPBASE){ return mySectionData->checkOpenData(parent, slist); }
    else if(dtype==TPWALLOPEN){ return mySectionData->checkBaseData(parent, slist); }
    else if(dtype==TPDAMPER){ return mySectionData->checkDamperData(parent, slist); }
    else if(dtype==TPISO){ return mySectionData->checkIsoData(parent, slist); }
    return true;
}
*/

bool UnifiedData::checkTempData(QWidget *parent, DATATYPE dtype,
                                const QList<SectionValues> &slist) const
{
    if (dtype == DATATYPE::TPCOLUMN) {
        return mySectionData->checkColumnData(parent, slist);
    } else if (dtype == DATATYPE::TPGIRDER) {
        return mySectionData->checkGirderData(parent, slist);
    } else if (dtype == DATATYPE::TPBRACE) {
        return mySectionData->checkBraceData(parent, slist);
    } else if (dtype == DATATYPE::TPBEAM) {
        return mySectionData->checkBeamData(parent, slist);
    } else if (dtype == DATATYPE::TPSLAB) {
        return mySectionData->checkSlabData(parent, slist);
    } else if (dtype == DATATYPE::TPWALL) {
        return mySectionData->checkWallData(parent, slist);
    } else if (dtype == DATATYPE::TPUWALL) {
        return mySectionData->checkUWallData(parent, slist);
    } else if (dtype == DATATYPE::TPWALLOPEN) {
        return mySectionData->checkOpenData(parent, slist);
    } else if (dtype == DATATYPE::TPBASE) {
        return mySectionData->checkBaseData(parent, slist);
    } else if (dtype == DATATYPE::TPDAMPER) {
        return mySectionData->checkDamperData(parent, slist);
    } else if (dtype == DATATYPE::TPISO) {
        return mySectionData->checkIsoData(parent, slist);
    }
    return true;
}

bool UnifiedData::checkTempData(QWidget *parent, const QList<JOINTLOADVALUES> &list) const
{
    return ( myLoadData->checkJointLoadData( parent, list ) );
}

bool UnifiedData::checkTempData(QWidget *parent, const QList<MEMBERLOADVALUES> &list) const
{
    return ( myLoadData->checkMemberLoadData( parent, list ) );
}

bool UnifiedData::checkTempData(QWidget *parent, const QList<SLABLOADVALUES> &list) const
{
    return ( myLoadData->checkSlabLoadData( parent, list ) );
}

/* 一時データの登録  */

void UnifiedData::registerTempData(DATATYPE type)
{
    if ( type == DATATYPE::TPFLOOR ) {
        myFloorData->registerTempData();
    } else if ( type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP ) {
        myFrameData->registerTempData();
    } else if ( type == DATATYPE::TPMACONC || type == DATATYPE::TPMASTEEL || type == DATATYPE::TPMAREIN || type == DATATYPE::TPMAREIND ) {
        myMaterialData->registerTempData();
    } else if ( type == DATATYPE::TPCOLUMN ) {
        mySectionData->registerColumnData();
    } else if ( type == DATATYPE::TPGIRDER ) {
        mySectionData->registerGirderData();
    } else if ( type == DATATYPE::TPBRACE ) {
        mySectionData->registerBraceData();
    } else if ( type == DATATYPE::TPBEAM ) {
        mySectionData->registerBeamData();
    } else if ( type == DATATYPE::TPSLAB ) {
        mySectionData->registerSlabData();
    } else if ( type == DATATYPE::TPWALL ) {
        mySectionData->registerWallData();
    } else if ( type == DATATYPE::TPUWALL ) {
        mySectionData->registerUWallData();
    } else if ( type == DATATYPE::TPBASE ) {
        mySectionData->registerBaseData();
    } else if ( type == DATATYPE::TPWALLOPEN ) {
        mySectionData->registerOpenData();
    } else if ( type == DATATYPE::TPDAMPER ) {
        mySectionData->registerDamperData();
    } else if ( type == DATATYPE::TPISO ) {
        mySectionData->registerIsoData();
    } else if ( type == DATATYPE::TPJOINTLOAD ) {
        myLoadData->registerJointLoadData();
    } else if ( type == DATATYPE::TPMEMBERLOAD ) {
        myLoadData->registerMemberLoadData();
    } else if ( type == DATATYPE::TPSLABLOAD ) {
        myLoadData->registerSlabLoadData();
    }

    emit dataUpdated(type);
    emit elementCountChanged();
    if ( type == DATATYPE::TPFRAMEG ) emit dataUpdated(DATATYPE::TPFRAMEP);
    if ( type == DATATYPE::TPMACONC ) {
        emit dataUpdated(DATATYPE::TPMASTEEL);
        emit dataUpdated(DATATYPE::TPMAREIN);
        emit dataUpdated(DATATYPE::TPMAREIND);
    }

    QString msg;
    if (type == DATATYPE::TPFLOOR) {
        msg = tr("defined Floor.");
    } else if (type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP) {
        msg = tr("defined Frame.");
    } else if (type == DATATYPE::TPMACONC || type == DATATYPE::TPMASTEEL
               || type == DATATYPE::TPMAREIN || type == DATATYPE::TPMAREIND ) {
        msg = tr("defined Material.");
    } else if (type == DATATYPE::TPCOLUMN) {
        msg = tr("defined Column List.");
    } else if (type == DATATYPE::TPGIRDER) {
        msg = tr("defined Girder List.");
    } else if (type == DATATYPE::TPBRACE) {
        msg = tr("defined Brace List.");
    } else if (type == DATATYPE::TPBEAM) {
        msg = tr("defined Beam List.");
    } else if (type == DATATYPE::TPSLAB) {
        msg = tr("defined Slab List.");
    } else if (type == DATATYPE::TPWALL) {
        msg = tr("defined Wall List.");
    } else if (type == DATATYPE::TPUWALL) {
        msg = tr("defined UnderGroundWall List.");
    } else if (type == DATATYPE::TPBASE) {
        msg = tr("defined ColumnBase List.");
    } else if (type == DATATYPE::TPWALLOPEN) {
        msg = tr("defined WallOpen List.");
    } else if (type == DATATYPE::TPDAMPER) {
        msg = tr("defined DamperElement List.");
    } else if (type == DATATYPE::TPISO) {
        msg = tr("defined IsolationElement List.");
    } else if (type == DATATYPE::TPJOINTLOAD) {
        msg = tr("defined JointLoad List.");
    } else if (type == DATATYPE::TPMEMBERLOAD) {
        msg = tr("defined MemberLoad List.");
    } else if (type == DATATYPE::TPSLABLOAD) {
        msg = tr("defined SlabLoad List.");
    }

    if (type == DATATYPE::TPFRAMEG) {
        QStringList strlist = getNameList(DATATYPE::TPFRAMEG);
        msg += tr(" FrameGroup:\"");
        msg += strlist.join(",");
        msg += "\"";
        strlist = getNameList(DATATYPE::TPFRAMEP);
        msg += tr(" Frame:\"");
        msg += strlist.join(",");
        msg += "\"";
    } else {
        QStringList strlist = getNameList(type);
        msg += " \"";
        msg += strlist.join(",");
        msg += "\"";
    }
    emit signalSendLogMessage(msg);
}

void UnifiedData::registerAnalysisFloorData(const QList<FLOORVALUES> &fvlist,
                                            const QUuid &b_id, const QUuid &t_id )
{
    myFloorData->registerAnalysisFloorData(fvlist, b_id, t_id);
    emit dataUpdated(DATATYPE::TPAFLOOR);
}

/* 一時データの破棄  */

void UnifiedData::clearTempData(DATATYPE type)
{
    if (type == DATATYPE::TPFLOOR) {
        myFloorData->clearTempData();
    } else if (type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP) {
        myFrameData->clearTempData();
    } else if (type == DATATYPE::TPMACONC || type == DATATYPE::TPMASTEEL
               || type == DATATYPE::TPMAREIN || type == DATATYPE::TPMAREIND ) {
        myMaterialData->clearTempData();
    } else if (type == DATATYPE::TPCOLUMN || type == DATATYPE::TPGIRDER || type == DATATYPE::TPBRACE || type == DATATYPE::TPBEAM ||
               type == DATATYPE::TPSLAB || type == DATATYPE::TPWALL  || type == DATATYPE::TPUWALL ||
               type == DATATYPE::TPBASE || type == DATATYPE::TPWALLOPEN || type == DATATYPE::TPDAMPER || type == DATATYPE::TPISO ) {
        mySectionData->clearTempData();
    } else if (type == DATATYPE::TPJOINTLOAD || type == DATATYPE::TPMEMBERLOAD || type == DATATYPE::TPSLABLOAD) {
        myLoadData->clearTempData();
    }
}

/* ファイル読込、ファイル保存 */

void UnifiedData::registerTempData(const QList<FLOORVALUES> &list)
{
    myFloorData->registerTempData( list );
    emit dataUpdated(DATATYPE::TPFLOOR);
}

void UnifiedData::registerTempData(const QList<FRAMEGROUPVALUE> &glist,
                                   const QList<FRAMEPOINTSVALUE> &flist)
{
    myFrameData->registerTempData( glist, flist );
//    emit dataUpdated(TPFRAMEG);
    emit dataUpdated(DATATYPE::TPFRAMEP);
}

void UnifiedData::registerTempData(const QList<MATERIALVALUES> &list)
{
    myMaterialData->registerTempData( list );
    emit dataUpdated(DATATYPE::TPMACONC);
    emit dataUpdated(DATATYPE::TPMASTEEL);
    emit dataUpdated(DATATYPE::TPMAREIN);
    emit dataUpdated(DATATYPE::TPMAREIND);
}

void UnifiedData::registerTempData(DATATYPE type, const QList<SectionValues> &slist)
{
    mySectionData->registerTempData( type, slist );
    emit dataUpdated(type);
    emit elementCountChanged();
}

void UnifiedData::registerTempData(const QList<JOINTLOADVALUES> &slist)
{
    myLoadData->registerJointLoadData( slist );
    emit dataUpdated(DATATYPE::TPJOINTLOAD);
    emit elementCountChanged();
}

void UnifiedData::registerTempData(const QList<MEMBERLOADVALUES> &slist)
{
    myLoadData->registerMemberLoadData( slist );
    emit dataUpdated(DATATYPE::TPMEMBERLOAD);
    emit elementCountChanged();
}

void UnifiedData::registerTempData(const QList<SLABLOADVALUES> &slist)
{
    myLoadData->registerSlabLoadData( slist );
    emit dataUpdated(DATATYPE::TPSLABLOAD);
    emit elementCountChanged();
}

void UnifiedData::clearDefinedData()
{
    myFloorData->clearData();
    myFrameData->clearData();
    myMaterialData->clearData();
    mySectionData->clearData();
    myLoadData->clearData();
    emit dataUpdated(DATATYPE::TPFLOOR);
    emit dataUpdated(DATATYPE::TPAFLOOR);
//    emit dataUpdated(TPFRAMEG);
    emit dataUpdated(DATATYPE::TPFRAMEP);
    emit dataUpdated(DATATYPE::TPMACONC);
    emit dataUpdated(DATATYPE::TPMASTEEL);
    emit dataUpdated(DATATYPE::TPMAREIN);
    emit dataUpdated(DATATYPE::TPMAREIND);
    emit dataUpdated(DATATYPE::TPCOLUMN);
    emit dataUpdated(DATATYPE::TPGIRDER);
    emit dataUpdated(DATATYPE::TPBRACE);
    emit dataUpdated(DATATYPE::TPBEAM);
    emit dataUpdated(DATATYPE::TPSLAB);
    emit dataUpdated(DATATYPE::TPWALL);
    emit dataUpdated(DATATYPE::TPUWALL);
    emit dataUpdated(DATATYPE::TPDAMPER);
    emit dataUpdated(DATATYPE::TPISO);
    emit dataUpdated(DATATYPE::TPWALLOPEN);
    emit dataUpdated(DATATYPE::TPBASE);
    emit dataUpdated(DATATYPE::TPJOINTLOAD);
    emit dataUpdated(DATATYPE::TPMEMBERLOAD);
    emit dataUpdated(DATATYPE::TPSLABLOAD);
}

bool UnifiedData::checkUnifiedData( QDataStream &infile, const LibraryVersion &ver ) const
{
    qDebug()<<"chk unified data";
    bool ok = myJointData->checkData(infile, ver);
    if ( !ok ) return false;
    return myMemberData->checkData(infile, ver);
}


void UnifiedData::readTextJointData( QTextStream &infile, const LibraryVersion &ver)
{
    myJointData->readData(infile, ver);
    emit dataReaded();
    emit elementCountChanged();
}
void UnifiedData::readTextMemberData( QTextStream &infile, DATATYPE type,const LibraryVersion &ver)
{
    myMemberData->readData(infile,type, ver);
    emit dataReaded();
    emit elementCountChanged();
}

void UnifiedData::readAdditionalData( QDataStream &infile, const LibraryVersion &ver,
                                      const QHash<QUuid, QString> &fidToName, const FILEREADPATTERN &fpt )
{
    QHash<QUuid, QUuid> idToId = myJointData->readAdditionalData(infile, ver, fidToName, fpt);
    myMemberData->readAdditionalData(infile, ver, idToId, fpt);
    emit dataReaded();
    emit elementCountChanged();
}

void UnifiedData::writeLimitedData(QDataStream &outfile,
                                   const QList<QUuid> &limitedFloors) const
{
    QList<JointData *> jdlist;
    Q_FOREACH ( JointData *jd, this->jointListOfAll() )
        if ( jd->containsFloor(limitedFloors) ) jdlist.append(jd);

    QList<MemberData *> mdlist;
    Q_FOREACH ( MemberData *md, this->memberListOfAll() )
        if ( md->containsFloors(limitedFloors) ) mdlist.append(md);

    outfile << jdlist.count();
    Q_FOREACH (JointData *jd, jdlist) jd->writeP3dData(outfile);

    myMemberData->writeLimitedData(outfile, mdlist);
}

void UnifiedData::writeDividedData(QDataStream &outfileA, QDataStream &outfileB,
                                   const QList<QUuid> &limitedFloors) const
{
    QList<JointData *> jdlistA, jdlistB;
    Q_FOREACH ( JointData *jd, this->jointListOfAll() ) {
        ( jd->containsFloor(limitedFloors) )  ? jdlistA.append(jd) : jdlistB.append(jd) ;
    }

    QList<MemberData *> mdlistA, mdlistB;
    Q_FOREACH ( MemberData *md, this->memberListOfAll() ) {
        QList<JointData *> c_jdlist;
        Q_FOREACH ( JointData *jd, md->getJointList() ) {
            if ( jd->containsFloor(limitedFloors) ) c_jdlist.append(jd);
        }
        if ( c_jdlist.count() == md->getJointList().count() ) {
            mdlistA.append(md);
        } else {
            mdlistB.append(md);
            Q_FOREACH ( JointData *jd, c_jdlist )
                if ( !jdlistB.contains(jd) ) jdlistB.append(jd);
        }
    }

    outfileA << jdlistA.count();
    Q_FOREACH (JointData *jd, jdlistA) jd->writeP3dData(outfileA);
    outfileB << jdlistB.count();
    Q_FOREACH (JointData *jd, jdlistB) jd->writeP3dData(outfileB);

    myMemberData->writeLimitedData(outfileA, mdlistA);
    myMemberData->writeLimitedData(outfileB, mdlistB);
}

/* 節点の追加、削除、変更 */

JointData *UnifiedData::createJoint(const GlobalPoint &gp, bool signalOn, const QUuid &id)
{
    JointData *jd = myJointData->createJoint(gp, id);
    if ( jd == nullptr ) return jd;

    if (signalOn) {
        // Log Message
        QString lmsg = "create Joint. \"";
        lmsg += QString("No.%1, (X,Y,Z)=(%2,%3,%4)").arg(indexOfJoint(jd) + 1)
                .arg(gp.x(), 0, 'f', 3).arg(gp.y(), 0, 'f', 3).arg(gp.z(), 0, 'f', 3);
        lmsg += "\"";
        emit signalSendLogMessage(lmsg);

        emit jointAppended( QList<JointData *>() << jd );
        emit elementCountChanged();
    }
    return jd;
}

JointData *UnifiedData::createAnalysisJoint(const GlobalPoint &gp, const int jointName, bool signalOn, const QUuid &id)
{
    JointData *jd = myJointData->createJoint(gp, jointName, id);
    if ( jd == nullptr ) return jd;

    if (signalOn) {
        // Log Message
        QString lmsg = "create Joint. \"";
        lmsg += QString("No.%1, (X,Y,Z)=(%2,%3,%4)").arg(indexOfJoint(jd) + 1)
                .arg(gp.x(), 0, 'f', 3).arg(gp.y(), 0, 'f', 3).arg(gp.z(), 0, 'f', 3);
        lmsg += "\"";
        emit signalSendLogMessage(lmsg);

        emit jointAppended( QList<JointData *>() << jd );
        emit elementCountChanged();
    }
    return jd;
}

QList<JointData *> UnifiedData::createJoints(const QList<GlobalPoint> &gplist, bool signalOn,
                                             const QList<QUuid> &idlist)
{
    QList<JointData *> jdlist;
    for ( int i = 0; i < gplist.count(); i++ ) {
        QUuid id = ( idlist.count() > i ) ? idlist.at(i) : QUuid() ;
        JointData *jd = myJointData->createJoint(gplist.at(i), id);
        if (jd != nullptr) jdlist.append(jd);
    }
    if ( jdlist.isEmpty() ) return jdlist;

    if ( signalOn ) {
        // Log Message
        QString lmsg = QString("create Joints. \"count=%1\"").arg(jdlist.count());
        emit signalSendLogMessage(lmsg);

        emit jointAppended(jdlist);
        emit elementCountChanged();
    }
    return jdlist;
}

JointData *UnifiedData::nearlyJoint(const GlobalPoint &gp, qreal eps) const
{
    return myJointData->nearlyJoint(gp, eps);
}
QList<JointData*> UnifiedData::getJointBetween(const GlobalPoint &p1,const GlobalPoint &p2, qreal eps)const
{
    QList<JointData*> joints;
    Q_FOREACH(JointData* jd,myJointData->jointListOfAll()){
        qreal x1=p1.x();
        qreal y1=p1.y();
        qreal z1=p1.z();
        qreal x2=p2.x();
        qreal y2=p2.y();
        qreal z2=p2.z();
        qreal x0=jd->xPos();
        qreal y0=jd->yPos();
        qreal z0=jd->zPos();

        if(abs((x2-x1)*(y0-y1)-(y2-y1)*(x0-x1))>eps)continue;
        if(abs((x2-x1)*(z0-z1)-(z2-z1)*(x0-x1))>eps)continue;
        if((qMin(x1,x2)-eps<=x0 && x0<=qMax(x1,x2)+eps)
                &&(qMin(y1,y2)-eps<=y0 && y0<=qMax(y1,y2)+eps)
                &&(qMin(z1,z2)-eps<=z0 && z0<=qMax(z1,z2)+eps))joints.append(jd);

    }
    return joints;
}
bool UnifiedData::isExistedJoint(JointData *jd) const
{
    return myJointData->isExistedJoint(jd);
}

int UnifiedData::indexOfJoint(JointData *jd) const
{
    return myJointData->indexOfJoint(jd);
}

JointData *UnifiedData::jointIndexToPointer(int idx) const
{
    return myJointData->jointIndexToPointer(idx);
}

bool UnifiedData::existOverlapMember(const QList<JointData *> &jdlist, DATATYPE dtype)
{
    bool isClosed = ( dtype == DATATYPE::TPSLAB || dtype == DATATYPE::TPWALL || dtype == DATATYPE::TPUWALL || dtype == DATATYPE::TPSLABLOAD );
    Q_FOREACH ( MemberData *r_md, jdlist.first()->getRelatedMembers() ) {
        if ( r_md->dataType() != dtype ) continue;
        if ( this->isOverlapJoints( jdlist, r_md->getJointList(), isClosed ) ) return true;
    }
    return false;
}

bool UnifiedData::isOverlapJoints(const QList<JointData *> &jlist1,
                                  const QList<JointData *> &jlist2, bool isClosed) const
{
    if ( jlist1.count() == 1 || jlist2.count() == 1 ) {
        if ( jlist1.count() != jlist2.count() ) return false;
        if ( jlist1.first() != jlist2.first() ) return false;
        return true;
    } else if ( isClosed ) {
        QList<JointData *> s_jlist1 = this->simplifiedMemberJoints(jlist1, true);
        QList<JointData *> s_jlist2 = this->simplifiedMemberJoints(jlist2, true);
        if ( s_jlist1.count() != s_jlist2.count() ) return false;
        bool isOverlap = true;
        Q_FOREACH ( JointData *j1, s_jlist1 ) {
            if ( !s_jlist2.contains(j1) ) {
                isOverlap = false;
                break;
            }
        }
        return isOverlap;
    } else {
        QList<JointData *> s_jlist1 = this->simplifiedMemberJoints(jlist1, false);
        QList<JointData *> s_jlist2 = this->simplifiedMemberJoints(jlist2, false);
        if ( s_jlist1.count() != s_jlist2.count() ) return false;
        if ( s_jlist1.first() != s_jlist2.first() && s_jlist1.first() != s_jlist2.last() ) return false;
        if ( s_jlist1.last() != s_jlist2.first() && s_jlist1.last() != s_jlist2.last() ) return false;
        bool isOverlap = true;
        Q_FOREACH ( JointData *j1, s_jlist1 ) {
            if ( !s_jlist2.contains(j1) ) {
                isOverlap = false;
                break;
            }
        }
        return isOverlap;
    }
}

QList<JointData *> UnifiedData::simplifiedMemberJoints( const QList<JointData *> &jdlist,
                                                        bool isClosed ) const
{
    return myJointData->simplifiedMemberJoints(jdlist, isClosed);
}

void UnifiedData::removeJoints(QList<JointData *> &jdlist, bool signalOn)
{
    if ( jdlist.isEmpty() ) return;

    QList<JointData *> removed_jdlist;
    QList<JointData *> changed_jdlist;

    QMultiHash<int, MemberData *> modified_mdtable;
    QMultiHash<int, MemberData *> removed_mdtable;

    Q_FOREACH ( JointData *jd, jdlist ) {

        Q_FOREACH ( MemberData *md, jd->getCriticalMember() ) {
            if ( !removed_mdtable.contains(static_cast<int>(md->dataType()), md) ) {
                removed_mdtable.insert(static_cast<int>(md->dataType()), md);
                if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                    Q_FOREACH ( MemberData *cmd, md->getRelatedMembers() ) {
                        if ( !cmd->isGirderType() ) continue; // フェイス再計算の必要ない部材
                        if ( !modified_mdtable.contains(static_cast<int>(cmd->dataType()), cmd) ) {
                            modified_mdtable.insert(static_cast<int>(cmd->dataType()), cmd);
                        }
                    }
                } else if ( md->isGirderType() ) {
                    Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                        Q_FOREACH ( MemberData *cmd, jd->getComposeMembers() ) {
                            if ( !cmd->isGirderType() ) continue;
                            if ( !modified_mdtable.contains(static_cast<int>(cmd->dataType()), cmd) ) {
                                modified_mdtable.insert(static_cast<int>(cmd->dataType()), cmd);
                            }
                        }
                    }
                }
            }
        }
        Q_FOREACH ( MemberData *md, jd->getRelatedMembers() ) {
            if ( !modified_mdtable.contains(static_cast<int>(md->dataType()), md) ) {
                modified_mdtable.insert(static_cast<int>(md->dataType()), md);
                if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                    Q_FOREACH ( MemberData *cmd, md->getRelatedMembers() ) {
                        if ( !cmd->isGirderType() ) continue; // フェイス再計算の必要ない部材
                        if ( !modified_mdtable.contains(static_cast<int>(cmd->dataType()), cmd) ) {
                            modified_mdtable.insert(static_cast<int>(cmd->dataType()), cmd);
                        }
                    }
                }
            }
        }
        if ( jd->isComposeJoint() ) {
            Q_FOREACH ( JointData *l_jd, jd->linkedJoints() ) {
                if ( l_jd->isInteriorJoint() && !jdlist.contains(l_jd)
                        && !changed_jdlist.contains(l_jd) )
                    changed_jdlist.append(l_jd);
            }
        }
        if ( myJointData->removeJoint(jd) ) removed_jdlist.append(jd);
    }

    QList<MemberData *> changed_mdlist;
    Q_FOREACH ( int type, modified_mdtable.uniqueKeys() ) {
        Q_FOREACH ( MemberData *md, modified_mdtable.values(type) ) {
            if ( !removed_mdtable.contains(type, md) ) {
                changed_mdlist.append(md);
                md->recalcLineLength();
            }
        }
    }

    emit existCopyBuffer(this);

    if ( !signalOn ) return;

    // Log Message
    QString lmsg = QString("remove Joints. \"count=%1\"").arg(removed_jdlist.count());
    emit signalSendLogMessage(lmsg);
    /*
        Q_FOREACH( DATATYPE type, removed_mdtable.uniqueKeys() ){
            emit memberRemoved( removed_mdtable.values(type), type );
            // Log Message
            QString lmsg=QString("remove depended Members. \"type=")
                    +dataTypeToEnglish(type)
                    +QString(", count=%1\"").arg(removed_mdtable.values(type).count());
            sendLogMessage(lmsg);
        }
    */
    // Log Message

    if ( !removed_mdtable.isEmpty() ) {
        lmsg = QString("remove depended Members. \"");
        Q_FOREACH ( int type, removed_mdtable.uniqueKeys() ) {
            emit memberRemoved( removed_mdtable.values(type), static_cast<DATATYPE>(type) );
            if ( lmsg.mid(lmsg.count() - 1) != "\"" ) lmsg += QString(", ");
            lmsg += dataTypeToEnglish(static_cast<DATATYPE>(type)) + QString(":%1").arg(removed_mdtable.values(type).count());
        }
        lmsg += "\"";
        emit signalSendLogMessage(lmsg);
    }

    emit jointRemoved(removed_jdlist);
    emit jointChanged(changed_jdlist);
    emit memberChanged(changed_mdlist);

    emit elementCountChanged();
}

bool UnifiedData::changeJoints(QList<JointData *> &jdlist, const QList<GlobalPoint> &gplist,
                               bool signalOn)
{
    if ( !myJointData->checkMovedPosition(jdlist, gplist) ) return false;

    QList<JointData *> changed_jdlist;
    for ( int i = 0; i < jdlist.count(); i++ ) {
        if ( myJointData->changeJoint(jdlist.at(i), gplist.at(i)) ) changed_jdlist.append(jdlist.at(i));
    }

    QList<MemberData *> changed_mdlist;
    Q_FOREACH ( JointData *jd, changed_jdlist ) {
        Q_FOREACH ( MemberData *md, jd->getRelatedMembers() ) {
            if ( !changed_mdlist.contains(md) ) {
                // 所属階によって断面が変わる部材タイプ：柱・大梁
                if ( md->dataType() == DATATYPE::TPCOLUMN || md->dataType() == DATATYPE::TPGIRDER ) md->resetLineShape(*this);
                md->recalcLineLength();
                changed_mdlist.append(md);
                if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                    Q_FOREACH ( MemberData *cmd, md->getRelatedMembers() ) {
                        if ( !cmd->isGirderType() ) continue; // フェイス再計算の必要ない部材
                        cmd->recalcLineLength();
                        if ( !changed_mdlist.contains(cmd) ) changed_mdlist.append(cmd);
                    }
                }
            }
        }
    }

    if ( !signalOn ) return true;

    // Log Message
    QString lmsg = QString("move Joints. \"count=%1\"").arg(changed_jdlist.count());
    emit signalSendLogMessage(lmsg);

    emit jointChanged(changed_jdlist);
    emit memberChanged(changed_mdlist);
    return true;
}


void UnifiedData::changeJointsAttachment( QList<JointData *> &changed_jdlist,
                                          const QList<QUuid> &idlist,
                                          DATATYPE type, bool signalOn )
{
    if ( type != DATATYPE::TPFLOOR && type != DATATYPE::TPFRAMEP ) return;
    QList<QUuid> changed_idlist;
    Q_FOREACH ( QUuid id, idlist ) {
        if ( isExistedID(id, type) ) changed_idlist.append(id);
    }

    if ( type == DATATYPE::TPFLOOR ) {
        Q_FOREACH ( JointData *jd, changed_jdlist ) jd->replaceAttachedFloor(changed_idlist);
    }

    if ( type == DATATYPE::TPFRAMEP ) {
        Q_FOREACH ( JointData *jd, changed_jdlist ) jd->replaceAttachedFrame(changed_idlist);
    }

    QList<MemberData *> changed_mdlist, r_changed_mdlist;
    Q_FOREACH (JointData *jd, changed_jdlist) {
        Q_FOREACH ( MemberData *md, jd->getRelatedMembers() ) {
            if ( changed_mdlist.contains(md) ) continue;
            changed_mdlist.append(md);

            // 所属階によって断面が変わる部材タイプ：柱/大梁
            if ( md->dataType() != DATATYPE::TPCOLUMN && md->dataType() != DATATYPE::TPGIRDER ) continue;
            if ( md->iJoint() != jd ) continue;
            md->resetLineShape(*this);
            md->recalcLineLength();
            if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                Q_FOREACH ( MemberData *cmd, md->getRelatedMembers() ) {
                    if ( !cmd->isGirderType() ) continue; // フェイス再計算の必要ない部材
                    if ( !r_changed_mdlist.contains(cmd) ) r_changed_mdlist.append(cmd);
                }
            }
        }
    }

    Q_FOREACH ( MemberData *md, r_changed_mdlist ) {
        md->recalcLineLength();
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }


    if ( !signalOn ) return;

    // Log Message
    QString lmsg = QString("change Joints attachment. \"count=%1, ").arg(changed_jdlist.count());
    lmsg += "change " + dataTypeToEnglish(type) + "\"";
    emit signalSendLogMessage(lmsg);

    emit jointChanged(changed_jdlist);
    emit memberChanged(changed_mdlist);
}

void UnifiedData::changeJointsAttachment( QList<JointData *> &changed_jdlist,
                                          const QList<QList<QUuid> > &idlist,
                                          DATATYPE type, bool signalOn )
{
    if ( type != DATATYPE::TPFLOOR && type != DATATYPE::TPFRAMEP ) return;
    if ( changed_jdlist.count() != idlist.count() ) return;

    for ( int i = 0; i < changed_jdlist.count(); i++ ) {
        JointData *jd = changed_jdlist.at(i);
        if ( type == DATATYPE::TPFLOOR ) {
            jd->replaceAttachedFloor(idlist.at(i));
        } else if ( type == DATATYPE::TPFRAMEP ) {
            jd->replaceAttachedFrame(idlist.at(i));
        }
    }

    QList<MemberData *> changed_mdlist, r_changed_mdlist;
    Q_FOREACH (JointData *jd, changed_jdlist) {
        Q_FOREACH ( MemberData *md, jd->getRelatedMembers() ) {
            if ( changed_mdlist.contains(md) ) continue;
            changed_mdlist.append(md);

            // 所属階によって断面が変わる部材タイプ：柱/大梁
            if ( md->dataType() != DATATYPE::TPCOLUMN && md->dataType() != DATATYPE::TPGIRDER ) continue;
            if ( md->iJoint() != jd ) continue;
            md->resetLineShape(*this);
            md->recalcLineLength();
            if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                Q_FOREACH ( MemberData *cmd, md->getRelatedMembers() ) {
                    if ( !cmd->isGirderType() ) continue; // フェイス再計算の必要ない部材
                    if ( !r_changed_mdlist.contains(cmd) ) r_changed_mdlist.append(cmd);
                }
            }
        }
    }

    Q_FOREACH ( MemberData *md, r_changed_mdlist ) {
        md->recalcLineLength();
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }


    if ( !signalOn ) return;

    // Log Message
    QString lmsg = QString("change Joints attachment. \"count=%1, ").arg(changed_jdlist.count());
    lmsg += "change " + dataTypeToEnglish(type) + "\"";
    emit signalSendLogMessage(lmsg);

    emit jointChanged(changed_jdlist);
    emit memberChanged(changed_mdlist);
}

void UnifiedData::appendJointsAttachment(QList<JointData *> &changed_jdlist,
                                         const QList<QUuid> &idlist,
                                         DATATYPE type, bool signalOn)
{
    if ( type != DATATYPE::TPFLOOR && type != DATATYPE::TPFRAMEP ) return;
    QList<QUuid> changed_idlist;
    Q_FOREACH (QUuid id, idlist) {
        if ( isExistedID(id, type) ) changed_idlist.append(id);
    }

    if ( type == DATATYPE::TPFLOOR ) {
        Q_FOREACH ( JointData *jd, changed_jdlist ) jd->appendAttachedFloor(changed_idlist);
    }

    if ( type == DATATYPE::TPFRAMEP ) {
        Q_FOREACH ( JointData *jd, changed_jdlist ) jd->appendAttachedFrame(changed_idlist);
    }

    QList<MemberData *> changed_mdlist, r_changed_mdlist;
    Q_FOREACH ( JointData *jd, changed_jdlist ) {
        Q_FOREACH ( MemberData *md, jd->getRelatedMembers() ) {
            if ( changed_mdlist.contains(md) ) continue;
            changed_mdlist.append(md);

            // 所属階によって断面が変わる部材タイプ：柱/大梁
            if ( md->dataType() != DATATYPE::TPCOLUMN && md->dataType() != DATATYPE::TPGIRDER ) continue;
            if ( md->iJoint() != jd ) continue;
            md->resetLineShape(*this);
            md->recalcLineLength();
            if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                Q_FOREACH ( MemberData *cmd, md->getRelatedMembers() ) {
                    if ( !cmd->isGirderType() ) continue; // フェイス再計算の必要ない部材
                    if ( !r_changed_mdlist.contains(cmd) ) r_changed_mdlist.append(cmd);
                }
            }
        }
    }

    Q_FOREACH ( MemberData *md, r_changed_mdlist ) {
        md->recalcLineLength();
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }

    if ( !signalOn ) return;

    // Log Message
    QString lmsg = QString("append Joints attachment. \"count=%1, ").arg(changed_jdlist.count());
    lmsg += "change " + dataTypeToEnglish(type) + "\"";
    emit signalSendLogMessage(lmsg);

    emit jointChanged(changed_jdlist);
    emit memberChanged(changed_mdlist);
}

void UnifiedData::completeJointsAttachment(const QList<JointData *> &jdlist, bool overwrite)
{
    // Z座標(1mm誤差)でまとめる
    QMultiHash<int, JointData *> zToJoints;
    Q_FOREACH ( JointData *jd, jdlist ) {
        int iz = qRound(jd->zPos() * 1000.0);
        zToJoints.insert(iz, jd);
    }

    QList<MemberData *> changed_mdlist, r_changed_mdlist;

    Q_FOREACH ( int iz, zToJoints.uniqueKeys() ) {
        QList<JointData *>  z_jdlist = zToJoints.values(iz);
        QUuid on_fl = getFloorOfHeight(z_jdlist.first()->zPos());
        if ( !on_fl.isNull() ) {
            QList<FRAMEPOINTSVALUE> fplist = getFrameValuesOfFloor(on_fl);
            Q_FOREACH ( JointData *z_jd, z_jdlist ) {
                if ( overwrite ) {
                    z_jd->clearAttachedFloor();
                    z_jd->clearAttachedFrame();
                }
                z_jd->appendAttachedFloor(on_fl);
                Q_FOREACH ( FRAMEPOINTSVALUE fp, fplist ) {
                    if ( fp.pointList.isEmpty() ) continue;
                    bool online = false;
                    for (int i = 0; i < fp.pointList.count() - 1; i++) {
                        if ( CalcVector2D::between(fp.pointList.at(i), fp.pointList.at(i + 1), QPointF(z_jd->xPos(),
                                                                                                       z_jd->yPos())) ) {
                            online = true;
                            break;
                        }
                    }
                    if (online) z_jd->appendAttachedFrame(fp.UUID);
                }
            }
        } else {
            QList<QUuid> ud_fl = getNearlyFloorOfHeight(z_jdlist.first()->zPos());

            if ( ud_fl.count() == 1 ) {
                QList<FRAMEPOINTSVALUE> fplist = getFrameValuesOfFloor(ud_fl.first());
                Q_FOREACH ( JointData *z_jd, z_jdlist ) {
                    if ( overwrite ) {
                        z_jd->clearAttachedFloor();
                        z_jd->clearAttachedFrame();
                    }
                    z_jd->appendAttachedFloor(ud_fl.first());
                    Q_FOREACH ( FRAMEPOINTSVALUE fp, fplist ) {
                        if ( fp.pointList.isEmpty() ) continue;
                        bool online = false;
                        for (int i = 0; i < fp.pointList.count() - 1; i++) {
                            if ( CalcVector2D::between(fp.pointList.at(i), fp.pointList.at(i + 1), QPointF(z_jd->xPos(),
                                                                                                           z_jd->yPos())) ) {
                                online = true;
                                break;
                            }
                        }
                        if (online) z_jd->appendAttachedFrame(fp.UUID);
                    }
                }
            } else if ( ud_fl.count() > 1 ) {
                QHash<QUuid, QList<QPointF> > d_plist; // 下階のFramePoint
                QHash<QUuid, QList<QPointF> > u_plist; // 上階のFramePoint
                Q_FOREACH ( FRAMEPOINTSVALUE fp, getFrameValuesOfFloor(ud_fl.first()) ) {
                    if ( !d_plist.contains(fp.UUID) ) d_plist.insert(fp.UUID, fp.pointList);
                }
                Q_FOREACH ( FRAMEPOINTSVALUE fp, getFrameValuesOfFloor(ud_fl.last()) ) {
                    if ( !u_plist.contains(fp.UUID) ) u_plist.insert(fp.UUID, fp.pointList);
                }
                qreal hh = z_jdlist.first()->zPos();
                QHash<QUuid, QList<QPointF> > t_plist; // 当該高さのFramePoint
                Q_FOREACH ( QUuid fr_id, d_plist.keys() ) {
                    if ( u_plist.contains(fr_id) ) {
                        QList<QPointF> plist1 = d_plist.value(fr_id);
                        qreal h1 = getFloorHeight(ud_fl.first());
                        QList<QPointF> plist2 = d_plist.value(fr_id);
                        qreal h2 = getFloorHeight(ud_fl.last());
                        qreal rr = (hh - h1) / (h2 - h1);
                        QList<QPointF> c_plist = CalcVector2D::combinePointList(plist1, plist2, rr);
                        t_plist.insert(fr_id, c_plist);
                    } else {
                        t_plist.insert(fr_id, d_plist.value(fr_id));
                    }
                }
                Q_FOREACH ( QUuid fr_id, u_plist.keys() ) {
                    if ( u_plist.contains(fr_id) )continue;
                    t_plist.insert(fr_id, u_plist.value(fr_id));
                }

                Q_FOREACH ( JointData *z_jd, z_jdlist ) {
                    if ( overwrite ) {
                        z_jd->clearAttachedFloor();
                        z_jd->clearAttachedFrame();
                    }
                    Q_FOREACH ( QUuid fr_id, t_plist.keys() ) {
                        QList<QPointF> fpoints = t_plist.value(fr_id);
                        bool online = false;
                        for (int i = 0; i < fpoints.count() - 1; i++) {
                            if ( CalcVector2D::between(fpoints.at(i), fpoints.at(i + 1), QPointF(z_jd->xPos(),
                                                                                                 z_jd->yPos())) ) {
                                online = true;
                                break;
                            }
                        }
                        if (online) z_jd->appendAttachedFrame(fr_id);
                    }
                }
            }
        }

        Q_FOREACH ( JointData *z_jd, z_jdlist ) {
            Q_FOREACH ( MemberData *md, z_jd->getRelatedMembers() ) {
                if ( changed_mdlist.contains(md) ) continue;
                changed_mdlist.append(md);

                // 所属階によって断面が変わる部材タイプ：柱/大梁
                if ( md->dataType() != DATATYPE::TPCOLUMN && md->dataType() != DATATYPE::TPGIRDER ) continue;
                if ( md->iJoint() != z_jd ) continue;
                md->resetLineShape(*this);
                md->recalcLineLength();
                if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                    Q_FOREACH ( MemberData *cmd, md->getRelatedMembers() ) {
                        if ( !cmd->isGirderType() ) continue; // フェイス再計算の必要ない部材
                        if ( !r_changed_mdlist.contains(cmd) ) r_changed_mdlist.append(cmd);
                    }
                } else if ( md->isGirderType() ) {
                    Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                        Q_FOREACH ( MemberData *cmd, jd->getComposeMembers() ) {
                            if ( !cmd->isGirderType() ) continue;
                            if ( !r_changed_mdlist.contains(cmd) ) r_changed_mdlist.append(cmd);
                        }
                    }
                }
            }
        }
    }

    Q_FOREACH ( MemberData *md, r_changed_mdlist ) {
        md->recalcLineLength();
        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }

    // Log Message
    QString lmsg = QString("auto change Joints attachment. \"count=%1\"").arg(jdlist.count());
    emit signalSendLogMessage(lmsg);

    emit jointChanged(jdlist);
    emit memberChanged(changed_mdlist);
}

QList<JointData *> UnifiedData::jointListOfAll() const
{
    return myJointData->jointListOfAll();
}

QList<JointData *> UnifiedData::jointListOfFloor(const QUuid &id) const
{
    return myJointData->jointListOfFloor(id);
}

QList<JointData *> UnifiedData::jointListOfFloor(const QList<QUuid> &idlist) const
{
    return myJointData->jointListOfFloor(idlist);
}

QList<JointData *> UnifiedData::jointListOfFrame(const QUuid &id) const
{
    return myJointData->jointListOfFrame(id);
}

QList<JointData *> UnifiedData::jointListOfFrame(const QList<QUuid> &idlist) const
{
    return myJointData->jointListOfFrame(idlist);
}

bool UnifiedData::replaceJointIndexes( const QList<JointData *> &jdlist )
{
    return myJointData->replaceJointIndexes(jdlist);
}

JointData *UnifiedData::jointIDToPointer(const QUuid &id) const
{
    return myJointData->jointIDToPointer(id);
}

QList<JointData *> UnifiedData::jointIDsToPointers(const QList<QUuid> &idlist) const
{
    return myJointData->jointIDsToPointers(idlist);
}

void UnifiedData::calcCrossPoints(const QUuid &id, const QList<FRAMEPOINTSVALUE> &frvalues,
                                  QList<GlobalPoint> &gplist, QList<GlobalPoint> &cplist )
{
    myJointData->calcCrossPoints(id, frvalues, gplist, cplist);
}



/* 部材の追加、削除、変更 */

MemberData *UnifiedData::createMember(const QList<JointData *> &jlist, DATATYPE type,
                                      const QString &str, const QUuid &id)
{
    return myMemberData->createMember(jlist, type, str, id);
}

QList<MemberData *>  UnifiedData::createMembers(const QList< QList<JointData *> > &jd_alllist,
                                                DATATYPE type,
                                                const QString &str, const QList<QUuid> &idlist )
{
    QList<MemberData *> created_mdlist;
//    QList<MemberData*> changed_mdlist;

    for ( int i = 0; i < jd_alllist.count(); i++ ) {

        QList<JointData *> jdlist = jd_alllist.at(i);
        QUuid id = ( idlist.count() > i ) ? idlist.at(i) : QUuid() ;

        MemberData *md = myMemberData->createMember(jdlist, type, str, id);
        created_mdlist.append(md);
        /*
                if( md==nullptr ) continue;
                if( type!=TPCOLUMN ) continue;

                Q_FOREACH(JointData* jd, jdlist){
                    QList<MemberData*> mdlist=jd->getComposeMembers();
                    Q_FOREACH(MemberData* mm, mdlist){
                        if( !mm->isGirderType() ) continue;
                        if( !changed_mdlist.contains(mm) && !created_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
        */
    }
    /*
        if( !changed_mdlist.isEmpty() ){
            Q_FOREACH(MemberData* md, changed_mdlist) md->recalcLineLength();
        }
    */
    /*
        if( !signalOn ) return created_mdlist;

        // Log Message
        QString lmsg="create Members. \"type="+dataTypeToEnglish(type);
        lmsg+=QString(", count=%1, name=").arg(jd_alllist.count());
        lmsg+=( str.isEmpty() ) ? "(dummy)" : str;
        lmsg+="\"";
        sendLogMessage(lmsg);

    //    if( !changed_mdlist.isEmpty() ) emit memberChanged(changed_mdlist);
        QList<MemberData*> e_created_mdlist;
        Q_FOREACH( MemberData* md, created_mdlist ){
            if( md!=nullptr ) e_created_mdlist.append(md);
        }
        emit memberAppended( e_created_mdlist );
        emit elementCountChanged();
    */
    return created_mdlist;
}

void UnifiedData::removeMembers(const QList<MemberData *> &remove_mdlist, bool signalOn)
{
    QList<JointData *> changed_jdlist;
    QList<MemberData *> changed_mdlist;

    QMultiHash<int, MemberData *> removed_mdtable;

    Q_FOREACH ( MemberData *md, remove_mdlist ) {
        Q_FOREACH ( JointData *jd, md->getJointList() )
            if ( !changed_jdlist.contains(jd) ) changed_jdlist.append(jd);

        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( MemberData *r_md, md->getRelatedMembers() ) {
                if ( !r_md->isGirderType() ) continue;
                if ( !changed_mdlist.contains(r_md) && !remove_mdlist.contains(r_md) ) changed_mdlist.append(r_md);
            }
        } else if ( md->isGirderType() ) {
            Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( !mm->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(mm) && !remove_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
        removed_mdtable.insert(static_cast<int>(md->dataType()), md);
        myMemberData->removeMember(md);
    }

    emit existCopyBuffer(this);

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH (MemberData *md, changed_mdlist) md->recalcLineLength();
    }

    if ( !signalOn ) return;

    // Log Message
    QString lmsg = QString("remove Members. \"");
    Q_FOREACH ( int type, removed_mdtable.uniqueKeys() ) {
        emit memberRemoved( removed_mdtable.values(type), static_cast<DATATYPE>(type) );
        if ( lmsg.mid(lmsg.count() - 1) != "\"" ) lmsg += QString(", ");
        lmsg += dataTypeToEnglish(static_cast<DATATYPE>(type)) + QString(":%1").arg(removed_mdtable.values(type).count());
    }
    lmsg += "\"";
    emit signalSendLogMessage(lmsg);

    if ( !changed_mdlist.isEmpty() ) emit memberChanged(changed_mdlist);
    emit jointChanged(changed_jdlist);
    emit elementCountChanged();
}

void UnifiedData::removeJointOfMember(JointData *jd, MemberData *md)
{
    myMemberData->removeJointOfMember(jd, md);
    // recalc や　emit　はここでは行わない
}

void UnifiedData::removeJointOfMember(JointData *jd, const QList<MemberData *> &mdlist)
{
    myMemberData->removeJointOfMember(jd, mdlist);
    // recalc や　emit　はここでは行わない
}

void UnifiedData::removeJointsOfMember(const QList<JointData *> &jdlist, MemberData *md)
{
    myMemberData->removeJointsOfMember(jdlist, md);
    // recalc や　emit　はここでは行わない
}

bool UnifiedData::isExistedMember(MemberData *md) const
{
    return myMemberData->isExistedMember(md);
}

QList<MemberData *> UnifiedData::memberListOfAll() const
{
    return myMemberData->memberListOfAll();
}

QList<MemberData *> UnifiedData::memberListExceptLoad() const
{
    return myMemberData->memberListExceptLoad();
}

QList<MemberData *> UnifiedData::memberListOfFloor(const QUuid &id) const
{
    return myMemberData->memberListOfFloor(id);
}

QList<MemberData *> UnifiedData::memberListOfFrame(const QUuid &id) const
{
    return myMemberData->memberListOfFrame(id);
}

QList<MemberData *> UnifiedData::memberListOfType(DATATYPE dtype) const
{
    return myMemberData->memberListOfType(dtype);
}

QList<MemberData *> UnifiedData::memberListOfType(const QList<DATATYPE> &types) const
{
    return myMemberData->memberListOfType(types);
}

MemberData *UnifiedData::memberIDToPointer(const QUuid &id) const
{
    return myMemberData->memberIDToPointer(id);
}


QList<MemberData *> UnifiedData::memberIDsToPointers(const QList<QUuid> &idlist) const
{
    return myMemberData->memberIDsToPointers(idlist);
}

bool UnifiedData::interiorMember( MemberData *md, const QList<JointData *> &jdlist )
{
    return myMemberData->interiorMember(md, jdlist);
}
/*
void UnifiedData::interiorMembers(const QList<MemberData*>& in_mdlist,
                                  const QList< QList<JointData*> >& in_jdlist, bool signalOn)
{
    QList<MemberData*> changed_mdlist;
    QList<JointData*> changed_jdlist;

    for( int i=0 ; i<in_mdlist.count() ; i++ ){

        MemberData* md=in_mdlist.at(i);
        QList<JointData*> jdlist=in_jdlist.at(i);

        if( myMemberData->interiorMember(md,jdlist) ){

            if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);

            if( md->dataType()==TPCOLUMN || md->isGirderType() ){
                Q_FOREACH( JointData* jd, jdlist ){
                    Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                        if( !mm->isGirderType() ) continue;
                        if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            }

            Q_FOREACH(JointData* jd, jdlist)
                if( !changed_jdlist.contains(jd) ) changed_jdlist.append(jd);
        }
    }

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH(MemberData* md, changed_mdlist) md->recalcLineLength();
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("interior Members. \"count=%1\"").arg(in_mdlist.count());
    sendLogMessage(lmsg);

    if( !changed_mdlist.isEmpty() ) emit memberChanged(changed_mdlist);
    emit jointChanged( changed_jdlist );
    emit elementCountChanged();
}
*/
// 元mdを残したまま、mdを分割したmdlistを返す
QList<MemberData *> UnifiedData::divideMember(MemberData *md, const QList<JointData *> &jdlist,
                                              const QList<QUuid> &idlist)
{
    return myMemberData->divideMember(*this, md, jdlist, idlist);
}

/*
QList<MemberData*> UnifiedData::divideMember(MemberData* md, const QList<JointData*>& jdlist,
                                             bool signalOn, const QList<QUuid> & idlist )
{
    QList<MemberData*> created_mdlist;
    QList<MemberData*> removed_mdlist;
    QList<MemberData*> changed_mdlist;

    if( md->isClosedMember() ){ // 面材を構成点分割する場合は内分点分割と同義とする
        if( myMemberData->interiorMember(md,jdlist) ){
            if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
        }
    }else{
        // myMemberData->divideMemberではmdは削除されない
        QList<MemberData*> mdlist=myMemberData->divideMember(md,jdlist,idlist);
        if( !mdlist.isEmpty() ){
            created_mdlist.append(mdlist);
            removed_mdlist.append(md);
            if( md->dataType()==TPCOLUMN || md->isGirderType() ){
                Q_FOREACH( JointData* jd, md->getJointList() ){
                    if( jdlist.contains(jd) ){ // 分割点に取り付く梁材のフェイス変更
                        Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                            if( mm==md ) continue;
                            if( !mm->isGirderType() ) continue;
                            if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                        }
                    }else if( md->dataType()==TPCOLUMN ){ // 柱内分点に取り付く梁材のフェイス変更
                        Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                            if( mm==md ) continue;
                            if( !mm->isGirderType() ) continue;
                            if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                        }
                    }
                }
            }
        }
    }

    //  removeMembers()により、元部材のremovedシグナル、節点のchangedシグナルが発生
    removeMembers(removed_mdlist,signalOn);

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH(MemberData* mm, changed_mdlist) mm->recalcLineLength();
        if( signalOn ) emit memberChanged(changed_mdlist);
    }

    if( !signalOn ) return created_mdlist;

    emit memberAppended(created_mdlist);
    emit elementCountChanged();
    return created_mdlist;
}
*/

// 元mdを残したまま、mdを分割したmdlistを返す
QList<MemberData *> UnifiedData::divideMemberByInteriorJoints(MemberData *md,
                                                              const QList<JointData *> &jdlist,
                                                              const QList<QUuid> &idlist)
{
    return myMemberData->divideMemberByInteriorJoints(*this, md, jdlist, idlist);
}
/*
QList<MemberData*> UnifiedData::divideMemberByInteriorJoints(MemberData* md, const QList<JointData*>& jdlist,
                                                             bool signalOn, const QList<QUuid> & idlist)
{
    QList<MemberData*> created_mdlist;
    QList<MemberData*> changed_mdlist;

    QList<MemberData*> mdlist=myMemberData->divideMemberByInteriorJoints(md,jdlist,idlist);
    if( mdlist.isEmpty() ) return QList<MemberData*>();

    created_mdlist.append(mdlist);

    if( md->dataType()==TPCOLUMN || md->isGirderType() ){
        Q_FOREACH( JointData* jd, md->getJointList() ){
            if( jdlist.contains(jd) ){ // 分割点に取り付く梁材のフェイス変更
                Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                    if( mm==md ) continue;
                    if( !mm->isGirderType() ) continue;
                    if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }else if( md->dataType()==TPCOLUMN ){ // 柱内分点に取り付く梁材のフェイス変更
                Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                    if( mm==md ) continue;
                    if( !mm->isGirderType() ) continue;
                    if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    //  removeMembers()により、元部材のremovedシグナル、節点のchangedシグナルが発生
    removeMembers( QList<MemberData*>() << md, signalOn );

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH(MemberData* mm, changed_mdlist) mm->recalcLineLength();
    }

    if( !signalOn ) return created_mdlist;

    emit memberChanged(changed_mdlist);
    emit memberAppended(created_mdlist);
    emit elementCountChanged();
    return created_mdlist;
}
*/

/*
void UnifiedData::divideMembers(const QList<MemberData*>& in_mdlist,
                                const QList< QList<JointData*> >& in_jdlist, bool signalOn)
{
    QList<MemberData*> created_mdlist;
    QList<MemberData*> removed_mdlist;
    QList<MemberData*> changed_mdlist;

    for( int i=0 ; i<in_mdlist.count() ; i++ ){
        MemberData* md=in_mdlist.at(i);
        QList<JointData*> jdlist=in_jdlist.at(i);

        if( md->isClosedMember() ){ // 面材を構成点分割する場合は内分点分割と同義とする
            if( myMemberData->interiorMember(md,jdlist) ){
                if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
            }
            continue;
        }

        QList<MemberData*> mdlist=myMemberData->divideMember(md,jdlist);
        if( mdlist.count()>0 ){
            created_mdlist.append(mdlist);
            removed_mdlist.append(md);
            if( md->dataType()==TPCOLUMN ){
                Q_FOREACH(JointData* jd, jdlist){
                    QList<MemberData*> mdlist=jd->getComposeMember();
                    Q_FOREACH(MemberData* mm, mdlist){
                        if( !mm->isGirderType() ) continue;
                        if( !in_mdlist.contains(mm) && !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            }
        }
    }

    //  removeMembers()により、元部材のremovedシグナル、節点のchangedシグナルが発生
    removeMembers(removed_mdlist,signalOn);

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH(MemberData* md, changed_mdlist) md->recalcLineLength();
        if( signalOn ) emit memberChanged(changed_mdlist);
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("divide Members. \"count=%1\"").arg(in_mdlist.count());
    sendLogMessage(lmsg);

    emit memberAppended(created_mdlist);
    emit elementCountChanged();
}
*/
/*
void UnifiedData::divideMembersByInteriorJoints(const QList<MemberData*>& in_mdlist, bool signalOn)
{
    QList<MemberData*> created_mdlist;
    QList<MemberData*> removed_mdlist;
    QList<MemberData*> changed_mdlist;

    for( int i=0 ; i<in_mdlist.count() ; i++ ){
        MemberData* md=in_mdlist.at(i);
        QList<JointData*> jdlist=md->getInteriorJointList();

        QList<MemberData*> mdlist=myMemberData->divideMemberEach(md);
        if( mdlist.count()>0 ){
            created_mdlist.append(mdlist);
            removed_mdlist.append(md);

            if(md->dataType()==TPCOLUMN){
                Q_FOREACH(JointData* jd, jdlist){
                    QList<MemberData*> mdlist=jd->getComposeMember();
                    Q_FOREACH(MemberData* mm, mdlist){
                        if( !mm->isGirderType() ) continue;
                        if( !in_mdlist.contains(mm) && !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                    }
                }
            }

        }
    }

    //  removeMembers()により、元部材のremovedシグナル、節点のchangedシグナルが発生
    removeMembers(removed_mdlist);

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH(MemberData* md, changed_mdlist) md->recalcLineLength();
        if( signalOn ) emit memberChanged(changed_mdlist);
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("divide Members by interior Joints. \"count=%1\"").arg(in_mdlist.count());
    sendLogMessage(lmsg);

    emit memberAppended(created_mdlist);
    emit elementCountChanged();
}
*/

void UnifiedData::uniteMembers(const QList<MemberData *> &mdlist)
{
    /*
        MemberData* remain_md=mdlist.first();
        QList<JointData*> changed_jdlist;
        QList<MemberData*> changed_mdlist;
        QList<MemberData*> removed_mdlist=mdlist;

        DATATYPE dtype=remain_md->dataType();
        removed_mdlist.removeFirst();
        Q_FOREACH( MemberData* md, removed_mdlist ){
            changed_jdlist.append( md->iJoint() );
        }
    */
    myMemberData->uniteMembers(mdlist);
    /*
        if( dtype==TPCOLUMN || remain_md->isGirderType() ){

            Q_FOREACH( JointData* jd, remain_md->getInteriorJointList() ){
                Q_FOREACH( MemberData* md, jd->getComposeMembers() ){
                    if( !md->isGirderType() ) continue;
                    if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
                }
            }

            if( dtype==TPCOLUMN ){
                Q_FOREACH( MemberData* md, remain_md->getComposeMembers() ){
                    if( remain_md==md ) continue;
                    if( !md->isGirderType() ) continue;
                    if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
                }
            }
       }

        if( !changed_mdlist.isEmpty() ){
            Q_FOREACH( MemberData* md, changed_mdlist ) md->recalcLineLength();
        }

        if( !signalOn ) return;

        // Log Message
        QString lmsg=QString("unite Members. \"name=");
        lmsg+=( remain_md->sectionName().isEmpty() ) ? "(dummy)" : remain_md->sectionName();
        lmsg+=QString(", count=%1 to One\"").arg(mdlist.count());
        sendLogMessage(lmsg);

        emit memberRemoved(removed_mdlist,dtype);
        emit jointChanged(changed_jdlist);
        if( !changed_mdlist.contains(remain_md) ) changed_mdlist.append(remain_md);
        emit memberChanged(changed_mdlist);

        emit elementCountChanged();
    */
}
/*
void UnifiedData::replaceMemberJoint( MemberData* md, JointData* j1, JointData* j2 , bool signalOn)
{
    md->replaceJoint(j1,j2);
    if(signalOn) emit jointChanged( QList<JointData*>() << j1 );

    QList<MemberData*> changed_mdlist;

    if( md->dataType()==TPCOLUMN || md->isGirderType() ){

        Q_FOREACH( MemberData* mm, j1->getComposeMembers() ){
            if( !mm->isGirderType() ) continue;
            if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
        }

        Q_FOREACH( JointData* jd, md->getInteriorJointList() ){
            Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                if( !mm->isGirderType() ) continue;
                if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
            }
        }

        if( md->dataType()==TPCOLUMN ){
            Q_FOREACH( MemberData* mm, md->getComposeMembers() ){
                if( md==mm ) continue;
                if( !mm->isGirderType() ) continue;
                if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
            }
        }
    }

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH(MemberData* mm, changed_mdlist) mm->recalcLineLength();
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("replace Member Joint. \"name=");
    lmsg+=( md->sectionName().isEmpty() ) ? "(dummy)" : md->sectionName();
    lmsg+=QString(", No.%1 to No.%2\"").arg(indexOfJoint(j1)+1).arg(indexOfJoint(j2)+1);
    sendLogMessage(lmsg);

    if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    emit memberChanged(changed_mdlist);
}
*/

/*
void UnifiedData::replaceJointList( MemberData* md, const QList<JointData*> & jdlist, bool signalOn )
{
    QList<JointData*> old_jlist=md->getJointList();
    md->replaceJointList(jdlist);
    if(signalOn) emit jointChanged(old_jlist);

    QList<MemberData*> changed_mdlist;

    if(md->dataType()==TPCOLUMN){
        Q_FOREACH( JointData* jd, old_jlist ){
            Q_FOREACH( MemberData* mm, jd->getRelatedMembers() ){
                if( !mm->isGirderType() ) continue;
                if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
            }
        }
    }

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH(MemberData* mm, changed_mdlist) mm->recalcLineLength();
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("replace Member JointList. \"name=");
    lmsg+=( md->sectionName().isEmpty() ) ? "(dummy)" : md->sectionName();
    lmsg+=QString("\"");
    sendLogMessage(lmsg);

    if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    emit memberChanged(changed_mdlist);
}
*/

/*
void UnifiedData::reverseMemberJoint( const QList<MemberData*>& mdlist , bool signalOn)
{
    QList<MemberData*> changed_mdlist;

    Q_FOREACH( MemberData* md, mdlist ){
        md->reverseJointIndex();
        if( md->dataType()==TPCOLUMN ){
            Q_FOREACH( JointData* jd, md->getJointList() ){
                Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                    if( !mm->isGirderType() ) continue;
                    if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH( MemberData* md, changed_mdlist ) md->recalcLineLength();
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("reverse Members I-J. \"count=%1\"").arg(mdlist.count());
    sendLogMessage(lmsg);

    Q_FOREACH( MemberData* md, mdlist ){
        if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }
    emit memberChanged(changed_mdlist);
}
*/

/*
void UnifiedData::changeMemberSection(const QList<MemberData*>& mdlist, const QString& name , bool signalOn)
{
    QList<MemberData*> changed_mdlist;

    Q_FOREACH( MemberData* md, mdlist ){
        md->setSectionName(name);
        if( md->dataType()==TPCOLUMN ){
            Q_FOREACH( JointData* jd, md->getJointList() ){
                Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                    if( !mm->isGirderType() ) continue;
                    if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }else if( md->isGirderType() ){
            Q_FOREACH( JointData* jd, md->getInteriorJointList() ){
                Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                    if( !mm->isGirderType() ) continue;
                    if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH(MemberData* md, changed_mdlist) md->recalcLineLength();
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("change Members section name. \"count=%1, to ").arg(mdlist.count());
    lmsg+=( name.isEmpty() ) ? "(dummy)" : name;
    lmsg+="\"";
    sendLogMessage(lmsg);

    Q_FOREACH( MemberData* md, mdlist ){
        if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }
    emit memberChanged(changed_mdlist);
    emit elementCountChanged();
}
*/
/*
void UnifiedData::changeBaseOfColumns(const QList<MemberData*>& mdlist,const QString& bname, bool signalOn)
{
    Q_FOREACH(MemberData* md, mdlist){
        md->setBaseName(bname);
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("change ColumnBase. \"count=%1, to ").arg(mdlist.count());
    lmsg+=( bname.isEmpty() ) ? "(dummy)" : bname;
    lmsg+="\"";
    sendLogMessage(lmsg);

    emit memberChanged(mdlist);
}
*/
/*
void UnifiedData::removeBaseOfColumns(const QList<MemberData*>& mdlist, bool signalOn)
{
    Q_FOREACH(MemberData* md, mdlist){
        md->clearBaseName();
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("remove ColumnBase. \"count=%1\"").arg(mdlist.count());
    sendLogMessage(lmsg);

    emit memberChanged(mdlist);
}
*/
/*
void UnifiedData::changeCodeAngle(const QList<MemberData*>& mdlist, qreal ag)
{
    Q_FOREACH(MemberData* md, mdlist)  md->setCodeAngle(ag);

    QList<MemberData*> changed_mdlist;

    Q_FOREACH(MemberData* md, mdlist){
        md->setCodeAngle(ag);
        if( md->dataType()==TPCOLUMN ){
            Q_FOREACH( JointData* jd, md->getJointList() ){
                Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                    if( !mm->isGirderType() ) continue;
                    if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH(MemberData* md, changed_mdlist) md->recalcLineLength();
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("change Members code angle. \"count=%1, to %2 degree\"")
            .arg(mdlist.count()).arg(qRound(ag*180.0/M_PI));
    sendLogMessage(lmsg);

    Q_FOREACH( MemberData* md, mdlist ){
        if( !changed_mdlist.contains(md) ) changed_mdlist.append(md);
    }
    emit memberChanged(changed_mdlist);
}
*/
/*
void UnifiedData::changeShiftValue(const QList<MemberData*>& mdlist,bool y_chg, qreal shy,
                                   bool z_chg, qreal shz, bool signalOn)
{
    Q_FOREACH(MemberData* md, mdlist){
        if( y_chg ) md->setShiftY(shy);
        if( z_chg ) md->setShiftZ(shz);
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("change Members shift value. \"count=%1, to").arg(mdlist.count());
    if( y_chg ) lmsg+=QString(" shiftY=%1mm").arg(qRound(shy));
    if( z_chg ) lmsg+=QString(" shiftZ=%1mm").arg(qRound(shz));
    lmsg+="\"";
    sendLogMessage(lmsg);

    emit memberChanged(mdlist);
}
*/

/*
void UnifiedData::changeFace(const QList<MemberData*>& mdlist,
                             bool ir_chg,int i_id,bool il_chg,qreal i_value,
                             bool jr_chg,int j_id,bool jl_chg,qreal j_value ) // , bool signalOn)
{
    QList<MemberData*> changed_mdlist;

    Q_FOREACH( MemberData* md, mdlist ){
        md->setFaceValues(ir_chg, i_id, il_chg, i_value,
                          jr_chg, j_id, jl_chg, j_value);
        if( md->dataType()==TPCOLUMN ){
            Q_FOREACH( JointData* jd, md->getJointList() ){
                Q_FOREACH( MemberData* mm, jd->getComposeMembers() ){
                    if( !mm->isGirderType() ) continue;
                    if( !changed_mdlist.contains(mm) ) changed_mdlist.append(mm);
                }
            }
        }
    }

    if( !changed_mdlist.isEmpty() ){
        Q_FOREACH(MemberData* md, changed_mdlist) md->recalcLineLength();
    }

    if( !signalOn ) return;

    // Log Message
    QString lmsg=QString("change Members face value. \"count=%1, change").arg(mdlist.count());
    if( ir_chg || il_chg ) lmsg+=QString(" iFace");
    if( jr_chg || jl_chg ) lmsg+=QString(" jFace");
    lmsg+="\"";
    sendLogMessage(lmsg);

    emit memberChanged( changed_mdlist+mdlist );
}
*/
/*
void UnifiedData::changeWallOpen(const QList<MemberData*>& mdlist,
                                 const QString& name, bool signalOn)
{
    Q_FOREACH(MemberData* md, mdlist) md->setWallOpenName(name);

    if( !signalOn ) return;

    // Log Message
    QString lmsg;
    if( name.isEmpty() ){
        lmsg=QString("clear WallOpen. \"count=%1").arg(mdlist.count());
    }else{
        lmsg=QString("change WallOpen. \"count=%1, to").arg(mdlist.count());
        lmsg+=QString(" name=")+name;
    }

    lmsg+="\"";
    sendLogMessage(lmsg);

    emit memberChanged(mdlist);
}
*/

void UnifiedData::recalcMemberCoordinate()
{
    myMemberData->recalcMemberCoordinate();
}

/* 材料情報 */

MATERIALVALUES UnifiedData::getMaterialValue( DATATYPE dtype, const QString &name ) const
{
    return myMaterialData->getMaterialValue(dtype, name);
}

MATERIALVALUES UnifiedData::getReinforceValue( const QString &name ) const
{
    return myMaterialData->getReinforceValue(name);
}

/* 断面情報 */
SectionValues UnifiedData::getSectionValues(DATATYPE type, const QString &name, qreal hh) const
{
    return mySectionData->sectionNameToValue(*this, type, name, hh);
}

QList<SectionValues> UnifiedData::getSectionValueList(DATATYPE type) const{
    return mySectionData->getSectionValues(type);
}

/*
QString UnifiedData::inputNameToSectionName(const QString& inpName, DATATYPE dtype) const
{
    return mySectionData->inputNameToSectionName(inpName,dtype);
}
*/
/*
QStringList UnifiedData::getInputNameList(DATATYPE dtype) const
{
    return mySectionData->getInputNameList(dtype);
}
*/
/*
QStringList UnifiedData::getCombineNameList(DATATYPE dtype) const
{
    QString defaultName;
    if(dtype==TPCOLUMN){
        defaultName=QString(tr("c000: (dummy)"));
    }else if(dtype==TPGIRDER){
        defaultName=QString(tr("g000: (dummy)"));
    }else if(dtype==TPBRACE){
        defaultName=QString(tr("v000: (dummy)"));
    }else if(dtype==TPWALL){
        defaultName=QString(tr("w000: (dummy)"));
    }else if(dtype==TPBEAM || dtype==TPSTUD || dtype==TPUWALL ||
             dtype==TPSLAB || dtype==TPDAMPER || dtype==TPSLABLOAD ||
             dtype==TPMEMBERLOAD || dtype==TPJOINTLOAD ){
        defaultName=QString(tr("(dummy)"));
    }else if(dtype==TPBASE || dtype==TPWALLOPEN){
        defaultName=QString(tr("(none)"));
    }
    QStringList nameList=mySectionData->getCombineNameList(dtype);
    nameList.prepend(defaultName);
    return nameList;
}
*/

LINESHAPE_IN UnifiedData::sectionNameToLineShape(DATATYPE type, const QString &str, qreal hh) const
{
    return mySectionData->sectionNameToLineShape(*this, type, str, hh);
}

LINEPROPERTY UnifiedData::sectionNameToLineProperty(DATATYPE type, const QString &str,
                                                    qreal hh) const
{
    qDebug()<<"UnifiedData::sectionNameToLineProperty()"<<(int)type<<str<<hh;
    return mySectionData->sectionNameToLineProperty(*this, type, str, hh);
}

BASEVALUES UnifiedData::getBaseValues(const QString &str) const
{
    return mySectionData->getBaseValues(str);
}

QList<WallOpenParameter> UnifiedData::getWallOpenValues(const QString &str) const
{
    return mySectionData->getWallOpenValues(str);
}

PLANEPROPERTY UnifiedData::sectionNameToPlaneProperty(DATATYPE type, const QString &str,
                                                      qreal hh) const
{
    return mySectionData->sectionNameToPlaneProperty(*this, type, str, hh);
}

SECTIONWEIGHT UnifiedData::sectionNameToUnitWeight(DATATYPE type, const QString &str,
                                                   qreal hh) const
{
    return mySectionData->sectionNameToUnitWeight(*this, type, str, hh);
}

STRUCTTYPE UnifiedData::sectionNameToStructureType(DATATYPE type, const QString &str,
                                                   qreal hh) const
{
    return mySectionData->sectionNameToStructureType(*this, type, str, hh);
}

qreal UnifiedData::getSlabLoadDir(const QString &str) const
{
    return mySectionData->getSlabLoadDir(str);
}

bool UnifiedData::isStructualSection(DATATYPE type, const QString &str, qreal hh) const
{
    return mySectionData->isStructualSection(*this, type, str, hh);
}


/*
VLINESHAPE UnifiedData::sectionNameToVLineShape(DATATYPE, const QString &, qreal len) const
{

}
*/

/* 荷重情報 */
JOINTLOADVALUES UnifiedData::getJointLoadValue( const QString &str ) const
{
    return myLoadData->getJointLoadValue(str);
}

MEMBERLOADVALUES UnifiedData::getMemberLoadValue( const QString &str ) const
{
    return myLoadData->getMemberLoadValue(str);
}

SLABLOADVALUES UnifiedData::getSlabLoadValue( const QString &str ) const
{
    return myLoadData->getSlabLoadValue(str);
}

QList<JOINTLOADVALUES> UnifiedData::getJointLoadList() const
{
    return myLoadData->getJointLoadList();
}

QList<MEMBERLOADVALUES> UnifiedData::getMemberLoadList() const
{
    return myLoadData->getMemberLoadList();
}

QList<SLABLOADVALUES> UnifiedData::getSlabLoadList() const
{
    return myLoadData->getSlabLoadList();
}

QString UnifiedData::loadNameToLoadProperty(DATATYPE dtype, const QString &str) const
{
    return myLoadData->loadNameToLoadProperty(dtype, str);
}


/* 消去された断面情報を部材に反映 */
void UnifiedData::updateDeletedSection(DATATYPE type, const QStringList &inplist)
{
    QList<MemberData *> mdlist = myMemberData->updateDeletedSection(*this, type, inplist);
    QList<MemberData *> changed_mdlist;

    Q_FOREACH (MemberData *md, mdlist) {

        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);

        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( MemberData *r_md, md->getRelatedMembers() ) {
                if ( !r_md->isGirderType() ) continue;
                if ( !changed_mdlist.contains(r_md) ) changed_mdlist.append(r_md);
            }
        } else if ( md->isGirderType() ) {
            Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *r_md, jd->getComposeMembers() ) {
                    if ( r_md == md ) continue;
                    if ( !r_md->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(r_md) ) changed_mdlist.append(r_md);
                }
            }
        }

    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH (MemberData *md, changed_mdlist) {
            if ( md->isLineMember(false) ) md->recalcLineLength();
        }
        emit memberChanged(changed_mdlist);
    }
}

/* 変更された断面情報を部材に反映 */
void UnifiedData::updateChangedSection(DATATYPE type, const QStringList &inplist)
{
    QList<MemberData *> mdlist = myMemberData->updateChangedSection(*this, type, inplist);
    QList<MemberData *> changed_mdlist;

    Q_FOREACH (MemberData *md, mdlist) {

        if ( !changed_mdlist.contains(md) ) changed_mdlist.append(md);

        if ( md->dataType() == DATATYPE::TPCOLUMN ) {
            Q_FOREACH ( MemberData *r_md, md->getRelatedMembers() ) {
                if ( !r_md->isGirderType() ) continue;
                if ( !changed_mdlist.contains(r_md) ) changed_mdlist.append(r_md);
            }
        } else if ( md->isGirderType() ) {
            Q_FOREACH ( JointData *jd, md->getInteriorJointList() ) {
                Q_FOREACH ( MemberData *r_md, jd->getComposeMembers() ) {
                    if ( r_md == md ) continue;
                    if ( !r_md->isGirderType() ) continue;
                    if ( !changed_mdlist.contains(r_md) ) changed_mdlist.append(r_md);
                }
            }
        }

    }

    if ( !changed_mdlist.isEmpty() ) {
        Q_FOREACH (MemberData *md, changed_mdlist) {
            if ( md->isLineMember(false) ) md->recalcLineLength();
        }
        emit memberChanged(changed_mdlist);
    }
}

void UnifiedData::adjustJointAttachment(DATATYPE dtype, const QList<QUuid> &idlist)
{
    myJointData->removeJointAttachments(dtype, idlist);
}

/* 階コピー */
void UnifiedData::executeFloorCopy( FLOORCOPYTERM copyTerm, QProgressDialog *progress,
                                    QWidget *parent )
{
    Q_UNUSED(parent);
    progress->setValue(5);

    // コピー元の階の節点情報
    QUuid sourceID = copyTerm.sourceFloorID;
    qreal sourceHeight = getFloorHeight(sourceID);
    QList<JointData *> sourceJoints = myJointData->jointListOfFloor(sourceID);
    QList<XYZ> sourceEXYZ;

    QUuid upperID = getUpperFloorID(sourceID);
    qreal upperHeight = getFloorHeight(upperID);
    QList<JointData *> upperJoints;
    QList<XYZ> upperEXYZ;

    QList<int> errorIndexes;

    for ( int i = 0; i < sourceJoints.count(); i++ ) {

        JointData *jd = sourceJoints.at(i);
        QList<QUuid> frames = jd->getAttachedFrameList();
        QUuid firstFrame = ( frames.count() > 0 ) ? frames.first() : QUuid() ;

        if ( copyTerm.isLimitXYValue ) {
            bool isOut = false;
            if ( jd->xPos() < copyTerm.minimumXY.x() || jd->xPos() > copyTerm.maximumXY.x() ) isOut = true;
            if ( jd->yPos() < copyTerm.minimumXY.y() || jd->yPos() > copyTerm.maximumXY.y() ) isOut = true;
            if ( isOut ) {
                errorIndexes.append(i);
                continue;
            }
        }

        if ( copyTerm.isLimitFrame ) {
            bool isContain = false;
            for ( int j = 0; j < frames.count(); j++ ) {
                if ( copyTerm.limitedFrames.contains(frames.at(j)) ) {
                    firstFrame = frames.at(j);
                    isContain = true;
                    break;
                }
            }
            if ( !isContain ) {
                errorIndexes.append(i);
                continue;
            }
        }

        if ( copyTerm.isLimitDataType ) {
            bool isExist = false;
            Q_FOREACH ( MemberData *md, jd->getRelatedMembers() ) {
                if ( !copyTerm.limitedDataTypes.contains(md->dataType()) ) continue;
                bool isContain = true;
                Q_FOREACH ( JointData *jj, md->getJointList() ) {
                    if ( jj->containsFloor(sourceID) ) continue;
                    if ( copyTerm.isCopyUpperFloor && jj->containsFloor(upperID) ) continue;
                    isContain = false;
                    break;
                }
                if ( isContain ) {
                    isExist = true;
                    break;
                }
            }
            if ( !isExist ) {
                errorIndexes.append(i);
                continue;
            }
        }

        if ( copyTerm.isXYBase ) {

            sourceEXYZ.append( XYZ( jd->xPos(), jd->yPos(), jd->zPos() - sourceHeight ) );

        } else {

            if ( frames.count() < 1 ) {
                errorIndexes.append(i);
                continue;
            }
            FRAMEPOINTSVALUE fp = getFrameValueOfFloor(firstFrame, sourceID);
            QList<QPointF> points = fp.pointList;
            bool isOpen = ( fp.frameType != FrameType::FRAMETYPE::CLCIRCLE && fp.frameType != FrameType::FRAMETYPE::CLVARIABLE );
            if ( !isOpen ) points.append( points.first() );
            qreal len = 0.0;
            for ( int j = 1; j < points.count(); j++ ) {
                len += CalcVector2D::length( points.at(j) - points.at(j - 1) );
            }
            if ( len < 1.0e-3 ) {
                errorIndexes.append(i);
                continue;
            }
            QPointF elvXY = CalcVector2D::globalXYToElvXZ(points, QPointF(jd->xPos(), jd->yPos()), isOpen);
            // 第一所属通りの、通りx座標の比率、通りy座標、通りz座標を手掛かりにコピー
            sourceEXYZ.append( XYZ( elvXY.x() / len, jd->zPos() - sourceHeight, elvXY.y() ) );

        }
    }

    for ( int i = sourceJoints.count() - 1; i >= 0; i-- ) {
        if ( errorIndexes.contains(i) ) sourceJoints.removeAt(i);
    }
//    qDebug() << "sourceJoints" << sourceJoints.count();

    errorIndexes.clear();

    // コピー元の上階の節点情報

    if ( copyTerm.isCopyUpperFloor ) {

        Q_FOREACH ( JointData *jd, sourceJoints ) {

            Q_FOREACH ( MemberData *md, jd->getRelatedMembers() ) {
                if (  copyTerm.isLimitDataType && !copyTerm.limitedDataTypes.contains(md->dataType()) ) continue;
                bool isContain = true;
                Q_FOREACH ( JointData *jj, md->getJointList() ) {
                    if ( !jj->containsFloor(sourceID) && !jj->containsFloor(upperID) ) {
                        isContain = false;
                        break;
                    }
                }
                if ( isContain ) {
                    Q_FOREACH ( JointData *jj, md->getJointList() ) {
                        if ( jj->containsFloor(upperID) && !upperJoints.contains(jj) ) upperJoints.append(jj);
                    }
                }
            }
        }

        QList<int> errorIndexes;

        for ( int i = 0; i < upperJoints.count(); i++ ) {

            JointData *jd = upperJoints.at(i);
            QList<QUuid> frames = jd->getAttachedFrameList();
            QUuid firstFrame = ( frames.count() > 0 ) ? frames.first() : QUuid() ;

            if ( copyTerm.isLimitXYValue ) {
                bool isOut = false;
                if ( jd->xPos() < copyTerm.minimumXY.x() || jd->xPos() > copyTerm.maximumXY.x() ) isOut = true;
                if ( jd->yPos() < copyTerm.minimumXY.y() || jd->yPos() > copyTerm.maximumXY.y() ) isOut = true;
                if ( isOut ) {
                    errorIndexes.append(i);
                    continue;
                }
            }

            if ( copyTerm.isLimitFrame ) {
                bool isContain = false;
                for ( int j = 0; j < frames.count(); j++ ) {
                    if ( copyTerm.limitedFrames.contains(frames.at(j)) ) {
                        firstFrame = frames.at(j);
                        isContain = true;
                        break;
                    }
                }
                if ( !isContain ) {
                    errorIndexes.append(i);
                    continue;
                }
            }

            if ( copyTerm.isXYBase ) {

                upperEXYZ.append( XYZ( jd->xPos(), jd->yPos(), jd->zPos() - upperHeight ) );

            } else {

                if ( frames.count() < 1 ) {
                    errorIndexes.append(i);
                    continue;
                }
                FRAMEPOINTSVALUE fp = getFrameValueOfFloor(firstFrame, upperID);
                QList<QPointF> points = fp.pointList;
                bool isOpen = ( fp.frameType != FrameType::FRAMETYPE::CLCIRCLE && fp.frameType != FrameType::FRAMETYPE::CLVARIABLE );
                if ( !isOpen ) points.append( points.first() );
                qreal len = 0.0;
                for ( int j = 1; j < points.count(); j++ ) {
                    len += CalcVector2D::length( points.at(j) - points.at(j - 1) );
                }
                if ( len < 1.0e-3 ) {
                    errorIndexes.append(i);
                    continue;
                }
                QPointF elvXY = CalcVector2D::globalXYToElvXZ(points, QPointF(jd->xPos(), jd->yPos()), isOpen);
                upperEXYZ.append( XYZ( elvXY.x() / len, jd->zPos() - upperHeight, elvXY.y() ) );

            }
        }

        for ( int i = upperJoints.count() - 1; i >= 0; i-- ) {
            if ( errorIndexes.contains(i) ) upperJoints.removeAt(i);
        }

    }
//    qDebug() << "upperJoints" << upperJoints.count();

    // コピー元の部材情報
    QList<MemberData *> sourceMembers;
    Q_FOREACH ( JointData *jd, sourceJoints ) {
        Q_FOREACH ( MemberData *md, jd->getRelatedMembers() ) {
            if ( copyTerm.isLimitDataType && !copyTerm.limitedDataTypes.contains(md->dataType()) ) continue;
            bool isContain = true;
            Q_FOREACH ( JointData *jj, md->getJointList() ) {
                if ( jj == jd ) continue;
                if ( !sourceJoints.contains(jj) && !upperJoints.contains(jj) ) {
                    isContain = false;
                    break;
                }
            }
            if ( isContain && !sourceMembers.contains(md) ) sourceMembers.append(md);
        }
    }
//    qDebug() << "sourceMembers" << sourceMembers.count();

    QList<JointData *> sourceTotalJoints = sourceJoints;
    sourceTotalJoints.append(upperJoints);

    progress->setValue(10);
    int ipp = 10;
    qreal progValue = 90.0 / qreal(copyTerm.destFloors.count());

    // 各階ごとに節点コピー、部材コピー
    QList<JointData *> changedJoints;
    QList<MemberData *> changedMembers;
    Q_FOREACH ( QUuid destFloorID, copyTerm.destFloors ) {

        ipp += int(progValue);
        progress->setValue(ipp);

        qreal destHeight = getFloorHeight(destFloorID);
        QList<JointData *> destJoints;

        QUuid destUpperFloor = getUpperFloorID(destFloorID);
        qreal destUpperHeight = getFloorHeight(destUpperFloor);
        QList<JointData *> destUpperJoints;

        QList<JointData *> newJoints;
        QList<MemberData *> newMembers;

        for ( int i = 0; i < sourceJoints.count(); i++ ) {

            JointData *jd = sourceJoints.at(i);
            XYZ xyz = sourceEXYZ.at(i);
            GlobalPoint gp;

            if ( copyTerm.isXYBase ) {
                gp = GlobalPoint(xyz.x, xyz.y, xyz.z + destHeight);
            } else {
                FRAMEPOINTSVALUE fp = getFrameValueOfFloor(jd->getAttachedFrameList().first(), destFloorID);
                QList<QPointF> points = fp.pointList;
                bool isOpen = ( fp.frameType != FrameType::FRAMETYPE::CLCIRCLE && fp.frameType != FrameType::FRAMETYPE::CLVARIABLE );
                if ( !isOpen ) points.append( points.first() );
                if ( points.isEmpty() ) {
                    destJoints.append(nullptr);
                    continue;
                }
                qreal len = 0.0;
                for ( int j = 1; j < points.count(); j++ ) {
                    len += CalcVector2D::length( points.at(j) - points.at(j - 1) );
                }
                QPointF gXY = CalcVector2D::elvXZToGlobalXY(points, QPointF(xyz.x * len, xyz.z), isOpen);
                gp = GlobalPoint(gXY.x(), gXY.y(), xyz.y + destHeight);
            }

            JointData *near_jd = nearlyJoint(gp);
            if ( near_jd == nullptr ) {
                gp.appendAttachedFloor( destFloorID );
                gp.appendAttachedFrame( jd->getAttachedFrameList() );
                near_jd = myJointData->createJoint(gp);
                newJoints.append(near_jd);
            } else {
                if ( !changedJoints.contains(near_jd) ) changedJoints.append(near_jd);
                if ( copyTerm.isOverwriteJoint ) {
                    near_jd->clearAttachedFloor();
                    near_jd->clearAttachedFrame();
                    near_jd->appendAttachedFloor( destFloorID );
                    near_jd->appendAttachedFrame( jd->getAttachedFrameList() );
                }
            }
            destJoints.append(near_jd);
        }

        for ( int i = 0; i < upperJoints.count(); i++ ) {

            JointData *near_jd = nullptr;
            if ( destUpperFloor.isNull() ) {
                destUpperJoints.append(near_jd);
                continue;
            }

            JointData *jd = upperJoints.at(i);
            XYZ xyz = upperEXYZ.at(i);
            GlobalPoint gp;

            if ( copyTerm.isXYBase ) {
                gp = GlobalPoint(xyz.x, xyz.y, xyz.z + destUpperHeight);
            } else {
                FRAMEPOINTSVALUE fp = getFrameValueOfFloor(jd->getAttachedFrameList().first(), destUpperFloor);
                QList<QPointF> points = fp.pointList;
                bool isOpen = ( fp.frameType != FrameType::FRAMETYPE::CLCIRCLE && fp.frameType != FrameType::FRAMETYPE::CLVARIABLE );
                if ( !isOpen ) points.append( points.first() );
                if ( points.isEmpty() ) {
                    destUpperJoints.append(nullptr);
                    continue;
                }
                qreal len = 0.0;
                for ( int j = 1; j < points.count(); j++ ) {
                    len += CalcVector2D::length( points.at(j) - points.at(j - 1) );
                }
                QPointF gXY = CalcVector2D::elvXZToGlobalXY(points, QPointF(xyz.x * len, xyz.z), isOpen);
                gp = GlobalPoint(gXY.x(), gXY.y(), xyz.y + destUpperHeight);
            }

            near_jd = nearlyJoint(gp);
            if ( near_jd == nullptr ) {
                gp.appendAttachedFloor( destUpperFloor );
                gp.appendAttachedFrame( jd->getAttachedFrameList() );
                near_jd = myJointData->createJoint(gp);
                newJoints.append(near_jd);
            } else {
                if ( !changedJoints.contains(near_jd) ) changedJoints.append(near_jd);
                if ( copyTerm.isOverwriteJoint ) {
                    near_jd->clearAttachedFloor();
                    near_jd->clearAttachedFrame();
                    near_jd->appendAttachedFloor( destUpperFloor );
                    near_jd->appendAttachedFrame( jd->getAttachedFrameList() );
                }
            }
            destUpperJoints.append(near_jd);
        }

        QList<JointData *> destTotalJoints = destJoints;
        destTotalJoints.append(destUpperJoints);

        for ( int i = 0; i < sourceMembers.count(); i++ ) {

            MemberData *md = sourceMembers.at(i);
            QList<int> idxList;
            QList<JointData *> d_jointList;
            bool isError = false;
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                int idx = sourceTotalJoints.indexOf(jd);
                idxList.append( sourceTotalJoints.indexOf(jd) );
                JointData *jj = destTotalJoints.at(idx);
                if ( jj == nullptr ) {
                    isError = true;
                    break;
                }
                d_jointList.append(jj);
            }
            if ( isError ) continue;

            JointData *first_jd = ( md->isClosedMember() )
                                  ? simplifiedMemberJoints(d_jointList, true).first()
                                  : d_jointList.first();

            MemberData *overlapMember = nullptr;
            Q_FOREACH ( MemberData *r_md, first_jd->getRelatedMembers() ) {
                if ( md->dataType() != r_md->dataType() ) continue;
                /*
                                if( myJointData->isOverlapJoints( d_jointList, r_md->getJointList() ) ){
                                    overlapMember=r_md;
                                    break;
                                }
                */
                if ( this->isOverlapJoints( d_jointList, r_md->getJointList(), r_md->isClosedMember() ) ) {
                    overlapMember = r_md;
                    break;
                }
            }

            if ( overlapMember == nullptr ) {

                MemberData *new_md = myMemberData->createMember( d_jointList, md->dataType(), md->sectionName() );
                newMembers.append(new_md);
                if ( new_md->dataType() == DATATYPE::TPCOLUMN ) {
                    Q_FOREACH ( JointData *jd, d_jointList ) {
                        Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                            if ( !mm->isGirderType() ) continue;
                            if ( !changedMembers.contains(mm) && !newMembers.contains(mm) ) changedMembers.append(mm);
                        }
                    }
                } else if ( new_md->isGirderType() ) {
                    Q_FOREACH ( JointData *jd, new_md->getInteriorJointList() ) {
                        Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                            if ( !mm->isGirderType() ) continue;
                            if ( !changedMembers.contains(mm) && !newMembers.contains(mm) ) changedMembers.append(mm);
                        }
                    }
                }
                new_md->copyValuesFromMemberData(*this, md);

            } else if ( copyTerm.isOverwriteMember ) {

                Q_FOREACH ( JointData *jd, overlapMember->getJointList() ) {
                    if ( !changedJoints.contains(jd) ) changedJoints.append(jd);
                }
                overlapMember->replaceJointList(d_jointList);
                overlapMember->copyValuesFromMemberData(*this, md);
                if ( overlapMember->dataType() == DATATYPE::TPCOLUMN ) {
                    Q_FOREACH ( JointData *jd, d_jointList ) {
                        Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                            if ( !mm->isGirderType() ) continue;
                            if ( !changedMembers.contains(mm) && !newMembers.contains(mm) ) changedMembers.append(mm);
                        }
                    }
                } else if ( overlapMember->isGirderType() ) {
                    Q_FOREACH ( JointData *jd, overlapMember->getInteriorJointList() ) {
                        Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                            if ( !mm->isGirderType() ) continue;
                            if ( !changedMembers.contains(mm) && !newMembers.contains(mm) ) changedMembers.append(mm);
                        }
                    }
                }
                if ( !changedMembers.contains(overlapMember) ) changedMembers.append(overlapMember);
            }
        }

        emit jointAppended( newJoints );
        emit memberAppended( newMembers );
    }

    emit jointChanged( changedJoints );
    if ( !changedMembers.isEmpty() ) {
        Q_FOREACH (MemberData *md, changedMembers) md->recalcLineLength();
        emit memberChanged(changedMembers);
    }
    emit elementCountChanged();

    // Log Message
    QString lmsg = QString("execute floor copy. \"sourceFloor=");
    lmsg += idToName(copyTerm.sourceFloorID, DATATYPE::TPFLOOR) + ", ";
    lmsg += QString("destinationFloor=");
    for (int i = 0; i < copyTerm.destFloors.count() ; i++) {
        lmsg += idToName(copyTerm.destFloors.at(i), DATATYPE::TPFLOOR) + ",";
    }
    lmsg += QString(" isBaseOnXY=");
    lmsg += ( copyTerm.isXYBase ) ? "true," : "false," ;
    lmsg += QString(" isCopyUpperFloor=");
    lmsg += ( copyTerm.isCopyUpperFloor ) ? "true," : "false," ;
    lmsg += QString(" isOverwriteJoint=");
    lmsg += ( copyTerm.isOverwriteJoint ) ? "true," : "false," ;
    lmsg += QString(" isOverwriteJoint=");
    lmsg += ( copyTerm.isOverwriteMember ) ? "true," : "false," ;
    lmsg += QString(" isOverwriteMember=");
    lmsg += ( copyTerm.isLimitFrame ) ? "true," : "false," ;
    lmsg += QString(" isLimitFrame=");
    lmsg += ( copyTerm.isLimitDataType ) ? "true," : "false," ;
    lmsg += QString(" isLimitDataType=");
    lmsg += ( copyTerm.isCopyUpperFloor ) ? "true," : "false," ;
    lmsg += QString(" isLimitXYValue=");
    lmsg += ( copyTerm.isLimitXYValue ) ? "true," : "false" ;
    lmsg += "\"";
    emit signalSendLogMessage(lmsg);
}


void UnifiedData::executeJointSort( const JOINTSORTTERM &term )
{
    myJointData->executeJointSort(term);

    emit jointChanged( myJointData->jointListOfAll() );
}


} // namespace post3dapp
