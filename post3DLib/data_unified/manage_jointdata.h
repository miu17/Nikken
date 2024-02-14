#ifndef MANAGEJOINTDATA_H
#define MANAGEJOINTDATA_H

#include <QObject>
#include "define_unifieddata.h" // default引数でUnifiedDataTypeを使用
#include "calc_vector3d.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
enum class DATATYPE;
enum class UnifiedDataType;
class GlobalPoint;
class JointData;
class LibraryVersion;
class MemberData;
class N3DUuidTable;
class UnifiedData;
struct FRAMEPOINTSVALUE;
struct JOINTSORTTERM;
struct JOINTSPRING;
struct FILEREADPATTERN;

class POST3D_CLASS_DLLSPEC ManageJointData : public QObject
{
    Q_OBJECT

public:

    ManageJointData(UnifiedDataType, QObject *parent = nullptr);
    ManageJointData(const ManageJointData&, UnifiedDataType, QObject *parent = nullptr);
    void clearData();
    bool checkData(QDataStream &, const LibraryVersion &ver) const;
    void readData(QDataStream &, const LibraryVersion &ver);
    void readData(QTextStream &, const LibraryVersion &ver);
    void writeP3dData(QDataStream &) const;
    void writePsvData(QTextStream &) const;
    QString writeNsvData(QSharedPointer<N3DUuidTable>) const;

    QHash<QUuid, QUuid> readAdditionalData(QDataStream &, const LibraryVersion &,
                                           const QHash<QUuid, QString> &, const FILEREADPATTERN &);

    int dataCount() const
    {
        return jointList.count();
    }

    JointData *createJoint(const GlobalPoint &, const int jointName, const QUuid &id = QUuid());
    JointData *createJoint(const GlobalPoint &, const QUuid &id = QUuid());
    JointData *nearlyJoint(const GlobalPoint &gp, qreal eps = 1.0e-5) const;
    bool isExistedJoint(JointData *);
    bool isExistedID(const QUuid &) const;
    int indexOfJoint(JointData *) const;
    JointData *jointIndexToPointer(int) const;
    bool removeJoint(JointData *);
    bool checkMovedPosition(const QList<JointData *> &, const QList<GlobalPoint> &);
    bool changeJoint(JointData *, const GlobalPoint &);

//    bool isOverlapJoints( const QList<JointData*> &jdlist1, const QList<JointData*> &jdlist2 ) const;
    QList<JointData *> simplifiedMemberJoints( const QList<JointData *> &,
                                               bool isClosed = false ) const;

    void removeJointAttachments(DATATYPE, const QList<QUuid> &);

    QList<JointData *> jointListOfAll() const;
    QList<JointData *> jointListOfFloor(const QUuid &) const;
    QList<JointData *> jointListOfFloor(const QList<QUuid> &) const;
    QList<JointData *> jointListOfFrame(const QUuid &) const;
    QList<JointData *> jointListOfFrame(const QList<QUuid> &) const;
    bool replaceJointIndexes( const QList<JointData *> & );

    QList<JointData *> jointIDsToPointers(const QList<QUuid> &) const;
    JointData *jointIDToPointer(const QUuid &) const;


    void calcCrossPoints(const QUuid &, const QList<FRAMEPOINTSVALUE> &,
                         QList<GlobalPoint> &, QList<GlobalPoint> & );

    void executeJointSort( const JOINTSORTTERM & );
    QList<QUuid> getIDList(){
        qDebug()<<"jointidkeys="<<uuidTable.keys();
        return uuidTable.keys();
    }
    QUuid analysisJointNoToID(int no);

public slots:

    /*UnifiedData floorChange frameChange　で所属階、所属通りの整理*/

signals:

//    void jointAppended(const QList<JointData*> &);
//    void jointChanged(const QList<JointData*> &);
//    void jointRemoved(const QList<JointData*> &);


private:
    QList<JointData *> jointList;
    QHash<QUuid, JointData *> uuidTable;

    QList<QPointF> crossPointList( const QList<QPointF> &, const QList<QPointF> & );
    int indexOfGlobalPoint( const QList<GlobalPoint> &gplist, QPointF p, qreal eps );

    const UnifiedDataType isOutput;
};


class POST3D_CLASS_DLLSPEC JointData
{
public:
    JointData(const JointData&);
//    JointData( const GlobalPoint &gp, const QUuid &id = QUuid(), const int &jointNo = 0, bool isout = false);
    JointData( const GlobalPoint &, const int &, const QUuid &id = QUuid(), UnifiedDataType type = UnifiedDataType::Input);
    JointData( const GlobalPoint &, const QUuid &id = QUuid(), UnifiedDataType type =  UnifiedDataType::Input);
    JointData(QDataStream &, const LibraryVersion & , UnifiedDataType isout = UnifiedDataType::Input);
    JointData( QTextStream &, const LibraryVersion & ,UnifiedDataType isout = UnifiedDataType::Input);
    ~JointData();

    void writeP3dData(QDataStream &) const;
    void writePsvData(QTextStream &) const;
    QString writeNsvData(QSharedPointer<N3DUuidTable>) const;
    GlobalPoint toGlobalPoint() const;
//    void readData(QDataStream &);

    // ID
    QUuid getUuid() const
    {
        return jointID;
    }

    int getJointNo(UnifiedDataType) const;

    int getAnalysisJointNo() const
    {
        return analysisJointNo;
    }
    void setAnalysisJointNo(int no)
    {
        analysisJointNo = no;
    }

    // 座標
    void setXYZ(XYZ v)
    {
        jointXYZ = v;
    }
    void setX(qreal v)
    {
        jointXYZ.x = v;
    }
    void setY(qreal v)
    {
        jointXYZ.y = v;
    }
    void setZ(qreal v)
    {
        jointXYZ.z = v;
    }
    XYZ xyz(bool adjusted = false ) const; // { return jointXYZ; }
    qreal xPos() const
    {
        return jointXYZ.x;
    }
    qreal yPos() const
    {
        return  jointXYZ.y;
    }
    qreal zPos( bool adjusted = false ) const; // { return jointXYZ.z; }

    //境界条件
    void setBoundary(const QVector<int> &boundary)
    {
        jointBoundary = boundary;
    }
    QVector<int> getBoundary() const
    {
        return jointBoundary;
    }
    void setJointSpring(QList<JOINTSPRING *> springs)
    {
        jointSpring = springs;
    }
    QList<JOINTSPRING *> getJointSpring()const
    {
        return jointSpring;
    }

//    bool isNearlyPoint(const XYZ &pp, qreal eps=1.0e-3) const;
    bool isNearlyPoint(const GlobalPoint &gp, qreal eps = 1.0e-3) const;

    // 共有する部材

    QList<MemberData *> getRelatedMembers() const
    {
        return composeMemberList + interiorMemberList;
    }
    QList<MemberData *> getCriticalMember() const;
    QList<MemberData *> getInteriorMembers() const
    {
        return interiorMemberList;
    }

    void appendComposeMember(MemberData *md);
    void removeComposeMember(MemberData *md);
    bool containsComposeMember(MemberData *md) const
    {
        return composeMemberList.contains(md);
    }
    QList<MemberData *> getComposeMembers() const
    {
        return composeMemberList;
    }

    void appendInteriorMember(MemberData *md);
    void removeInteriorMember(MemberData *md);
    bool containsInteriorMember(MemberData *md) const
    {
        return interiorMemberList.contains(md);
    }

    bool isLoadJoint() const;
    bool isInteriorJoint() const;
    bool isComposeJoint() const;
    bool isIndependentJoint() const;
    void interiorToCompose(MemberData *md);
    void composeToInterior(MemberData *md);

    void removeMemberOfJoint(MemberData *md);
    bool containsMember(MemberData *md) const
    {
        return ( interiorMemberList.contains(md) || composeMemberList.contains(md) );
    }

    // その節点と部材を介して前後関係になっている節点
    QList<JointData *> linkedJoints() const;
    QList<JointData *> linkedJoints(const QList<DATATYPE> &) const;
    QList<JointData *> linkedAnalysisJoints(const UnifiedData&) const;

    // その節点と部材を介して繋がっている節点（他点を挟んでも良い）
    QList<JointData *> relatedJoints() const;
    QList<JointData *> relatedJoints(const QList<DATATYPE> &) const;

    // 所属階、所属通り
    QList<QUuid> getAttachedFloorList() const;
    QList<QUuid> getAttachedFrameList() const;
    QUuid getFirstFloor() const;  // 全所属階のうち最下階のID

    // 解析節点として残る点か否か
    bool isAnalysisJoint(const UnifiedData&) const;

    void appendAttachedFloor(const QUuid &id);
    void appendAttachedFloor(const QList<QUuid> &idlist);
    void appendAttachedFrame(const QUuid &id);
    void appendAttachedFrame(const QList<QUuid> &idlist);
    void removeAttachedFloor(const QUuid &id);
    void removeAttachedFloor(const QList<QUuid> &idlist);
    void removeAttachedFrame(const QUuid &id);
    void removeAttachedFrame(const QList<QUuid> &idlist);
    void replaceAttachedFloor(const QList<QUuid> &idlist);
    void replaceAttachedFrame(const QList<QUuid> &idlist);
    void clearAttachedFloor();
    void clearAttachedFrame();

    bool containsFloor(const QUuid &id) const;
    bool containsFloor(const QList<QUuid> &idlist) const;
    bool containsFrame(const QUuid &id) const;
    bool containsFrame(const QList<QUuid> &idlist) const;

    void clearNotFoundFloor();
    void clearNotFoundFrame();

private:
    UnifiedDataType isOutput;

    QUuid jointID;
    int analysisJointNo; // 番号
    int analysisJointName; // 節点名
    XYZ jointXYZ;
    QVector<int> jointBoundary;//x y z rx ry rz
    QList<JOINTSPRING *> jointSpring;

    QList<MemberData *> composeMemberList;
    QList<MemberData *> interiorMemberList;

    QList<QUuid> attachedFloor;
    QList<QUuid> attachedFrame;
};


struct JOINTGROUP {
    explicit JOINTGROUP( JointData *_j1 = nullptr,
                         JointData *_j2 = nullptr,
                         JointData *_j3 = nullptr )
        : j1(_j1), j2(_j2), j3(_j3) { }

    JointData *j1;
    JointData *j2;
    JointData *j3;
};
struct POST3D_CLASS_DLLSPEC JOINTSPRING {
    explicit JOINTSPRING() { }
    QUuid uid;
    int springNo;
    bool isRotation;
    VEC3D springVec;

};
} // namespace post3dapp
#endif
