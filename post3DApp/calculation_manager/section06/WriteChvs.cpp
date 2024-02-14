#include "abstractwrite.h"
namespace post3dapp{
const QString WriteChvs::Command = u8"*CHVS";
const QString WriteChvs::Japanese = u8"鉛直荷重に対する架構の断面検討軸組図";

WriteChvs::WriteChvs(const QStringList& list):AbstractWrite(list){};

QString WriteChvs::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
