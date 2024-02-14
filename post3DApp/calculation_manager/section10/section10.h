#ifndef SECTION10_H
#define SECTION10_H
#include "abstractwrite.h"
namespace post3dapp{

class Section10
{
public:
    Section10();
    static bool Contain(QString command){
        if (command == "*PROGRAME") return true;
        if (command == "*MESSAGE") return true;
        return false;
    }
    void set(QString, QStringList);
    QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
private:
    QString chapter = u8"プログラムの運用状況";
    WritePrograme *programe = nullptr;
    WriteMessage *message = nullptr;
};
} // namespace post3dapp

#endif // SECTION10_H
