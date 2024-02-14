#include "section02.h"
namespace post3dapp{

Section02::Section02()
{

}

void Section02::set(QString command, QStringList list){
    if (command == "*CALC") calc = new WriteCalc(list);
    if (command == "*STND") stnd = new WriteStnd(list);
    if (command == "*POLICY") policy = new WritePolicy(list);
    if (command == "*STT") stt = new WriteStt(list);
    if (command == "*MATC") matc = new WriteMatc(list);
    if (command == "*MATR") matr = new WriteMatr(list);
    if (command == "*MATS") mats = new WriteMats(list);
}

QString Section02::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter02} \n";
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (calc){
        txt += calc->context(maker, setting);
    }
    if (stnd){
        txt += stnd->context(maker, setting);
    }
    if (policy){
        txt += policy->context(maker, setting);
    }
    if (stt){
        txt += stt->context(maker, setting);
    }
    if (matc){
        txt += matc->context(maker, setting);
    }
    if (matr){
        txt += matr->context(maker, setting);
    }
    if (mats){
        txt += mats->context(maker, setting);
    }

    txt += u8"\\end{chapter02} \n";
    return txt;

}
} // namespace post3dapp

