#ifndef SECTION02_H
#define SECTION02_H
#include "abstractwrite.h"
namespace post3dapp{

class Section02
{
public:
    Section02();
    static bool Contain(QString command){
        if (command == "*CALC") return true;
        if (command == "*STND") return true;
        if (command == "*POLICY") return true;
        if (command == "*STT") return true;
        if (command == "*MATC") return true;
        if (command == "*MATR") return true;
        if (command == "*MATS") return true;
        return false;
    }

    void set(QString, QStringList);
    QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
private:
    QString chapter = u8"設計方針と使用材料";
    WriteCalc *calc = nullptr;
    WriteStnd *stnd = nullptr;
    WritePolicy *policy = nullptr;
    WriteStt *stt = nullptr;
    WriteMatc *matc = nullptr;
    WriteMatr *matr = nullptr;
    WriteMats *mats = nullptr;
};
} // namespace post3dapp

#endif // SECTION02_H
