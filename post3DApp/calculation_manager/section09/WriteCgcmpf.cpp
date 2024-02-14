#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteCgcmpf::Command = u8"*CGCMPF";
const QString WriteCgcmpf::Japanese = u8"階毎に集計した柱梁パネル耐力比 ( 冷間成型角型鋼管 )";

WriteCgcmpf::WriteCgcmpf(const QStringList& list):AbstractWrite(list){};

QString WriteCgcmpf::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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
