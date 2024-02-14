#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteCgcmpm::Command = u8"*CGCMPM";
const QString WriteCgcmpm::Japanese = u8"各節点毎の柱梁パネル耐力比 ( 冷間成型角型鋼管 )";

WriteCgcmpm::WriteCgcmpm(const QStringList& list):AbstractWrite(list){};

QString WriteCgcmpm::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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
