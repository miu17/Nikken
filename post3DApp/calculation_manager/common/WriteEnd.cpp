#include "abstractwrite.h"
namespace post3dapp{
const QString WriteEnd::Command = u8"*END";
const QString WriteEnd::Japanese = QString{};

WriteEnd::WriteEnd(const QStringList& list):AbstractWrite(list){};

QString WriteEnd::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    return QString();
}

} // namespace post3dapp

