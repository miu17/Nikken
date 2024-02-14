#ifndef UNIFIEDDATA_H
#define UNIFIEDDATA_H

#include <QUuid>
#include "define_unifieddata.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
class QProgressDialog;
class QTextStream;
class QUndoCommand;
class QUndoStack;

namespace post3dapp{

enum class DATATYPE;
enum class STRUCTTYPE;
enum class UnifiedDataType;
enum class LOADTYPE;
class GlobalPoint;
class JointData;
class LibraryVersion;
class ManageFloorData;
class ManageFrameData;
class ManageJointData;
class ManageLoadData;
class ManageMaterialData;
class ManageMemberData;
class ManageSectionData;
class MemberData;
struct BASEVALUES;
struct FILEREADPATTERN;
struct FLOORCOPYTERM;
struct FLOORVALUES;
struct FRAMEGROUPVALUE;
struct FRAMEPOINTSVALUE;
struct JOINTLOADVALUES;
struct JOINTSORTTERM;
struct LINESHAPE_IN;
struct LINEPROPERTY;
struct MATERIALVALUES;
struct MEMBERLOADVALUES;
struct PLANEPROPERTY;
struct SectionValues;
struct SECTIONWEIGHT;
struct SLABLOADVALUES;
struct WallOpenParameter;
struct XYZ;

static const int IFFIX = 99999;

class POST3D_CLASS_DLLSPEC UnifiedData : public QObject
{
    Q_OBJECT

public:
    static QString dataTypeToJapanese(DATATYPE);
    static QString dataTypeToEnglish(DATATYPE);
    static DATATYPE stringToDataType(const QString &);
    /* ファイル */
    QString fileName()const{return filename;}
    void setFileName(const QString &name){filename=name;}

    /* 名前とＩＤ */
    QString idToName(const QUuid &id, DATATYPE) const;
    QUuid nameToID(const QString &str, DATATYPE) const;
    QStringList idsToNames(const QList<QUuid> &id, DATATYPE) const;
    QList<QUuid> namesToIDs(const QStringList &str, DATATYPE) const;
    bool isExistedName(const QString &str, DATATYPE) const;
    bool isExistedID(const QUuid &id, DATATYPE) const;
    virtual QList<QUuid> getIDList(DATATYPE) const;
    QStringList getNameList(DATATYPE) const;
    QStringList getReinforceNameList() const;


    /* no とID */
    QUuid noToID(const int no,DATATYPE)const;

    /* 合計数問い合わせ */
    int getDataCount(DATATYPE type, bool isList = false) const;
    QHash<QString, int> getMemberSectionCount(DATATYPE type);

    /* 階の問い合わせ */
    FLOORVALUES getFloorValue(const QUuid &uid) const;
    QList<FLOORVALUES> getFloorList() const;
    qreal getAdjustedFloorLevel(const QUuid &uid) const;
    bool isChildFloor(const QUuid &uid) const;
    qreal getFloorHeight(const QUuid &uid) const;
    qreal getMinimumFloorHeight() const;
    qreal getMaximumFloorHeight() const;
    QList<QUuid> getFloorListInRange(qreal hmin, qreal hmax) const;
    QList<QUuid> getNearlyFloorOfHeight(qreal h) const;
    QList<QUuid> getNearlyFloorOfHeight(const QList<QUuid> &idlist, qreal h) const;
    QList<QUuid> sortFloorOrderOfHeight(const QList<QUuid> &idlist) const;
    QUuid getUpperFloorID(const QUuid &fl, int iu = 1) const;
    QList<QUuid> getFloorRange(const QUuid &, const QUuid &) const;
    QUuid getNearlyDownFloorID(qreal hh) const;
    bool withinFloors(const QUuid &f1, const QUuid &f2, qreal hh) const;
    bool withinFloors(const QUuid &f1, const QUuid &f2,
                      const QUuid &f3) const; // f3がf1とf2の間かどうか
    QUuid getFloorOfHeight(qreal hh) const;
    QUuid getBaseLevelFloor() const;
    QUuid getRoofTopFloor() const;

    /* 通りの問い合わせ */
    QList<FRAMEPOINTSVALUE> getFrameValuesOfFloor( const QUuid &fid) const;
    FRAMEPOINTSVALUE getFrameValueOfFloor( const QUuid &fr_id, const QUuid &fl_id ) const;
    QList<QPointF> getFramePointsOfFloor( const QUuid &fr_id, const QUuid &fl_id) const;
    QList<QUuid> getFloorListOfFrame( const QUuid &fr_id) const;
    FrameType::FRAMETYPE getFrameTypeOfFloor( const QUuid &fr_id, const QUuid &fl_id = QUuid() ) const;
    QList<QUuid> getFrameRange(const QUuid &, const QUuid &) const;
    //ibaraki defined
//    QList<QUuid> getFrameGroupListOfFloor()const;
//    QList<QUuid> getFrameListOfGroup()const;
//    qreal getFrameCoordinate()const;
    QList<FRAMEGROUPVALUE> getFrameGroupList()const;
    QList<FRAMEPOINTSVALUE> getFramePointList()const;
    QUuid getJointId(const QString& floorName, const QString& gridName1, const QString& gridName2);


    /*座標系の変換関数 xyz XYZ  単位m*/
    XYZ frameXYZToGlobalXYZ(const QUuid &fr_id, XYZ p0) const;
    QList<XYZ> frameXYZToGlobalXYZ(const QUuid &fr_id, const QList<XYZ> &p0_list) const;
    XYZ globalXYZToFrameXYZ(const QUuid &fr_id, XYZ xyz) const;
    QList<XYZ> globalXYZToFrameXYZ(const QUuid &fr_id, const QList<XYZ> &p0_list) const;
    qreal globalXYZToFrameAngle(const QUuid &fr_id, XYZ p0) const;
    qreal globalZToClosedFrameEndX(const QUuid &fr_id, qreal z) const;

    /* データチェック＆登録 */
    bool checkTempData(QWidget *, const QList<FLOORVALUES> &) const;
    bool checkTempData(QWidget *, const QList<FRAMEGROUPVALUE> &,
                       const QList<FRAMEPOINTSVALUE> &) const;
    bool checkTempData(QWidget *, const QList<MATERIALVALUES> &) const;

    bool checkTempData(QWidget *, DATATYPE, const QList<SectionValues> &) const;

    bool checkTempData(QWidget *, const QList<JOINTLOADVALUES> &) const;
    bool checkTempData(QWidget *, const QList<MEMBERLOADVALUES> &) const;
    bool checkTempData(QWidget *, const QList<SLABLOADVALUES> &) const;

    void clearTempData(DATATYPE type);
    void registerTempData(DATATYPE type);
    void registerAnalysisFloorData(const QList<FLOORVALUES> &, const QUuid &, const QUuid &);

    /* ファイル読込、ファイル保存 */
    void registerTempData(const QList<FLOORVALUES> &);
    void registerTempData(const QList<FRAMEGROUPVALUE> &, const QList<FRAMEPOINTSVALUE> &);
    void registerTempData(const QList<MATERIALVALUES> &);
    void registerTempData(DATATYPE, const QList<SectionValues> &);
    void registerTempData(const QList<JOINTLOADVALUES> &);
    void registerTempData(const QList<MEMBERLOADVALUES> &);
    void registerTempData(const QList<SLABLOADVALUES> &);
    void clearDefinedData();
    bool checkUnifiedData(QDataStream &, const LibraryVersion &ver)const;
    void readTextJointData(QTextStream &, const LibraryVersion &ver);
    void readTextMemberData(QTextStream &, DATATYPE type, const LibraryVersion &ver);
    void readAdditionalData(QDataStream &, const LibraryVersion &,
                            const QHash<QUuid, QString> &, const FILEREADPATTERN & );
    void writeLimitedData(QDataStream &, const QList<QUuid> &) const;
    void writeDividedData(QDataStream &, QDataStream &,
                          const QList<QUuid> &) const;

    /* 節点の追加、削除、変更 */
    JointData *createAnalysisJoint(const GlobalPoint &gp, const int, bool signalOn = true, const QUuid &id = QUuid());
    JointData *createJoint(const GlobalPoint &gp, bool signalOn = true, const QUuid &id = QUuid());
    QList<JointData *> createJoints(const QList<GlobalPoint> &gplist, bool signalOn = true,
                                    const QList<QUuid> &idlist = QList<QUuid>());
    void removeJoints(QList<JointData *> &, bool signalOn = true);
    bool changeJoints(QList<JointData *> &, const QList<GlobalPoint> &, bool signalOn = true);
    // 指定の全節点に同じ属性を付加する
    void appendJointsAttachment(QList<JointData *> &, const QList<QUuid> &,
                                DATATYPE, bool signalOn = true);
    // 指定の全節点を一様な属性に変更
    void changeJointsAttachment(QList<JointData *> &, const QList<QUuid> &,
                                DATATYPE, bool signalOn = true);
    // 指定の節点それぞれの属性を変更
    void changeJointsAttachment(QList<JointData *> &, const QList<QList<QUuid> > &,
                                DATATYPE, bool signalOn = true);
    void completeJointsAttachment(const QList<JointData *> &,
                                   bool overwrite = true); // 属性の自動補完

    QList<JointData*> getJointBetween(const GlobalPoint &p1,const GlobalPoint &p2, qreal eps = 0.001)const;
    JointData *nearlyJoint(const GlobalPoint &gp, qreal eps = 1.0e-5) const;
    bool isExistedJoint(JointData *) const;
    int indexOfJoint(JointData *) const;
    JointData *jointIndexToPointer(int) const;

    bool existOverlapMember(const QList<JointData *> &, DATATYPE);
    bool isOverlapJoints(const QList<JointData *> &, const QList<JointData *> &,
                         bool isClosed = false) const;
    QList<JointData *> simplifiedMemberJoints( const QList<JointData *> &,
                                               bool isClosed = false ) const;

    QList<JointData *> jointListOfAll() const;
    QList<JointData *> jointListOfFloor(const QUuid &) const;
    QList<JointData *> jointListOfFloor(const QList<QUuid> &) const;
    QList<JointData *> jointListOfFrame(const QUuid &) const;
    QList<JointData *> jointListOfFrame(const QList<QUuid> &) const;
    bool replaceJointIndexes( const QList<JointData *> & );

    JointData *jointIDToPointer(const QUuid &) const;
    QList<JointData *> jointIDsToPointers(const QList<QUuid> &) const;

    void calcCrossPoints(const QUuid &, const QList<FRAMEPOINTSVALUE> &,
                         QList<GlobalPoint> &, QList<GlobalPoint> & );




    /* 部材の追加、削除、変更 */
    MemberData *createMember(const QList<JointData *> &, DATATYPE, const QString &,
                             const QUuid &id = QUuid() );
    QList<MemberData *> createMembers(const QList< QList<JointData *> > &, DATATYPE,
                                      const QString &, const QList<QUuid> &idlist = QList<QUuid>() );
    void removeMembers(const QList<MemberData *> &, bool signalOn = true);
    void removeJointOfMember(JointData *, MemberData *);
    void removeJointOfMember(JointData *, const QList<MemberData *> &);
    void removeJointsOfMember(const QList<JointData *> &, MemberData *);
    bool isExistedMember(MemberData *) const;

    QList<MemberData *> memberListOfAll() const;
    QList<MemberData *> memberListExceptLoad() const;
    QList<MemberData *> memberListOfFloor(const QUuid &) const;
    QList<MemberData *> memberListOfFrame(const QUuid &) const;
    QList<MemberData *> memberListOfType(DATATYPE type) const;
    QList<MemberData *> memberListOfType(const QList<DATATYPE> &types) const;

    MemberData *memberIDToPointer(const QUuid &) const;
    QList<MemberData *> memberIDsToPointers(const QList<QUuid> &) const;

    bool interiorMember( MemberData *, const QList<JointData *> & );
    QList<MemberData *> divideMember( MemberData *, const QList<JointData *> &,
                                      const QList<QUuid> &idlist = QList<QUuid>() );

    QList<MemberData *> divideMemberByInteriorJoints( MemberData *, const QList<JointData *> &,
                                                      const QList<QUuid> &idlist = QList<QUuid>() );
    void uniteMembers(const QList<MemberData *> &);
    void recalcMemberCoordinate();


    /* 材料情報 */
    MATERIALVALUES getMaterialValue( DATATYPE, const QString & ) const;
    MATERIALVALUES getReinforceValue( const QString & ) const;


    /* 断面情報 */

    LINESHAPE_IN sectionNameToLineShape(DATATYPE, const QString &, qreal hh = 0.0) const;
    LINEPROPERTY sectionNameToLineProperty(DATATYPE, const QString &, qreal hh = 0.0) const;

    BASEVALUES getBaseValues(const QString &) const;
    QList<WallOpenParameter> getWallOpenValues(const QString &) const;
    SectionValues getSectionValues(DATATYPE, const QString &, qreal hh = 0.0) const;
    PLANEPROPERTY sectionNameToPlaneProperty(DATATYPE, const QString &, qreal hh = 0.0) const;
    SECTIONWEIGHT sectionNameToUnitWeight(DATATYPE, const QString &, qreal hh = 0.0) const;
    STRUCTTYPE sectionNameToStructureType(DATATYPE, const QString &, qreal hh = 0.0) const;
    qreal getSlabLoadDir(const QString &) const;
    bool isStructualSection(DATATYPE, const QString &, qreal hh = 0.0) const;

    QList<SectionValues> getSectionValueList(DATATYPE) const;

    /* 荷重情報 */
    JOINTLOADVALUES getJointLoadValue( const QString & ) const;
    MEMBERLOADVALUES getMemberLoadValue( const QString & ) const;
    SLABLOADVALUES getSlabLoadValue( const QString & ) const;

    QList<JOINTLOADVALUES> getJointLoadList() const;
    QList<MEMBERLOADVALUES> getMemberLoadList() const;
    QList<SLABLOADVALUES> getSlabLoadList() const;

    QString loadNameToLoadProperty(DATATYPE, const QString &) const;


    /* 階コピー */
    void executeFloorCopy( FLOORCOPYTERM term, QProgressDialog *progress, QWidget *parent );


    /* 節点番号ソート */
    void executeJointSort( const JOINTSORTTERM & );


    /* シグナルの強制発生 */
    void signalJointAppended(const QList<JointData *> &);
    void signalJointChanged(const QList<JointData *> &);
    void signalJointRemoved(const QList<JointData *> &);
    void signalMemberAppended(const QList<MemberData *> &);
    void signalMemberChanged(const QList<MemberData *> &);
    void signalMemberRemoved(const QList<MemberData *> &, DATATYPE);
    void signalElementCountChanged();


public slots:

    void updateDeletedSection(DATATYPE, const QStringList &);
    void updateChangedSection(DATATYPE, const QStringList &);

    void adjustJointAttachment(DATATYPE, const QList<QUuid> &);

signals:

    void dataUpdated(DATATYPE);

    void dataCleared();
    void dataReaded();


    void elementCountChanged();

    /*Jointに関するシグナル*/
    void jointAppended(const QList<JointData *> &);
    void jointChanged(const QList<JointData *> &);
    void jointRemoved(const QList<JointData *> &);

    /*Memberに関するシグナル*/
    void memberAppended(const QList<MemberData *> &);
    void memberChanged(const QList<MemberData *> &);
    void memberRemoved(const QList<MemberData *> &, DATATYPE);

    void signalSendLogMessage(const QString&);

    void existCopyBuffer(UnifiedData*);
    void clearCopyBuffer();
protected:
    UnifiedData(UnifiedDataType);
    UnifiedData(const UnifiedData &);
    UnifiedData &operator=(const UnifiedData &);
    virtual ~UnifiedData(){};

    ManageFloorData *myFloorData;
    ManageFrameData *myFrameData;
    ManageMaterialData *myMaterialData;
    ManageSectionData *mySectionData;
    ManageJointData *myJointData;
    ManageMemberData *myMemberData;
    ManageLoadData *myLoadData;

    QString filename;

};
} // namespace post3dapp
#endif
