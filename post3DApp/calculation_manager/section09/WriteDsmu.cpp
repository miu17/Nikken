#include "abstractwrite.h"
namespace post3dapp{
const QString WriteDsmu::Command = u8"*DSMU";
const QString WriteDsmu::Japanese = u8"DS判定時の耐力図";

WriteDsmu::WriteDsmu(const QStringList& list):AbstractWrite(list){};

QString WriteDsmu::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
