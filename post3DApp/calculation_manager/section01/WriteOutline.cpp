#include "abstractwrite.h"
#include "drawtable.h"
namespace post3dapp{

const QString WriteOutline::Command = u8"*OUTLINE";
const QString WriteOutline::Japanese = u8"建物概要";

WriteOutline::WriteOutline(const QStringList& list):AbstractWrite(list){
}

QString WriteOutline::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    Q_UNUSED(setting)

    maker->setTableClass(new CalculationMakerLib::DrawTable(u8"TABLEA", 10));

    QString line;
    line += prefix();
    for (int i = 0; i < list.count(); i++)
    {
        line += text(list.at(i));
    }

    line += u8"\\includetable{TABLEA, CENTER, 160.0} \n";
    line += suffix();
    return line;
}

} // namespace post3dapp
