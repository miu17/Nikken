#include "abstractwrite.h"
namespace post3dapp{
const QString WriteChvp::Command = u8"*CHVP";
const QString WriteChvp::Japanese = u8"鉛直荷重に対する架構の断面検討伏図";

WriteChvp::WriteChvp(const QStringList& list):AbstractWrite(list){};

QString WriteChvp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
