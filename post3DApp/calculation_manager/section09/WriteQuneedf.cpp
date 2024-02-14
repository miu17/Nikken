#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQuneedf::Command = u8"*QUNEEDF";
const QString WriteQuneedf::Japanese = u8"必要保有水平耐力と保有水平耐力の比較図";

WriteQuneedf::WriteQuneedf(const QStringList& list):AbstractWrite(list){};

QString WriteQuneedf::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
