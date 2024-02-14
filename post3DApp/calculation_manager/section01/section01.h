#ifndef SECTION01_H
#define SECTION01_H
#include "abstractwrite.h"
namespace post3dapp{

class Section01
{
public:
    Section01();
    static bool Contain(QString command){
        if(command =="*OUTLINE") return true;
        if(command =="*PLAN") return true;
        if(command =="*SECTION") return true;
        if(command =="*LSGIR") return true;
        if(command =="*LSCOL") return true;
        if(command =="*LSWAL") return true;
        if(command =="*LSSWA") return true;
        if(command =="*LSFWA") return true;
        if(command =="*LSBRA") return true;
        if(command =="*LSBEAM") return true;
        if(command =="*LSSLAB") return true;
        if(command =="*LSOPEN") return true;
        if(command =="*LSBS") return true;
        if(command =="*LSBSRC") return true;
        if(command =="*LSDAMP") return true;
        if(command =="*LSMEN") return true;
        if(command =="*ALIST") return true;
        return false;
    }
    void set(QString, QStringList);
    QString write();
    QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
private:
    QString chapter = u8"建築物の構造設計概要";
    WriteAlist *alist = nullptr;
    WriteLsbeam *lsbeam = nullptr;
    WriteLsbra *lsbra = nullptr;
    WriteLsbs *lsbs = nullptr;
    WriteLsbsrc *lsbsrc = nullptr;
    WriteLscol *lscol = nullptr;
    WriteLsdamp *lsdamp = nullptr;
    WriteLsfwa *lsfwa = nullptr;
    WriteLsgir *lsgir = nullptr;
    WriteLsmen *lsmen = nullptr;
    WriteLsopen *lsopen = nullptr;
    WriteLsslab *lsslab = nullptr;
    WriteLsswa *lsswa = nullptr;
    WriteLswal *lswal = nullptr;
    WriteOutline *outline = nullptr;
    WritePlan *plan = nullptr;
    WriteSection *section = nullptr;
};
} // namespace post3dapp


#endif // OUTLINE_H
