#include "section_col_tablemodel.h"
#include "define_section_struct.h"

namespace post3dapp{
SectionColumnTableModel::SectionColumnTableModel(const SectionDataType &type, QObject *parent):
    SectionTableModel(type,parent)
{
}

SectionColumnTableModel::SectionColumnTableModel(const SectionColumnTableModel &model)
    :SectionTableModel(model){}


SectionValues SectionColumnTableModel::getMemberSection(int row) const
{
    return CreateLineMemberSection( getRowData(row), row );
}

QList<SectionValues> SectionColumnTableModel::getSectionList() const
{
    QList<SectionValues> sectionList;
    for ( int i = 0; i < allItems.rowCount(); i++ ) {
        if ( !allItems.isValidRow(i) ) continue;
        sectionList.append(CreateLineMemberSection(getRowData(i), i));
    }
    return sectionList;
}

LineMemberSection SectionColumnTableModel::CreateLineMemberSection(
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
    bool isCFT    = ( lm.sectionType == STRUCTTYPE::STP_CFT );

    //table3,4、チェック
    bool table3input = mySectionType.hasData(myValues, 3);
    bool table4input = mySectionType.hasData(myValues, 4); //columnの時はfalse

    int sectionnum;

    lm.startFloorID = myValues.at(coltag["FLOORFROM"]).toString();
   lm.endFloorID = myValues.at(coltag["FLOORTO"]).toString();
   LinePartsParameter *param = new LinePartsParameter[2];
   sectionnum = 2;

   if (isRC) {
       lm.line_param.concMaterial = myValues.at(coltag["CONC"]).toString();

       //端中
       for (int i = 0; i < sectionnum; ++i) {
           QString pre;
           if (i == 1 && !table3input) {
               param[1] = param[0];
               continue;
           }
           if (i == 0) {
               pre = "i";
           } else if (i == 1) {
               pre = "j";
           }
           if     (myValues.at(coltag[pre + "DY"]).toString().isEmpty()) {
               param[i].concType = ConcreteType::Type::UNDEFINED;
           } else if (myValues.at(coltag[pre + "DZ"]).toString().isEmpty()) {
               param[i].concType = ConcreteType::Type::CIRCLE;
           } else {
               param[i].concType = ConcreteType::Type::RECTANGLE;
           }
           if     (myValues.at(coltag[pre + "AXIALDY"]).toString().isEmpty()) {
               param[i].axialConcType = ConcreteType::Type::UNDEFINED;
           } else if (myValues.at(coltag[pre + "AXIALDZ"]).toString().isEmpty()) {
               param[i].axialConcType = ConcreteType::Type::CIRCLE;
           } else {
               param[i].axialConcType = ConcreteType::Type::RECTANGLE;
           }
           param[i].RK = myValues.at(coltag[pre + "RK"]).toString().isEmpty()
                         ? defaultValues[coltag[pre + "RK"]].toInt()
                         : myValues.at(coltag[pre + "RK"]).toInt();
           param[i].RY = myValues.at(coltag[pre + "RY"]).toString().isEmpty()
                         ? defaultValues[coltag[pre + "RY"]].toInt()
                         : myValues.at(coltag[pre + "RY"]).toInt();
           param[i].RY1 = param[i].RY;
           param[i].Dy = myValues.at(coltag[pre + "DY"]).toDouble();
           param[i].Dz = myValues.at(coltag[pre + "DZ"]).toDouble();
           if (param[i].concType == ConcreteType::Type::CIRCLE) {
               param[i].bar_readOK = ReadBarArrangement::readBarsColumnCirc(myValues.at(
                                                                                coltag[pre + "MAINBAR1"]).toString(), &param[i].mainBarArrange1);
               if (!param[i].bar_readOK) {
                   ReadBarArrangement::readBarsColumnCirc("0", &param[i].mainBarArrange1);
               }
           } else if (param[i].concType == ConcreteType::Type::RECTANGLE) {
               ReadBarArrangement::readBarsColumn(myValues.at(coltag[pre + "MAINBAR1"]).toString(),
                                                  &param[i].mainBarArrange1);
               ReadBarArrangement::readBarsColumn(myValues.at(coltag[pre + "MAINBAR2"]).toString(),
                                                  &param[i].mainBarArrange2);
               param[i].bar_readOK = ReadBarArrangement::barErrorCheckColumn(param[i].mainBarArrange1,
                                                                             param[i].mainBarArrange2);
               if (!param[i].bar_readOK) {
                   ReadBarArrangement::readBarsColumn("0", &param[i].mainBarArrange1);
                   ReadBarArrangement::readBarsColumn("0", &param[i].mainBarArrange2);
               }
           }
           param[i].mainBar1 = myValues.at(coltag[pre + "MAINBARPHI"]).toString();
           param[i].hoopY = myValues.at(coltag[pre + "HOOPY"]).toInt();
           param[i].hoopZ = myValues.at(coltag[pre + "HOOPZ"]).toInt();
           param[i].hoopBar = myValues.at(coltag[pre + "HOOPPHI"]).toString();
           param[i].hoopPitch = ( myValues.at(coltag[pre + "HOOPPITCH"]).toString().isEmpty() )
                                ? defaultValues[coltag[pre + "HOOPPITCH"]].toInt()
                                : myValues.at(coltag[pre + "HOOPPITCH"]).toInt();
           param[i].axialCenterY = myValues.at(coltag[pre + "AXIALCENTERY"]).toDouble();
           param[i].axialCenterZ = myValues.at(coltag[pre + "AXIALCENTERZ"]).toDouble();
           param[i].axialDy = myValues.at(coltag[pre + "AXIALDY"]).toDouble();
           param[i].axialDz = myValues.at(coltag[pre + "AXIALDZ"]).toDouble();
           if (param[i].axialConcType == ConcreteType::Type::CIRCLE) {
               param[i].bar_readOK = ReadBarArrangement::readBarsColumnCirc(myValues.at(
                                                                                coltag[pre + "AXIALBAR1"]).toString(), &param[i].axialBarArrange1);
               if (!param[i].bar_readOK) {
                   ReadBarArrangement::readBarsColumnCirc("0", &param[i].axialBarArrange1);
               }
           } else if (param[i].axialConcType == ConcreteType::Type::RECTANGLE) {
               ReadBarArrangement::readBarsColumn(myValues.at(coltag[pre + "AXIALBAR1"]).toString(),
                                                  &param[i].axialBarArrange1);
               ReadBarArrangement::readBarsColumn(myValues.at(coltag[pre + "AXIALBAR2"]).toString(),
                                                  &param[i].axialBarArrange2);
               param[i].bar_readOK = ReadBarArrangement::barErrorCheckColumn(param[i].axialBarArrange1,
                                                                             param[i].axialBarArrange2);
               if (!param[i].bar_readOK) {
                   ReadBarArrangement::readBarsColumn("0", &param[i].axialBarArrange1);
                   ReadBarArrangement::readBarsColumn("0", &param[i].axialBarArrange2);
               }
           }
           param[i].axialBar = myValues.at(coltag[pre + "AXIALBARPHI"]).toString();
       }
   } else if (isS) {
       lm.line_param.i_length = myValues.at(coltag["iLENGTH"]).toDouble();
       if ( lm.line_param.i_length < EPS ) lm.line_param.i_length = 0.0;
       lm.line_param.basePlate = myValues.at(coltag["BASELIST"]).toString();

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

           param[i].H_T = myValues.at(coltag[pre + "H_T"]).toDouble();
           param[i].B_T = myValues.at(coltag[pre + "B_T"]).toDouble();
           param[i].t1_T = myValues.at(coltag[pre + "T1_T"]).toDouble();
           param[i].t2_T = myValues.at(coltag[pre + "T2_T"]).toDouble();
       }

   } else if (isSRC) {
       lm.line_param.concMaterial = myValues.at(coltag["CONC"]).toString();
       lm.line_param.filledConcMaterial = myValues.at(coltag["FILLEDCONC"]).toString();
       lm.line_param.i_length = myValues.at(coltag["iLENGTH"]).toDouble();
       if ( lm.line_param.i_length < EPS ) lm.line_param.i_length = 0.0;
       lm.line_param.basePlate = myValues.at(coltag["BASELIST"]).toString();

       //端中
       for (int i = 0; i < sectionnum; ++i) {
           if (i == 1 && !table3input) {
               param[1] = param[0];
               continue;
           }
           QString pre;
           if (i == 0) {
               pre = "i";
           } else if (i == 1) {
               pre = "j";
           }

           if     (myValues.at(coltag[pre + "DY"]).toString().isEmpty()) {
               param[i].concType = ConcreteType::Type::UNDEFINED;
           } else if (myValues.at(coltag[pre + "DZ"]).toString().isEmpty()) {
               param[i].concType = ConcreteType::Type::CIRCLE;
           } else {
               param[i].concType = ConcreteType::Type::RECTANGLE;
           }
           if     (myValues.at(coltag[pre + "AXIALDY"]).toString().isEmpty()) {
               param[i].axialConcType = ConcreteType::Type::UNDEFINED;
           } else if (myValues.at(coltag[pre + "AXIALDZ"]).toString().isEmpty()) {
               param[i].axialConcType = ConcreteType::Type::CIRCLE;
           } else {
               param[i].axialConcType = ConcreteType::Type::RECTANGLE;
           }
           param[i].RK = myValues.at(coltag[pre + "RK"]).toString().isEmpty()
                         ? defaultValues[coltag[pre + "RK"]].toInt()
                         : myValues.at(coltag[pre + "RK"]).toInt();
           param[i].RY = myValues.at(coltag[pre + "RY"]).toString().isEmpty()
                         ? defaultValues[coltag[pre + "RY"]].toInt()
                         : myValues.at(coltag[pre + "RY"]).toInt();
           param[i].RY1 = param[i].RY;
           param[i].Dy = myValues.at(coltag[pre + "DY"]).toDouble();
           param[i].Dz = myValues.at(coltag[pre + "DZ"]).toDouble();
           if (param[i].concType == ConcreteType::Type::CIRCLE) {
               param[i].bar_readOK = ReadBarArrangement::readBarsColumnCirc(myValues.at(
                                                                                coltag[pre + "MAINBAR1"]).toString(), &param[i].mainBarArrange1);
               if (!param[i].bar_readOK) {
                   ReadBarArrangement::readBarsColumnCirc("0", &param[i].mainBarArrange1);
               }
           } else if (param[i].concType == ConcreteType::Type::RECTANGLE) {
               ReadBarArrangement::readBarsColumn(myValues.at(coltag[pre + "MAINBAR1"]).toString(),
                                                  &param[i].mainBarArrange1);
               ReadBarArrangement::readBarsColumn(myValues.at(coltag[pre + "MAINBAR2"]).toString(),
                                                  &param[i].mainBarArrange2);
               param[i].bar_readOK = ReadBarArrangement::barErrorCheckColumn(param[i].mainBarArrange1,
                                                                             param[i].mainBarArrange2);
               if (!param[i].bar_readOK) {
                   ReadBarArrangement::readBarsColumn("0", &param[i].mainBarArrange1);
                   ReadBarArrangement::readBarsColumn("0", &param[i].mainBarArrange2);
               }
           }
           param[i].mainBar1 = myValues.at(coltag[pre + "MAINBARPHI"]).toString();
           param[i].hoopY = myValues.at(coltag[pre + "HOOPY"]).toInt();
           param[i].hoopZ = myValues.at(coltag[pre + "HOOPZ"]).toInt();
           param[i].hoopBar = myValues.at(coltag[pre + "HOOPPHI"]).toString();
           param[i].hoopPitch = myValues.at(coltag[pre + "HOOPPITCH"]).toString().isEmpty()
                                ? defaultValues[coltag[pre + "HOOPPITCH"]].toInt()
                                : myValues.at(coltag[pre + "HOOPPITCH"]).toInt();

           param[i].axialCenterY = myValues.at(coltag[pre + "AXIALCENTERY"]).toDouble();
           param[i].axialCenterZ = myValues.at(coltag[pre + "AXIALCENTERZ"]).toDouble();
           param[i].axialDy = myValues.at(coltag[pre + "AXIALDY"]).toDouble();
           param[i].axialDz = myValues.at(coltag[pre + "AXIALDZ"]).toDouble();
           if (param[i].axialConcType == ConcreteType::Type::CIRCLE) {
               param[i].bar_readOK = ReadBarArrangement::readBarsColumnCirc(myValues.at(
                                                                                coltag[pre + "AXIALBAR1"]).toString(), &param[i].axialBarArrange1);
               if (!param[i].bar_readOK) {
                   ReadBarArrangement::readBarsColumnCirc("0", &param[i].axialBarArrange1);
               }
           } else if (param[i].axialConcType == ConcreteType::Type::RECTANGLE) {
               ReadBarArrangement::readBarsColumn(myValues.at(coltag[pre + "AXIALBAR1"]).toString(),
                                                  &param[i].axialBarArrange1);
               ReadBarArrangement::readBarsColumn(myValues.at(coltag[pre + "AXIALBAR2"]).toString(),
                                                  &param[i].axialBarArrange2);
               param[i].bar_readOK = ReadBarArrangement::barErrorCheckColumn(param[i].axialBarArrange1,
                                                                             param[i].axialBarArrange2);
               if (!param[i].bar_readOK) {
                   ReadBarArrangement::readBarsColumn("0", &param[i].axialBarArrange1);
                   ReadBarArrangement::readBarsColumn("0", &param[i].axialBarArrange2);
               }
           }
           param[i].axialBar = myValues.at(coltag[pre + "AXIALBARPHI"]).toString();

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
           param[i].cY = myValues.at(coltag[pre + "COODINATEY"]).toDouble();
           param[i].cZ = myValues.at(coltag[pre + "COODINATEZ"]).toDouble();


           bname = myValues.at(coltag[pre + "SHAPELISTNAME_T"]).toString();
           stype = SteelType::stringToEnum(bname);
           if ( stype == SteelType::Type::UNDEFINED && !bname.isEmpty() ) { // 規格断面
               QString sname = myValues.at(coltag[pre + "STANDARDSHAPESTEEL_T"]).toString();
               param[i].standardName_T = SectionDataType::standardSteelSectionName(bname, sname);
               QString stp = SectionDataType::standardSteelSectionValue(param[i].standardName_T,
                                                                        StandardSectionTable::SteelFormat::ST_TP);
               param[i].steelType_T = SteelType::stringToEnum(stp);
           } else { // 定義断面
               param[i].steelType_T = stype;
               param[i].standardName_T = QString();

           }
           param[i].H_T = myValues.at(coltag[pre + "H_T"]).toDouble();
           param[i].B_T = myValues.at(coltag[pre + "B_T"]).toDouble();
           param[i].t1_T = myValues.at(coltag[pre + "T1_T"]).toDouble();
           param[i].t2_T = myValues.at(coltag[pre + "T2_T"]).toDouble();

           if ( param[i].steelType_T == SteelType::Type::ST_BX ||
                   param[i].steelType == SteelType::Type::ST_WBX ) {
               if ( param[i].B_T < EPS ) param[i].B_T = param[i].H_T;
               if ( param[i].t2_T < EPS ) param[i].t2_T = param[i].t1_T;
           }

           if ( param[i].steelType_T == SteelType::Type::ST_P ||
                   param[i].steelType_T == SteelType::Type::ST_WP || param[i].steelType_T == SteelType::Type::ST_WBR ) {
               param[i].B_T = param[i].H_T;
               param[i].t2_T = param[i].t1_T;
           }
           param[i].cY_T = myValues.at(coltag[pre + "COODINATEY_T"]).toDouble();
           param[i].cZ_T = myValues.at(coltag[pre + "COODINATEZ_T"]).toDouble();

       }
   } else if (isCFT) {

       lm.line_param.filledConcMaterial = myValues.at(coltag["FILLEDCONC"]).toString();
       lm.line_param.i_length = myValues.at(coltag["iLENGTH"]).toDouble();
       if ( lm.line_param.i_length < EPS ) lm.line_param.i_length = 0.0;
       lm.line_param.basePlate = myValues.at(coltag["BASELIST"]).toString();

       //i,j端
       for (int i = 0; i < sectionnum; ++i) {
           if (i == 1 && !table3input) {
               param[1] = param[0];
               continue;
           }
           QString pre;
           if (i == 0) {
               pre = "i";
           } else if (i == 1) {
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
   }//isCFT

   lm.i_param = param[0];
   lm.c_param = param[0];
   lm.j_param = param[1];

   delete [] param;


    return lm;
}
} // namespace post3dapp
