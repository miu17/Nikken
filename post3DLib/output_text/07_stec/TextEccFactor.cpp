#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextEccFactor::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"■ 架構外雑壁剛性").append("\n");
    second.append(u8"■ 架構外雑壁剛性を 非考慮(階全体)").append("\n");
    second.append(u8"■ 架構外雑壁剛性を 考慮  (階全体)").append("\n");
    second.append(u8"■ 架構外雑壁剛性を 非考慮(指定領域)").append("\n");
    second.append(u8"■ 架構外雑壁剛性を 考慮  (指定領域)").append("\n");
    second.append(u8"■ 偏心率判定結果").append("\n");
    second.append(u8"+--------+-------------------+-------------------+-----------------------------------+-----------------------------------+-----------------------------------+").append("\n");
    second.append(u8"|        |                   |                   |                                   |  DIRECTION-1 ( ANGLE = XXXX.XX )  |  DIRECTION-2 ( ANGLE = XXXX.XX )  |").append("\n");
    second.append(u8"|        |                   |                   |                                   +-----------------------------------+-----------------------------------+").append("\n");
    second.append(u8"| FLOOR  |   CENTER OF S.F   |   CENTER OF RIG.  |    K1          K2          KR     |   RE1      E2      ECC.F.  JUDGE  |   RE2      E1      ECC.F.  JUDGE  |").append("\n");
    second.append(u8"|        |   XC(M)    YC(M)  |   XK(M)    YK(M)  | (KN/CM)     (KN/CM)    (KN*M2/CM) |   (M)      (M)                    |   (M)      (M)                    |").append("\n");
    second.append(u8"+--------+-------------------+-------------------+-----------------------------------+-----------------------------------+-----------------------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX |                   |                   |                                   |                                   |                                   |").append("\n");
        second.append(u8"|        | XXXX.XXX XXXX.XXX | XXXX.XXX XXXX.XXX |XXXXXXXX.X XXXXXXXX.X XXXXXXXXXXX. | XXXX.XXX XXXX.XXX XXX.XXX XXXXXXX | XXXX.XXX XXXX.XXX XXX.XXX XXXXXXX |").append("\n");
        second.append(u8"+--------+-------------------+-------------------+-----------------------------------+-----------------------------------+-----------------------------------+").append("\n");
    //}
    second.append(u8"+--------+--------+-------------------+-------------------+-----------------------------------+---------------------------+---------------------------+").append("\n");
    second.append(u8"|        |        |                   |                   |                                   | DIR-1 ( ANGLE = XXXX.XX ) | DIR-2 ( ANGLE = XXXX.XX ) |").append("\n");
    second.append(u8"|        |        |                   |                   |                                   +---------------------------+---------------------------+").append("\n");
    second.append(u8"|  AREA  | FLOOR  |   CENTER OF S.F   |   CENTER OF RIG.  |    K1          K2          KR     |   RE1      E2      ECC.F. |   RE2      E1      ECC.F. |").append("\n");
    second.append(u8"|        |        |   XC(M)    YC(M)  |   XK(M)    YK(M)  | (KN/CM)     (KN/CM)    (KN*M2/CM) |   (M)      (M)            |   (M)      (M)            |").append("\n");
    second.append(u8"+--------+--------+-------------------+-------------------+-----------------------------------+---------------------------+---------------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"|        |        |                   |                   |                                   |                           |                           |").append("\n");
        second.append(u8"| XXXXXX | XXXXXX | XXXX.XXX XXXX.XXX | XXXX.XXX XXXX.XXX |XXXXXXXX.X XXXXXXXX.X XXXXXXXXXXX. | XXXX.XXX XXXX.XXX XXX.XXX | XXXX.XXX XXXX.XXX XXX.XXX |").append("\n");
        second.append(u8"+--------+--------+-------------------+-------------------+-----------------------------------+---------------------------+---------------------------+").append("\n");
    //}
    second.append(u8"+--------+-------------------+---------------------------+---------------------------+").append("\n");
    second.append(u8"|        |                   | DIR-1 ( ANGLE = XXXX.XX ) | DIR-2 ( ANGLE = XXXX.XX ) |").append("\n");
    second.append(u8"|        |                   +---------------------------+---------------------------+").append("\n");
    second.append(u8"| FLOOR  |    AC      AWZ    |     KC           KZW      |     KC           KZW      |").append("\n");
    second.append(u8"|        |   (M2)     (M2)   |   (KN/CM)     (KN/CM/M2)  |   (KN/CM)     (KN/CM/M2)  |").append("\n");
    second.append(u8"+--------+-------------------+---------------------------+---------------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX |                   |                           |                           |").append("\n");
        second.append(u8"|        | XXXX.XXX XXXX.XXX |  XXXXXXXX.X   XXXXXX.XXX  |  XXXXXXXX.X   XXXXXX.XXX  |").append("\n");
        second.append(u8"+--------+-------------------+---------------------------+---------------------------+").append("\n");
    //}
    second.append(u8"+--------+-----------------------------------------------------------+-----------------------------------------------------------+").append("\n");
    second.append(u8"|        |             DIRECTION-1 ( ANGLE = XXXX.XX )               |             DIRECTION-2 ( ANGLE = XXXX.XX )               |").append("\n");
    second.append(u8"|        +-----------------------------------------------------------+-----------------------------------------------------------+").append("\n");
    second.append(u8"| FLOOR  |  階全体     階全体  指定領域  指定領域   判定用    判定   |  階全体     階全体  指定領域  指定領域   判定用    判定   |").append("\n");
    second.append(u8"|        |雑壁非考慮 雑壁考慮 雑壁非考慮 雑壁考慮                    |雑壁非考慮 雑壁考慮 雑壁非考慮 雑壁考慮                    |").append("\n");
    second.append(u8"+--------+-----------------------------------------------------------+-----------------------------------------------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX |                                                           |                                                           |").append("\n");
        second.append(u8"|        |  XXX.XXX   XXX.XXX   XXX.XXX   -------   XXX.XXX  XXXXXXX |  XXX.XXX   XXX.XXX   XXX.XXX   -------   XXX.XXX  XXXXXXX |").append("\n");
        second.append(u8"+--------+-----------------------------------------------------------+-----------------------------------------------------------+").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};}
} // namespace post3dapp
