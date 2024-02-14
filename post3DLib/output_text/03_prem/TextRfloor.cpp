#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextRfloor::write(const FixedData& fixedData) const{
    //PR065

    QString second;
    second.append(u8"+------+--------+------------------+----------------------------+--------------+------------------+-------------+-----------------+").append("\n");
    second.append(u8"| SEQ. |  R.FL  |   CENTER JOINT   |   CENTER OF SEISMIC MASS   | SEISMIC MASS | ROTATIVE INERTIA | SEISMIC MASS| ROTATIVE INERTIA|").append("\n");
    second.append(u8"|  NO. |  NAME  | (SEQ.NO.)   NAME |   X(M)     Y(M)     Z(M)   |     (KN)     |      (kN*M2)     |     (t)     |      (t*M2)     |").append("\n");
    second.append(u8"+------+--------+------------------+----------------------------+--------------+------------------+-------------+-----------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXX | XXXXXX | ( XXXXX ) XXXXXX | XXXX.XXX XXXX.XXX XXXX.XXX | XXXXXXXXXX.X | XXXXXXXXXXXXXX.X | XXXXXXXXX.X | XXXXXXXXXXXXX.X |").append("\n");
        second.append(u8"+------+--------+------------------+----------------------------+--------------+------------------+-------------+-----------------+                                                           TOTAL  XXXXXXXXXX.X").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
