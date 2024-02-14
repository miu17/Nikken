#ifndef INDIVIDUALSETTINGTYPE_H
#define INDIVIDUALSETTINGTYPE_H

#include "define_customtable.h"
#include "define_defaultvalue.h"
namespace post3dapp{
class IndividualSettingType : public CustomTableDataType
{

public:

    IndividualSettingType()
    {

        COLUMNS = 5;

        CREGULATION column_regulation[5] = {
            /* 0*/  { "No.", REGULATION::defaultIndivisualSettingType, "NO" },
            /* 1*/  { u8"部材符号\n荷重符号", REGULATION::defaultIndivisualSettingType, "LABEL" },
            /* 2*/  { u8"線の色", REGULATION::defaultIndivisualSettingType, "LINECOLOR" },
            /* 3*/  { u8"塗りつぶし", REGULATION::defaultIndivisualSettingType, "FILLCOLOR" },
            /* 4*/  { u8"透過度", REGULATION::defaultIndivisualSettingType, "ALPHA" }
        };

        populateList(column_regulation);

    }

};
} // namespace post3dapp


#endif
