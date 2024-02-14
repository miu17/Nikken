#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQugrc::Command = u8"*QUGRC";
const QString WriteQugrc::Japanese = u8"保有水平耐力時のせん断余裕度( RC 大梁 )";

WriteQugrc::WriteQugrc(const QStringList& list):AbstractWrite(list){};

QString WriteQugrc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
