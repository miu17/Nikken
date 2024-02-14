#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteStnd::Command = u8"*STND";
const QString WriteStnd::Japanese = u8"準拠基規準";

WriteStnd::WriteStnd(const QStringList& list):AbstractWrite(list){};

QString WriteStnd::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += subsection(u8"準拠基・規準");
    /*
    auto second = AbstractText::CreateAndWrite(*UnifiedFixedData::getInstance(), list);
    QStringList osubh = second.second.split("\n");
    for (int i = 0; i < osubh.count(); i++){
        line += text(osubh.at(i));
    }
    */
    line += end();
    return line;
}
} // namespace post3dapp

