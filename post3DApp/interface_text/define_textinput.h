#ifndef DEFINE_TEXTINPUT_H
#define DEFINE_TEXTINPUT_H

#include "define_customtable.h"
#include "define_defaultvalue.h"

/* 節点テーブル */
namespace post3dapp{
class JointTableForm : public CustomTableDataType
{

public:

    JointTableForm()
    {

        COLUMNS = 7;
        uuidCol = 6;

        CREGULATION column_regulation[7] = {
            /* 0*/  { "No.", REGULATION::defaultJointTable, "NO" },
            /* 1*/  { u8"X座標(m)", REGULATION::defaultJointTable, "X" },
            /* 2*/  { u8"Y座標(m)", REGULATION::defaultJointTable, "Y" },
            /* 3*/  { u8"Z座標(m)", REGULATION::defaultJointTable, "Z" },
            /* 4*/  { u8"所属階", REGULATION::defaultJointTable, "FLOORS" },
            /* 5*/  { u8"所属通り", REGULATION::defaultJointTable, "FRAMES" },
            /* 6*/  { "UUID", REGULATION::defaultJointTable, "UUID" }
        };

        populateList(column_regulation);
    }

};

/* 部材テーブル */

class MemberTableForm : public CustomTableDataType
{

public:

    MemberTableForm()
    {

        COLUMNS = 15;
        uuidCol = 14;

        CREGULATION column_regulation[15] = {
            /* 0*/  { "No.", REGULATION::defaultMemberTable, "NO" },
            /* 1*/  { u8"部材種別", REGULATION::defaultMemberTable, "TYPE" },
            /* 2*/  { u8"部材符号", REGULATION::defaultMemberTable, "NAME" },
            /* 3*/  { u8"開口符号", REGULATION::defaultMemberTable, "WALLOPEN" },
            /* 4*/  { u8"節点構成", REGULATION::defaultMemberTable, "JOINTS" },
            /* 5*/  { u8"所属階\n(i端)", REGULATION::defaultMemberTable, "FLOORS" },
            /* 6*/  { u8"所属通り\n(全節点)", REGULATION::defaultMemberTable, "FRAMES" },
            /* 7*/  { u8"コードアングル\n(度)", REGULATION::defaultMemberTable, "ANGLE" },
            /* 8*/  { u8"フェイスルール\n(i端)", REGULATION::defaultMemberTable, "IFACERULE" },
            /* 9*/  { u8"フェイス長さ\ni端 (mm)", REGULATION::defaultMemberTable, "IFACELEN" },
            /*10*/  { u8"フェイスルール\n(j端)", REGULATION::defaultMemberTable, "JFACERULE" },
            /*11*/  { u8"フェイス長さ\nj端 (mm)", REGULATION::defaultMemberTable, "JFACELEN" },
            /*12*/  { u8"寄り長さ\ny (mm)", REGULATION::defaultMemberTable, "SHIFTY" },
            /*13*/  { u8"寄り長さ\nz (mm)", REGULATION::defaultMemberTable, "SHIFTZ" },
            /*14*/  { "UUID", REGULATION::defaultMemberTable, "UUID" }
        };

        populateList(column_regulation);
        disableColumns << taggedColumnNumber["FLOORS"] << taggedColumnNumber["FRAMES"];
        limitedStrings.insert(taggedColumnNumber["TYPE"], REGULATION::defaultMemberTableElementList);
        limitedStrings.insert(taggedColumnNumber["IFACERULE"], REGULATION::defaultMemberTableFaceRule);
        limitedStrings.insert(taggedColumnNumber["JFACERULE"], REGULATION::defaultMemberTableFaceRule);

    }
};


/* 荷重テーブル */

class LoadTableForm : public CustomTableDataType
{

public:

    LoadTableForm()
    {

        COLUMNS = 7;
        uuidCol = 6;

        CREGULATION column_regulation[7] = {
            /* 0*/  { "No.", REGULATION::defaultLoadTable, "NO" },
            /* 1*/  { u8"荷重種類", REGULATION::defaultLoadTable, "TYPE" },
            /* 2*/  { u8"荷重符号", REGULATION::defaultLoadTable, "NAME" },
            /* 3*/  { u8"節点構成", REGULATION::defaultLoadTable, "JOINTS" },
            /* 4*/  { u8"所属階\n(i端)", REGULATION::defaultLoadTable, "FLOORS" },
            /* 5*/  { u8"所属通り\n(全節点)", REGULATION::defaultLoadTable, "FRAMES" },
            /* 6*/  { "UUID", REGULATION::defaultLoadTable, "UUID" }
        };

        populateList(column_regulation);

        disableColumns << taggedColumnNumber["FLOORS"] << taggedColumnNumber["FRAMES"];
        limitedStrings.insert(taggedColumnNumber["TYPE"], REGULATION::defaultLoadTableElementList);

    }


};
} // namespace post3dapp

#endif
