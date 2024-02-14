#ifndef READ_FROM_BLD3D_H
#define READ_FROM_BLD3D_H
#include <QList>
#include "define_section_struct.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class JointData;
class InterpreterCsv;
class MemberData;
struct MATERIALVALUES;

class POST3D_CLASS_DLLSPEC readFromBld3d
{

public:

    readFromBld3d(){}
    void readCsvData(QFile*);
    void divideMembersByJoints() const;
    void deleteIndependentJoints() const;

private:
    void readCsv2(QFile*);
    bool isOnLinePoint(JointData* jd,MemberData* md) const;
    QList<MemberData *> searchContainMembers(JointData* jd) const;

    QList<QStringList> strctLines;
    QList<QStringList> floorLines;
    QList<QStringList> mtralLines;
    QList<QStringList> gridLines;

    QList<QStringList> jointLines;
    QList<QStringList> memberLines;
    QList<QStringList> plateLines;
    QList<QStringList> openLines;

    QHash<int,QStringList> colvalue1;
    QHash<int,QStringList> colvalue2;
    QHash<int,QStringList> colvalue3;
    QHash<int,QStringList> colvalue4;
    QHash<int,QStringList> colvalue5;

    QHash<int,QStringList> girvalue1;
    QHash<int,QStringList> girvalue2;
    QHash<int,QStringList> girvalue3;
    QHash<int,QStringList> girvalue4;
    QHash<int,QStringList> girvalue5;
    QHash<int,QStringList> girvalue6;
    QHash<int,QStringList> girvalue7;

    QHash<int,QStringList> beamvalue1;
    QHash<int,QStringList> beamvalue2;
    QHash<int,QStringList> beamvalue3;
    QHash<int,QStringList> beamvalue4;
    QHash<int,QStringList> beamvalue5;
    QHash<int,QStringList> beamvalue6;
    QHash<int,QStringList> beamvalue7;

    QHash<int,QStringList> bracevalue1;
    QHash<int,QStringList> bracevalue2;

    QHash<int,QStringList> slabvalue1;
    QHash<int,QStringList> slabvalue2;
    QHash<int,QStringList> slabvalue3;

    QHash<int,QStringList> wallvalue1;
    QHash<int,QStringList> wallvalue2;

    QHash<int,QStringList> uwallvalue1;
    QHash<int,QStringList> uwallvalue2;
    QHash<int,QStringList> uwallvalue3;

    QHash<int,QStringList> wolistvalue;
    QHash<int,QString> wopenNoToName;

    QHash<int, QStringList> mloadvalue;
    QHash<int, QStringList> jloadvalue;

    QHash<int, QStringList> plistvalue1;
    QHash<int, QStringList> plistvalue2;
    QHash<int, QStringList> plistvalue3;
    QHash<int, QStringList> plistvalue4;
    QHash<int, QStringList> plistvalue5;

    QHash<int, QStringList> swallvalue1;
    QHash<int, QStringList> swallvalue2;

    QHash<int, QStringList> fllistvalue;
    QHash<int, QStringList> lllistvalue;
    QHash<int, QStringList> mllistvalue;
    QHash<int, QStringList> jllistvalue;


    QHash<int, QString> sectionNameHash;
    QHash<int, FLOORVALUES> floorHash;

    QSharedPointer<InterpreterCsv> interpretCsv();
    QHash<int, FLOORVALUES> interpretFloors(QSharedPointer<InterpreterCsv>) const;
    QList<FRAMEGROUPVALUE> interpretFrames() const;
    QList<MATERIALVALUES> interpretMaterials() const;
    QHash<int, FRAMEPOINTSVALUE> setFramePointList(QSharedPointer<InterpreterCsv>) const;
    void interpretColumnSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretGirderSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretBraceSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretBeamSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretPostSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretSlabSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretUwallSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretWallSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretSteelWallSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretWallOpenSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretJointLoadSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretMemberLoadSectionValues(QSharedPointer<InterpreterCsv>);
    void interpretSlabLoadSectionValues(QSharedPointer<InterpreterCsv>);
    QHash<int, int> interpretJoints();
    void interpretMemberCsv();
    void interpretPlateCsv(QSharedPointer<InterpreterCsv>);
    void interpretWallOpen(QSharedPointer<InterpreterCsv>);

    LinePartsParameter SetColumnRcParameter(const QStringList&) const;
    LinePartsParameter SetColumnSteelParameter(const QStringList&, LinePartsParameter) const;
    LinePartsParameter SetGirderRcParameter(const QStringList&) const;
    LinePartsParameter SetGirderSteelParameter(const QStringList&, LinePartsParameter) const;
    QPair<QUuid, QUuid> getFloorUuid(const QHash<int, FLOORVALUES>&, const QString&, const QString&) const;

    //QHash<int, QList<int>> memberLoadJointHash;
    QHash<QList<int>, int> tempDeadLoadJointHash; // b3dとp3dの形式と異なるので仮保存
    QHash<QList<int>, int> tempLiveLoadJointHash; // b3dとp3dの形式と異なるので仮保存

    QHash<int, int> mergedJointHash; // 同じ座標は統合する
    QHash<int, FRAMEPOINTSVALUE> frameHash;
    QHash<int, QSet<int>> openPlateHash;

    // makeunifiedDataでセットする
    QHash<int, GlobalPoint> globalPointHash;
    // ManageMemberData
    // uuidTableとMemberData
    QHash<int, QList<int>> columnJointHash;
    QHash<int, MEMBERVALUES> columnHash;
    QHash<int, QList<int>> girderJointHash;
    QHash<int, MEMBERVALUES> girderHash;
    QHash<int, QList<int>> braceJointHash;
    QHash<int, MEMBERVALUES> braceHash;
    QHash<int, QList<int>> beamJointHash;
    QHash<int, MEMBERVALUES> beamHash;
    QHash<int, QList<int>> slabJointHash;
    QHash<int, MEMBERVALUES> slabHash;
    QHash<int, QList<int>> wallJointHash;
    QHash<int, MEMBERVALUES> wallHash;
    QHash<int, QList<int>> uwallJointHash;
    QHash<int, MEMBERVALUES> uwallHash;

    QHash<int, QList<int>> slabLoadJointHash; // deadLoadとliveLoadの組み合わせ
    QHash<int, MEMBERVALUES> slabLoadHash;

    void makeUnifiedData();


};
} // namespace post3dapp
#endif // READ_FROM_BLD3D_H
