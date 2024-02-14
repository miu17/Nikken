#include "abstractwrite.h"
namespace post3dapp{
const QString WriteDssh::Command = u8"*DSSH";
const QString WriteDssh::Japanese = u8"DS判定時のせん断余裕度図";

WriteDssh::WriteDssh(const QStringList& list):AbstractWrite(list){};

QString WriteDssh::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
