#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextSeismicLoad::write(const FixedData& fixedData) const{
    //PR201
    QString second;
    second.append(u8"■ 荷重名     XXXXXX           荷重タイプ XXX").append("\n");
    second.append(u8"                               GL-FLOOR = XXXXXX   ROOF-FLOOR = XXXXXX   ( XXXXXXXX HEIGHT )").append("\n");
    second.append(u8"   Z = -----   TC(S) = -----   RT = -----   T1(S) = -----   C0 = -----   Ki0= -----").append("\n");
    second.append(u8"+--------+---------+---------+-----+---------+--------+--------+--------+--------+-----------+").append("\n");
    second.append(u8"| FLOOR  | TOTAL.H | FLOOR.H | STR |   WI    |  ALPI  |   AI   |   CI   |   KI   |     QI    |").append("\n");
    second.append(u8"|        |   (M)   |   (M)   |     |  (KN)   |        |        |        |        |    (KN)   |").append("\n");
    second.append(u8"+--------+---------+---------+-----+---------+--------+--------+--------+--------+-----------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX | XXX.XXX |         |     | XXXXX.X |        |        |        | XX.XXX |           |").append("\n");
        second.append(u8"|        |         |  XX.XXX | XXX |         | ------ | ------ | XX.XXX |        | XXXXXXX.X |").append("\n");
        second.append(u8"+--------+---------+---------+-----+---------+--------+--------+--------+--------+-----------+").append("\n");
    //}
    second.append(u8"+--------+---------+---------+-----+---------+--------+--------+--------+-----------+----+----+----+------+------+------+--------+-----------+").append("\n");
    second.append(u8"| FLOOR  | TOTAL.H | FLOOR.H | STR |   WI    |  ALPI  |   AI   |   CI   |    QUD    | FR | WR | BR |  DS  |  FE  |  FS  |   KI   |     QI    |").append("\n");
    second.append(u8"|        |   (M)   |   (M)   |     |  (KN)   |        |        |        |   (KN)    |    |    |    |      |      |      |        |    (KN)   |").append("\n");
    second.append(u8"+--------+---------+---------+-----+---------+--------+--------+--------+-----------+----+----+----+------+------+------+--------+-----------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX | XXX.XXX |         |     | XXXXX.X |        |        |        |           |    |    |    |      |      |      | XX.XXX |           |").append("\n");
        second.append(u8"|        |         |  XX.XXX | XXX |         | XX.XXX | XX.XXX | XX.XXX | XXXXXXX.X | XX | XX | XX | X.XX | X.XX | X.XX |        | XXXXXXX.X |").append("\n");
        second.append(u8"+--------+---------+---------+-----+---------+--------+--------+--------+-----------+----+----+----+------+------+------+--------+-----------+").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
