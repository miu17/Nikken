#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WritePnlmdl::Command = u8"*PNLMDL";
const QString WritePnlmdl::Japanese = u8"柱梁接合部パネルのモデル化";


WritePnlmdl::WritePnlmdl(const QStringList& list):AbstractWrite(list){};

QString WritePnlmdl::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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
