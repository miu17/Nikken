#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFrMemR::write(const FixedData& fixedData) const{
    //PR610

    QString second;
    second.append(u8"XXX ZZ- ZZ  架構毎の部材角 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"■ XXXXXX FRAME  GIRDER **  UNIT= X 1000").append("\n");
    second.append(u8"■ XXXXXX FRAME  COLUMN **  UNIT= X 1000").append("\n");
    second.append(u8"■ XXXXXX FRAME  BRACE  **  UNIT= X 1000").append("\n");
    second.append(u8"  SEQ.  LIST").append("\n");
    second.append(u8"        FL     FRAME  GR1    GR2      DX/L0        RY.I      RZ.I      R.I         RY.J      RZ.J      R.J        DS/L0").append("\n");
    second.append(u8"        J1     J2            L0(M)  ( YIELD )   ( YIELD ) ( YIELD ) ( YIELD )   ( YIELD ) ( YIELD ) ( YIELD )   ( YIELD )").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXXXXXXXXXXXX").append("\n");
        second.append(u8"        XXXXXX XXXXXX XXXXXX XXXXXX  XXX.XXX     XXXXXXX   XXXXXXX   XXXXXXX     XXXXXXX   XXXXXXX   XXXXXXX     XXXXXXX").append("\n");
        second.append(u8"        XXXXXX-XXXXXX        XX.XXX (XXX.XXX)   (XXXXXXX) (XXXXXXX) (XXXXXXX)   (XXXXXXX) (XXXXXXX) (XXXXXXX)   (XXXXXXX)").append("\n");
    //}
    second.append(u8"■ XXXXXX FRAME  WALL   **").append("\n");
    second.append(u8"  SEQ.      J1     J2     J3     J4 FL     FRAME  GR1    GR2    LIST      RANK      Q       QU    QU/Q").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXX ( X ) XXXXXXXX XXXXXXXX XX.XXX").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
