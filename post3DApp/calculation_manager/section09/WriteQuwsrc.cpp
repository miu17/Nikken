#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQuwsrc::Command = u8"*QUWSRC";
const QString WriteQuwsrc::Japanese = u8"保有水平耐力時のせん断余裕度( SRC 耐力壁 )";

WriteQuwsrc::WriteQuwsrc(const QStringList& list):AbstractWrite(list){};

QString WriteQuwsrc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
