#include "abstractwrite.h"
namespace post3dapp{
const QString WriteMrnk::Command = u8"*MRNK";
const QString WriteMrnk::Japanese = u8"部材種別図";

WriteMrnk::WriteMrnk(const QStringList& list):AbstractWrite(list){};

QString WriteMrnk::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp

