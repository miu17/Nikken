#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextQuNeed::write(const FixedData& fixedData) const{
    //PR606

    QString second;
    second.append(u8"XXX ZZ- ZZ  必要保有水平耐力と保有水平耐力の比較表 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"■  XXXXXXXXXX   ( ANGLE = XXXX.XX )   DS : XXXXXXXXXX  QU : XXXXXXXXXX").append("\n");
    second.append(u8"+--------+-----+----------------------+----------------------+------+--------------------------+-----------+-----------+--------+------+").append("\n");
    second.append(u8"|        |解析 |   柱・梁群のランク   | 耐震要素群のランク   |耐震要|                          | 必要保有  |   保有    |        |      |").append("\n");
    second.append(u8"|  階名  |構造 |  FA  FB  FC  FD RANK |  WA  WB  WC  WD RANK |素分担|   DS   FS   FE    Ｑud   | 水平耐力  | 水平耐力  | Qu/Qun | 判定 |").append("\n");
    second.append(u8"|        |形式 |   %   %   %   %      |   %   %   %   %      |率 βu|                   (kN)   | Ｑun(kN)  |  Ｑu(kN)  |        |      |").append("\n");
    second.append(u8"+--------+-----+----------------------+----------------------+------+--------------------------+-----------+-----------+--------+------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX |     |                      |                      |      |                          |           |           |        |      |").append("\n");
        second.append(u8"|        | XXX | XXX XXX XXX XXX  X   | XXX XXX XXX XXX  X   | X.XX | X.XX X.XX X.XX XXXXXXX.X | XXXXXXX.X | XXXXXXX.X | XXX.XX |  XX  |").append("\n");
        second.append(u8"+--------+-----+----------------------+----------------------+------+--------------------------+-----------+-----------+--------+------+").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
