#include "abstractwrite.h"
namespace post3dapp{

const QString WriteContents2::Command = u8"*CONTENTS2";
const QString WriteContents2::Japanese = u8"構造計算書（その2）目次";

WriteContents2::WriteContents2(const QStringList& list):AbstractWrite(list){};

QString WriteContents2::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    Q_UNUSED(setting)
    Q_UNUSED(maker)

    return QString();
}

} // namespace post3dapp
