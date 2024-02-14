#ifndef FIXEDTABLEDATA_H
#define FIXEDTABLEDATA_H

#include "define_unifieddata.h"
#include "fixed_data.h"

namespace post3dapp{
class CustomTableModel;
enum class DATATYPE;

class AnalysisFloorTableModel;
class FrameSettingTableModel;
class FrameSettingSubTableModel;
class MaterialTableModel;
class SectionBaseTableModel;
class SectionBeamTableModel;
class SectionBraceTableModel;
class SectionColumnTableModel;
class SectionDamperTableModel;
class SectionGirderTableModel;
class SectionIsolatorTableModel;
class SectionOpeningTableModel;
class SectionSlabTableModel;
class SectionWallTableModel;
class SectionUWallTableModel;

class FixedTableData : public FixedData
{
    Q_OBJECT

public:
    static FixedTableData* Create();
    void setTableModel(CustomTableModel*, DATATYPE, STRUCTTYPE type = STRUCTTYPE::NOSTRUCTTYPE);
private:
    FixedTableData(bool, const QString&);
    AnalysisFloorTableModel* analysisFloorModel;
    CustomTableModel* floorModel;
    FrameSettingTableModel* frameModel;
    FrameSettingSubTableModel* frameSubModel;
    CustomTableModel* jointLoadModel;
    CustomTableModel* memberLoadModel;
    CustomTableModel* slabLoadModel;
    MaterialTableModel* materialModel;
    SectionBaseTableModel* baseSModel;
    SectionBaseTableModel* baseSrcModel;
    SectionBeamTableModel* beamRcModel;
    SectionBeamTableModel* beamSModel;
    SectionBeamTableModel* beamSrcModel;
    SectionBraceTableModel* braceModel;
    SectionColumnTableModel* colRcModel;
    SectionColumnTableModel* colSModel;
    SectionColumnTableModel* colSrcModel;
    SectionColumnTableModel* colCftModel;
    SectionDamperTableModel* damperModel;
    SectionGirderTableModel* girRcModel;
    SectionGirderTableModel* girSModel;
    SectionGirderTableModel* girSrcModel;
    SectionIsolatorTableModel* isolatorModel;
    SectionOpeningTableModel* openingModel;
    SectionSlabTableModel* slabModel;
    SectionWallTableModel* regularWallModel;
    SectionWallTableModel* steelWallModel;
    SectionUWallTableModel* underGroundWallModel;

    // GUIを経由するか経由しないかでOutputTextをかき分ける
    QStringList getGirderRcList() const override;
    QStringList getGirderSList() const override;
    QStringList getGirderSrcList() const override;
    QStringList getColumnRcList() const override;
    QStringList getColumnSList() const override;
    QStringList getColumnSrcList() const override;
    QStringList getColumnCftList() const override;
    QStringList getBeamRcList() const override;
    QStringList getBeamSList() const override;
    QStringList getBeamSrcList() const override;
    QStringList getBraceList() const override;
    QStringList getSlabList() const override;
    QStringList getWallList() const override;
    QStringList getFoundationWallList() const override;
    QStringList getSteelWallList() const override;
    QStringList getOpenList() const override;
    QStringList getBaseSList() const override;
    QStringList getBaseSrcList() const override;
    QStringList getDamperList() const override;
    QStringList getIsoList() const override;
    QStringList getJointLoadList() const override;
    QStringList getMemberLoadList() const override;
    QStringList getSlabLoadList() const override;
};
} // namespace post3dapp

#endif // FIXEDTABLEDATA_H
