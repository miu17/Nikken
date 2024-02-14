#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQucsrc::Command = u8"*QUCSRC";
const QString WriteQucsrc::Japanese = u8"保有水平耐力時のせん断余裕度( SRC 柱 )";

WriteQucsrc::WriteQucsrc(const QStringList& list):AbstractWrite(list){};

QString WriteQucsrc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
