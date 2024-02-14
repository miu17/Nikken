#include "section_slab_tablemodel.h"
#include "define_section_struct.h"
#include "section_tableview.h"

namespace post3dapp{
SectionSlabTableModel::SectionSlabTableModel(const SectionDataType &type, QObject *parent):
    SectionTableModel(type,parent)
{
}

SectionSlabTableModel::SectionSlabTableModel(const SectionSlabTableModel &model)
    :SectionTableModel(model){}


SectionValues SectionSlabTableModel::getMemberSection(int row) const
{
    return CreatePlaneMemberSection( getRowData(row), row );
}

QList<SectionValues> SectionSlabTableModel::getSectionList() const
{
    QList<SectionValues> sectionList;
    for ( int i = 0; i < allItems.rowCount(); i++ ) {
        if ( !allItems.isValidRow(i) ) continue;
        sectionList.append( CreatePlaneMemberSection( getRowData(i), i ) );
    }
    return sectionList;
}

PlaneMemberSection SectionSlabTableModel::CreatePlaneMemberSection(
        const QList<QVariant> &myValues,
        int currentrow) const
{
    PlaneMemberSection pm;
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();

    pm.sectionName = myValues.at(coltag["LABEL"]).toString();
    pm.memberType = mySectionType.getMemberType();
    pm.sectionType = mySectionType.getStructType();

    pm.plane_param.finishLoad = myValues.at(coltag["FINISHLOAD"]).toDouble();
    pm.plane_param.concMaterial = myValues.at(coltag["CONC"]).toString();
    pm.plane_param.concThickness = myValues.at(coltag["CONCTHICKNESS"]).toDouble();
    pm.plane_param.steelMaterial = myValues.at(coltag["STEEL"]).toString();
    pm.plane_param.steelThickness = myValues.at(coltag["STEELTHICKNESS"]).toDouble();

    QHash<int, QVariant> defaultValues = mySectionType.getLineDefaultValues(currentrow);

    //一般
    pm.plane_param.coverU = myValues.at(coltag["COVERU"]).toString().isEmpty()
                         ? defaultValues[coltag["COVERU"]].toDouble()
                         : myValues.at(coltag["COVERU"]).toDouble();
    pm.plane_param.coverD = myValues.at(coltag["COVERB"]).toString().isEmpty()
                         ? defaultValues[coltag["COVERB"]].toDouble()
                         : myValues.at(coltag["COVERB"]).toDouble();
    qreal v_dir = myValues.at(coltag["LISTLOADDIRECTION"]).toString().isEmpty()
                  ? defaultValues[coltag["LISTLOADDIRECTION"]].toDouble()
                  : myValues.at(coltag["LISTLOADDIRECTION"]).toDouble();
    pm.plane_param.loadDir = ( v_dir < 0.0 - 1.0e-5 || v_dir > 360.0 + 1.0e-5 ) ? -1.0 : v_dir;

    pm.plane_param.planeType = PlaneType::stringToEnum( myValues.at(coltag["SLABTYPE"]).toString() );
    if ( pm.plane_param.planeType == PlaneType::Type::UNDEFINED ) pm.plane_param.planeType = PlaneType::Type::NONSTRUCT;

    //配筋

    QString def_phi = myValues.at(coltag["STANDARDBARPHI"]).toString().isEmpty()
                      ? defaultValues[coltag["STANDARDBARPHI"]].toString()
                      : myValues.at(coltag["STANDARDBARPHI"]).toString();
    qreal def_pitch = myValues.at(coltag["STANDARDBARPITCH"]).toString().isEmpty()
                      ? defaultValues[coltag["STANDARDBARPITCH"]].toDouble()
                      : myValues.at(coltag["STANDARDBARPITCH"]).toDouble();

    pm.plane_param.sBar_upperEdgeA.phi   = myValues.at(coltag["sUPPEREDGE_A_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["sUPPEREDGE_A_BARPHI"]).toString();
    pm.plane_param.sBar_upperEdgeA.pitch  = myValues.at(
                                             coltag["sUPPEREDGE_A_BARPITCH"]).toString().isEmpty()
                                         ? def_pitch : myValues.at(coltag["sUPPEREDGE_A_BARPITCH"  ]).toDouble();
    pm.plane_param.sBar_lowerEdgeA.phi   = myValues.at(coltag["sLOWEREDGE_A_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["sLOWEREDGE_A_BARPHI"]).toString();
    pm.plane_param.sBar_lowerEdgeA.pitch  = myValues.at(
                                             coltag["sLOWEREDGE_A_BARPITCH"]).toString().isEmpty()
                                         ? def_pitch : myValues.at(coltag["sLOWEREDGE_A_BARPITCH"  ]).toDouble();
    pm.plane_param.sBar_upperCenterA.phi = myValues.at(
                                            coltag["sUPPERCENTER_A_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["sUPPERCENTER_A_BARPHI"]).toString();
    pm.plane_param.sBar_upperCenterA.pitch = myValues.at(
                                              coltag["sUPPERCENTER_A_BARPITCH"]).toString().isEmpty()
                                          ? def_pitch : myValues.at(coltag["sUPPERCENTER_A_BARPITCH"]).toDouble();
    pm.plane_param.sBar_lowerCenterA.phi = myValues.at(
                                            coltag["sLOWERCENTER_A_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["sLOWERCENTER_A_BARPHI"]).toString();
    pm.plane_param.sBar_lowerCenterA.pitch = myValues.at(
                                              coltag["sLOWERCENTER_A_BARPITCH"]).toString().isEmpty()
                                          ? def_pitch : myValues.at(coltag["sLOWERCENTER_A_BARPITCH"]).toDouble();
    pm.plane_param.sBar_upperEdgeCenterB.phi   = myValues.at(coltag["sUPPER_B_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["sUPPER_B_BARPHI"]).toString();
    pm.plane_param.sBar_upperEdgeCenterB.pitch  = myValues.at(coltag["sUPPER_B_BARPITCH"]).toString().isEmpty()
                                         ? def_pitch : myValues.at(coltag["sUPPER_B_BARPITCH"      ]).toDouble();
    pm.plane_param.sBar_lowerEdgeCenterB.phi   = myValues.at(coltag["sLOWER_B_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["sLOWER_B_BARPHI"]).toString();
    pm.plane_param.sBar_lowerEdgeCenterB.pitch  = myValues.at(coltag["sLOWER_B_BARPITCH"]).toString().isEmpty()
                                         ? def_pitch : myValues.at(coltag["sLOWER_B_BARPITCH"      ]).toDouble();
    pm.plane_param.lBar_upperEdgeA.phi   = myValues.at(coltag["lUPPEREDGE_A_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["lUPPEREDGE_A_BARPHI"]).toString();
    pm.plane_param.lBar_upperEdgeA.pitch  = myValues.at(
                                             coltag["lUPPEREDGE_A_BARPITCH"]).toString().isEmpty()
                                         ? def_pitch : myValues.at(coltag["lUPPEREDGE_A_BARPITCH"  ]).toDouble();
    pm.plane_param.lBar_lowerEdgeA.phi   = myValues.at(coltag["lLOWEREDGE_A_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["lLOWEREDGE_A_BARPHI"]).toString();
    pm.plane_param.lBar_lowerEdgeA.pitch  = myValues.at(
                                             coltag["lLOWEREDGE_A_BARPITCH"]).toString().isEmpty()
                                         ? def_pitch : myValues.at(coltag["lLOWEREDGE_A_BARPITCH"  ]).toDouble();
    pm.plane_param.lBar_upperCenterA.phi = myValues.at(
                                            coltag["lUPPERCENTER_A_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["lUPPERCENTER_A_BARPHI"]).toString();
    pm.plane_param.lBar_upperCenterA.pitch = myValues.at(
                                              coltag["lUPPERCENTER_A_BARPITCH"]).toString().isEmpty()
                                          ? def_pitch : myValues.at(coltag["lUPPERCENTER_A_BARPITCH"]).toDouble();
    pm.plane_param.lBar_lowerCenterA.phi = myValues.at(
                                            coltag["lLOWERCENTER_A_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["lLOWERCENTER_A_BARPHI"]).toString();
    pm.plane_param.lBar_lowerCenterA.pitch = myValues.at(
                                              coltag["lLOWERCENTER_A_BARPITCH"]).toString().isEmpty()
                                          ? def_pitch : myValues.at(coltag["lLOWERCENTER_A_BARPITCH"]).toDouble();
    pm.plane_param.lBar_upperEdgeCenterB.phi   = myValues.at(coltag["lUPPER_B_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["lUPPER_B_BARPHI"]).toString();
    pm.plane_param.lBar_upperEdgeCenterB.pitch  = myValues.at(coltag["lUPPER_B_BARPITCH"]).toString().isEmpty()
                                         ? def_pitch : myValues.at(coltag["lUPPER_B_BARPITCH"      ]).toDouble();
    pm.plane_param.lBar_lowerEdgeCenterB.phi   = myValues.at(coltag["lLOWER_B_BARPHI"]).toString().isEmpty()
                                        ? def_phi : myValues.at(coltag["lLOWER_B_BARPHI"]).toString();
    pm.plane_param.lBar_lowerEdgeCenterB.pitch  = myValues.at(coltag["lLOWER_B_BARPITCH"]).toString().isEmpty()
                                         ? def_pitch : myValues.at(coltag["lLOWER_B_BARPITCH"      ]).toDouble();
    return pm;
}
} // namespace post3dapp
