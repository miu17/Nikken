#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextMessage::write(const FixedData& fixedData) const{

    //PR702

    QString second;
    second.append(u8"+----------------------+----------------+----------------+----------------+----------------+----------------+").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|    サブシステム名    |   ＥＲＲＯＲ   | ＷＡＲＮＩＮＧ | ＮＯ　ＧＯＯＤ | ＣＡＵＴＩＯＮ |    実行状況    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|======================|================|================|================|================|================|").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|     建物形状入力     |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|       重量拾い       |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|    形状デ－タ変換    |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|  共通解析モデル作成  |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|       荷重作成       |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|    解析モデル作成    |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|       応力解析       |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"| 偏心率・剛性率等算定 |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|   Ｎ－Ｍ曲面の作成   |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|       断面検討       |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|  保有水平耐力の検討  |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|    計算内容の確認    |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXXXXX    |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"+----------------------+----------------+----------------+----------------+----------------+----------------+").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"|      合      計      |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXX       |                |").append("\n");
    second.append(u8"|                      |                |                |                |                |                |").append("\n");
    second.append(u8"+----------------------+----------------+----------------+----------------+----------------+----------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"").append("\n");
    //}
    second.append(u8"                                        構造計算は問題なく終了しました").append("\n");
    second.append(u8"                                       インプットデ－タを見直して下さい").append("\n");
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
