#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQupsrc::Command = u8"*QUPSRC";
const QString WriteQupsrc::Japanese = u8"保有水平耐力時のせん断余裕度( SRC 柱梁接合部パネル )";

WriteQupsrc::WriteQupsrc(const QStringList& list):AbstractWrite(list){};

QString WriteQupsrc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp

