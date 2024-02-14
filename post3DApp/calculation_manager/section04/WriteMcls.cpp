#include "abstractwrite.h"
namespace post3dapp{
const QString WriteMcls::Command = u8"*MCLS";
const QString WriteMcls::Japanese = u8"圧縮座屈長軸組図";

WriteMcls::WriteMcls(const QStringList& list):AbstractWrite(list){};

QString WriteMcls::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
