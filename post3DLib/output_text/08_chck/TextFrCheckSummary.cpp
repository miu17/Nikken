#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFrCheckSummary::write(const FixedData& fixedData) const{
    //PR501

    QString second;
    second.append(u8"■ XXXXXX FRAME  GIRDER **").append("\n");
    second.append(u8"■ XXXXXX FRAME  COLUMN **").append("\n");
    second.append(u8"■ XXXXXX FRAME  BRACE  **").append("\n");
    second.append(u8"  SEQ.      J1     J2 FL     FRAME  GR1    GR2            LIST     +-MD/MA( N-M INT.)-+     +-------QD/QA------+      ND/NA").append("\n");
    second.append(u8"                                                  CASE                (I)    (C)    (J)        (I)    (C)    (J)        (J)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX-XXXXXX         XXXXXXXXXXXXXXXX").append("\n");
        second.append(u8"                                                  XXXXXXXXXX       XX.XXX XX.XXX XX.XXX XX  XX.XXX XX.XXX XX.XXX XX  XX.XXX").append("\n");
    //}
    second.append(u8"■ XXXXXX FRAME  WALL   **").append("\n");
    second.append(u8"  SEQ.      J1     J2     J3     J4 FL     FRAME  GR1    GR2    LIST").append("\n");
    second.append(u8"                                                                CASE              QD/QA").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX-XXXXXX         XXXXXXXX").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
