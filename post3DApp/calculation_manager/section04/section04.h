#ifndef SECTION04_H
#define SECTION04_H
#include "abstractwrite.h"
namespace post3dapp{


class Section04
{
public:
    Section04();
    static bool Contain(QString command){
        if (command == "*POLICYEC") return true;
        if (command == "*CMODEL") return true;
        if (command == "*EMODEL") return true;
        if (command == "*MODELP") return true;
        if (command == "*MODELS") return true;
        if (command == "*MPHAIP") return true;
        if (command == "*MPHAIS") return true;
        if (command == "*MRLP") return true;
        if (command == "*MRLS") return true;
        if (command == "*MCLP") return true;
        if (command == "*MCLS") return true;
        if (command == "*MBLP") return true;
        if (command == "*MBLS") return true;
        if (command == "*ANACON") return true;
        if (command == "*RFLOOR") return true;
        if (command == "*JSPRING") return true;
        if (command == "*MSPRING") return true;
        if (command == "*MDETAIL") return true;
        if (command == "*AUTOPHAI") return true;
        if (command == "*AUTOLK") return true;
        if (command == "*JNT") return true;
        if (command == "*INDEX") return true;
        if (command == "*FRAME") return true;
        if (command == "*PNLMDL") return true;
        return false;
    }
    void set(QString, QStringList);
    QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
private:
    QString chapter = u8"応力計算基本仮定";
    WritePolicyec *policyec = nullptr;
    WriteCmodel *cmodel = nullptr;
    WriteEmodel *emodel = nullptr;
    WriteModelp *modelp = nullptr;
    WriteModels *models = nullptr;
    WriteMphaip *mphaip = nullptr;
    WriteMphais *mphais = nullptr;
    WriteMrlp *mrlp = nullptr;
    WriteMrls *mrls = nullptr;
    WriteMclp *mclp = nullptr;
    WriteMcls *mcls = nullptr;
    WriteMblp *mblp = nullptr;
    WriteMbls *mbls = nullptr;
    WriteAnacon *anacon = nullptr;
    WriteRfloor *rfloor = nullptr;
    WriteJspring *jspring = nullptr;
    WriteMspring *mspring = nullptr;
    WriteMdetail *mdetail = nullptr;
    WriteAutophai *autophai = nullptr;
    WriteAutolk *autolk = nullptr;
    WriteJnt *jnt = nullptr;
    WriteIndex *index = nullptr;
    WriteFrame *frame = nullptr;
    WritePnlmdl *pnlmdl = nullptr;
};
} // namespace post3dapp


#endif // SECTION04_H
