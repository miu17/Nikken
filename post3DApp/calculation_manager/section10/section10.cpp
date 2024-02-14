#include "section10.h"
namespace post3dapp{

Section10::Section10()
{

}

void Section10::set(QString command, QStringList list){
    if (command == "*PROGRAME") programe = new WritePrograme(list);
    if (command == "*MESSAGE") message = new WriteMessage(list);
}

QString Section10::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter10}";
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (programe){
        txt += programe->context(maker, setting);
    }
    if (message){
        txt += message->context(maker, setting);
    }
    txt += u8"\\end{chapter10}";
    return txt;
}
} // namespace post3dapp
