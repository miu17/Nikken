#include "abstractwrite.h"
namespace post3dapp{
const QString WriteCalc::Command = u8"*CALC";
const QString WriteCalc::Japanese = u8"構造設計方針";

WriteCalc::WriteCalc(const QStringList& list):AbstractWrite(list){};

QString WriteCalc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    Q_UNUSED(setting)
    Q_UNUSED(maker)
    QString line;
    line += prefix();
    line += subsection(u8"計算ルート");
    line += text(u8"X方向 ルート3");
    line += text(u8"Y方向　ルート3");
    line += subsection(u8"適用する構造計算");

    return line;
}

} // namespace post3dapp
