#include "abstractwrite.h"
namespace post3dapp{

const QString WriteContents::Command = u8"*CONTENTS";
const QString WriteContents::Japanese = u8"構造計算書（その1）目次";

WriteContents::WriteContents(const QStringList& list):AbstractWrite(list){};

QString WriteContents::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    Q_UNUSED(setting)
    Q_UNUSED(maker)
    return u8"\\contents{} \n";
}
} // namespace post3dapp



