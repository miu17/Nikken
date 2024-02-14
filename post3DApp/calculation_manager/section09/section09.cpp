#include "section09.h"
namespace post3dapp{

Section09::Section09()
{

}

void Section09::set(QString command, QStringList list){
    if (command == "*POLICYQUDS")  policyquds = new WritePolicyquds(list);
    if (command == "*QUDS")  quds = new WriteQuds(list);
    if (command == "*SEISLP")  seislp = new WriteSeislp(list);
    if (command == "*POLICYPC")  policypc = new WritePolicypc(list);
    if (command == "*PMODEL")  pmodel = new WritePmodel(list);
    if (command == "*DSPUSH")  dspush = new WriteDspush(list);
    if (command == "*DSGENE")  dsgene = new WriteDsgene(list);
    if (command == "*DSSTEP")  dsstep = new WriteDsstep(list);
    if (command == "*DSCURVE")  dscurve = new WriteDscurve(list);
    if (command == "*DSSTR")  dsstr = new WriteDsstr(list);
    if (command == "*DSCOND")  dscond = new WriteDscond(list);
    if (command == "*MRNK")  mrnk = new WriteMrnk(list);
    if (command == "*RNCS")  rncs = new WriteRncs(list);
    if (command == "*RNCRC")  rncrc = new WriteRncrc(list);
    if (command == "*RNCSRC")  rncsrc = new WriteRncsrc(list);
    if (command == "*RNCCFT")  rnccft = new WriteRnccft(list);
    if (command == "*RNGS")  rngs = new WriteRngs(list);
    if (command == "*RNGRC")  rngrc = new WriteRngrc(list);
    if (command == "*RNGSRC")  rngsrc = new WriteRngsrc(list);
    if (command == "*RNVS")  rnvs = new WriteRnvs(list);
    if (command == "*RNWRC")  rnwrc = new WriteRnwrc(list);
    if (command == "*RNWSRC")  rnwsrc = new WriteRnwsrc(list);
    if (command == "*RNPRC")  rnprc = new WriteRnprc(list);
    if (command == "*QUN")  qun = new WriteQun(list);
    if (command == "*GRBEND")  grbend = new WriteGrbend(list);
    if (command == "*GRBONDI")  grbondi = new WriteGrbondi(list);
    if (command == "*GRBONDP")  grbondp = new WriteGrbondp(list);
    if (command == "*YOKOHOGOU")  yokohogou = new WriteYokohogou(list);
    if (command == "*DSSH")  dssh = new WriteDssh(list);
    if (command == "*DSMU")  dsmu = new WriteDsmu(list);
    if (command == "*QUPUSH")  qupush = new WriteQupush(list);
    if (command == "*QUGENE")  qugene = new WriteQugene(list);
    if (command == "*QUSTEP")  qustep = new WriteQustep(list);
    if (command == "*QUCURVE")  qucurve = new WriteQucurve(list);
    if (command == "*QUCOND")  qucond = new WriteQucond(list);
    if (command == "*QUDETRM")  qudetrm = new WriteQudetrm(list);
    if (command == "*QUSTR")  qustr = new WriteQustr(list);
    if (command == "*QUCRC")  qucrc = new WriteQucrc(list);
    if (command == "*QUCSRC")  qucsrc = new WriteQucsrc(list);
    if (command == "*QUGRC")  qugrc = new WriteQugrc(list);
    if (command == "*QUGSRC")  qugsrc = new WriteQugsrc(list);
    if (command == "*QUWRC")  quwrc = new WriteQuwrc(list);
    if (command == "*QUWSRC")  quwsrc = new WriteQuwsrc(list);
    if (command == "*QUPS")  qups = new WriteQups(list);
    if (command == "*QUPRC")  quprc = new WriteQuprc(list);
    if (command == "*QUPSRC")  qupsrc = new WriteQupsrc(list);
    if (command == "*QUPCFT")  qupcft = new WriteQupcft(list);
    if (command == "*QUSH")  qush = new WriteQush(list);
    if (command == "*DSQUCURVE")  dsqucurve = new WriteDsqucurve(list);
    if (command == "*QUNEED")  quneed = new WriteQuneed(list);
    if (command == "*QUNEEDF")  quneedf = new WriteQuneedf(list);
    if (command == "*CGCMPM")  cgcmpm = new WriteCgcmpm(list);
    if (command == "*CGCMPF")  cgcmpf = new WriteCgcmpf(list);
    if (command == "*MBCPR")  mbcpr = new WriteMbcpr(list);
    if (command == "*QBCPR")  qbcpr = new WriteQbcpr(list);
    if (command == "*MULT")  mult = new WriteMult(list);
}

QString Section09::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter09}";
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (policyquds){
        txt += policyquds->context(maker, setting);
    }
    if (quds){
        txt += quds->context(maker, setting);
    }
    if (seislp){
        txt += seislp->context(maker, setting);
    }
    if (policypc){
        txt += policypc->context(maker, setting);
    }
    if (pmodel){
        txt += pmodel->context(maker, setting);
    }
    if (dspush){
        txt += dspush->context(maker, setting);
    }
    if (dsgene){
        txt += dsgene->context(maker, setting);
    }
    if (dsstep){
        txt += dsstep->context(maker, setting);
    }
    if (dscurve){
        txt += dscurve->context(maker, setting);
    }
    if (dsstr){
        txt += dsstr->context(maker, setting);
    }
    if (dscond){
        txt += dscond->context(maker, setting);
    }
    if (mrnk){
        txt += mrnk->context(maker, setting);
    }
    if (rncs){
        txt += rncs->context(maker, setting);
    }
    if (rncrc){
        txt += rncrc->context(maker, setting);
    }
    if (rncsrc){
        txt += rncsrc->context(maker, setting);
    }
    if (rnccft){
        txt += rnccft->context(maker, setting);
    }
    if (rngs){
        txt += rngs->context(maker, setting);
    }
    if (rngrc){
        txt += rngrc->context(maker, setting);
    }
    if (rngsrc){
        txt += rngsrc->context(maker, setting);
    }
    if (rnvs){
        txt += rnvs->context(maker, setting);
    }
    if (rnwrc){
        txt += rnwrc->context(maker, setting);
    }
    if (rnwsrc){
        txt += rnwsrc->context(maker, setting);
    }
    if (rnprc){
        txt += rnprc->context(maker, setting);
    }
    if (qun){
        txt += qun->context(maker, setting);
    }
    if (grbend){
        txt += grbend->context(maker, setting);
    }
    if (grbondi){
        txt += grbondi->context(maker, setting);
    }
    if (grbondp){
        txt += grbondp->context(maker, setting);
    }
    if (yokohogou){
        txt += yokohogou->context(maker, setting);
    }
    if (dssh){
        txt += dssh->context(maker, setting);
    }
    if (dsmu){
        txt += dsmu->context(maker, setting);
    }
    if (qupush){
        txt += qupush->context(maker, setting);
    }
    if (qugene){
        txt += qugene->context(maker, setting);
    }
    if (qustep){
        txt += qustep->context(maker, setting);
    }
    if (qucurve){
        txt += qucurve->context(maker, setting);
    }
    if (qucond){
        txt += qucond->context(maker, setting);
    }
    if (qudetrm){
        txt += qudetrm->context(maker, setting);
    }
    if (qustr){
        txt += qustr->context(maker, setting);
    }
    if (qucrc){
        txt += qucrc->context(maker, setting);
    }
    if (qucsrc){
        txt += qucsrc->context(maker, setting);
    }
    if (qugrc){
        txt += qugrc->context(maker, setting);
    }
    if (qugsrc){
        txt += qugsrc->context(maker, setting);
    }
    if (quwrc){
        txt += quwrc->context(maker, setting);
    }
    if (quwsrc){
        txt += quwsrc->context(maker, setting);
    }
    if (qups){
        txt += qups->context(maker, setting);
    }
    if (quprc){
        txt += quprc->context(maker, setting);
    }
    if (qupsrc){
        txt += qupsrc->context(maker, setting);
    }
    if (qupcft){
        txt += qupcft->context(maker, setting);
    }
    if (qush){
        txt += qush->context(maker, setting);
    }
    if (dsqucurve){
        txt += dsqucurve->context(maker, setting);
    }
    if (quneed){
        txt += quneed->context(maker, setting);
    }
    if (quneedf){
        txt += quneedf->context(maker, setting);
    }
    if (cgcmpm){
        txt += cgcmpm->context(maker, setting);
    }
    if (cgcmpf){
        txt += cgcmpf->context(maker, setting);
    }
    if (mbcpr){
        txt += mbcpr->context(maker, setting);
    }
    if (qbcpr){
        txt += qbcpr->context(maker, setting);
    }
    if (mult){
        txt += mult->context(maker, setting);
    }

    txt += u8"\\end{chapter09}";
    return txt;
}
} // namespace post3dapp
