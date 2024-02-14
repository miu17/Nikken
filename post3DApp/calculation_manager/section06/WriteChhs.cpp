#include "abstractwrite.h"
namespace post3dapp{
const QString WriteChhs::Command = u8"*CHHS";
const QString WriteChhs::Japanese = u8"水平荷重に対する架構の断面検討軸組図(検定比)";

WriteChhs::WriteChhs(const QStringList& list):AbstractWrite(list){};

QString WriteChhs::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
