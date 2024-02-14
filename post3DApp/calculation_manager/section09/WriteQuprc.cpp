#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQuprc::Command = u8"*QUPRC";
const QString WriteQuprc::Japanese = u8"保有水平耐力時のせん断余裕度( RC 柱梁接合部パネル )";

WriteQuprc::WriteQuprc(const QStringList& list):AbstractWrite(list){};

QString WriteQuprc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
