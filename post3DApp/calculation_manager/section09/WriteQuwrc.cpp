#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQuwrc::Command = u8"*QUWRC";
const QString WriteQuwrc::Japanese = u8"保有水平耐力時のせん断余裕度( RC 耐力壁 )";

WriteQuwrc::WriteQuwrc(const QStringList& list):AbstractWrite(list){};

QString WriteQuwrc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
