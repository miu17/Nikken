#include "section01.h"
namespace post3dapp{

Section01::Section01()
{
}

void Section01::set(QString command, QStringList list){
    if(command =="*OUTLINE") outline = new WriteOutline(list);
    if(command =="*PLAN") plan = new WritePlan(list);
    if(command =="*SECTION") section = new WriteSection(list);
    if(command =="*LSGIR") lsgir = new WriteLsgir(list);
    if(command =="*LSCOL") lscol = new WriteLscol(list);
    if(command =="*LSWAL") lswal = new WriteLswal(list);
    if(command =="*LSSWA") lsswa = new WriteLsswa(list);
    if(command =="*LSFWA") lsfwa = new WriteLsfwa(list);
    if(command =="*LSBRA") lsbra = new WriteLsbra(list);
    if(command =="*LSBEAM") lsbeam = new WriteLsbeam(list);
    if(command =="*LSSLAB") lsslab = new WriteLsslab(list);
    if(command =="*LSOPEN") lsopen = new WriteLsopen(list);
    if(command =="*LSBS") lsbs = new WriteLsbs(list);
    if(command =="*LSBSRC") lsbsrc = new WriteLsbsrc(list);
    if(command =="*LSDAMP") lsdamp = new WriteLsdamp(list);
    if(command =="*LSMEN") lsmen = new WriteLsmen(list);
    if(command =="*ALIST") alist = new WriteAlist(list);
}

QString Section01::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting){
    QString txt;
    txt += u8"\\begin{chapter01} \n";
    txt += u8"\\newpage{} \n" ;
    txt += u8"\\chapter{" + chapter + u8"} \n";
    if (outline){
        txt += outline->context(maker, setting);
    }
    if (plan){
        txt += plan->context(maker, setting);
    }
    if (section){
        txt += section->context(maker, setting);
    }
    if (lsgir){
        txt += lsgir->context(maker, setting);
    }
    if (lscol){
        txt += lscol->context(maker, setting);
    }
    if (lswal){
        txt += lswal->context(maker, setting);
    }
    if (lsswa){
        txt += lsswa->context(maker, setting);
    }
    if (lsfwa){
        txt += lsfwa->context(maker, setting);
    }
    if (lsbra){
        txt += lsbra->context(maker, setting);
    }
    if (lsbeam){
        txt += lsbeam->context(maker, setting);
    }
    if (lsslab){
        txt += lsslab->context(maker, setting);
    }
    if (lsopen){
        txt += lsopen->context(maker, setting);
    }
    if (lsbs){
        txt += lsbs->context(maker, setting);
    }
    if (lsbsrc){
        txt += lsbsrc->context(maker, setting);
    }
    if (lsdamp){
        txt += lsdamp->context(maker, setting);
    }
    if (lsmen){
        txt += lsmen->context(maker, setting);
    }
    if (alist){
        txt += alist->context(maker, setting);
    }
    txt += u8"\\end{chapter01} \n";
    return txt;
}
} // namespace post3dapp
