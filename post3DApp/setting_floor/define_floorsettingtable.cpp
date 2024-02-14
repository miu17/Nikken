#include "define_floorsettingtable.h"

#include "define_defaultvalue.h"
namespace post3dapp{
FloorDataType::FloorDataType()
{

    COLUMNS = 7;
    myDataType = DATATYPE::TPFLOOR;
    nameCol = 1;
    uuidCol = 6;

    CREGULATION column_regulation[7] = {
        /* 0*/  { "No.", REGULATION::defaultFloor, "NO" },
        /* 1*/  { u8"階名", REGULATION::defaultFloor, "NAME" },
        /* 2*/  { u8"高さ(m)", REGULATION::defaultFloor, "HEIGHT" },
        /* 3*/  { u8"階スケールの表示", REGULATION::defaultFloor, "FLOORSCALE" },
        /* 4*/  { u8"コンクリート強度(小梁)", REGULATION::defaultFloor, "BEAM" },
        /* 5*/  { u8"コンクリート強度(スラブ)", REGULATION::defaultFloor, "SLAB" },
        /* 6*/  { "UUID", REGULATION::defaultFloor, "UUID" }
    };

    populateList(column_regulation);
    limitedStrings.insert( taggedColumnNumber["FLOORSCALE"], REGULATION::defaultFloorScale);

    referenceColumns.insert(taggedColumnNumber["BEAM"], DATATYPE::TPMACONC);
    referenceColumns.insert(taggedColumnNumber["SLAB"], DATATYPE::TPMACONC);
}
} // namespace post3dapp
