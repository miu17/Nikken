#include "define_analysisfloor.h"
#include "define_defaultvalue.h"

namespace post3dapp{
AnalysisFloorDataType::AnalysisFloorDataType()
{

    COLUMNS = 5;
    myDataType = DATATYPE::TPAFLOOR;
    nameCol = 1;

    CREGULATION column_regulation[5] = {
        /* 0*/  { "No.", REGULATION::defaultAnalysisFloor, "NO" },
        /* 1*/  { u8"階名", REGULATION::defaultAnalysisFloor, "FLOOR" },
        /* 2*/  { u8"高さ補正±(m)", REGULATION::defaultAnalysisFloor, "LEVEL" },
        /* 3*/  { u8"補助階", REGULATION::defaultAnalysisFloor, "CHILDFLOOR" },
        /* 4*/  { u8"所属階", REGULATION::defaultAnalysisFloor, "PARENTFLOOR" }
    };

    populateList(column_regulation);

    disableColumns << taggedColumnNumber["FLOOR"];

    limitedStrings.insert(taggedColumnNumber["CHILDFLOOR"], QStringList{REGULATION::defaultAnalysisFloorChildFloor});
    referenceColumns.insert(taggedColumnNumber["PARENTFLOOR"], DATATYPE::TPFLOOR);
}
} // namespace post3dapp
