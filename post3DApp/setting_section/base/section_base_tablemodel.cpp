#include "section_base_tablemodel.h"
#include "define_section_struct.h"


namespace post3dapp{
SectionBaseTableModel::SectionBaseTableModel(const SectionDataType &type, QObject *parent):
    SectionTableModel(type,parent)
{
    standardList = SectionDataType::standardSectionFileList(StandardSectionTable::listType::BASE);
}

SectionBaseTableModel::SectionBaseTableModel(const SectionBaseTableModel &model)
    : SectionTableModel(model){}


SectionValues SectionBaseTableModel::getMemberSection(int row) const
{
    return CreateBaseMemberSection( getRowData(row), row );
}

QList<SectionValues> SectionBaseTableModel::getSectionList() const
{
    QList<SectionValues> sectionList;
    for ( int i = 0; i < allItems.rowCount(); i++ ) {
        if ( !allItems.isValidRow(i) ) continue;
        sectionList.append( CreateBaseMemberSection( getRowData(i),i ) );
    }
    return sectionList;
}

bool SectionBaseTableModel::isBlockedItem(const QModelIndex &index) const
{
    int row = index.row();
    int col = index.column();

    //規格断面によるDisable
    if ( !shapetype_columns.isEmpty() ) {
        QHash<QString, int> coltag = mySectionType.getTaggedColumns();
        if ( shapetype_columns.contains(col - 1) ) {
            QModelIndex idx = index.model()->index(row, col - 1);
            if ( !standardList.contains(idx.data().toString()) ) return true;
        }

        int maxscol = 0;
        foreach (int scol, shapetype_columns) {
            if (maxscol < scol)maxscol = scol;
        }
        foreach (int scol, shapetype_columns) {
            QModelIndex idx = index.model()->index(row, scol);
            if (standardList.contains(idx.data().toString()) && col > maxscol + 1)return true;
        }
    }

    if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC){
        int baseType = mySectionType.getTaggedColumn("BASEPLATETYPE");
        BasePlateType::Type type =  BasePlateType::stringToEnum(index.model()->index(row, baseType).data().toString());
        if (type == BasePlateType::Type::RECTANGLE || type == BasePlateType::Type::CIRCLE){
            if (col == mySectionType.getTaggedColumn("SML")){
                return true;
            }
        }
    }


    return false;
}

BaseMemberSection SectionBaseTableModel::CreateBaseMemberSection(
        const QList<QVariant> &myValues,
        int currentrow) const
{
    BaseMemberSection bs;
    QHash<int, QVariant> defaultValues = mySectionType.getLineDefaultValues(currentrow);
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();

    bs.sectionName = myValues.at(coltag["LABEL"]).toString();
    bs.memberType = mySectionType.getMemberType();
    bs.sectionType = mySectionType.getStructType();

    if (mySectionType.getStructType() == STRUCTTYPE::STP_S) {

        QStringList sfiles = mySectionType.standardSectionFileList(StandardSectionTable::listType::BASE);
        bs.base_param.isStandardType = sfiles.contains( myValues.at(coltag["STANDARDTYPE"]).toString() );
        bs.base_param.productName = ( bs.base_param.isStandardType )
                                 ? myValues.at(coltag["PRODUCTNAME"]).toString() : QString() ;
        bs.base_param.basePlateType = BasePlateType::Type::RECTANGLE;
        bs.base_param.sizeB = myValues.at(coltag["B"]).toString().isEmpty()
                           ? defaultValues[coltag["B"]].toDouble() : myValues.at(coltag["B"]).toDouble();
        bs.base_param.sizeD = myValues.at(coltag["D"]).toString().isEmpty()
                           ? bs.base_param.sizeB : myValues.at(coltag["D"]).toDouble();
        bs.base_param.sizeT = myValues.at(coltag["T"]).toString().isEmpty()
                           ? defaultValues[coltag["T"]].toDouble() : myValues.at(coltag["T"]).toDouble();
        bs.base_param.SML = 0;
        bs.base_param.basePlateMaterial = myValues.at(coltag["BASEPLATEMATERIAL"]).toString();

        bs.base_param.boltType = BaseBoltType::Type::RECTANGLE;
        bs.base_param.bolt_ny1 = myValues.at(coltag["BOLTNY1"]).toString().isEmpty()
                              ? defaultValues[coltag["BOLTNY1"]].toInt()
                              : myValues.at(coltag["BOLTNY1"]).toInt();
        bs.base_param.bolt_ny2 = bs.base_param.bolt_ny1;
        bs.base_param.bolt_nz1 = myValues.at(coltag["BOLTNZ1"]).toString().isEmpty()
                              ? bs.base_param.bolt_ny1 : myValues.at(coltag["BOLTNZ1"]).toInt();
        bs.base_param.bolt_nz2 = bs.base_param.bolt_nz1;
        bs.base_param.boltPhi = myValues.at(coltag["BOLTPHI"]).toString().isEmpty()
                             ? defaultValues[coltag["BOLTPHI"]].toInt()
                             : myValues.at(coltag["BOLTPHI"]).toInt();
        bs.base_param.screwPhi = 0;
        bs.base_param.boltHolePhi = bs.base_param.boltPhi; // 描画用に代入
        bs.base_param.edgehy1 = myValues.at(coltag["EDGEDISTANCEY1"]).toString().isEmpty()
                             ? defaultValues[coltag["EDGEDISTANCEY1"]].toDouble()
                             : myValues.at(coltag["EDGEDISTANCEY1"]).toDouble();
        bs.base_param.edgehy2 = bs.base_param.edgehy1;
        bs.base_param.edgehz1 = myValues.at(coltag["EDGEDISTANCEZ1"]).toString().isEmpty()
                             ? defaultValues[coltag["EDGEDISTANCEZ1"]].toDouble()
                             : myValues.at(coltag["EDGEDISTANCEZ1"]).toDouble();
        bs.base_param.edgehz2 = bs.base_param.edgehz1;
        bs.base_param.boltMaterial = myValues.at(coltag["BOLTMATERIAL"]).toString();
        bs.base_param.anchorLength = myValues.at(coltag["BOLTLENGTH"]).toString().isEmpty()
                                  ? defaultValues[coltag["BOLTLENGTH"]].toDouble()
                                  : myValues.at(coltag["BOLTLENGTH"]).toDouble();
        bs.base_param.hasElongationAbility = true;
        bs.base_param.anchorArea = 0;
        bs.base_param.sizeBc = 0;
        bs.base_param.sizeDc = 0;
        bs.base_param.sizeH = 0;
        bs.base_param.projectedarea = 0;

    } else if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC) {

        bs.base_param.isStandardType = false;
        bs.base_param.B0   = myValues.at(coltag["B0"]).toString().isEmpty()
                          ? defaultValues[coltag["B0"]].toDouble()
                          : myValues.at(coltag["B0"]).toDouble();
        bs.base_param.D0   = myValues.at(coltag["D0"]).toString().isEmpty()
                          ? bs.base_param.B0 : myValues.at(coltag["D0"]).toDouble();
        bs.base_param.concMaterial = myValues.at(coltag["CONC"]).toString();

        bs.base_param.productName = QString();
        QString str = myValues.at(coltag["BASEPLATETYPE"]).toString().isEmpty()
                      ? defaultValues[coltag["BASEPLATETYPE"]].toString()
                      : myValues.at(coltag["BASEPLATETYPE"]).toString();
        bs.base_param.basePlateType  = BasePlateType::stringToEnum(str);
        bs.base_param.sizeB     = myValues.at(coltag["B"]).toString().isEmpty()
                               ? defaultValues[coltag["B"]].toDouble()
                               : myValues.at(coltag["B"]).toDouble();
        bs.base_param.sizeD     = myValues.at(coltag["D"]).toString().isEmpty()
                               ? bs.base_param.sizeB : myValues.at(coltag["D"]).toDouble();
        bs.base_param.sizeT     = myValues.at(coltag["T"]).toString().isEmpty()
                               ? defaultValues[coltag["T"]].toDouble()
                               : myValues.at(coltag["T"]).toDouble();
        bs.base_param.SML     = myValues.at(coltag["SML"]).toString().isEmpty()
                             ? defaultValues[coltag["SML"]].toDouble()
                             : myValues.at(coltag["SML"]).toDouble();
        bs.base_param.edgehy1   = myValues.at(coltag["EDGEDISTANCEY1"]).toString().isEmpty()
                               ? defaultValues[coltag["EDGEDISTANCEY1"]].toDouble()
                               : myValues.at(coltag["EDGEDISTANCEY1"]).toDouble();
        bs.base_param.edgehy2   = myValues.at(coltag["EDGEDISTANCEY2"]).toString().isEmpty()
                               ? bs.base_param.edgehy1
                               : myValues.at(coltag["EDGEDISTANCEY2"]).toDouble();
        bs.base_param.edgehz1   = myValues.at(coltag["EDGEDISTANCEZ1"]).toString().isEmpty()
                               ? defaultValues[coltag["EDGEDISTANCEZ1"]].toDouble()
                               : myValues.at(coltag["EDGEDISTANCEZ1"]).toDouble();
        bs.base_param.edgehz2   = myValues.at(coltag["EDGEDISTANCEZ2"]).toString().isEmpty()
                               ? bs.base_param.edgehz1
                               : myValues.at(coltag["EDGEDISTANCEZ2"]).toDouble();
        str = myValues.at(coltag["BOLTARRANGEMENTTYPE"]).toString().isEmpty() ?
                      defaultValues[coltag["BOLTARRANGEMENTTYPE"]].toString() : myValues.at(
                          coltag["BOLTARRANGEMENTTYPE"]).toString();
        bs.base_param.boltType  = BaseBoltType::stringToEnum(str);
        bs.base_param.bolt_ny1   = myValues.at(coltag["BOLTNY1"]).toString().isEmpty()
                                ? defaultValues[coltag["BOLTNY1"]].toInt()
                                : myValues.at(coltag["BOLTNY1"]).toInt();
        bs.base_param.bolt_ny2   = myValues.at(coltag["BOLTNY2"]).toString().isEmpty()
                                ? bs.base_param.bolt_ny1 : myValues.at(coltag["BOLTNY2"]).toInt();
        bs.base_param.bolt_nz1   = myValues.at(coltag["BOLTNZ1"]).toString().isEmpty()
                                ? defaultValues[coltag["BOLTNZ1"]].toInt()
                                : myValues.at(coltag["BOLTNZ1"]).toInt();
        bs.base_param.bolt_nz2   = myValues.at(coltag["BOLTNZ2"]).toString().isEmpty()
                                ? bs.base_param.bolt_nz1 : myValues.at(coltag["BOLTNZ2"]).toInt();
        bs.base_param.boltPhi   = myValues.at(coltag["BOLTPHI"]).toString().isEmpty()
                               ? defaultValues[coltag["BOLTPHI"]].toInt()
                               : myValues.at(coltag["BOLTPHI"]).toInt();
        bs.base_param.boltHolePhi   = bs.base_param.boltPhi; // 描画用に代入
        bs.base_param.boltMaterial = myValues.at(coltag["BOLTMATERIAL"]).toString();

    }
    return bs;
}
} // namespace post3dapp
