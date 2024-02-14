#include "define_sectiontable.h"

#include "define_defaultvalue.h"
#include "define_section_struct.h"
namespace post3dapp
{
    const StandardSectionTable *SectionDataType::standardSection = new StandardSectionTable;

    SectionDataType::SectionDataType()
    {
        myDataType = DATATYPE::NODATATYPE;
        mySectionType = STRUCTTYPE::NOSTRUCTTYPE;
    }

    int SectionDataType::getSeparateColumn1() const
    {
        return spCol1;
    }
    int SectionDataType::getSeparateColumn2() const
    {
        return spCol2;
    }
    int SectionDataType::getSeparateColumn3() const
    {
        return spCol3;
    }

    QList<int> SectionDataType::getShapeTypeColumns() const
    {
        return shapeTypeColumns;
    }
    QList<int> SectionDataType::getDirectInputColumns() const
    {
        return directInputColumns;
    }

    STRUCTTYPE SectionDataType::getStructType() const
    {
        return mySectionType;
    }
    DATATYPE SectionDataType::getMemberType() const
    {
        return myDataType;
    }

    // 規格断面　リスト情報
    QStringList SectionDataType::standardSectionFileList(StandardSectionTable::listType type)
    {
        return standardSection->getSectionFileList(type);
    }
    QStringList SectionDataType::standardSectionNameList(const QString &fname)
    {
        return standardSection->getSectionNameList(fname);
    }
    bool SectionDataType::standardSectionNameExists(const QString &sname)
    {
        return standardSection->existsSectionName(sname);
    }
    QString SectionDataType::standardSteelSectionName(const QString &bname, const QString &sname)
    {
        return standardSection->steelSectionName(bname, sname);
    }
    QString SectionDataType::standardSteelSectionValue(const QString &sname, StandardSectionTable::SteelFormat fm)
    {
        return standardSection->getSteelSectionValue(sname, fm);
    }
    QString SectionDataType::standardBaseSectionValue(const QString &sname, StandardSectionTable::BaseFormat fm)
    {
        return standardSection->getBaseSectionValue(sname, fm);
    }
    QString SectionDataType::standardIsolatorSectionValue(const QString &sname,
                                                          StandardSectionTable::IsolatorFormat fm)
    {
        return standardSection->getIsolatorSectionValue(sname, fm);
    }

    QHash<int, QHash<int, QVariant>> SectionDataType::getDefaultValuePatch() const
    {
        return defaultValuePatch;
    }
    QHash<int, QVariant> SectionDataType::getLineDefaultValues(int row) const
    {
        QHash<int, QVariant> linedefaultvalues = defaultValues;
        for (int col = 0; col < COLUMNS; ++col)
        {
            if (defaultValuePatch[row][col].isValid())
                linedefaultvalues.insert(col, defaultValuePatch[row][col]);
        }
        return linedefaultvalues;
    }
    void SectionDataType::setDefaultValuePatch(int row, int col, const QVariant &dvalue)
    {
        if (!dvalue.toString().isEmpty())
        {
            defaultValuePatch[row].insert(col, dvalue);
        }
        else
        {
            defaultValuePatch[row].remove(col);
        }
    }

    /* 柱断面：ＲＣ */
    ColumnRCTable::ColumnRCTable()
    {
        myDataType = DATATYPE::TPCOLUMN;
        mySectionType = STRUCTTYPE::STP_RC;

        COLUMNS = 42;
        nameCol = 1;
        spCol1 = 6;
        spCol2 = 24;
        spCol3 = -1;

        CREGULATION column_regulation[42] = {
            /* 0*/ {u8"No.", REGULATION::defaultColumnRc, u8"No"},
            /* 1*/ {u8"符号", REGULATION::defaultColumnRc, u8"LABEL"},
            /* 2*/ {u8"階範囲\n階～", REGULATION::defaultColumnRc, "FLOORFROM"},
            /* 3*/ {u8"階範囲\n～階", REGULATION::defaultColumnRc, "FLOORTO"},
            /* 4*/ {u8"仕上重量\n(N/m)", REGULATION::defaultColumnRc, "FINISHLOAD"},
            /* 5*/ {u8"コンクリート材種", REGULATION::defaultColumnRc, "CONC"},
            // i
            /* 6*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultColumnRc, "iDY"},
            /* 7*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultColumnRc, "iDZ"},
            /* 8*/ {u8"主筋本数\n(y) n", REGULATION::defaultColumnRc, "iMAINBAR1"},
            /* 9*/ {u8"主筋本数\n(z) n", REGULATION::defaultColumnRc, "iMAINBAR2"},
            /*10*/ {u8"主筋径\nD", REGULATION::defaultColumnRc, "iMAINBARPHI"},
            /*11*/ {u8"芯鉄筋位置\nay (mm)", REGULATION::defaultColumnRc, "iAXIALCENTERY"},
            /*12*/ {u8"芯鉄筋位置\naz (mm)", REGULATION::defaultColumnRc, "iAXIALCENTERZ"},
            /*13*/ {u8"芯鉄筋\naDy (mm)", REGULATION::defaultColumnRc, "iAXIALDY"},
            /*14*/ {u8"芯鉄筋\naDz (mm)", REGULATION::defaultColumnRc, "iAXIALDZ"},
            /*15*/ {u8"芯鉄筋本数\n(y) n", REGULATION::defaultColumnRc, "iAXIALBAR1"},
            /*16*/ {u8"芯鉄筋本数\n(z) n", REGULATION::defaultColumnRc, "iAXIALBAR2"},
            /*17*/ {u8"芯鉄筋径\nD", REGULATION::defaultColumnRc, "iAXIALBARPHI"},
            /*18*/ {u8"帯筋本数\n(y) n", REGULATION::defaultColumnRc, "iHOOPY"},
            /*19*/ {u8"帯筋本数\n(z) n", REGULATION::defaultColumnRc, "iHOOPZ"},
            /*20*/ {u8"帯筋径\nD", REGULATION::defaultColumnRc, "iHOOPPHI"},
            /*21*/ {u8"帯筋ピッチ\n@", REGULATION::defaultColumnRc, "iHOOPPITCH"},
            /*22*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultColumnRc, "iRK"},
            /*23*/ {u8"主筋間隔\nRY (mm)", REGULATION::defaultColumnRc, "iRY"},
            // j
            /*24*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultColumnRc, "jDY", spCol1},
            /*25*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultColumnRc, "jDZ", spCol1 + 1},
            /*26*/ {u8"主筋本数\n(y) n", REGULATION::defaultColumnRc, "jDY", spCol1 + 2},
            /*27*/ {u8"主筋本数\n(z) n", REGULATION::defaultColumnRc, "jMAINBAR2", spCol1 + 3},
            /*28*/ {u8"主筋径\nD", REGULATION::defaultColumnRc, "jMAINBARPHI", spCol1 + 4},
            /*29*/ {u8"芯鉄筋位置\nay (mm)", REGULATION::defaultColumnRc, "jAXIALCENTERY", spCol1 + 5},
            /*30*/ {u8"芯鉄筋位置\naz (mm)", REGULATION::defaultColumnRc, "jAXIALCENTERZ", spCol1 + 6},
            /*31*/ {u8"芯鉄筋\naDy (mm)", REGULATION::defaultColumnRc, "jAXIALDY", spCol1 + 7},
            /*32*/ {u8"芯鉄筋\naDz (mm)", REGULATION::defaultColumnRc, "jAXIALDZ", spCol1 + 8},
            /*33*/ {u8"芯鉄筋本数\n(y) n", REGULATION::defaultColumnRc, "jAXIALBAR1", spCol1 + 9},
            /*34*/ {u8"芯鉄筋本数\n(z) n", REGULATION::defaultColumnRc, "jAXIALBAR2", spCol1 + 10},
            /*35*/ {u8"芯鉄筋径\nD", REGULATION::defaultColumnRc, "jAXIALBARPHI", spCol1 + 11},
            /*36*/ {u8"帯筋本数\n(y) n", REGULATION::defaultColumnRc, "jHOOPY", spCol1 + 12},
            /*37*/ {u8"帯筋本数\n(z) n", REGULATION::defaultColumnRc, "jHOOPZ", spCol1 + 13},
            /*38*/ {u8"帯筋径\nD", REGULATION::defaultColumnRc, "jHOOPPHI", spCol1 + 14},
            /*39*/ {u8"帯筋ピッチ\n@", REGULATION::defaultColumnRc, "jHOOPPITCH", spCol1 + 15},
            /*40*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultColumnRc, "jRK", spCol1 + 16},
            /*41*/ {u8"主筋間隔\nRY (mm)", REGULATION::defaultColumnRc, "jRY", spCol1 + 17}};

        populateList(column_regulation);

        referenceColumns.insert(taggedColumnNumber["FLOORFROM"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["FLOORTO"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["CONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["iMAINBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iAXIALBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jMAINBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jAXIALBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jHOOPPHI"], DATATYPE::TPMAREIN);
    }

    /* 柱断面：Ｓ */
    ColumnSTable::ColumnSTable()
    {
        myDataType = DATATYPE::TPCOLUMN;
        mySectionType = STRUCTTYPE::STP_S;
        COLUMNS = 29;
        nameCol = 1;
        spCol1 = 7;
        spCol2 = 18;
        spCol3 = -1;

        CREGULATION column_regulation[29] = {
            /* 0*/ {"No.", REGULATION::defaultColumnS, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultColumnS, "LABEL"},
            /* 2*/ {u8"階範囲\n階～", REGULATION::defaultColumnS, "FLOORFROM"},
            /* 3*/ {u8"階範囲\n～階", REGULATION::defaultColumnS, "FLOORTO"},
            /* 4*/ {u8"仕上重量\n(N/m)", REGULATION::defaultColumnS, "FINISHLOAD"},
            /* 5*/ {u8"鉄骨断面切り替え高さ\ni端 (mm)", REGULATION::defaultColumnS, "iLENGTH"},
            /* 6*/ {u8"柱脚リスト", REGULATION::defaultColumnS, "BASELIST"},
            // i
            /* 7*/ {u8"鉄骨材種", REGULATION::defaultColumnS, "iSTEEL"},
            /* 8*/ {u8"断面タイプ(y)", REGULATION::defaultColumnS, "iSHAPELISTNAME"},
            /* 9*/ {u8"断面選択(y)", REGULATION::defaultColumnS, "iSTANDARDSHAPESTEEL"},
            /*10*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultColumnS, "iH"},
            /*11*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultColumnS, "iB"},
            /*12*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultColumnS, "iT1"},
            /*13*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultColumnS, "iT2"},
            /*14*/ {u8"十字断面(z)\nH (mm)", REGULATION::defaultColumnS, "iH_T"},
            /*15*/ {u8"十字断面(z)\nB (mm)", REGULATION::defaultColumnS, "iB_T"},
            /*16*/ {u8"十字断面(z)\nt1 (mm)", REGULATION::defaultColumnS, "iT1_T"},
            /*17*/ {u8"十字断面(z)\nt2 (mm)", REGULATION::defaultColumnS, "iT2_T"},
            // j
            /*18*/ {u8"鉄骨材種", REGULATION::defaultColumnS, "jSTEEL", spCol1},
            /*19*/ {u8"断面タイプ(y)", REGULATION::defaultColumnS, "jSHAPELISTNAME", spCol1 + 1},
            /*20*/ {u8"断面選択(y)", REGULATION::defaultColumnS, "jSTANDARDSHAPESTEEL", spCol1 + 2},
            /*21*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultColumnS, "jH", spCol1 + 3},
            /*22*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultColumnS, "jB", spCol1 + 4},
            /*23*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultColumnS, "jT1", spCol1 + 5},
            /*24*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultColumnS, "jT2", spCol1 + 6},
            /*25*/ {u8"十字断面(z)\nH (mm)", REGULATION::defaultColumnS, "jH_T", spCol1 + 7},
            /*26*/ {u8"十字断面(z)\nB (mm)", REGULATION::defaultColumnS, "jB_T", spCol1 + 8},
            /*27*/ {u8"十字断面(z)\nt1 (mm)", REGULATION::defaultColumnS, "jT1_T", spCol1 + 9},
            /*28*/ {u8"十字断面(z)\nt2 (mm)", REGULATION::defaultColumnS, "jT2_T", spCol1 + 10}};

        populateList(column_regulation);

        QStringList slist_steelshape{REGULATION::defaultColumnSSteelShape};
        slist_steelshape.append(standardSectionFileList(StandardSectionTable::listType::STEEL));

        limitedStrings.insert(taggedColumnNumber["iSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["iSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["jSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["jSTANDARDSHAPESTEEL"], {""});

        referenceColumns.insert(taggedColumnNumber["FLOORFROM"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["FLOORTO"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["BASELIST"], DATATYPE::TPBASE);
        referenceColumns.insert(taggedColumnNumber["iSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jSTEEL"], DATATYPE::TPMASTEEL);

        shapeTypeColumns << taggedColumnNumber["iSHAPELISTNAME"] << taggedColumnNumber["jSHAPELISTNAME"];
    }

    /* 柱断面：ＳＲＣ */
    ColumnSRCTable::ColumnSRCTable()
    {
        myDataType = DATATYPE::TPCOLUMN;
        mySectionType = STRUCTTYPE::STP_SRC;
        COLUMNS = 79;
        nameCol = 1;
        spCol1 = 9;
        spCol2 = 44;
        spCol3 = -1;

        CREGULATION column_regulation[79] = {
            /* 0*/ {"No.", REGULATION::defaultColumnSrc, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultColumnSrc, "LABEL"},
            /* 2*/ {u8"階範囲\n階～", REGULATION::defaultColumnSrc, "FLOORFROM"},
            /* 3*/ {u8"階範囲\n～階", REGULATION::defaultColumnSrc, "FLOORTO"},
            /* 4*/ {u8"仕上重量\n(N/m)", REGULATION::defaultColumnSrc, "FINISHLOAD"},
            /* 5*/ {u8"コンクリート材種", REGULATION::defaultColumnSrc, "CONC"},
            /* 6*/ {u8"中詰コンクリート\n材種", REGULATION::defaultColumnSrc, "FILLEDCONC"},
            /* 7*/ {u8"鉄骨断面切り替え高さ\ni端 (mm)", REGULATION::defaultColumnSrc, "iLENGTH"},
            /* 8*/ {u8"柱脚リスト", REGULATION::defaultColumnSrc, "BASELIST"},
            // i
            /* 9*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultColumnSrc, "iDY"},
            /*10*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultColumnSrc, "iDZ"},
            /*11*/ {u8"主筋本数\n(y) n", REGULATION::defaultColumnSrc, "iMAINBAR1"},
            /*12*/ {u8"主筋本数\n(z) n", REGULATION::defaultColumnSrc, "iMAINBAR2"},
            /*13*/ {u8"主筋径\nD", REGULATION::defaultColumnSrc, "iMAINBARPHI"},
            /*14*/ {u8"芯鉄筋位置\nay (mm)", REGULATION::defaultColumnSrc, "iAXIALCENTERY"},
            /*15*/ {u8"芯鉄筋位置\naz (mm)", REGULATION::defaultColumnSrc, "iAXIALCENTERZ"},
            /*16*/ {u8"芯鉄筋\naDy (mm)", REGULATION::defaultColumnSrc, "iAXIALDY"},
            /*17*/ {u8"芯鉄筋\naDz (mm)", REGULATION::defaultColumnSrc, "iAXIALDZ"},
            /*18*/ {u8"芯鉄筋本数\n(y) n", REGULATION::defaultColumnSrc, "iAXIALBAR1"},
            /*19*/ {u8"芯鉄筋本数\n(z) n", REGULATION::defaultColumnSrc, "iAXIALBAR2"},
            /*20*/ {u8"芯鉄筋径\nD", REGULATION::defaultColumnSrc, "iAXIALBARPHI"},
            /*21*/ {u8"帯筋本数\n(y) n", REGULATION::defaultColumnSrc, "iHOOPY"},
            /*22*/ {u8"帯筋本数\n(z) n", REGULATION::defaultColumnSrc, "iHOOPZ"},
            /*23*/ {u8"帯筋径\nD", REGULATION::defaultColumnSrc, "iHOOPPHI"},
            /*24*/ {u8"帯筋ピッチ\n@", REGULATION::defaultColumnSrc, "iHOOPPITCH"},
            /*25*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultColumnSrc, "iRK"},
            /*26*/ {u8"主筋間隔\nRY (mm)", REGULATION::defaultColumnSrc, "iRY"},
            /*27*/ {u8"鉄骨材種", REGULATION::defaultColumnSrc, "iSTEEL"},
            /*28*/ {u8"断面タイプ(y)", REGULATION::defaultColumnSrc, "iSHAPELISTNAME"},
            /*29*/ {u8"断面選択(y)", REGULATION::defaultColumnSrc, "iSTANDARDSHAPESTEEL"},
            /*30*/ {u8"鉄骨断面(y)\nH (mm)", REGULATION::defaultColumnSrc, "iH"},
            /*31*/ {u8"鉄骨断面(y)\nB (mm)", REGULATION::defaultColumnSrc, "iB"},
            /*32*/ {u8"鉄骨断面(y)\nt1 (mm)", REGULATION::defaultColumnSrc, "iT1"},
            /*33*/ {u8"鉄骨断面(y)\nt2 (mm)", REGULATION::defaultColumnSrc, "iT2"},
            /*34*/ {u8"鉄骨位置(y)\nRy (mm)", REGULATION::defaultColumnSrc, "iCOODINATEY"},
            /*35*/ {u8"鉄骨位置(y)\nRz (mm)", REGULATION::defaultColumnSrc, "iCOODINATEZ"},
            /*36*/ {u8"断面タイプ(z)", REGULATION::defaultColumnSrc, "iSHAPELISTNAME_T"},
            /*37*/ {u8"断面選択(z)", REGULATION::defaultColumnSrc, "iSTANDARDSHAPESTEEL_T"},
            /*38*/ {u8"鉄骨断面(z)\nH (mm)", REGULATION::defaultColumnSrc, "iH_T"},
            /*39*/ {u8"鉄骨断面(z)\nB (mm)", REGULATION::defaultColumnSrc, "iB_T"},
            /*40*/ {u8"鉄骨断面(z)\nt1 (mm)", REGULATION::defaultColumnSrc, "iT1_T"},
            /*41*/ {u8"鉄骨断面(z)\nt2 (mm)", REGULATION::defaultColumnSrc, "iT2_T"},
            /*42*/ {u8"鉄骨位置(z)\nRy (mm)", REGULATION::defaultColumnSrc, "iCOODINATEY_T"},
            /*43*/ {u8"鉄骨位置(z)\nRz (mm)", REGULATION::defaultColumnSrc, "iCOODINATEZ_T"},
            // j
            /*44*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultColumnSrc, "jDY", spCol1},
            /*45*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultColumnSrc, "jDZ", spCol1 + 1},
            /*46*/ {u8"主筋本数\n(y) n", REGULATION::defaultColumnSrc, "jMAINBAR1", spCol1 + 2},
            /*47*/ {u8"主筋本数\n(z) n", REGULATION::defaultColumnSrc, "jMAINBAR2", spCol1 + 3},
            /*48*/ {u8"主筋径\nD", REGULATION::defaultColumnSrc, "jMAINBARPHI", spCol1 + 4},
            /*40*/ {u8"芯鉄筋位置\nay (mm)", REGULATION::defaultColumnSrc, "jAXIALCENTERY", spCol1 + 5},
            /*50*/ {u8"芯鉄筋位置\naz (mm)", REGULATION::defaultColumnSrc, "jAXIALCENTERZ", spCol1 + 6},
            /*51*/ {u8"芯鉄筋\naDy (mm)", REGULATION::defaultColumnSrc, "jAXIALDY", spCol1 + 7},
            /*52*/ {u8"芯鉄筋\naDz (mm)", REGULATION::defaultColumnSrc, "jAXIALDZ", spCol1 + 8},
            /*53*/ {u8"芯鉄筋本数\n(y) n", REGULATION::defaultColumnSrc, "jAXIALBAR1", spCol1 + 9},
            /*54*/ {u8"芯鉄筋本数\n(z) n", REGULATION::defaultColumnSrc, "jAXIALBAR2", spCol1 + 10},
            /*55*/ {u8"芯鉄筋径\nD", REGULATION::defaultColumnSrc, "jAXIALBARPHI", spCol1 + 11},
            /*56*/ {u8"帯筋本数\n(y) n", REGULATION::defaultColumnSrc, "jHOOPY", spCol1 + 12},
            /*57*/ {u8"帯筋本数\n(z) n", REGULATION::defaultColumnSrc, "jHOOPZ", spCol1 + 13},
            /*58*/ {u8"帯筋径\nD", REGULATION::defaultColumnSrc, "jHOOPPHI", spCol1 + 14},
            /*59*/ {u8"帯筋ピッチ\n@", REGULATION::defaultColumnSrc, "jHOOPPITCH", spCol1 + 15},
            /*60*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultColumnSrc, "jRK", spCol1 + 16},
            /*61*/ {u8"主筋間隔\nRY (mm)", REGULATION::defaultColumnSrc, "jRY", spCol1 + 17},
            /*62*/ {u8"鉄骨材種", REGULATION::defaultColumnSrc, "jSTEEL", spCol1 + 18},
            /*63*/ {u8"断面タイプ(y)", REGULATION::defaultColumnSrc, "jSHAPELISTNAME", spCol1 + 19},
            /*64*/ {u8"断面タイプ(y)", REGULATION::defaultColumnSrc, "jSTANDARDSHAPESTEEL", spCol1 + 20},
            /*65*/ {u8"鉄骨断面(y)\nH (mm)", REGULATION::defaultColumnSrc, "jH", spCol1 + 21},
            /*66*/ {u8"鉄骨断面(y)\nB (mm)", REGULATION::defaultColumnSrc, "jB", spCol1 + 22},
            /*67*/ {u8"鉄骨断面(y)\nt1 (mm)", REGULATION::defaultColumnSrc, "jT1", spCol1 + 23},
            /*68*/ {u8"鉄骨断面(y)\nt2 (mm)", REGULATION::defaultColumnSrc, "jT2", spCol1 + 24},
            /*69*/ {u8"鉄骨位置(y)\nRy (mm)", REGULATION::defaultColumnSrc, "jCOODINATEY", spCol1 + 25},
            /*70*/ {u8"鉄骨位置(y)\nRz (mm)", REGULATION::defaultColumnSrc, "jCOODINATEZ", spCol1 + 26},
            /*71*/ {u8"断面タイプ(z)", REGULATION::defaultColumnSrc, "jSHAPELISTNAME_T", spCol1 + 27},
            /*72*/ {u8"断面選択(z)", REGULATION::defaultColumnSrc, "jSTANDARDSHAPESTEEL_T", spCol1 + 28},
            /*73*/ {u8"鉄骨断面(z)\nH (mm)", REGULATION::defaultColumnSrc, "jH_T", spCol1 + 29},
            /*74*/ {u8"鉄骨断面(z)\nB (mm)", REGULATION::defaultColumnSrc, "jB_T", spCol1 + 30},
            /*75*/ {u8"鉄骨断面(z)\nt1 (mm)", REGULATION::defaultColumnSrc, "jT1_T", spCol1 + 31},
            /*76*/ {u8"鉄骨断面(z)\nt2 (mm)", REGULATION::defaultColumnSrc, "jT2_T", spCol1 + 32},
            /*77*/ {u8"鉄骨位置(z)\nRy (mm)", REGULATION::defaultColumnSrc, "jCOODINATEY_T", spCol1 + 33},
            /*78*/ {u8"鉄骨位置(z)\nRz (mm)", REGULATION::defaultColumnSrc, "jCOODINATEZ_T", spCol1 + 34}};

        populateList(column_regulation);

        QStringList slist_steelshape{REGULATION::defaultColumnSrcSteelShape};
        slist_steelshape.append(standardSectionFileList(StandardSectionTable::listType::COLUMNSRCSTEEL));
        QStringList slist_steelshapeT{REGULATION::defaultColumnSrcSteelShapeT};
        slist_steelshapeT.append(standardSectionFileList(StandardSectionTable::listType::COLUMNSRCSTEELT));

        limitedStrings.insert(taggedColumnNumber["iSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["iSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["iSHAPELISTNAME_T"], slist_steelshapeT);
        limitedStrings.insert(taggedColumnNumber["iSTANDARDSHAPESTEEL_T"], {""});
        limitedStrings.insert(taggedColumnNumber["jSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["jSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["jSHAPELISTNAME_T"], slist_steelshapeT);
        limitedStrings.insert(taggedColumnNumber["jSTANDARDSHAPESTEEL_T"], {""});

        referenceColumns.insert(taggedColumnNumber["FLOORFROM"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["FLOORTO"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["CONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["FILLEDCONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["BASELIST"], DATATYPE::TPBASE);
        referenceColumns.insert(taggedColumnNumber["iMAINBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iAXIALBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jMAINBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jAXIALBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jSTEEL"], DATATYPE::TPMASTEEL);

        shapeTypeColumns << taggedColumnNumber["iSHAPELISTNAME"] << taggedColumnNumber["iSHAPELISTNAME_T"]
                         << taggedColumnNumber["jSHAPELISTNAME"] << taggedColumnNumber["jSHAPELISTNAME_T"];
    }

    /* 柱断面：ＣＦＴ */
    ColumnCFTTable::ColumnCFTTable()
    {
        myDataType = DATATYPE::TPCOLUMN;
        mySectionType = STRUCTTYPE::STP_CFT;
        COLUMNS = 22;
        nameCol = 1;
        spCol1 = 8;
        spCol2 = 15;
        spCol3 = -1;

        CREGULATION column_regulation[22] = {
            /* 0*/ {"No.", REGULATION::defaultColumnCft, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultColumnCft, "LABEL"},
            /* 2*/ {u8"階範囲\n階～", REGULATION::defaultColumnCft, "FLOORFROM"},
            /* 3*/ {u8"階範囲\n～階", REGULATION::defaultColumnCft, "FLOORTO"},
            /* 4*/ {u8"仕上重量\n(N/m)", REGULATION::defaultColumnCft, "FINISHLOAD"},
            /* 5*/ {u8"中詰コンクリート\n材種", REGULATION::defaultColumnCft, "FILLEDCONC"},
            /* 6*/ {u8"鉄骨断面切り替え高さ\ni端 (mm)", REGULATION::defaultColumnCft, "iLENGTH"},
            /* 7*/ {u8"柱脚リスト", REGULATION::defaultColumnCft, "BASELIST"},
            // i
            /* 8*/ {u8"鉄骨材種", REGULATION::defaultColumnCft, "iSTEEL"},
            /* 9*/ {u8"断面タイプ", REGULATION::defaultColumnCft, "iSHAPELISTNAME"},
            /*10*/ {u8"断面選択", REGULATION::defaultColumnCft, "iSTANDARDSHAPESTEEL"},
            /*11*/ {u8"鉄骨断面(y)\nH (mm)", REGULATION::defaultColumnCft, "iH"},
            /*12*/ {u8"鉄骨断面(y)\nB (mm)", REGULATION::defaultColumnCft, "iB"},
            /*13*/ {u8"鉄骨断面(y)\nt1 (mm)", REGULATION::defaultColumnCft, "iT1"},
            /*14*/ {u8"鉄骨断面(y)\nt2 (mm)", REGULATION::defaultColumnCft, "iT2"},
            // j
            /*15*/ {u8"鉄骨材種", REGULATION::defaultColumnCft, "jSTEEL", spCol1},
            /*16*/ {u8"断面タイプ", REGULATION::defaultColumnCft, "jSHAPELISTNAME", spCol1 + 1},
            /*17*/ {u8"断面選択", REGULATION::defaultColumnCft, "jSTANDARDSHAPESTEEL", spCol1 + 2},
            /*18*/ {u8"鉄骨断面(y)\nH (mm)", REGULATION::defaultColumnCft, "jH", spCol1 + 3},
            /*19*/ {u8"鉄骨断面(y)\nB (mm)", REGULATION::defaultColumnCft, "jB", spCol1 + 4},
            /*20*/ {u8"鉄骨断面(y)\nt1 (mm)", REGULATION::defaultColumnCft, "jT1", spCol1 + 5},
            /*21*/ {u8"鉄骨断面(y)\nt2 (mm)", REGULATION::defaultColumnCft, "jT2", spCol1 + 6}};

        populateList(column_regulation);

        QStringList slist_steelshape{REGULATION::defaultColumnCftSteelShape};
        slist_steelshape.append(standardSectionFileList(StandardSectionTable::listType::CFT));

        limitedStrings.insert(taggedColumnNumber["iSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["iSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["jSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["jSTANDARDSHAPESTEEL"], {""});

        referenceColumns.insert(taggedColumnNumber["FLOORFROM"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["FLOORTO"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["FILLEDCONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["BASELIST"], DATATYPE::TPBASE);
        referenceColumns.insert(taggedColumnNumber["iSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jSTEEL"], DATATYPE::TPMASTEEL);

        shapeTypeColumns << taggedColumnNumber["iSHAPELISTNAME"] << taggedColumnNumber["jSHAPELISTNAME"];
    }

    GirderRCTable::GirderRCTable()
    {
        myDataType = DATATYPE::TPGIRDER;
        mySectionType = STRUCTTYPE::STP_RC;
        COLUMNS = 46;
        nameCol = 1;
        spCol1 = 7;
        spCol2 = 20;
        spCol3 = 33;

        CREGULATION column_regulation[46] = {
            /* 0*/ {"No.", REGULATION::defaultGirderRc, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultGirderRc, "LABEL"},
            /* 2*/ {u8"階範囲\n階～", REGULATION::defaultGirderRc, "FLOORFROM"},
            /* 3*/ {u8"階範囲\n～階", REGULATION::defaultGirderRc, "FLOORTO"},
            /* 4*/ {u8"一般／片持", REGULATION::defaultGirderRc, "CANTILEVER"},
            /* 5*/ {u8"仕上重量\n(N/m)", REGULATION::defaultGirderRc, "FINISHLOAD"},
            /* 6*/ {u8"コンクリート材種", REGULATION::defaultGirderRc, "CONC"},
            // i
            /* 7*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultGirderRc, "iDY"},
            /* 8*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultGirderRc, "iDZ"},
            /* 9*/ {u8"上端筋\nn", REGULATION::defaultGirderRc, "iMAINBAR1"},
            /*11*/ {u8"下端筋\nn", REGULATION::defaultGirderRc, "iMAINBAR2"},
            /*10*/ {u8"上端筋径\nD", REGULATION::defaultGirderRc, "iMAINBARPHI1"},
            /*12*/ {u8"下端筋径\nD", REGULATION::defaultGirderRc, "iMAINBARPHI2"},
            /*13*/ {u8"帯筋本数\nn", REGULATION::defaultGirderRc, "iHOOPZ"},
            /*14*/ {u8"帯筋径\nD", REGULATION::defaultGirderRc, "iHOOPPHI"},
            /*15*/ {u8"帯筋ピッチ\n@", REGULATION::defaultGirderRc, "iHOOPPITCH"},
            /*16*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultGirderRc, "iRK"},
            /*17*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultGirderRc, "iRKU"},
            /*18*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultGirderRc, "iRKB"},
            /*19*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultGirderRc, "iRY"},
            // m
            /*20*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultGirderRc, "mDY", spCol1},
            /*21*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultGirderRc, "mDZ", spCol1 + 1},
            /*22*/ {u8"上端筋\nn", REGULATION::defaultGirderRc, "mMAINBAR1", spCol1 + 2},
            /*23*/ {u8"下端筋\nn", REGULATION::defaultGirderRc, "mMAINBAR2", spCol1 + 3},
            /*24*/ {u8"上端筋径\nD", REGULATION::defaultGirderRc, "mMAINBARPHI1", spCol1 + 4},
            /*25*/ {u8"下端筋径\nD", REGULATION::defaultGirderRc, "mMAINBARPHI2", spCol1 + 5},
            /*26*/ {u8"帯筋本数\nn", REGULATION::defaultGirderRc, "mHOOPZ", spCol1 + 6},
            /*27*/ {u8"帯筋径\nD", REGULATION::defaultGirderRc, "mHOOPPHI", spCol1 + 7},
            /*28*/ {u8"帯筋ピッチ\n@", REGULATION::defaultGirderRc, "mHOOPPITCH", spCol1 + 8},
            /*29*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultGirderRc, "mRK", spCol1 + 9},
            /*30*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultGirderRc, "mRKU", spCol1 + 10},
            /*31*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultGirderRc, "mRKB", spCol1 + 11},
            /*32*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultGirderRc, "mRY", spCol1 + 12},
            // j
            /*33*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultGirderRc, "jDY", spCol1},
            /*34*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultGirderRc, "jDZ", spCol1 + 1},
            /*35*/ {u8"上端筋\nn", REGULATION::defaultGirderRc, "jMAINBAR1", spCol1 + 2},
            /*36*/ {u8"下端筋\nn", REGULATION::defaultGirderRc, "jMAINBAR2", spCol1 + 3},
            /*37*/ {u8"上端筋径\nD", REGULATION::defaultGirderRc, "jMAINBARPHI1", spCol1 + 4},
            /*38*/ {u8"下端筋径\nD", REGULATION::defaultGirderRc, "jMAINBARPHI2", spCol1 + 5},
            /*39*/ {u8"帯筋本数\nn", REGULATION::defaultGirderRc, "jHOOPZ", spCol1 + 6},
            /*40*/ {u8"帯筋径\nD", REGULATION::defaultGirderRc, "jHOOPPHI", spCol1 + 7},
            /*41*/ {u8"帯筋ピッチ\n@", REGULATION::defaultGirderRc, "jHOOPPITCH", spCol1 + 8},
            /*42*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultGirderRc, "jRK", spCol1 + 9},
            /*43*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultGirderRc, "jRKU", spCol1 + 10},
            /*44*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultGirderRc, "jRKB", spCol1 + 11},
            /*45*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultGirderRc, "jRY", spCol1 + 12}};

        populateList(column_regulation);

        limitedStrings.insert(taggedColumnNumber["CANTILEVER"], REGULATION::defaultGirderCantilever);

        referenceColumns.insert(taggedColumnNumber["FLOORFROM"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["FLOORTO"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["CONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["iMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jHOOPPHI"], DATATYPE::TPMAREIN);
    }

    /* 梁断面：Ｓ */
    GirderSTable::GirderSTable()
    {
        myDataType = DATATYPE::TPGIRDER;
        mySectionType = STRUCTTYPE::STP_S;
        COLUMNS = 34;
        nameCol = 1;
        spCol1 = 10;
        spCol2 = 18;
        spCol3 = 26;

        CREGULATION column_regulation[34] = {
            /* 0*/ {"No.", REGULATION::defaultGirderS, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultGirderS, "LABEL"},
            /* 2*/ {u8"階範囲\n階～", REGULATION::defaultGirderS, "FLOORFROM"},
            /* 3*/ {u8"階範囲\n～階", REGULATION::defaultGirderS, "FLOORTO"},
            /* 4*/ {u8"一般／片持", REGULATION::defaultGirderS, "CANTILEVER"},
            /* 5*/ {u8"仕上重量\n(N/m)", REGULATION::defaultGirderS, "FINISHLOAD"},
            /* 6*/ {u8"鉄骨断面切り替え長さ\ni端 (mm)", REGULATION::defaultGirderS, "iLENGTH"},
            /* 7*/ {u8"鉄骨断面切り替え長さ\nj端 (mm)", REGULATION::defaultGirderS, "jLENGTH"},
            /* 8*/ {u8"梁端補剛\ni端", REGULATION::defaultGirderS, "iENDSTIFFENER"},
            /* 9*/ {u8"梁端補剛\nj端", REGULATION::defaultGirderS, "jENDSTIFFENER"},
            // i
            /*10*/ {u8"鉄骨材種", REGULATION::defaultGirderS, "iSTEEL"},
            /*11*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultGirderS, "iSTEELWEB"},
            /*12*/ {u8"断面タイプ", REGULATION::defaultGirderS, "iSHAPELISTNAME"},
            /*13*/ {u8"断面選択", REGULATION::defaultGirderS, "iSTANDARDSHAPESTEEL"},
            /*14*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultGirderS, "iH"},
            /*15*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultGirderS, "iB"},
            /*16*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultGirderS, "iT1"},
            /*17*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultGirderS, "iT2"},
            // m
            /*18*/ {u8"鉄骨材種", REGULATION::defaultGirderS, "mSTEEL", spCol1},
            /*19*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultGirderS, "mSTEELWEB", spCol1 + 1},
            /*20*/ {u8"断面タイプ", REGULATION::defaultGirderS, "mSHAPELISTNAME", spCol1 + 2},
            /*21*/ {u8"断面選択", REGULATION::defaultGirderS, "mSTANDARDSHAPESTEEL", spCol1 + 3},
            /*22*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultGirderS, "mH", spCol1 + 4},
            /*23*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultGirderS, "mB", spCol1 + 5},
            /*24*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultGirderS, "mT1", spCol1 + 6},
            /*25*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultGirderS, "mT2", spCol1 + 7},
            // j
            /*26*/ {u8"鉄骨材種", REGULATION::defaultGirderS, "jSTEEL", spCol1},
            /*27*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultGirderS, "jSTEELWEB", spCol1 + 1},
            /*28*/ {u8"断面タイプ", REGULATION::defaultGirderS, "jSHAPELISTNAME", spCol1 + 2},
            /*29*/ {u8"断面選択", REGULATION::defaultGirderS, "jSTANDARDSHAPESTEEL", spCol1 + 3},
            /*30*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultGirderS, "jH", spCol1 + 4},
            /*31*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultGirderS, "jB", spCol1 + 5},
            /*32*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultGirderS, "jT1", spCol1 + 6},
            /*33*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultGirderS, "jT2", spCol1 + 7}};

        populateList(column_regulation);

        QStringList slist_steelshape{REGULATION::defaultGirderSSteelShape};
        slist_steelshape.append(standardSectionFileList(StandardSectionTable::listType::STEEL));

        limitedStrings.insert(taggedColumnNumber["CANTILEVER"], REGULATION::defaultGirderCantilever);
        limitedStrings.insert(taggedColumnNumber["iENDSTIFFENER"], REGULATION::defaultGirderSStiffner);
        limitedStrings.insert(taggedColumnNumber["jENDSTIFFENER"], REGULATION::defaultGirderSStiffner);
        limitedStrings.insert(taggedColumnNumber["iSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["iSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["mSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["mSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["jSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["jSTANDARDSHAPESTEEL"], {""});

        referenceColumns.insert(taggedColumnNumber["FLOORFROM"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["FLOORTO"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["iSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["iSTEELWEB"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["mSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["mSTEELWEB"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jSTEELWEB"], DATATYPE::TPMASTEEL);

        shapeTypeColumns << taggedColumnNumber["iSHAPELISTNAME"] << taggedColumnNumber["mSHAPELISTNAME"]
                         << taggedColumnNumber["jSHAPELISTNAME"];
    }

    /* 梁断面：ＳＲＣ */
    GirderSRCTable::GirderSRCTable()
    {
        myDataType = DATATYPE::TPGIRDER;
        mySectionType = STRUCTTYPE::STP_SRC;
        COLUMNS = 81;
        nameCol = 1;
        spCol1 = 9;
        spCol2 = 33;
        spCol3 = 57;

        CREGULATION column_regulation[81] = {
            /* 0*/ {"No.", REGULATION::defaultGirderSrc, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultGirderSrc, "LABEL"},
            /* 2*/ {u8"階範囲\n階～", REGULATION::defaultGirderSrc, "FLOORFROM"},
            /* 3*/ {u8"階範囲\n～階", REGULATION::defaultGirderSrc, "FLOORTO"},
            /* 4*/ {u8"一般／片持", REGULATION::defaultGirderSrc, "CANTILEVER"},
            /* 5*/ {u8"仕上重量\n(N/m)", REGULATION::defaultGirderSrc, "FINISHLOAD"},
            /* 6*/ {u8"コンクリート材種", REGULATION::defaultGirderSrc, "CONC"},
            /* 7*/ {u8"鉄骨断面切り替え長さ\ni端 (mm)", REGULATION::defaultGirderSrc, "iLENGTH"},
            /* 8*/ {u8"鉄骨断面切り替え長さ\nj端 (mm)", REGULATION::defaultGirderSrc, "jLENGTH"},
            // i
            /* 9*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultGirderSrc, "iDY"},
            /*10*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultGirderSrc, "iDZ"},
            /*11*/ {u8"上端筋\nn", REGULATION::defaultGirderSrc, "iMAINBAR1"},
            /*13*/ {u8"下端筋\nn", REGULATION::defaultGirderSrc, "iMAINBAR2"},
            /*12*/ {u8"上端筋径\nD", REGULATION::defaultGirderSrc, "iMAINBARPHI1"},
            /*14*/ {u8"下端筋径\nD", REGULATION::defaultGirderSrc, "iMAINBARPHI2"},
            /*15*/ {u8"帯筋本数\nn", REGULATION::defaultGirderSrc, "iHOOPZ"},
            /*16*/ {u8"帯筋径\nD", REGULATION::defaultGirderSrc, "iHOOPPHI"},
            /*17*/ {u8"帯筋ピッチ\n@", REGULATION::defaultGirderSrc, "iHOOPPITCH"},
            /*18*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultGirderSrc, "iRK"},
            /*19*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultGirderSrc, "iRKU"},
            /*20*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultGirderSrc, "iRKB"},
            /*21*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultGirderSrc, "iRY"},
            /*22*/ {u8"主筋距離\nRY1 (mm)", REGULATION::defaultGirderSrc, "iRY1"},
            /*23*/ {u8"鉄骨材種", REGULATION::defaultGirderSrc, "iSTEEL"},
            /*24*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultGirderSrc, "iSTEELWEB"},
            /*25*/ {u8"断面タイプ", REGULATION::defaultGirderSrc, "iSHAPELISTNAME"},
            /*26*/ {u8"断面選択", REGULATION::defaultGirderSrc, "iSTANDARDSHAPESTEEL"},
            /*27*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultGirderSrc, "iH"},
            /*28*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultGirderSrc, "iB"},
            /*29*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultGirderSrc, "iT1"},
            /*30*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultGirderSrc, "iT2"},
            /*31*/ {u8"鉄骨位置：\nRy (mm)", REGULATION::defaultGirderSrc, "iCOODINATEY"},
            /*32*/ {u8"鉄骨位置：\nRz (mm)", REGULATION::defaultGirderSrc, "iCOODINATEZ"},
            // c
            /*33*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultGirderSrc, "mDY", spCol1},
            /*34*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultGirderSrc, "mDZ", spCol1 + 1},
            /*35*/ {u8"上端筋\nn", REGULATION::defaultGirderSrc, "mMAINBAR1", spCol1 + 2},
            /*36*/ {u8"下端筋\nn", REGULATION::defaultGirderSrc, "mMAINBAR2", spCol1 + 3},
            /*37*/ {u8"上端筋径\nD", REGULATION::defaultGirderSrc, "mMAINBARPHI1", spCol1 + 4},
            /*38*/ {u8"下端筋径\nD", REGULATION::defaultGirderSrc, "mMAINBARPHI2", spCol1 + 5},
            /*39*/ {u8"帯筋本数\nn", REGULATION::defaultGirderSrc, "mHOOPZ", spCol1 + 6},
            /*40*/ {u8"帯筋径\nD", REGULATION::defaultGirderSrc, "mHOOPPHI", spCol1 + 7},
            /*41*/ {u8"帯筋ピッチ\n@", REGULATION::defaultGirderSrc, "mHOOPPITCH", spCol1 + 8},
            /*42*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultGirderSrc, "mRK", spCol1 + 9},
            /*43*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultGirderSrc, "mRKU", spCol1 + 10},
            /*44*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultGirderSrc, "mRKB", spCol1 + 11},
            /*45*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultGirderSrc, "mRY", spCol1 + 12},
            /*46*/ {u8"主筋距離\nRY1 (mm)", REGULATION::defaultGirderSrc, "mRY1", spCol1 + 13},
            /*47*/ {u8"鉄骨材種", REGULATION::defaultGirderSrc, "mSTEEL", spCol1 + 14},
            /*48*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultGirderSrc, "mSTEELWEB", spCol1 + 15},
            /*49*/ {u8"断面タイプ", REGULATION::defaultGirderSrc, "mSHAPELISTNAME", spCol1 + 16},
            /*50*/ {u8"断面選択", REGULATION::defaultGirderSrc, "mSTANDARDSHAPESTEEL", spCol1 + 17},
            /*51*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultGirderSrc, "mH", spCol1 + 18},
            /*52*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultGirderSrc, "mB", spCol1 + 19},
            /*53*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultGirderSrc, "mT1", spCol1 + 20},
            /*54*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultGirderSrc, "mT2", spCol1 + 21},
            /*55*/ {u8"鉄骨位置：\nRy (mm)", REGULATION::defaultGirderSrc, "mCOODINATEY", spCol1 + 22},
            /*56*/ {u8"鉄骨位置：\nRz (mm)", REGULATION::defaultGirderSrc, "mCOODINATEZ", spCol1 + 23},
            // j
            /*57*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultGirderSrc, "jDY", spCol1},
            /*58*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultGirderSrc, "jDZ", spCol1 + 1},
            /*59*/ {u8"上端筋\nn", REGULATION::defaultGirderSrc, "jMAINBAR1", spCol1 + 2},
            /*60*/ {u8"下端筋\nn", REGULATION::defaultGirderSrc, "jMAINBAR2", spCol1 + 3},
            /*61*/ {u8"上端筋径\nD", REGULATION::defaultGirderSrc, "jMAINBARPHI1", spCol1 + 4},
            /*62*/ {u8"下端筋径\nD", REGULATION::defaultGirderSrc, "jMAINBARPHI2", spCol1 + 5},
            /*63*/ {u8"帯筋本数\nn", REGULATION::defaultGirderSrc, "jHOOPZ", spCol1 + 6},
            /*64*/ {u8"帯筋径\nD", REGULATION::defaultGirderSrc, "jHOOPPHI", spCol1 + 7},
            /*65*/ {u8"帯筋ピッチ\n@", REGULATION::defaultGirderSrc, "jHOOPPITCH", spCol1 + 8},
            /*66*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultGirderSrc, "jRK", spCol1 + 9},
            /*67*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultGirderSrc, "jRKU", spCol1 + 10},
            /*68*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultGirderSrc, "jRKB", spCol1 + 11},
            /*69*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultGirderSrc, "jRY", spCol1 + 12},
            /*70*/ {u8"主筋距離\nRY1 (mm)", REGULATION::defaultGirderSrc, "jRY1", spCol1 + 13},
            /*71*/ {u8"鉄骨材種", REGULATION::defaultGirderSrc, "jSTEEL", spCol1 + 14},
            /*72*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultGirderSrc, "jSTEELWEB", spCol1 + 15},
            /*73*/ {u8"断面タイプ", REGULATION::defaultGirderSrc, "jSHAPELISTNAME", spCol1 + 16},
            /*74*/ {u8"断面選択", REGULATION::defaultGirderSrc, "jSTANDARDSHAPESTEEL", spCol1 + 17},
            /*75*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultGirderSrc, "jH", spCol1 + 18},
            /*76*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultGirderSrc, "jB", spCol1 + 19},
            /*77*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultGirderSrc, "jT1", spCol1 + 20},
            /*78*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultGirderSrc, "jT2", spCol1 + 21},
            /*79*/ {u8"鉄骨位置：\nRy (mm)", REGULATION::defaultGirderSrc, "jCOODINATEY", spCol1 + 22},
            /*80*/ {u8"鉄骨位置：\nRz (mm)", REGULATION::defaultGirderSrc, "jCOODINATEZ", spCol1 + 23}};

        populateList(column_regulation);

        QStringList slist_steelshape{REGULATION::defaultGirderSrcSteelShape};
        slist_steelshape.append(standardSectionFileList(StandardSectionTable::listType::STEEL));

        limitedStrings.insert(taggedColumnNumber["CANTILEVER"], REGULATION::defaultGirderCantilever);
        limitedStrings.insert(taggedColumnNumber["iSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["iSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["mSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["mSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["jSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["jSTANDARDSHAPESTEEL"], {""});

        referenceColumns.insert(taggedColumnNumber["FLOORFROM"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["FLOORTO"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["CONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["iMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["iSTEELWEB"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["mMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["mSTEELWEB"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jSTEELWEB"], DATATYPE::TPMASTEEL);

        shapeTypeColumns << taggedColumnNumber["iSHAPELISTNAME"] << taggedColumnNumber["mSHAPELISTNAME"]
                         << taggedColumnNumber["jSHAPELISTNAME"];
    }

    /* ブレース断面：Ｓ */
    BraceSTable::BraceSTable()
    {
        myDataType = DATATYPE::TPBRACE;
        mySectionType = STRUCTTYPE::STP_S;
        COLUMNS = 33;
        nameCol = 1;
        spCol1 = 12;
        spCol2 = 19;
        spCol3 = 26;

        CREGULATION column_regulation[33] = {
            /* 0*/ {"No.", REGULATION::defaultBraceS, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultBraceS, "LABEL"},
            /* 2*/ {u8"仕上重量\n(N/m)", REGULATION::defaultBraceS, "FINISHLOAD"},
            /* 3*/ {u8"圧縮抵抗性", REGULATION::defaultBraceS, "COMPRESSIVERESISTANCE"},
            /* 4*/ {u8"鉄骨断面切り替え長さ\ni端 (mm)", REGULATION::defaultBraceS, "iLENGTH"},
            /* 5*/ {u8"鉄骨断面切り替え長さ\nj端 (mm)", REGULATION::defaultBraceS, "jLENGTH"},
            /* 6*/ {u8"※断面積\nA(cm2)", REGULATION::defaultBraceS, "AREA"},
            /* 7*/ {u8"※有効断面積\nAe(cm2)", REGULATION::defaultBraceS, "EFFECTIVEAREA"},
            /* 8*/ {u8"※細長比\nλ", REGULATION::defaultBraceS, "SLENDERNESSRATIO"},
            /* 9*/ {u8"※単位重量\nW(N/m)", REGULATION::defaultBraceS, "UNITMASS"},
            /*10*/ {u8"※表示用高さ\n(mm)", REGULATION::defaultBraceS, "DISPLAYHEIGHT"},
            /*11*/ {u8"※表示用幅\n(mm)", REGULATION::defaultBraceS, "DISPLAYWIDTH"},
            // i
            /*12*/ {u8"鉄骨材種", REGULATION::defaultBraceS, "iSTEEL"},
            /*13*/ {u8"断面タイプ", REGULATION::defaultBraceS, "iSHAPELISTNAME"},
            /*14*/ {u8"断面選択", REGULATION::defaultBraceS, "iSTANDARDSHAPESTEEL"},
            /*15*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultBraceS, "iH"},
            /*16*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultBraceS, "iB"},
            /*17*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultBraceS, "iT1"},
            /*18*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultBraceS, "iT2"},
            // c
            /*19*/ {u8"鉄骨材種", REGULATION::defaultBraceS, "mSTEEL", spCol1},
            /*20*/ {u8"断面タイプ", REGULATION::defaultBraceS, "mSHAPELISTNAME", spCol1 + 1},
            /*21*/ {u8"断面選択", REGULATION::defaultBraceS, "mSTANDARDSHAPESTEEL", spCol1 + 2},
            /*22*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultBraceS, "mH", spCol1 + 3},
            /*23*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultBraceS, "mB", spCol1 + 4},
            /*24*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultBraceS, "mT1", spCol1 + 5},
            /*25*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultBraceS, "mT2", spCol1 + 6},
            // j
            /*26*/ {u8"鉄骨材種", REGULATION::defaultBraceS, "jSTEEL", spCol1},
            /*27*/ {u8"断面タイプ", REGULATION::defaultBraceS, "jSHAPELISTNAME", spCol1 + 1},
            /*28*/ {u8"断面選択", REGULATION::defaultBraceS, "jSTANDARDSHAPESTEEL", spCol1 + 2},
            /*29*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultBraceS, "jH", spCol1 + 3},
            /*30*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultBraceS, "jB", spCol1 + 4},
            /*31*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultBraceS, "jT1", spCol1 + 5},
            /*32*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultBraceS, "jT2", spCol1 + 6}};

        populateList(column_regulation);

        QStringList slist_steelshape{REGULATION::defaultBraceSSteelShape};
        slist_steelshape.append(standardSectionFileList(StandardSectionTable::listType::STEEL));

        limitedStrings.insert(taggedColumnNumber["COMPRESSIVERESISTANCE"], REGULATION::defaultBraceSCompressiveResistance);
        limitedStrings.insert(taggedColumnNumber["iSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["iSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["mSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["mSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["jSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["jSTANDARDSHAPESTEEL"], {""});

        referenceColumns.insert(taggedColumnNumber["iSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["mSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jSTEEL"], DATATYPE::TPMASTEEL);

        shapeTypeColumns << taggedColumnNumber["iSHAPELISTNAME"] << taggedColumnNumber["mSHAPELISTNAME"]
                         << taggedColumnNumber["jSHAPELISTNAME"];
        directInputColumns << taggedColumnNumber["AREA"] << taggedColumnNumber["EFFECTIVEAREA"]
                           << taggedColumnNumber["SLENDERNESSRATIO"] << taggedColumnNumber["UNITMASS"]
                           << taggedColumnNumber["DISPLAYHEIGHT"] << taggedColumnNumber["DISPLAYWIDTH"];
    }

    /* 小梁断面RC */
    BeamRCTable::BeamRCTable()
    {
        myDataType = DATATYPE::TPBEAM;
        mySectionType = STRUCTTYPE::STP_RC;
        COLUMNS = 44;
        nameCol = 1;
        spCol1 = 5;
        spCol2 = 18;
        spCol3 = 31;

        CREGULATION column_regulation[44] = {
            /* 0*/ {"No.", REGULATION::defaultBeamRc, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultBeamRc, "LABEL"},
            /* 2*/ {u8"一般／片持", REGULATION::defaultBeamRc, "CANTILEVER"},
            /* 3*/ {u8"仕上重量\n(N/m)", REGULATION::defaultBeamRc, "FINISHLOAD"},
            /* 4*/ {u8"コンクリート材種", REGULATION::defaultBeamRc, "CONC"},
            // i
            /* 5*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultBeamRc, "iDY"},
            /* 6*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultBeamRc, "iDZ"},
            /* 7*/ {u8"上端筋\nn", REGULATION::defaultBeamRc, "iMAINBAR1"},
            /* 8*/ {u8"下端筋\nn", REGULATION::defaultBeamRc, "iMAINBAR2"},
            /* 9*/ {u8"上端筋径\nD", REGULATION::defaultBeamRc, "iMAINBARPHI1"},
            /*10*/ {u8"下端筋径\nD", REGULATION::defaultBeamRc, "iMAINBARPHI2"},
            /*11*/ {u8"帯筋本数\nn", REGULATION::defaultBeamRc, "iHOOPZ"},
            /*12*/ {u8"帯筋径\nD", REGULATION::defaultBeamRc, "iHOOPPHI"},
            /*13*/ {u8"帯筋ピッチ\n@", REGULATION::defaultBeamRc, "iHOOPPITCH"},
            /*14*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultBeamRc, "iRK"},
            /*15*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultBeamRc, "iRKU"},
            /*16*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultBeamRc, "iRKB"},
            /*17*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultBeamRc, "iRY"},
            // c
            /*18*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultBeamRc, "mDY", spCol1},
            /*19*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultBeamRc, "mDZ", spCol1 + 1},
            /*20*/ {u8"上端筋\nn", REGULATION::defaultBeamRc, "mMAINBAR1", spCol1 + 2},
            /*21*/ {u8"下端筋\nn", REGULATION::defaultBeamRc, "mMAINBAR2", spCol1 + 3},
            /*22*/ {u8"上端筋径\nD", REGULATION::defaultBeamRc, "mMAINBARPHI1", spCol1 + 4},
            /*23*/ {u8"下端筋径\nD", REGULATION::defaultBeamRc, "mMAINBARPHI2", spCol1 + 5},
            /*24*/ {u8"帯筋本数\nn", REGULATION::defaultBeamRc, "mHOOPZ", spCol1 + 6},
            /*25*/ {u8"帯筋径\nD", REGULATION::defaultBeamRc, "mHOOPPHI", spCol1 + 7},
            /*26*/ {u8"帯筋ピッチ\n@", REGULATION::defaultBeamRc, "mHOOPPITCH", spCol1 + 8},
            /*27*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultBeamRc, "mRK", spCol1 + 9},
            /*28*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultBeamRc, "mRKU", spCol1 + 10},
            /*29*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultBeamRc, "mRKB", spCol1 + 11},
            /*30*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultBeamRc, "mRY", spCol1 + 12},
            // j
            /*31*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultBeamRc, "jDY", spCol1},
            /*32*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultBeamRc, "jDZ", spCol1 + 1},
            /*33*/ {u8"上端筋\nn", REGULATION::defaultBeamRc, "jMAINBAR1", spCol1 + 2},
            /*34*/ {u8"下端筋\nn", REGULATION::defaultBeamRc, "jMAINBAR2", spCol1 + 3},
            /*35*/ {u8"上端筋径\nD", REGULATION::defaultBeamRc, "jMAINBARPHI1", spCol1 + 4},
            /*36*/ {u8"下端筋径\nD", REGULATION::defaultBeamRc, "jMAINBARPHI2", spCol1 + 5},
            /*37*/ {u8"帯筋本数\nn", REGULATION::defaultBeamRc, "jHOOPZ", spCol1 + 6},
            /*38*/ {u8"帯筋径\nD", REGULATION::defaultBeamRc, "jHOOPPHI", spCol1 + 7},
            /*39*/ {u8"帯筋ピッチ\n@", REGULATION::defaultBeamRc, "jHOOPPITCH", spCol1 + 8},
            /*40*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultBeamRc, "jRK", spCol1 + 9},
            /*41*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultBeamRc, "jRKU", spCol1 + 10},
            /*42*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultBeamRc, "jRKB", spCol1 + 11},
            /*43*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultBeamRc, "jRY", spCol1 + 12}};

        populateList(column_regulation);
        limitedStrings.insert(taggedColumnNumber["CANTILEVER"], REGULATION::defaultGirderCantilever);

        referenceColumns.insert(taggedColumnNumber["CONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["iMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jHOOPPHI"], DATATYPE::TPMAREIN);
    }

    /* 小梁断面：Ｓ */
    BeamSTable::BeamSTable()
    {
        myDataType = DATATYPE::TPBEAM;
        mySectionType = STRUCTTYPE::STP_S;
        COLUMNS = 30;
        nameCol = 1;
        spCol1 = 6;
        spCol2 = 14;
        spCol3 = 22;

        CREGULATION column_regulation[30] = {
            /* 0*/ {"No.", REGULATION::defaultBeamS, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultBeamS, "LABEL"},
            /* 2*/ {u8"一般／片持", REGULATION::defaultBeamS, "CANTILEVER"},
            /* 3*/ {u8"仕上重量\n(N/m)", REGULATION::defaultBeamS, "FINISHLOAD"},
            /* 4*/ {u8"鉄骨断面切り替え長さ\ni端 (mm)", REGULATION::defaultBeamS, "iLENGTH"},
            /* 5*/ {u8"鉄骨断面切り替え長さ\nj端 (mm)", REGULATION::defaultBeamS, "jLENGTH"},
            // i
            /* 6*/ {u8"鉄骨材種", REGULATION::defaultBeamS, "iSTEEL"},
            /* 7*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultBeamS, "iSTEELWEB"},
            /* 8*/ {u8"断面タイプ", REGULATION::defaultBeamS, "iSHAPELISTNAME"},
            /* 9*/ {u8"断面選択", REGULATION::defaultBeamS, "iSTANDARDSHAPESTEEL"},
            /*10*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultBeamS, "iH"},
            /*11*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultBeamS, "iB"},
            /*12*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultBeamS, "iT1"},
            /*13*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultBeamS, "iT2"},
            // c
            /*14*/ {u8"鉄骨材種", REGULATION::defaultBeamS, "mSTEEL", spCol1},
            /*15*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultBeamS, "mSTEELWEB", spCol1 + 1},
            /*16*/ {u8"断面タイプ", REGULATION::defaultBeamS, "mSHAPELISTNAME", spCol1 + 2},
            /*17*/ {u8"断面選択", REGULATION::defaultBeamS, "mSTANDARDSHAPESTEEL", spCol1 + 3},
            /*18*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultBeamS, "mH", spCol1 + 4},
            /*19*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultBeamS, "mB", spCol1 + 5},
            /*20*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultBeamS, "mT1", spCol1 + 6},
            /*21*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultBeamS, "mT2", spCol1 + 7},
            // j
            /*22*/ {u8"鉄骨材種", REGULATION::defaultBeamS, "jSTEEL", spCol1},
            /*23*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultBeamS, "jSTEELWEB", spCol1},
            /*24*/ {u8"断面タイプ", REGULATION::defaultBeamS, "jSHAPELISTNAME", spCol1 + 1},
            /*25*/ {u8"断面選択", REGULATION::defaultBeamS, "jSTANDARDSHAPESTEEL", spCol1 + 2},
            /*26*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultBeamS, "jH", spCol1 + 3},
            /*27*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultBeamS, "jB", spCol1 + 4},
            /*28*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultBeamS, "jT1", spCol1 + 5},
            /*29*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultBeamS, "jT2", spCol1 + 6}};

        populateList(column_regulation);

        QStringList slist_steelshape{REGULATION::defaultBeamSSteelShape};
        slist_steelshape.append(standardSectionFileList(StandardSectionTable::listType::STEEL));

        limitedStrings.insert(taggedColumnNumber["CANTILEVER"], REGULATION::defaultGirderCantilever);
        limitedStrings.insert(taggedColumnNumber["iSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["iSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["mSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["mSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["jSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["jSTANDARDSHAPESTEEL"], {""});

        referenceColumns.insert(taggedColumnNumber["iSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["iSTEELWEB"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["mSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["mSTEELWEB"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jSTEELWEB"], DATATYPE::TPMASTEEL);

        shapeTypeColumns << taggedColumnNumber["iSHAPELISTNAME"] << taggedColumnNumber["mSHAPELISTNAME"]
                         << taggedColumnNumber["jSHAPELISTNAME"];
    }

    /* 小梁断面：ＳＲＣ */
    BeamSRCTable::BeamSRCTable()
    {
        myDataType = DATATYPE::TPBEAM;
        mySectionType = STRUCTTYPE::STP_SRC;
        COLUMNS = 79;
        nameCol = 1;
        spCol1 = 7;
        spCol2 = 31;
        spCol3 = 55;

        CREGULATION column_regulation[79] = {
            /* 0*/ {"No.", REGULATION::defaultBeamSrc, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultBeamSrc, "LABEL"},
            /* 2*/ {u8"一般／片持", REGULATION::defaultBeamSrc, "CANTILEVER"},
            /* 3*/ {u8"仕上重量\n(N/m)", REGULATION::defaultBeamSrc, "FINISHLOAD"},
            /* 4*/ {u8"コンクリート材種", REGULATION::defaultBeamSrc, "CONC"},
            /* 5*/ {u8"鉄骨断面切り替え長さ\ni端 (mm)", REGULATION::defaultBeamSrc, "iLENGTH"},
            /* 6*/ {u8"鉄骨断面切り替え長さ\nj端 (mm)", REGULATION::defaultBeamSrc, "jLENGTH"},
            // i
            /* 7*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultBeamSrc, "iDY"},
            /* 8*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultBeamSrc, "iDZ"},
            /* 9*/ {u8"上端筋\nn", REGULATION::defaultBeamSrc, "iMAINBAR1"},
            /*10*/ {u8"下端筋\nn", REGULATION::defaultBeamSrc, "iMAINBAR2"},
            /*11*/ {u8"上端筋径\nD", REGULATION::defaultBeamSrc, "iMAINBARPHI1"},
            /*12*/ {u8"下端筋径\nD", REGULATION::defaultBeamSrc, "iMAINBARPHI2"},
            /*13*/ {u8"帯筋本数\nn", REGULATION::defaultBeamSrc, "iHOOPZ"},
            /*14*/ {u8"帯筋径\nD", REGULATION::defaultBeamSrc, "iHOOPPHI"},
            /*15*/ {u8"帯筋ピッチ\n@", REGULATION::defaultBeamSrc, "iHOOPPITCH"},
            /*16*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultBeamSrc, "iRK"},
            /*17*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultBeamSrc, "iRKU"},
            /*18*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultBeamSrc, "iRKB"},
            /*19*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultBeamSrc, "iRY"},
            /*20*/ {u8"主筋距離\nRY1 (mm)", REGULATION::defaultBeamSrc, "iRY1"},
            /*21*/ {u8"鉄骨材種", REGULATION::defaultBeamSrc, "iSTEEL"},
            /*22*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultBeamSrc, "iSTEELWEB"},
            /*23*/ {u8"断面タイプ", REGULATION::defaultBeamSrc, "iSHAPELISTNAME"},
            /*24*/ {u8"断面選択", REGULATION::defaultBeamSrc, "iSTANDARDSHAPESTEEL"},
            /*25*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultBeamSrc, "iH"},
            /*26*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultBeamSrc, "iB"},
            /*27*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultBeamSrc, "iT1"},
            /*28*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultBeamSrc, "iT2"},
            /*29*/ {u8"鉄骨位置：\nRy (mm)", REGULATION::defaultBeamSrc, "iCOODINATEY"},
            /*30*/ {u8"鉄骨位置：\nRz (mm)", REGULATION::defaultBeamSrc, "iCOODINATEZ"},
            // c
            /*31*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultBeamSrc, "mDY", spCol1},
            /*32*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultBeamSrc, "mDZ", spCol1 + 1},
            /*33*/ {u8"上端筋\nn", REGULATION::defaultBeamSrc, "mMAINBAR1", spCol1 + 2},
            /*34*/ {u8"下端筋\nn", REGULATION::defaultBeamSrc, "mMAINBAR2", spCol1 + 3},
            /*35*/ {u8"上端筋径\nD", REGULATION::defaultBeamSrc, "mMAINBARPHI1", spCol1 + 4},
            /*36*/ {u8"下端筋径\nD", REGULATION::defaultBeamSrc, "mMAINBARPHI2", spCol1 + 5},
            /*37*/ {u8"帯筋本数\nn", REGULATION::defaultBeamSrc, "mHOOPZ", spCol1 + 6},
            /*38*/ {u8"帯筋径\nD", REGULATION::defaultBeamSrc, "mHOOPPHI", spCol1 + 7},
            /*39*/ {u8"帯筋ピッチ\n@", REGULATION::defaultBeamSrc, "mHOOPPITCH", spCol1 + 8},
            /*40*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultBeamSrc, "mRK", spCol1 + 9},
            /*41*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultBeamSrc, "mRKU", spCol1 + 10},
            /*42*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultBeamSrc, "mRKB", spCol1 + 11},
            /*43*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultBeamSrc, "mRY", spCol1 + 12},
            /*44*/ {u8"主筋距離\nRY1 (mm)", REGULATION::defaultBeamSrc, "mRY1", spCol1 + 13},
            /*45*/ {u8"鉄骨材種", REGULATION::defaultBeamSrc, "mSTEEL", spCol1 + 14},
            /*46*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultBeamSrc, "mSTEELWEB", spCol1 + 15},
            /*47*/ {u8"断面タイプ", REGULATION::defaultBeamSrc, "mSHAPELISTNAME", spCol1 + 16},
            /*48*/ {u8"断面選択", REGULATION::defaultBeamSrc, "mSTANDARDSHAPESTEEL", spCol1 + 17},
            /*49*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultBeamSrc, "mH", spCol1 + 18},
            /*50*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultBeamSrc, "mB", spCol1 + 19},
            /*51*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultBeamSrc, "mT1", spCol1 + 20},
            /*52*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultBeamSrc, "mT2", spCol1 + 21},
            /*53*/ {u8"鉄骨位置：\nRy (mm)", REGULATION::defaultBeamSrc, "mCOODINATEY", spCol1 + 22},
            /*54*/ {u8"鉄骨位置：\nRz (mm)", REGULATION::defaultBeamSrc, "mCOODINATEZ", spCol1 + 23},
            // j
            /*55*/ {u8"コンクリート断面\nDy (mm)", REGULATION::defaultBeamSrc, "jDY", spCol1},
            /*56*/ {u8"コンクリート断面\nDz (mm)", REGULATION::defaultBeamSrc, "jDZ", spCol1 + 1},
            /*57*/ {u8"上端筋\nn", REGULATION::defaultBeamSrc, "jMAINBAR1", spCol1 + 2},
            /*58*/ {u8"下端筋\nn", REGULATION::defaultBeamSrc, "jMAINBAR2", spCol1 + 3},
            /*59*/ {u8"上端筋径\nD", REGULATION::defaultBeamSrc, "jMAINBARPHI1", spCol1 + 4},
            /*60*/ {u8"下端筋径\nD", REGULATION::defaultBeamSrc, "jMAINBARPHI2", spCol1 + 5},
            /*61*/ {u8"帯筋本数\nn", REGULATION::defaultBeamSrc, "jHOOPZ", spCol1 + 6},
            /*62*/ {u8"帯筋径\nD", REGULATION::defaultBeamSrc, "jHOOPPHI", spCol1 + 7},
            /*63*/ {u8"帯筋ピッチ\n@", REGULATION::defaultBeamSrc, "jHOOPPITCH", spCol1 + 8},
            /*64*/ {u8"主筋間隔\nRK (mm)", REGULATION::defaultBeamSrc, "jRK", spCol1 + 9},
            /*65*/ {u8"主筋距離上\nRKu (mm)", REGULATION::defaultBeamSrc, "jRKU", spCol1 + 10},
            /*66*/ {u8"主筋距離下\nRKb (mm)", REGULATION::defaultBeamSrc, "jRKB", spCol1 + 11},
            /*67*/ {u8"主筋距離\nRY (mm)", REGULATION::defaultBeamSrc, "jRY", spCol1 + 12},
            /*68*/ {u8"主筋距離\nRY1 (mm)", REGULATION::defaultBeamSrc, "jRY1", spCol1 + 13},
            /*69*/ {u8"鉄骨材種", REGULATION::defaultBeamSrc, "jSTEEL", spCol1 + 14},
            /*70*/ {u8"鉄骨材種(ウェブ)", REGULATION::defaultBeamSrc, "jSTEELWEB", spCol1 + 15},
            /*71*/ {u8"断面タイプ", REGULATION::defaultBeamSrc, "jSHAPELISTNAME", spCol1 + 16},
            /*72*/ {u8"断面選択", REGULATION::defaultBeamSrc, "jSTANDARDSHAPESTEEL", spCol1 + 17},
            /*73*/ {u8"鉄骨断面\nH (mm)", REGULATION::defaultBeamSrc, "jH", spCol1 + 18},
            /*74*/ {u8"鉄骨断面\nB (mm)", REGULATION::defaultBeamSrc, "jB", spCol1 + 19},
            /*75*/ {u8"鉄骨断面\nt1 (mm)", REGULATION::defaultBeamSrc, "jT1", spCol1 + 20},
            /*76*/ {u8"鉄骨断面\nt2 (mm)", REGULATION::defaultBeamSrc, "jT2", spCol1 + 21},
            /*77*/ {u8"鉄骨位置：\nRy (mm)", REGULATION::defaultBeamSrc, "jCOODINATEY", spCol1 + 22},
            /*78*/ {u8"鉄骨位置：\nRz (mm)", REGULATION::defaultBeamSrc, "jCOODINATEZ", spCol1 + 23}};

        populateList(column_regulation);

        QStringList slist_steelshape{REGULATION::defaultBeamSrcSteelShape};
        slist_steelshape.append(standardSectionFileList(StandardSectionTable::listType::STEEL));

        limitedStrings.insert(taggedColumnNumber["CANTILEVER"], REGULATION::defaultGirderCantilever);
        limitedStrings.insert(taggedColumnNumber["iSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["iSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["mSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["mSTANDARDSHAPESTEEL"], {""});
        limitedStrings.insert(taggedColumnNumber["jSHAPELISTNAME"], slist_steelshape);
        limitedStrings.insert(taggedColumnNumber["jSTANDARDSHAPESTEEL"], {""});

        referenceColumns.insert(taggedColumnNumber["CONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["iMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["iSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["iSTEELWEB"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["mMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["mSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["mSTEELWEB"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jMAINBARPHI1"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jMAINBARPHI2"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jHOOPPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["jSTEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["jSTEELWEB"], DATATYPE::TPMASTEEL);

        shapeTypeColumns << taggedColumnNumber["iSHAPELISTNAME"] << taggedColumnNumber["mSHAPELISTNAME"]
                         << taggedColumnNumber["jSHAPELISTNAME"];
    }

    WallRegularTable::WallRegularTable()
    {
        myDataType = DATATYPE::TPWALL;
        mySectionType = STRUCTTYPE::STP_RC;

        COLUMNS = 27;
        nameCol = 1;
        spCol1 = 10;
        spCol2 = 15;
        spCol3 = 21;

        CREGULATION column_regulation[27] = {
            /* 0*/ {"No.", REGULATION::defaultWallRc, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultWallRc, "LABEL"},
            /* 2*/ {u8"階範囲\n階～", REGULATION::defaultWallRc, "FLOORFROM"},
            /* 3*/ {u8"階範囲\n～階", REGULATION::defaultWallRc, "FLOORTO"},
            /* 4*/ {u8"壁種別", REGULATION::defaultWallRc, "WALLTYPE"},
            /* 5*/ {u8"コンクリート材種", REGULATION::defaultWallRc, "CONC"},
            /* 6*/ {u8"コンクリート厚\n(mm)", REGULATION::defaultWallRc, "CONCTHICKNESS"},
            /* 7*/ {u8"鋼材種", REGULATION::defaultWallRc, "STEEL"},
            /* 8*/ {u8"鋼板厚\n(mm)", REGULATION::defaultWallRc, "STEELTHICKNESS"},
            /* 9*/ {u8"仕上重量\n(N/m2)", REGULATION::defaultWallRc, "FINISHLOAD"},
            // 配筋
            /*10*/ {u8"配筋タイプ", REGULATION::defaultWallRc, "BARARRANGEMENTTYPE"},
            /*11*/ {u8"タテ筋径\nD", REGULATION::defaultWallRc, "vBARPHI"},
            /*12*/ {u8"タテ筋ピッチ\n＠", REGULATION::defaultWallRc, "vBARPITCH"},
            /*13*/ {u8"ヨコ筋径\nD", REGULATION::defaultWallRc, "hBARPHI", 11},
            /*14*/ {u8"ヨコ筋ピッチ\n＠", REGULATION::defaultWallRc, "hBARPITCH", 12},
            // 開口補強筋（≦1000）
            /*15*/ {u8"タテ筋本数\nn", REGULATION::defaultWallRc, "vSMALLOPENINGBARNUM"},
            /*16*/ {u8"タテ筋径\nD", REGULATION::defaultWallRc, "vSMALLOPENINGBARPHI"},
            /*17*/ {u8"ヨコ筋径\nn", REGULATION::defaultWallRc, "hSMALLOPENINGBARNUM"},
            /*18*/ {u8"ヨコ筋径\nD", REGULATION::defaultWallRc, "hSMALLOPENINGBARPHI"},
            /*19*/ {u8"ナナメ筋本数\nn", REGULATION::defaultWallRc, "dSMALLOPENINGBARNUM"},
            /*20*/ {u8"ナナメ筋径\nD", REGULATION::defaultWallRc, "dSMALLOPENINGBARPHI"},
            // 開口補強筋（＞1000）
            /*21*/ {u8"タテ筋本数\nn", REGULATION::defaultWallRc, "vLARGEOPENINGBARNUM"},
            /*22*/ {u8"タテ筋径\nD", REGULATION::defaultWallRc, "vLARGEOPENINGBARPHI"},
            /*23*/ {u8"ヨコ筋径\nn", REGULATION::defaultWallRc, "hLARGEOPENINGBARNUM"},
            /*24*/ {u8"ヨコ筋径\nD", REGULATION::defaultWallRc, "hLARGEOPENINGBARPHI"},
            /*25*/ {u8"ナナメ筋本数\nn", REGULATION::defaultWallRc, "dLARGEOPENINGBARNUM"},
            /*26*/ {u8"ナナメ筋径\nD", REGULATION::defaultWallRc, "dLARGEOPENINGBARPHI"}};

        populateList(column_regulation);

        limitedStrings.insert(taggedColumnNumber["WALLTYPE"], REGULATION::defaultWallRcType);
        limitedStrings.insert(taggedColumnNumber["BARARRANGEMENTTYPE"], REGULATION::defaultWallRcBarArrangementType);

        referenceColumns.insert(taggedColumnNumber["FLOORFROM"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["FLOORTO"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["CONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["STEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["vBARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["hBARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["vSMALLOPENINGBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["hSMALLOPENINGBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["dSMALLOPENINGBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["vLARGEOPENINGBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["hLARGEOPENINGBARPHI"], DATATYPE::TPMAREIN);
        referenceColumns.insert(taggedColumnNumber["dLARGEOPENINGBARPHI"], DATATYPE::TPMAREIN);
    }

    WallSteelTable::WallSteelTable()
    {
        myDataType = DATATYPE::TPWALL;
        mySectionType = STRUCTTYPE::STP_S;
        COLUMNS = 16;
        nameCol = 1;
        spCol1 = 8;
        spCol2 = -1;
        spCol3 = -1;

        CREGULATION column_regulation[16] = {
            /* 0*/ {"No.", REGULATION::defaultWallS, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultWallS, "LABEL"},
            /* 2*/ {u8"階範囲\n階～", REGULATION::defaultWallS, "FLOORFROM"},
            /* 3*/ {u8"階範囲\n～階", REGULATION::defaultWallS, "FLOORTO"},
            /* 4*/ {u8"壁種別", REGULATION::defaultWallS, "WALLTYPE"},
            /* 5*/ {u8"鋼材種", REGULATION::defaultWallS, "STEEL"},
            /* 6*/ {u8"鋼板厚\n(mm)", REGULATION::defaultWallS, "STEELTHICKNESS"},
            /* 7*/ {u8"仕上重量\n(N/m2)", REGULATION::defaultWallS, "FINISHLOAD"},
            // リブ
            /* 8*/ {u8"縦リブピッチ", REGULATION::defaultWallS, "vRIBPITCH"},
            /* 9*/ {u8"縦リブタイプ", REGULATION::defaultWallS, "vRIBTYPE"},
            /*10*/ {u8"縦リブ厚", REGULATION::defaultWallS, "vRIBTHICKNESS"},
            /*11*/ {u8"縦リブ幅", REGULATION::defaultWallS, "vRIBBREADTH"},
            /*12*/ {u8"横リブピッチ", REGULATION::defaultWallS, "hRIBPITCH"},
            /*13*/ {u8"横リブタイプ", REGULATION::defaultWallS, "hRIBTYPE"},
            /*14*/ {u8"横リブ厚", REGULATION::defaultWallS, "hRIBTHICKNESS"},
            /*15*/ {u8"横リブ幅", REGULATION::defaultWallS, "hRIBBREADTH"},
        };

        populateList(column_regulation);

        limitedStrings.insert(taggedColumnNumber["WALLTYPE"], REGULATION::defaultWallSType);

        referenceColumns.insert(taggedColumnNumber["FLOORFROM"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["FLOORTO"], DATATYPE::TPFLOOR);
        referenceColumns.insert(taggedColumnNumber["STEEL"], DATATYPE::TPMASTEEL);
    }

    WallUnderGroundTable::WallUnderGroundTable()
    {
        myDataType = DATATYPE::TPUWALL;
        COLUMNS = 29;
        nameCol = 1;
        spCol1 = 9;
        spCol2 = 19;
        spCol3 = -1;

        CREGULATION column_regulation[29] = {
            /* 0*/ {"No.", REGULATION::defaultWallUnderGround, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultWallUnderGround, "LABEL"},
            /* 2*/ {u8"コンクリート材種", REGULATION::defaultWallUnderGround, "CONC"},
            /* 3*/ {u8"コンクリート厚\n(mm)", REGULATION::defaultWallUnderGround, "CONCTHICKNESS"},
            /* 4*/ {u8"鋼材種", REGULATION::defaultWallUnderGround, "STEEL"},
            /* 5*/ {u8"鋼板厚\n(mm)", REGULATION::defaultWallUnderGround, "STEELTHICKNESS"},
            /* 6*/ {u8"仕上重量\n(N/m2)", REGULATION::defaultWallUnderGround, "FINISHLOAD"},
            /* 7*/ {u8"標準径\nD", REGULATION::defaultWallUnderGround, "STANDARDBARPHI"},
            /* 8*/ {u8"標準ピッチ\n＠", REGULATION::defaultWallUnderGround, "STANDARDBARPITCH"},
            // 外側
            /* 9*/ {u8"縦筋上部\nD", REGULATION::defaultWallUnderGround, "vOUTERTOP_BARPHI", 7},
            /*10*/ {u8"縦筋上部\n＠", REGULATION::defaultWallUnderGround, "vOUTERTOP_BARPITCH", 8},
            /*11*/ {u8"縦筋中部\nD", REGULATION::defaultWallUnderGround, "vOUTERMIDDLE_BARPHI", 7},
            /*12*/ {u8"縦筋中部\n＠", REGULATION::defaultWallUnderGround, "vOUTERMIDDLE_BARPITCH", 8},
            /*13*/ {u8"縦筋下部\nD", REGULATION::defaultWallUnderGround, "vOUTERBOTTOM_BARPHI", 7},
            /*14*/ {u8"縦筋下部\n＠", REGULATION::defaultWallUnderGround, "vOUTERBOTTOM_BARPITCH", 8},
            /*15*/ {u8"横筋端部\nD", REGULATION::defaultWallUnderGround, "hOUTEREDGE_BARPHI", 7},
            /*16*/ {u8"横筋端部\n＠", REGULATION::defaultWallUnderGround, "hOUTEREDGE_BARPITCH", 8},
            /*17*/ {u8"横筋中央\nD", REGULATION::defaultWallUnderGround, "hOUTERCENTER_BARPHI", 7},
            /*18*/ {u8"横筋中央\n＠", REGULATION::defaultWallUnderGround, "hOUTERCENTER_BARPITCH", 8},
            // 内側
            /*19*/ {u8"縦筋上部\nD", REGULATION::defaultWallUnderGround, "vINNERTOP_BARPHI", 7},
            /*20*/ {u8"縦筋上部\n＠", REGULATION::defaultWallUnderGround, "vINNERTOP_BARPITCH", 8},
            /*21*/ {u8"縦筋中部\nD", REGULATION::defaultWallUnderGround, "vINNERMIDDLE_BARPHI", 7},
            /*22*/ {u8"縦筋中部\n＠", REGULATION::defaultWallUnderGround, "vINNERMIDDLE_BARPITCH", 8},
            /*23*/ {u8"縦筋下部\nD", REGULATION::defaultWallUnderGround, "vINNERBOTTOM_BARPHI", 7},
            /*24*/ {u8"縦筋下部\n＠", REGULATION::defaultWallUnderGround, "vINNERBOTTOM_BARPITCH", 8},
            /*25*/ {u8"横筋端部\nD", REGULATION::defaultWallUnderGround, "hINNEREDGE_BARPHI", 7},
            /*26*/ {u8"横筋端部\n＠", REGULATION::defaultWallUnderGround, "hINNEREDGE_BARPITCH", 8},
            /*27*/ {u8"横筋中央\nD", REGULATION::defaultWallUnderGround, "hINNERCENTER_BARPHI", 7},
            /*28*/ {u8"横筋中央\n＠", REGULATION::defaultWallUnderGround, "hINNERCENTER_BARPITCH", 8}};

        populateList(column_regulation);

        referenceColumns.insert(taggedColumnNumber["CONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["STEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["STANDARDBARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["vOUTERTOP_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["vOUTERMIDDLE_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["vOUTERBOTTOM_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["hOUTEREDGE_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["hOUTERCENTER_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["vINNERTOP_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["vINNERMIDDLE_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["vINNERBOTTOM_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["hINNEREDGE_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["hINNERCENTER_BARPHI"], DATATYPE::TPMAREIND);
    }

    SlabTable::SlabTable()
    {
        myDataType = DATATYPE::TPSLAB;
        COLUMNS = 35;
        nameCol = 1;
        spCol1 = 11;
        spCol2 = 23;
        spCol3 = -1;

        CREGULATION column_regulation[35] = {
            /* 0*/ {"No.", REGULATION::defaultSlab, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultSlab, "LABEL"},
            /* 2*/ {u8"スラブ種別", REGULATION::defaultSlab, "SLABTYPE"},
            /* 3*/ {u8"コンクリート材種", REGULATION::defaultSlab, "CONC"},
            /* 4*/ {u8"コンクリート厚\n(mm)", REGULATION::defaultSlab, "CONCTHICKNESS"},
            /* 5*/ {u8"鋼材種", REGULATION::defaultSlab, "STEEL"},
            /* 6*/ {u8"鋼板厚\n(mm)", REGULATION::defaultSlab, "STEELTHICKNESS"},
            /* 7*/ {u8"仕上重量\n(N/m2)", REGULATION::defaultSlab, "FINISHLOAD"},
            /* 8*/ {u8"荷重方向\n(度)", REGULATION::defaultSlab, "LISTLOADDIRECTION"},
            /* 9*/ {u8"標準径\nD", REGULATION::defaultSlab, "STANDARDBARPHI"},
            /*10*/ {u8"標準ピッチ\n＠", REGULATION::defaultSlab, "STANDARDBARPITCH"},
            // 短辺方向
            /*11*/ {u8"A部-端部\n上側D", REGULATION::defaultSlab, "sUPPEREDGE_A_BARPHI", 11},
            /*12*/ {u8"A部-端部\n上側＠", REGULATION::defaultSlab, "sUPPEREDGE_A_BARPITCH", 12},
            /*13*/ {u8"A部-端部\n下側D", REGULATION::defaultSlab, "sLOWEREDGE_A_BARPHI", 11},
            /*14*/ {u8"A部-端部\n下側＠", REGULATION::defaultSlab, "sLOWEREDGE_A_BARPITCH", 12},
            /*15*/ {u8"A部-中央部\n上側D", REGULATION::defaultSlab, "sUPPERCENTER_A_BARPHI", 11},
            /*16*/ {u8"A部-中央部\n上側＠", REGULATION::defaultSlab, "sUPPERCENTER_A_BARPITCH", 12},
            /*17*/ {u8"A部-中央部\n下側D", REGULATION::defaultSlab, "sLOWERCENTER_A_BARPHI", 11},
            /*18*/ {u8"A部-中央部\n下側＠", REGULATION::defaultSlab, "sLOWERCENTER_A_BARPITCH", 12},
            /*19*/ {u8"B部-端中共\n上側D", REGULATION::defaultSlab, "sUPPER_B_BARPHI", 11},
            /*20*/ {u8"B部-端中共\n上側＠", REGULATION::defaultSlab, "sUPPER_B_BARPITCH", 12},
            /*21*/ {u8"B部-端中共\n下側D", REGULATION::defaultSlab, "sLOWER_B_BARPHI", 11},
            /*22*/ {u8"B部-端中共\n下側＠", REGULATION::defaultSlab, "sLOWER_B_BARPITCH", 12},
            // 長辺方向
            /*23*/ {u8"A部-端部\n上側D", REGULATION::defaultSlab, "lUPPEREDGE_A_BARPHI", 11},
            /*24*/ {u8"A部-端部\n上側＠", REGULATION::defaultSlab, "lUPPEREDGE_A_BARPITCH", 12},
            /*25*/ {u8"A部-端部\n下側D", REGULATION::defaultSlab, "lLOWEREDGE_A_BARPHI", 11},
            /*26*/ {u8"A部-端部\n下側＠", REGULATION::defaultSlab, "lLOWEREDGE_A_BARPITCH", 12},
            /*27*/ {u8"A部-中央部\n上側D", REGULATION::defaultSlab, "lUPPERCENTER_A_BARPHI", 11},
            /*28*/ {u8"A部-中央部\n上側＠", REGULATION::defaultSlab, "lUPPERCENTER_A_BARPITCH", 12},
            /*29*/ {u8"A部-中央部\n下側D", REGULATION::defaultSlab, "lLOWERCENTER_A_BARPHI", 11},
            /*30*/ {u8"A部-中央部\n下側＠", REGULATION::defaultSlab, "lLOWERCENTER_A_BARPITCH", 12},
            /*31*/ {u8"B部-端中共\n上側D", REGULATION::defaultSlab, "lUPPER_B_BARPHI", 11},
            /*32*/ {u8"B部-端中共\n上側＠", REGULATION::defaultSlab, "lUPPER_B_BARPITCH", 12},
            /*33*/ {u8"B部-端中共\n下側D", REGULATION::defaultSlab, "lLOWER_B_BARPHI", 11},
            /*34*/ {u8"B部-端中共\n下側＠", REGULATION::defaultSlab, "lLOWER_B_BARPITCH", 12}};

        populateList(column_regulation);
        limitedStrings.insert(taggedColumnNumber["SLABTYPE"], REGULATION::defaultSlabType);

        referenceColumns.insert(taggedColumnNumber["CONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["STEEL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["STANDARDBARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["sUPPEREDGE_A_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["sLOWEREDGE_A_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["sUPPERCENTER_A_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["sLOWERCENTER_A_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["sUPPER_B_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["sLOWER_B_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["lUPPEREDGE_A_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["lLOWEREDGE_A_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["lUPPERCENTER_A_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["lLOWERCENTER_A_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["lUPPER_B_BARPHI"], DATATYPE::TPMAREIND);
        referenceColumns.insert(taggedColumnNumber["lLOWER_B_BARPHI"], DATATYPE::TPMAREIND);
    }

    // S造露出柱脚

    BaseSTable::BaseSTable()
    {
        myDataType = DATATYPE::TPBASE;
        mySectionType = STRUCTTYPE::STP_S;
        COLUMNS = 15;
        nameCol = 1;
        spCol1 = 4;
        spCol2 = 8;
        spCol3 = -1;

        CREGULATION column_regulation[15] = {
            /* 0*/ {"No.", REGULATION::defaultBaseS, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultBaseS, "LABEL"},
            /* 2*/ {u8"在来品／既製品", REGULATION::defaultBaseS, "STANDARDTYPE"},
            /* 3*/ {u8"規格", REGULATION::defaultBaseS, "PRODUCTNAME"},
            // BASEPLATE
            /* 4*/ {"B\n(mm)", REGULATION::defaultBaseS, "B"},
            /* 5*/ {"D\n(mm)", REGULATION::defaultBaseS, "D", 4},
            /* 6*/ {"t\n(mm)", REGULATION::defaultBaseS, "T"},
            /* 7*/ {u8"材種", REGULATION::defaultBaseS, "BASEPLATEMATERIAL"},
            // BOLT
            /* 8*/ {u8"縁端距離\ndty(mm)", REGULATION::defaultBaseS, "EDGEDISTANCEY1"},
            /* 9*/ {u8"縁端距離\ndtz(mm)", REGULATION::defaultBaseS, "EDGEDISTANCEZ1"},
            /*10*/ {u8"本数\nny(本)", REGULATION::defaultBaseS, "BOLTNY1"},
            /*11*/ {u8"本数\nnz(本)", REGULATION::defaultBaseS, "BOLTNZ1", 11},
            /*12*/ {u8"呼び径\n(mm)", REGULATION::defaultBaseS, "BOLTPHI"},
            /*13*/ {u8"材種", REGULATION::defaultBaseS, "BOLTMATERIAL"},
            /*14*/ {u8"長さ\nlb(mm)", REGULATION::defaultBaseS, "BOLTLENGTH"},
        };

        populateList(column_regulation);

        QStringList slist_basetype{REGULATION::defaultBaseSType};
        slist_basetype = slist_basetype + standardSectionFileList(StandardSectionTable::listType::BASE);

        limitedStrings.insert(taggedColumnNumber["STANDARDTYPE"], slist_basetype);
        limitedStrings.insert(taggedColumnNumber["PRODUCTNAME"], {""});
        shapeTypeColumns << taggedColumnNumber["STANDARDTYPE"];

        referenceColumns.insert(taggedColumnNumber["BASEPLATEMATERIAL"], DATATYPE::TPMASTEEL);
        referenceColumns.insert(taggedColumnNumber["BOLTMATERIAL"], DATATYPE::TPMASTEEL);
    }

    // SRC造非埋め込み柱脚
    BaseSRCTable::BaseSRCTable()
    {
        myDataType = DATATYPE::TPBASE;
        mySectionType = STRUCTTYPE::STP_SRC;
        COLUMNS = 21;
        nameCol = 1;
        spCol1 = 5;
        spCol2 = 10;
        spCol3 = -1;

        CREGULATION column_regulation[21] = {
            /* 0*/ {"No.", REGULATION::defaultBaseSrc, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultBaseSrc, "LABEL"},
            /* 2*/ {u8"BPL端あき\nB0(mm)", REGULATION::defaultBaseSrc, "B0"},
            /* 3*/ {u8"BPL端あき\nD0(mm)", REGULATION::defaultBaseSrc, "D0"},
            /* 4*/ {u8"コンクリート材種", REGULATION::defaultBaseSrc, "CONC"},
            // BASEPLATE
            /* 5*/ {u8"形状タイプ", REGULATION::defaultBaseSrc, "BASEPLATETYPE"},
            /* 6*/ {u8"部材幅\nB(mm)", REGULATION::defaultBaseSrc, "B"},
            /* 7*/ {u8"部材せい\nD(mm)", REGULATION::defaultBaseSrc, "D", 6},
            /* 8*/ {u8"部材厚\nt(mm)", REGULATION::defaultBaseSrc, "T"},
            /* 9*/ {u8"隅切り長\nSML(mm)", REGULATION::defaultBaseSrc, "SML"},
            // BOLT
            /*10*/ {u8"縁端距離\nhy1(mm)", REGULATION::defaultBaseSrc, "EDGEDISTANCEY1"},
            /*11*/ {u8"縁端距離\nhy2(mm)", REGULATION::defaultBaseSrc, "EDGEDISTANCEY2", 10},
            /*12*/ {u8"縁端距離\nhz1(mm)", REGULATION::defaultBaseSrc, "EDGEDISTANCEZ1"},
            /*13*/ {u8"縁端距離\nhz2(mm)", REGULATION::defaultBaseSrc, "EDGEDISTANCEZ2", 12},
            /*14*/ {u8"配置タイプ", REGULATION::defaultBaseSrc, "BOLTARRANGEMENTTYPE"},
            /*15*/ {u8"本数\nny1(本)", REGULATION::defaultBaseSrc, "BOLTNY1"},
            /*16*/ {u8"本数\nny2(本)", REGULATION::defaultBaseSrc, "BOLTNY2", 15},
            /*17*/ {u8"本数\nnz1(本)", REGULATION::defaultBaseSrc, "BOLTNZ1"},
            /*18*/ {u8"本数\nnz2(本)", REGULATION::defaultBaseSrc, "BOLTNZ2", 17},
            /*19*/ {u8"呼び径\n(mm)", REGULATION::defaultBaseSrc, "BOLTPHI"},
            /*20*/ {u8"材種", REGULATION::defaultBaseSrc, "BOLTMATERIAL"}};

        populateList(column_regulation);

        QStringList slist_bplshape{REGULATION::defaultBaseSrcBplShape};
        QStringList slist_bolttype{
            BaseBoltType::enumToString(BaseBoltType::Type::UNDEFINED),
            BaseBoltType::enumToString(BaseBoltType::Type::RECTANGLE),
            BaseBoltType::enumToString(BaseBoltType::Type::PARALLEL_LR),
            BaseBoltType::enumToString(BaseBoltType::Type::PARALLEL_UD),
            BaseBoltType::enumToString(BaseBoltType::Type::DIAMOND),
            BaseBoltType::enumToString(BaseBoltType::Type::CIRCLE)};

        limitedStrings.insert(taggedColumnNumber["BASEPLATETYPE"], slist_bplshape);
        limitedStrings.insert(taggedColumnNumber["BOLTARRANGEMENTTYPE"], slist_bolttype);

        referenceColumns.insert(taggedColumnNumber["CONC"], DATATYPE::TPMACONC);
        referenceColumns.insert(taggedColumnNumber["BOLTMATERIAL"], DATATYPE::TPMASTEEL);
    }

    /* 免震装置 */
    IsolatorTable::IsolatorTable()
    {
        myDataType = DATATYPE::TPISO;
        COLUMNS = 24;
        nameCol = 1;
        spCol1 = -1;
        spCol2 = -1;
        spCol3 = -1;

        CREGULATION column_regulation[24] = {
            /* 0*/ {"No.", REGULATION::defaultIsolator, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultIsolator, "LABEL"},
            /* 2*/ {u8"種類", REGULATION::defaultIsolator, "PRODUCTTYPE"},
            /* 3*/ {u8"型番", REGULATION::defaultIsolator, "PRODUCTNAME"},

            /* 4*/ {u8"復元力特性（せん断）", REGULATION::defaultIsolator, "SHEARHYSTERESIS"},
            /* 5*/ {u8"初期剛性hK1(kN/m)", REGULATION::defaultIsolator, "hK1"},
            /* 6*/ {u8"二次剛性\nhK2(kN/m)", REGULATION::defaultIsolator, "hK2"},
            /* 7*/ {u8"三次剛性\nhK3(kN/m)", REGULATION::defaultIsolator, "hK3"},
            /* 8*/ {u8"降伏荷重\nhFy1(kN)", REGULATION::defaultIsolator, "hFY1"},
            /* 9*/ {u8"降伏荷重\nhFy2(kN)", REGULATION::defaultIsolator, "hFY2"},
            /*10*/ {u8"復元力特性(軸)", REGULATION::defaultIsolator, "AXIALHYSTERESIS"},
            /*11*/ {u8"初期剛性\n\vK1(kN/m)", REGULATION::defaultIsolator, "vK1"},
            /*12*/ {u8"引張二次剛性\nvK2+(kN/m)", REGULATION::defaultIsolator, "vK2"},
            /*13*/ {u8"引張三次剛性\nvK3+(kN/m)", REGULATION::defaultIsolator, "vK3"},
            /*14*/ {u8"引張降伏荷重\nvFy1+(kN)", REGULATION::defaultIsolator, "vFY1_PLUS"},
            /*15*/ {u8"引張降伏荷重\nvFy2+(kN)", REGULATION::defaultIsolator, "vFY2_PLUS"},

            /*16*/ {u8"部材重量\n(kN)", REGULATION::defaultIsolator, "SELFWEIGHT"},
            /*17*/ {u8"表示形状", REGULATION::defaultIsolator, "RENDERSHAPE"},
            /*18*/ {u8"表示サイズ\nD(mm)", REGULATION::defaultIsolator, "RENDERSIZED"},
            /*19*/ {u8"表示サイズ\nH(mm)", REGULATION::defaultIsolator, "RENDERSIZEH"},
            /*20*/ {u8"設置高さ\nL(mm)", REGULATION::defaultIsolator, "SETLEVEL"},
            /*21*/ {u8"割線剛性計算変形\n(mm)", REGULATION::defaultIsolator, "DISPLACEMENT"},
            /*22*/ {u8"せん断モデルのばね数", REGULATION::defaultIsolator, "SPRING"},
            /*23*/ {u8"反曲点高さ比", REGULATION::defaultIsolator, "INFLECTION"}};

        populateList(column_regulation);

        QStringList slist_type{REGULATION::defaultIsolatorType};
        slist_type += standardSectionFileList(StandardSectionTable::listType::ISOLATOR);
        limitedStrings.insert(taggedColumnNumber["PRODUCTTYPE"], slist_type);
        limitedStrings.insert(taggedColumnNumber["PRODUCTNAME"], {""});
        limitedStrings.insert(taggedColumnNumber["SHEARHYSTERESIS"], REGULATION::defaultIsolatorShearHysteresis);
        limitedStrings.insert(taggedColumnNumber["AXIALHYSTERESIS"], REGULATION::defaultIsolatorAxialHysteresis);
        limitedStrings.insert(taggedColumnNumber["RENDERSHAPE"], REGULATION::defaultIsolatorRender);
        shapeTypeColumns << taggedColumnNumber["PRODUCTTYPE"];
    }

    // 制振装置
    DamperTable::DamperTable()
    {
        myDataType = DATATYPE::TPDAMPER;
        COLUMNS = 17;
        nameCol = 1;
        spCol1 = -1;
        spCol2 = -1;
        spCol3 = -1;

        CREGULATION column_regulation[17] = {
            /* 0*/ {"No.", REGULATION::defaultDamper, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultDamper, "LABEL"},
            /* 2*/ {u8"種類", REGULATION::defaultDamper, "PRODUCTTYPE"},
            /* 3*/ {u8"単位", REGULATION::defaultDamper, "UNITS"},
            /* 4*/ {u8"パラメータ(カンマ区切り)", REGULATION::defaultDamper, "PARAMETERS"},
            /* 5*/ {u8"重量\n(kN)", REGULATION::defaultDamper, "SELFWEIGHT"},
            /* 6*/ {u8"方向ベクトル\nX", REGULATION::defaultDamper, "VECTOR_X"},
            /* 7*/ {u8"方向ベクトル\nY", REGULATION::defaultDamper, "VECTOR_Y"},
            /* 8*/ {u8"方向ベクトル\nZ", REGULATION::defaultDamper, "VECTOR_Z"},
            /* 9*/ {u8"静的解析用\n固有周期T1(秒)", REGULATION::defaultDamper, "NATURALPERIOD"},
            /*10*/ {u8"静的解析用最大変形", REGULATION::defaultDamper, "DISPLACEMENT"},
            /*11*/ {u8"モデル化フラグ", REGULATION::defaultDamper, "MODEL_FLAG"},
            /*12*/ {u8"付帯梁に自動設定する左右梁剛域長の和", REGULATION::defaultDamper, "RIGID_LENGTH"},
            /*13*/ {u8"ダンパーの作用方向", REGULATION::defaultDamper, "DAMPER_DIRECTION"},
            /*14*/ {u8"状態T解析に用いるダンパー部荷重", REGULATION::defaultDamper, "TANALYSIS"},
            /*15*/ {u8"履歴面積低減係数", REGULATION::defaultDamper, "FACTOR"},
            /*16*/ {u8"架構抵抗剛性", REGULATION::defaultDamper, "FRAME_RIGIDITY"}};

        populateList(column_regulation);

        limitedStrings.insert(taggedColumnNumber["PRODUCTTYPE"], REGULATION::defaultDamperType);
        QStringList damperUnit;
        Q_FOREACH (auto u, REGULATION::defaultDamperUnit)
        {
            damperUnit.append(u.join(":"));
        }
        limitedStrings.insert(taggedColumnNumber["UNITS"], damperUnit);
    }

    // 開口
    OpeningTable::OpeningTable()
    {
        myDataType = DATATYPE::TPWALLOPEN;
        COLUMNS = 16;
        nameCol = 1;
        spCol1 = -1;
        spCol2 = -1;
        spCol3 = -1;

        CREGULATION column_regulation[16] = {
            /* 0*/ {"No.", REGULATION::defaultOpening, "NO"},
            /* 1*/ {u8"符号", REGULATION::defaultOpening, "LABEL"},
            /* 2*/ {u8"開口種別", REGULATION::defaultOpening, "OPENINGTYPE"},
            /* 3*/ {u8"開口位置\n左右基準", REGULATION::defaultOpening, "LRBASE"},
            /* 4*/ {u8"開口位置\n左右", REGULATION::defaultOpening, "LR"},
            /* 5*/ {u8"開口位置\n上下基準", REGULATION::defaultOpening, "UDBASE"},
            /* 6*/ {u8"開口位置\n上下", REGULATION::defaultOpening, "UD"},
            /* 7*/ {u8"開口原点\nox(mm)", REGULATION::defaultOpening, "POSITIONX"},
            /* 8*/ {u8"開口原点\noy(mm)", REGULATION::defaultOpening, "POSITIONY"},
            /* 9*/ {u8"開口範囲\n幅(mm)", REGULATION::defaultOpening, "WIDTH"},
            /*10*/ {u8"開口範囲\n高さ(mm)", REGULATION::defaultOpening, "HEIGHT"},
            /*11*/ {u8"スリット範囲\nL(mm)", REGULATION::defaultOpening, "SLITL"},
            /*12*/ {u8"スリット範囲\nR(mm)", REGULATION::defaultOpening, "SLITR"},
            /*13*/ {u8"スリット範囲\nB(mm)", REGULATION::defaultOpening, "SLITB"},
            /*14*/ {u8"スリット範囲\nT(mm)", REGULATION::defaultOpening, "SLITT"},
            /*15*/ {u8"コメント", REGULATION::defaultOpening, "COMMENT"}};

        CustomTableDataType::populateList(column_regulation);
        limitedStrings.insert(taggedColumnNumber["OPENINGTYPE"], REGULATION::defaultOpeningType);
        limitedStrings.insert(taggedColumnNumber["LRBASE"], REGULATION::defaultOpeningLrBase);
        limitedStrings.insert(taggedColumnNumber["LR"], REGULATION::defaultOpeningLr);
        limitedStrings.insert(taggedColumnNumber["UDBASE"], REGULATION::defaultOpeningUdBase);
        limitedStrings.insert(taggedColumnNumber["UD"], REGULATION::defaultOpeningUd);
    }

    bool SectionDataType::hasData(const QList<QVariant> &myValues, int tableNo) const
    {
        // table1 共通、table2 i断面、table3 m断面(girder)、table4 j断面（girder）
        bool isInputted = false;
        int from;
        int to;
        if (myDataType == DATATYPE::TPCOLUMN || myDataType == DATATYPE::TPUWALL || myDataType == DATATYPE::TPSLAB || (myDataType == DATATYPE::TPBASE && mySectionType == STRUCTTYPE::STP_SRC))
        {
            switch (tableNo)
            {
            case 1:
                from = 0;
                to = spCol1;
                break;
            case 2:
                from = spCol1;
                to = spCol2;
                break;
            case 3:
                from = spCol2;
                to = this->count();
                break;
            default:
                return false;
            }
        }
        else if (myDataType == DATATYPE::TPGIRDER || myDataType == DATATYPE::TPBEAM || myDataType == DATATYPE::TPBRACE || (myDataType == DATATYPE::TPWALL && mySectionType == STRUCTTYPE::STP_RC)|| (myDataType == DATATYPE::TPBASE && mySectionType == STRUCTTYPE::STP_S))
        {
            switch (tableNo)
            {
            case 1:
                from = 0;
                to = spCol1;
                break;
            case 2:
                from = spCol1;
                to = spCol2;
                break;
            case 3:
                from = spCol2;
                to = spCol3;
                break;
            case 4:
                from = spCol3;
                to = this->count();
                break;
            default:
                return false;
            }
        }
        else if (myDataType == DATATYPE::TPISO || myDataType == DATATYPE::TPDAMPER || myDataType == DATATYPE::TPWALLOPEN)
        {
            switch (tableNo)
            {
            case 1:
                from = 0;
                to = this->count();
                break;
            default:
                return false;
            }
        }
        else if ((myDataType == DATATYPE::TPWALL)&& (mySectionType == STRUCTTYPE::STP_S) )
        {
            switch (tableNo)
            {
            case 1:
                from = 0;
                to = spCol1;
                break;
            case 2:
                from = spCol1;
                to = this->count();
                break;
            default:
                return false;
            }
        }

        for (int i = from; i < to; ++i)
        {
            if (!myValues.at(i).toString().isEmpty())
            {
                isInputted = true;
                break;
            }
        }
        return isInputted;
    }
} // namespace post3dapp
