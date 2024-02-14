#include "abstractwrite.h"
namespace post3dapp{
const QString WriteQush::Command = u8"*QUSH";
const QString WriteQush::Japanese = u8"保有水平耐力時のせん断余裕度図";

WriteQush::WriteQush(const QStringList& list):AbstractWrite(list){};

QString WriteQush::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
