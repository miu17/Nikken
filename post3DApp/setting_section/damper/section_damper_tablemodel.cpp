#include "section_damper_tablemodel.h"
#include "define_section_struct.h"

namespace post3dapp{
SectionDamperTableModel::SectionDamperTableModel(const SectionDataType &type, QObject *parent):
    SectionTableModel(type,parent)
{
}

SectionDamperTableModel::SectionDamperTableModel(const SectionDamperTableModel &model)
    :SectionTableModel(model){}


SectionValues SectionDamperTableModel::getMemberSection(int row) const
{
    return CreateDamperSection( getRowData(row), row );
}

QList<SectionValues> SectionDamperTableModel::getSectionList() const
{
    QList<SectionValues> sectionList;
    for ( int i = 0; i < allItems.rowCount(); i++ ) {
        if ( !allItems.isValidRow(i) ) continue;
        sectionList.append( CreateDamperSection( getRowData(i), i ) );
    }
    return sectionList;
}

DamperSection SectionDamperTableModel::CreateDamperSection(
        const QList<QVariant> &myValues,
        int currentrow) const
{
    DamperSection ds;
    QHash<int, QVariant> defaultValues = mySectionType.getLineDefaultValues(currentrow);

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();

    ds.sectionName = myValues.at(coltag["LABEL"]).toString();
    ds.memberType = mySectionType.getMemberType();
    ds.sectionType = mySectionType.getStructType();
    QString ptype = myValues.at(coltag["PRODUCTTYPE"]).toString().isEmpty()
                    ? defaultValues[coltag["PRODUCTTYPE"]].toString()
                    : myValues.at(coltag["PRODUCTTYPE"]).toString();
    ds.damper_param.productType = DamperType::stringToEnum(ptype);
    ds.damper_param.selfWeight = myValues.at(coltag["SELFWEIGHT"]).toString().isEmpty()
                              ? defaultValues[coltag["SELFWEIGHT"]].toDouble()
                              : myValues.at(coltag["SELFWEIGHT"]).toDouble();
    ds.damper_param.vecX  = myValues.at(coltag["VECTOR_X"]).toString().isEmpty()
                         ? defaultValues[coltag["VECTOR_X"]].toDouble()
                         : myValues.at(coltag["VECTOR_X"]).toDouble();
    ds.damper_param.vecY  = myValues.at(coltag["VECTOR_Y"]).toString().isEmpty()
                         ? defaultValues[coltag["VECTOR_Y"]].toDouble()
                         : myValues.at(coltag["VECTOR_Y"]).toDouble();
    ds.damper_param.vecZ  = myValues.at(coltag["VECTOR_Z"]).toString().isEmpty()
                         ? defaultValues[coltag["VECTOR_Z"]].toDouble()
                         : myValues.at(coltag["VECTOR_Z"]).toDouble();
    auto array = myValues.at(coltag["PARAMETERS"]).toString().split(',');
    QList<double> parameters;
    for(int i = 0; i < array.count(); i++){
        parameters.append(array.at(i).toDouble());
    }
    ds.damper_param.parameters = parameters;
    ds.damper_param.period  = myValues.at(coltag["NATURALPERIOD"]).toString().isEmpty()
                           ? defaultValues[coltag["NATURALPERIOD"]].toDouble()
                           : myValues.at(coltag["NATURALPERIOD"]).toDouble();
    ds.damper_param.maxDisp  = myValues.at(coltag["DISPLACEMENT"]).toString().isEmpty()
                            ? defaultValues[coltag["DISPLACEMENT"]].toDouble()
                            : myValues.at(coltag["DISPLACEMENT"]).toDouble();
    ds.damper_param.modelFlag  = myValues.at(coltag["MODEL_FLAG"]).toString().isEmpty()
                            ? defaultValues[coltag["MODEL_FLAG"]].toDouble()
                            : myValues.at(coltag["MODEL_FLAG"]).toDouble();
    ds.damper_param.rigidLength  = myValues.at(coltag["RIGID_LENGTH"]).toString().isEmpty()
                            ? defaultValues[coltag["RIGID_LENGTH"]].toDouble()
                            : myValues.at(coltag["RIGID_LENGTH"]).toDouble();
    ds.damper_param.damperDirection  = myValues.at(coltag["DAMPER_DIRECTION"]).toString().isEmpty()
                            ? defaultValues[coltag["DAMPER_DIRECTION"]].toDouble()
                            : myValues.at(coltag["DAMPER_DIRECTION"]).toDouble();
    ds.damper_param.tAnalysis  = myValues.at(coltag["TANALYSIS"]).toString().isEmpty()
                            ? defaultValues[coltag["TANALYSIS"]].toDouble()
                            : myValues.at(coltag["TANALYSIS"]).toDouble();
    ds.damper_param.factor  = myValues.at(coltag["FACTOR"]).toString().isEmpty()
                            ? defaultValues[coltag["FACTOR"]].toDouble()
                            : myValues.at(coltag["FACTOR"]).toDouble();
    ds.damper_param.frameRigidity  = myValues.at(coltag["FRAME_RIGIDITY"]).toString().isEmpty()
                            ? defaultValues[coltag["FRAME_RIGIDITY"]].toDouble()
                            : myValues.at(coltag["FRAME_RIGIDITY"]).toDouble();

    return ds;
}
} // namespace post3dapp
