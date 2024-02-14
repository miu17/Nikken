#ifndef SECTION08_H
#define SECTION08_H
#include "abstractwrite.h"
namespace post3dapp{

class Section08
{
public:
    Section08();
    static bool Contain(QString command){
        if (command == "*POLICYSTEC") return true;
        if (command == "*MXDFRM") return true;
        if (command == "*STFF") return true;
        if (command == "*ECCF") return true;
        if (command == "*PRESTEC") return true;
        if (command == "*GCKCP") return true;
        if (command == "*STECF") return true;
        if (command == "*CWAREA") return true;
        if (command == "*CGCMP1") return true;
        return false;
    }
    void set(QString, QStringList);
    QString context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting&);
private:
    QString chapter = u8"層間変形角・剛性率・偏心率・壁量";
    WritePolicystec *policystec = nullptr;
    WriteMxdfrm *mxdfrm = nullptr;
    WriteStff *stff = nullptr;
    WriteEccf *eccf = nullptr;
    WritePrestec *prestec = nullptr;
    WriteGckcp *gckcp = nullptr;
    WriteStecf *stecf = nullptr;
    WriteCwarea *cwarea = nullptr;
    WriteCgcmp1 *cgcmp1 = nullptr;
};
} // namespace post3dapp

#endif // SECTION08_H
