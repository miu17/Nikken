#include "section04.h"
namespace post3dapp{

Section04::Section04()
{

}

void Section04::set(QString command, QStringList list){
    if(command =="*POLICYEC") policyec = new WritePolicyec(list);
    if(command =="*CMODEL") cmodel = new WriteCmodel(list);
    if(command =="*EMODEL") emodel = new WriteEmodel(list);
    if(command =="*MODELP") modelp = new WriteModelp(list);
    if(command =="*MODELS") models = new WriteModels(list);
    if(command =="*MPHAIP") mphaip = new WriteMphaip(list);
    if(command =="*MPHAIS") mphais = new WriteMphais(list);
    if(command =="*MRLP") mrlp = new WriteMrlp(list);
    if(command =="*MRLS") mrls = new WriteMrls(list);
    if(command =="*MCLP") mclp = new WriteMclp(list);
    if(command =="*MCLS") mcls = new WriteMcls(list);
    if(command =="*MBLP") mblp = new WriteMblp(list);
    if(command =="*MBLS") mbls = new WriteMbls(list);
    if(command =="*ANACON") anacon = new WriteAnacon(list);
    if(command =="*RFLOOR") rfloor = new WriteRfloor(list);
    if(command =="*JSPRING") jspring = new WriteJspring(list);
    if(command =="*MSPRING") mspring = new WriteMspring(list);
    if(command =="*MDETAIL") mdetail = new WriteMdetail(list);
    if(command =="*AUTOPHAI") autophai = new WriteAutophai(list);
    if(command =="*AUTOLK") autolk = new WriteAutolk(list);
    if(command =="*JNT") jnt = new WriteJnt(list);
    if(command =="*INDEX") index = new WriteIndex(list);
    if(command =="*FRAME") frame = new WriteFrame(list);
    if(command =="*PNLMDL") pnlmdl = new WritePnlmdl(list);
}

QString Section04::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter04}";
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (policyec){
        txt += policyec->context(maker, setting);
    }
    if (cmodel){
        txt += cmodel->context(maker, setting);
    }
    if (emodel){
        txt += emodel->context(maker, setting);
    }
    if (modelp){
        txt += modelp->context(maker, setting);
    }
    if (models){
        txt += models->context(maker, setting);
    }
    if (mphaip){
        txt += mphaip->context(maker, setting);
    }
    if (mphais){
        txt += mphais->context(maker, setting);
    }
    if (mrlp){
        txt += mrlp->context(maker, setting);
    }
    if (mrls){
        txt += mrls->context(maker, setting);
    }
    if (mclp){
        txt += mclp->context(maker, setting);
    }
    if (mcls){
        txt += mcls->context(maker, setting);
    }
    if (mblp){
        txt += mblp->context(maker, setting);
    }
    if (mbls){
        txt += mbls->context(maker, setting);
    }
    if (anacon){
        txt += anacon->context(maker, setting);
    }
    if (rfloor){
        txt += rfloor->context(maker, setting);
    }
    if (jspring){
        txt += jspring->context(maker, setting);
    }
    if (mspring){
        txt += mspring->context(maker, setting);
    }
    if (mdetail){
        txt += mdetail->context(maker, setting);
    }
    if (autophai){
        txt += autophai->context(maker, setting);
    }
    if (autolk){
        txt += autolk->context(maker, setting);
    }
    if (jnt){
        txt += jnt->context(maker, setting);
    }
    if (index){
        txt += index->context(maker, setting);
    }
    if (frame){
        txt += frame->context(maker, setting);
    }
    if (pnlmdl){
        txt += pnlmdl->context(maker, setting);
    }

    txt += u8"\\end{chapter04}";
    return txt;
}
} // namespace post3dapp
