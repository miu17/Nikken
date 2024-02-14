#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteOpinionc::Command = u8"*OPINIONC";
const QString WriteOpinionc::Japanese = u8"計算結果（終了メッセージ）に対する所見";

WriteOpinionc::WriteOpinionc(const QStringList& list):AbstractWrite(list){};

QString WriteOpinionc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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

