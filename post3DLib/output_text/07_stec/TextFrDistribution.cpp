#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFrDistribution::write(const FixedData& fixedData) const{
    //PR407

    QString second;
    second.append(u8"** XXXXXX FL  ( CASE : XXXXXXXXXX   FILE : XXXXXXXXXX   FORCE ANGLE : XXXX.XX )").append("\n");
    second.append(u8"+--------+--------+---------+------------------+------------------+------------------+------------------+------------------+------------------+").append("\n");
    second.append(u8"| FLOOR  | FRAME  |  ANGLE  | FRAME   (KN) (%) | BRACE   (KN) (%) | WALL    (KN) (%) | DAMPER  (KN) (%) | OTHER   (KN) (%) | TOTAL   (KN) (%) |").append("\n");
    second.append(u8"+--------+--------+---------+------------------+------------------+------------------+------------------+------------------+------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX | XXXXXX | XXXX.XX |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |").append("\n");
        second.append(u8"+--------+--------+---------+------------------+------------------+------------------+------------------+------------------+------------------+").append("\n");
    //}
    second.append(u8"                       SUM   XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)").append("\n");
    second.append(u8"                       SHEAR XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)").append("\n");
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
