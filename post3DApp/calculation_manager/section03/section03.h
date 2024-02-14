#ifndef SECTION03_H
#define SECTION03_H
#include "abstractwrite.h"
namespace post3dapp{

class Section03
{
public:
    Section03();
    static bool Contain(QString command){
        if (command == "*CCS") return true;
        if (command == "*POLICYLD") return true;
        if (command == "*LDL") return true;
        if (command == "*LDF") return true;
        if (command == "*LDM") return true;
        if (command == "*LDJ") return true;
        if (command == "*SNOW") return true;
        if (command == "*WIND") return true;
        if (command == "*OTHER") return true;
        if (command == "*LPLAN") return true;
        if (command == "*SEISLE") return true;
        if (command == "*LCMQP") return true;
        if (command == "*LCMQS") return true;
        if (command == "*JMASSP") return true;
        if (command == "*JMASSS") return true;
        if (command == "*ESTIM") return true;
        return false;
    }
    void set(QString, QStringList);
    QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
private:
    QString chapter = u8"荷重・外力の条件";
    WriteCcs *ccs = nullptr;
    WritePolicyld *policyld = nullptr;
    WriteLdl *ldl = nullptr;
    WriteLdf *ldf = nullptr;
    WriteLdm *ldm = nullptr;
    WriteLdj *ldj = nullptr;
    WriteSnow *snow = nullptr;
    WriteWind *wind = nullptr;
    WriteOther *other = nullptr;
    WriteLplan *lplan = nullptr;
    WriteSeisle *seisle = nullptr;
    WriteLcmqp *lcmqp = nullptr;
    WriteLcmqs *lcmqs = nullptr;
    WriteJmassp *jmassp = nullptr;
    WriteJmasss *jmasss = nullptr;
    WriteEstim *estim = nullptr;
};
} // namespace post3dapp

#endif // SECTION03_H
