#include "abstractwrite.h"
namespace post3dapp{
const QString WriteMclp::Command = u8"*MCLP";
const QString WriteMclp::Japanese = u8"圧縮座屈長伏図";

WriteMclp::WriteMclp(const QStringList& list):AbstractWrite(list){};

QString WriteMclp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
