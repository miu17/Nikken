#include "abstractwrite.h"

namespace post3dapp{
const QString WriteGckcp::Command = u8"*GCKCP";
const QString WriteGckcp::Japanese = u8"層せん断力中心・剛心伏図";

WriteGckcp::WriteGckcp(const QStringList& list):AbstractWrite(list){};

QString WriteGckcp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += suffix();
    return line;

}
} // namespace post3dapp
