#ifndef SECTION07_H
#define SECTION07_H
#include "abstractwrite.h"
namespace post3dapp{

class Section07
{
public:
    Section07();
    static bool Contain(QString command){
        if (command == "*POLICYF") return true;
        return false;
    }
    void set(QString, QStringList);
    QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
private:
    QString chapter = u8"基礎・地盤の検討";
    WritePolicyf *policyf = nullptr;
};
} // namespace post3dapp

#endif // SECTION07_H
