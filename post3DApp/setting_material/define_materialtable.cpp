#include "define_materialtable.h"

#include "define_defaultvalue.h"
#include "define_material_struct.h"

namespace post3dapp{
const StandardMaterialTable *MaterialDataType::standardMaterial = new StandardMaterialTable;

MaterialDataType::MaterialDataType() {}

int MaterialDataType::getSeparateColumn1() const
{
    return spCol1;
}


QList<int> MaterialDataType::getShapeTypeColumns() const
{
    return shapeTypeColumns;
}

//規格断面　リスト情報
QStringList MaterialDataType::standardMaterialNameList(const QString &mtype)
{
    if ( mtype == MaterialType::enumToString(MaterialType::Type::CONCRETE) ) return standardMaterial->getMaterialNameList("CONCRETE");
    if ( mtype == MaterialType::enumToString(MaterialType::Type::STEEL) ) return standardMaterial->getMaterialNameList("STEEL");
    if ( mtype == MaterialType::enumToString(MaterialType::Type::REINFORCE) ) return standardMaterial->getMaterialNameList("REINFORCE");
    return QStringList();
}
bool MaterialDataType::standardMaterialNameExists(const QString &mname)
{
    return standardMaterial->existsMaterialName(mname);
}
QString MaterialDataType::standardMaterialValue(const QString &mname, StandardMaterialTable::format fm)
{
    return standardMaterial->getMaterialValue(mname, fm);
}

//行データに対し、指定したテーブルの入力有無のチェック
QHash<int, QHash<int, QVariant> > MaterialDataType::getDefaultValuePatch() const
{
    return defaultValuePatch;
}
QHash<int, QVariant> MaterialDataType::getLineDefaultValues(int row) const
{
    QHash<int, QVariant> linedefaultvalues = defaultValues;
    for (int col = 0; col < COLUMNS; ++col) {
        if ( defaultValuePatch[row][col].isValid() )
            linedefaultvalues.insert(col, defaultValuePatch[row][col]);
    }
    return linedefaultvalues;
}
void MaterialDataType::setDefaultValuePatch(int row, int col, const QVariant &dvalue)
{
    if (!dvalue.toString().isEmpty()) {
        defaultValuePatch[row].insert(col, dvalue);
    } else {
        defaultValuePatch[row].remove(col);
    }
}

MaterialTable::MaterialTable()
{
    COLUMNS = 26;
    spCol1 = 6;

    CREGULATION column_regulation[26] = {
        /* 0*/  { "No.", REGULATION::defaultMaterial, "NO"},
        /* 1*/  { u8"区分", REGULATION::defaultMaterial, "CATEGORY"},
        /* 2*/  { u8"材種名", REGULATION::defaultMaterial, "LABEL"},
        /* 3*/  { u8"鉄筋\nシンボル記号", REGULATION::defaultMaterial, "SYMBOL"},
        /* 4*/  { u8"鉄筋範囲\nD～", REGULATION::defaultMaterial, "RANGEFROM"},
        /* 5*/  { u8"鉄筋範囲\n～D", REGULATION::defaultMaterial, "RANGETO"},
        //i
        /* 6*/  { u8"ヤング係数\nE (N/mm2)", REGULATION::defaultMaterial, "E"},
        /* 7*/  { u8"せん断弾性係数\nG (N/mm2)", REGULATION::defaultMaterial, "G"},
        /* 8*/  { u8"単位重量1\nkN/m3", REGULATION::defaultMaterial, "UNITWEIGHT1"},
        /* 9*/  { u8"単位重量2\nkN/m3", REGULATION::defaultMaterial, "UNITWEIGHT2"},
        /*10*/  { u8"ヤング係数比\nn", REGULATION::defaultMaterial, "MODULARRATIO"},
        /*11*/  { u8"F値\n(N/mm2)", REGULATION::defaultMaterial, "F_STRENGTH"},
        /*12*/  { u8"引張\n長期(N/mm2)", REGULATION::defaultMaterial, "T_L"},
        /*13*/  { u8"引張\n短期(N/mm2)", REGULATION::defaultMaterial, "T_S"},
        /*14*/  { u8"引張\n終局(N/mm2)", REGULATION::defaultMaterial, "T_U"},
        /*15*/  { u8"圧縮\n長期(N/mm2)", REGULATION::defaultMaterial, "C_L"},
        /*16*/  { u8"圧縮\n短期(N/mm2)", REGULATION::defaultMaterial, "C_S"},
        /*17*/  { u8"圧縮\n終局(N/mm2)", REGULATION::defaultMaterial, "C_U"},
        /*18*/  { u8"せん断\n長期(N/mm2)", REGULATION::defaultMaterial, "S_L"},
        /*19*/  { u8"せん断\n短期(N/mm2)", REGULATION::defaultMaterial, "S_S"},
        /*20*/  { u8"せん断\n終局(N/mm2)", REGULATION::defaultMaterial, "S_U"},
        /*21*/  { u8"付着\n長期(N/mm2)", REGULATION::defaultMaterial, "A_L"},
        /*22*/  { u8"付着\n短期(N/mm2)", REGULATION::defaultMaterial, "A_S"},
        /*23*/  { u8"付着\n終局(N/mm2)", REGULATION::defaultMaterial, "A_U"},
        /*24*/  { u8"付着 上端\n長期(N/mm2)", REGULATION::defaultMaterial, "UA_L"},
        /*25*/  { u8"付着 上端\n短期(N/mm2)", REGULATION::defaultMaterial, "UA_S"}
    };

    populateList(column_regulation);

    const QStringList slist_type{
        "",
        MaterialType::enumToString(MaterialType::Type::CONCRETE),
        MaterialType::enumToString(MaterialType::Type::STEEL),
        MaterialType::enumToString(MaterialType::Type::REINFORCE)
    };
    limitedStrings.insert( taggedColumnNumber["CATEGORY"], slist_type);
    limitedStrings.insert( taggedColumnNumber["SYMBOL"], REGULATION::defaultMaterialSymbol);
    shapeTypeColumns << taggedColumnNumber["CATEGORY"] ;
}
} // namespace post3dapp
