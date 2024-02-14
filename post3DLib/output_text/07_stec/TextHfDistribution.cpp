#include "abstracttext.h"
#include "fortranFunc.h"
namespace post3dapp{
/*
TextHfDistribution::TextHfDistribution(const QStringList& list)
    :AbstractText(list){}

const QString TextHfDistribution::Command = u8"*HfDistribution";
const QString TextHfDistribution::Japanese = u8"層せん断力分担率の出力";

QPair<QPair<QString, QString>, QString> TextHfDistribution::write(const FixedData& fixedData) const{
    //PR041

    QString second;
    second.append(u8"+-------------------------------------------------------------------------------------------------+").append("\n");
    second.append(u8"|                CASE : XXXXXXXXXX      FILE : XXXXXXXXXX      FORCE ANGLE = XXXX.XX              |").append("\n");
    second.append(u8"+--------+--------------------------------------------------------+-------------------------------+").append("\n");
    second.append(u8"| FLOOR  |                    SHEAR FORCE (KN)                    |            RATIO (%)          |").append("\n");
    second.append(u8"|        |     TOTAL   COLUMN     WALL    BRACE   DAMPER    OTHER |  COL.  WALL  BRA. DAMP.  OTH. |").append("\n");
    second.append(u8"+--------+--------------------------------------------------------+-------------------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX |                                                        |                               |").append("\n");
        second.append(u8"|        |  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X | XXX.X XXX.X XXX.X XXX.X XXX.X |").append("\n");
        second.append(u8"+--------+--------------------------------------------------------+-------------------------------+").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{Command, Japanese}, second};
}
*/

void TextHfDistribution::CallFortran(const UnifiedOutput& pstn) const
{
    /*
    auto cmdl = pstn.getCmdlData();
    qDebug() << "icard" << cmdl.icard;
    int* IUNIT = &cmdl.iunit;
    int* IFL = cmdl.ifl.data()->data();
    int* NUMFL = cmdl.numfl.data();
    char* NAMEFL = cmdl.namefl.data()->first().toUtf8().data();
    int* MXNFL = &cmdl.mxnfl;
    int* NUMFG = &cmdl.numfg;

    auto stec = pstn.getStecData();
    double* OUTS2 = stec.outs2.data()->data()->data()->data()->data();
    char* NMFEFS = stec.nmfefs.data()->first().toUtf8().data();
    char* CRSTDR = stec.crstdr.data()->first().toUtf8().data();
    double* CEDIR = stec.cedir.data()->data();
    int* MCASE = stec.mcase.data();
    int* MXCASE = &stec.mxcase;
    int* NSET = &stec.nset;


    PR401(IUNIT, IFL, NUMFL, NAMEFL, MXNFL, NUMFG, OUTS2, NMFEFS,
          CRSTDR, CEDIR, MCASE, MXCASE, NSET);
    */
};

} // namespace post3dapp
