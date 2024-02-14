#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextUltimateState::write(const FixedData& fixedData) const{
    //PR607

    QString second;
    second.append(u8"XXX ZZ- ZZ  想定終局せん断力 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8" ( ANGLE = XXXX.XX )").append("\n");
    second.append(u8"+--------+-----+-----------+-----------------------------------------+-----------------------------------------+").append("\n");
    second.append(u8"| FLOOR  | STR |           |            LAST STEP STATE              |           ASSUMED BROKEN STATE          |").append("\n");
    second.append(u8"|        |     |     QUN   |   FRAME    BRACE      WALL     TOTAL    |    FRAME  ( BRACE ) (  WALL ) ( TOTAL ) |").append("\n");
    second.append(u8"|        |     |    (KN)   |    (KN)     (KN)      (KN)      (KN)    |     (KN)     (KN)      (KN)      (KN)   |").append("\n");
    second.append(u8"+--------+-----+-----------+-----------------------------------------+-----------------------------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX |     |           |                                         |                                         |").append("\n");
        second.append(u8"|        | XXX | XXXXXXX.X | XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X | XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X |").append("\n");
        second.append(u8"+--------+-----+-----------+-----------------------------------------+-----------------------------------------+").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
