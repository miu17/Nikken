#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQups::Command = u8"*QUPS";
const QString WriteQups::Japanese = u8"保有水平耐力時のせん断余裕度( S 柱梁接合部パネル )";

WriteQups::WriteQups(const QStringList& list):AbstractWrite(list){};

QString WriteQups::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
