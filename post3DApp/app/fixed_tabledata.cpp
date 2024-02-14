#include "fixed_tabledata.h"

#include <QDateTime>

#include "analysisfloor_tablemodel.h"
#include "framesetting_tablemodel.h"
#include "framesetting_subtablemodel.h"
#include "material_tablemodel.h"
#include "section_base_tablemodel.h"
#include "section_beam_tablemodel.h"
#include "section_brace_tablemodel.h"
#include "section_col_tablemodel.h"
#include "section_damper_tablemodel.h"
#include "section_gir_tablemodel.h"
#include "section_isolator_tablemodel.h"
#include "section_opening_tablemodel.h"
#include "section_slab_tablemodel.h"
#include "section_wall_tablemodel.h"
#include "section_uwall_tablemodel.h"



namespace post3dapp{
FixedTableData::FixedTableData(bool _isDummy, const QString& _time)
    :FixedData(_isDummy, _time)
{

}

FixedTableData* FixedTableData::Create(){
    FixedTableData* unifiedCopyData = new FixedTableData(false, QDateTime::currentDateTime().toString());
    return unifiedCopyData;
}

void FixedTableData::setTableModel(CustomTableModel* model, DATATYPE data, STRUCTTYPE str){
    switch(data){
    case DATATYPE::TPAFLOOR:
        analysisFloorModel = new AnalysisFloorTableModel(*static_cast<AnalysisFloorTableModel*>(model));
        return;
    case DATATYPE::TPFLOOR:
        floorModel = new CustomTableModel(*model);
        return;
    case DATATYPE::TPFRAMEG:
        frameModel = new FrameSettingTableModel(*static_cast<FrameSettingTableModel*>(model));
        return;
    case DATATYPE::TPFRAMEP:
        frameSubModel = new FrameSettingSubTableModel(*static_cast<FrameSettingSubTableModel*>(model));
        return;
    case DATATYPE::TPMACONC:
    case DATATYPE::TPMASTEEL:
    case DATATYPE::TPMAREIN:
        materialModel = new MaterialTableModel(*static_cast<MaterialTableModel*>(model));
        return;
    case DATATYPE::TPCOLUMN:
        switch(str){
        case STRUCTTYPE::STP_RC:
            colRcModel = new SectionColumnTableModel(*static_cast<SectionColumnTableModel*>(model));
            return;
        case STRUCTTYPE::STP_S:
            colSModel = new SectionColumnTableModel(*static_cast<SectionColumnTableModel*>(model));
            return;
        case STRUCTTYPE::STP_SRC:
            colSrcModel = new SectionColumnTableModel(*static_cast<SectionColumnTableModel*>(model));
            return;
        case STRUCTTYPE::STP_CFT:
            colCftModel = new SectionColumnTableModel(*static_cast<SectionColumnTableModel*>(model));
            return;
        case STRUCTTYPE::NOSTRUCTTYPE:
            return;
        }
    case DATATYPE::TPGIRDER:
        switch(str){
        case STRUCTTYPE::STP_RC:
            girRcModel = new SectionGirderTableModel(*static_cast<SectionGirderTableModel*>(model));
            return;
        case STRUCTTYPE::STP_S:
            girSModel = new SectionGirderTableModel(*static_cast<SectionGirderTableModel*>(model));
            return;
        case STRUCTTYPE::STP_SRC:
            girSrcModel = new SectionGirderTableModel(*static_cast<SectionGirderTableModel*>(model));
            return;
        case STRUCTTYPE::STP_CFT:
        case STRUCTTYPE::NOSTRUCTTYPE:
            return;
        }
    case DATATYPE::TPBRACE:
        braceModel = new SectionBraceTableModel(*static_cast<SectionBraceTableModel*>(model));
        return;
    case DATATYPE::TPBEAM:
        switch(str){
        case STRUCTTYPE::STP_RC:
            beamRcModel = new SectionBeamTableModel(*static_cast<SectionBeamTableModel*>(model));
            return;
        case STRUCTTYPE::STP_S:
            beamSModel = new SectionBeamTableModel(*static_cast<SectionBeamTableModel*>(model));
            return;
        case STRUCTTYPE::STP_SRC:
            beamSrcModel = new SectionBeamTableModel(*static_cast<SectionBeamTableModel*>(model));
            return;
        case STRUCTTYPE::STP_CFT:
        case STRUCTTYPE::NOSTRUCTTYPE:
            return;
        }
    case DATATYPE::TPSLAB:
        slabModel = new SectionSlabTableModel(*static_cast<SectionSlabTableModel*>(model));
        return;
    case DATATYPE::TPWALL:
        switch(str){
        case STRUCTTYPE::STP_RC:
            regularWallModel = new SectionWallTableModel(*static_cast<SectionWallTableModel*>(model));
            return;
        case STRUCTTYPE::STP_S:
            steelWallModel = new SectionWallTableModel(*static_cast<SectionWallTableModel*>(model));
            return;
        default:
            return;
        }
    case DATATYPE::TPUWALL:
        underGroundWallModel = new SectionUWallTableModel(*static_cast<SectionUWallTableModel*>(model));
        return;
    case DATATYPE::TPBASE:
        switch (str) {
        case STRUCTTYPE::STP_S:
            baseSModel = new SectionBaseTableModel(*static_cast<SectionBaseTableModel*>(model));
            return;
        case STRUCTTYPE::STP_SRC:
            baseSrcModel = new SectionBaseTableModel(*static_cast<SectionBaseTableModel*>(model));
            return;
        default:
            return;
        }
    case DATATYPE::TPWALLOPEN:
        openingModel = new SectionOpeningTableModel(*static_cast<SectionOpeningTableModel*>(model));
        return;
    case DATATYPE::TPDAMPER:
        damperModel = new SectionDamperTableModel(*static_cast<SectionDamperTableModel*>(model));
        return;
    case DATATYPE::TPISO:
        isolatorModel = new SectionIsolatorTableModel(*static_cast<SectionIsolatorTableModel*>(model));
        return;
    case DATATYPE::TPJOINTLOAD:
        jointLoadModel = new CustomTableModel(*model);
        return;
    case DATATYPE::TPMEMBERLOAD:
        memberLoadModel = new CustomTableModel(*model);
        return;
    case DATATYPE::TPSLABLOAD:
        slabLoadModel = new CustomTableModel(*model);
        return;
    default:
        break;
    }
}

QStringList FixedTableData::getGirderRcList() const
{
    return girRcModel->getOutputTextList();
}
QStringList FixedTableData::getGirderSList() const
{
    return girSModel->getOutputTextList();
}
QStringList FixedTableData::getGirderSrcList() const
{
    return girSrcModel->getOutputTextList();
}
QStringList FixedTableData::getColumnRcList() const
{
    return colRcModel->getOutputTextList();
}
QStringList FixedTableData::getColumnSList() const
{
    return colSModel->getOutputTextList();
}
QStringList FixedTableData::getColumnSrcList() const
{
    return colSrcModel->getOutputTextList();
}
QStringList FixedTableData::getColumnCftList() const
{
    return colCftModel->getOutputTextList();
}
QStringList FixedTableData::getBeamRcList() const
{
    return beamRcModel->getOutputTextList();
}
QStringList FixedTableData::getBeamSList() const
{
    return beamSModel->getOutputTextList();
}
QStringList FixedTableData::getBeamSrcList() const
{
    return beamSrcModel->getOutputTextList();
}
QStringList FixedTableData::getBraceList() const
{
    return braceModel->getOutputTextList();
}
QStringList FixedTableData::getWallList() const
{
    return regularWallModel->getOutputTextList();
}
QStringList FixedTableData::getFoundationWallList() const
{
    return underGroundWallModel->getOutputTextList();
}
QStringList FixedTableData::getSteelWallList() const
{
    return steelWallModel->getOutputTextList();
}
QStringList FixedTableData::getSlabList() const
{
    return slabModel->getOutputTextList();
}

QStringList FixedTableData::getOpenList() const
{
    return openingModel->getOutputTextList();
}
QStringList FixedTableData::getBaseSList() const
{
    return baseSModel->getOutputTextList();
}
QStringList FixedTableData::getBaseSrcList() const
{
    return baseSrcModel->getOutputTextList();
}
QStringList FixedTableData::getDamperList() const
{
    return damperModel->getOutputTextList();
}
QStringList FixedTableData::getIsoList() const
{
    return isolatorModel->getOutputTextList();
}
QStringList FixedTableData::getJointLoadList() const
{
    return jointLoadModel->getOutputTextList();
}
QStringList FixedTableData::getMemberLoadList() const
{
    return memberLoadModel->getOutputTextList();
}
QStringList FixedTableData::getSlabLoadList() const
{
    return slabLoadModel->getOutputTextList();
}

} // namespace post3dapp
