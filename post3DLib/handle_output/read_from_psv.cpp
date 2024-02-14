#include "read_from_psv.h"

#include "define_bar_arrangement.h"
#include "define_defaultvalue.h"
#include "fixed_data.h"
#include "interpreter_csv.h"
#include "unified_inputdata.h"
#include "version.h"
namespace post3dapp{

ReadFromPsv::ReadFromPsv()
{
}

void ReadFromPsv::ReadPsv(QFile *file)
{
    QSharedPointer<InterpreterCsv> interpreter = QSharedPointer<InterpreterCsv>(new InterpreterCsv());

    QTextStream infile(file);
    infile.setCodec("UTF-8");
    LibraryVersion ver(infile.readLine());
    while(!infile.atEnd()){
        QString str = infile.readLine(0);
        if(str == "**FLOOR"){
            readFloor(infile, interpreter);
        }else if(str == "**FRAME"){
            readFrame(infile, interpreter);
        }else if(str == "**MATERIAL"){
            readMaterial(infile, interpreter);
        }else if(str == "**COLUMN"){
            readRcColumn(infile, interpreter);
            readSColumn(infile, interpreter);
            readSrcColumn(infile, interpreter);
            readCftColumn(infile, interpreter);
        }else if(str == "**GIRDER"){
            readRcGirder(infile, interpreter);
            readSGirder(infile, interpreter);
            readSrcGirder(infile, interpreter);
        }else if(str == "**BRACE"){
            readSBrace(infile, interpreter);
        }else if(str == "**BEAM"){
            readRcBeam(infile, interpreter);
            readSBeam(infile, interpreter);
            readSrcBeam(infile, interpreter);
        }else if(str == "**SLAB"){
            readSlab(infile, interpreter);
        }else if(str == "**WALL"){
            readRcWall(infile, interpreter);
            readSWall(infile, interpreter);
        }else if(str == "**UWALL"){
            readFoundationWall(infile, interpreter);
        }else if(str == "**BASEPLATE"){
            readSBase(infile, interpreter);
            readSrcBase(infile, interpreter);
        }else if(str == "**WALLOPEN"){
            readOpening(infile, interpreter);
        }else if(str == "**DAMPER"){
            readDamper(infile, interpreter);
        }else if(str == "**ISOLATOR"){
            readIsolator(infile, interpreter);
        }else if(str == "**JOINTLOAD"){
            readJointLoad(infile, interpreter);
        }else if(str == "**MEMBERLOAD"){
            readMemberLoad(infile, interpreter);
        }else if(str == "**SLABLOAD"){
            readSlabLoad(infile, interpreter);
        }else if(str == "**ANALYSISFLOOR"){
            readAnalysisFloor(infile, interpreter);
        }else if(str == "**GENE"){
            readAnalysisCondition(infile, interpreter);
            break;
        }
    }
    arrange(interpreter);
    interpreter->makeDefinedData();

    while(!infile.atEnd()){
        QString str = infile.readLine(0);
        if(str == "**JOINT"){
            UnifiedInputData::getInInstance()->readTextJointData(infile, ver);
        }else if(str == "**COLUMN_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile,DATATYPE::TPCOLUMN, ver);
        }else if(str == "**GIRDER_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile,DATATYPE::TPGIRDER, ver);
        }else if(str == "**BRACE_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile,DATATYPE::TPBRACE, ver);
        }else if(str == "**BEAM_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile,DATATYPE::TPBEAM, ver);
        }else if(str == "**SLAB_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPSLAB,ver);
        }else if(str == "**WALL_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile,DATATYPE::TPWALL, ver);
        }else if(str == "**UWALL_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile,DATATYPE::TPUWALL, ver);
        }else if(str == "**DAMPER_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile,DATATYPE::TPDAMPER, ver);
        }else if(str == "**ISOLATOR_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile,DATATYPE::TPISO, ver);
        }else if(str == "**JOINTLOAD_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile, DATATYPE::TPJOINTLOAD,ver);
        }else if(str == "**MEMBERLOAD_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile,DATATYPE::TPMEMBERLOAD, ver);
        }else if(str == "**SLABLOAD_MEMBER"){
            UnifiedInputData::getInInstance()->readTextMemberData(infile,DATATYPE::TPSLABLOAD, ver);
        }
    }
}

void ReadFromPsv::readFloor(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    int count = infile.readLine().split(" ").at(0).toInt();
    infile.readLine(); // コメント行
    for (int i = 0; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        FLOORVALUES fv(parseString(list.at(1), REGULATION::defaultFloor, "NAME"),
                       parseDouble(list.at(2), REGULATION::defaultFloor, "HEIGHT"),
                       parseString(list.at(3), REGULATION::defaultFloor, "FLOORSCALE") == u8"表示する",
                       parseString(list.at(4), REGULATION::defaultFloor, "BEAM"),
                       parseString(list.at(5), REGULATION::defaultFloor, "SLAB"),
                       QUuid(parseString(list.at(6), REGULATION::defaultFloor, "UUID")));
        fv.floorType = FLOORTYPE::FL_OVERGROUND;
        interpreter->uuidTable.insert(fv.UUID, fv.floorName);
        if (interpreter->sortedFloors.count() == 0){
            interpreter->sortedFloors.append(fv);
        }else{
            if (interpreter->sortedFloors.last().floorHeight == fv.floorHeight)
                throw;

            if (interpreter->sortedFloors.last().floorHeight < fv.floorHeight){
                interpreter->sortedFloors.append(fv);
            }else{
                for(int i = 0; i < interpreter->sortedFloors.count(); i++){
                    if (fv.floorHeight < interpreter->sortedFloors.at(i).floorHeight){
                        interpreter->sortedFloors.insert(i, fv);
                        break;
                    }
                }
            }
        }
    }

}

void ReadFromPsv::readFrame(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    int count = infile.readLine().split(" ").at(0).toInt();
    infile.readLine(); // コメント行
    for (int i = 0; i < count; i ++){
        QStringList list = infile.readLine().split("\t");

        QString typeString = parseString(list.at(2), REGULATION::defaultFrameGroup, u8"FRAMETYPE");
        FrameType::FRAMETYPE type = FrameType::stringToEnum(typeString);
        FRAMEGROUPVALUE fgv(parseString(list.at(1), REGULATION::defaultFrameGroup, u8"FRAMEGROUP"),
                            type,
                            parseInt(list.at(3), REGULATION::defaultFrameGroup, u8"FRAMESUM"),
                            QPointF(parseDouble(list.at(4), REGULATION::defaultFrameGroup, u8"OX"), parseDouble(list.at(5), REGULATION::defaultFrameGroup, u8"OY")),
                            parseDouble(list.at(6), REGULATION::defaultFrameGroup, u8"STARTANGLE"),
                            parseDouble(list.at(7), REGULATION::defaultFrameGroup, u8"ENDANGLE"),
                            parseInt(list.at(8), REGULATION::defaultFrameGroup, u8"DIVISIONS"),
                            parseDouble(list.at(9), REGULATION::defaultFrameGroup, u8"LENGTHPLUS"),
                            parseDouble(list.at(10), REGULATION::defaultFrameGroup, u8"LENGTHMINUS"),
                            interpreter->uuidTable.key(parseString(list.at(11), REGULATION::defaultFrameGroup, u8"FLOORFROM")),
                            interpreter->uuidTable.key(parseString(list.at(12), REGULATION::defaultFrameGroup, u8"FLOORTO")),
                            QUuid{parseString(list.at(13), REGULATION::defaultFrameGroup, u8"GID")}
                            );
        interpreter->framegroups.append(fgv);
    }
    infile.readLine();
    infile.readLine();
    foreach(auto fgv, interpreter->framegroups){
        double totalspan = 0.0;
        for (int i = 0; i < fgv.count; i++){
            bool isFirst = i == 0;
            QStringList list = infile.readLine().split("\t");
            FRAMEPOINTSVALUE fpv = interpreter->CalculateFramePointsValue(
                        fgv,
                        totalspan,
                        list.at(4).trimmed(),
                        false,
                        isFirst,
                        list.at(3).trimmed(),
                        QUuid{list.at(9).trimmed()});
            interpreter->frames.append(fpv);
        }
    }
}

void ReadFromPsv::readMaterial(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    int count = infile.readLine().split(" ").at(0).toInt();
    infile.readLine();
    for (int i = 0; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        MATERIALVALUES mv(MaterialType::stringToEnum(parseString(list.at(1), REGULATION::defaultMaterial, "CATEGORY")),
                          parseString(list.at(2), REGULATION::defaultMaterial, "LABEL"),
                          parseString(list.at(3), REGULATION::defaultMaterial, "SYMBOL"),
                          parseInt(list.at(4), REGULATION::defaultMaterial, "RANGEFROM"),
                          parseInt(list.at(5), REGULATION::defaultMaterial, "RANGETO"),
                          parseDouble(list.at(6), REGULATION::defaultMaterial, "E"),
                          parseDouble(list.at(7), REGULATION::defaultMaterial, "G"),
                          parseDouble(list.at(8), REGULATION::defaultMaterial, "UNITWEIGHT1"),
                          parseDouble(list.at(9), REGULATION::defaultMaterial, "UNITWEIGHT2"),
                          parseDouble(list.at(10), REGULATION::defaultMaterial, "MODULARRATIO"),
                          parseDouble(list.at(11), REGULATION::defaultMaterial, "F_STRENGTH"),
                          parseDouble(list.at(12), REGULATION::defaultMaterial, "T_L"),
                          parseDouble(list.at(13), REGULATION::defaultMaterial, "T_S"),
                          parseDouble(list.at(14), REGULATION::defaultMaterial, "T_U"),
                          parseDouble(list.at(15), REGULATION::defaultMaterial, "C_L"),
                          parseDouble(list.at(16), REGULATION::defaultMaterial, "C_S"),
                          parseDouble(list.at(17), REGULATION::defaultMaterial, "C_U"),
                          parseDouble(list.at(18), REGULATION::defaultMaterial, "S_L"),
                          parseDouble(list.at(19), REGULATION::defaultMaterial, "S_S"),
                          parseDouble(list.at(20), REGULATION::defaultMaterial, "S_U"),
                          parseDouble(list.at(21), REGULATION::defaultMaterial, "A_L"),
                          parseDouble(list.at(22), REGULATION::defaultMaterial, "A_S"),
                          parseDouble(list.at(23), REGULATION::defaultMaterial, "A_U"),
                          parseDouble(list.at(24), REGULATION::defaultMaterial, "UA_L"),
                          parseDouble(list.at(25), REGULATION::defaultMaterial, "UA_S")
                          );
        interpreter->materials.append(mv);
    }
}

void ReadFromPsv::readRcColumn(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultColumnRc, "LABEL"));
        sv.memberType = DATATYPE::TPCOLUMN;
        sv.sectionType = STRUCTTYPE::STP_RC;
        sv.startFloorID = interpreter->getOrCreateUuid(parseString(list.at(2), REGULATION::defaultColumnRc, "FLOORFROM"));
        sv.endFloorID = interpreter->getOrCreateUuid(parseString(list.at(3), REGULATION::defaultColumnRc, "FLOORTO"));
        sv.line_param.finishLoad = parseInt(list.at(4), REGULATION::defaultColumnRc, u8"FINISHLOAD");
        sv.line_param.concMaterial = parseString(list.at(5), REGULATION::defaultColumnRc, u8"CONC");
        sv.i_param.Dy = parseDouble(list.at(6), REGULATION::defaultColumnRc, u8"iDY");
        sv.i_param.Dz = parseDouble(list.at(7), REGULATION::defaultColumnRc, u8"iDZ");
        QString iMainBar1 = parseString(list.at(8), REGULATION::defaultColumnRc, u8"iMAINBAR1");
        QString iMainBar2{};
        if (qAbs(sv.i_param.Dz) < 0.00001){
            ReadBarArrangement::readBarsColumnCirc(iMainBar1, &sv.i_param.mainBarArrange1);
            sv.i_param.concType = ConcreteType::Type::CIRCLE;
        }else{
            ReadBarArrangement::readBarsColumn(iMainBar1, &sv.i_param.mainBarArrange1);
            iMainBar2 = parseString(list.at(9), REGULATION::defaultColumnRc, u8"iMAINBAR2");
            ReadBarArrangement::readBarsColumn(iMainBar2, &sv.i_param.mainBarArrange2);
            sv.i_param.concType = ConcreteType::Type::RECTANGLE;
        }
        sv.i_param.mainBar1 = parseString(list.at(10), REGULATION::defaultColumnRc, u8"iMAINBARPHI");
        sv.i_param.axialCenterY = parseDouble(list.at(11), REGULATION::defaultColumnRc, u8"iAXIALCENTERY");
        sv.i_param.axialCenterZ = parseDouble(list.at(12), REGULATION::defaultColumnRc, u8"iAXIALCENTERZ");
        sv.i_param.axialDy = parseDouble(list.at(13), REGULATION::defaultColumnRc, u8"iAXIALDY");
        sv.i_param.axialDz = parseDouble(list.at(14), REGULATION::defaultColumnRc, u8"iAXIALDZ");
        QString iAxialBar1 = parseString(list.at(15), REGULATION::defaultColumnRc, u8"iAXIALBAR1");
        ReadBarArrangement::readBarsColumn(iAxialBar1, &sv.i_param.axialBarArrange1);
        QString iAxialBar2 = parseString(list.at(16), REGULATION::defaultColumnRc, u8"iAXIALBAR2");
        ReadBarArrangement::readBarsColumn(iAxialBar2, &sv.i_param.axialBarArrange2);
        sv.i_param.axialBar = parseString(list.at(17), REGULATION::defaultColumnRc, u8"iAXIALBARPHI");
        sv.i_param.hoopY = parseInt(list.at(18), REGULATION::defaultColumnRc, u8"iHOOPY");
        sv.i_param.hoopZ = parseInt(list.at(19), REGULATION::defaultColumnRc, u8"iHOOPZ");
        sv.i_param.hoopBar = parseString(list.at(20), REGULATION::defaultColumnRc, u8"iHOOPPHI");
        sv.i_param.hoopPitch = parseInt(list.at(21), REGULATION::defaultColumnRc, u8"iHOOPPITCH");
        sv.i_param.RK = parseDouble(list.at(22), REGULATION::defaultColumnRc, u8"iRK");
        sv.i_param.RY = parseDouble(list.at(23), REGULATION::defaultColumnRc, u8"iRY");

        sv.j_param.Dy = parseDouble(list.at(24), REGULATION::defaultColumnRc, u8"jDY", sv.i_param.Dy);
        sv.j_param.Dz = parseDouble(list.at(25), REGULATION::defaultColumnRc, u8"jDZ", sv.i_param.Dz);
        QString jMainBar1 = parseString(list.at(26), REGULATION::defaultColumnRc, u8"jMAINBAR1", iMainBar1);
        if (qAbs(sv.j_param.Dz) < 0.00001){
            ReadBarArrangement::readBarsColumnCirc(jMainBar1, &sv.j_param.mainBarArrange1);
            sv.j_param.concType = ConcreteType::Type::CIRCLE;
        }else{
            ReadBarArrangement::readBarsColumn(jMainBar1, &sv.j_param.mainBarArrange1);
            QString jMainBar2 = parseString(list.at(27), REGULATION::defaultColumnRc, u8"jMAINBAR2", iMainBar2);
            ReadBarArrangement::readBarsColumn(jMainBar2, &sv.j_param.mainBarArrange2);
            sv.j_param.concType = ConcreteType::Type::RECTANGLE;
        }
        sv.j_param.mainBar1 = parseString(list.at(28), REGULATION::defaultColumnRc, u8"jMAINBARPHI", sv.i_param.mainBar1);
        sv.j_param.axialCenterY = parseDouble(list.at(29), REGULATION::defaultColumnRc, u8"jAXIALCENTERY", sv.i_param.axialCenterY);
        sv.j_param.axialCenterZ = parseDouble(list.at(30), REGULATION::defaultColumnRc, u8"jAXIALCENTERZ", sv.i_param.axialCenterZ);
        sv.j_param.axialDy = parseDouble(list.at(31), REGULATION::defaultColumnRc, u8"jAXIALDY", sv.i_param.axialDy);
        sv.j_param.axialDz = parseDouble(list.at(32), REGULATION::defaultColumnRc, u8"jAXIALDZ", sv.i_param.axialDz);
        QString jAxialBar1 = parseString(list.at(33), REGULATION::defaultColumnRc, u8"jAXIALBAR1", iAxialBar1);
        ReadBarArrangement::readBarsColumn(jAxialBar1, &sv.j_param.axialBarArrange1);
        QString jAxialBar2 = parseString(list.at(34), REGULATION::defaultColumnRc, u8"jAXIALBAR2", iAxialBar2);
        ReadBarArrangement::readBarsColumn(jAxialBar2, &sv.j_param.axialBarArrange2);
        sv.j_param.axialBar = parseString(list.at(35), REGULATION::defaultColumnRc, u8"jAXIALBARPHI", sv.i_param.axialBar);
        sv.j_param.hoopY = parseInt(list.at(36), REGULATION::defaultColumnRc, u8"jHOOPY", sv.i_param.hoopY);
        sv.j_param.hoopZ = parseInt(list.at(37), REGULATION::defaultColumnRc, u8"jHOOPZ", sv.i_param.hoopZ);
        sv.j_param.hoopBar = parseString(list.at(38), REGULATION::defaultColumnRc, u8"jHOOPPHI", sv.i_param.hoopBar);
        sv.j_param.hoopPitch = parseInt(list.at(39), REGULATION::defaultColumnRc, u8"jHOOPPITCH", sv.i_param.hoopPitch);
        sv.j_param.RK = parseDouble(list.at(40), REGULATION::defaultColumnRc, u8"jRK", sv.i_param.RK);
        sv.j_param.RY = parseDouble(list.at(41), REGULATION::defaultColumnRc, u8"jRY", sv.i_param.RY);
        interpreter->rcColumnList.append(sv);
    }

}

void ReadFromPsv::readSColumn(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultColumnS, "LABEL"));
        sv.memberType = DATATYPE::TPCOLUMN;
        sv.sectionType = STRUCTTYPE::STP_S;
        sv.startFloorID = interpreter->getOrCreateUuid(parseString(list.at(2), REGULATION::defaultColumnS, u8"FLOORFROM"));
        sv.endFloorID = interpreter->getOrCreateUuid(parseString(list.at(3), REGULATION::defaultColumnS, u8"FLOORTO"));
        sv.line_param.finishLoad = parseInt(list.at(4), REGULATION::defaultColumnS, "FINISHLOAD");
        sv.line_param.i_length = parseDouble(list.at(5), REGULATION::defaultColumnS, "iLENGTH");
        sv.line_param.basePlate = parseString(list.at(6), REGULATION::defaultColumnS, "BASELIST");
        sv.i_param.steelMaterial = parseString(list.at(7), REGULATION::defaultColumnS, "iSTEEL");
        QString iSteelType = parseString(list.at(8), REGULATION::defaultColumnS, "iSHAPELISTNAME");
        sv.i_param.steelType = SteelType::stringToEnum(iSteelType);
        sv.i_param.standardName = parseString(list.at(9), REGULATION::defaultColumnS, "iSTANDARDSHAPESTEEL");
        sv.i_param.sizeH = parseDouble(list.at(10), REGULATION::defaultColumnS, "iH");
        sv.i_param.sizeB = parseDouble(list.at(11), REGULATION::defaultColumnS, "iB");
        sv.i_param.t1 = parseDouble(list.at(12), REGULATION::defaultColumnS, "iT1");
        sv.i_param.t2 = parseDouble(list.at(13), REGULATION::defaultColumnS, "iT2");
        sv.i_param.H_T = parseDouble(list.at(14), REGULATION::defaultColumnS, "iH_T");
        sv.i_param.B_T = parseDouble(list.at(15), REGULATION::defaultColumnS, "iB_T");
        sv.i_param.t1_T = parseDouble(list.at(16), REGULATION::defaultColumnS, "iT1_T");
        sv.i_param.t2_T = parseDouble(list.at(17), REGULATION::defaultColumnS, "iT2_T");

        sv.j_param.steelMaterial = parseString(list.at(18), REGULATION::defaultColumnS, "jSTEEL", sv.j_param.steelMaterial);
        sv.j_param.steelType = SteelType::stringToEnum(parseString(list.at(19), REGULATION::defaultColumnS, "jSHAPELISTNAME", iSteelType));
        sv.j_param.standardName = parseString(list.at(20), REGULATION::defaultColumnS, "jSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.j_param.sizeH = parseDouble(list.at(21), REGULATION::defaultColumnS, "jH", sv.i_param.sizeH);
        sv.j_param.sizeB = parseDouble(list.at(22), REGULATION::defaultColumnS, "jB", sv.i_param.sizeB);
        sv.j_param.t1 = parseDouble(list.at(23), REGULATION::defaultColumnS, "jT1", sv.i_param.t1);
        sv.j_param.t2 = parseDouble(list.at(24), REGULATION::defaultColumnS, "jT2", sv.i_param.t2);
        sv.j_param.H_T = parseDouble(list.at(25), REGULATION::defaultColumnS, "jH_T", sv.i_param.H_T);
        sv.j_param.B_T = parseDouble(list.at(26), REGULATION::defaultColumnS, "jB_T", sv.i_param.B_T);
        sv.j_param.t1_T = parseDouble(list.at(27), REGULATION::defaultColumnS, "jT1_T", sv.i_param.t1_T);
        sv.j_param.t2_T = parseDouble(list.at(28), REGULATION::defaultColumnS, "jT2_T", sv.i_param.t2_T);
        interpreter->sColumnList.append(sv);
    }
}

void ReadFromPsv::readSrcColumn(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultColumnSrc, "LABEL"));
        sv.memberType = DATATYPE::TPCOLUMN;
        sv.sectionType = STRUCTTYPE::STP_SRC;
        sv.startFloorID = interpreter->getOrCreateUuid(parseString(list.at(2), REGULATION::defaultColumnSrc, "FLOORFROM"));
        sv.endFloorID = interpreter->getOrCreateUuid(parseString(list.at(3), REGULATION::defaultColumnSrc, "FLOORTO"));
        sv.line_param.finishLoad = parseInt(list.at(4), REGULATION::defaultColumnSrc, "FINISHLOAD");
        sv.line_param.concMaterial = parseString(list.at(5), REGULATION::defaultColumnSrc, "CONC");
        sv.line_param.filledConcMaterial = parseString(list.at(6), REGULATION::defaultColumnSrc, "FILLEDCONC");
        sv.line_param.i_length = parseDouble(list.at(7), REGULATION::defaultColumnSrc, "iLENGTH");
        sv.line_param.basePlate = parseString(list.at(8), REGULATION::defaultColumnSrc, "BASELIST");
        sv.i_param.Dy = parseDouble(list.at(9), REGULATION::defaultColumnSrc, "iDY");
        sv.i_param.Dz = parseDouble(list.at(10), REGULATION::defaultColumnSrc, "iDZ");
        QString iMainBarArrange1 = parseString(list.at(11), REGULATION::defaultColumnSrc, "iMAINBAR1");
        QString iMainBarArrange2 = parseString(list.at(12), REGULATION::defaultColumnSrc, "iMAINBAR2");
        if (qAbs(sv.i_param.Dz) < 0.00001){
            ReadBarArrangement::readBarsColumnCirc(iMainBarArrange1, &sv.i_param.mainBarArrange1);
            sv.i_param.concType = ConcreteType::Type::CIRCLE;
        }else{
            ReadBarArrangement::readBarsColumn(iMainBarArrange1, &sv.i_param.mainBarArrange1);
            ReadBarArrangement::readBarsColumn(iMainBarArrange2, &sv.i_param.mainBarArrange2);
            sv.i_param.concType = ConcreteType::Type::RECTANGLE;
        }
        sv.i_param.mainBar1 = parseString(list.at(13), REGULATION::defaultColumnSrc, "iMAINBARPHI");
        sv.i_param.axialCenterY = parseDouble(list.at(14), REGULATION::defaultColumnSrc, "iAXIALCENTERY");
        sv.i_param.axialCenterZ = parseDouble(list.at(15), REGULATION::defaultColumnSrc, "iAXIALCENTERZ");
        sv.i_param.axialDy = parseDouble(list.at(16), REGULATION::defaultColumnSrc, "iAXIALDY");
        sv.i_param.axialDz = parseDouble(list.at(17), REGULATION::defaultColumnSrc, "iAXIALDZ");
        QString iAxialBarArrange1 = parseString(list.at(18), REGULATION::defaultColumnSrc, "iAXIALBAR1");
        QString iAxialBarArrange2 = parseString(list.at(19), REGULATION::defaultColumnSrc, "iAXIALBAR2");
        ReadBarArrangement::readBarsColumn(iAxialBarArrange1, &sv.i_param.axialBarArrange1);
        ReadBarArrangement::readBarsColumn(iAxialBarArrange2, &sv.i_param.axialBarArrange2);
        sv.i_param.axialBar = parseString(list.at(20), REGULATION::defaultColumnSrc, "iAXIALBARPHI");
        sv.i_param.hoopY = parseInt(list.at(21), REGULATION::defaultColumnSrc, "iHOOPY");
        sv.i_param.hoopZ = parseInt(list.at(22), REGULATION::defaultColumnSrc, "iHOOPZ");
        sv.i_param.hoopBar = parseString(list.at(23), REGULATION::defaultColumnSrc, "iHOOPPHI");
        sv.i_param.hoopPitch = parseInt(list.at(24), REGULATION::defaultColumnSrc, "iHOOPPITCH");
        sv.i_param.RK = parseDouble(list.at(25), REGULATION::defaultColumnSrc, "iRK");
        sv.i_param.RY = parseDouble(list.at(26), REGULATION::defaultColumnSrc, "iRY");
        sv.i_param.steelMaterial = parseString(list.at(27), REGULATION::defaultColumnSrc, "iSTEEL");
        QString iShapeListName = parseString(list.at(28), REGULATION::defaultColumnSrc, "iSHAPELISTNAME");
        sv.i_param.steelType = SteelType::stringToEnum(iShapeListName);
        sv.i_param.standardName = parseString(list.at(29), REGULATION::defaultColumnSrc, "iSTANDARDSHAPESTEEL");
        sv.i_param.sizeH = parseDouble(list.at(30), REGULATION::defaultColumnSrc, "iH");
        sv.i_param.sizeB = parseDouble(list.at(31), REGULATION::defaultColumnSrc, "iB");
        sv.i_param.t1 = parseDouble(list.at(32), REGULATION::defaultColumnSrc, "iT1");
        sv.i_param.t2 = parseDouble(list.at(33), REGULATION::defaultColumnSrc, "iT2");
        sv.i_param.cY = parseDouble(list.at(34), REGULATION::defaultColumnSrc, "iCOODINATEY");
        sv.i_param.cZ = parseDouble(list.at(35), REGULATION::defaultColumnSrc, "iCOODINATEZ");
        QString iShapeListName_T = parseString(list.at(36), REGULATION::defaultColumnSrc, "iSHAPELISTNAME_T");
        sv.i_param.steelType_T = SteelType::stringToEnum(iShapeListName_T);
        sv.i_param.standardName_T = parseString(list.at(37), REGULATION::defaultColumnSrc, "iSTANDARDSHAPESTEEL_T");
        sv.i_param.H_T = parseDouble(list.at(38), REGULATION::defaultColumnSrc, "iH_T");
        sv.i_param.B_T = parseDouble(list.at(39), REGULATION::defaultColumnSrc, "iB_T");
        sv.i_param.t1_T = parseDouble(list.at(40), REGULATION::defaultColumnSrc, "iT1_T");
        sv.i_param.t2_T = parseDouble(list.at(41), REGULATION::defaultColumnSrc, "iT2_T");
        sv.i_param.cY_T = parseDouble(list.at(42), REGULATION::defaultColumnSrc, "iCOODINATEY_T");
        sv.i_param.cZ_T = parseDouble(list.at(43), REGULATION::defaultColumnSrc, "iCOODINATEZ_T");

        sv.j_param.Dy = parseDouble(list.at(44), REGULATION::defaultColumnSrc, "jDY", sv.i_param.Dy);
        sv.j_param.Dz = parseDouble(list.at(45), REGULATION::defaultColumnSrc, "jDZ", sv.i_param.Dz);
        QString jMainBarArrange1 = parseString(list.at(46), REGULATION::defaultColumnSrc, "jMAINBAR1", iMainBarArrange1);
        if (qAbs(sv.j_param.Dz) < 0.00001){
            ReadBarArrangement::readBarsColumnCirc(jMainBarArrange1, &sv.j_param.mainBarArrange1);
            sv.j_param.concType = ConcreteType::Type::CIRCLE;
        }else{
            QString jMainBarArrange2 = parseString(list.at(47), REGULATION::defaultColumnSrc, "jMAINBAR2", iMainBarArrange2);
            ReadBarArrangement::readBarsColumn(jMainBarArrange1, &sv.j_param.mainBarArrange1);
            ReadBarArrangement::readBarsColumn(jMainBarArrange2, &sv.j_param.mainBarArrange2);
            sv.j_param.concType = ConcreteType::Type::RECTANGLE;
        }
        sv.j_param.mainBar1 = parseString(list.at(48), REGULATION::defaultColumnSrc, "jMAINBARPHI", sv.i_param.mainBar1);
        sv.j_param.axialCenterY = parseDouble(list.at(49), REGULATION::defaultColumnSrc, "jAXIALCENTERY", sv.i_param.axialCenterY);
        sv.j_param.axialCenterZ = parseDouble(list.at(50), REGULATION::defaultColumnSrc, "jAXIALCENTERZ", sv.i_param.axialCenterZ);
        sv.j_param.axialDy = parseDouble(list.at(51), REGULATION::defaultColumnSrc, "jAXIALDY", sv.i_param.axialDy);
        sv.j_param.axialDz = parseDouble(list.at(52), REGULATION::defaultColumnSrc, "jAXIALDZ", sv.i_param.axialDz);
        QString jAxialBarArrange1 = parseString(list.at(53), REGULATION::defaultColumnSrc, "jAXIALBAR1", iAxialBarArrange1);
        ReadBarArrangement::readBarsColumn(jAxialBarArrange1, &sv.j_param.axialBarArrange1);
        QString jAxialBarArrange2 = parseString(list.at(54), REGULATION::defaultColumnSrc, "jAXIALBAR2", iAxialBarArrange2);
        ReadBarArrangement::readBarsColumn(jAxialBarArrange2, &sv.j_param.axialBarArrange2);
        sv.j_param.axialBar = parseString(list.at(55), REGULATION::defaultColumnSrc, "jAXIALBARPHI", sv.i_param.axialBar);
        sv.j_param.hoopY = parseInt(list.at(56), REGULATION::defaultColumnSrc, "jHOOPY", sv.i_param.hoopY);
        sv.j_param.hoopZ = parseInt(list.at(57), REGULATION::defaultColumnSrc, "jHOOPZ", sv.i_param.hoopZ);
        sv.j_param.hoopBar = parseString(list.at(58), REGULATION::defaultColumnSrc, "jHOOPPHI", sv.i_param.hoopBar);
        sv.j_param.hoopPitch = parseInt(list.at(59), REGULATION::defaultColumnSrc, "jHOOPPITCH", sv.i_param.hoopPitch);
        sv.j_param.RK = parseDouble(list.at(60), REGULATION::defaultColumnSrc, "jRK", sv.i_param.RK);
        sv.j_param.RY = parseDouble(list.at(61), REGULATION::defaultColumnSrc, "jRY", sv.i_param.RY);
        sv.j_param.steelMaterial = parseString(list.at(62), REGULATION::defaultColumnSrc, "jSTEEL", sv.i_param.steelMaterial);
        sv.j_param.steelType = SteelType::stringToEnum(parseString(list.at(63), REGULATION::defaultColumnSrc, "jSHAPELISTNAME", iShapeListName));
        sv.j_param.standardName = parseString(list.at(64), REGULATION::defaultColumnSrc, "jSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.j_param.sizeH = parseDouble(list.at(65), REGULATION::defaultColumnSrc, "jH", sv.i_param.sizeH);
        sv.j_param.sizeB =  parseDouble(list.at(66), REGULATION::defaultColumnSrc, "jB", sv.i_param.sizeB);
        sv.j_param.t1 = parseDouble(list.at(67), REGULATION::defaultColumnSrc, "jT1", sv.i_param.t1);
        sv.j_param.t2 = parseDouble(list.at(68), REGULATION::defaultColumnSrc, "jT2", sv.i_param.t2);
        sv.j_param.cY = parseDouble(list.at(69), REGULATION::defaultColumnSrc, "jCOODINATEY", sv.i_param.cY);
        sv.j_param.cZ = parseDouble(list.at(70), REGULATION::defaultColumnSrc, "jCOODINATEZ", sv.i_param.cZ);
        sv.j_param.steelType_T = SteelType::stringToEnum(parseString(list.at(71), REGULATION::defaultColumnSrc, "jSHAPELISTNAME_T", iShapeListName_T));
        sv.j_param.standardName_T = parseString(list.at(72), REGULATION::defaultColumnSrc, "jSTANDARDSHAPESTEEL_T", sv.i_param.standardName_T);
        sv.j_param.H_T = parseDouble(list.at(73), REGULATION::defaultColumnSrc, "jH_T", sv.i_param.H_T);
        sv.j_param.B_T = parseDouble(list.at(74), REGULATION::defaultColumnSrc, "jB_T", sv.i_param.B_T);
        sv.j_param.t1_T = parseDouble(list.at(75), REGULATION::defaultColumnSrc, "jT1_T", sv.i_param.t1_T);
        sv.j_param.t2_T = parseDouble(list.at(76), REGULATION::defaultColumnSrc, "jT2_T", sv.i_param.t2_T);
        sv.j_param.cY_T = parseDouble(list.at(77), REGULATION::defaultColumnSrc, "jCOODINATEY_T", sv.i_param.cY_T);
        sv.j_param.cZ_T = parseDouble(list.at(78), REGULATION::defaultColumnSrc, "jCOODINATEZ_T", sv.i_param.cZ_T);

        interpreter->srcColumnList.append(sv);
    }
}

void ReadFromPsv::readCftColumn(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultColumnCft, "LABEL"));
        sv.memberType = DATATYPE::TPCOLUMN;
        sv.sectionType = STRUCTTYPE::STP_CFT;
        sv.startFloorID = interpreter->getOrCreateUuid(parseString(list.at(2), REGULATION::defaultColumnCft, "FLOORFROM"));
        sv.endFloorID = interpreter->getOrCreateUuid(parseString(list.at(3), REGULATION::defaultColumnCft, "FLOORTO"));
        sv.line_param.finishLoad = parseInt(list.at(4), REGULATION::defaultColumnCft, "FINISHLOAD");
        sv.line_param.filledConcMaterial = parseString(list.at(5), REGULATION::defaultColumnCft, "FILLEDCONC");
        sv.line_param.i_length = parseDouble(list.at(6), REGULATION::defaultColumnCft, "iLENGTH");
        sv.line_param.basePlate = parseString(list.at(7), REGULATION::defaultColumnCft, "BASELIST");
        sv.i_param.steelMaterial = parseString(list.at(8), REGULATION::defaultColumnCft, "iSTEEL");
        QString iShapeListName = parseString(list.at(9), REGULATION::defaultColumnCft, "iSHAPELISTNAME");
        sv.i_param.steelType = SteelType::stringToEnum(iShapeListName);
        sv.i_param.standardName = parseString(list.at(10), REGULATION::defaultColumnCft, "iSTANDARDSHAPESTEEL");
        sv.i_param.sizeH = parseDouble(list.at(11), REGULATION::defaultColumnCft, "iH");
        sv.i_param.sizeB = parseDouble(list.at(12), REGULATION::defaultColumnCft, "iB");
        sv.i_param.t1 = parseDouble(list.at(13), REGULATION::defaultColumnCft, "iT1");
        sv.i_param.t2 = parseDouble(list.at(14), REGULATION::defaultColumnCft, "iT2");

        sv.j_param.steelMaterial = parseString(list.at(15), REGULATION::defaultColumnCft, "jSTEEL", sv.i_param.steelMaterial);
        sv.j_param.steelType = SteelType::stringToEnum(parseString(list.at(16), REGULATION::defaultColumnCft, "jSHAPELISTNAME", iShapeListName));
        sv.j_param.standardName = parseString(list.at(17), REGULATION::defaultColumnCft, "jSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.j_param.sizeH = parseDouble(list.at(18), REGULATION::defaultColumnCft, "jH", sv.i_param.sizeH);
        sv.j_param.sizeB = parseDouble(list.at(19), REGULATION::defaultColumnCft, "jB", sv.i_param.sizeB);
        sv.j_param.t1 = parseDouble(list.at(20), REGULATION::defaultColumnCft, "jT1", sv.i_param.t1);
        sv.j_param.t2 = parseDouble(list.at(21), REGULATION::defaultColumnCft, "jT2", sv.i_param.t2);
        interpreter->cftColumnList.append(sv);
    }
}

void ReadFromPsv::readRcGirder(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultGirderRc, "LABEL"));
        sv.memberType = DATATYPE::TPGIRDER;
        sv.sectionType = STRUCTTYPE::STP_RC;
        sv.startFloorID = interpreter->getOrCreateUuid(parseString(list.at(2), REGULATION::defaultGirderRc, "FLOORFROM"));
        sv.endFloorID = interpreter->getOrCreateUuid(parseString(list.at(3), REGULATION::defaultGirderRc, "FLOORTO"));
        sv.line_param.isCantilever = parseString(list.at(4), REGULATION::defaultGirderRc, "CANTILEVER") != u8"一般";
        sv.line_param.finishLoad = parseInt(list.at(5), REGULATION::defaultGirderRc, "FINISHLOAD");
        sv.line_param.concMaterial = parseString(list.at(6), REGULATION::defaultGirderRc, "CONC");

        sv.i_param.Dy = parseDouble(list.at(7), REGULATION::defaultGirderRc, "iDY");
        sv.i_param.Dz = parseDouble(list.at(8), REGULATION::defaultGirderRc, "iDZ");
        QString iMainBarArrange1 = parseString(list.at(9), REGULATION::defaultGirderRc, "iMAINBAR1");
        ReadBarArrangement::readBarsGirder(iMainBarArrange1, &sv.i_param.mainBarArrange1);
        QString iMainBarArrange2 = parseString(list.at(10), REGULATION::defaultGirderRc, "iMAINBAR2");
        ReadBarArrangement::readBarsGirder(iMainBarArrange2, &sv.i_param.mainBarArrange2);
        sv.i_param.concType = ConcreteType::Type::RECTANGLE;
        sv.i_param.mainBar1 = parseString(list.at(11), REGULATION::defaultGirderRc, "iMAINBARPHI1");
        sv.i_param.mainBar2 = parseString(list.at(12), REGULATION::defaultGirderRc, "iMAINBARPHI2");
        sv.i_param.hoopZ = parseInt(list.at(13), REGULATION::defaultGirderRc, "iHOOPZ");
        sv.i_param.hoopBar = parseString(list.at(14), REGULATION::defaultGirderRc, "iHOOPPHI");
        sv.i_param.hoopPitch = parseInt(list.at(15), REGULATION::defaultGirderRc, "iHOOPPITCH");
        sv.i_param.RK = parseDouble(list.at(16), REGULATION::defaultGirderRc, "iRK");
        sv.i_param.RKu = parseDouble(list.at(17), REGULATION::defaultGirderRc, "iRKU");
        sv.i_param.RKb = parseDouble(list.at(18), REGULATION::defaultGirderRc, "iRKB");
        sv.i_param.RY = parseDouble(list.at(19), REGULATION::defaultGirderRc, "iRY");

        sv.c_param.Dy = parseDouble(list.at(20), REGULATION::defaultGirderRc, "mDY", sv.i_param.Dy);
        sv.c_param.Dz = parseDouble(list.at(21), REGULATION::defaultGirderRc, "mDY", sv.i_param.Dz);
        ReadBarArrangement::readBarsGirder(parseString(list.at(22), REGULATION::defaultGirderRc, "mMAINBAR1", iMainBarArrange1), &sv.c_param.mainBarArrange1);
        ReadBarArrangement::readBarsGirder(parseString(list.at(23), REGULATION::defaultGirderRc, "mMAINBAR2", iMainBarArrange2), &sv.c_param.mainBarArrange2);
        sv.c_param.concType = ConcreteType::Type::RECTANGLE;
        sv.c_param.mainBar1 = parseString(list.at(24), REGULATION::defaultGirderRc, "mMAINBARPHI1", sv.i_param.mainBar1);
        sv.c_param.mainBar2 = parseString(list.at(25), REGULATION::defaultGirderRc, "mMAINBARPHI2", sv.i_param.mainBar2);
        sv.c_param.hoopZ = parseInt(list.at(26), REGULATION::defaultGirderRc, "mHOOPZ", sv.i_param.hoopZ);
        sv.c_param.hoopBar = parseString(list.at(27), REGULATION::defaultGirderRc, "mHOOPPHI", sv.i_param.hoopBar);
        sv.c_param.hoopPitch = parseInt(list.at(28), REGULATION::defaultGirderRc, "mHOOPPITCH", sv.i_param.hoopPitch);
        sv.c_param.RK = parseDouble(list.at(29), REGULATION::defaultGirderRc, "mRK", sv.i_param.RK);
        sv.c_param.RKu = parseDouble(list.at(30), REGULATION::defaultGirderRc, "mRKU", sv.i_param.RKu);
        sv.c_param.RKb = parseDouble(list.at(31), REGULATION::defaultGirderRc, "mRKB", sv.i_param.RKb);
        sv.c_param.RY = parseDouble(list.at(32), REGULATION::defaultGirderRc, "mRY", sv.i_param.RY);

        sv.j_param.Dy = parseDouble(list.at(33), REGULATION::defaultGirderRc, "jDY", sv.i_param.Dy);
        sv.j_param.Dz = parseDouble(list.at(34), REGULATION::defaultGirderRc, "jDZ", sv.i_param.Dz);
        ReadBarArrangement::readBarsGirder(parseString(list.at(35), REGULATION::defaultGirderRc, "jMAINBAR1", iMainBarArrange1), &sv.c_param.mainBarArrange1);
        ReadBarArrangement::readBarsGirder(parseString(list.at(36), REGULATION::defaultGirderRc, "jMAINBAR2", iMainBarArrange2), &sv.c_param.mainBarArrange2);
        sv.j_param.concType = ConcreteType::Type::RECTANGLE;
        sv.j_param.mainBar1 = parseString(list.at(37), REGULATION::defaultGirderRc, "jMAINBARPHI1", sv.i_param.mainBar1);
        sv.j_param.mainBar2 = parseString(list.at(38), REGULATION::defaultGirderRc, "jMAINBARPHI2", sv.i_param.mainBar2);
        sv.j_param.hoopZ = parseInt(list.at(39), REGULATION::defaultGirderRc, "jHOOPZ", sv.i_param.hoopZ);
        sv.j_param.hoopBar = parseString(list.at(40), REGULATION::defaultGirderRc, "jHOOPPHI", sv.i_param.hoopBar);
        sv.j_param.hoopPitch = parseInt(list.at(41), REGULATION::defaultGirderRc, "jHOOPPITCH", sv.i_param.hoopPitch);
        sv.j_param.RK = parseDouble(list.at(42), REGULATION::defaultGirderRc, "jRK", sv.i_param.RK);
        sv.j_param.RKu = parseDouble(list.at(43), REGULATION::defaultGirderRc, "jRKU", sv.i_param.RKu);
        sv.j_param.RKb = parseDouble(list.at(44), REGULATION::defaultGirderRc, "jRKB", sv.i_param.RKb);
        sv.j_param.RY = parseDouble(list.at(45), REGULATION::defaultGirderRc, "jRY", sv.i_param.RY);
        interpreter->rcGirderList.append(sv);

    }
}

void ReadFromPsv::readSGirder(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultGirderS, "LABEL"));
        sv.memberType = DATATYPE::TPGIRDER;
        sv.sectionType = STRUCTTYPE::STP_S;
        sv.startFloorID = interpreter->getOrCreateUuid(parseString(list.at(2), REGULATION::defaultGirderS, "FLOORFROM"));
        sv.endFloorID = interpreter->getOrCreateUuid(parseString(list.at(3), REGULATION::defaultGirderS, "FLOORTO"));
        sv.line_param.isCantilever = parseString(list.at(4), REGULATION::defaultGirderS, "CANTILEVER") != u8"一般";
        sv.line_param.finishLoad = parseInt(list.at(5), REGULATION::defaultGirderS, "FINISHLOAD");
        sv.line_param.i_length = parseDouble(list.at(6), REGULATION::defaultGirderS, "iLENGTH");
        sv.line_param.j_length = parseDouble(list.at(7), REGULATION::defaultGirderS, "jLENGTH");
        sv.line_param.i_hasStiffener = parseString(list.at(8), REGULATION::defaultGirderS, "iENDSTIFFENER") == u8"無し";
        sv.line_param.j_hasStiffener = parseString(list.at(9), REGULATION::defaultGirderS, "jENDSTIFFENER") == u8"無し";

        sv.i_param.steelMaterial = parseString(list.at(10), REGULATION::defaultGirderS, "iSTEEL");
        sv.i_param.steelWebMaterial = parseString(list.at(11), REGULATION::defaultGirderS, "iSTEELWEB");
        QString iSteelType = parseString(list.at(12), REGULATION::defaultGirderS, "iSHAPELISTNAME");
        sv.i_param.steelType = SteelType::stringToEnum(iSteelType);
        sv.i_param.standardName = parseString(list.at(13), REGULATION::defaultGirderS, "iSTANDARDSHAPESTEEL");
        sv.i_param.sizeH = parseDouble(list.at(14), REGULATION::defaultGirderS, "iH");
        sv.i_param.sizeB = parseDouble(list.at(15), REGULATION::defaultGirderS, "iB");
        sv.i_param.t1 = parseDouble(list.at(16), REGULATION::defaultGirderS, "iT1");
        sv.i_param.t2 = parseDouble(list.at(17), REGULATION::defaultGirderS, "iT2");

        sv.c_param.steelMaterial = parseString(list.at(18), REGULATION::defaultGirderS, "mSTEEL", sv.i_param.steelMaterial);
        sv.c_param.steelWebMaterial = parseString(list.at(19), REGULATION::defaultGirderS, "mSTEELWEB", sv.i_param.steelWebMaterial);
        sv.c_param.steelType = SteelType::stringToEnum(parseString(list.at(20), REGULATION::defaultGirderS, "mSHAPELISTNAME", iSteelType));
        sv.c_param.standardName = parseString(list.at(21), REGULATION::defaultGirderS, "mSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.c_param.sizeH = parseDouble(list.at(22), REGULATION::defaultGirderS, "mH", sv.i_param.sizeH);
        sv.c_param.sizeB = parseDouble(list.at(23), REGULATION::defaultGirderS, "mB", sv.i_param.sizeB);
        sv.c_param.t1 = parseDouble(list.at(24), REGULATION::defaultGirderS, "mT1", sv.i_param.t1);
        sv.c_param.t2 = parseDouble(list.at(25), REGULATION::defaultGirderS, "mT2", sv.i_param.t2);

        sv.j_param.steelMaterial = parseString(list.at(26), REGULATION::defaultGirderS, "jSTEEL", sv.i_param.steelMaterial);
        sv.j_param.steelWebMaterial = parseString(list.at(27), REGULATION::defaultGirderS, "jSTEELWEB", sv.i_param.steelWebMaterial);
        sv.j_param.steelType = SteelType::stringToEnum(parseString(list.at(28), REGULATION::defaultGirderS, "jSHAPELISTNAME", iSteelType));
        sv.j_param.standardName = parseString(list.at(29), REGULATION::defaultGirderS, "jSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.j_param.sizeH = parseDouble(list.at(30), REGULATION::defaultGirderS, "jH", sv.i_param.sizeH);
        sv.j_param.sizeB = parseDouble(list.at(31), REGULATION::defaultGirderS, "jB", sv.i_param.sizeB);
        sv.j_param.t1 = parseDouble(list.at(32), REGULATION::defaultGirderS, "jT1", sv.i_param.t1);
        sv.j_param.t2 = parseDouble(list.at(33), REGULATION::defaultGirderS, "jT2", sv.i_param.t2);
        interpreter->sGirderList.append(sv);
    }
}

void ReadFromPsv::readSrcGirder(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultGirderSrc, "LABEL"));
        sv.memberType = DATATYPE::TPGIRDER;
        sv.sectionType = STRUCTTYPE::STP_SRC;
        sv.startFloorID = interpreter->getOrCreateUuid(parseString(list.at(2), REGULATION::defaultGirderSrc, "FLOORFROM"));
        sv.endFloorID = interpreter->getOrCreateUuid(parseString(list.at(3), REGULATION::defaultGirderSrc, "FLOORTO"));
        sv.line_param.isCantilever = parseString(list.at(4), REGULATION::defaultGirderSrc, "CANTILEVER") != u8"一般";
        sv.line_param.finishLoad = parseInt(list.at(5), REGULATION::defaultGirderSrc, "FINISHLOAD");
        sv.line_param.concMaterial = parseString(list.at(6), REGULATION::defaultGirderSrc, "CONC");
        sv.line_param.i_length = parseDouble(list.at(7), REGULATION::defaultGirderSrc, "iLENGTH");
        sv.line_param.j_length = parseDouble(list.at(8), REGULATION::defaultGirderSrc, "jLENGTH");

        sv.i_param.Dy = parseDouble(list.at(9), REGULATION::defaultGirderSrc, "iDY");
        sv.i_param.Dz = parseDouble(list.at(10), REGULATION::defaultGirderSrc, "iDZ");
        QString iMainBarArrange1 = parseString(list.at(11), REGULATION::defaultGirderSrc, "iMAINBAR1");
        ReadBarArrangement::readBarsGirder(iMainBarArrange1, &sv.i_param.mainBarArrange1);
        QString iMainBarArrange2 = parseString(list.at(12), REGULATION::defaultGirderSrc, "iMAINBAR2");
        ReadBarArrangement::readBarsGirder(iMainBarArrange2, &sv.i_param.mainBarArrange2);
        sv.i_param.concType = ConcreteType::Type::RECTANGLE;
        sv.i_param.mainBar1 = parseString(list.at(13), REGULATION::defaultGirderSrc, "iMAINBARPHI1");
        sv.i_param.mainBar2 = parseString(list.at(14), REGULATION::defaultGirderSrc, "iMAINBARPHI2");
        sv.i_param.hoopZ = parseInt(list.at(15), REGULATION::defaultGirderSrc, "iHOOPZ");
        sv.i_param.hoopBar = parseString(list.at(16), REGULATION::defaultGirderSrc, "iHOOPPHI");
        sv.i_param.hoopPitch = parseInt(list.at(17), REGULATION::defaultGirderSrc, "iHOOPPITCH");
        sv.i_param.RK = parseDouble(list.at(18), REGULATION::defaultGirderSrc, "iRK");
        sv.i_param.RKu = parseDouble(list.at(19), REGULATION::defaultGirderSrc, "iRKU");
        sv.i_param.RKb = parseDouble(list.at(20), REGULATION::defaultGirderSrc, "iRKB");
        sv.i_param.RY = parseDouble(list.at(21), REGULATION::defaultGirderSrc, "iRY");
        sv.i_param.RY1 = parseDouble(list.at(22), REGULATION::defaultGirderSrc, "iRY1");
        sv.i_param.steelMaterial = parseString(list.at(23), REGULATION::defaultGirderSrc, "iSTEEL");
        sv.i_param.steelWebMaterial = parseString(list.at(24), REGULATION::defaultGirderSrc, "iSTEELWEB");
        QString iSteelType = parseString(list.at(25), REGULATION::defaultGirderSrc, "iSHAPELISTNAME");
        sv.i_param.steelType = SteelType::stringToEnum(iSteelType);
        sv.i_param.standardName = parseString(list.at(26), REGULATION::defaultGirderSrc, "iSTANDARDSHAPESTEEL");
        sv.i_param.sizeH = parseDouble(list.at(27), REGULATION::defaultGirderSrc, "iH");
        sv.i_param.sizeB = parseDouble(list.at(28), REGULATION::defaultGirderSrc, "iB");
        sv.i_param.t1 = parseDouble(list.at(29), REGULATION::defaultGirderSrc, "iT1");
        sv.i_param.t2 = parseDouble(list.at(30), REGULATION::defaultGirderSrc, "iT2");
        sv.i_param.cY = parseDouble(list.at(31), REGULATION::defaultGirderSrc, "iCOODINATEY");
        sv.i_param.cZ = parseDouble(list.at(32), REGULATION::defaultGirderSrc, "iCOODINATEZ");

        sv.c_param.Dy = parseDouble(list.at(33), REGULATION::defaultGirderSrc, "mDY", sv.i_param.Dy);
        sv.c_param.Dz = parseDouble(list.at(34), REGULATION::defaultGirderSrc, "mDZ", sv.i_param.Dz);
        ReadBarArrangement::readBarsGirder(parseString(list.at(35), REGULATION::defaultGirderSrc, "mMAINBAR1", iMainBarArrange1), &sv.c_param.mainBarArrange1);
        ReadBarArrangement::readBarsGirder(parseString(list.at(36), REGULATION::defaultGirderSrc, "mMAINBAR2", iMainBarArrange2), &sv.c_param.mainBarArrange2);
        sv.c_param.concType = ConcreteType::Type::RECTANGLE;
        sv.c_param.mainBar1 = parseString(list.at(37), REGULATION::defaultGirderSrc, "mMAINBARPHI1", sv.i_param.mainBar1);
        sv.c_param.mainBar2 = parseString(list.at(38), REGULATION::defaultGirderSrc, "mMAINBARPHI2", sv.i_param.mainBar2);
        sv.c_param.hoopZ = parseInt(list.at(39), REGULATION::defaultGirderSrc, "mHOOPZ", sv.i_param.hoopZ);
        sv.c_param.hoopBar = parseString(list.at(40), REGULATION::defaultGirderSrc, "mHOOPPHI", sv.i_param.hoopBar);
        sv.c_param.hoopPitch = parseInt(list.at(41), REGULATION::defaultGirderSrc, "mHOOPPITCH", sv.i_param.hoopPitch);
        sv.c_param.RK = parseDouble(list.at(42), REGULATION::defaultGirderSrc, "mRK", sv.i_param.RK);
        sv.c_param.RKu = parseDouble(list.at(43), REGULATION::defaultGirderSrc, "mRKU", sv.i_param.RKu);
        sv.c_param.RKb = parseDouble(list.at(44), REGULATION::defaultGirderSrc, "mRKB", sv.i_param.RKb);
        sv.c_param.RY = parseDouble(list.at(45), REGULATION::defaultGirderSrc, "mRY", sv.i_param.RY);
        sv.c_param.RY1 = parseDouble(list.at(46), REGULATION::defaultGirderSrc, "mRY1", sv.i_param.RY1);
        sv.c_param.steelMaterial = parseString(list.at(47), REGULATION::defaultGirderSrc, "mSTEEL", sv.i_param.steelMaterial);
        sv.c_param.steelWebMaterial = parseString(list.at(48), REGULATION::defaultGirderSrc, "mSTEELWEB", sv.i_param.steelWebMaterial);
        sv.c_param.steelType = SteelType::stringToEnum(parseString(list.at(49), REGULATION::defaultGirderSrc, "mSHAPELISTNAME", iSteelType));
        sv.c_param.standardName = parseString(list.at(50), REGULATION::defaultGirderSrc, "mSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.c_param.sizeH = parseDouble(list.at(51), REGULATION::defaultGirderSrc, "mH", sv.i_param.sizeH);
        sv.c_param.sizeB = parseDouble(list.at(52), REGULATION::defaultGirderSrc, "mB", sv.i_param.sizeB);
        sv.c_param.t1 = parseDouble(list.at(53), REGULATION::defaultGirderSrc, "mT1", sv.i_param.t1);
        sv.c_param.t2 = parseDouble(list.at(54), REGULATION::defaultGirderSrc, "mT2", sv.i_param.t2);
        sv.c_param.cY = parseDouble(list.at(55), REGULATION::defaultGirderSrc, "mCOODINATEY", sv.i_param.cY);
        sv.c_param.cZ = parseDouble(list.at(56), REGULATION::defaultGirderSrc, "mCOODINATEZ", sv.i_param.cZ);

        sv.j_param.Dy = parseDouble(list.at(57), REGULATION::defaultGirderSrc, "jDY", sv.i_param.Dy);
        sv.j_param.Dz = parseDouble(list.at(58), REGULATION::defaultGirderSrc, "jDZ", sv.i_param.Dz);
        ReadBarArrangement::readBarsGirder(parseString(list.at(59), REGULATION::defaultGirderSrc, "jMAINBAR1", iMainBarArrange1), &sv.j_param.mainBarArrange1);
        ReadBarArrangement::readBarsGirder(parseString(list.at(60), REGULATION::defaultGirderSrc, "jMAINBAR2", iMainBarArrange2), &sv.j_param.mainBarArrange2);
        sv.j_param.concType = ConcreteType::Type::RECTANGLE;
        sv.j_param.mainBar1 = parseString(list.at(61), REGULATION::defaultGirderSrc, "jMAINBARPHI1", sv.i_param.mainBar1);
        sv.j_param.mainBar2 = parseString(list.at(62), REGULATION::defaultGirderSrc, "jMAINBARPHI2", sv.i_param.mainBar2);
        sv.j_param.hoopZ = parseInt(list.at(63), REGULATION::defaultGirderSrc, "jHOOPZ", sv.i_param.hoopZ);
        sv.j_param.hoopBar = parseString(list.at(64), REGULATION::defaultGirderSrc, "jHOOPPHI", sv.i_param.hoopBar);
        sv.j_param.hoopPitch = parseInt(list.at(65), REGULATION::defaultGirderSrc, "jHOOPPITCH", sv.i_param.hoopPitch);
        sv.j_param.RK = parseDouble(list.at(66), REGULATION::defaultGirderSrc, "jRK", sv.i_param.RK);
        sv.j_param.RKu = parseDouble(list.at(67), REGULATION::defaultGirderSrc, "jRKU", sv.i_param.RKu);
        sv.j_param.RKb = parseDouble(list.at(68), REGULATION::defaultGirderSrc, "jRKB", sv.i_param.RKb);
        sv.j_param.RY = parseDouble(list.at(69), REGULATION::defaultGirderSrc, "jRY", sv.i_param.RY);
        sv.j_param.RY1 = parseDouble(list.at(70), REGULATION::defaultGirderSrc, "jRY1", sv.i_param.RY1);
        sv.j_param.steelMaterial = parseString(list.at(71), REGULATION::defaultGirderSrc, "jSTEEL", sv.i_param.steelMaterial);
        sv.j_param.steelWebMaterial = parseString(list.at(72), REGULATION::defaultGirderSrc, "jSTEELWEB", sv.i_param.steelWebMaterial);
        sv.j_param.steelType = SteelType::stringToEnum(parseString(list.at(73), REGULATION::defaultGirderSrc, "jSHAPELISTNAME", iSteelType));
        sv.j_param.standardName = parseString(list.at(74), REGULATION::defaultGirderSrc, "jSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.j_param.sizeH = parseDouble(list.at(75), REGULATION::defaultGirderSrc, "jH", sv.i_param.sizeH);
        sv.j_param.sizeB = parseDouble(list.at(76), REGULATION::defaultGirderSrc, "jB", sv.i_param.sizeB);
        sv.j_param.t1 = parseDouble(list.at(77), REGULATION::defaultGirderSrc, "jT1", sv.i_param.t1);
        sv.j_param.t2 = parseDouble(list.at(78), REGULATION::defaultGirderSrc, "jT2", sv.i_param.t2);
        sv.j_param.cY = parseDouble(list.at(79), REGULATION::defaultGirderSrc, "jCOODINATEY", sv.i_param.cY);
        sv.j_param.cZ = parseDouble(list.at(80), REGULATION::defaultGirderSrc, "jCOODINATEZ", sv.i_param.cZ);

        interpreter->srcGirderList.append(sv);
    }
}

void ReadFromPsv::readSBrace(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultBraceS, "LABEL"));
        sv.memberType = DATATYPE::TPBRACE;
        sv.sectionType = STRUCTTYPE::STP_S;
        sv.line_param.finishLoad = parseInt(list.at(2), REGULATION::defaultBraceS, "FINISHLOAD");
        sv.line_param.hasCompResistance = parseString(list.at(3), REGULATION::defaultBraceS, "COMPRESSIVERESISTANCE") == u8"圧縮引張共";
        sv.line_param.i_length = parseDouble(list.at(4), REGULATION::defaultBraceS, "iLENGTH");
        sv.line_param.j_length = parseDouble(list.at(5), REGULATION::defaultBraceS, "jLENGTH");
        sv.line_param.direct_area = parseDouble(list.at(6), REGULATION::defaultBraceS, "AREA");
        sv.line_param.direct_effectiveArea = parseDouble(list.at(7), REGULATION::defaultBraceS, "EFFECTIVEAREA");
        sv.line_param.direct_slenderRatio = parseDouble(list.at(8), REGULATION::defaultBraceS, "SLENDERNESSRATIO");
        sv.line_param.direct_unitmass = parseDouble(list.at(9), REGULATION::defaultBraceS, "UNITMASS");
        //double displayHeight = list.at(10).toDouble();
        //double displayWidth = list.at(11).toDouble();

        sv.i_param.steelMaterial = parseString(list.at(12), REGULATION::defaultBraceS, "iSTEEL");
        QString iSteelType = parseString(list.at(13), REGULATION::defaultBraceS, "iSHAPELISTNAME");
        sv.i_param.steelType = SteelType::stringToEnum(iSteelType);
        sv.i_param.standardName = parseString(list.at(14), REGULATION::defaultBraceS, "iSTANDARDSHAPESTEEL");
        sv.i_param.sizeH = parseDouble(list.at(15), REGULATION::defaultBraceS, "iH");
        sv.i_param.sizeB = parseDouble(list.at(16), REGULATION::defaultBraceS, "iB");
        sv.i_param.t1 = parseDouble(list.at(17), REGULATION::defaultBraceS, "iT1");
        sv.i_param.t2 = parseDouble(list.at(18), REGULATION::defaultBraceS, "iT2");

        sv.c_param.steelMaterial = parseString(list.at(19), REGULATION::defaultBraceS, "mSTEEL", sv.i_param.steelMaterial);
        sv.c_param.steelType = SteelType::stringToEnum(parseString(list.at(20), REGULATION::defaultBraceS, "mSHAPELISTNAME", iSteelType));
        sv.c_param.standardName = parseString(list.at(21), REGULATION::defaultBraceS, "mSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.c_param.sizeH = parseDouble(list.at(22), REGULATION::defaultBraceS, "mH", sv.i_param.sizeH);
        sv.c_param.sizeB = parseDouble(list.at(23), REGULATION::defaultBraceS, "mB", sv.i_param.sizeB);
        sv.c_param.t1 = parseDouble(list.at(24), REGULATION::defaultBraceS, "mT1", sv.i_param.t1);
        sv.c_param.t2 = parseDouble(list.at(25), REGULATION::defaultBraceS, "mT2", sv.i_param.t2);

        sv.j_param.steelMaterial = parseString(list.at(26), REGULATION::defaultBraceS, "jSTEEL", sv.i_param.steelMaterial);
        sv.j_param.steelType = SteelType::stringToEnum(parseString(list.at(27), REGULATION::defaultBraceS, "jSHAPELISTNAME", iSteelType));
        sv.j_param.standardName = parseString(list.at(28), REGULATION::defaultBraceS, "jSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.j_param.sizeH = parseDouble(list.at(29), REGULATION::defaultBraceS, "jH", sv.i_param.sizeH);
        sv.j_param.sizeB = parseDouble(list.at(30), REGULATION::defaultBraceS, "jB", sv.i_param.sizeB);
        sv.j_param.t1 = parseDouble(list.at(31), REGULATION::defaultBraceS, "jT1", sv.i_param.t1);
        sv.j_param.t2 = parseDouble(list.at(32), REGULATION::defaultBraceS, "jT2", sv.i_param.t2);

        interpreter->sBraceList.append(sv);
    }
}

void ReadFromPsv::readRcBeam(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultBeamRc, "LABEL"));
        sv.memberType = DATATYPE::TPBEAM;
        sv.sectionType = STRUCTTYPE::STP_RC;
        sv.line_param.isCantilever = parseString(list.at(2), REGULATION::defaultBeamRc, "CANTILEVER") != u8"一般";
        sv.line_param.finishLoad = parseInt(list.at(3), REGULATION::defaultBeamRc, "FINISHLOAD");
        sv.line_param.concMaterial = parseString(list.at(4), REGULATION::defaultBeamRc, "CONC");
        sv.i_param.Dy = parseDouble(list.at(5), REGULATION::defaultBeamRc, "iDY");
        sv.i_param.Dz = parseDouble(list.at(6), REGULATION::defaultBeamRc, "iDZ");
        QString iMainBarArrange1 = parseString(list.at(7), REGULATION::defaultBeamRc, "iMAINBAR1");
        ReadBarArrangement::readBarsGirder(iMainBarArrange1, &sv.i_param.mainBarArrange1);
        QString iMainBarArrange2 = parseString(list.at(8), REGULATION::defaultBeamRc, "iMAINBAR2");
        ReadBarArrangement::readBarsGirder(iMainBarArrange2, &sv.i_param.mainBarArrange2);
        sv.i_param.concType = ConcreteType::Type::RECTANGLE;
        sv.i_param.mainBar1 = parseString(list.at(9), REGULATION::defaultBeamRc, "iMAINBARPHI1");
        sv.i_param.mainBar2 = parseString(list.at(10), REGULATION::defaultBeamRc, "iMAINBARPHI2");
        sv.i_param.hoopZ = parseInt(list.at(11), REGULATION::defaultBeamRc, "iHOOPZ");
        sv.i_param.hoopBar = parseString(list.at(12), REGULATION::defaultBeamRc, "iHOOPPHI");
        sv.i_param.hoopPitch = parseInt(list.at(13), REGULATION::defaultBeamRc, "iHOOPPITCH");
        sv.i_param.RK = parseDouble(list.at(14), REGULATION::defaultBeamRc, "iRK");
        sv.i_param.RKu = parseDouble(list.at(15), REGULATION::defaultBeamRc, "iRKU");
        sv.i_param.RKb = parseDouble(list.at(16), REGULATION::defaultBeamRc, "iRKB");
        sv.i_param.RY = parseDouble(list.at(17), REGULATION::defaultBeamRc, "iRY");

        sv.c_param.Dy = parseDouble(list.at(18), REGULATION::defaultBeamRc, "mDY", sv.i_param.Dy);
        sv.c_param.Dz = parseDouble(list.at(19), REGULATION::defaultBeamRc, "mDZ", sv.i_param.Dz);
        ReadBarArrangement::readBarsGirder(parseString(list.at(20), REGULATION::defaultBeamRc, "mMAINBAR1", iMainBarArrange1), &sv.c_param.mainBarArrange1);
        ReadBarArrangement::readBarsGirder(parseString(list.at(21), REGULATION::defaultBeamRc, "mMAINBAR2", iMainBarArrange2), &sv.c_param.mainBarArrange2);
        sv.c_param.concType = ConcreteType::Type::RECTANGLE;
        sv.c_param.mainBar1 = parseString(list.at(22), REGULATION::defaultBeamRc, "mMAINBARPHI1", sv.i_param.mainBar1);
        sv.c_param.mainBar2 = parseString(list.at(23), REGULATION::defaultBeamRc, "mMAINBARPHI2", sv.i_param.mainBar2);
        sv.c_param.hoopZ = parseInt(list.at(24), REGULATION::defaultBeamRc, "mHOOPZ", sv.i_param.hoopZ);
        sv.c_param.hoopBar = parseString(list.at(25), REGULATION::defaultBeamRc, "mHOOPPHI", sv.i_param.hoopBar);
        sv.c_param.hoopPitch = parseInt(list.at(26), REGULATION::defaultBeamRc, "mHOOPPITCH", sv.i_param.hoopPitch);
        sv.c_param.RK = parseDouble(list.at(27), REGULATION::defaultBeamRc, "mRK", sv.i_param.RK);
        sv.c_param.RKu = parseDouble(list.at(28), REGULATION::defaultBeamRc, "mRKU", sv.i_param.RKu);
        sv.c_param.RKb = parseDouble(list.at(29), REGULATION::defaultBeamRc, "mRKB", sv.i_param.RKb);
        sv.c_param.RY = parseDouble(list.at(30), REGULATION::defaultBeamRc, "mRY", sv.i_param.RY);

        sv.j_param.Dy = parseDouble(list.at(31), REGULATION::defaultBeamRc, "jDY", sv.i_param.Dy);
        sv.j_param.Dz = parseDouble(list.at(32), REGULATION::defaultBeamRc, "jDZ", sv.i_param.Dz);
        ReadBarArrangement::readBarsGirder(parseString(list.at(33), REGULATION::defaultBeamRc, "jMAINBAR1", iMainBarArrange1), &sv.j_param.mainBarArrange1);
        ReadBarArrangement::readBarsGirder(parseString(list.at(34), REGULATION::defaultBeamRc, "jMAINBAR2", iMainBarArrange2), &sv.j_param.mainBarArrange2);
        sv.j_param.concType = ConcreteType::Type::RECTANGLE;
        sv.j_param.mainBar1 = parseString(list.at(35), REGULATION::defaultBeamRc, "jMAINBARPHI1", sv.i_param.mainBar1);
        sv.j_param.mainBar2 = parseString(list.at(36), REGULATION::defaultBeamRc, "jMAINBARPHI2", sv.i_param.mainBar2);
        sv.j_param.hoopZ = parseInt(list.at(37), REGULATION::defaultBeamRc, "jHOOPZ", sv.i_param.hoopZ);
        sv.j_param.hoopBar = parseString(list.at(38), REGULATION::defaultBeamRc, "jHOOPPHI", sv.i_param.hoopBar);
        sv.j_param.hoopPitch = parseInt(list.at(39), REGULATION::defaultBeamRc, "jHOOPPITCH", sv.i_param.hoopPitch);
        sv.j_param.RK = parseDouble(list.at(40), REGULATION::defaultBeamRc, "jRK", sv.i_param.RK);
        sv.j_param.RKu = parseDouble(list.at(41), REGULATION::defaultBeamRc, "jRKU", sv.i_param.RKu);
        sv.j_param.RKb = parseDouble(list.at(42), REGULATION::defaultBeamRc, "jRKB", sv.i_param.RKb);
        sv.j_param.RY = parseDouble(list.at(43), REGULATION::defaultBeamRc, "jRY", sv.i_param.RY);

        interpreter->rcBeamList.append(sv);
    }
}

void ReadFromPsv::readSBeam(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultBeamS, "LABEL"));
        sv.memberType = DATATYPE::TPBEAM;
        sv.sectionType = STRUCTTYPE::STP_S;
        sv.line_param.isCantilever = parseString(list.at(2), REGULATION::defaultBeamS, "CANTILEVER") != u8"一般";
        sv.line_param.finishLoad = parseInt(list.at(3), REGULATION::defaultBeamS, "FINISHLOAD");
        sv.line_param.i_length = parseDouble(list.at(4), REGULATION::defaultBeamS, "iLENGTH");
        sv.line_param.j_length = parseDouble(list.at(5), REGULATION::defaultBeamS, "jLENGTH");

        sv.i_param.steelMaterial = parseString(list.at(6), REGULATION::defaultBeamS, "iSTEEL");
        sv.i_param.steelWebMaterial = parseString(list.at(7), REGULATION::defaultBeamS, "iSTEELWEB");
        QString iSteelType = parseString(list.at(8), REGULATION::defaultBeamS, "iSHAPELISTNAME");
        sv.i_param.steelType = SteelType::stringToEnum(iSteelType);
        sv.i_param.standardName = parseString(list.at(9), REGULATION::defaultBeamS, "iSTANDARDSHAPESTEEL");
        sv.i_param.sizeH = parseDouble(list.at(10), REGULATION::defaultBeamS, "iH");
        sv.i_param.sizeB = parseDouble(list.at(11), REGULATION::defaultBeamS, "iB");
        sv.i_param.t1 = parseDouble(list.at(12), REGULATION::defaultBeamS, "iT1");
        sv.i_param.t2 = parseDouble(list.at(13), REGULATION::defaultBeamS, "iT2");

        sv.c_param.steelMaterial = parseString(list.at(14), REGULATION::defaultBeamS, "mSTEEL", sv.i_param.steelMaterial);
        sv.c_param.steelWebMaterial = parseString(list.at(15), REGULATION::defaultBeamS, "mSTEELWEB", sv.i_param.steelWebMaterial);
        sv.c_param.steelType = SteelType::stringToEnum(parseString(list.at(16), REGULATION::defaultBeamS, "mSHAPELISTNAME", iSteelType));
        sv.c_param.standardName = parseString(list.at(17), REGULATION::defaultBeamS, "mSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.c_param.sizeH = parseDouble(list.at(18), REGULATION::defaultBeamS, "mH", sv.i_param.sizeH);
        sv.c_param.sizeB = parseDouble(list.at(19), REGULATION::defaultBeamS, "mB", sv.i_param.sizeB);
        sv.c_param.t1 = parseDouble(list.at(20), REGULATION::defaultBeamS, "mT1", sv.i_param.t1);
        sv.c_param.t2 = parseDouble(list.at(21), REGULATION::defaultBeamS, "mT2", sv.i_param.t2);

        sv.j_param.steelMaterial = parseString(list.at(22), REGULATION::defaultBeamS, "jSTEEL", sv.i_param.steelMaterial);
        sv.j_param.steelWebMaterial = parseString(list.at(23), REGULATION::defaultBeamS, "jSTEELWEB", sv.i_param.steelWebMaterial);
        sv.j_param.steelType = SteelType::stringToEnum(parseString(list.at(24), REGULATION::defaultBeamS, "jSHAPELISTNAME", iSteelType));
        sv.j_param.standardName = parseString(list.at(25), REGULATION::defaultBeamS, "jSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.j_param.sizeH = parseDouble(list.at(26), REGULATION::defaultBeamS, "jH", sv.i_param.sizeH);
        sv.j_param.sizeB = parseDouble(list.at(27), REGULATION::defaultBeamS, "jB", sv.i_param.sizeB);
        sv.j_param.t1 = parseDouble(list.at(28), REGULATION::defaultBeamS, "jT1", sv.i_param.t1);
        sv.j_param.t2 = parseDouble(list.at(29), REGULATION::defaultBeamS, "jT2", sv.i_param.t2);
        interpreter->sBeamList.append(sv);
    }
}

void ReadFromPsv::readSrcBeam(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultBeamSrc, "LABEL"));
        sv.memberType = DATATYPE::TPBEAM;
        sv.sectionType = STRUCTTYPE::STP_SRC;
        sv.line_param.isCantilever = parseString(list.at(2), REGULATION::defaultBeamSrc, "CANTILEVER") != u8"一般";
        sv.line_param.finishLoad = parseInt(list.at(3), REGULATION::defaultBeamSrc, "FINISHLOAD");
        sv.line_param.concMaterial = parseString(list.at(4), REGULATION::defaultBeamSrc, "CONC");
        sv.line_param.i_length = parseDouble(list.at(5), REGULATION::defaultBeamSrc, "iLENGTH");
        sv.line_param.j_length = parseDouble(list.at(6), REGULATION::defaultBeamSrc, "jLENGTH");

        sv.i_param.Dy = parseDouble(list.at(7), REGULATION::defaultBeamSrc, "iDY");
        sv.i_param.Dz = parseDouble(list.at(8), REGULATION::defaultBeamSrc, "iDZ");
        QString iMainBarArrange1 = parseString(list.at(9), REGULATION::defaultBeamSrc, "iMAINBAR1");
        ReadBarArrangement::readBarsGirder(iMainBarArrange1, &sv.i_param.mainBarArrange1);
        QString iMainBarArrange2 = parseString(list.at(10), REGULATION::defaultBeamSrc, "iMAINBAR2");
        ReadBarArrangement::readBarsGirder(iMainBarArrange2, &sv.i_param.mainBarArrange2);
        sv.i_param.concType = ConcreteType::Type::RECTANGLE;
        sv.i_param.mainBar1 = parseString(list.at(11), REGULATION::defaultBeamSrc, "iMAINBARPHI1");
        sv.i_param.mainBar2 = parseString(list.at(12), REGULATION::defaultBeamSrc, "iMAINBARPHI2");
        sv.i_param.hoopZ = parseInt(list.at(13), REGULATION::defaultBeamSrc, "iHOOPZ");
        sv.i_param.hoopBar = parseString(list.at(14), REGULATION::defaultBeamSrc, "iHOOPPHI");
        sv.i_param.hoopPitch = parseInt(list.at(15), REGULATION::defaultBeamSrc, "iHOOPPITCH");
        sv.i_param.RK = parseDouble(list.at(16), REGULATION::defaultBeamSrc, "iRK");
        sv.i_param.RKu = parseDouble(list.at(17), REGULATION::defaultBeamSrc, "iRKU");
        sv.i_param.RKb = parseDouble(list.at(18), REGULATION::defaultBeamSrc, "iRKB");
        sv.i_param.RY = parseDouble(list.at(19), REGULATION::defaultBeamSrc, "iRY");
        sv.i_param.RY1 = parseDouble(list.at(20), REGULATION::defaultBeamSrc, "iRY1");
        sv.i_param.steelMaterial = parseString(list.at(21), REGULATION::defaultBeamSrc, "iSTEEL");
        sv.i_param.steelWebMaterial = parseString(list.at(22), REGULATION::defaultBeamSrc, "iSTEELWEB");
        QString iSteelType = parseString(list.at(23), REGULATION::defaultBeamSrc, "iSHAPELISTNAME");
        sv.i_param.steelType = SteelType::stringToEnum(iSteelType);
        sv.i_param.standardName = parseString(list.at(24), REGULATION::defaultBeamSrc, "iSTANDARDSHAPESTEEL");
        sv.i_param.sizeH = parseDouble(list.at(25), REGULATION::defaultBeamSrc, "iH");
        sv.i_param.sizeB = parseDouble(list.at(26), REGULATION::defaultBeamSrc, "iB");
        sv.i_param.t1 = parseDouble(list.at(27), REGULATION::defaultBeamSrc, "iT1");
        sv.i_param.t2 = parseDouble(list.at(28), REGULATION::defaultBeamSrc, "iT2");
        sv.i_param.cY = parseDouble(list.at(29), REGULATION::defaultBeamSrc, "iCOODINATEY");
        sv.i_param.cZ = parseDouble(list.at(30), REGULATION::defaultBeamSrc, "iCOODINATEZ");

        sv.c_param.Dy = parseDouble(list.at(31), REGULATION::defaultBeamSrc, "mDY", sv.i_param.Dy);
        sv.c_param.Dz = parseDouble(list.at(32), REGULATION::defaultBeamSrc, "mDZ", sv.i_param.Dz);
        ReadBarArrangement::readBarsGirder(parseString(list.at(33), REGULATION::defaultBeamSrc, "mMAINBAR1", iMainBarArrange1), &sv.c_param.mainBarArrange1);
        ReadBarArrangement::readBarsGirder(parseString(list.at(34), REGULATION::defaultBeamSrc, "mMAINBAR2", iMainBarArrange2), &sv.c_param.mainBarArrange2);
        sv.c_param.concType = ConcreteType::Type::RECTANGLE;
        sv.c_param.mainBar1 = parseString(list.at(35), REGULATION::defaultBeamSrc, "mMAINBARPHI1", sv.i_param.mainBar1);
        sv.c_param.mainBar2 = parseString(list.at(36), REGULATION::defaultBeamSrc, "mMAINBARPHI2", sv.i_param.mainBar2);
        sv.c_param.hoopZ = parseInt(list.at(37), REGULATION::defaultBeamSrc, "mHOOPZ", sv.i_param.hoopZ);
        sv.c_param.hoopBar = parseString(list.at(38), REGULATION::defaultBeamSrc, "mHOOPPHI", sv.i_param.hoopBar);
        sv.c_param.hoopPitch = parseInt(list.at(39), REGULATION::defaultBeamSrc, "mHOOPPITCH", sv.i_param.hoopPitch);
        sv.c_param.RK = parseDouble(list.at(40), REGULATION::defaultBeamSrc, "mRK", sv.i_param.RK);
        sv.c_param.RKu = parseDouble(list.at(41), REGULATION::defaultBeamSrc, "mRKU", sv.i_param.RKu);
        sv.c_param.RKb = parseDouble(list.at(42), REGULATION::defaultBeamSrc, "mRKB", sv.i_param.RKb);
        sv.c_param.RY = parseDouble(list.at(43), REGULATION::defaultBeamSrc, "mRY", sv.i_param.RY);
        sv.c_param.RY1 = parseDouble(list.at(44), REGULATION::defaultBeamSrc, "mRY1", sv.i_param.RY1);
        sv.c_param.steelMaterial = parseString(list.at(45), REGULATION::defaultBeamSrc, "mSTEEL", sv.i_param.steelMaterial);
        sv.c_param.steelWebMaterial = parseString(list.at(46), REGULATION::defaultBeamSrc, "mSTEELWEB", sv.i_param.steelWebMaterial);
        sv.c_param.steelType = SteelType::stringToEnum(parseString(list.at(47), REGULATION::defaultBeamSrc, "mSHAPELISTNAME", iSteelType));
        sv.c_param.standardName = parseString(list.at(48), REGULATION::defaultBeamSrc, "mSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.c_param.sizeH = parseDouble(list.at(49), REGULATION::defaultBeamSrc, "mH", sv.i_param.sizeH);
        sv.c_param.sizeB = parseDouble(list.at(50), REGULATION::defaultBeamSrc, "mB", sv.i_param.sizeB);
        sv.c_param.t1 = parseDouble(list.at(51), REGULATION::defaultBeamSrc, "mT1", sv.i_param.t1);
        sv.c_param.t2 = parseDouble(list.at(52), REGULATION::defaultBeamSrc, "mT2", sv.i_param.t2);
        sv.c_param.cY = parseDouble(list.at(53), REGULATION::defaultBeamSrc, "mCOODINATEY", sv.i_param.cY);
        sv.c_param.cZ = parseDouble(list.at(54), REGULATION::defaultBeamSrc, "mCOODINATEZ", sv.i_param.cZ);

        sv.j_param.Dy = parseDouble(list.at(55), REGULATION::defaultBeamSrc, "jDY", sv.i_param.Dy);
        sv.j_param.Dz = parseDouble(list.at(56), REGULATION::defaultBeamSrc, "jDZ", sv.i_param.Dz);
        ReadBarArrangement::readBarsGirder(parseString(list.at(57), REGULATION::defaultBeamSrc, "jMAINBAR1", iMainBarArrange1), &sv.j_param.mainBarArrange1);
        ReadBarArrangement::readBarsGirder(parseString(list.at(58), REGULATION::defaultBeamSrc, "jMAINBAR2", iMainBarArrange2), &sv.j_param.mainBarArrange2);
        sv.j_param.concType = ConcreteType::Type::RECTANGLE;
        sv.j_param.mainBar1 = parseString(list.at(59), REGULATION::defaultBeamSrc, "jMAINBARPHI1", sv.i_param.mainBar1);
        sv.j_param.mainBar2 = parseString(list.at(60), REGULATION::defaultBeamSrc, "jMAINBARPHI2", sv.i_param.mainBar2);
        sv.j_param.hoopZ = parseInt(list.at(61), REGULATION::defaultBeamSrc, "jHOOPZ", sv.i_param.hoopZ);
        sv.j_param.hoopBar = parseString(list.at(62), REGULATION::defaultBeamSrc, "jHOOPPHI", sv.i_param.hoopBar);
        sv.j_param.hoopPitch = parseInt(list.at(63), REGULATION::defaultBeamSrc, "jHOOPPITCH", sv.i_param.hoopPitch);
        sv.j_param.RK = parseDouble(list.at(64), REGULATION::defaultBeamSrc, "jRK", sv.i_param.RK);
        sv.j_param.RKu = parseDouble(list.at(65), REGULATION::defaultBeamSrc, "jRKU", sv.i_param.RKu);
        sv.j_param.RKb = parseDouble(list.at(66), REGULATION::defaultBeamSrc, "jRKB", sv.i_param.RKb);
        sv.j_param.RY = parseDouble(list.at(67), REGULATION::defaultBeamSrc, "jRY", sv.i_param.RY);
        sv.j_param.RY1 = parseDouble(list.at(68), REGULATION::defaultBeamSrc, "jRY1", sv.i_param.RY1);
        sv.j_param.steelMaterial = parseString(list.at(69), REGULATION::defaultBeamSrc, "jSTEEL", sv.i_param.steelMaterial);
        sv.j_param.steelWebMaterial = parseString(list.at(70), REGULATION::defaultBeamSrc, "jSTEELWEB", sv.i_param.steelWebMaterial);
        sv.j_param.steelType = SteelType::stringToEnum(parseString(list.at(71), REGULATION::defaultBeamSrc, "jSHAPELISTNAME", iSteelType));
        sv.j_param.standardName = parseString(list.at(72), REGULATION::defaultBeamSrc, "jSTANDARDSHAPESTEEL", sv.i_param.standardName);
        sv.j_param.sizeH = parseDouble(list.at(73), REGULATION::defaultBeamSrc, "jH", sv.i_param.sizeH);
        sv.j_param.sizeB = parseDouble(list.at(74), REGULATION::defaultBeamSrc, "jB", sv.i_param.sizeB);
        sv.j_param.t1 = parseDouble(list.at(75), REGULATION::defaultBeamSrc, "jT1", sv.i_param.t1);
        sv.j_param.t2 = parseDouble(list.at(76), REGULATION::defaultBeamSrc, "jT2", sv.i_param.t2);
        sv.j_param.cY = parseDouble(list.at(77), REGULATION::defaultBeamSrc, "jCOODINATEY", sv.i_param.cY);
        sv.j_param.cZ = parseDouble(list.at(78), REGULATION::defaultBeamSrc, "jCOODINATEZ", sv.i_param.cZ);
        interpreter->srcBeamList.append(sv);
    }
}

void ReadFromPsv::readSlab(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultSlab, "LABEL"));
        sv.memberType = DATATYPE::TPSLAB;
        sv.sectionType = STRUCTTYPE::STP_RC;
        sv.plane_param.planeType = PlaneType::stringToEnum(parseString(list.at(2), REGULATION::defaultSlab, "SLABTYPE"));
        sv.plane_param.concMaterial = parseString(list.at(3), REGULATION::defaultSlab, "CONC");
        sv.plane_param.concThickness = parseDouble(list.at(4), REGULATION::defaultSlab, "CONCTHICKNESS");
        sv.plane_param.steelMaterial = parseString(list.at(5), REGULATION::defaultSlab, "STEEL");
        sv.plane_param.steelThickness = parseDouble(list.at(6), REGULATION::defaultSlab, "STEELTHICKNESS");
        sv.plane_param.finishLoad = parseInt(list.at(7), REGULATION::defaultSlab, "FINISHLOAD");
        sv.plane_param.loadDir = parseDouble(list.at(8), REGULATION::defaultSlab, "LISTLOADDIRECTION");
        QString starndardPhi = parseString(list.at(9), REGULATION::defaultSlab, "STANDARDBARPHI");
        int standardPitch = parseInt(list.at(10), REGULATION::defaultSlab, "STANDARDBARPITCH");
        sv.plane_param.sBar_upperEdgeA = PlaneSectionParameter::Bar(parseString(list.at(11), REGULATION::defaultSlab, "sUPPEREDGE_A_BARPHI", starndardPhi),
                                                                    parseInt(list.at(12), REGULATION::defaultSlab, "sUPPEREDGE_A_BARPITCH", standardPitch));
        sv.plane_param.sBar_lowerEdgeA = PlaneSectionParameter::Bar(parseString(list.at(13), REGULATION::defaultSlab, "sLOWEREDGE_A_BARPHI", starndardPhi),
                                                                    parseInt(list.at(14), REGULATION::defaultSlab, "sLOWEREDGE_A_BARPITCH", standardPitch));
        sv.plane_param.sBar_upperCenterA = PlaneSectionParameter::Bar(parseString(list.at(15), REGULATION::defaultSlab, "sUPPERCENTER_A_BARPHI", starndardPhi),
                                                                      parseInt(list.at(16), REGULATION::defaultSlab, "sUPPERCENTER_A_BARPITCH", standardPitch));
        sv.plane_param.sBar_lowerCenterA = PlaneSectionParameter::Bar(parseString(list.at(17), REGULATION::defaultSlab, "sLOWERCENTER_A_BARPHI", starndardPhi),
                                                                      parseInt(list.at(18), REGULATION::defaultSlab, "sLOWERCENTER_A_BARPITCH", standardPitch));
        sv.plane_param.sBar_upperEdgeCenterB = PlaneSectionParameter::Bar(parseString(list.at(19), REGULATION::defaultSlab, "sUPPER_B_BARPHI", starndardPhi),
                                                                          parseInt(list.at(20), REGULATION::defaultSlab, "sUPPER_B_BARPITCH", standardPitch));
        sv.plane_param.sBar_lowerEdgeCenterB = PlaneSectionParameter::Bar(parseString(list.at(21), REGULATION::defaultSlab, "sLOWER_B_BARPHI", starndardPhi),
                                                                          parseInt(list.at(22), REGULATION::defaultSlab, "sLOWER_B_BARPITCH", standardPitch));
        sv.plane_param.lBar_upperEdgeA = PlaneSectionParameter::Bar(parseString(list.at(23), REGULATION::defaultSlab, "lUPPEREDGE_A_BARPHI", starndardPhi),
                                                                    parseInt(list.at(24), REGULATION::defaultSlab, "lUPPEREDGE_A_BARPITCH", standardPitch));
        sv.plane_param.lBar_lowerEdgeA = PlaneSectionParameter::Bar(parseString(list.at(25), REGULATION::defaultSlab, "lLOWEREDGE_A_BARPHI", starndardPhi),
                                                                    parseInt(list.at(26), REGULATION::defaultSlab, "lLOWEREDGE_A_BARPITCH", standardPitch));
        sv.plane_param.lBar_upperCenterA = PlaneSectionParameter::Bar(parseString(list.at(27), REGULATION::defaultSlab, "lUPPERCENTER_A_BARPHI", starndardPhi),
                                                                      parseInt(list.at(28), REGULATION::defaultSlab, "lUPPERCENTER_A_BARPITCH", standardPitch));
        sv.plane_param.lBar_lowerCenterA = PlaneSectionParameter::Bar(parseString(list.at(29), REGULATION::defaultSlab, "lLOWERCENTER_A_BARPHI", starndardPhi),
                                                                      parseInt(list.at(30), REGULATION::defaultSlab, "lLOWERCENTER_A_BARPITCH", standardPitch));
        sv.plane_param.lBar_upperEdgeCenterB = PlaneSectionParameter::Bar(parseString(list.at(31), REGULATION::defaultSlab, "lUPPER_B_BARPHI", starndardPhi),
                                                                          parseInt(list.at(32), REGULATION::defaultSlab, "lUPPER_B_BARPITCH", standardPitch));
        sv.plane_param.lBar_lowerEdgeCenterB = PlaneSectionParameter::Bar(parseString(list.at(33), REGULATION::defaultSlab, "lLOWER_B_BARPHI", starndardPhi),
                                                                          parseInt(list.at(34), REGULATION::defaultSlab, "lLOWER_B_BARPITCH", standardPitch));
        interpreter->rcSlabList.append(sv);
    }
}

void ReadFromPsv::readRcWall(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultWallRc, "LABEL"));
        sv.memberType = DATATYPE::TPWALL;
        sv.sectionType = STRUCTTYPE::STP_RC;
        sv.startFloorID = interpreter->getOrCreateUuid(parseString(list.at(2), REGULATION::defaultWallRc, "FLOORFROM"));
        sv.endFloorID = interpreter->getOrCreateUuid(parseString(list.at(3), REGULATION::defaultWallRc, "FLOORTO"));
        sv.plane_param.planeType = PlaneType::stringToEnum(parseString(list.at(4), REGULATION::defaultWallRc, "WALLTYPE"));
        sv.plane_param.concMaterial = parseString(list.at(5), REGULATION::defaultWallRc, "CONC");
        sv.plane_param.concThickness = parseDouble(list.at(6), REGULATION::defaultWallRc, "CONCTHICKNESS");
        sv.plane_param.steelMaterial = parseString(list.at(7), REGULATION::defaultWallRc, "STEEL");
        sv.plane_param.steelThickness = parseDouble(list.at(8), REGULATION::defaultWallRc, "STEELTHICKNESS");
        sv.plane_param.finishLoad = parseInt(list.at(9), REGULATION::defaultWallRc, "FINISHLOAD");
        sv.plane_param.isDoubleBar = parseString(list.at(10), REGULATION::defaultWallRc, "BARARRANGEMENTTYPE") == u8"ダブル";
        QString vBarPhi = parseString(list.at(11), REGULATION::defaultWallRc, "vBARPHI");
        int vBarPitch = parseInt(list.at(12), REGULATION::defaultWallRc, "vBARPITCH");
        sv.plane_param.vBar = PlaneSectionParameter::Bar(vBarPhi, vBarPitch);
        sv.plane_param.hBar = PlaneSectionParameter::Bar(parseString(list.at(13), REGULATION::defaultWallRc, "hBARPHI", vBarPhi),
                                                         parseInt(list.at(14), REGULATION::defaultWallRc, "hBARPITCH", vBarPitch));
        sv.plane_param.vOpeningBar_small = PlaneSectionParameter::OpeningBar(parseString(list.at(16), REGULATION::defaultWallRc, "vSMALLOPENINGBARPHI"),
                                                                             parseInt(list.at(15), REGULATION::defaultWallRc, "vSMALLOPENINGBARNUM"));
        sv.plane_param.hOpeningBar_small = PlaneSectionParameter::OpeningBar(parseString(list.at(18), REGULATION::defaultWallRc, "hSMALLOPENINGBARPHI"),
                                                                             parseInt(list.at(17), REGULATION::defaultWallRc, "hSMALLOPENINGBARNUM"));
        sv.plane_param.dOpeningBar_small = PlaneSectionParameter::OpeningBar(parseString(list.at(20), REGULATION::defaultWallRc, "dSMALLOPENINGBARPHI"),
                                                                             parseInt(list.at(19), REGULATION::defaultWallRc, "dSMALLOPENINGBARNUM"));
        sv.plane_param.vOpeningBar_large = PlaneSectionParameter::OpeningBar(parseString(list.at(22), REGULATION::defaultWallRc, "vLARGEOPENINGBARPHI"),
                                                                             parseInt(list.at(21), REGULATION::defaultWallRc, "vLARGEOPENINGBARNUM"));
        sv.plane_param.hOpeningBar_large = PlaneSectionParameter::OpeningBar(parseString(list.at(24), REGULATION::defaultWallRc, "hLARGEOPENINGBARPHI"),
                                                                             parseInt(list.at(23), REGULATION::defaultWallRc, "hLARGEOPENINGBARNUM"));
        sv.plane_param.dOpeningBar_large = PlaneSectionParameter::OpeningBar(parseString(list.at(26), REGULATION::defaultWallRc, "dLARGEOPENINGBARPHI"),
                                                                             parseInt(list.at(25), REGULATION::defaultWallRc, "dLARGEOPENINGBARNUM"));
        interpreter->rcWallList.append(sv);
    }
}

void ReadFromPsv::readSWall(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultWallS, "LABEL"));
        sv.memberType = DATATYPE::TPWALL;
        sv.sectionType = STRUCTTYPE::STP_S;
        sv.startFloorID = interpreter->getOrCreateUuid(parseString(list.at(2), REGULATION::defaultWallS, "FLOORFROM"));
        sv.endFloorID = interpreter->getOrCreateUuid(parseString(list.at(3), REGULATION::defaultWallS, "FLOORTO"));
        sv.plane_param.planeType = PlaneType::stringToEnum(parseString(list.at(4), REGULATION::defaultWallS, "WALLTYPE"));
        sv.plane_param.steelMaterial = parseString(list.at(5), REGULATION::defaultWallS, "STEEL");
        sv.plane_param.steelThickness = parseDouble(list.at(6), REGULATION::defaultWallS, "STEELTHICKNESS");
        sv.plane_param.finishLoad = parseInt(list.at(7), REGULATION::defaultWallS, "FINISHLOAD");
        // vRib_pitch;               //　縦リブピッチ
        // vRib_type;                // 縦リブタイプ
        // vRib_thickness;           // 縦リブ厚
        // vRib_width;               // 縦リブ幅
        // hRib_pitch;               // 横リブピッチ
        // hRib_type;                // 横リブタイプ
        // hRib_thickness;           // 横リブ厚
        // hRib_width;               // 横リブ幅

        interpreter->sWallList.append(sv);
    }
}

void ReadFromPsv::readFoundationWall(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultWallUnderGround, "LABEL"));
        sv.memberType = DATATYPE::TPUWALL;
        sv.sectionType = STRUCTTYPE::STP_RC;
        sv.plane_param.concMaterial = parseString(list.at(2), REGULATION::defaultWallUnderGround, "CONC");
        sv.plane_param.concThickness = parseDouble(list.at(3), REGULATION::defaultWallUnderGround, "CONCTHICKNESS");
        sv.plane_param.steelMaterial = parseString(list.at(4), REGULATION::defaultWallUnderGround, "STEEL");
        sv.plane_param.steelThickness = parseDouble(list.at(5), REGULATION::defaultWallUnderGround, "STEELTHICKNESS");
        sv.plane_param.finishLoad = parseInt(list.at(6), REGULATION::defaultWallUnderGround, "FINISHLOAD");
        QString standardPhi = parseString(list.at(7), REGULATION::defaultWallUnderGround, "STANDARDBARPHI");
        int standardpitch = parseInt(list.at(8), REGULATION::defaultWallUnderGround, "STANDARDBARPITCH");
        sv.plane_param.vBar_outerTop = PlaneSectionParameter::Bar(parseString(list.at(1), REGULATION::defaultWallUnderGround, "vOUTERTOP_BARPHI", standardPhi),
                                                                  parseInt(list.at(1), REGULATION::defaultWallUnderGround, "vOUTERTOP_BARPITCH", standardpitch));
        sv.plane_param.vBar_outerMiddle = PlaneSectionParameter::Bar(parseString(list.at(1), REGULATION::defaultWallUnderGround, "vOUTERMIDDLE_BARPHI", standardPhi),
                                                                     parseInt(list.at(1), REGULATION::defaultWallUnderGround, "vOUTERMIDDLE_BARPITCH", standardpitch));
        sv.plane_param.vBar_outerBottom = PlaneSectionParameter::Bar(parseString(list.at(1), REGULATION::defaultWallUnderGround, "vOUTERBOTTOM_BARPHI", standardPhi),
                                                                     parseInt(list.at(1), REGULATION::defaultWallUnderGround, "vOUTERBOTTOM_BARPITCH", standardpitch));
        sv.plane_param.hBar_outerEdge = PlaneSectionParameter::Bar(parseString(list.at(1), REGULATION::defaultWallUnderGround, "hOUTEREDGE_BARPHI", standardPhi),
                                                                   parseInt(list.at(1), REGULATION::defaultWallUnderGround, "hOUTEREDGE_BARPITCH", standardpitch));
        sv.plane_param.hBar_outerCenter = PlaneSectionParameter::Bar(parseString(list.at(1), REGULATION::defaultWallUnderGround, "hOUTERCENTER_BARPHI", standardPhi),
                                                                     parseInt(list.at(1), REGULATION::defaultWallUnderGround, "hOUTERCENTER_BARPITCH", standardpitch));
        sv.plane_param.vBar_innerTop = PlaneSectionParameter::Bar(parseString(list.at(1), REGULATION::defaultWallUnderGround, "vINNERTOP_BARPHI", standardPhi),
                                                                  parseInt(list.at(1), REGULATION::defaultWallUnderGround, "vINNERTOP_BARPITCH", standardpitch));
        sv.plane_param.vBar_innerMiddle = PlaneSectionParameter::Bar(parseString(list.at(1), REGULATION::defaultWallUnderGround, "vINNERMIDDLE_BARPHI", standardPhi),
                                                                     parseInt(list.at(1), REGULATION::defaultWallUnderGround, "vINNERMIDDLE_BARPITCH", standardpitch));
        sv.plane_param.vBar_innerBottom = PlaneSectionParameter::Bar(parseString(list.at(1), REGULATION::defaultWallUnderGround, "vINNERBOTTOM_BARPHI", standardPhi),
                                                                     parseInt(list.at(1), REGULATION::defaultWallUnderGround, "vINNERBOTTOM_BARPITCH", standardpitch));
        sv.plane_param.hBar_innerEdge = PlaneSectionParameter::Bar(parseString(list.at(1), REGULATION::defaultWallUnderGround, "hINNEREDGE_BARPHI", standardPhi),
                                                                   parseInt(list.at(1), REGULATION::defaultWallUnderGround, "hINNEREDGE_BARPITCH", standardpitch));
        sv.plane_param.hBar_innerCenter = PlaneSectionParameter::Bar(parseString(list.at(1), REGULATION::defaultWallUnderGround, "hINNERCENTER_BARPHI", standardPhi),
                                                                     parseInt(list.at(1), REGULATION::defaultWallUnderGround, "hINNERCENTER_BARPITCH", standardpitch));
        interpreter->rcUwallList.append(sv);
    }

}

void ReadFromPsv::readSBase(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultBaseS, "LABEL"));
        sv.memberType = DATATYPE::TPBASE;
        sv.sectionType = STRUCTTYPE::STP_S;
        sv.base_param.isStandardType = parseString(list.at(2), REGULATION::defaultBaseS, "STANDARDTYPE") == u8"在来品";
        sv.base_param.productName = parseString(list.at(3), REGULATION::defaultBaseS, "PRODUCTNAME");
        sv.base_param.sizeB = parseDouble(list.at(4), REGULATION::defaultBaseS, "B");
        sv.base_param.sizeD = parseDouble(list.at(5), REGULATION::defaultBaseS, "D", sv.base_param.sizeB);
        sv.base_param.sizeT = parseDouble(list.at(6), REGULATION::defaultBaseS, "T");
        sv.base_param.basePlateMaterial = parseString(list.at(7), REGULATION::defaultBaseS, "BASEPLATEMATERIAL");
        sv.base_param.edgehy1 = parseDouble(list.at(8), REGULATION::defaultBaseS, "EDGEDISTANCEY1");
        sv.base_param.edgehz1 = parseDouble(list.at(9), REGULATION::defaultBaseS, "EDGEDISTANCEZ1");
        sv.base_param.bolt_ny1 = parseInt(list.at(10), REGULATION::defaultBaseS, "BOLTNY1");
        sv.base_param.bolt_nz1 = parseInt(list.at(11), REGULATION::defaultBaseS, "BOLTNZ1", sv.base_param.bolt_ny1);
        sv.base_param.boltPhi = parseInt(list.at(12), REGULATION::defaultBaseS, "BOLTPHI");
        sv.base_param.boltMaterial = parseString(list.at(13), REGULATION::defaultBaseS, "BOLTMATERIAL");
        sv.base_param.anchorLength = parseDouble(list.at(14), REGULATION::defaultBaseS, "BOLTLENGTH");
        interpreter->sBaseList.append(sv);
    }
}

void ReadFromPsv::readSrcBase(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultBaseSrc, "LABEL"));
        sv.memberType = DATATYPE::TPBASE;
        sv.sectionType = STRUCTTYPE::STP_SRC;
        sv.base_param.B0 = parseDouble(list.at(2), REGULATION::defaultBaseSrc, "B0");
        sv.base_param.D0 = parseDouble(list.at(3), REGULATION::defaultBaseSrc, "D0");
        sv.base_param.concMaterial = parseString(list.at(4), REGULATION::defaultBaseSrc, "CONC");
        sv.base_param.basePlateType = BasePlateType::stringToEnum(parseString(list.at(5), REGULATION::defaultBaseSrc, "BASEPLATETYPE"));
        sv.base_param.sizeB = parseDouble(list.at(6), REGULATION::defaultBaseSrc, "B");
        sv.base_param.sizeD = parseDouble(list.at(7), REGULATION::defaultBaseSrc, "D", sv.base_param.sizeB);
        sv.base_param.sizeT = parseDouble(list.at(8), REGULATION::defaultBaseSrc, "T");
        sv.base_param.SML = parseDouble(list.at(9), REGULATION::defaultBaseSrc, "SML");
        sv.base_param.edgehy1 = parseDouble(list.at(10), REGULATION::defaultBaseSrc, "EDGEDISTANCEY1");
        sv.base_param.edgehy2 = parseDouble(list.at(11), REGULATION::defaultBaseSrc, "EDGEDISTANCEY2", sv.base_param.edgehy1);
        sv.base_param.edgehz1 = parseDouble(list.at(12), REGULATION::defaultBaseSrc, "EDGEDISTANCEZ1");;
        sv.base_param.edgehz2 = parseDouble(list.at(13), REGULATION::defaultBaseSrc, "EDGEDISTANCEZ2", sv.base_param.edgehz1);
        sv.base_param.boltType = BaseBoltType::stringToEnum(parseString(list.at(14), REGULATION::defaultBaseSrc, "BOLTARRANGEMENTTYPE"));
        sv.base_param.bolt_ny1 = parseInt(list.at(15), REGULATION::defaultBaseSrc, "BOLTNY1");
        sv.base_param.bolt_ny2 = parseInt(list.at(16), REGULATION::defaultBaseSrc, "BOLTNY2", sv.base_param.bolt_ny1);
        sv.base_param.bolt_nz1 = parseInt(list.at(17), REGULATION::defaultBaseSrc, "BOLTNZ1");
        sv.base_param.bolt_nz2 = parseInt(list.at(18), REGULATION::defaultBaseSrc, "BOLTNZ2", sv.base_param.bolt_nz1);
        sv.base_param.boltPhi = parseInt(list.at(19), REGULATION::defaultBaseSrc, "BOLTPHI");
        sv.base_param.boltMaterial = parseString(list.at(20), REGULATION::defaultBaseSrc, "BOLTMATERIAL");
        interpreter->srcBaseList.append(sv);
    }
}

void ReadFromPsv::readOpening(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultOpening, "LABEL"));
        sv.memberType = DATATYPE::TPWALLOPEN;
        sv.wopen_param.openType = WallOpenType::stringToEnumType(parseString(list.at(2), REGULATION::defaultOpening, "OPENINGTYPE"));
        sv.wopen_param.isHorizontalFace = parseString(list.at(3), REGULATION::defaultOpening, "LRBASE") == u8"内法から";
        sv.wopen_param.hPosition = WallOpenType::stringToEnumH(parseString(list.at(4), REGULATION::defaultOpening, "LR"));
        sv.wopen_param.isVerticalFace = parseString(list.at(5), REGULATION::defaultOpening, "UDBASE") == u8"内法から";
        sv.wopen_param.vPosition = WallOpenType::stringToEnumV(parseString(list.at(6), REGULATION::defaultOpening, "UD"));
        sv.wopen_param.ox = parseDouble(list.at(7), REGULATION::defaultOpening, "POSITIONX");
        sv.wopen_param.oy = parseDouble(list.at(8), REGULATION::defaultOpening, "POSITIONY");
        sv.wopen_param.width = parseDouble(list.at(9), REGULATION::defaultOpening, "WIDTH");
        sv.wopen_param.height = parseDouble(list.at(10), REGULATION::defaultOpening, "HEIGHT");
        sv.wopen_param.slitL = parseDouble(list.at(11), REGULATION::defaultOpening, "SLITL");
        sv.wopen_param.slitR = parseDouble(list.at(12), REGULATION::defaultOpening, "SLITR");
        sv.wopen_param.slitB = parseDouble(list.at(13), REGULATION::defaultOpening, "SLITB");
        sv.wopen_param.slitT = parseDouble(list.at(14), REGULATION::defaultOpening, "SLITT");
        sv.wopen_param.comment = parseString(list.at(15), REGULATION::defaultOpening, "COMMENT");
        interpreter->wallOpenList.append(sv);
    }
}

void ReadFromPsv::readDamper(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultDamper, "LABEL"));
        sv.memberType = DATATYPE::TPDAMPER;
        sv.damper_param.productType = DamperType::stringToEnum(parseString(list.at(2), REGULATION::defaultDamper, "PRODUCTTYPE"));
        // unit list.at(3).toDouble();
        auto parameters = parseString(list.at(4), REGULATION::defaultDamper, "PARAMETERS").split("/");
        foreach(auto p, parameters){
            sv.damper_param.parameters.append(p.toDouble());
        }
        sv.damper_param.selfWeight = parseDouble(list.at(5), REGULATION::defaultDamper, "SELFWEIGHT");
        sv.damper_param.vecX = parseDouble(list.at(6), REGULATION::defaultDamper, "VECTOR_X");
        sv.damper_param.vecY = parseDouble(list.at(7), REGULATION::defaultDamper, "VECTOR_Y");
        sv.damper_param.vecZ = parseDouble(list.at(8), REGULATION::defaultDamper, "VECTOR_Z");
        sv.damper_param.period = parseDouble(list.at(9), REGULATION::defaultDamper, "NATURALPERIOD");
        sv.damper_param.maxDisp = parseDouble(list.at(10), REGULATION::defaultDamper, "DISPLACEMENT");
        sv.damper_param.modelFlag = parseInt(list.at(11), REGULATION::defaultDamper, "MODEL_FLAG");
        sv.damper_param.rigidLength = parseDouble(list.at(12), REGULATION::defaultDamper, "RIGID_LENGTH");
        sv.damper_param.damperDirection = parseDouble(list.at(13), REGULATION::defaultDamper, "DAMPER_DIRECTION");
        sv.damper_param.tAnalysis = parseDouble(list.at(14), REGULATION::defaultDamper, "TANALYSIS");
        sv.damper_param.factor = parseDouble(list.at(15), REGULATION::defaultDamper, "FACTOR");
        sv.damper_param.frameRigidity = parseDouble(list.at(16), REGULATION::defaultDamper, "FRAME_RIGIDITY");
        interpreter->damperList.append(sv);
    }
}

void ReadFromPsv::readIsolator(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;

        SectionValues sv(parseString(list.at(1), REGULATION::defaultIsolator, "LABEL"));
        sv.memberType = DATATYPE::TPISO;
        sv.isolator_param.productType = parseString(list.at(1), REGULATION::defaultIsolator, "PRODUCTTYPE");
        sv.isolator_param.productName = parseString(list.at(1), REGULATION::defaultIsolator, "PRODUCTNAME");
        // shearhystresis
        // hk1
        // hk2
        // hk3
        // hfy1
        // hfy2
        // axialhystresis
        // vk1
        // vk2
        // vk3
        // vfy1plus
        // vfy2plus
        sv.isolator_param.selfWeight = parseDouble(list.at(16), REGULATION::defaultIsolator, "SELFWEIGHT");
        sv.isolator_param.renderShape = parseString(list.at(17), REGULATION::defaultIsolator, "RENDERSHAPE");
        sv.isolator_param.sizeD = parseDouble(list.at(18), REGULATION::defaultIsolator, "RENDERSIZED");
        sv.isolator_param.sizeH = parseDouble(list.at(19), REGULATION::defaultIsolator, "RENDERSIZEH");
        sv.isolator_param.settingLevel = parseDouble(list.at(20), REGULATION::defaultIsolator, "SETLEVEL");
        sv.isolator_param.displacement = parseDouble(list.at(21), REGULATION::defaultIsolator, "DISPLACEMENT");
        sv.isolator_param.springNumber = parseInt(list.at(22), REGULATION::defaultIsolator, "SPRING");
        sv.isolator_param.inflectionRatio = parseDouble(list.at(23), REGULATION::defaultIsolator, "INFLECTION");
        interpreter->isolatorList.append(sv);
    }

}

void ReadFromPsv::readJointLoad(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    QList<JOINTLOADVALUES> temp;

    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;
        QString name = parseString(list.at(1), REGULATION::defaultJointLoad, "NAME");
        JOINTLOADVALUES jv;
        bool hasItem = false;
        for (int i = 0; i < temp.count(); i++){
            if (temp.at(i).loadName == name){
                hasItem = true;
                jv = temp.at(i);
                temp.removeAt(i);
                break;
            }
        }
        if (!hasItem)
            jv = JOINTLOADVALUES(name);

        jv.loadTable.insert(
                        LoadType::stringToEnum(parseString(list.at(2), REGULATION::defaultJointLoad, "TYPE")),
                        JOINTLOADDIM(parseDouble(list.at(3), REGULATION::defaultJointLoad, "FX"),
                                     parseDouble(list.at(4), REGULATION::defaultJointLoad, "FY"),
                                     parseDouble(list.at(5), REGULATION::defaultJointLoad, "FZ"),
                                     parseDouble(list.at(6), REGULATION::defaultJointLoad, "MX"),
                                     parseDouble(list.at(7), REGULATION::defaultJointLoad, "MY"),
                                     parseDouble(list.at(8), REGULATION::defaultJointLoad, "MZ"),
                                     parseString(list.at(9), REGULATION::defaultJointLoad, "COMMENT")));
        temp.append(jv);
    }
    interpreter->jointLoadList.append(temp);
}

void ReadFromPsv::readMemberLoad(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    QList<MEMBERLOADVALUES> temp;

    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;
        QString name = parseString(list.at(1), REGULATION::defaultMemberLoad, "NAME");
        MEMBERLOADVALUES mv;
        bool hasItem = false;
        for (int i = 0; i < temp.count(); i++){
            if (temp.at(i).loadName == name){
                hasItem = true;
                mv = temp.at(i);
                temp.removeAt(i);
                break;
            }
        }
        if (!hasItem)
            mv = MEMBERLOADVALUES(name);

        LoadType::LOADTYPE type =  LoadType::stringToEnum(parseString(list.at(2), REGULATION::defaultMemberLoad, "TYPE"));
        LCORSYSTEM lcor = parseString(list.at(3), REGULATION::defaultMemberLoad, "COORDINATE") == u8"全体座標系" ? LCORSYSTEM::LCOR_GLOBAL : LCORSYSTEM::LCOR_LOCAL;
        QString direction = parseString(list.at(4), REGULATION::defaultMemberLoad, "DIRECTION");
        LOADDIR ldir;
        if (direction == REGULATION::defaultMemberLoadDirection.at(0))
            ldir = LOADDIR::LDIR_X;
        else if(direction == REGULATION::defaultMemberLoadDirection.at(1))
            ldir = LOADDIR::LDIR_Y;
        else if(direction == REGULATION::defaultMemberLoadDirection.at(2))
            ldir = LOADDIR::LDIR_Z;
        else
            qFatal(u8"LOADDIR");

        mv.loadTable.insert(
                        type,
                        MEMBERLOADDIM(lcor,
                                      ldir,
                                      parseDouble(list.at(5), REGULATION::defaultMemberLoad, "WA"),
                                      parseDouble(list.at(6), REGULATION::defaultMemberLoad, "WB"),
                                      parseString(list.at(7), REGULATION::defaultMemberLoad, "COMMENT")));
        temp.append(mv);
    }
    interpreter->memberLoadList.append(temp);
}

void ReadFromPsv::readSlabLoad(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;
        SLABLOADVALUES sv(parseString(list.at(1), REGULATION::defaultSlabLoad, "NAME"));
        sv.sDL = parseInt(list.at(2), REGULATION::defaultSlabLoad, "DL");
        sv.sLL0 = parseInt(list.at(3), REGULATION::defaultSlabLoad, "LL0");
        sv.sLL1 = parseInt(list.at(4), REGULATION::defaultSlabLoad, "LL1");
        sv.sLL2 = parseInt(list.at(5), REGULATION::defaultSlabLoad, "LL2");
        sv.sLL3 = parseInt(list.at(6), REGULATION::defaultSlabLoad, "LL3");
        sv.sLL4 = parseInt(list.at(7), REGULATION::defaultSlabLoad, "LL4");
        sv.sLLE = parseInt(list.at(8), REGULATION::defaultSlabLoad, "LLE");
        sv.notes = parseString(list.at(9), REGULATION::defaultSlabLoad, "COMMENT");
        interpreter->slabLoadList.append(sv);
    }
}

void ReadFromPsv::readAnalysisFloor(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    auto floors = infile.readLine().split(",");
    interpreter->baseFloor = QUuid(floors.at(0));
    interpreter->roofFloor = QUuid(floors.at(1));

    auto split = infile.readLine().split(" ");
    int count = split.at(0).toInt();
    int column = split.at(1).toInt();
    infile.readLine();
    for (int i = 0 ; i < count; i++){
        QStringList list = infile.readLine().split("\t");
        if (list.count() != column || list.at(1).trimmed() == "")
            continue;
        FLOORVALUES fv = interpreter->sortedFloors.at(i);
        fv.adjustedLevel = parseDouble(list.at(2), REGULATION::defaultAnalysisFloor, u8"LEVEL");
        fv.isChildFloor = parseString(list.at(3), REGULATION::defaultAnalysisFloor, u8"CHILDFLOOR") != u8"一般階";
        fv.parentID = QUuid(parseString(list.at(4), REGULATION::defaultAnalysisFloor, u8"PARENTFLOOR"));
        interpreter->AnalysisFloors.append(fv);

        interpreter->sortedFloors[i].adjustedLevel = fv.adjustedLevel;
        interpreter->sortedFloors[i].isChildFloor = fv.isChildFloor;
        interpreter->sortedFloors[i].parentID = fv.parentID;
    }

}

void ReadFromPsv::readAnalysisCondition(QTextStream &infile, QSharedPointer<InterpreterCsv> interpreter)
{
    QString gene;
    while(true){
        QString line = infile.readLine();
        if (line == "**ANALYSISCONDITION"){
            break;
        }
        gene.append(line).append("\n");
    }
    interpreter->gene = gene;

    QString analysisCondition;
    int lineCount = infile.readLine().toInt();
    for (int i = 0; i < lineCount; i++){
        analysisCondition.append(infile.readLine());
        if (i != lineCount - 1)
            analysisCondition.append("\n");
    }
    interpreter->analysisCondition = analysisCondition;
}


void ReadFromPsv::arrange(QSharedPointer<InterpreterCsv> interpreter)
{
    if (interpreter->baseFloor == QUuid{})
        return;
    bool isUnderGround = false;
    for(int i = interpreter->sortedFloors.count() -1; i >= 0; i--){
        if (isUnderGround && interpreter->sortedFloors.at(i).floorType != FLOORTYPE::FL_CHILD)
            interpreter->sortedFloors[i].floorType = FLOORTYPE::FL_UNDERGROUND;

        if (interpreter->sortedFloors.at(i).UUID == interpreter->baseFloor)
            isUnderGround = true;
    }

    if (interpreter->roofFloor == QUuid{})
        return;
    bool isPentHouse = false;
    for (int i = 0; i < interpreter->sortedFloors.count(); i++){
        if (isPentHouse && interpreter->sortedFloors.at(i).floorType != FLOORTYPE::FL_CHILD)
            interpreter->sortedFloors[i].floorType = FLOORTYPE::FL_PENTHOUSE;

        if (interpreter->sortedFloors.at(i).UUID == interpreter->roofFloor)
            isPentHouse = true;
    }
}

QString ReadFromPsv::parseString(const QString &split, const QMap<QString, REGULATION> &map, const QString &tag, const QVariant& ref)
{
    if (!map.contains(tag))
        qFatal(u8"parseString not contain");

    auto reg = map.value(tag);
    if (reg.cellType != TABLEDATATYPE::TB_STRING)
        qFatal(u8"parseString not Equal cellType");

    QString val = split.trimmed();
    if (val != QString{}){
        return val;
    }

    if (ref != QVariant()){
        return ref.toString();
    }


    return reg.defValue.toString();
}

double ReadFromPsv::parseDouble(const QString &split, const QMap<QString, REGULATION> &map, const QString &tag, const QVariant& ref)
{
    if (!map.contains(tag))
        qFatal(u8"parseDouble not contain");

    auto reg = map.value(tag);
    if (reg.cellType != TABLEDATATYPE::TB_DOUBLE)
        qFatal(u8"parseDouble not Equal cellType");

    QString val = split.trimmed();
    if (val != QString{})
        return val.toDouble();

    if (ref != QVariant())
        return ref.toDouble();


    return reg.defValue.toDouble();
}

int ReadFromPsv::parseInt(const QString &split, const QMap<QString, REGULATION> &map, const QString &tag, const QVariant& ref)
{
    if (!map.contains(tag))
        qFatal(u8"parseInt not contain");

    auto reg = map.value(tag);
    if (reg.cellType != TABLEDATATYPE::TB_INT && reg.cellType != TABLEDATATYPE::TB_BOOL)
        qFatal(u8"parseInt not Equal cellType");

    QString val = split.trimmed();
    if (val != QString{})
        return val.toInt();

    if (ref != QVariant())
        return ref.toInt();

    return reg.defValue.toInt();
}



} // namespace post3dapp
