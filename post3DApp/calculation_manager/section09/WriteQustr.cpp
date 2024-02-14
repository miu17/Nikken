#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQustr::Command = u8"*QUSTR";
const QString WriteQustr::Japanese = u8"保有水平耐力時の応力図";

WriteQustr::WriteQustr(const QStringList& list):AbstractWrite(list){};

QString WriteQustr::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp

