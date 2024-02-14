#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteQbcpr::Command = u8"*QBCPR";
const QString WriteQbcpr::Japanese = u8"BCP,BCR使用柱の局部崩壊による保有水平耐力の評価";

WriteQbcpr::WriteQbcpr(const QStringList& list):AbstractWrite(list){};

QString WriteQbcpr::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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
