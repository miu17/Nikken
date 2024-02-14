#ifndef DEFINEOUTPUT_H
#define DEFINEOUTPUT_H

#include "define_unifieddata.h"
#include "member_force.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{

/* 任意項目出力図の種類 <- OutputFigureTextに日本語*/
enum class OUTFIGTYPE {
    UNDEFINED,      // 未定義
    PLANINPUT,      // 入力中の伏図：表示設定どおり
    PLAN,           // 解析実行時の伏図：荷重は表示しない
    LOADPLAN,       // 解析実行時の荷重伏図：部材符号は表示しない
    SECTIONINPUT,   // 入力中の軸組図：表示設定どおり
    SECTION,        // 解析実行時の軸組図：荷重は表示しない
    LOADSECTION,    // 解析実行時の荷重軸組図：部材符号は表示しない
    // PERS
    // PERSINPUT
    CALCLOAD2D,     // 荷重拾い(2D)
    // CALCLOAD3D,    // 荷重拾い(3D)
    MODEL,
    LENGTH,
    FACELENGTH,
    COORDINATE,
    RIGIDLENGTH,
    LATERALBUCKLEN,
    EULERBUCKLEN,
    JOINTMASS,
    OUTERFORCE,
    OUTERMOMENT,
    RIGIDITYPHI,
    CMQ,
    AXIALMAP,
    REACTMAP,
    COMBIREACTMAP,
    SHEARMAP,
    VERTICALLOADMAP,
    LAYERDEFORMMAP,
    LAYERSTIFFMAP,
    GCKCP,          // NewBUILDING3Dでやめる？
    NSTRESS,
    FSTRESS,
    RSTRESS,
    AXIAL,
    TORSION,
    DEFORM,
    DISP,
    CHECKRATIO,
    ALLOWRATIOE,
    ALLOWRATIOU,
    PLASTICRATIO,
    RANK,
    ALLOW,
    STRENGTH,
    YIELDSTEP,
    YIELDLOADMAG,
    CenterOfRigidity,
    QDelta,
    QDeltaFrameFloors,
    QDeltaFloorFrames,
    QTheta,
    QThetaFrameFloors,
    QThetaFloorFrames,
    QuNeedf,
    NMINT
};



/* 結果ウィジェットに表示するテキスト項目*/
enum class OUTTEXTTYPE {
    Alist,
    Estim,
    Rfloor,
    Ststiff,
    Frstiff,
    Frdist,
};

class POST3D_CLASS_DLLSPEC OutputFigureText
{
public:
    static QString outfigtypeToString(OUTFIGTYPE);
    static QString outfigtypeToEnglish(OUTFIGTYPE);
};



/* 荷重拾いタイプ　..全て、伝達荷重、手動入力、自重 */
enum class LOADSUMTYPE { LOADSUM_ALL = 0, LOADSUM_AUTO, LOADSUM_MANUAL, LOADSUM_SELF };

/* 表示方向　..面内、面外、軸方向 */
enum class OUTPUTDIR { ODIR_PLIN = 0, ODIR_PLOUT, ODIR_AXIAL };

/* 荷重拾い(3D)表示方向 ..yz軸方向、x軸方向 */
enum class CALCOUTPUTDIR {CALC_ODIR_YZ, CALC_ODIR_X};

/* モード or βU */
enum MODE_TYPE {MODE = 0, BETAX_U, BETAY_U};


/* 分布荷重　wa,wb */
struct POST3D_CLASS_DLLSPEC WABLOAD {
    explicit WABLOAD( const QPointF &_wa = QPointF(),
                      const QPointF &_wb = QPointF() )
        : loadWA(_wa), loadWB(_wb) { }

    QPointF loadWA;          // a(m),wa(kN/m)
    QPointF loadWB;          // b(m),wb(kN/m)
};

/* 部材荷重 */
struct POST3D_CLASS_DLLSPEC MLOADSUM {
    explicit MLOADSUM( const QString &_str = QString(),
                       LoadType::LOADTYPE _type = LoadType::LTYPE_DL,
                       LOADDIR _dir = LOADDIR::LDIR_X,
                       QList<qreal> _dload = QList<qreal>(),
                       QList<QPointF> _cload = QList<QPointF>(),
                       WABLOAD _wab = WABLOAD() )
        : loadName(_str), loadType(_type), loadDir(_dir),
          sumUpLoad(_dload), concentratedLoad(_cload), loadWAB(_wab) { }

    QString loadName;           // 荷重符号、ブランク時は拾われた荷重
    LoadType::LOADTYPE loadType;          // DL,LL1,LL2,LL3,LL4,LL5,LLE
    LOADDIR loadDir;            // X,Y,Z,MX,MY,MZ
    QList<qreal> sumUpLoad;           // kN
    QList<QPointF> concentratedLoad;  // m,kN
    WABLOAD loadWAB;                  // m,kN/m
};

/* 節点荷重 */
struct POST3D_CLASS_DLLSPEC JLOADSUM {
    explicit JLOADSUM( const QString &_str = QString(),
                       LoadType::LOADTYPE _type = LoadType::LTYPE_DL,
                       LOADDIR _dir = LOADDIR::LDIR_X,
                       qreal _load = 0.0 )
        : loadName(_str), loadType(_type), loadDir(_dir),
          concentratedLoad(_load) { }

    QString loadName;           // 荷重符号、ブランク時は拾われた荷重
    LoadType::LOADTYPE loadType;          // DL,LL1,LL2,LL3,LL4,LL5,LLE
    LOADDIR loadDir;            // X,Y,Z,MX,MY,MZ ...Global
    qreal concentratedLoad;
};


/*以下茨木追加*/
struct POST3D_CLASS_DLLSPEC MRESULT {
    /* フェイス位置応力 */
    QHash<QString, QList<MemberForce> > faceSectionalForce;
    /* 節点位置応力 */
    QHash<QString, QList<MemberForce> > nodeSectionalForce;
    /* 降伏状態 */
    enum class YIELD_STATE {NOT = 0, YIELD, HINGE, SHEAR_CRACK, SHEAR_YIELD, AXIAL_YIELD};
    QHash<QString, QList<QList<YIELD_STATE> > > yieldstate; //key=stresscase,list=[ijc][step]

};
struct POST3D_CLASS_DLLSPEC JRESULT {
    /* 節点変位 */
    QHash<QString, QList<XYZ> > jointDisp; //stepのlist
    /* 節点回転 */
    QHash<QString, QList<XYZ> > jointRotate; //stepのlist
    /* 節点反力 */
    QHash<QString, QList<QList<qreal> > >react; //react[loadcase][step][dof]

};
struct POST3D_CLASS_DLLSPEC SRESULT {
    QHash<QString, QVector<qreal> > springValue;
};

/* モード変位 */
struct POST3D_CLASS_DLLSPEC MODEDATA {
    void setModeData(QHash<QUuid, QVector<XYZ> > _mode, QVector<QVector<qreal> >_beta,
                     QVector<qreal> _naturalPeriod, QVector<qreal> _totalMass);
    XYZ getModeDisp(MODE_TYPE type, QUuid uid, int nmode, qreal dispratio = 1, qreal time = 0.0,
                    qreal timespeedratio = 1.0);
    void clear();
    int nummode;//正の整数
//    int numj;//正の整数
    QVector<qreal> naturalPeriod;//size=mode数
    QVector<QVector<qreal> > beta;//X Y Z rx ry rz * mode数
    QVector<qreal> totalMass;//X Y Z rz ry rz
    QVector<QVector<qreal> > effectiveMassRatio;//beta*beta/totalmass
    QHash<QUuid, QVector<XYZ> > mode; //mode[nummode]
    QHash<QUuid, QVector<XYZ> > standardizedMode;
    //最大最小値の絶対値が1になるように倍率をかける
    void resetStandardizedMode();
    void resetEffectiveMassRatio();
};
struct POST3D_CLASS_DLLSPEC CHCKDATA {
    //New3Dになって検定比がデフォルトの出力になったので、配列の中身など変更無いか確認する必要あり
    qint32 numch;
    QStringList namech;
    QHash<QUuid, QVector<QVector<float> > > allowanceRateBending; //曲げ余裕率[numch][icj]
    QHash<QUuid, QVector<QVector<float> > >
    verificationRatioShear; //せん断余裕率（SRCの場合はRC部分）[numch][icj]
    QHash<QUuid, QVector<QVector<float> > >
    verificationRatioShear2; //せん断余裕率(SRC部材のS部分,RC部材の付着)[numch][icj]
    QHash<QUuid, QVector<QVector<float> > > verificationRatioBending; //検定比[numch][icj]
    void clear();
};
struct POST3D_CLASS_DLLSPEC QUDSDATA {
    QUDSDATA(){angle=0;direction=1;dsFileName=QString();quFileName=QString();fes=QString();comment=QString();}
    qreal angle;
    int direction; // 1 or 2
    QString dsFileName;
    QString quFileName;
    QString fes;
    QString comment;

};
struct POST3D_CLASS_DLLSPEC PREMDATA {
    qreal Lky;//圧縮座屈長さ
    qreal Lkz;
};

struct POST3D_CLASS_DLLSPEC PREADATA {
    //多用するので読み下したデータ
    //EAとかEIとか剛性増大率とかのMODEL,MEMBERに紐づいたものなど
    qreal EIy;
    qreal phaiIy;
    qreal EIz;
    qreal phaiIz;
    qreal lenBeamB;
    qreal lenBeamU;
    qreal lenColumn;
    qreal phaiB;
    qreal phaiU;


};

} // namespace post3dapp
#endif
