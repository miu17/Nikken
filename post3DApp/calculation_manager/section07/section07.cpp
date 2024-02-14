#include "section07.h"
namespace post3dapp{

Section07::Section07()
{

}

void Section07::set(QString command, QStringList list){
    if (command == "*POLICYF") policyf = new WritePolicyf(list);
}

QString Section07::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter07}";
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (policyf){
        txt += policyf->context(maker, setting);
    }
    txt += u8"\\end{chapter07}";
    return txt;
}
} // namespace post3dapp
