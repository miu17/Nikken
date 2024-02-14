#ifndef FORTRANFUNC_H
#define FORTRANFUNC_H
#include <QtCore>

extern "C" {
struct ifile{
    qint32 if1, if2, if3, if4, if5, if6, if7, if8, if9;
    qint32 if22, if30, if31, if32, if33, if34, if41;
};
extern ifile IFILE;

struct comchr{
    char ica[160], icaold[160], subnam[6][10];
};
extern comchr COMCHR;

struct comint{
    qint32 ierror, numsub;
};
extern comint COMINT;

struct compr{
    qint32 line, lenc, ksect, ksub;
};

struct comdt{
    char dtime[17];
};

struct comerr{
    qint32 icnerr, icnwar, icncau, icnngd;
};

struct nversn{
    qint32 iver;
    qint32 nver;
};
extern nversn NVERSN;

void TESTTEST(double mat1[], double mat2[],int*,int*);

void VADD(double a1[], double a2[],int*);

//floatのQListで保存しているので、doubleの配列に変換したい
//QList<double>はdoubleの配列のレイアウトと同じであることが保証されているか？
//戻り値に使える、new deleteの処理が省略できる、などはQListの優位性。QVectorでも良い。
//中間荷重による応力計算
void SETML2(double posrst[], //inout
            int* IM    ,int* ICL   ,double* CC    ,int* KPOS  ,double* POS   ,
            int NLHMM[] ,int ILHMM[] ,double DLHMM[] ,int LNDLH[] ,int* MXLHMM,
            int IFPMM[] ,double DEBMM[],int INDM[]  ,double EVMEM[] ,int* NUMM  );
//部材のたわみ計算
//曲線のセット
void STCRV(double R[],double F[],int pin[],double theta[],double* Lc);
//たわみの計算
void GTDLT(double* xx,double* delta);
void GTDLT2(double* xx,double R[],double F[],int pin[],
            double* Lc,double Mr[],double Qr[],double* EI,
            double* sa,double* sb,double* wa,double* wb,
            double* delta);

void CHKVER(int* NVER);
void CDIMP1(int* NUMIB, int* MXRSCS, int* NUMCS, int* NUMMO, int* NUMUL,
            int* MXMPR, int* MXPRFR, int* MXPRFL, int* NUMPR, int* NUMPL,
            int* NUMQC, int* MXLQC,
            char NAMEMO[50][10], char NAMEUL[50][10], char NAMEQN[50][10], char NAMERS[50][10], int* IRSAM,
int* NUMRS, int* MXRSST, int* MXRSNM, int* MXRSSA, int* NUMNM, int* NUMSA, int* NUMAIX,
int* MXMPGR, int* NUMMAP,
char NAMEDY[50][10], int* NUMDY, int NUMDPE[50], int* NUMDPX,
int* KSUM, int* MXADDG,
int* KPLTSEC, int* KPLTZWM,
char* CASEDQ, int* NUMDQ);
void SETFR(int NCGRJ[], int IGRJ[], int* MXGRJ,
           int NCGRM[], int IGRM[], int INDGRM[], int IYZMEM[], int* MXGRM,
           int NCGRW[], int IGRW[], int INDGRW[], int* MXGRW,
           int* NUMGR, int* IGG, double* DGG, double* DGR, int* MXNGR, int* NUMGG,
           int* NJFL, int* JNTFL, int* MXJFL,
           int* NJGR, int* JNTGR, int* MXJGR,
           double* XYZ, int* NUMJ,
           int* INDM, double* EVMEM, int* NUMM,
           int* IWW1, int* NUMWW);
void RFINIT1();
void PR000(int*, int*, char[]); // SECTION
void PR021(); // TITLE
void PR401(int* IUNIT,
           int IFL[], int NUMFL[], char NAMEFL[], int* MXNFL, int* NUMFG,
           double OUTS2[], char NMFEFS[], char CRSTDR[], double CEDIR[], int MCASE[],
           int* MXCASE, int* NSET); // HF-DISTRIBUTION
void PR701(int* IUNIT, int IROUTE[], double ROUTE[],
           int NUMFL[], char NAMEFL[], int IFL[], double DFL[], int LSFL[], int ICFL[], double HFL[],
           double FLMASS[], double ROFG[], int LFG[], double DFG[], int* MXNFL, int* NUMFG,
           char* NAMEAC, double DAC[], int* NUMAC);

};


#endif // FORTRANFUNC_H
