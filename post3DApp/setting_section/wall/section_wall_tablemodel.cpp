#include "section_wall_tablemodel.h"
#include "define_section_struct.h"

namespace post3dapp{
SectionWallTableModel::SectionWallTableModel(const SectionDataType &type, QObject *parent):
    SectionTableModel(type,parent)
{
}

SectionWallTableModel::SectionWallTableModel(const SectionWallTableModel &model)
    :SectionTableModel(model){}


SectionValues SectionWallTableModel::getMemberSection(int row) const
{
    return CreatePlaneMemberSection( getRowData(row), row );
}

QList<SectionValues> SectionWallTableModel::getSectionList() const
{
    QList<SectionValues> sectionList;
    for ( int i = 0; i < allItems.rowCount(); i++ ) {
        if ( !allItems.isValidRow(i) ) continue;
        sectionList.append( CreatePlaneMemberSection( getRowData(i), i ) );
    }
    return sectionList;
}

PlaneMemberSection SectionWallTableModel::CreatePlaneMemberSection(
        const QList<QVariant> &myValues,
        int currentrow) const
{
    PlaneMemberSection pm;
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();

    pm.sectionName = myValues.at(coltag["LABEL"]).toString();
    pm.memberType = mySectionType.getMemberType();
    pm.sectionType = mySectionType.getStructType();
    pm.plane_param.finishLoad = myValues.at(coltag["FINISHLOAD"]).toDouble();
    QHash<int, QVariant> defaultValues = mySectionType.getLineDefaultValues(currentrow);

    if (mySectionType.getStructType()==STRUCTTYPE::STP_RC) {

        pm.startFloorID = myValues.at(coltag["FLOORFROM"]).toString();
        pm.endFloorID = myValues.at(coltag["FLOORTO"]).toString();
        pm.plane_param.concMaterial = myValues.at(coltag["CONC"]).toString();
        pm.plane_param.concThickness = myValues.at(coltag["CONCTHICKNESS"]).toDouble();
        pm.plane_param.steelMaterial = myValues.at(coltag["STEEL"]).toString();
        pm.plane_param.steelThickness = myValues.at(coltag["STEELTHICKNESS"]).toDouble();
        //一般
        pm.plane_param.planeType = PlaneType::stringToEnum( myValues.at(coltag["WALLTYPE"]).toString() );
        if ( pm.plane_param.planeType == PlaneType::Type::UNDEFINED ) pm.plane_param.planeType = PlaneType::Type::WALL;
        pm.plane_param.isDoubleBar = ( myValues.at(coltag["BARARRANGEMENTTYPE"]).toString() != u8"シングル");

        //配筋
        pm.plane_param.vBar.phi = myValues.at(coltag["vBARPHI"]).toString().isEmpty()
                               ? defaultValues[coltag["vBARPHI"]].toString() : myValues.at(coltag["vBARPHI"]).toString();
        pm.plane_param.hBar.phi = myValues.at(coltag["hBARPHI"]).toString().isEmpty()
                               ? pm.plane_param.vBar.phi : myValues.at(coltag["hBARPHI"]).toString();
        pm.plane_param.vBar.pitch = myValues.at(coltag["vBARPITCH"]).toString().isEmpty()
                                 ? defaultValues[coltag["vBARPITCH"]].toDouble() : myValues.at(coltag["vBARPITCH"]).toDouble();
        pm.plane_param.hBar.pitch = myValues.at(coltag["hBARPITCH"]).toString().isEmpty()
                                 ? pm.plane_param.vBar.pitch : myValues.at(coltag["hBARPITCH"]).toDouble();

        //開口補強筋
        pm.plane_param.vOpeningBar_small.num = myValues.at(coltag["vSMALLOPENINGBARNUM"]).toString().isEmpty()
                                            ? defaultValues[coltag["vSMALLOPENINGBARNUM"]].toInt()
                                            : myValues.at(coltag["vSMALLOPENINGBARNUM"]).toInt();
        pm.plane_param.hOpeningBar_small.num = myValues.at(coltag["hSMALLOPENINGBARNUM"]).toString().isEmpty()
                                            ? defaultValues[coltag["hSMALLOPENINGBARNUM"]].toInt()
                                            : myValues.at(coltag["hSMALLOPENINGBARNUM"]).toInt();
        pm.plane_param.dOpeningBar_small.num = myValues.at(coltag["dSMALLOPENINGBARNUM"]).toString().isEmpty()
                                            ? defaultValues[coltag["dSMALLOPENINGBARNUM"]].toInt()
                                            : myValues.at(coltag["dSMALLOPENINGBARNUM"]).toInt();
        pm.plane_param.vOpeningBar_large.num = myValues.at(coltag["vLARGEOPENINGBARNUM"]).toString().isEmpty()
                                            ? defaultValues[coltag["vLARGEOPENINGBARNUM"]].toInt()
                                            : myValues.at(coltag["vLARGEOPENINGBARNUM"]).toInt();
        pm.plane_param.hOpeningBar_large.num = myValues.at(coltag["hLARGEOPENINGBARNUM"]).toString().isEmpty()
                                            ? defaultValues[coltag["hLARGEOPENINGBARNUM"]].toInt()
                                            : myValues.at(coltag["hLARGEOPENINGBARNUM"]).toInt();
        pm.plane_param.dOpeningBar_large.num = myValues.at(coltag["dLARGEOPENINGBARNUM"]).toString().isEmpty()
                                            ? defaultValues[coltag["dLARGEOPENINGBARNUM"]].toInt()
                                            : myValues.at(coltag["dLARGEOPENINGBARNUM"]).toInt();

        pm.plane_param.vOpeningBar_small.phi = myValues.at(coltag["vSMALLOPENINGBARPHI"]).toString().isEmpty()
                                            ? defaultValues[coltag["vSMALLOPENINGBARPHI"]].toString()
                                            : myValues.at(coltag["vSMALLOPENINGBARPHI"]).toString();
        pm.plane_param.hOpeningBar_small.phi = myValues.at(coltag["hSMALLOPENINGBARPHI"]).toString().isEmpty()
                                            ? defaultValues[coltag["hSMALLOPENINGBARPHI"]].toString()
                                            : myValues.at(coltag["hSMALLOPENINGBARPHI"]).toString();
        pm.plane_param.dOpeningBar_small.phi = myValues.at(coltag["dSMALLOPENINGBARPHI"]).toString().isEmpty()
                                            ? defaultValues[coltag["dSMALLOPENINGBARPHI"]].toString()
                                            : myValues.at(coltag["dSMALLOPENINGBARPHI"]).toString();
        pm.plane_param.vOpeningBar_large.phi = myValues.at(coltag["vLARGEOPENINGBARPHI"]).toString().isEmpty()
                                            ? defaultValues[coltag["vLARGEOPENINGBARPHI"]].toString()
                                            : myValues.at(coltag["vLARGEOPENINGBARPHI"]).toString();
        pm.plane_param.hOpeningBar_large.phi = myValues.at(coltag["hLARGEOPENINGBARPHI"]).toString().isEmpty()
                                            ? defaultValues[coltag["hLARGEOPENINGBARPHI"]].toString()
                                            : myValues.at(coltag["hLARGEOPENINGBARPHI"]).toString();
        pm.plane_param.dOpeningBar_large.phi = myValues.at(coltag["dLARGEOPENINGBARPHI"]).toString().isEmpty()
                                            ? defaultValues[coltag["dLARGEOPENINGBARPHI"]].toString()
                                            : myValues.at(coltag["dLARGEOPENINGBARPHI"]).toString();

    } else if (mySectionType.getStructType()==STRUCTTYPE::STP_S) {
        pm.startFloorID = myValues.at(coltag["FLOORFROM"]).toString();
        pm.endFloorID = myValues.at(coltag["FLOORTO"]).toString();
        pm.plane_param.planeType = PlaneType::stringToEnum( myValues.at(coltag["WALLTYPE"]).toString() );
        pm.plane_param.steelMaterial = myValues.at(coltag["STEEL"]).toString();
        pm.plane_param.steelThickness = myValues.at(coltag["STEELTHICKNESS"]).toDouble();

    }
    return pm;
}
} // namespace post3dapp
