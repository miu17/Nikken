#ifndef SECTION11_H
#define SECTION11_H
#include "abstractwrite.h"
namespace post3dapp{

class Section11
{
public:
    Section11();
    static bool Contain(QString command){
        if (command == "*OPINIONC") return true;
        if (command == "*OPINION") return true;
        return false;
    }
    void set(QString, QStringList);
    QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
private:
    QString chapter = u8"総合所見";
    WriteOpinionc *opinionc = nullptr;
    WriteOpinion *opinion = nullptr;
};
} // namespace post3dapp

#endif // SECTION11_H
