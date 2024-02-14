#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteEmodel::Command = u8"*EMODEL";
const QString WriteEmodel::Japanese = u8"許容応力度設計用解析モデル毎の修正条件";

WriteEmodel::WriteEmodel(const QStringList& list):AbstractWrite(list){};

QString WriteEmodel::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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


