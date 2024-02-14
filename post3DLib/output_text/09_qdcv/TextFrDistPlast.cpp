#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFrDistPlast::write(const FixedData& fixedData) const{
    //PR605

    QString second;
    second.append(u8"XXX ZZ- ZZ  弾塑性解析の架構の層せん断力分担 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"■ XXXXXX FL   STEP=XXXXX   COEF.=XXX.XXXX   ( FORCE ANGLE = XXXX.XX )").append("\n");
    second.append(u8"+-------------------+--------+--------+---------+------------------+------------------+------------------+------------------+").append("\n");
    second.append(u8"| STEP (   COEF.  ) | FLOOR  | FRAME  |  ANGLE  | FRAME   (KN) (%) | BRACE   (KN) (%) | WALL    (KN) (%) | TOTAL   (KN) (%) |").append("\n");
    second.append(u8"+-------------------+--------+--------+---------+------------------+------------------+------------------+------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"|XXXXX ( XXX.XXXX ) | XXXXXX | XXXXXX | XXXX.XX |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |").append("\n");
        second.append(u8"+-------------------+--------+--------+---------+------------------+------------------+------------------+------------------+").append("\n");
    //}
    second.append(u8"                                           SUM   XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)").append("\n");
    second.append(u8"                                           SHEAR XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)").append("\n");
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
