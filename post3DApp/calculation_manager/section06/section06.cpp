#include "section06.h"
namespace post3dapp{

Section06::Section06()
{

}

void Section06::set(QString command, QStringList list){
    if(command =="*POLICYCH") policych = new WritePolicych(list);
    if(command =="*CHCASE") chcase = new WriteChcase(list);
    if(command =="*CHVP") chvp = new WriteChvp(list);
    if(command =="*CHVS") chvs = new WriteChvs(list);
    if(command =="*CHHP") chhp = new WriteChhp(list);
    if(command =="*CHHS") chhs = new WriteChhs(list);
    if(command =="*CHLISTM") chlistm = new WriteChlistm(list);
    if(command =="*CHLISTQ") chlistq = new WriteChlistq(list);
    if(command =="*CHCOL") chcol = new WriteChcol(list);
    if(command =="*CHGIR") chgir = new WriteChgir(list);
    if(command =="*CHWAL") chwal = new WriteChwal(list);
    if(command =="*CHBRA") chbra = new WriteChbra(list);
    if(command =="*CHOPEN") chopen = new WriteChopen(list);
    if(command =="*CHPS2") chps2 = new WriteChps2(list);
    if(command =="*CHPRC2") chprc2 = new WriteChprc2(list);
    if(command =="*CHPSRC2") chpsrc2 = new WriteChpsrc2(list);
    if(command =="*CHPCFT2") chpcft2 = new WriteChpcft2(list);
    if(command =="*CHPS3") chps3 = new WriteChps3(list);
    if(command =="*CHPRC3") chprc3 = new WriteChprc3(list);
    if(command =="*CHPSRC3") chpsrc3 = new WriteChpsrc3(list);
    if(command =="*CHPCFT3") chpcft3 = new WriteChpcft3(list);
    if(command =="*CHSRCBASE") chsrcbase = new WriteChsrcbase(list);
    if(command =="*CHSBASE") chsbase = new WriteChsbase(list);
    if(command =="*CHNEOBASE") chneobase = new WriteChneobase(list);
    if(command =="*CHNEWBASE") chnewbase = new WriteChnewbase(list);
    if(command =="*CHBPBASE") chbpbase = new WriteChbpbase(list);
    if(command =="*CHCUTOFF") chcutoff = new WriteChcutoff(list);
    if(command =="*CHBONDL1") chbondl1 = new WriteChbondl1(list);
    if(command =="*CHBONDS1") chbonds1 = new WriteChbonds1(list);
    if(command =="*CHBONDS2") chbonds2 = new WriteChbonds2(list);
}

QString Section06::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter06}";
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (policych){
        txt += policych->context(maker, setting);
    }
    if (chcase){
        txt += chcase->context(maker, setting);
    }
    if (chvp){
        txt += chvp->context(maker, setting);
    }
    if (chvs){
        txt += chvs->context(maker, setting);
    }
    if (chhp){
        txt += chhp->context(maker, setting);
    }
    if (chhs){
        txt += chhs->context(maker, setting);
    }
    if (chlistm){
        txt += chlistm->context(maker, setting);
    }
    if (chlistq){
        txt += chlistq->context(maker, setting);
    }
    if (chcol){
        txt += chcol->context(maker, setting);
    }
    if (chgir){
        txt += chgir->context(maker, setting);
    }
    if (chwal){
        txt += chwal->context(maker, setting);
    }
    if (chbra){
        txt += chbra->context(maker, setting);
    }
    if (chopen){
        txt += chopen->context(maker, setting);
    }
    if (chps2){
        txt += chps2->context(maker, setting);
    }
    if (chprc2){
        txt += chprc2->context(maker, setting);
    }
    if (chpsrc2){
        txt += chpsrc2->context(maker, setting);
    }
    if (chpcft2){
        txt += chpcft2->context(maker, setting);
    }
    if (chps3){
        txt += chps3->context(maker, setting);
    }
    if (chprc3){
        txt += chprc3->context(maker, setting);
    }
    if (chpsrc3){
        txt += chpsrc3->context(maker, setting);
    }
    if (chpcft3){
        txt += chpcft3->context(maker, setting);
    }
    if (chsrcbase){
        txt += chsrcbase->context(maker, setting);
    }
    if (chsbase){
        txt += chsbase->context(maker, setting);
    }
    if (chneobase){
        txt += chneobase->context(maker, setting);
    }
    if (chnewbase){
        txt += chnewbase->context(maker, setting);
    }
    if (chbpbase){
        txt += chbpbase->context(maker, setting);
    }
    if (chcutoff){
        txt += chcutoff->context(maker, setting);
    }
    if (chbondl1){
        txt += chbondl1->context(maker, setting);
    }
    if (chbonds1){
        txt += chbonds1->context(maker, setting);
    }
    if (chbonds2){
        txt += chbonds2->context(maker, setting);
    }

    txt += u8"\\end{chapter06}";
    return txt;
}
} // namespace post3dapp


