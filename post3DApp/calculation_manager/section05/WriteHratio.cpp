#include "abstractwrite.h"
#include "nposk.h"
namespace post3dapp{
const QString WriteHratio::Command = u8"*HRATIO";
const QString WriteHratio::Japanese = u8"主架構と耐震要素の分担率";

WriteHratio::WriteHratio(const QStringList& list):AbstractWrite(list){};

QString WriteHratio::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += u8"\\indentL{0.0}\n";
    line += u8"\\linespacing{0.1} \n";
    line += font(u8"ＭＳ 明朝", u8"6.0");
    /*
    QStringList osubh = Nposk::osubhTexts.value(command);
    for (int i = 0; i < osubh.count(); i++){
        line += text(osubh.at(i));
    }
    */
    line += suffix();
    return line;

}
} // namespace post3dapp
