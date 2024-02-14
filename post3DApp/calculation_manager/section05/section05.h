#ifndef SECTION05_H
#define SECTION05_H
#include "abstractwrite.h"
namespace post3dapp{

class Section05
{
public:
    Section05();
    static bool Contain(QString command){
        if (command == "*OUTCS") return true;
        if (command == "*VJSTRP") return true;
        if (command == "*VJSTRS") return true;
        if (command == "*VFSTRP") return true;
        if (command == "*VFSTRS") return true;
        if (command == "*HRATIO") return true;
        if (command == "*HJSTRP") return true;
        if (command == "*HJSTRS") return true;
        if (command == "*HFSTRP") return true;
        if (command == "*HFSTRS") return true;
        if (command == "*CREZ") return true;
        if (command == "*REACT") return true;
        if (command == "*VLOADMA") return true;
        if (command == "*STSTIFF") return true;
        if (command == "*FRSTIFF") return true;
        if (command == "*FRDIST") return true;
        if (command == "*QMAP") return true;
        return false;
    }
    void set(QString, QStringList);
    QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
private:
    QString chapter = u8"応力計算結果";
    WriteOutcs *outcs = nullptr;
    WriteVjstrp *vjstrp = nullptr;
    WriteVjstrs *vjstrs = nullptr;
    WriteVfstrp *vfstrp = nullptr;
    WriteVfstrs *vfstrs = nullptr;
    WriteHratio *hratio = nullptr;
    WriteHjstrp *hjstrp = nullptr;
    WriteHjstrs *hjstrs = nullptr;
    WriteHfstrp *hfstrp = nullptr;
    WriteHfstrs *hfstrs = nullptr;
    WriteCrez *crez = nullptr;
    WriteReact *react = nullptr;
    WriteStstiff *ststiff = nullptr;
    WriteFrstiff *frstiff = nullptr;
    WriteFrdist *frdist = nullptr;
    WriteQmap *qmap = nullptr;

};
} // namespace post3dapp

#endif // SECTION05_H
