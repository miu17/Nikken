#include "section_brace_tablemodel.h"
#include "define_section_struct.h"

namespace post3dapp{
SectionBraceTableModel::SectionBraceTableModel(const SectionDataType &type, QObject *parent):
    SectionTableModel(type,parent)
{

}
SectionBraceTableModel::SectionBraceTableModel(const SectionBraceTableModel & model)
    : SectionTableModel(model){}

SectionValues SectionBraceTableModel::getMemberSection(int row) const
{
    return CreateLineMemberSection( getRowData(row), row );
}

QList<SectionValues> SectionBraceTableModel::getSectionList() const
{
    QList<SectionValues> sectionList;
    for ( int i = 0; i < allItems.rowCount(); i++ ) {
        if ( !allItems.isValidRow(i) ) continue;
        sectionList.append(CreateLineMemberSection(getRowData(i), i));
    }
    return sectionList;
}

LineMemberSection SectionBraceTableModel::CreateLineMemberSection(
        const QList<QVariant> &myValues,
        int currentrow) const
{
    LineMemberSection lm;
    const qreal EPS = 1.0e-5;
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();

    // 共通
    lm.sectionName = myValues.at(coltag["LABEL"]).toString();
    lm.memberType = mySectionType.getMemberType();
    lm.sectionType = mySectionType.getStructType();

    lm.line_param.finishLoad = myValues.at(coltag["FINISHLOAD"]).toDouble();

    QHash<int, QVariant> defaultValues = mySectionType.getLineDefaultValues(currentrow);
    bool isS      = ( lm.sectionType == STRUCTTYPE::STP_S );
    //table3,4、チェック
    bool table3input = mySectionType.hasData(myValues, 3);
    bool table4input = mySectionType.hasData(myValues, 4); //columnの時はfalse

    const int sectionnum = 3;
    LinePartsParameter *param = new LinePartsParameter[sectionnum];

    if (isS) {
        lm.line_param.i_length = myValues.at(coltag["iLENGTH"]).toDouble();
        if ( lm.line_param.i_length < EPS ) lm.line_param.i_length = 0.0;
        lm.line_param.j_length = myValues.at(coltag["jLENGTH"]).toDouble();
        if ( lm.line_param.j_length < EPS ) lm.line_param.j_length = 0.0;

        lm.line_param.hasCompResistance = (myValues.at(coltag["COMPRESSIVERESISTANCE"]).toString() == u8"引張にのみ有効") ? false : true;
        lm.line_param.direct_area = myValues.at(coltag["AREA"]).toDouble();
        lm.line_param.direct_effectiveArea = myValues.at(coltag["EFFECTIVEAREA"]).toDouble();
        lm.line_param.direct_slenderRatio = myValues.at(coltag["SLENDERNESSRATIO"]).toDouble();
        lm.line_param.direct_unitmass = myValues.at(coltag["UNITMASS"]).toDouble();
        //端中
        for (int i = 0; i < sectionnum; ++i) {
            if (i == 1 && !table3input) {
                param[1] = param[0];
                continue;
            }
            if (i == 2 && !table4input) {
                param[2] = param[0];
                continue;
            }
            QString pre;
            if (i == 0) {
                pre = "i";
            } else if (i == 1) {
                pre = "m";
            } else if (i == 2) {
                pre = "j";
            }
            param[i].steelMaterial = myValues.at(coltag[pre + "STEEL"]).toString();

            QString bname = myValues.at(coltag[pre + "SHAPELISTNAME"]).toString();
            SteelType::Type stype = SteelType::stringToEnum(bname);
            if ( stype == SteelType::Type::UNDEFINED && !bname.isEmpty() ) { // 規格断面
                QString sname = myValues.at(coltag[pre + "STANDARDSHAPESTEEL"]).toString();
                param[i].standardName = SectionDataType::standardSteelSectionName(bname, sname);
                QString stp = SectionDataType::standardSteelSectionValue(param[i].standardName,
                                                                         StandardSectionTable::SteelFormat::ST_TP);
                param[i].steelType = SteelType::stringToEnum(stp);
            } else { // 定義断面
                param[i].steelType = stype;
                param[i].standardName = QString();

            }
            param[i].sizeH = myValues.at(coltag[pre + "H"]).toDouble();
            param[i].sizeB = myValues.at(coltag[pre + "B"]).toDouble();
            param[i].t1 = myValues.at(coltag[pre + "T1"]).toDouble();
            param[i].t2 = myValues.at(coltag[pre + "T2"]).toDouble();

            if ( param[i].steelType == SteelType::Type::ST_BX ||
                    param[i].steelType == SteelType::Type::ST_WBX ) {
                if ( param[i].sizeB < EPS ) param[i].sizeB = param[i].sizeH;
                if ( param[i].t2 < EPS ) param[i].t2 = param[i].t1;
            }

            if ( param[i].steelType == SteelType::Type::ST_P ||
                    param[i].steelType == SteelType::Type::ST_WP || param[i].steelType == SteelType::Type::ST_WBR ) {
                param[i].sizeB = param[i].sizeH;
                param[i].t2 = param[i].t1;
            }
        }
        lm.i_param = param[0];
        lm.c_param = param[1];
        lm.j_param = param[2];
    }

    delete [] param;

    return lm;
}

bool SectionBraceTableModel::isBlockedItem(const QModelIndex &index) const
{
    int row = index.row();
    int col = index.column();

    //規格断面によるDisable
    if ( !shapetype_columns.isEmpty() ) {

        // ブレース直接入力
        QList<int> dindexlist = mySectionType.getDirectInputColumns();
        int maxdcol = 0;
        bool isDirectInput = false;
        Q_FOREACH( int dcol, dindexlist ) {
            if ( maxdcol < dcol ) maxdcol = dcol;
            if (allItems.isValid(row, dcol)) isDirectInput = true;
        }
        if (isDirectInput){
            if (maxdcol < col) return true;
        }else{
            bool hasSection = false;
            for (int i = maxdcol; i <COLUMNS; i++){
                if (allItems.isValid(row, i)) {
                    hasSection = true;
                    break;
                }
            }
            if (hasSection){
               if (dindexlist.contains(col)) return true;
            }
        }
    }

    return SectionTableModel::isBlockedItem(index);
}
} // namespace post3dapp
