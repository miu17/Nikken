#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteAutophai::Command = u8"*AUTOPHAI";
const QString WriteAutophai::Japanese = u8"スラブによる大梁の曲げ剛性増大率の自動計算結果";

WriteAutophai::WriteAutophai(const QStringList& list):AbstractWrite(list){};

QString WriteAutophai::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    /*
    auto second = AbstractText::CreateAndWrite(*UnifiedFixedData::getInstance(), list);
    QStringList osubh = second.second.split("\n");
    for (int i = 0; i < osubh.count(); i++){
        line += text(osubh.at(i));
    }
    */
    line += suffix();
    return line;

}

} // namespace post3dapp
