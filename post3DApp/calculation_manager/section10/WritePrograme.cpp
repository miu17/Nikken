#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WritePrograme::Command = u8"*PROGRAME";
const QString WritePrograme::Japanese = u8"プログラムの運用のためにモデル化した箇所とその説明";

WritePrograme::WritePrograme(const QStringList& list):AbstractWrite(list){};

QString WritePrograme::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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
