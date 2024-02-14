#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQugsrc::Command = u8"*QUGSRC";
const QString WriteQugsrc::Japanese = u8"保有水平耐力時のせん断余裕度( SRC 大梁 )";

WriteQugsrc::WriteQugsrc(const QStringList& list):AbstractWrite(list){};

QString WriteQugsrc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
