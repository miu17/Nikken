#include "section_beam_tablemodel.h"
#include "define_section_struct.h"

namespace post3dapp{
SectionBeamTableModel::SectionBeamTableModel(const SectionDataType &type, QObject *parent):
    SectionTableModel(type,parent)
{

}

SectionBeamTableModel::SectionBeamTableModel(const SectionBeamTableModel& model)
    : SectionTableModel(model){}

SectionValues SectionBeamTableModel::getMemberSection(int row) const
{
    return CreateLineMemberSection( getRowData(row), row );
}

QList<SectionValues> SectionBeamTableModel::getSectionList() const
{
    QList<SectionValues> sectionList;
    for ( int i = 0; i < allItems.rowCount(); i++ ) {
        if ( !allItems.isValidRow(i) ) continue;
        sectionList.append(CreateLineMemberSection(getRowData(i), i));
    }
    return sectionList;
}

LineMemberSection SectionBeamTableModel::CreateLineMemberSection(
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
    bool isRC     = ( lm.sectionType == STRUCTTYPE::STP_RC );
    bool isSRC    = ( lm.sectionType == STRUCTTYPE::STP_SRC );

    //table3,4、チェック
    bool table3input = mySectionType.hasData(myValues, 3);
    bool table4input = mySectionType.hasData(myValues, 4); //columnの時はfalse

    lm.line_param.isCantilever = ( myValues.at(coltag["CANTILEVER"]).toString() == u8"片持");
    lm.startFloorID = myValues.at(coltag["FLOORFROM"]).toString();
    lm.endFloorID = myValues.at(coltag["FLOORTO"]).toString();
    LinePartsParameter *param = new LinePartsParameter[3];
    const int sectionnum = 3;

    if (isRC) {
        lm.line_param.concMaterial = myValues.at(coltag["CONC"]).toString();

        //端中
        for (int i = 0; i < sectionnum; ++i) {
            QString pre;
            if (i == 1 && !table3input) {
                param[1] = param[0];
                continue;
            }
            if (i == 2 && !table4input) {
                param[2] = param[0];
                continue;
            }
            if (i == 0) {
                pre = "i";
            } else if (i == 1) {
                pre = "m";
            } else if (i == 2) {
                pre = "j";
            }
            if     (myValues.at(coltag[pre + "DY"]).toString().isEmpty()) {
                param[i].concType = ConcreteType::Type::UNDEFINED;
            } else if (myValues.at(coltag[pre + "DZ"]).toString().isEmpty()) {
                param[i].concType = ConcreteType::Type::UNDEFINED;
            } else {
                param[i].concType = ConcreteType::Type::RECTANGLE;
            }
            param[i].RK = myValues.at(coltag[pre + "RK"]).toString().isEmpty()
                          ? defaultValues[coltag[pre + "RK"]].toInt()
                          : myValues.at(coltag[pre + "RK"]).toInt();
            param[i].RKu = myValues.at(coltag[pre + "RKU"]).toString().isEmpty()
                           ? defaultValues[coltag[pre + "RKU"]].toInt()
                           : myValues.at(coltag[pre + "RKU"]).toInt();
            param[i].RKb = myValues.at(coltag[pre + "RKB"]).toString().isEmpty()
                           ? defaultValues[coltag[pre + "RKB"]].toInt()
                           : myValues.at(coltag[pre + "RKB"]).toInt();
            param[i].RY = myValues.at(coltag[pre + "RY"]).toString().isEmpty()
                          ? defaultValues[coltag[pre + "RY"]].toInt()
                          : myValues.at(coltag[pre + "RY"]).toInt();
            param[i].RY1 = param[i].RY;
            param[i].Dy = myValues.at(coltag[pre + "DY"]).toDouble();
            param[i].Dz = myValues.at(coltag[pre + "DZ"]).toDouble();
            ReadBarArrangement::readBarsGirder(myValues.at(coltag[pre + "MAINBAR1"]).toString(),
                                               &param[i].mainBarArrange1);
            ReadBarArrangement::readBarsGirder(myValues.at(coltag[pre + "MAINBAR2"]).toString(),
                                               &param[i].mainBarArrange2);
            param[i].bar_readOK = ReadBarArrangement::barErrorCheckGirder(param[i].mainBarArrange1,
                                                                          param[i].mainBarArrange2);
            if (!param[i].bar_readOK) {
                ReadBarArrangement::readBarsGirder("0", &param[i].mainBarArrange1);
                ReadBarArrangement::readBarsGirder("0", &param[i].mainBarArrange2);
            }
            param[i].mainBar1 = myValues.at(coltag[pre + "MAINBARPHI1"]).toString();
            param[i].mainBar2 = myValues.at(coltag[pre + "MAINBARPHI2"]).toString().isEmpty()
                ? param[i].mainBar1
                : myValues.at(coltag[pre + "MAINBARPHI2"]).toString();
            param[i].hoopY = 2;
            param[i].hoopZ = myValues.at(coltag[pre + "HOOPZ"]).toInt();
            param[i].hoopBar = myValues.at(coltag[pre + "HOOPPHI"]).toString();
            param[i].hoopPitch = myValues.at(coltag[pre + "HOOPPITCH"]).toString().isEmpty()
                                 ? defaultValues[coltag[pre + "HOOPPITCH"]].toInt()
                                 : myValues.at(coltag[pre + "HOOPPITCH"]).toInt();
        }
    } else if (isS) {
        lm.line_param.i_length = myValues.at(coltag["iLENGTH"]).toDouble();
        if ( lm.line_param.i_length < EPS ) lm.line_param.i_length = 0.0;
        lm.line_param.j_length = myValues.at(coltag["jLENGTH"]).toDouble();
        if ( lm.line_param.j_length < EPS ) lm.line_param.j_length = 0.0;

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
            param[i].steelWebMaterial = myValues.at(coltag[pre + "STEELWEB"]).toString().isEmpty()
                    ? param[i].steelMaterial
                    : myValues.at(coltag[pre + "STEELWEB"]).toString();

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

    } else if (isSRC) {
        lm.line_param.concMaterial = myValues.at(coltag["CONC"]).toString();
        lm.line_param.i_length = myValues.at(coltag["iLENGTH"]).toDouble();
        if ( lm.line_param.i_length < EPS ) lm.line_param.i_length = 0.0;
        lm.line_param.j_length = myValues.at(coltag["jLENGTH"]).toDouble();
        if ( lm.line_param.j_length < EPS ) lm.line_param.j_length = 0.0;

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

//                param[i].hasfilledconc=true;
            if     (myValues.at(coltag[pre + "DY"]).toString().isEmpty()) {
                param[i].concType = ConcreteType::Type::UNDEFINED;
            } else if (myValues.at(coltag[pre + "DZ"]).toString().isEmpty()) {
                param[i].concType = ConcreteType::Type::UNDEFINED;
            } else {
                param[i].concType = ConcreteType::Type::RECTANGLE;
            }
            param[i].RK = myValues.at(coltag[pre + "RK"]).toString().isEmpty()
                          ? defaultValues[coltag[pre + "RK"]].toInt()
                          : myValues.at(coltag[pre + "RK"]).toInt();
            param[i].RKu = myValues.at(coltag[pre + "RKU"]).toString().isEmpty()
                           ? defaultValues[coltag[pre + "RKU"]].toInt()
                           : myValues.at(coltag[pre + "RKU"]).toInt();
            param[i].RKb = myValues.at(coltag[pre + "RKB"]).toString().isEmpty()
                           ? defaultValues[coltag[pre + "RKB"]].toInt()
                           : myValues.at(coltag[pre + "RKB"]).toInt();
            param[i].RY = myValues.at(coltag[pre + "RY"]).toString().isEmpty()
                          ? defaultValues[coltag[pre + "RY"]].toInt()
                          : myValues.at(coltag[pre + "RY"]).toInt();
            param[i].RY1 = myValues.at(coltag[pre + "RY1"]).toString().isEmpty()
                           ? defaultValues[coltag[pre + "RY1"]].toInt()
                           : myValues.at(coltag[pre + "RY1"]).toInt();

            param[i].Dy = myValues.at(coltag[pre + "DY"]).toDouble();
            param[i].Dz = myValues.at(coltag[pre + "DZ"]).toDouble();
            ReadBarArrangement::readBarsGirder(myValues.at(coltag[pre + "MAINBAR1"]).toString(),
                                               &param[i].mainBarArrange1);
            ReadBarArrangement::readBarsGirder(myValues.at(coltag[pre + "MAINBAR2"]).toString(),
                                               &param[i].mainBarArrange2);
            param[i].bar_readOK = ReadBarArrangement::barErrorCheckGirder(param[i].mainBarArrange1,
                                                                          param[i].mainBarArrange2);
            if (!param[i].bar_readOK) {
                ReadBarArrangement::readBarsGirder("0", &param[i].mainBarArrange1);
                ReadBarArrangement::readBarsGirder("0", &param[i].mainBarArrange2);
            }
            param[i].mainBar1 = myValues.at(coltag[pre + "MAINBARPHI1"]).toString();
            param[i].mainBar2 = myValues.at(coltag[pre + "MAINBARPHI2"]).toString().isEmpty()
                ? param[i].mainBar1
                : myValues.at(coltag[pre + "MAINBARPHI2"]).toString();
            param[i].hoopY = 2;
            param[i].hoopZ = myValues.at(coltag[pre + "HOOPZ"]).toInt();
            param[i].hoopBar = myValues.at(coltag[pre + "HOOPPHI"]).toString();
            param[i].hoopPitch = myValues.at(coltag[pre + "HOOPPITCH"]).toString().isEmpty()
                                 ? defaultValues[coltag[pre + "HOOPPITCH"]].toInt()
                                 : myValues.at(coltag[pre + "HOOPPITCH"]).toInt();

            param[i].steelMaterial = myValues.at(coltag[pre + "STEEL"]).toString();
            param[i].steelWebMaterial = myValues.at(coltag[pre + "STEELWEB"]).toString().isEmpty()
                    ? param[i].steelMaterial
                    : myValues.at(coltag[pre + "STEELWEB"]).toString();

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
            param[i].cY = myValues.at(coltag[pre + "COODINATEY"]).toDouble();
            param[i].cZ = myValues.at(coltag[pre + "COODINATEZ"]).toDouble();
        }
    }
    lm.i_param = param[0];
    lm.c_param = param[1];
    lm.j_param = param[2];

    delete [] param;

    return lm;
}
} // namespace post3dapp
