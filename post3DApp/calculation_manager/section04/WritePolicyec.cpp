#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WritePolicyec::Command = u8"*POLICYEC";
const QString WritePolicyec::Japanese = u8"応力計算の考え方と設定";

WritePolicyec::WritePolicyec(const QStringList& list):AbstractWrite(list){};

QString WritePolicyec::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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

