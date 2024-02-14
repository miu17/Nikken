﻿#ifndef DEFINE_NPSTN_H
#define DEFINE_NPSTN_H

#include "define_unifieddata.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
struct XYZ;

struct DataNBLD3D_CMDL {
    //nainpm.f
    static const qint32 mxnumb = 15000, iffix = 99999, nroute = 4, nrofg = 4, nlfg = 2, ndgg = 4,
                        ndfl = 6,
                        mxjdof = 9, ndim = 3, njgk = 2, ndrl = 2, ndd4 = 5, ndas = 9, ndac = 12, ndar = 7,
                        ndrr = 2, nils1 = 20, nils2 = 26, nils3=10,ndls1 = 44, ndls2 = 18, ndls3 = 35, nicj = 3,
                        nyz = 2, nftbc = 3, nlsu = 3, mxmn = 2, nimm1 = 14, ndmm1 = 40, mxpin = 5, nxy = 2, nij = 2,
                        ndmme = 24, nindmes = 3, nibl = 4, ndbl = 2, nisw = 9, ndsw = 5, niswor = 12,
                        niso = 3, ndso = 4, niww1 = 13, ndww1 = 20, nxz = 2, niwz1 = 4, ndwz1 = 5,
                        niws1 = 8, ndebws = 4, nizz = 2, ndzz = 2, nzdmax = 20, ndzze = 3, niap = 2,
                        ndap = 7, nisp = 6, ndsp = 11, nipp = 4, niab = 3, ndab = 7, nibj = 4, ndbj = 3,
                        nibz = 2, ndbz = 6, nibn = 5, ndbn = 9, nibq = 5, ndbq = 12, niby = 5, ndby = 12,
                        nrbyc = 7, ibymax = 4, nibs = 5, ndbs = 13, nibt = 5, ndbt = 9, nibw = 6, ndbw = 14,
                        nimt = 9, nrbbc = 8, nrect = 4, nisdp = 10, ndsdp = 55, niei = 20, ndei = 20,
                        npnlj1 = 20;
    qint32 msize, nband, mxnfl, numfg, mxngr, numgg, numj, mxjfl, mxjgr, mxjm,
           numrld, numrl, maxsl, mjslvd,mjslv, mxid4, numd4d, numd4, numasd, numas,
           numacd, numac, numard, numar, numrrd, numrr, numlsd, numls, mxjs, mxss,
           mxjc, mxcc, mxjr, mxrr, nummd, numm, numbl, numswd, numsw, mxso, numsod, numso,
           mxww, numwwd, numww, mxwz, numwzd, numwz, mxmw, nummwd, nummw, mxmz, nummzd,
           nummz, numzzd, numzz, numwsd, numws, numapd, numap, numspd, numsp, numpd, nump,
           numabd, numab, numbjd, numbj, numbzd, numbz, numbnd, numbn, numbqd, numbq,
           lbymax, numbyd, numby, numbsd, numbs, numbtd, numbt, numbwd, numbw, nummtd,
           nummt, numbd, numb, numcol, mxgrj, mxgrm, mxgrw, iexmcb, nwk, nwke, numsdp,
           mxmems, numemsd, numems, numeid, numei, jsize,numla;
    qint32 iunit;
    qint32 numd3,numlbd,numlb,mxdlb1,mxdlb2,mxdlb3,mxdlb4,numlrd,numlr,mxbeam,
           numlmd ,numlm,mxlhmm,numild,numil,numdld,numdl ,numsdpd;

    //Extract only necessary data
    QString icard;
    QString title;
    QVector<qint32> iroute;
    QVector<qint32> numfl;
    QVector<QVector<qint32> > kfl;
    QVector<QVector<qint32> > ifl;
    QVector<QVector<qint32> > lfg;
    QVector<QStringList> namefl;
    QVector<qint32> numgr;
    QVector<QVector<double> >ahfl;
    QVector<double>dfg;
    QVector<QStringList> namegr;
    QVector<QVector<double> >dgr;
    QVector<QVector<qint32> >lgr;
    QVector<qint32> igg;
    QVector<QVector<double> >dgg;
    QVector<qint32> jntnm;
    QStringList namej;
    QVector<qint32> jntrl;
    QVector<qint32> jrgdf;
    QVector<qint32> ncid4;
    QVector<QVector<qint32>> id4;

    QVector<QVector<qint32> >indj;
    QVector<QVector<double> > xyz;
    QVector<qint32> njfl;
    QVector<QVector<QVector<qint32> > > jntfl;
    QVector<qint32> njgr;
    QVector<QVector<QVector<qint32> > > jntgr;
    QVector<qint32> jnti;
    QVector<QVector<qint32> > jnto;
    QVector<QVector<qint32> > jntom;
    QVector<qint32> njmem;
    QVector<QVector<qint32>> jmem;
    QVector<qint32> numsl;
    QVector<QVector<double> > cosl;
    QVector<QVector<double> > drl;
//    QVector<QVector<double> > das;
//    QVector<QVector<double> > dac;
    QStringList namels;
    QVector<QVector<qint32> > ils1;
    QVector<QVector<QVector<qint32> > > ils2;
    QVector<QVector<QVector<double> > > dls1;
    QVector<QVector<qint32> > imm1;
    QVector<QVector<qint32> > indm;
    QVector<QVector<qint32> > ifpmm;
    QVector<QVector<double> > dmm1;
    QVector<QVector<QVector<double> > > evmem;
    QStringList namebl;
    QVector<QVector<qint32> > mbl;
    QStringList namesw;
    QVector<QVector<qint32> > isw;
    QVector<QVector<double> > dsw;
    QStringList nameso;
    QVector<QVector<QVector<qint32> > > iso;
    QVector<QVector<QVector<double> > > dso;
    QVector<qint32> nso;
    QVector<QVector<qint32> > iww1;
    QVector<QVector<qint32> > iww2;
    QVector<QVector<qint32> > iww3;
    QVector<QVector<double> > dww1;
    QVector<QVector<qint32> > iwz1;
    QVector<QVector<qint32> > iwz2;
    QVector<QVector<QVector<double> > > evmemw;
    QVector<QVector<qint32> > iws1;
    QVector<QVector<double> > debws;
    QVector<QVector<QVector<double> > > evws;

    QVector<QVector<qint32> > isp;
    QVector<QVector<double> > dsp;
    QVector<QVector<qint32> > iab;
    QVector<QVector<qint32> > ibj;
    QVector<QVector<qint32> > ibn;
    QVector<QVector<qint32> > ibq;
    QVector<QVector<qint32> > iby;
    QVector<QVector<qint32> > ibs;
    QVector<QVector<qint32> > ibt;
    QVector<QVector<qint32> > imt;
    QStringList cbz;
    QVector<QVector<double> > dab;
    QVector<QVector<double> > dbj;
    QVector<QVector<double> > dbz;
    QVector<QVector<double> > dbn;
    QVector<QVector<double> > dbq;
    QVector<QVector<double> > dby;
    QVector<QVector<double> > dbs;
    QVector<QVector<double> > dbt;
    QVector<QVector<double> > dbw;

    QVector<qint32 > nmz;
    QVector<QVector<qint32> > imz;
    QVector<QVector<double> > dmz;

    QVector<QVector<qint32> > izz;
    QVector<QVector<QVector<double>> > dzz;
    QVector<QVector<double> > dzze;

    QVector<QVector<qint32> > mdlfl;
    QVector<QVector<QVector<qint32> > > jdspfl;

    QStringList named4;
    QStringList nameas;
    QStringList nameac;
    QStringList namear;
    QStringList namerr;

    QVector<qint32> nspby;
    QVector<QVector<qint32> > jbycon ;
    QVector<QVector<QVector<double>> > dbycon;
    QVector<qint32> jbbcon ;
    QVector<QVector<double>> dbbcon;
    QVector<QVector<double>> dmme ;

    QStringList namedl;
    QVector<QVector<qint32> >idl;
    QVector<QVector<double>>ddl;
    QVector<QVector<qint32> >isdp;
    QVector<QVector<double>>dsdp;
    QVector<QVector<double>>pnlj1;
    QVector<QVector<qint32> >imztyp ;

    QVector<qint32 >jntpp;
    QVector<QVector<qint32> >ipp ;
    QVector<QVector<QVector<double>> > evpp;

    void clear()
    {

        title.clear();
        lfg.clear();
        ifl.clear();
        kfl.clear();
        namefl.clear();
        ahfl.clear();
        dfg.clear();
        numgr.clear();
        namegr.clear();
        dgr.clear();
        lgr.clear();
        igg.clear();
        dgg.clear();
        jntnm.clear();
        namej.clear();
        jntrl.clear();
        jrgdf.clear();
        indj.clear();
        xyz.clear();
        njfl.clear();
        jntfl.clear();
        njgr.clear();
        jntgr.clear();
        jnti.clear();
        jnto.clear();
        jntom.clear();
        njmem.clear();
        jmem.clear();
        numsl.clear();
        id4.clear();
        ncid4.clear();
        cosl.clear();//das.clear();dac.clear();
        drl.clear();
        namels.clear();
        ils1.clear();
        ils2.clear();
        dls1.clear();
        imm1.clear();
        indm.clear();
        ifpmm.clear();
        dmm1.clear();
        evmem.clear();
        namebl.clear();
        mbl.clear();
        namesw.clear();
        isw.clear();
        dsw.clear();
        nameso.clear();
        iso.clear();
        dso.clear();
        nso.clear();
        iww1.clear();
        iww2.clear();
        iww3.clear();
        dww1.clear();
        iwz1.clear();
        iwz2.clear();
        evmemw.clear();
        iws1.clear();
        debws.clear();
        evws.clear();
        isp.clear();
        dsp.clear();
        iab.clear();
        ibj.clear();
        ibn.clear();
        ibq.clear();
        iby.clear();
        ibt.clear();
        imt.clear();
        cbz.clear();
        dab.clear();
        dbj.clear();
        dbz.clear();
        dbn.clear();
        dbq.clear();
        dby.clear();
        dbs.clear();
        dbt.clear();
        dbw.clear();
        mdlfl.clear();
        jdspfl.clear();
        named4.clear();
        nameas.clear();
        nameac.clear();
        namear.clear();
        namerr.clear();
        izz.clear();
        dzz.clear();
        dzze.clear();
        nspby.clear();
        jbycon.clear();
        dbycon.clear();
        jbbcon.clear();
        dbbcon.clear();
        dmme.clear();
        namedl.clear();
        idl.clear();
        ddl.clear();
        isdp.clear();
        dsdp.clear();
        pnlj1.clear();
        imztyp.clear();

        evpp.clear();
        ipp.clear();
        jntpp.clear();
    }
};
struct DataNBLD3D_LOAD {
    //pres/nainps.f
    static const qint32 nfrcfl = 3, ndlemm = 5, ncmqmm = 14, nilemm = 5;
    qint32 mxlemm = 50000;
    static const qint32 ndleww=2,ncmqws=10,ndlews=2,jwcase=8,nisefg=3,nisefl=4;
    static const qint32 ndsefg=8,ndsefl=8,niwifg=3,niwifl=2,ndwifg=6,ndwifl=10;
    static const qint32 nilhmm=5,ndlhmm=5,nlndle=2,nlndlh=2;
    qint32 numc, lxlemm, numcl, numcld, numse, numsed, mxwifl, numwi, numwid,mxlhmm;

    QVector<qint32>indle;
    QStringList namel;
    QStringList titc;
    QVector<qint32>indse;
    QVector<qint32>indwi;
    QVector<double>dirse;
    QVector<QVector<QVector<qint32> > >kincj;
    QVector<QVector<QVector<QVector<double> > > >frcfl;
    QVector<QVector<qint32> >ibc;
    QVector<QVector<QVector<double> > >fincj;
    QVector<qint32>nlemm;
    QVector<QVector<QVector<qint32> > >ilemm;
    QVector<QVector<QVector<double> > >cmqmm;

    QVector<QVector<QVector<double> > >dlemm;
    QVector<QVector<QVector<double> > >clemm;
    QVector<QVector<QVector<double> > >cmqmmf;
    QVector<QVector<QVector<double> > >cmqww;
    QVector<QVector<QVector<double> > >dleww;
    QVector<QVector<QVector<double> > >cmqws;
    QVector<QVector<QVector<double> > >dlews;
    QVector<QVector<double> > semass;
    QVector<QVector<QVector<double> > >flmass;
    QVector<QVector<QVector<qint32> > >isefg;
    QVector<QVector<QVector<QVector<qint32>>>>isefl;
    QStringList namese;
    QVector<qint32>itypse;

    QVector<QVector<QVector<double>>> dsefg;
    QVector<QVector<QVector<QVector<double>>>> dsefl;
    QVector<QVector<QVector<qint32> > >iwifg;
    QVector<QVector<QVector<qint32> > >nwifl;
    QVector<QVector<QVector<QVector<QVector<qint32>>> > >iwifl;
    QStringList namewi;
    QVector<qint32> itypwi;

    QVector<QVector<QVector<double> > >dwifg;
    QVector<QVector<QVector<QVector<QVector<double>>>> >dwifl;
    QVector<qint32> nlhmm;
    QVector<QVector<QVector<qint32> > >ilhmm;

    QVector<QVector<QVector<double> > >dlhmm;
    QVector<QVector<QVector<qint32> > >lndle;
    QVector<QVector<QVector<qint32> > >lndlh;

    void clear()
    {
        indle.clear();
        namel.clear();
        titc.clear();
        indse.clear();
        indwi.clear();
        dirse.clear();
        frcfl.clear();
        ibc.clear();
        fincj.clear();
        nlemm.clear();
        ilemm.clear();
        cmqmm.clear();
    }

};

struct DataNBLD3D_RSTStep{
    qint32 iastep, iend;
    QVector<QVector<double> > ujnt;
    QVector<QVector<double> > fintj;
    QVector<QVector<double> > react;
    QVector<QVector<double> > rstmm;
    QVector<QVector<QVector<double> > > phaiyz;
    QVector<QVector<qint32> > ipspmm;
    QVector<QVector<double> > ductmm;
    QVector<QVector<qint32> > kpspmm;
    QVector<QVector<QVector<double> > > rstww;
    QVector<QVector<qint32> > ipspww;
    QVector<QVector<double> > fintws;
    QVector<QVector<QVector<double> > > rstws;
    QVector<QVector<double> > rstpp;
    QVector<QVector<qint32> > ipsppp;
    QVector<double> rstbb;
    QVector<qint32> ipspbb;
    QVector<QVector<double> > rstby;
    QVector<qint32> ipspby;
    QVector<QVector<double> > rstbs;
    qint32 kcasol;
    QVector<qint32> icase;
    QVector<double> ccase;
    QVector<QVector<double> > dsdpv;
    QVector<QVector<QVector<double>>> squmm;
    QVector<double> squww;
    void clear()
    {
        ujnt.clear();
        fintj.clear();
        react.clear();
        rstmm.clear();
        phaiyz.clear();
        ipspmm.clear();
        ductmm.clear();
        kpspmm.clear();
        rstww.clear();
        ipspww.clear();
        fintws.clear();
        rstws.clear();
        rstpp.clear();
        ipsppp.clear();
        rstbb.clear();
        ipspbb.clear();
        rstby.clear();
        ipspby.clear();
        rstbs.clear();
        icase.clear();
        ccase.clear();
        dsdpv.clear();
        squmm.clear();
        squww.clear();
    }
};

struct DataNBLD3D_RST {
    static const qint32 mxjdof = 9, mxjdf6 = 6, nxy = 2, nyz = 2, nxz = 2,
                        nij = 2, ndim = 3, nrect = 4, nstp2 = 2,ngintj=4, mxmtyp = 6, lmtyp = 11, mxpin = 5,
                        ndeb = 30, nrst = 8, nrstmm = 8, nipspm = 3, nphai = 6, nengmm = 3, nductm = 7,
                        msdd = 3, nimsc = 1, nrmsc = 9, njmsv = 4, ndmsv = 9, mrdd = 2, nimrc = 1,
                        nrmrc = 7, njmrv = 2, ndmrv = 4, nibrc = 1, nrbrc = 5, njbrv = 2, ndbrv = 7,
                        lmtypw = 5, ndebww = 14, nelbww = 3, nrstww = 4, nipspw = 3, msddw = 2, nimscw = 1,
                        nrmscw = 8, njmsvw = 4, ndmsvw = 9, nimrcm = 1, nrmrcm = 6, njmrvm = 6,
                        ndmrvw = 64, ndebws = 4,
                        nrstws = 3, ngaus = 9, nadfws = 8, ndebpp = 3, npdim = 3, nppdd = 2, nippc = 1,
                        nrppc = 8, njppv = 2, ndppv = 4, nibj = 4, ndbj = 3, nibn = 5, ndbn = 9, nibq = 5,
                        ndbq = 12, niby = 5, ndby = 12, nibs = 5, ndbs = 13, nrstbs = 1,
                        nibt = 5, ndbt = 9, nibw = 6, ndbw = 14, nibbc = 1, nrbbc = 8, njbbv = 2, ndbbv = 4,
                        nrstby = 2, nbydd = 2, nibyc = 1, nrbyc = 7, njbyv = 2, ndbyv = 4, ndsdpv = 10;
    QString nameam;
    QList<DataNBLD3D_RSTStep> stressSteps;
};

struct DataNBLD3D_RSTQD {
    static const qint32 nxyr=3,nualst=13,nualgr=8,nhng=13,ndufmm=18,nultmm=90,nstpmm=6,nultww=7,
    nstpww=4,nstppp=2,npdim=3,mxkcis=50,mxkcqs=50,
    nfgqun=23,nfrw=2,nmamem=21,nrdmem=14,nramem=31,nmaplt=5,nrdplt=1,nkajnt=4,nsajnt=4,nkamem=3,nsamem=11,
    nicpnl=3,ndcpnl=5,ndsfl=3,
    ncqumm=17,ncquww=7,ncqupp=4,nlqumm=8,nlquww=2,nlqupp=3,ncgpjnt=6,ncgpcmp=15,nrstmm=8,
    njsscb=2,nesscb=15,nesrcb=22,
    nibar1=2,nibar2=3,nibar3=3,ndbar1=22,ndbar2=9,ndbar3=11;
    qint32 nstp,numis,numqs,numisd,numqsd,ksolv,idir,iend,lqustep;
    QString nameam,nmrsu;
    QStringList nameis,nameqs;

    QVector<qint32> iastep,nkcis,nkcqs;
    QVector<QVector<qint32>> istpmm,istpww,ikcis,ikcqs,mamem,maplt,
    lqumm,lquww,lqust,jsscb,ibar1,ibar2,ibar3;
    QVector<QVector<QVector<qint32>>> iflhng,istppp,mfgqun,icpnl,lqupp;
    QVector<QVector<QVector<QVector<qint32>>>> kajnt,kamem;
    QVector<QVector<QVector<QVector<QVector<qint32>>>>> igrhng;

    QVector<double> castep;
    QVector<qreal> coefis,coefqs,sedir;
    QVector<QVector<qreal>> ckcis,ckcqs;
    QVector<QVector<double>> dufmm,ultmm,cstpmm,ultww,cstpww,rdplt,ramem,
    cqumm,cquww,cqust,cgpjnt,rstmml,esscb,esrcb,dbar1,dbar2,dbar3;
    QVector<QVector<QVector<double>>> cstppp,rfgqun,rdmem,dcpnl,dsfl,cqupp,cgpcmp;
    QVector<QVector<QVector<QVector<double>>>> qst,ust,ualst,sajnt,samem;
    //ualst
    //2:th,3:q,7:dx,8:dy？解析変数excelとどっちが正か
    //ityp=0:ds,1:qu
    //iseq[0]=ids,iseq[1]=iqu
    //outplot/c_makeQD.cの600行目の後ろくらい参照

    //igrhng
    //部材種別ごとの降伏ステップ
    //rfgqun
    //16:Qun,17:Qu
    QVector<QVector<QVector<QVector<QVector<QVector<double>>>>>> qstgr,ustgr,ualgr;
    void clear()
    {
        nameis.clear();nameqs.clear();

        nkcis.clear();nkcqs.clear();
        istpmm.clear();istpww.clear();ikcis.clear();ikcqs.clear();mamem.clear();maplt.clear();
        lqumm.clear();lquww.clear();lqust.clear();jsscb.clear();ibar1.clear();ibar2.clear();ibar3.clear();
        iflhng.clear();istppp.clear();mfgqun.clear();icpnl.clear();lqupp.clear();
        kajnt.clear();kamem.clear();
        igrhng.clear();

        castep.clear();coefis.clear();coefqs.clear();sedir.clear();
        dufmm.clear();ultmm.clear();cstpmm.clear();ultww.clear();cstpww.clear();ckcis.clear();ckcqs.clear();rdplt.clear();
        cqumm.clear();cquww.clear();cqust.clear();cgpjnt.clear();rstmml.clear();esscb.clear();esrcb.clear();dbar1.clear();dbar2.clear();dbar3.clear();
        cstppp.clear();rfgqun.clear();rdmem.clear();dcpnl.clear();dsfl.clear();cqupp.clear();cgpcmp.clear();
        qst.clear();ust.clear();ualst.clear();sajnt.clear();samem.clear();
        qstgr.clear();ustgr.clear();ualgr.clear();
    }
};
struct DataNBLD3D_PREA{
    //prea/nainpa.f
    static const qint32 nimm1=14,nimm2=5,nimm3=9,nimm4=30,
    ndmm1=40,ndmm2=16,ndmm3=17,ndmm4=8,ndmm5=20,ndmm6=20,mxpin=5,
    ndeb=30,lmtyp=11,nzdof=8,nzsiz=36,nimsc=1,nrmsc=9,imsmax=80,nimrc=1,nrmrc=7,imrmax=4,

    //nihsc=1,nrhsc=9,ihlmax=10,ihsmax=100,nihrc=1,nrhrc=7,ihrmax=4,
    nibrc=1,nrbrc=5,niaxc=2,

    ndaxc=25,nrstmm=8,ncmqmm=14,msdd=3,nisw=9,ndsw=5,niww1=13,niww4=9,ndww1=20,ndww2=18,
    ndww3=16,lmtypw=5,ndebww=14,nrect=4,nimscw=1,nrmscw=8,imsmxw=20,nimrcw=1,nrmrcw=6,

    nrstww=4,neww=3,niwz1=4,ndwz1=5,ndwz2=3,nizz=2,ndzz=2,nzdmax=20,niws1=8,ndws1=1,ndebws=4,
    nizs=2,ndzs=5,niap=2,ndap=7,nisp=6,ndsp=11,nipp=4,ndebpp=3,

    nrppc=8,npdim=3,nrstpp=1,niab=3,nibj=4,nibn=5,nibq=5,niby=5,nibs=5,nibt=5,nibw=6,nimt=9,
    nibbc=1,nrbbc=8,nibyc=1,nrbyc=7,
    nrstbb=1,nrstby=2,niban=10,nrban=50,nicrf=60,mxkcis=50,mxkcqs=50;//,nicrf1=11,nicrf2=11,nicrf3=3;
    qint32 numwp,numwpd,numez,numezd,numms,nummsd,numbr,numbrd,numcb,
    lmsmax,lmrmax,lmsmxw,mxzs,numzs,numzsd,numis,numqs,numaxc,numaxcd,
    numhg,numhgd,lhlmax,lhsmax,lhrmax,numhs,numhsd;

    qint32 numisd,numqsd,numg1,numg1d,
    numhg3,numhg3d,numhg2,numhg2d,
    ifbmax,numfs,numfsd,numfb,numfbd,
    nrstn,nrstm,numnmu,numnmud,numnmp,numnmpd,
    njdel,nd4del,
    nrstnw,numhgw,numhgwd,numnmw,numnmwd,
    maxsla,mjslvad,mjslva,msize,nband,nwk;

    QVector<qint32>nmtyp;

    QVector<QVector<qint32>> mtyp;//lmtype,nummd
    QVector<QVector<double>> debmm;//ndeb,nummd
    QVector<QVector<double>> debww;//ndebww,numwwd
    QVector<QVector<double>> dmm2;//ndmm2,nummd
    QVector<QVector<double>> rstmmi;//nrstmm,nummd
    QVector<QVector<qint32>> imm2;//nimm2,nummd
    QVector<QVector<QVector<QVector<double>>>> dmscow;
    QVector<QVector<qint32>> indws,jppcon;
    QVector<qint32> nspby;
    QVector<QVector<QVector<double>>> dnms;

    QVector<QVector<QVector<double>>> dmm3;
    QVector<QVector<QVector<QVector<double>>>> dmm5;
    QVector<QVector<qint32>> iww4;
    QVector<QVector<double>> dww2;
    QVector<QVector<double>> dww3;
    QVector<qint32> nzs;
    QVector<QVector<QVector<qint32>>> izs;
    QVector<QVector<QVector<double>>> dzs;
    QVector<QVector<double>> dwz2;
    QVector<qint32> icrf;
    QVector<double> coefis;
    QStringList nameis;
    QVector<qint32> nkcis;
    QVector<QVector<qint32>> ikcis;
    QVector<QVector<double>> ckcis;
    QVector<double> coefqs;
    QStringList nameqs;
    QVector<qint32> nkcqs;
    QVector<QVector<qint32>> ikcqs;
    QVector<QVector<double>> ckcqs;
    QVector<QVector<QVector<double>>> rstppi;
    QVector<QVector<double>> rstbbi;
    QVector<QVector<double>> rstbyi;
    QVector<QVector<QVector<qint32>>> iban;
    QVector<QVector<QVector<double>>> rban;
    QVector<QVector<double>> dmm6;
    QVector<QVector<qint32>> imm4;
    //以降省略
    void clear(){mtyp.clear();debmm.clear();dmm2.clear();rstmmi.clear();imm2.clear();
                debww.clear();dmscow.clear();indws.clear(),jppcon.clear();nspby.clear();dnms.clear();
                dmm3.clear();dmm5.clear();iww4.clear();dww2.clear();dww3.clear();
                nzs.clear();izs.clear();dzs.clear();dwz2.clear();icrf.clear();
                coefis.clear();nameis.clear();nameis.clear();nkcis.clear();
                ikcis.clear();ckcis.clear();coefqs.clear();nameqs.clear();
                nkcqs.clear();ikcqs.clear();ckcqs.clear();rstppi.clear();
                rstbbi.clear();rstbyi.clear();iban.clear();rban.clear();
                dmm6.clear();imm4.clear();}

};
struct DataNBLD3D_QUDS {
    int numdq,numdqd;
    QStringList cnamedq;
    QVector<QStringList> casedq;
    QStringList casefes;
    QStringList commdq;
    QVector<qreal> angldq;
    QVector<int> kdir;
    void clear(){
        cnamedq.clear();casedq.clear();casefes.clear();commdq.clear();
        angldq.clear();kdir.clear();
    }
};

struct DataNBLD3D_MODE {
    int nummode;//正の整数
    int numj;//正の整数
    QVector<qreal> naturalPeriod;//size=mode数
    QVector<qreal> totalMass;//X Y Z rz ry rz
    QVector<QVector<qreal> > effectiveMassRatio;//beta*beta/totalmass
    QVector<QVector<XYZ> > mode; //mode[nummode][jno]
    QVector<QVector<qreal> > beta;//X Y Z rx ry rz * mode数
    void clear()
    {
        naturalPeriod.clear();
        totalMass.clear();
        effectiveMassRatio.clear();
        mode.clear();
        beta.clear();
    }
};
struct DataNBLD3D_CHCK {
    //定数
    static const qint32 nchp=3,nrstmc = 30,nrstma = 33,nrstwc = 2,nrstwa = 17,nich=2,
    nimmch=40,nippch=10,ndppch=30,niopch=10,ndopch=30,nrstn=21,nrstm=25,nicoff=4,ndcoff=12,nisscb=2,ndsscb=15,ndsrcb=22;

    qint32 numch,mxrsch;
    QStringList namech;
    QVector<qint32> nrsch;
    QVector<QVector<qint32>> irsch;
    QVector<QVector<qreal>> corsch;
    QVector<QStringList> nmrsch;

    QVector<QVector<QVector<QVector<double> > > > rstmma;
    QVector<QVector<QVector<QVector<double> > > > rstmmc;
    QVector<QVector<QVector<double> > > rstwwa;
    QVector<QVector<QVector<double> > > rstwwc;

    QVector<QVector<qint32> > ich;
    QVector<QVector<double> > cupw50;
    QVector<QVector<QVector<qint32> > > immch;
    QVector<QVector<QVector<QVector<qint32> > > > ippch;
    QVector<QVector<QVector<QVector<double> > > > dppch;
    QVector<QVector<QVector<qint32> > > iopch;
    QVector<QVector<QVector<double> > > dopch;
    QVector<QVector<QVector<QVector<double> > > > tnsec;
    QVector<QVector<QVector<QVector<QVector<QVector<double> >>>>> tmsec;
    QVector<QVector<QVector<qint32> > > icoff;
    QVector<QVector<QVector<double> > > dcoff;
    QVector<QVector<QVector<qint32> > > isscb;
    QVector<QVector<QVector<double> > > dsscb;
    QVector<QVector<QVector<double> > > dsrcb;
    void clear()
    {
        nrsch.clear();irsch.clear();corsch.clear();nmrsch.clear();
        rstmma.clear();rstwwa.clear();
        rstmmc.clear();rstwwc.clear();
        ich.clear();cupw50.clear();immch.clear();ippch.clear();dppch.clear();iopch.clear();dopch.clear();
        tnsec.clear();tmsec.clear();icoff.clear();dcoff.clear();isscb.clear();dsscb.clear();dsrcb.clear();
    }
};

struct DataNBLD3D_GENE{
     qint32 kflags, vflags;
};

struct DataNBLD3D_STEC{
    static constexpr qint32 nintec = 5;
    static constexpr qint32 ndls1 = 44;
    static constexpr qint32 nxy = 2;
    static constexpr qint32 nouts1 = 28;
    static constexpr qint32 nouts2 = 38;
    static constexpr qint32 noute1 = 20;
    static constexpr qint32 noute2 = 4;
    static constexpr qint32 nvalgr = 14;
    static constexpr qint32 nicgc = 4;
    static constexpr qint32 ndcgc = 6;

    qint32 numec, numecd, mxcase, nset;
    QStringList nameec;
    QVector<QVector<qint32>> intec;
    QVector<QVector<QVector<double>>> bbhfl;
    QVector<QVector<QVector<QVector<double>>>> outs1;
    QVector<QVector<QVector<QVector<QVector<double>>>>> outs2;
    QVector<QVector<QVector<double>>> oute1;
    QVector<QVector<QVector<QVector<double>>>> oute2;
    QVector<QVector<QVector<QVector<QVector<QVector<QVector<double>>>>>>> valgr;
    QVector<QVector<QVector<QVector<qint32>>>> icgc;
    QVector<QVector<QVector<QVector<double>>>> dcgc;
    QVector<QStringList> nmfefs, crstdr;
    QVector<qint32> mcase;
    QVector<QVector<double>> sedir;
    QVector<QStringList> nmrste;
    QVector<QVector<double>> cedir;
    void clear()
    {
        nameec.clear();
        intec.clear();
        bbhfl.clear();
        outs1.clear();
        outs2.clear();
        oute1.clear();
        oute2.clear();
        valgr.clear();
        icgc.clear();
        dcgc.clear();
        nmfefs.clear();
        crstdr.clear();
        mcase.clear();
        sedir.clear();
        nmrste.clear();
        cedir.clear();
    }
};

struct DataNBLD3D_NMIN{
    static constexpr qint32 nptdd = 3;

    qint32 nrstn,nrstm;
    QVector<QVector<QVector<double>>> rnss1;
    QVector<QVector<QVector<double>>> rnss2;
    QVector<QVector<QVector<double>>> rnss3;
    QVector<QVector<QVector<double>>> rnrc1;
    QVector<QVector<QVector<double>>> rnrc2;
    QVector<QVector<QVector<double>>> rnrc3;
    QVector<QVector<QVector<double>>> rnsec1;
    QVector<QVector<QVector<double>>> rnsec2;
    QVector<QVector<QVector<double>>> rnsec3;
    QVector<QVector<QVector<double>>> rnsec5;
    QVector<QVector<QVector<QVector<QVector<double>>>>> rmss1;
    QVector<QVector<QVector<QVector<QVector<double>>>>> rmss2;
    QVector<QVector<QVector<QVector<QVector<double>>>>> rmss3;
    QVector<QVector<QVector<QVector<QVector<double>>>>> rmrc1;
    QVector<QVector<QVector<QVector<QVector<double>>>>> rmrc2;
    QVector<QVector<QVector<QVector<QVector<double>>>>> rmrc3;
    QVector<QVector<QVector<QVector<QVector<double>>>>> rmsec1;
    QVector<QVector<QVector<QVector<QVector<double>>>>> rmsec2;
    QVector<QVector<QVector<QVector<QVector<double>>>>> rmsec3;
    QVector<QVector<QVector<QVector<QVector<double>>>>> rmsec5;
    QVector<QVector<QVector<QVector<QVector<double>>>>> ptddnm;
    void clear()
    {
        rnss1.clear();
        rnss2.clear();
        rnss3.clear();
        rnrc1.clear();
        rnrc2.clear();
        rnrc3.clear();
        rnsec1.clear();
        rnsec2.clear();
        rnsec3.clear();
        rnsec5.clear();
        rmss1.clear();
        rmss2.clear();
        rmss3.clear();
        rmrc1.clear();
        rmrc2.clear();
        rmrc3.clear();
        rmsec1.clear();
        rmsec2.clear();
        rmsec3.clear();
        rmsec5.clear();
        ptddnm.clear();
    }
};
} // namespace post3dapp
#endif // DEFINE_NPSTN_H