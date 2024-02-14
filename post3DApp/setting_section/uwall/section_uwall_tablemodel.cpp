#include "section_uwall_tablemodel.h"
#include "define_section_struct.h"

namespace post3dapp{
SectionUWallTableModel::SectionUWallTableModel(const SectionDataType &type, QObject *parent):
    SectionTableModel(type,parent)
{
}

SectionUWallTableModel::SectionUWallTableModel(const SectionUWallTableModel &model)
    :SectionTableModel(model){}


SectionValues SectionUWallTableModel::getMemberSection(int row) const
{
    return CreatePlaneMemberSection( getRowData(row), row );
}

QList<SectionValues> SectionUWallTableModel::getSectionList() const
{
    QList<SectionValues> sectionList;
    for ( int i = 0; i < allItems.rowCount(); i++ ) {
        if ( !allItems.isValidRow(i) ) continue;
        sectionList.append( CreatePlaneMemberSection( getRowData(i), i ) );
    }
    return sectionList;
}

PlaneMemberSection SectionUWallTableModel::CreatePlaneMemberSection(
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

if (mySectionType.getMemberType() == DATATYPE::TPUWALL) {

        pm.plane_param.concMaterial = myValues.at(coltag["CONC"]).toString();
        pm.plane_param.concThickness = myValues.at(coltag["CONCTHICKNESS"]).toDouble();
        pm.plane_param.steelMaterial = myValues.at(coltag["STEEL"]).toString();
        pm.plane_param.steelThickness = myValues.at(coltag["STEELTHICKNESS"]).toDouble();

        //一般
        pm.plane_param.planeType = PlaneType::Type::WALL;
        pm.plane_param.isDoubleBar = true;

        //配筋

        QString def_phi = myValues.at(coltag["STANDARDBARPHI"]).toString().isEmpty()
                          ? defaultValues[coltag["STANDARDBARPHI"]].toString()
                          : myValues.at(coltag["STANDARDBARPHI"]).toString();
        qreal def_pitch = myValues.at(coltag["STANDARDBARPITCH"]).toString().isEmpty()
                          ? defaultValues[coltag["STANDARDBARPITCH"]].toDouble()
                          : myValues.at(coltag["STANDARDBARPITCH"]).toDouble();

        pm.plane_param.vBar_innerTop.phi    = myValues.at(coltag["vINNERTOP_BARPHI"]).toString().isEmpty()
                                           ? def_phi : myValues.at(coltag["vINNERTOP_BARPHI"]).toString();
        pm.plane_param.vBar_innerMiddle.phi = myValues.at(coltag["vINNERMIDDLE_BARPHI"]).toString().isEmpty()
                                           ? def_phi : myValues.at(coltag["vINNERMIDDLE_BARPHI"]).toString();
        pm.plane_param.vBar_innerBottom.phi = myValues.at(coltag["vINNERBOTTOM_BARPHI"]).toString().isEmpty()
                                           ? def_phi : myValues.at(coltag["vINNERBOTTOM_BARPHI"]).toString();
        pm.plane_param.vBar_outerTop.phi    = myValues.at(coltag["vOUTERTOP_BARPHI"]).toString().isEmpty()
                                           ? def_phi : myValues.at(coltag["vOUTERTOP_BARPHI"]).toString();
        pm.plane_param.vBar_outerMiddle.phi = myValues.at(coltag["vOUTERMIDDLE_BARPHI"]).toString().isEmpty()
                                           ? def_phi : myValues.at(coltag["vOUTERMIDDLE_BARPHI"]).toString();
        pm.plane_param.vBar_outerBottom.phi = myValues.at(coltag["vOUTERBOTTOM_BARPHI"]).toString().isEmpty()
                                           ? def_phi : myValues.at(coltag["vOUTERBOTTOM_BARPHI"]).toString();
        pm.plane_param.hBar_innerEdge.phi   = myValues.at(coltag["hINNEREDGE_BARPHI"]).toString().isEmpty()
                                           ? def_phi : myValues.at(coltag["hINNEREDGE_BARPHI"]).toString();
        pm.plane_param.hBar_innerCenter.phi = myValues.at(coltag["hINNERCENTER_BARPHI"]).toString().isEmpty()
                                           ? def_phi : myValues.at(coltag["hINNERCENTER_BARPHI"]).toString();
        pm.plane_param.hBar_outerEdge.phi   = myValues.at(coltag["hOUTEREDGE_BARPHI"]).toString().isEmpty()
                                           ? def_phi : myValues.at(coltag["hOUTEREDGE_BARPHI"]).toString();
        pm.plane_param.hBar_outerCenter.phi = myValues.at(coltag["hOUTERCENTER_BARPHI"]).toString().isEmpty()
                                           ? def_phi : myValues.at(coltag["hOUTERCENTER_BARPHI"]).toString();
        pm.plane_param.vBar_innerTop.pitch   = myValues.at(coltag["vINNERTOP_BARPITCH"]).toString().isEmpty()
                                            ? def_pitch : myValues.at(coltag["vINNERTOP_BARPITCH"]).toDouble();
        pm.plane_param.vBar_innerMiddle.pitch = myValues.at(
                                                 coltag["vINNERMIDDLE_BARPITCH"]).toString().isEmpty()
                                             ? def_pitch : myValues.at(coltag["vINNERMIDDLE_BARPITCH"]).toDouble();
        pm.plane_param.vBar_innerBottom.pitch = myValues.at(
                                                 coltag["vINNERBOTTOM_BARPITCH"]).toString().isEmpty()
                                             ? def_pitch : myValues.at(coltag["vINNERBOTTOM_BARPITCH"]).toDouble();
        pm.plane_param.vBar_outerTop.pitch   = myValues.at(coltag["vOUTERTOP_BARPITCH"]).toString().isEmpty()
                                            ? def_pitch : myValues.at(coltag["vOUTERTOP_BARPITCH"]).toDouble();
        pm.plane_param.vBar_outerMiddle.pitch = myValues.at(
                                                 coltag["vOUTERMIDDLE_BARPITCH"]).toString().isEmpty()
                                             ? def_pitch : myValues.at(coltag["vOUTERMIDDLE_BARPITCH"]).toDouble();
        pm.plane_param.vBar_outerBottom.pitch = myValues.at(
                                                 coltag["vOUTERBOTTOM_BARPITCH"]).toString().isEmpty()
                                             ? def_pitch : myValues.at(coltag["vOUTERBOTTOM_BARPITCH"]).toDouble();
        pm.plane_param.hBar_innerEdge.pitch  = myValues.at(coltag["hINNEREDGE_BARPITCH"]).toString().isEmpty()
                                            ? def_pitch : myValues.at(coltag["hINNEREDGE_BARPITCH"]).toDouble();
        pm.plane_param.hBar_innerCenter.pitch = myValues.at(
                                                 coltag["hINNERCENTER_BARPITCH"]).toString().isEmpty()
                                             ? def_pitch : myValues.at(coltag["hINNERCENTER_BARPITCH"]).toDouble();
        pm.plane_param.hBar_outerEdge.pitch  = myValues.at(coltag["hOUTEREDGE_BARPITCH"]).toString().isEmpty()
                                            ? def_pitch : myValues.at(coltag["hOUTEREDGE_BARPITCH"]).toDouble();
        pm.plane_param.hBar_outerCenter.pitch = myValues.at(
                                                 coltag["hOUTERCENTER_BARPITCH"]).toString().isEmpty()
                                             ? def_pitch : myValues.at(coltag["hOUTERCENTER_BARPITCH"]).toDouble();

    }
    return pm;
}
} // namespace post3dapp
