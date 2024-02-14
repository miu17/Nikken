#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQucrc::Command = u8"*QUCRC";
const QString WriteQucrc::Japanese = u8"保有水平耐力時のせん断余裕度( RC 柱 )";

WriteQucrc::WriteQucrc(const QStringList& list):AbstractWrite(list){};

QString WriteQucrc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
