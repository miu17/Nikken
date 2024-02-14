#include "abstractwrite.h"
namespace post3dapp{
const QString WriteDsstr::Command = u8"*DSSTR";
const QString WriteDsstr::Japanese = u8"構造特性係数Ds算定のための荷重増分解析最終ステップ応力図";

WriteDsstr::WriteDsstr(const QStringList& list):AbstractWrite(list){};

QString WriteDsstr::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
