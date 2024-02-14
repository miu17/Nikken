#include "section_opening_tablemodel.h"
#include "define_section_struct.h"


namespace post3dapp{
SectionOpeningTableModel::SectionOpeningTableModel(const SectionDataType &type, QObject *parent):
    SectionTableModel(type,parent)
{
}

SectionOpeningTableModel::SectionOpeningTableModel(const SectionOpeningTableModel &model)
    :SectionTableModel(model){}

bool SectionOpeningTableModel::isBlockedItem(const QModelIndex &index) const
{
    int row = index.row();
    int col = index.column();

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    int g_col = coltag["LRBASE"];
    int s_col = coltag["SLITL"];
    if ( g_col <= col && col < s_col ) {
        QModelIndex idx = index.model()->index(row, coltag["OPENINGTYPE"]);
        return ( idx.data().toString() == u8"スリット" );
    } else if ( s_col <= col && col < coltag["COMMENT"]) {
        QModelIndex idx = index.model()->index(row, coltag["OPENINGTYPE"]);
        return ( idx.data().toString() != u8"スリット" );
    }
    return false;
}

SectionValues SectionOpeningTableModel::getMemberSection(int row) const
{
    return CreateWallOpenSection( getRowData(row), row );
}

QList<SectionValues> SectionOpeningTableModel::getSectionList() const
{
    QList<SectionValues> sectionList;
    for ( int i = 0; i < allItems.rowCount(); i++ ) {
        if ( !allItems.isValidRow(i) ) continue;
        sectionList.append( CreateWallOpenSection( getRowData(i),  i ) );
    }
    return sectionList;
}

WallOpenSection SectionOpeningTableModel::CreateWallOpenSection(
        const QList<QVariant> &myValues,
        int currentrow) const
{
    WallOpenSection wl;
    QHash<int, QVariant> defaultValues = mySectionType.getLineDefaultValues(currentrow);

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();

    wl.sectionName = myValues.at(coltag["LABEL"]).toString();
    wl.memberType = mySectionType.getMemberType();
    wl.sectionType = mySectionType.getStructType();

    QString str;

    str = myValues.at(coltag["OPENINGTYPE"]).toString().isEmpty()
          ? defaultValues[coltag["OPENINGTYPE"]].toString()
          : myValues.at(coltag["OPENINGTYPE"]).toString();
    wl.wopen_param.openType = WallOpenType::stringToEnumType(str);
    str = myValues.at(coltag["LRBASE"]).toString();
    wl.wopen_param.isHorizontalFace = str == u8"内法から";
    str = myValues.at(coltag["LR"]).toString();
    wl.wopen_param.hPosition = WallOpenType::stringToEnumH(str);
    str = myValues.at(coltag["UDBASE"]).toString();
    wl.wopen_param.isVerticalFace = str == u8"内法から";
    str = myValues.at(coltag["UD"]).toString();
    wl.wopen_param.vPosition = WallOpenType::stringToEnumV(str);

    wl.wopen_param.ox  = myValues.at(coltag["POSITIONX"]).toString().isEmpty()
                      ? defaultValues[coltag["POSITIONX"]].toDouble()
                      : myValues.at(coltag["POSITIONX"]).toDouble();
    wl.wopen_param.oy  = myValues.at(coltag["POSITIONY"]).toString().isEmpty()
                      ? defaultValues[coltag["POSITIONY"]].toDouble()
                      : myValues.at(coltag["POSITIONY"]).toDouble();
    wl.wopen_param.width  = myValues.at(coltag["WIDTH"]).toString().isEmpty()
                         ? defaultValues[coltag["WIDTH"]].toDouble()
                         : myValues.at(coltag["WIDTH"]).toDouble();
    wl.wopen_param.height  = myValues.at(coltag["HEIGHT"]).toString().isEmpty()
                          ? defaultValues[coltag["HEIGHT"]].toDouble()
                          : myValues.at(coltag["HEIGHT"]).toDouble();

    wl.wopen_param.slitR = myValues.at(coltag["SLITR"]).toString().isEmpty()
                        ? defaultValues[coltag["SLITR"]].toDouble()
                        : myValues.at(coltag["SLITR"]).toDouble();
    wl.wopen_param.slitL = myValues.at(coltag["SLITL"]).toString().isEmpty()
                        ? defaultValues[coltag["SLITL"]].toDouble()
                        : myValues.at(coltag["SLITL"]).toDouble();
    wl.wopen_param.slitT = myValues.at(coltag["SLITT"]).toString().isEmpty()
                        ? defaultValues[coltag["SLITT"]].toDouble()
                        : myValues.at(coltag["SLITT"]).toDouble();
    wl.wopen_param.slitB = myValues.at(coltag["SLITB"]).toString().isEmpty()
                        ? defaultValues[coltag["SLITB"]].toDouble()
                        : myValues.at(coltag["SLITB"]).toDouble();
    wl.wopen_param.comment = myValues.at(coltag["COMMENT"]).toString();

    return wl;
}
} // namespace post3dapp
