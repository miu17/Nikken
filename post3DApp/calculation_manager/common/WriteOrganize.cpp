#include "abstractwrite.h"
namespace post3dapp{

const QString WriteOrganize::Command = u8"*ORGANIZE";
const QString WriteOrganize::Japanese = u8"構成と見方";

WriteOrganize::WriteOrganize(const QStringList& list):AbstractWrite(list){};

QString WriteOrganize::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    Q_UNUSED(setting)
    Q_UNUSED(maker)
    QString line;
    line += begin();
    line += newPage();
    line += title(u8"・BUILDING 3D構造計算書の構成とその見方");
    line += text(u8"BUILDING 3D");
    line += text(u8"構造計算書の構成とその見方");
    line += text(u8"[構造計算書の構成]");
    line += text(u8"◆建築物の構造設計概要・・・・・・・(システム出力)+(一部手書き)");
    line += text(u8"◆設計方針と使用材料　　・・・・・・・(システム出力)+(一部手書き)");
    line += text(u8"◆荷重・外力の条件　　　　・・・・・・・(システム出力)+(一部手書き)");
    line += text(u8"◆応力計算基本仮定　　　・・・・・・・(システム出力)+(一部手書き)");
    line += text(u8"◆断面検討　　　　　　　　　・・・・・・・(システム出力)+(一部手書き)");
    line += text(u8"◆基礎・地盤の検討　　　　・・・・・・・(システム出力)+(一部手書き)");
    line += text(u8"◆層間変形角・剛性率・偏心率等・(システム出力)+(一部手書き)");
    line += text(u8"◆保有水平耐力　　　　　　・・・・・・・(システム出力)+(一部手書き)");
    line += text(u8"◆プログラムの運用状況　　・・・・・・・(システム出力)+(一部手書き)");
    line += text(u8"　　　　　　　　　　　　　　　　　　　　　　 システムの作動状態のまとめ、建物がシステムの");
    line += text(u8"　　　　　　　　　　　　　　　　　　　　　　 適用範囲にあることを確認");
    line += text(u8"◆総合所見　　　　　　　　　・・・・・・・(システム出力)+(一部手書き)");
    line += end();
    return line;
}

} // namespace post3dapp

