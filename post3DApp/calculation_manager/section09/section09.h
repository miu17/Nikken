#ifndef SECTION09_H
#define SECTION09_H
#include "abstractwrite.h"
namespace post3dapp{

class Section09
{
public:
    Section09();
    static bool Contain(QString command){
        if (command == "*POLICYQUDS") return true;
        if (command == "*QUDS") return true;
        if (command == "*SEISLP") return true;
        if (command == "*POLICYPC") return true;
        if (command == "*PMODEL") return true;
        if (command == "*DSPUSH") return true;
        if (command == "*DSGENE") return true;
        if (command == "*DSSTEP") return true;
        if (command == "*DSCURVE") return true;
        if (command == "*DSSTR") return true;
        if (command == "*DSCOND") return true;
        if (command == "*MRNK") return true;
        if (command == "*RNCS") return true;
        if (command == "*RNCRC") return true;
        if (command == "*RNCSRC") return true;
        if (command == "*RNCCFT") return true;
        if (command == "*RNGS") return true;
        if (command == "*RNGRC") return true;
        if (command == "*RNGSRC") return true;
        if (command == "*RNVS") return true;
        if (command == "*RNWRC") return true;
        if (command == "*RNWSRC") return true;
        if (command == "*RNPRC") return true;
        if (command == "*QUN") return true;
        if (command == "*GRBEND") return true;
        if (command == "*GRBONDI") return true;
        if (command == "*GRBONDP") return true;
        if (command == "*YOKOHOGOU") return true;
        if (command == "*DSSH") return true;
        if (command == "*DSMU") return true;
        if (command == "*QUPUSH") return true;
        if (command == "*QUGENE") return true;
        if (command == "*QUSTEP") return true;
        if (command == "*QUCURVE") return true;
        if (command == "*QUCOND") return true;
        if (command == "*QUDETRM") return true;
        if (command == "*QUSTR") return true;
        if (command == "*QUCRC") return true;
        if (command == "*QUCSRC") return true;
        if (command == "*QUGRC") return true;
        if (command == "*QUGSRC") return true;
        if (command == "*QUWRC") return true;
        if (command == "*QUWSRC") return true;
        if (command == "*QUPS") return true;
        if (command == "*QUPRC") return true;
        if (command == "*QUPSRC") return true;
        if (command == "*QUPCFT") return true;
        if (command == "*QUSH") return true;
        if (command == "*DSQUCURVE") return true;
        if (command == "*QUNEED") return true;
        if (command == "*QUNEEDF") return true;
        if (command == "*CGCMPM") return true;
        if (command == "*CGCMPF") return true;
        if (command == "*MBCPR") return true;
        if (command == "*QBCPR") return true;
        if (command == "*MULT") return true;
        return false;
    }
    void set(QString, QStringList);
    QString context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
private:
    QString chapter = u8"保有水平耐力";
    WritePolicyquds *policyquds = nullptr;
    WriteQuds *quds = nullptr;
    WriteSeislp *seislp = nullptr;
    WritePolicypc *policypc = nullptr;
    WritePmodel *pmodel = nullptr;
    WriteDspush *dspush = nullptr;
    WriteDsgene *dsgene = nullptr;
    WriteDsstep *dsstep = nullptr;
    WriteDscurve *dscurve = nullptr;
    WriteDsstr *dsstr = nullptr;
    WriteDscond *dscond = nullptr;
    WriteMrnk *mrnk = nullptr;
    WriteRncs *rncs = nullptr;
    WriteRncrc *rncrc = nullptr;
    WriteRncsrc *rncsrc = nullptr;
    WriteRnccft *rnccft = nullptr;
    WriteRngs *rngs = nullptr;
    WriteRngrc *rngrc = nullptr;
    WriteRngsrc *rngsrc = nullptr;
    WriteRnvs *rnvs = nullptr;
    WriteRnwrc *rnwrc = nullptr;
    WriteRnwsrc *rnwsrc = nullptr;
    WriteRnprc *rnprc = nullptr;
    WriteQun *qun = nullptr;
    WriteGrbend *grbend = nullptr;
    WriteGrbondi *grbondi = nullptr;
    WriteGrbondp *grbondp = nullptr;
    WriteYokohogou *yokohogou = nullptr;
    WriteDssh *dssh = nullptr;
    WriteDsmu *dsmu = nullptr;
    WriteQupush *qupush = nullptr;
    WriteQugene *qugene = nullptr;
    WriteQustep *qustep = nullptr;
    WriteQucurve *qucurve = nullptr;
    WriteQucond *qucond = nullptr;
    WriteQudetrm *qudetrm = nullptr;
    WriteQustr *qustr = nullptr;
    WriteQucrc *qucrc = nullptr;
    WriteQucsrc *qucsrc = nullptr;
    WriteQugrc *qugrc = nullptr;
    WriteQugsrc *qugsrc = nullptr;
    WriteQuwrc *quwrc = nullptr;
    WriteQuwsrc *quwsrc = nullptr;
    WriteQups *qups = nullptr;
    WriteQuprc *quprc = nullptr;
    WriteQupsrc *qupsrc = nullptr;
    WriteQupcft *qupcft = nullptr;
    WriteQush *qush = nullptr;
    WriteDsqucurve *dsqucurve = nullptr;
    WriteQuneed *quneed = nullptr;
    WriteQuneedf *quneedf = nullptr;
    WriteCgcmpm *cgcmpm = nullptr;
    WriteCgcmpf *cgcmpf = nullptr;
    WriteMbcpr *mbcpr = nullptr;
    WriteQbcpr *qbcpr = nullptr;
    WriteMult *mult = nullptr;
};
} // namespace post3dapp


#endif // SECTION09_H
