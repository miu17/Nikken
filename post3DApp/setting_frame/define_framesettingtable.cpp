#include "define_framesettingtable.h"

#include "define_defaultvalue.h"
namespace post3dapp{
FrameDataType::FrameDataType()
{

    COLUMNS = 14;
    myDataType = DATATYPE::TPFRAMEG;
    nameCol = 1;
    uuidCol = 13;

    CREGULATION column_regulation[14] = {
        /* 0*/  { "No.", REGULATION::defaultFrameGroup, "NO" },
        /* 1*/  { u8"通りグループ", REGULATION::defaultFrameGroup, "FRAMEGROUP" },
        /* 2*/  { u8"種類", REGULATION::defaultFrameGroup, "FRAMETYPE" },
        /* 3*/  { u8"通り", REGULATION::defaultFrameGroup, "FRAMESUM" },
        /* 4*/  { u8"基準点\nX座標(m)", REGULATION::defaultFrameGroup, "OX" },
        /* 5*/  { u8"基準点\nY座標(m)", REGULATION::defaultFrameGroup, "OY" },
        /* 6*/  { u8"基準角度\n開始角度", REGULATION::defaultFrameGroup, "STARTANGLE" },
        /* 7*/  { u8"終了角度", REGULATION::defaultFrameGroup, "ENDANGLE" },
        /* 8*/  { u8"分割数", REGULATION::defaultFrameGroup, "DIVISIONS" },
        /* 9*/  { u8"通り長さ\n正方向(m)", REGULATION::defaultFrameGroup, "LENGTHPLUS" },
        /*10*/  { u8"通り長さ\n負方向(m)", REGULATION::defaultFrameGroup, "LENGTHMINUS" },
        /*11*/  { u8"階範囲\n階～", REGULATION::defaultFrameGroup, "FLOORFROM" },
        /*12*/  { u8"階範囲\n～階", REGULATION::defaultFrameGroup, "FLOORTO" },
        /*13*/  { "GID", REGULATION::defaultFrameGroup, "GID" }
        /*14*/ // { tr("UUID"), TABLEDATATYPE::TB_STRING, QVariant(), -1, "UUID" }
    };

    populateList(column_regulation);

    limitedStrings.insert( taggedColumnNumber["FRAMETYPE"], {REGULATION::defaultFrameGroupFrameType});
    referenceColumns.insert(taggedColumnNumber["FLOORFROM"], DATATYPE::TPFLOOR);
    referenceColumns.insert(taggedColumnNumber["FLOORTO"], DATATYPE::TPFLOOR);
}

FrameSubDataType::FrameSubDataType()
{

    COLUMNS = 10;
    myDataType = DATATYPE::TPFRAMEP;
    nameCol = 3;
    uuidCol = 9;

    CREGULATION column_regulation[10] = {
        /* 0*/  { "No.", REGULATION::defaultFrameSubGroup, "NO" },
        /* 1*/  { u8"通りグループ", REGULATION::defaultFrameSubGroup, "FRAMEGROUP" },
        /* 2*/  { u8"種類", REGULATION::defaultFrameSubGroup, "FRAMETYPE" },
        /* 3*/  { u8"通り名", REGULATION::defaultFrameSubGroup, "NAME" },
        /* 4*/  { u8"スパン(m)", REGULATION::defaultFrameSubGroup, "SPAN" },
        /* 5*/  { u8"加算角度\n(度)", REGULATION::defaultFrameSubGroup, "ANGLE" },
        /* 6*/  { u8"ボタン", REGULATION::defaultFrameSubGroup, "BUTTON" },
        /* 7*/  { u8"点列座標", REGULATION::defaultFrameSubGroup, "XYPOINTS" },
        /* 8*/  { "GID", REGULATION::defaultFrameSubGroup, "GID" },
        /* 9*/  { "UUID", REGULATION::defaultFrameSubGroup, "UUID" }
    };

    populateList(column_regulation);
    limitedStrings.insert( taggedColumnNumber["FRAMETYPE"], {REGULATION::defaultFrameGroupFrameType});
}

FrameXYDataType::FrameXYDataType()
{

    COLUMNS = 3;

    CREGULATION column_regulation[3] = {
        /* 0*/  { "No.", REGULATION::defaultFrameXY, "NO" },
        /* 1*/  { u8"X座標\n(m)", REGULATION::defaultFrameXY, "X" },
        /* 2*/  { u8"Y座標\n(m)", REGULATION::defaultFrameXY, "Y" }
    };

    populateList(column_regulation);
}
} // namespace post3dapp
