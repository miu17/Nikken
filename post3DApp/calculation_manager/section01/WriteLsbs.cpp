#include "abstractwrite.h"
#include "nposk.h"
namespace post3dapp{

const QString WriteLsbs::Command = u8"*LSBS";
const QString WriteLsbs::Japanese = u8"S露出柱脚リスト";

WriteLsbs::WriteLsbs(const QStringList& list):AbstractWrite(list){};

QString WriteLsbs::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    Q_UNUSED(setting)
    Q_UNUSED(maker)
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

