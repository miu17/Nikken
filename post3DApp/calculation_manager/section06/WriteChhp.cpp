#include "abstractwrite.h"
namespace post3dapp{
const QString WriteChhp::Command = u8"*CHHP";
const QString WriteChhp::Japanese = u8"水平荷重に対する架構の断面検討伏図(検定比)";

WriteChhp::WriteChhp(const QStringList& list):AbstractWrite(list){};

QString WriteChhp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
