#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WritePolicyquds::Command = u8"*POLICYQUDS";
const QString WritePolicyquds::Japanese = u8"保有水平耐力の検討方針";

WritePolicyquds::WritePolicyquds(const QStringList& list):AbstractWrite(list){};

QString WritePolicyquds::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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
