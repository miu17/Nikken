#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WritePolicystec::Command = u8"*POLICYSTEC";
const QString WritePolicystec::Japanese = u8"剛性率・偏心率の計算方針";

WritePolicystec::WritePolicystec(const QStringList& list):AbstractWrite(list){};

QString WritePolicystec::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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
