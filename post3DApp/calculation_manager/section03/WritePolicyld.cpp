#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WritePolicyld::Command = u8"*POLICYLD";
const QString WritePolicyld::Japanese = u8"形状認識・部材の荷重拾い壁量算定・応力解析荷重計算の考え方と設定";

WritePolicyld::WritePolicyld(const QStringList& list):AbstractWrite(list){}

QString WritePolicyld::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    /*
    auto second = AbstractText::CreateAndWrite(*UnifiedFixedData::getInstance(), list);
    QStringList osubh = second.second.split("\n");
    for (int i = 0; i < osubh.count(); i++){
        QString temp = osubh.at(i);
        line += text(temp.replace("'", "’"));
    }
    */
    line += suffix();
    return line;

}

} // namespace post3dapp
