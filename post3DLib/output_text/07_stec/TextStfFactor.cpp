#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextStfFactor::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"■ 架構外雑壁剛性").append("\n");
    second.append(u8"■ 架構外雑壁剛性を 非考慮(階全体)").append("\n");
    second.append(u8"■ 架構外雑壁剛性を 考慮  (階全体)").append("\n");
    second.append(u8"■ 架構外雑壁剛性を 非考慮(指定領域のグループ)").append("\n");
    second.append(u8"■ 架構外雑壁剛性を 考慮  (指定領域のグループ)").append("\n");
    second.append(u8"■ 剛性率判定結果").append("\n");
    second.append(u8"+--------+---------------------------------------------------------------+---------------------------------------------------------------+").append("\n");
    second.append(u8"|        |              DIRECTION-1 ( ANGLE = XXXX.XX )                  |               DIRECTION-2 ( ANGLE = XXXX.XX )                 |").append("\n");
    second.append(u8"|        +---------------------------------------------------------------+---------------------------------------------------------------+").append("\n");
    second.append(u8"| FLOOR  |  SHEAR.F  STIFFNESS    DISP. FLOOR.H  H/DISP.  S.D.F   JUDGE  |   SHEAR.F  STIFFNESS   DISP. FLOOR.H  H/MXD    S.D.F  JUDGE   |").append("\n");
    second.append(u8"|        |   (KN)      (KN/CM)    (MM)    (M)                            |    (KN)      (KN/CM)   (MM)    (M)                            |").append("\n");
    second.append(u8"+--------+---------------------------------------------------------------+---------------------------------------------------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX |                                                               |                                                               |").append("\n");
        second.append(u8"|        | XXXXXX.X XXXXXXXX.XX XXXXX.XX XX.XXX XXXXXXX. XXX.XXX XXXXXXX | XXXXXX.X XXXXXXXX.XX XXXXX.XX XX.XXX XXXXXXX. XXX.XXX XXXXXXX |").append("\n");
        second.append(u8"+--------+---------------------------------------------------------------+---------------------------------------------------------------+").append("\n");
    //}
    second.append(u8"+--------+--------+---------------------------------------------------------------+---------------------------------------------------------------+").append("\n");
    second.append(u8"|        |        |              DIRECTION-1 ( ANGLE = XXXX.XX )                  |               DIRECTION-2 ( ANGLE = XXXX.XX )                 |").append("\n");
    second.append(u8"|        |        +---------------------------------------------------------------+---------------------------------------------------------------+").append("\n");
    second.append(u8"| AREA   | FLOOR  |  SHEAR.F  STIFFNESS    DISP. FLOOR.H  H/DISP.  S.D.F   JUDGE  |   SHEAR.F  STIFFNESS   DISP. FLOOR.H  H/MXD    S.D.F  JUDGE   |").append("\n");
    second.append(u8"|        |        |   (KN)      (KN/CM)    (MM)    (M)                            |    (KN)      (KN/CM)   (MM)    (M)                            |").append("\n");
    second.append(u8"+--------+--------+---------------------------------------------------------------+---------------------------------------------------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"|        |        |                                                               |                                                               |").append("\n");
        second.append(u8"| XXXXXX | XXXXXX | XXXXXX.X XXXXXXXX.XX XXXXX.XX XX.XXX XXXXXXX. XXX.XXX XXXXXXX | XXXXXX.X XXXXXXXX.XX XXXXX.XX XX.XXX XXXXXXX. XXX.XXX XXXXXXX |").append("\n");
        second.append(u8"+--------+--------+---------------------------------------------------------------+---------------------------------------------------------------+").append("\n");
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
    second.append(u8"| FLOOR  |  階全体     階全体 指定領域G  指定領域G  判定用    判定   |  階全体     階全体 指定領域G  指定領域G  判定用    判定   |").append("\n");
    second.append(u8"|        |雑壁非考慮 雑壁考慮 雑壁非考慮 雑壁考慮                    |雑壁非考慮 雑壁考慮 雑壁非考慮 雑壁考慮                    |").append("\n");
    second.append(u8"+--------+-----------------------------------------------------------+-----------------------------------------------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"").append("\n");
        second.append(u8"").append("\n");
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
