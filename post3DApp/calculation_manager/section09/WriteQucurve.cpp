#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQucurve::Command = u8"*QUCURVE";
const QString WriteQucurve::Japanese = u8"各階の層せん断力－層間変形関係（ 保有水平耐力算定時 ）";


WriteQucurve::WriteQucurve(const QStringList& list):AbstractWrite(list){};

QString WriteQucurve::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
