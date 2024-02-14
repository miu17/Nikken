#include "section05.h"
namespace post3dapp{

Section05::Section05()
{

}

void Section05::set(QString command, QStringList list){
    if(command =="*OUTCS") outcs = new WriteOutcs(list);
    if(command =="*VJSTRP") vjstrp = new WriteVjstrp(list);
    if(command =="*VJSTRS") vjstrs = new WriteVjstrs(list);
    if(command =="*VFSTRP") vfstrp = new WriteVfstrp(list);
    if(command =="*VFSTRS") vfstrs = new WriteVfstrs(list);
    if(command =="*HRATIO") hratio = new WriteHratio(list);
    if(command =="*HJSTRP") hjstrp = new WriteHjstrp(list);
    if(command =="*HJSTRS") hjstrs = new WriteHjstrs(list);
    if(command =="*HFSTRP") hfstrp = new WriteHfstrp(list);
    if(command =="*HFSTRS") hfstrs = new WriteHfstrs(list);
    if(command =="*CREZ") crez = new WriteCrez(list);
    if(command =="*REACT") react = new WriteReact(list);
    if(command =="*STSTIFF") ststiff = new WriteStstiff(list);
    if(command =="*FRSTIFF") frstiff = new WriteFrstiff(list);
    if(command =="*FRDIST") frdist = new WriteFrdist(list);
    if(command =="*QMAP") qmap = new WriteQmap(list);

}

QString Section05::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter05}";
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (outcs){
        txt += outcs->context(maker, setting);
    }
    if (vjstrp){
        txt += vjstrp->context(maker, setting);
    }
    if (vjstrs){
        txt += vjstrs->context(maker, setting);
    }
    if (vfstrp){
        txt += vfstrp->context(maker, setting);
    }
    if (vfstrs){
        txt += vfstrs->context(maker, setting);
    }
    if (hratio){
        txt += hratio->context(maker, setting);
    }
    if (hjstrp){
        txt += hjstrp->context(maker, setting);
    }
    if (hfstrp){
        txt += hfstrp->context(maker, setting);
    }
    if (crez){
        txt += crez->context(maker, setting);
    }
    if (react){
        txt += react->context(maker, setting);
    }
    if (ststiff){
        txt += ststiff->context(maker, setting);
    }
    if (frstiff){
        txt += frstiff->context(maker, setting);
    }
    if (frdist){
        txt += frdist->context(maker, setting);
    }
    if (qmap){
        txt += qmap->context(maker, setting);
    }

    txt += u8"\\end{chapter05}";
    return txt;
}
} // namespace post3dapp
