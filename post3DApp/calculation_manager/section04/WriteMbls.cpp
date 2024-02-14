#include "abstractwrite.h"
namespace post3dapp{
const QString WriteMbls::Command = u8"*MBLS";
const QString WriteMbls::Japanese = u8"横座屈長軸組図";

WriteMbls::WriteMbls(const QStringList& list):AbstractWrite(list){};

QString WriteMbls::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
