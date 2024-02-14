#ifndef MANAGEELEMENTDATA_H
#define MANAGEELEMENTDATA_H

#include <QTextStream>
#include "define_unifieddata.h"
#include "calc_vector3d.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class JointData;
class LibraryVersion;
class MemberData;
class ManageJointData;
class UnifiedData;
class N3DUuidTable;
class POST3D_CLASS_DLLSPEC ManageMemberData : public QObject
{
    Q_OBJECT

public:

    ManageMemberData(UnifiedDataType isout, QObject *parent = nullptr);
    ManageMemberData(const ManageMemberData&, const ManageJointData&, const UnifiedData&, QObject *parent = nullptr);
    void clearData();
    bool checkData(QDataStream &, const LibraryVersion &ver) const;
    void readData(QDataStream &, const LibraryVersion &ver);
    void writeP3dData(QDataStream &) const;
    void readData(QTextStream &, DATATYPE type, const LibraryVersion &ver);
    void writePsvData(QTextStream &) const;
    QString writeNsvData(QSharedPointer<N3DUuidTable>) const;

    void readAdditionalData(QDataStream &, const LibraryVersion &,
                            const QHash<QUuid, QUuid> &, const FILEREADPATTERN &);

    void writeLimitedData(QDataStream &, const QList<MemberData *> &) const;


    int dataCount(DATATYPE type) const;
    QHash<QString, int> getMemberSectionCount(const UnifiedData&, DATATYPE type);

    MemberData *createMember(const QList<JointData *> &, DATATYPE, const QString &,
                             const QUuid &id = QUuid());
    void removeMember(MemberData * );
    void removeJointOfMember(JointData *, MemberData * );
    void removeJointOfMember(JointData *, const QList<MemberData *> & );
    void removeJointsOfMember(const QList<JointData *> &, MemberData * );
    bool isExistedMember(MemberData * ) const;
    bool isExistedID(const QUuid &) const;

    QList<MemberData *> memberListOfAll() const;
    QList<MemberData *> memberListExceptLoad() const;
    QList<MemberData *> memberListOfFloor(const QUuid &) const;
    QList<MemberData *> memberListOfFrame(const QUuid &) const;
    QList<MemberData *> memberListOfType(DATATYPE type) const;
    QList<MemberData *> memberListOfType(const QList<DATATYPE> &types) const;

    MemberData *memberIDToPointer(const QUuid &) const;
    QList<MemberData *> memberIDsToPointers(const QList<QUuid> &) const;

    // 部材の線上に内分点を作る
    bool interiorMember(MemberData *, QList<JointData *>);

    // 部材を分割する
    QList<MemberData *> divideMember(const UnifiedData&, MemberData *, const QList<JointData *> &,
                                     const QList<QUuid> &idlist = QList<QUuid>());

    // 部材を既存の内分点で分割する
    QList<MemberData *> divideMemberByInteriorJoints(const UnifiedData&, MemberData *, const QList<JointData *> &,
                                                     const QList<QUuid> &idlist = QList<QUuid>());

    // 部材を１本化する
    void uniteMembers( const QList<MemberData *> &mdlist );

    // 消去された断面情報を部材に反映
    QList<MemberData *> updateDeletedSection(const UnifiedData&, DATATYPE, const QStringList & );

    // 変更された断面情報を部材に反映
    QList<MemberData *> updateChangedSection(const UnifiedData&, DATATYPE, const QStringList & );

    // 部材座標系再計算
    void recalcMemberCoordinate();

    QList<MemberData *> replaceSectionName(const UnifiedData&, DATATYPE, const QString &, const QString &);

    QList<QUuid> getIDList(){
        return uuidTable.keys();
    }

public slots:

    /* UnifiedData sectionChange で断面の整理 */

signals:

private:

    QList<MemberData *> columnList;
    QList<MemberData *> girderList;
    QList<MemberData *> braceList;
    QList<MemberData *> beamList;

    QList<MemberData *> wallList;
    QList<MemberData *> uwallList;
    QList<MemberData *> slabList;

    QList<MemberData *> damperList;
    QList<MemberData *> isoList;

    QList<MemberData *> jointLoadList;
    QList<MemberData *> memberLoadList;
    QList<MemberData *> slabLoadList;

    QHash<QUuid, MemberData *> uuidTable;

    void appendDataList(MemberData * );
    void removeDataList(MemberData * );

    QHash<QString, int> countSectionName(const QList<MemberData *> &, const QStringList &,
                                         int counter[]);

    /*　柱脚、壁開口は部材に従属　*/

    const UnifiedDataType isOutput ;

    QString writeColumnNsvData(QSharedPointer<N3DUuidTable>, QList<MemberData*>*) const;
    QString writeGirderNsvData(QSharedPointer<N3DUuidTable>, QList<MemberData*>*) const;
    QString writeBraceNsvData(QSharedPointer<N3DUuidTable>, QList<MemberData*>*) const;
    QString writeDamperNsvData(QSharedPointer<N3DUuidTable>, QList<MemberData*>*) const;
    QString writeIsolatorNsvData(QSharedPointer<N3DUuidTable>, QList<MemberData*>*) const;
    QString writeSlabNsvData(QSharedPointer<N3DUuidTable>) const;
    QString writeWallNsvData(QSharedPointer<N3DUuidTable>, QList<MemberData*>*, QList<MemberData*>*) const;
    QString writeSteelWallNsvData(QSharedPointer<N3DUuidTable>) const;
    QString writeFoundationWallNsvData(QSharedPointer<N3DUuidTable>) const;

};
class POST3D_CLASS_DLLSPEC GeneralMemberData
{
public:
    // ID
    QUuid getUuid() const
    {
        return memberID;
    }

    JointData *iJoint()const;
    JointData *jJoint()const;
    bool isIJoint(const JointData*) const;
    bool isJJoint(const JointData*) const;

    XYZ iXYZ()const;
    XYZ jXYZ()const;

    bool containsFloor(const QUuid &id) const; // 部材内の全節点が含まれる
    bool containsFloors(const QList<QUuid> &idlist) const; // 部材内の全節点が含まれる
    bool containsFrame(const QUuid &id) const; // 部材内の全節点が含まれる
    bool containsFrames(const QList<QUuid> &idlist) const; // 部材内の全節点が含まれる
    bool touchFloor(const QUuid &id) const; // 部材内のある点が含まれる
    bool touchFloors(const QList<QUuid> &idlist) const; // 部材内のある点が含まれる
    bool touchFrame(const QUuid &id) const; // 部材内のある点が含まれる
    bool touchFrames(const QList<QUuid> &idlist) const; // 部材内のある点が含まれる
    QUuid getAttachedFloor() const; // 所属階（階はi端の所属、通りは両端の所属）

    VEC3D getUVector() const;
    VEC3D getVVector() const;
    VEC3D getWVector() const;

protected:

    QUuid memberID;
    QList<JointData *> jointList;
    VEC3D U_Vec;
    VEC3D V_Vec;

};
class POST3D_CLASS_DLLSPEC MemberData: public GeneralMemberData
{

public:
    MemberData(const MemberData&, const ManageJointData&, const UnifiedData&);
    MemberData(const QList<JointData *> &jlist, DATATYPE type, const QString &str,
                const QUuid &id = QUuid() , int _no = 0, UnifiedDataType isout = UnifiedDataType::Input); //  bool execRecalc=true);
    MemberData( QDataStream &, const LibraryVersion &,UnifiedDataType isout = UnifiedDataType::Input);
    MemberData( QTextStream &,const LibraryVersion &,UnifiedDataType isout = UnifiedDataType::Input);
    ~MemberData();

    // ID
    QUuid getUuid() const
    {
        return memberID;
    }

    void writeP3dData(QDataStream &) const;
    void writePsvData(QTextStream &) const;
    QList<QString> writeNsvData(QSharedPointer<N3DUuidTable>, const DATATYPE&) const;
    void copyValuesFromMemberData(const UnifiedData&, MemberData *md);
    void copyValues(const UnifiedData&, const MEMBERVALUES &);

    MEMBERVALUES getMemberValues() const
    {
        return myMemberValues;
    }
    int getMemberNo() const
    {
        return no;
    }
    DATATYPE dataType() const
    {
        return myMemberValues.dataType;
    }
    void setSectionName(const UnifiedData&, const QString &name);
    STRUCTTYPE structureType(const UnifiedData& ud)const
    {
        if(dataType()==DATATYPE::TPSLAB || dataType()==DATATYPE::TPWALL
                || dataType()==DATATYPE::TPUWALL){
            return getPlaneProperty(ud).structType;
//            return getPlaneProperty(ud).structType;
        }else{
            return getLineProperty(ud).structType;
        }
    }
    QString sectionName() const
    {
        return myMemberValues.sectionName;
    }
    int jointCount()
    {
        return jointList.count();
    }

    bool isLineMember(bool includingLoad = true) const;
    bool isClosedMember(bool includingLoad = true) const;
    bool isJointMember() const;
    bool isAnalysisLineMember() const;
    bool isAnalysisClosedMember(const UnifiedData&) const;
    bool isAnalysisMember(const UnifiedData&) const;
    bool isGirderType() const;

    QString basePlateName() const
    {
        return myLineShape.baseName;
    }

    bool hasWallOpen() const
    {
        return !myMemberValues.wallOpenName.isEmpty();
    }
    void setWallOpen(const QString &name);
    QString wallOpenName() const
    {
        return myMemberValues.wallOpenName;
    }
    void setFaceBaneName(const QString &name_iy,const QString &name_iz,const QString &name_jy,const QString &name_jz);

    void removeOneJoint(JointData *jd);
    void replaceJoint(JointData *a_jd, JointData *b_jd, bool recalc = true);
    void replaceJointList(const QList<JointData *> &new_list);
    void reverseJointIndex();  //　I,J端の入れ替え
    void changeJointList(const QList<JointData *> &jdlist) ; // 節点の総入れ替え

    void insertComposeJoint(int idx, JointData *jd);  // 面材のみ
    void insertInteriorJoint(int idx, JointData *jd);  //　線材
    int indexOfOnLineJoint(const JointData* jd) const;
    /* 与えられた節点群が部材軸上にある場合、全ての部材点と合わせて並び替えた点列を返す */
    QList<JointData *> orderOnLineJoint(const QList<JointData *> &) const;
    /* 3次元上の点を部材軸上に投影し、部材長さ系の長さを返す 単位:m系 */
    qreal perpLengthOfMember(const XYZ &) const;

    void appendIJoint(JointData *jd);  //　線材
    void appendJJoint(JointData *jd);  //　線材

    JointData *jointAt(int) const;
    int indexOfMemberJoint(const JointData*) const;

    JointData* previousJoint(const JointData*)const;
    JointData* nextJoint(const JointData*)const;
    QList<JointData *> aroundJoints(const JointData*) const;
    QList<JointData *> aroundAnalysisJoints(const UnifiedData&, const JointData*) const;

    bool isCriticalJoint (const JointData*) const;
    bool isComposeJoint (const JointData*) const;
    bool isCornerJoint (const JointData*) const;

    QList<JointData *> getJointList() const
    {
        return jointList;
    }


    QList<JointData *> getInteriorJointList() const;
    QList<JointData *> getAnalysisJointList(const UnifiedData&) const;
    QList<MemberData *> getRelatedMembers() const;
    QList<MemberData *> getComposeMembers() const;
    QList<MemberData *> getLineMembersAroundPlane() const;




    void setDirectionVector(VEC3D vx,VEC3D vy)
    {
        U_Vec = vx;
        V_Vec = vy;
    }
    void setCodeAngle(qreal ag = 0.0)
    {
        myMemberValues.codeAngle = ag;
        myLineShape.angle = ag;
    }
    void setShiftY(qreal shy = 0.0)
    {
        myMemberValues.shiftY = shy;
        myLineShape.shiftY = shy;
    }
    void setShiftZ(qreal shz = 0.0)
    {
        myMemberValues.shiftZ = shz;
        myLineShape.shiftZ = shz;
    }
    qreal codeAngle() const
    {
        return myMemberValues.codeAngle;
    }
    qreal getShiftY() const
    {
        return myMemberValues.shiftY;
    }
    qreal getShiftZ() const
    {
        return myMemberValues.shiftZ;
    }

    bool isInteriorJoint(JointData* const) const;
    QList<QUuid> getAttachedFrameList()const; // 所属通り（階はi端の所属、通りは両端の所属）

    void recalcLineLength();
    void recalcJointLength();
    void recalcFaceLength();
    void recalcPartition();
    void resetLineShape(const UnifiedData&);

    void recalcLocalVector(); // 部材座標系再計算

    // 単位 mm
    qreal getJointLength(int) const;
    qreal getJointLength(JointData *) const;
    qreal getTotalLength() const;
    qreal getInternalLength() const;

    HLINESHAPE getHLineShape() const;
    MEMBERPARTITION getMemberPartition() const;
    QList<qreal> getPartitionLenList() const;
    VLINESHAPE getVLineShape(qreal) const;
    MEMBERSIZE getSizeOfJoint(JointData *) const;
    LINEPROPERTY getLineProperty(const UnifiedData&) const;
    PLANEPROPERTY getPlaneProperty(const UnifiedData&) const;
    qreal getCenterWidth() const
    {
        return myLineShape.centerSizeB;
    }

    void setFaceValues(bool, int, bool, qreal, bool, int, bool, qreal);
    void setFaceValues(FACERULE, qreal, FACERULE, qreal);
    FACERULE getIFaceRule() const
    {
        return myMemberValues.iFaceRule;
    }
    FACERULE getJFaceRule() const
    {
        return myMemberValues.jFaceRule;
    }
    qreal getIFace() const
    {
        return myMemberValues.iFace;
    }
    qreal getJFace() const
    {
        return myMemberValues.jFace;
    }
    qreal getIFaceLength() const
    {
        return myPartition.lenFi;
    }
    qreal getJFaceLength() const
    {
        return myPartition.lenFj;
    }

    void setPartingValues(PARTINGLEN, PARTINGRULE, qreal, qreal, qreal, qreal);
    PARTINGLEN getPartingLength() const
    {
        return myMemberValues.partingLength;
    }
    PARTINGRULE getPartingRule() const
    {
        return myMemberValues.partingRule;
    }
    qreal getILength() const
    {
        return myMemberValues.iLength;
    }
    qreal getJLength() const
    {
        return myMemberValues.jLength;
    }
    qreal getIRatio() const
    {
        return myMemberValues.iRatio;
    }
    qreal getJRatio() const
    {
        return myMemberValues.jRatio;
    }

    qreal getPlaneThickness() const
    {
        return myPlaneValue.thickness;    // 単位 mm
    }
    PLANEMODEL getPlaneModel() const
    {
        return myPlaneValue.modelType;
    }
    void setPlaneModel(PLANEMODEL model)
    {
        myPlaneValue.modelType = model;
    }
    PLANESUBMODEL getPlaneSubModel() const
    {
        return myPlaneValue.modelSubType;
    }
    void setPlaneSubModel(PLANESUBMODEL model)
    {
        myPlaneValue.modelSubType = model;
    }

    QList<qreal> getSeparatedLength() const;

    void calcMidLocalVector( int idx, VEC3D &u_v, VEC3D &v_v, VEC3D &w_v );

    //qreal getAttachedHeight() const;  // 所属の高さ m
    qreal getAttachedHeight(const UnifiedData&) const; // コピーコンストラクター用

private:

    MEMBERVALUES myMemberValues;
    int no;

    QList<qreal> lenList;        // 部材長さ（mm）
    MEMBERPARTITION myPartition; // 部材分割位置（mm）
    HLINESHAPE myLineShape;
    PLANEPROPERTY myPlaneValue;

    UnifiedDataType isOutput;
};


} // namespace post3dapp
#endif
