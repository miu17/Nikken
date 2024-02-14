#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{

const QString WriteMbcpr::Command = u8"*MBCPR";
const QString WriteMbcpr::Japanese = u8"BCP,BCR使用各柱の局部崩壊の判定時保有せん断力";

WriteMbcpr::WriteMbcpr(const QStringList& list):AbstractWrite(list){};

QString WriteMbcpr::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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

