#include "abstractwrite.h"
namespace post3dapp{
const QString WriteDsqucurve::Command = u8"*DSQUCURVE";
const QString WriteDsqucurve::Japanese = u8"各階の層せん断力－層間変形関係";

WriteDsqucurve::WriteDsqucurve(const QStringList& list):AbstractWrite(list){};

QString WriteDsqucurve::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
