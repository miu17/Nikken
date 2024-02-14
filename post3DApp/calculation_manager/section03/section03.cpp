#include "section03.h"
namespace post3dapp{

Section03::Section03()
{

}

void Section03::set(QString command, QStringList list){
    if (command == "*CCS") ccs = new WriteCcs(list);
    if (command == "*POLICYLD") policyld = new WritePolicyld(list);
    if (command == "*LDL") ldl = new WriteLdl(list);
    if (command == "*LDF") ldf = new WriteLdf(list);
    if (command == "*LDM") ldm = new WriteLdm(list);
    if (command == "*LDJ") ldj = new WriteLdj(list);
    if (command == "*SNOW") snow = new WriteSnow(list);
    if (command == "*WIND") wind = new WriteWind(list);
    if (command == "*OTHER") other = new WriteOther(list);
    if (command == "*LPLAN") lplan = new WriteLplan(list);
    if (command == "*SEISLE") seisle = new WriteSeisle(list);
    if (command == "*LCMQP") lcmqp = new WriteLcmqp(list);
    if (command == "*LCMQS") lcmqs = new WriteLcmqs(list);
    if (command == "*JMASSP") jmassp = new WriteJmassp(list);
    if (command == "*JMASSS") jmasss = new WriteJmasss(list);
    if (command == "*ESTIM") estim = new WriteEstim(list);
}

QString Section03::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter03} \n";
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (ccs){
        txt += ccs->context(maker, setting);
    }
    if (policyld){
        txt += policyld->context(maker, setting);
    }
    if (ldl){
        txt += ldl->context(maker, setting);
    }
    if (ldf){
        txt += ldf->context(maker, setting);
    }
    if (ldm){
        txt += ldm->context(maker, setting);
    }
    if (ldj){
        txt += ldj->context(maker, setting);
    }
    if (snow){
        txt += snow->context(maker, setting);
    }
    if (wind){
        txt += wind->context(maker, setting);
    }
    if (other){
        txt += other->context(maker, setting);
    }
    if (lplan){
        txt += lplan->context(maker, setting);
    }
    if (seisle){
        txt += seisle->context(maker, setting);
    }
    if (lcmqp){
        txt += lcmqp->context(maker, setting);
    }
    if (lcmqs){
        txt += lcmqs->context(maker, setting);
    }
    if (jmassp){
        txt += jmassp->context(maker, setting);
    }
    if (jmasss){
        txt += jmasss->context(maker, setting);
    }
    if (estim){
        txt += estim->context(maker, setting);
    }
    txt += u8"\\end{chapter03}";
    return txt;
}
} // namespace post3dapp
