#include "abstractwrite.h"
namespace post3dapp{
const QString WriteMblp::Command = u8"*MBLP";
const QString WriteMblp::Japanese = u8"横座屈長伏図";

WriteMblp::WriteMblp(const QStringList& list):AbstractWrite(list){};

QString WriteMblp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
