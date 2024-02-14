#include "section11.h"
namespace post3dapp{

Section11::Section11()
{

}

void Section11::set(QString command, QStringList list){
    if (command == "*OPINIONC") opinionc = new WriteOpinionc(list);
    if (command == "*OPINION") opinion = new WriteOpinion(list);
}

QString Section11::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter11}";
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (opinionc){
        txt += opinionc->context(maker, setting);
    }
    if (opinion){
        txt += opinion->context(maker, setting);
    }

    txt += u8"\\end{chapter11}";
    return txt;
}
} // namespace post3dapp
