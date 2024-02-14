#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQupcft::Command = u8"*QUPCFT";
const QString WriteQupcft::Japanese = u8"保有水平耐力時のせん断余裕度( CFT 柱梁接合部パネル )";

WriteQupcft::WriteQupcft(const QStringList& list):AbstractWrite(list){};

QString WriteQupcft::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
