#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteCwarea::Command = u8"*CWAREA";
const QString WriteCwarea::Japanese = u8"壁と柱の水平断面積";

WriteCwarea::WriteCwarea(const QStringList& list):AbstractWrite(list){};

QString WriteCwarea::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += u8"\\indentL{0.0}\n";
    line += u8"\\linespacing{0.1} \n";
    line += font(u8"ＭＳ 明朝", u8"6.0");
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
