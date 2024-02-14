#ifndef OUTPUTTEXTPARSER_H
#define OUTPUTTEXTPARSER_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QStringList;

namespace post3dapp{
class UnifiedOutput;

class POST3D_CLASS_DLLSPEC OutputTextParser
{
public:
    OutputTextParser(const UnifiedOutput&);
    ~OutputTextParser();
    QList<QPair<QStringList, QPair<QPair<QString, QString>, QString>>> parse(const QList<QStringList>&);

    /*MAINP*/
    qint32 NUMIB;
    qint32 MXRSCS;
    qint32 NUMCS;
    qint32 NUMMO;
    qint32 NUMUL;
    qint32 MXMPR;
    qint32 MXPRFR;
    qint32 MXPRFL;
    qint32 NUMPR;
    qint32 NUMPL;
    qint32 NUMQC;
    qint32 MXLQC;
    static constexpr int MXCBUF = 50;
    char NAMEMO[MXCBUF][10];
    char NAMEUL[MXCBUF][10];
    char NAMEQN[MXCBUF][10];
    char NAMERS[MXCBUF][10];
    qint32 IRSAM;
    qint32 NUMRS;
    qint32 MXRSST;
    qint32 MXRSNM;
    qint32 MXRSSA;
    qint32 NUMNM;
    qint32 NUMSA;
    qint32 NUMAIX;
    qint32 MXMPGR;
    qint32 NUMMAP;
    char NAMEDY[MXCBUF][10];
    qint32 NUMDY;
    qint32 NUMDPE[MXCBUF];
    qint32 NUMDPX;
    qint32 KSUM;
    qint32 MXADDG;
    qint32 KPLTSEC;
    qint32 KPLTZWM;


    int* NCGRJ;
    int* IGRJ;
    int* NCGRM;
    int* IGRM;
    int* INDGRM;
    int* IYZMEM;
    int* NCGRW;
    int* IGRW;
    int* INDGRW;

private:
    void SetCommon() const;
    int getVersion(const QString&) const;
};
} // namespace post3dapp

#endif // OUTPUTTEXTPARSER_H
