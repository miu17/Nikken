#ifndef ANALYSISRANGEDATATYPE_H
#define ANALYSISRANGEDATATYPE_H

#include "define_customtable.h"
#include "define_defaultvalue.h"

namespace post3dapp{

/* 解析範囲指定定義 */

class AnalysisJointRangeDataType : public CustomTableDataType
{

public:

    AnalysisJointRangeDataType(){

        COLUMNS=5;
        QMap<QString, REGULATION> joint = {
            {u8"NO", REGULATION(TABLEDATATYPE::TB_STRING)},
            {u8"NAME", REGULATION(TABLEDATATYPE::TB_STRING)},
            {u8"JOINTS", REGULATION(TABLEDATATYPE::TB_STRING)},
            {u8"JOINTNUMBERS", REGULATION(TABLEDATATYPE::TB_STRING)},
            {u8"COMMENTS", REGULATION(TABLEDATATYPE::TB_STRING)}
        };

        CREGULATION column_regulation[5]={
            /* 0*/  { u8"No.", joint, "NO" },
            /* 1*/  { u8"グループ名", joint, "NAME" },
            /* 2*/  { u8"選択節点", joint, "JOINTS" },
            /* 3*/  { u8"節点番号", joint, "JOINTNUMBERS" },
            /* 4*/  { u8"コメント", joint, "COMMENT" }
         };

        populateList(column_regulation);

    }
};

class AnalysisMemberRangeDataType : public CustomTableDataType
{

public:

    AnalysisMemberRangeDataType(){

        COLUMNS=5;

        QMap<QString, REGULATION> member = {
            {u8"NO", REGULATION(TABLEDATATYPE::TB_STRING)},
            {u8"NAME", REGULATION(TABLEDATATYPE::TB_STRING)},
            {u8"MEMBERS", REGULATION(TABLEDATATYPE::TB_STRING)},
            {u8"DATATYPE", REGULATION(TABLEDATATYPE::TB_STRING)},
            {u8"COMMENTS", REGULATION(TABLEDATATYPE::TB_STRING)}
        };

        CREGULATION column_regulation[5]={
            /* 0*/  { u8"No.", member, "NO" },
            /* 1*/  { u8"グループ名", member, "NAME" },
            /* 2*/  { u8"選択部材", member, "MEMBERS" },
            /* 3*/  { u8"部材種別", member, "DATATYPE" },
            /* 4*/  { u8"コメント", member, "COMMENT" }
         };

        populateList(column_regulation);

    }
};

} // namespace post3dapp


#endif
