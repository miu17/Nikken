#include "abstractwrite.h"
namespace post3dapp{
const QString WriteDscurve::Command = u8"*DSCURVE";
const QString WriteDscurve::Japanese = u8"各階の層せん断力－層間変形関係（ 構造特性係数Ds算定時 ）";

WriteDscurve::WriteDscurve(const QStringList& list):AbstractWrite(list){};

QString WriteDscurve::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
