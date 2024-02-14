#include "define_loadsettingtable.h"

#include "define_defaultvalue.h"
namespace post3dapp{
JointLoadType::JointLoadType()
{

    COLUMNS = 10;
    nameCol = 1;

    CREGULATION regulation[10] = {
        /* 0*/  { "No.", REGULATION::defaultJointLoad, "NO" },
        /* 1*/  { u8"荷重符号", REGULATION::defaultJointLoad, "NAME" },
        /* 2*/  { u8"荷重種類", REGULATION::defaultJointLoad, "TYPE" },
        /* 3*/  { "Fx(kN)", REGULATION::defaultJointLoad, "FX" },
        /* 4*/  { "Fy(kN)", REGULATION::defaultJointLoad, "FY" },
        /* 5*/  { "Fz(kN)", REGULATION::defaultJointLoad, "FZ" },
        /* 6*/  { u8"Mx(kN・m)", REGULATION::defaultJointLoad, "MX" },
        /* 7*/  { u8"My(kN・m)", REGULATION::defaultJointLoad, "MY" },
        /* 8*/  { u8"Mz(kN・m)", REGULATION::defaultJointLoad, "MZ" },
        /* 9*/  { u8"コメント", REGULATION::defaultJointLoad, "COMMENT" }
    };

    populateList(regulation);
    limitedStrings.insert( taggedColumnNumber["TYPE"], REGULATION::defaultLoadType);

}

MemberLoadType::MemberLoadType()
{

    COLUMNS = 8;
    nameCol = 1;

    CREGULATION regulation[8] = {
        /* 0*/  { "No.", REGULATION::defaultMemberLoad, "NO" },
        /* 1*/  { u8"荷重符号", REGULATION::defaultMemberLoad, "NAME" },
        /* 2*/  { u8"荷重種類", REGULATION::defaultMemberLoad, "TYPE" },
        /* 3*/  { u8"座標系", REGULATION::defaultMemberLoad, "COORDINATE" },
        /* 4*/  { u8"荷重方向", REGULATION::defaultMemberLoad, "DIRECTION" },
        /* 5*/  { "wa(kN/m)", REGULATION::defaultMemberLoad, "WA" },
        /* 6*/  { "wb(kN/m)", REGULATION::defaultMemberLoad, "WB" },
        /* 7*/  { u8"コメント", REGULATION::defaultMemberLoad, "COMMENT" }
    };

    populateList(regulation);

    limitedStrings.insert( taggedColumnNumber["TYPE"], REGULATION::defaultLoadType);
    limitedStrings.insert( taggedColumnNumber["COORDINATE"], REGULATION::defaultMemberLoadCoordinate);
    limitedStrings.insert( taggedColumnNumber["DIRECTION"], REGULATION::defaultMemberLoadDirection);

}

SlabLoadType::SlabLoadType()
{

    COLUMNS = 10;
    nameCol = 1;

    CREGULATION regulation[10] = {
        /* 0*/  { "No.", REGULATION::defaultSlabLoad, "NO" },
        /* 1*/  { u8"荷重符号", REGULATION::defaultSlabLoad, "NAME" },
        /* 2*/  { u8"固定荷重DL\n床仕上げ(N/m^2)", REGULATION::defaultSlabLoad, "DL" },
        /* 3*/  { u8"積載荷重LL0\n床用(N/m^2)", REGULATION::defaultSlabLoad, "LL0" },
        /* 4*/  { u8"積載荷重LL1\n架構用(N/m^2)", REGULATION::defaultSlabLoad, "LL1" },
        /* 5*/  { u8"積載荷重LL2\n架構用(N/m^2)", REGULATION::defaultSlabLoad, "LL2" },
        /* 6*/  { u8"積載荷重LL3\n架構用(N/m^2)", REGULATION::defaultSlabLoad, "LL3" },
        /* 7*/  { u8"積載荷重LL4\n架構用(N/m^2)", REGULATION::defaultSlabLoad, "LL4" },
        /* 8*/  { u8"積載荷重LLE\n地震用(N/m^2)", REGULATION::defaultSlabLoad, "LLE" },
        /* 9*/  { u8"コメント", REGULATION::defaultSlabLoad, "COMMENT" }
    };

    populateList(regulation);
}
} // namespace post3dapp
