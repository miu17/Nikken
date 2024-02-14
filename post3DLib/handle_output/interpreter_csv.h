#ifndef INTERPRETERCSV_H
#define INTERPRETERCSV_H

#include "define_material_struct.h"
#include "define_section_struct.h"
#include "define_unifieddata.h"
namespace post3dapp{
class JointData;
class MemberData;
class InterpreterCsv
{
public:
    InterpreterCsv();
    void Register() const;

    // ManageFloorData
    // ManageFloorで二つ読み込む
    QList<FLOORVALUES> sortedFloors; // 下の階から順番
    QUuid baseFloor;
    QUuid roofFloor;
    QList<FLOORVALUES> AnalysisFloors;
    // ManageFrameData
    QList<FRAMEGROUPVALUE> framegroups;
    QList<FRAMEPOINTSVALUE> frames;
    // ManageMaterialData
    QList<MATERIALVALUES> materials;
    // ManageSectionData
    QList<SectionValues> rcColumnList;
    QList<SectionValues> sColumnList;
    QList<SectionValues> srcColumnList;
    QList<SectionValues> cftColumnList;
    QList<SectionValues> rcGirderList;
    QList<SectionValues> sGirderList;
    QList<SectionValues> srcGirderList;
    QList<SectionValues> sBraceList;
    QList<SectionValues> rcBeamList;
    QList<SectionValues> sBeamList;
    QList<SectionValues> srcBeamList;
    QList<SectionValues> rcSlabList;
    QList<SectionValues> rcWallList;
    QList<SectionValues> sWallList;
    QList<SectionValues> rcUwallList;
    QList<SectionValues> sBaseList;
    QList<SectionValues> srcBaseList;
    QList<SectionValues> wallOpenList;
    QList<SectionValues> damperList;
    QList<SectionValues> isolatorList;

    QList<JOINTLOADVALUES> jointLoadList;
    QList<MEMBERLOADVALUES> memberLoadList;
    QList<SLABLOADVALUES> slabLoadList;

    QString gene;
    QString analysisCondition;

    void makeDefinedData() const;

    QUuid getOrCreateUuid(QString);
    QHash<QUuid, QString> uuidTable;
    FRAMEPOINTSVALUE CalculateFramePointsValue(const FRAMEGROUPVALUE&, double&, const QString&, bool, bool&, const QString&, const QUuid& = nullptr) const;
private:

};

} // namespace post3dapp
#endif // INTERPRETERCSV_H
