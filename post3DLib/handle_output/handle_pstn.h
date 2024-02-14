#ifndef HANDLE_PSTN_H
#define HANDLE_PSTN_H

//#include <QWidget>
//#include <QProcess>
//#include <QFile>
//#include <QDir>
//#include <QHash>
//#include <QDebug>
//#include <QDateTime>
//#include <QFileDialog>
//#include <QProgressDialog>

//#include "define_unifieddata.h"
//#include "pstn_optional_data.h"

//#ifdef POST3D_LIB
//#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
//#else
//#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
//#endif
namespace post3dapp{


//struct DataBLD3D_CMDL {
//    static const qint32 mxnumb = 15000, iffix = 99999, nroute = 4, nrofg = 4, nlfg = 2, ndgg = 4,
//                        ndfl = 6,
//                        mxjdof = 9, ndim = 3, njgk = 2, ndrl = 2, ndd4 = 5, ndas = 9, ndac = 12, ndar = 7,
//                        ndrr = 2, nils1 = 20, nils2 = 26, ndls1 = 44, ndls2 = 18, ndls3 = 35, nicj = 3,
//                        nyz = 2, nftbc = 3, nlsu = 3, mxmn = 2, nimm1 = 14, ndmm1 = 26, mxpin = 5, nxy = 2, nij = 2,
//                        ndmme = 16, nindmes = 3, nibl = 4, ndbl = 2, nisw = 9, ndsw = 5, niswor = 12,
//                        niso = 3, ndso = 4, niww1 = 13, ndww1 = 14, nxz = 2, niwz1 = 4, ndwz1 = 5,
//                        niws1 = 8, ndebws = 4, nizz = 2, ndzz = 2, nzdmax = 20, ndzze = 3, niap = 2,
//                        ndap = 7, nisp = 5, ndsp = 7, nipp = 4, niab = 2, ndab = 7, nibj = 4, ndbj = 3,
//                        nibz = 2, ndbz = 6, nibn = 5, ndbn = 9, nibq = 5, ndbq = 12, niby = 5, ndby = 12,
//                        nrbyc = 7, ibymax = 4, nibs = 5, ndbs = 13, nibt = 5, ndbt = 9, nibw = 6, ndbw = 14,
//                        nimt = 9, nrbbc = 8, nrect = 4, nisdp = 5, ndsdp = 55, niei = 15, ndei = 20,
//                        npnlj1 = 5;
//    qint32 msize, nbande, mxnfl, numfg, mxngr, numgg, numj, mxjfl, mxjgr, mxjm,
//           numrld, numrl, maxsl, mjslvd, mjslv, mxid4, numd4d, numd4, numasd, numas,
//           numacd, numac, numard, numar, numrrd, numrr, numlsd, numls, mxjs, mxss,
//           mxjc, mxcc, mxjr, mxrr, nummd, numm, numbl, numswd, numsw, mxso, numsod, numso,
//           mxww, numwwd, numww, mxwz, numwzd, numwz, mxmw, nummwd, nummw, mxmz, nummzd,
//           nummz, numzzd, numzz, numwsd, numws, numapd, numap, numspd, numsp, numpd, nump,
//           numabd, numab, numbjd, numbj, numbzd, numbz, numbnd, numbn, numbqd, numbq,
//           lbymax, numbyd, numby, numbsd, numbs, numbtd, numbt, numbwd, numbw, nummtd,
//           nummt, numbd, numb, numcol, mxgrj, mxgrm, mxgrw, iexmcb, nwk, nwke, numsdp,
//           mxmems, numemsd, numems, numeid, numei, jsize;
//    //必要なデータだけ抽出
//    QString title;
//    QVector<qint32> numfl;
//    QVector<QVector<qint32> > kfl;
//    QVector<QVector<qint32> > ifl;
//    QVector<QVector<qint32> > lfg;
//    QVector<QStringList> namefl;
//    QVector<qint32> numgr;
//    QVector<QVector<double> >ahfl;
//    QVector<double>dfg;
//    QVector<QStringList> namegr;
//    QVector<QVector<double> >dgr;
//    QVector<QVector<qint32> >lgr;
//    QVector<qint32> igg;
//    QVector<QVector<double> >dgg;
//    QVector<qint32> jntnm;
//    QStringList namej;
//    QVector<QVector<qint32> >indj;
//    QVector<QVector<float> > xyz;
//    QVector<qint32> njfl;
//    QVector<QVector<QVector<qint32> > > jntfl;
//    QVector<qint32> njgr;
//    QVector<QVector<QVector<qint32> > > jntgr;
//    QVector<qint32> jnti;
//    QVector<QVector<qint32> > jnto;
//    QVector<QVector<qint32> > jntom;
//    QVector<qint32> numsl;
//    QVector<QVector<float> > cosl;
////    QVector<QVector<float> > das;
////    QVector<QVector<float> > dac;
//    QStringList namels;
//    QVector<QVector<qint32> > ils1;
//    QVector<QVector<QVector<qint32> > > ils2;
//    QVector<QVector<QVector<float> > > dls1;
//    QVector<QVector<qint32> > imm1;
//    QVector<QVector<qint32> > indm;
//    QVector<QVector<qint32> > ifpmm;
//    QVector<QVector<float> > dmm1;
//    QVector<QVector<QVector<float> > > evmem;
//    QStringList namebl;
//    QVector<QVector<qint32> > mbl;
//    QStringList namesw;
//    QVector<QVector<qint32> > isw;
//    QVector<QVector<float> > dsw;
//    QStringList nameso;
//    QVector<QVector<QVector<qint32> > > iso;
//    QVector<QVector<QVector<float> > > dso;
//    QVector<qint32> nso;
//    QVector<QVector<qint32> > iww1;
//    QVector<QVector<qint32> > iww2;
//    QVector<QVector<qint32> > iww3;
//    QVector<QVector<float> > dww1;
//    QVector<QVector<qint32> > iwz1;
//    QVector<QVector<qint32> > iwz2;
//    QVector<QVector<QVector<float> > > evmemw;
//    QVector<QVector<qint32> > iws1;
//    QVector<QVector<float> > debws;
//    QVector<QVector<QVector<float> > > evws;

//    QVector<QVector<qint32> > ibj;
//    QVector<QVector<qint32> > ibn;
//    QVector<QVector<qint32> > ibq;
//    QVector<QVector<qint32> > iby;
//    QVector<QVector<qint32> > ibs;
//    QVector<QVector<qint32> > ibt;
//    QStringList cbz;
//    QVector<QVector<float> > dab;
//    QVector<QVector<float> > dbj;
//    void clear()
//    {
//        title.clear();
//        lfg.clear();
//        ifl.clear();
//        kfl.clear();
//        namefl.clear();
//        ahfl.clear();
//        dfg.clear();
//        numgr.clear();
//        namegr.clear();
//        dgr.clear();
//        lgr.clear();
//        igg.clear();
//        dgg.clear();
//        jntnm.clear();
//        namej.clear();
//        indj.clear();
//        xyz.clear();
//        njfl.clear();
//        jntfl.clear();
//        njgr.clear();
//        jntgr.clear();
//        jnti.clear();
//        jnto.clear();
//        jntom.clear();
//        numsl.clear();
//        cosl.clear();//das.clear();dac.clear();
//        namels.clear();
//        ils1.clear();
//        ils2.clear();
//        dls1.clear();
//        imm1.clear();
//        indm.clear();
//        ifpmm.clear();
//        dmm1.clear();
//        evmem.clear();
//        namebl.clear();
//        mbl.clear();
//        namesw.clear();
//        isw.clear();
//        dsw.clear();
//        nameso.clear();
//        iso.clear();
//        dso.clear();
//        nso.clear();
//        iww1.clear();
//        iww2.clear();
//        iww3.clear();
//        dww1.clear();
//        iwz1.clear();
//        iwz2.clear();
//        evmemw.clear();
//        iws1.clear();
//        debws.clear();
//        evws.clear();
//        ibj.clear();
//        ibn.clear();
//        ibq.clear();
//        iby.clear();
//        ibt.clear();
//        cbz.clear();
//        dab.clear();
//        dbj.clear();
//    }
//};
//struct DataBLD3D_LOAD {
//    static const qint32 nfrcfl = 3, ndlemm = 5, ncmqmm = 14, nilemm = 5, mxlemm = 50000;
//    qint32 numc, lxlemm, numcl, numcld, numse, numsed, mxwifl, numwi, numwid;

//    QVector<qint32>indle;
//    QStringList namel;
//    QStringList titc;
//    QVector<qint32>indse;
//    QVector<qint32>indwi;
//    QVector<float>dirse;
//    QVector<QVector<QVector<QVector<float> > > >frcfl;
//    QVector<QVector<qint32> >ibc;
//    QVector<QVector<QVector<float> > >fincj;
//    QVector<qint32>nlemm;
//    QVector<QVector<QVector<qint32> > >ilemm;
//    QVector<QVector<QVector<float> > >cmqmm;
//    //以降省略
//    void clear()
//    {
//        indle.clear();
//        namel.clear();
//        titc.clear();
//        indse.clear();
//        indwi.clear();
//        dirse.clear();
//        frcfl.clear();
//        ibc.clear();
//        fincj.clear();
//        nlemm.clear();
//        ilemm.clear();
//        cmqmm.clear();
//    }

//};
//struct DataBLD3D_RST {
//    static const qint32 mxjdof = 9, mxjdf6 = 6, nxy = 2, nyz = 2, nxz = 2,
//                        nij = 2, ndim = 3, nrect = 4, nstp2 = 2, mxmtyp = 6, lmtyp = 5, mxpin = 5,
//                        ndeb = 30, nrst = 8, nrstmm = 8, nipspm = 3, nphai = 6, nengmm = 3, nductm = 7,
//                        msdd = 3, nimsc = 1, nrmsc = 9, njmsv = 4, ndmsv = 9, mrdd = 2, nimrc = 1,
//                        nrmrc = 7, njmrv = 2, ndmrv = 4, nibrc = 1, nrbrc = 5, njbrv = 2, ndbrv = 7,
//                        lmtypw = 4, ndebww = 14, nelbww = 3, nrstww = 4, nipspw = 3, msddw = 2, nimscw = 1,
//                        nrmscw = 8, njmsvw = 4, ndmsvw = 9, nimrcm = 1, nrmrcm = 6, njmrvm = 6,
//                        ndmrvw = 64, ndebws = 4,
//                        nrstws = 3, ngaus = 9, nadfws = 8, ndebpp = 3, npdim = 3, nppdd = 2, nippc = 1,
//                        nrppc = 8, njppv = 2, ndppv = 4, nibj = 4, ndbj = 3, nibn = 5, ndbn = 9, nibq = 5,
//                        ndbq = 12, niby = 5, ndby = 12, nibs = 5, ndbs = 13, nrstbs = 1,
//                        nibt = 5, ndbt = 9, nibw = 6, ndbw = 14, nibbc = 1, nrbbc = 8, njbbv = 2, ndbbv = 4,
//                        nrstby = 2, nbydd = 2, nibyc = 1, nrbyc = 7, njbyv = 2, ndbyv = 4, ndsdpv = 10;
//    qint32 iastep, iend;
//    QVector<QVector<float> > ujnt;
//    QVector<QVector<float> > fintj;
//    QVector<QVector<float> > react;
//    QVector<QVector<float> > rstmm;
//    QVector<QVector<QVector<float> > > phaiyz;
//    QVector<QVector<qint32> > ipspmm;
//    QVector<QVector<double> > ductmm;
//    QVector<QVector<qint32> > kpspmm;
//    QVector<QVector<QVector<float> > > rstww;
//    QVector<QVector<qint32> > ipspww;
//    QVector<QVector<float> > fintws;
//    QVector<QVector<QVector<float> > > rstws;
//    QVector<QVector<float> > rstpp;
//    QVector<QVector<qint32> > ipsppp;
//    QVector<float> rstbb;
//    QVector<qint32> ipspbb;
//    QVector<QVector<float> > rstby;
//    QVector<qint32> ipspby;
//    QVector<QVector<float> > rstbs;
//    qint32 kcasol;
//    QVector<qint32> icase;
//    QVector<double> ccase;
//    QVector<QVector<float> > dsdpv;
//    void clear()
//    {
//        ujnt.clear();
//        fintj.clear();
//        react.clear();
//        rstmm.clear();
//        phaiyz.clear();
//        ipspmm.clear();
//        ductmm.clear();
//        kpspmm.clear();
//        rstww.clear();
//        ipspww.clear();
//        fintws.clear();
//        rstws.clear();
//        rstpp.clear();
//        ipsppp.clear();
//        rstbb.clear();
//        ipspbb.clear();
//        rstby.clear();
//        ipspby.clear();
//        rstbs.clear();
//        icase.clear();
//        ccase.clear();
//        dsdpv.clear();
//    }
//};
//struct DataBLD3D_PREA{
//    static const qint32 nimm1=14,nimm2=5,nimm3=8,nimm4=4,
//    ndmm1=26,ndmm2=16,ndmm3=17,ndmm4=8,ndmm5=14,ndmm6=10,mxpin=5,
//    ndeb=30,lmtyp=5,nzdof=8,nzsiz=36,nimsc=1,nrmsc=9,imsmax=100,nimrc=1,nrmrc=7,imrmax=4,
//    nihsc=1,nrhsc=9,ihlmax=10,ihsmax=100,nihrc=1,nrhrc=7,ihrmax=4,nibrc=1,nrbrc=5,niaxc=2,
//    ndaxc=25,nrstmm=8,ncmqmm=14,msdd=3,nisw=9,ndsw=5,niww1=13,niww4=8,ndww1=14,ndww2=18,
//    ndww3=16,lmtypw=4,ndebww=14,nrect=4,nimscw=1,nrmscw=8,imsmxw=20,nimrcw=1,nrmrcw=6,
//    nrstww=4,neww=3,niwz1=4,ndwz1=5,ndwz2=3,nizz=2,ndzz=2,nzdmax=20,niws1=8,ndws1=1,ndebws=4,
//    nizs=2,ndzs=5,niap=2,ndap=7,nisp=5,ndsp=7,nipp=4,ndebpp=3,
//    nrppc=8,npdim=3,nrstpp=1,niab=2,nibj=4,nibn=5,nibq=5,niby=5,nibs=5,nibt=5,nibw=6,nimt=9,
//    nibbc=1,nrbbc=8,nibyc=1,nrbyc=7,
//    nrstbb=1,nrstby=2,niban=5,nrban=50,nicrf1=11,nicrf2=11,nicrf3=3;
//    qint32 numwp,numwpd,numez,numezd,numms,nummsd,numbr,numbrd,numcb,
//    lmsmax,lmrmax,lmsmxw,mxzs,numzs,numzsd,numis,numqs,numaxc,numaxcd,
//    numhg,numhgd,lhlmax,lhsmax,lhrmax,numhs,numhsd;
//    QVector<qint32>nmtyp;

//    QVector<QVector<qint32>> mtyp;//lmtype,nummd
//    QVector<QVector<float>> debmm;//ndeb,nummd
//    QVector<QVector<float>> debww;//ndebww,numwwd
//    QVector<QVector<float>> dmm2;//ndmm2,nummd
//    QVector<QVector<float>> rstmmi;//nrstmm,nummd
//    QVector<QVector<qint32>> imm2;//nimm2,nummd
//    //以降省略
//    void clear(){mtyp.clear();debmm.clear();dmm2.clear();rstmmi.clear();imm2.clear();
//                debww.clear();}

//};

//struct DataBLD3D_MODE {
//    int nummode;//正の整数
//    int numj;//正の整数
//    QVector<qreal> naturalPeriod;//size=mode数
//    QVector<qreal> totalMass;//X Y Z rz ry rz
//    QVector<QVector<qreal> > effectiveMassRatio;//beta*beta/totalmass
//    QVector<QVector<XYZ> > mode; //mode[nummode][jno]
//    QVector<QVector<qreal> > beta;//X Y Z rx ry rz * mode数
//    void clear()
//    {
//        naturalPeriod.clear();
//        totalMass.clear();
//        effectiveMassRatio.clear();
//        mode.clear();
//        beta.clear();
//    }
//};
//struct DataBLD3D_CHCK {
//    qint32 numch;
//    QStringList namech;
//    //[numch][nummd][nicj]
//    QVector<QVector<QVector<float> > > line_allowanceRateBending;//曲げ余裕率[numch][icj]
//    QVector<QVector<QVector<float> > >
//    line_verificationRatioShear;//せん断余裕率（SRCの場合はRC部分）[numch][icj]
//    QVector<QVector<QVector<float> > >
//    line_verificationRatioShear2;//せん断余裕率(SRC部材のS部分,RC部材の付着)[numch][icj]
//    QVector<QVector<QVector<float> > > line_verificationRatioBending;//検定比[numch][icj]
//    QVector<QVector<QVector<float> > > plane_allowanceRateBending;//曲げ余裕率[numch][icj]
//    QVector<QVector<QVector<float> > >
//    plane_verificationRatioShear;//せん断余裕率（SRCの場合はRC部分）[numch][icj]
//    QVector<QVector<QVector<float> > >
//    plane_verificationRatioShear2;//せん断余裕率(SRC部材のS部分,RC部材の付着)[numch][icj]
//    QVector<QVector<QVector<float> > > plane_verificationRatioBending;//検定比[numch][icj]
//    void clear()
//    {
//        line_allowanceRateBending.clear();
//        line_verificationRatioShear.clear();
//        line_verificationRatioShear2.clear();
//        line_verificationRatioBending.clear();
//        plane_allowanceRateBending.clear();
//        plane_verificationRatioShear.clear();
//        plane_verificationRatioShear2.clear();
//        plane_verificationRatioBending.clear();
//    }
//};

//class POST3D_CLASS_DLLSPEC handlePstn : public QObject
//{
//    Q_OBJECT

//public:

//    handlePstn(QObject *parent = 0): QObject(parent)
//    {
//        ver1_17 = QDateTime(QDate(2018, 1, 10)); //building3D ver 1.17のリリース日

//    }

//    int readBLD3D_CMDL();
//    int readRST();
////    extern int readOneRst(QString rstname);
//    int readBLD3D_LOAD();
//    int readBLD3D_CHCK();
//    int readBLD3D_MODE();
//    int readBLD3D_PREA();
//    //描画に不要な配列は読み飛ばしてるので必要に応じて定義
//    QString getFileName()
//    {
//        return QFileInfo(filename).fileName();
//    }

//    QStringList getStressCase()const
//    {
//        return stresscase;
//    }

////UnifiedOutput::getInstanceからアクセス
////    ChckData chck;
////    ModeData mode;
//    bool successToReadModelFile;
//    bool successToReadSomeRstFile;
//    bool successToReadLoadFile;
//    bool successToReadChckFile;
//    bool successToReadPreaFile;
//    bool existModeFile;

//private:
//    DataBLD3D_CMDL model;
//    DataBLD3D_LOAD load;
//    QHash<QString, QList<DataBLD3D_RST> > rst;
//    QHash<QString, DataBLD3D_PREA> prea;
//    DataBLD3D_MODE mode;
//    DataBLD3D_CHCK chck;
//    QList<OpeningSet *> openingList;
//    void sleep(unsigned long);
//    //現状未使用。バージョンで処理を分ける場合など想定
//    QDateTime ver1_17;
//    QDateTime timestamp;

//    bool toDecompressPstn;
//    bool readAllSteps;
//    QDir filepath;
//    QDir tmpdirpath;
//    QString filename;
//    QString apppath;
//    QString tmpdirname;
//    QStringList stresscase;
//    QStringList modelcase;

//    void makeUnifiedData();
//public slots:
//    int slotFileOpen();


//};
} // namespace post3dapp

#endif // HANDLE_PSTN_H
