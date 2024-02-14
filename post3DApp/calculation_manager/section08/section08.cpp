#include "section08.h"
namespace post3dapp{

Section08::Section08()
{

}

void Section08::set(QString command, QStringList list){
    if (command == "*POLICYSTEC") policystec = new WritePolicystec(list);
    if (command == "*MXDFRM") mxdfrm = new WriteMxdfrm(list);
    if (command == "*STFF") stff = new WriteStff(list);
    if (command == "*ECCF") eccf = new WriteEccf(list);
    if (command == "*PRESTEC") prestec = new WritePrestec(list);
    if (command == "*GCKCP") gckcp = new WriteGckcp(list);
    if (command == "*STECF") stecf = new WriteStecf(list);
    if (command == "*CWAREA") cwarea = new WriteCwarea(list);
    if (command == "*CGCMP1") cgcmp1 = new WriteCgcmp1(list);
}

QString Section08::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter08}";
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (policystec){
        txt += policystec->context(maker, setting);
    }
    if (mxdfrm){
        txt += mxdfrm->context(maker, setting);
    }
    if (stff){
        txt += stff->context(maker, setting);
    }
    if (eccf){
        txt += eccf->context(maker, setting);
    }
    if (prestec){
        txt += prestec->context(maker, setting);
    }
    if (gckcp){
        txt += gckcp->context(maker, setting);
    }
    if (stecf){
        txt += stecf->context(maker, setting);
    }
    if (cwarea){
        txt += cwarea->context(maker, setting);
    }
    if (cgcmp1){
        txt += cgcmp1->context(maker, setting);
    }

    txt += u8"\\end{chapter08}";
    return txt;
}
} // namespace post3dapp
