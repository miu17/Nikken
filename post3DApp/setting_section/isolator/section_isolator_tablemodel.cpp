#include "section_isolator_tablemodel.h"
#include "define_section_struct.h"


namespace post3dapp{
SectionIsolatorTableModel::SectionIsolatorTableModel(const SectionDataType &type, QObject *parent):
    SectionTableModel(type,parent)
{
}

SectionIsolatorTableModel::SectionIsolatorTableModel(const SectionIsolatorTableModel &model)
    :SectionTableModel(model){}


SectionValues SectionIsolatorTableModel::getMemberSection(int row) const
{
    return CreateIsolatorSection( getRowData(row), row );
}

QList<SectionValues> SectionIsolatorTableModel::getSectionList() const
{
    QList<SectionValues> sectionList;
    for ( int i = 0; i < allItems.rowCount(); i++ ) {
        if ( !allItems.isValidRow(i) ) continue;
        sectionList.append( CreateIsolatorSection( getRowData(i), i ) );
    }
    return sectionList;
}

bool SectionIsolatorTableModel::isBlockedItem(const QModelIndex &index) const
{
    int row = index.row();
    int col = index.column();

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    if ( !shapetype_columns.isEmpty() ) {
        if ( shapetype_columns.contains(col - 1) ) {
            QModelIndex idx = index.model()->index(row, col - 1);
            if ( !standardList.contains(idx.data().toString()) ) return true;
        }

        for ( int i = 2; i <= coltag["vFY1_PLUS"] - coltag["PRODUCTTYPE"]; i++ ) {
            if ( !shapetype_columns.contains(col - i) ) continue;
            QModelIndex idx = index.model()->index(row, col - i);
            if ( standardList.contains(idx.data().toString()) ) return true;
        }
    }

    QString hHysterisis = index.model()->index(row, coltag["SHEARHYSTERESIS"]).data().toString();
    int hHys = coltag["SHEARHYSTERESIS"];
    if (hHysterisis == u8"線形"){
        if (hHys + 1 < col && col < hHys + 6)
            return true;
    }else if (hHysterisis == u8"リバーシブルバイリニア" || hHysterisis == u8"バイリニア"){
        if (col == hHys + 3 )
            return true;
    }

    QString vHysterisis = index.model()->index(row, coltag["AXIALHYSTERESIS"]).data().toString();
    int vHys = coltag["AXIALHYSTERESIS"];
    if (vHysterisis == u8"線形"){
        if (vHys + 1 < col && col < vHys + 7)
            return true;
    }else if (vHysterisis == u8"引張側リバーシブルバイリニア"){
        if (col == vHys + 3 ||   col == vHys + 5)
            return true;
    }

    return false;
}


IsolatorSection SectionIsolatorTableModel::CreateIsolatorSection(
        const QList<QVariant> &myValues,
        int currentrow) const
{
    IsolatorSection iso;
    QHash<int, QVariant> defaultValues = mySectionType.getLineDefaultValues(currentrow);
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();

    iso.sectionName = myValues.at(coltag["LABEL"]).toString();
    iso.memberType = mySectionType.getMemberType();
    iso.sectionType = mySectionType.getStructType();

    iso.isolator_param.productType = myValues.at(coltag["PRODUCTTYPE"]).toString().isEmpty()
                                 ? defaultValues[coltag["PRODUCTTYPE"]].toString()
                                 : myValues.at(coltag["PRODUCTTYPE"]).toString();
    iso.isolator_param.productName = myValues.at(coltag["PRODUCTNAME"]).toString().isEmpty()
                                 ? defaultValues[coltag["PRODUCTNAME"]].toString()
                                 : myValues.at(coltag["PRODUCTNAME"]).toString();

    QString hys = myValues.at(coltag["SHEARHYSTERESIS"]).toString().isEmpty()
                  ? defaultValues[coltag["SHEARHYSTERESIS"]].toString()
                  : myValues.at(coltag["SHEARHYSTERESIS"]).toString();
    iso.hspring_param.hysteresis = SpringType::stringToEnum(hys);
    iso.hspring_param.K1 = myValues.at(coltag["hK1"]).toString().isEmpty()
                       ? defaultValues[coltag["hK1"]].toDouble()
                       : myValues.at(coltag["hK1"]).toDouble();
    iso.hspring_param.K2 = myValues.at(coltag["hK2"]).toString().isEmpty()
                       ? defaultValues[coltag["hK2"]].toDouble()
                       : myValues.at(coltag["hK2"]).toDouble();
    iso.hspring_param.K3 = myValues.at(coltag["hK3"]).toString().isEmpty()
                       ? defaultValues[coltag["hK3"]].toDouble()
                       : myValues.at(coltag["hK3"]).toDouble();
    //バイリニア
    //リバーシブルバイリニア
    if(SpringType::stringToEnum(hys)==SpringType::Type::BILINEAR
            ||SpringType::stringToEnum(hys)==SpringType::Type::REVERSIBLE_BILINEAR){
    iso.hspring_param.Fy1p = myValues.at(coltag["hFY1"]).toString().isEmpty()
                             ? defaultValues[coltag["hFY1"]].toDouble()
                             : myValues.at(coltag["hFY1"]).toDouble();
    iso.hspring_param.Fy1m = myValues.at(coltag["hFY2"]).toString().isEmpty()
                             ? defaultValues[coltag["hFY2"]].toDouble()
                             : myValues.at(coltag["hFY2"]).toDouble();
    }
    //トリリニア
    if(SpringType::stringToEnum(hys)==SpringType::Type::TRILINEAR){
    iso.hspring_param.Fy1p = myValues.at(coltag["hFY1"]).toString().isEmpty()
                             ? defaultValues[coltag["hFY1"]].toDouble()
                             : myValues.at(coltag["hFY1"]).toDouble();
    iso.hspring_param.Fy2p = myValues.at(coltag["hFY2"]).toString().isEmpty()
                             ? defaultValues[coltag["hFY2"]].toDouble()
                             : myValues.at(coltag["hFY2"]).toDouble();
    }

    hys = myValues.at(coltag["AXIALHYSTERESIS"]).toString().isEmpty()
                  ? defaultValues[coltag["AXIALHYSTERESIS"]].toString()
                  : myValues.at(coltag["AXIALHYSTERESIS"]).toString();
    iso.vspring_param.hysteresis = SpringType::stringToEnum(hys);
    iso.vspring_param.K1 = myValues.at(coltag["vK1"]).toString().isEmpty()
                       ? defaultValues[coltag["vK1"]].toDouble()
                       : myValues.at(coltag["vK1"]).toDouble();
    iso.vspring_param.K2 = myValues.at(coltag["vK2"]).toString().isEmpty()
                       ? defaultValues[coltag["vK2"]].toDouble()
                       : myValues.at(coltag["vK2"]).toDouble();
    iso.vspring_param.K3 = myValues.at(coltag["vK3"]).toString().isEmpty()
                       ? defaultValues[coltag["vK3"]].toDouble()
                       : myValues.at(coltag["vK3"]).toDouble();
    iso.vspring_param.Fy1p = myValues.at(coltag["vFY1_PLUS"]).toString().isEmpty()
            ? defaultValues[coltag["vFY1_PLUS"]].toDouble()
            : myValues.at(coltag["vFY1_PLUS"]).toDouble();
    iso.vspring_param.Fy2p = myValues.at(coltag["vFY2_PLUS"]).toString().isEmpty()
            ? defaultValues[coltag["vFY2_PLUS"]].toDouble()
            : myValues.at(coltag["vFY2_PLUS"]).toDouble();

    iso.isolator_param.selfWeight = myValues.at(coltag["SELFWEIGHT"]).toString().isEmpty()
                                ? defaultValues[coltag["SELFWEIGHT"]].toDouble()
                                : myValues.at(coltag["SELFWEIGHT"]).toDouble();

    QString str = myValues.at(coltag["RENDERSHAPE"]).toString().isEmpty()
                  ? defaultValues[coltag["RENDERSHAPE"]].toString()
                  : myValues.at(coltag["RENDERSHAPE"]).toString();
    iso.isolator_param.renderShape = str;
    iso.isolator_param.sizeD = myValues.at(coltag["RENDERSIZED"]).toString().isEmpty()
                           ? defaultValues[coltag["RENDERSIZED"]].toDouble()
                           : myValues.at(coltag["RENDERSIZED"]).toDouble();
    iso.isolator_param.sizeH = myValues.at(coltag["RENDERSIZEH"]).toString().isEmpty()
                           ? defaultValues[coltag["RENDERSIZEH"]].toDouble()
                           : myValues.at(coltag["RENDERSIZEH"]).toDouble();
    iso.isolator_param.settingLevel = myValues.at(coltag["SETLEVEL"]).toString().isEmpty()
                                  ? defaultValues[coltag["SETLEVEL"]].toDouble()
                                  : myValues.at(coltag["SETLEVEL"]).toDouble();
    iso.isolator_param.displacement = myValues.at(coltag["DISPLACEMENT"]).toString().isEmpty()
                                  ? defaultValues[coltag["DISPLACEMENT"]].toDouble()
                                  : myValues.at(coltag["DISPLACEMENT"]).toDouble();
    iso.isolator_param.springNumber = myValues.at(coltag["SPRING"]).toString().isEmpty()
                                  ? defaultValues[coltag["SPRING"]].toInt()
                                  : myValues.at(coltag["SPRING"]).toInt();
    iso.isolator_param.inflectionRatio = myValues.at(coltag["INFLECTION"]).toString().isEmpty()
                                  ? defaultValues[coltag["INFLECTION"]].toDouble()
                                  : myValues.at(coltag["INFLECTION"]).toDouble();

    return iso;
}
} // namespace post3dapp
