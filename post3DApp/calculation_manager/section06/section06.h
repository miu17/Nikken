#ifndef SECTION06_H
#define SECTION06_H
#include "abstractwrite.h"
namespace post3dapp{

class Section06
{
public:
    Section06();
    static bool Contain(QString command){
        if(command =="*POLICYCH") return true;
        if(command =="*CHCASE") return true;
        if(command =="*CHVP") return true;
        if(command =="*CHVS") return true;
        if(command =="*CHHP") return true;
        if(command =="*CHHS") return true;
        if(command =="*CHLISTM") return true;
        if(command =="*CHLISTQ") return true;
        if(command =="*CHCOL") return true;
        if(command =="*CHGIR") return true;
        if(command =="*CHWAL") return true;
        if(command =="*CHBRA") return true;
        if(command =="*CHOPEN") return true;
        if(command =="*CHPS2") return true;
        if(command =="*CHPRC2") return true;
        if(command =="*CHPSRC2") return true;
        if(command =="*CHPCFT2") return true;
        if(command =="*CHPS3") return true;
        if(command =="*CHPRC3") return true;
        if(command =="*CHPSRC3") return true;
        if(command =="*CHPCFT3") return true;
        if(command =="*CHSRCBASE") return true;
        if(command =="*CHSBASE") return true;
        if(command =="*CHNEOBASE") return true;
        if(command =="*CHNEWBASE") return true;
        if(command =="*CHBPBASE") return true;
        if(command =="*CHCUTOFF") return true;
        if(command =="*CHBONDL1") return true;
        if(command =="*CHBONDS1") return true;
        if(command =="*CHBONDS2") return true;
        return false;
    }
    void set(QString, QStringList);
    QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
private:
    QString chapter = u8"断面検討";
    WritePolicych *policych = nullptr;
    WriteChcase *chcase = nullptr;
    WriteChvp *chvp = nullptr;
    WriteChvs *chvs = nullptr;
    WriteChhp *chhp = nullptr;
    WriteChhs *chhs = nullptr;
    WriteChlistm *chlistm = nullptr;
    WriteChlistq *chlistq = nullptr;
    WriteChcol *chcol = nullptr;
    WriteChgir *chgir = nullptr;
    WriteChwal *chwal = nullptr;
    WriteChbra *chbra = nullptr;
    WriteChopen *chopen = nullptr;
    WriteChps2 *chps2 = nullptr;
    WriteChprc2 *chprc2 = nullptr;
    WriteChpsrc2  *chpsrc2 = nullptr;
    WriteChpcft2 *chpcft2 = nullptr;
    WriteChps3 *chps3 = nullptr;
    WriteChprc3 *chprc3 = nullptr;
    WriteChpsrc3 *chpsrc3 = nullptr;
    WriteChpcft3 *chpcft3 = nullptr;
    WriteChsrcbase *chsrcbase = nullptr;
    WriteChsbase *chsbase = nullptr;
    WriteChneobase *chneobase = nullptr;
    WriteChnewbase *chnewbase = nullptr;
    WriteChbpbase *chbpbase = nullptr;
    WriteChcutoff *chcutoff = nullptr;
    WriteChbondl1 *chbondl1 = nullptr;
    WriteChbonds1 *chbonds1 = nullptr;
    WriteChbonds2 *chbonds2 = nullptr;
};
} // namespace post3dapp

#endif // SECTION06_H
