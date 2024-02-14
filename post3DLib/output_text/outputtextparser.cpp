#include "outputtextparser.h"

#include "abstracttext.h"
#include "fixed_data.h"
#include "fortranFunc.h"
#include "unified_output.h"
#include "outputtext_reader.h"

namespace post3dapp{

OutputTextParser::OutputTextParser(const UnifiedOutput& pstn)
{
//C.... VERSION ..........................................................
    CHKVER(&NVERSN.nver);
    // GETARG()

// C.... FILE ALLOCATION ..................................................
    SetCommon(); // MAINP1
    // ERRINI('TCHK');

// C.....POS1..............................................................
    auto quds = pstn.getNbldQuDsData();

    /*
    CDIMP1(&NUMIB, &MXRSCS, &NUMCS, &NUMMO, &NUMUL,
           &MXMPR, &MXPRFR, &MXPRFL, &NUMPR, &NUMPL,
           &NUMQC, &MXLQC,
           NAMEMO, NAMEUL, NAMEQN, NAMERS, &IRSAM,
           &NUMRS, &MXRSST, &MXRSNM, &MXRSSA, &NUMNM, &NUMSA, &NUMAIX,
           &MXMPGR, &NUMMAP,
           NAMEDY, &NUMDY, NUMDPE, &NUMDPX,
           &KSUM, &MXADDG,
           &KPLTSEC, &KPLTZWM,
           quds.casedq.data()->first().toUtf8().data(), &quds.numdq);
    */
// C.....PREM..............................................................
    NVERSN.iver = getVersion(pstn.getCmdlData().icard);    // FROPEN()
    // READ BLD3D_CMDL

// C.....PRES..............................................................
    // READ BLD3D_LOAD

// C.....PREA..............................................................
    // READ NAMEMO[]

// C.....NMINT.............................................................
    // OPEN BLD3D_NMIN

// C.....STEC..............................................................
    // READ BLD3D_STEC

// C.....QDCV..............................................................
    // READ NAMEUL[]

// C.....CHECK.............................................................
    // READ BLD3D_CHCK

// C.....SDAMP.............................................................

// C------------------- DYNAMIC ADDRESSING --------------------------------

// CALL NAINP1 以下NAINP1

// C.... READ GENERAL FILE ( BLD3D_GENE ) .................................
// C.... READ PREM FILE ...................................................
// C..... READ SDMP FILE ..................................................
// C..... READ PRES FILE ..................................................
// C.... READ PREA FILE ...................................................
// C.... READ NMINT FILE ..................................................
// C.... READ STEC FILE ...................................................
// C.... READ QDCV FILE ...................................................
// C.... READ QUDS FILE ...................................................
// C.... READ CHECK FILE ..................................................
// C.... READ SOLD FILE ...................................................
// C...... READ INPUT DATA ................................................
    // CALL RDATP1
// C---- PLOT -------------------------------------------------------------
    // CALL PLOTP1
// C----- PLOT SECTION ----------------------------------------------------
    // CALL SECPLT
// C----- PLOT Z-WALL MEMBER SECTION --------------------------------------
    // CALL ZWSPLT
// C----- PLOT ALOWABLE N-MY-MZ INTERACTION CURVE -------------------------
    // CALL PLOTNM2
// C----- PLOT Q-D CURVE --------------------------------------------------
    // CALL PLOTQD
// C----- PLOT ULTIMATE N-MY-MZ INTERACTION CURVE -------------------------
    // CALL SETNMU
    // CALL REVNM
    // CALL PLOTNMU
    // CALL LAYOU3(-1)
// C---- PRINT -------------------------------------------------------------
    RFINIT1();
    auto cmdl = pstn.getCmdlData();
    NCGRJ = new int[cmdl.mxngr * cmdl.numgg];
    IGRJ = new int[cmdl.mxgrj * cmdl.mxngr * cmdl.numgg];
    NCGRM = new int[cmdl.mxngr * cmdl.numgg];
    IGRM = new int[cmdl.mxgrm * cmdl.mxngr * cmdl.numgg];
    INDGRM = new int[cmdl.nij * cmdl.mxgrm * cmdl.mxngr * cmdl.numgg];
    IYZMEM = new int[cmdl.mxgrm * cmdl.mxngr * cmdl.numgg];
    NCGRW = new int[cmdl.mxngr * cmdl.numgg];
    IGRW = new int[cmdl.mxgrw * cmdl.mxngr * cmdl.numgg];
    INDGRW = new int[cmdl.nrect * cmdl.mxgrw * cmdl.mxngr * cmdl.numgg];

    /*
    SETFR(NCGRJ, IGRJ, &cmdl.mxgrj,
          NCGRM, IGRM, INDGRM, IYZMEM, &cmdl.mxgrm,
          NCGRW, IGRW, INDGRW, &cmdl.mxgrw,
          cmdl.numgr.data(), cmdl.igg.data(), cmdl.dgg.data()->data(), cmdl.dgr.data()->data(), &cmdl.mxngr, &cmdl.numgg,
          cmdl.njfl.data(), cmdl.jntfl.data()->data()->data(), &cmdl.mxjfl,
          cmdl.njgr.data(), cmdl.jntgr.data()->data()->data(), &cmdl.mxjgr,
          cmdl.xyz.data()->data(), &cmdl.numj,
          cmdl.indm.data()->data(), cmdl.evmem.data()->data()->data(), &cmdl.numm,
          cmdl.iww1.data()->data(), &cmdl.numww);
    */

}

OutputTextParser::~OutputTextParser()
{
    delete[] NCGRJ;
    delete[] IGRJ;
    delete[] NCGRM;
    delete[] IGRM;
    delete[] INDGRM;
    delete[] IYZMEM;
    delete[] NCGRW;
    delete[] IGRW;
    delete[] INDGRW;
}

void OutputTextParser::SetCommon() const
{
    IFILE.if1 = 1;
    IFILE.if2 = 2;
    IFILE.if3 = 3;
    IFILE.if4 = 4;
    IFILE.if5 = 5;
    IFILE.if6 = 6;
    IFILE.if7 = 7;
    IFILE.if8 = 8;
    IFILE.if9 = 9;
    IFILE.if22 = 22;
    IFILE.if30 = 30;
    IFILE.if31 = 31;
    IFILE.if32 = 32;
    IFILE.if33 = 33;
    IFILE.if34 = 34;
    IFILE.if41 = 41;

    COMINT.ierror = 0;
    COMINT.numsub = 1;

    strcpy(COMCHR.subnam[1], "MAINP1");
}

int OutputTextParser::getVersion(const QString& icard) const
{
    QString ver = icard.mid(27, 3).replace('.', ""); //VER-2.6.0の2.6部分だけ
    bool ok;
    return ver.toInt(&ok);
}

QList<QPair<QStringList, QPair<QPair<QString, QString>, QString>>> OutputTextParser::parse(const QList<QStringList>& parseCard)
{
    QList<QPair<QStringList, QPair<QPair<QString, QString>, QString>>> ret;
    QString errorMessage;
    OutputTextStatus status;
    /*
    Q_FOREACH(auto list, parseCard){
        auto condition = OutputTextReader::MatchLineCondition(list, status, &errorMessage);
        auto aText = OutputTextReader::Create();
         auto second = AbstractText::CreateAndWrite(*UnifiedFixedData::getInstance(), list);
         ret.append(QPair<QStringList, QPair<QPair<QString, QString>, QString>>(list, second));
    }
    */
    return  ret;
}
} // namespace post3dapp
