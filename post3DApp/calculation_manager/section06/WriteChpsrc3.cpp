#include "abstractwrite.h"
#include "nposk.h"
namespace post3dapp{
const QString WriteChpsrc3::Command = u8"*CHPSRC3";
const QString WriteChpsrc3::Japanese = u8"SRC 接合部パネルの検討( 終局 )";

WriteChpsrc3::WriteChpsrc3(const QStringList& list):AbstractWrite(list){};

QString WriteChpsrc3::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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
