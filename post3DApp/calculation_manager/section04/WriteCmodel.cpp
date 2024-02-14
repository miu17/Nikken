#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteCmodel::Command = u8"*CMODEL";
const QString WriteCmodel::Japanese = u8"共通解析モデル化条件の設定";

WriteCmodel::WriteCmodel(const QStringList& list):AbstractWrite(list){};

QString WriteCmodel::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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


