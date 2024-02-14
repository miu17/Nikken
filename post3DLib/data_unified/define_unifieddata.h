#ifndef DEFINEUNIFIEDDATA_H
#define DEFINEUNIFIEDDATA_H

#include <QDebug>
#include <QHash>
#include <QRectF>
#include <QStringList>
#include <QTextStream>

#include "global_point.h"
#include "lineshape.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class N3DUuidTable;

/*
    データのタイプ ・・指定なし、階、通りグループ、通り、解析階、
               コンクリート材種、鉄骨材種、鉄筋径、鉄筋径（ダブル）、
               形状節点、柱、大梁、ブレース、小梁、スラブ、壁、鋼板壁、地下外壁、
               柱脚、開口、制振要素、免震要素、
               床荷重、部材荷重、節点荷重
*/
enum class DATATYPE { NODATATYPE = 0, TPFLOOR, TPFRAMEG, TPFRAMEP, TPAFLOOR,
                TPMACONC, TPMASTEEL, TPMAREIN, TPMAREIND,
                TPJOINT, TPCOLUMN, TPGIRDER, TPBRACE, TPBEAM, TPSLAB, TPWALL, TPUWALL,
                TPBASE, TPWALLOPEN, TPDAMPER, TPISO,
                TPJOINTLOAD, TPMEMBERLOAD, TPSLABLOAD, TPSPRING
              };
/* BUILDING3Dでnummとかnumwzとかnumbyとかで管理される配列に対応　*/
enum class B3D_DATATYPE {B3D_NODATATYPE = 0, B3D_TPSPRING_N, B3D_TPSPRING_Q, B3D_TPSPRING_MSS, B3D_TPSPRING_MT,B3D_TPLINEMEMBER,
                  B3D_TPWALL,B3D_TPZWALL,B3D_TPFLOOR,B3D_TPFRAME,B3D_TPFRAMEG,B3D_TPAFLOOR,B3D_TPJOINT
                        ,B3D_TPDAMPER,B3D_TPPLANE_WS};
/* 構造形式 ・・指定なし,RC,S,SRC,CFT */
enum class STRUCTTYPE {NOSTRUCTTYPE = 0, STP_RC, STP_S, STP_SRC, STP_CFT};

/* 形状入力、解析結果 */
enum class MDITYPE { MDIINPUT = 0, MDICOPY, MDICALCLOAD,MDIOUTPUT };

/* ウィンドウの種類 ・・指定無し、伏図、軸組図、３Ｄパース、テーブル */
enum class WINTYPE { NOWINTYPE = 0, WINPLAN, WINELEVATION, WINPERS, WINGRAPH, WINTABLE };

/* UnifiedDataの種類 */
enum class UnifiedDataType{Pstn, Input, FixedInput};

/* MDIサブウィンドウの型 */
struct POST3D_CLASS_DLLSPEC SUBWINSTATUS {
    explicit SUBWINSTATUS(
        MDITYPE mtype = MDITYPE::MDIINPUT,
        WINTYPE wtype = WINTYPE::NOWINTYPE)
        : MDIType(mtype), WinType(wtype){ }

    MDITYPE MDIType;
    WINTYPE WinType;
};


/* ファイルの追加読込*/

enum class FILEPRIOR { PRIOR_A = 0, PRIOR_B, PRIOR_AB };

struct POST3D_CLASS_DLLSPEC FILEREADPATTERN {
    explicit FILEREADPATTERN(
        const QString &_a = QString(),
        const QString &_b = QString(),
        const QUuid &bfid = QUuid(),
        FILEPRIOR _list = FILEPRIOR::PRIOR_A,
        FILEPRIOR _jid = FILEPRIOR::PRIOR_A,
        FILEPRIOR _mid = FILEPRIOR::PRIOR_A,
        FILEPRIOR _jxyz = FILEPRIOR::PRIOR_A,
        FILEPRIOR _mxyz = FILEPRIOR::PRIOR_A )
        : fileA(_a), fileB(_b), addedID(bfid),
          priorList(_list), priorJID(_jid), priorMID(_mid),
          priorJXYZ(_jxyz), priorMXYZ(_mxyz) { }

    QString fileA;
    QString fileB;
    QUuid addedID;
    FILEPRIOR priorList;
    FILEPRIOR priorJID;
    FILEPRIOR priorMID;
    FILEPRIOR priorJXYZ;
    FILEPRIOR priorMXYZ;
};

struct POST3D_CLASS_DLLSPEC FILESAVEPATTERN {
    explicit FILESAVEPATTERN(
        const QString &_a = QString(),
        const QString &_b = QString(),
        const QList<QUuid> &list = QList<QUuid>() )
        : fileA(_a), fileB(_b), floorList(list) { }

    QString fileA;
    QString fileB;
    QList<QUuid> floorList;
};


/* 階の種類　・・指定無し、地上階、地下階、塔屋階、補助階 */
enum class FLOORTYPE { FL_NOTYPE = 0, FL_OVERGROUND, FL_UNDERGROUND, FL_PENTHOUSE, FL_CHILD };

/* 階に関するデータ型 */
struct POST3D_CLASS_DLLSPEC FLOORVALUES {
    explicit FLOORVALUES(
        const QString &name = QString(),
        qreal height = 0.0,
        bool _displaysFloorScale = true,
        const QString &beam = QString(),
        const QString &slab = QString(),
        const QUuid &id = QUuid(),
        const QString &aname = QString(),
        qreal level = 0.0,
        bool _isChildFloor = false,
        const QUuid &pid = QUuid()
    )
        : floorName(name), floorHeight(height),
          displaysFloorScale(_displaysFloorScale), beamConcrete(beam), slabConcrete(slab),UUID(id),
          analysisName(aname), adjustedLevel(level),
          isChildFloor(_isChildFloor), parentID(pid) { }

    QString floorName;
    qreal floorHeight;      // 単位m
    bool displaysFloorScale;
    QString beamConcrete;
    QString slabConcrete;
    QUuid UUID;

    QString analysisName;
    qreal adjustedLevel;    // 単位m
    bool isChildFloor;
    QUuid parentID;
    FLOORTYPE floorType; //

    QString writeData(QSharedPointer<N3DUuidTable> table) const;

//    bool operator==(const FLOORVALUES& other) const { return (floorName==other.floorName && floorHeight==other.floorHeight); }
//    bool operator<(const FLOORVALUES& other) const { return (floorHeight<other.floorHeight); }

};


/* 通りの種類　・・指定無し、直線、円周、半径、直径、任意点列、閉じた円、閉じた点列 */
class POST3D_CLASS_DLLSPEC FrameType{
public:
    enum class FRAMETYPE {NOFRAMETYPE = 0, STRAIGHT, CIRCLE, RADIUS, DIAMETER, VARIABLE, CLCIRCLE, CLVARIABLE};
    static FRAMETYPE stringToEnum(const QString &str);
    static QString enumToString(const FRAMETYPE&);
};


/* 通りグループに関するデータ型 */
struct POST3D_CLASS_DLLSPEC FRAMEGROUPVALUE {
    explicit FRAMEGROUPVALUE(
        const QString &name = QString(),
        FrameType::FRAMETYPE type = FrameType::FRAMETYPE::NOFRAMETYPE,
        int sum = 0,
        const QPointF &po = QPointF(),
        qreal s_angle = 0.0,
        qreal e_angle = 0.0,
        int div = 1,
        qreal p_len = 1.0,
        qreal m_len = 1.0,
        QUuid s_floor = QUuid(),
        QUuid e_floor = QUuid(),
        const QUuid &gid = QUuid(),
        const QUuid &uid = QUuid() )
        : groupName(name), frameType(type), count(sum), basePoint(po),
          startAngle(s_angle), endAngle(e_angle), division(div),
          plusLength(p_len), minusLength(m_len),
          startFloorID(s_floor), endFloorID(e_floor),
          GID(gid) {Q_UNUSED(uid) }

    QString groupName;
    FrameType::FRAMETYPE frameType;
    int count;
    QPointF basePoint;  // 単位m
    qreal startAngle;   // 度
    qreal endAngle;     // 度
    int division;

    qreal plusLength;   // 単位m
    qreal minusLength;  // 単位m
    QUuid startFloorID;
    QUuid endFloorID;
    QUuid GID;

    QString writeData(QSharedPointer<N3DUuidTable> table) const;
};

/* 直線通りに関するデータ型 */
struct POST3D_CLASS_DLLSPEC STRAIGHTFRAMEVALUE {
    explicit STRAIGHTFRAMEVALUE(
        qreal _span = 0.0,
        qreal _angle = 0.0,
        const QPointF &p1 = QPointF(),
        bool ok1 = false,
        bool ok2 = false )
        : span(_span), angle(_angle), spoint(p1), isFirst(ok1), isLast(ok2) { }

    qreal span;
    qreal angle;    // 度単位
    QPointF spoint;
    bool isFirst;
    bool isLast;

    QString writeData(FrameType::FRAMETYPE type) const{
        switch(type){
        case FrameType::FRAMETYPE::NOFRAMETYPE:
            break;
        case FrameType::FRAMETYPE::STRAIGHT:
            return QString::number(span);
        case FrameType::FRAMETYPE::CIRCLE:
            return QString::number(span);
        case FrameType::FRAMETYPE::RADIUS:
            return QString::number(angle);
        case FrameType::FRAMETYPE::DIAMETER:
            return QString::number(angle);
        }
        return QString();
    }
};


/* 通りに関するデータ型 */
struct POST3D_CLASS_DLLSPEC FRAMEPOINTSVALUE {
    explicit FRAMEPOINTSVALUE(
        const QUuid &gid = QUuid(),
        const QList<QPointF> &list = QList<QPointF>(),
        const QString &name = QString(),
        const QPointF &npoint = QPointF(),
        FrameType::FRAMETYPE itype = FrameType::FRAMETYPE::NOFRAMETYPE,
        STRAIGHTFRAMEVALUE sv = STRAIGHTFRAMEVALUE(),
        const QUuid &uid = QUuid() )
        : GID(gid), pointList(list), frameName(name), namePoint(npoint),
          frameType(itype), straight(sv), UUID(uid) { }

    QUuid GID;
    QList<QPointF> pointList;
    QString frameName;
    QPointF namePoint;
    FrameType::FRAMETYPE frameType;
    STRAIGHTFRAMEVALUE straight;
    QUuid UUID;
    QString writeData(QSharedPointer<N3DUuidTable> table) const;


//    //TODO:3dPersで導入したが、一意に順序が定まれば何でもよい。範囲指定の観点から再実装？
//        bool operator==(const FRAMEPOINTSVALUE& other) const {
//            if (frameName==other.frameName && GID==other.GID && coordinate==other.coordinate){return true;}
//            else{return false;}
//        }
//        bool operator<(const FRAMEPOINTSVALUE& other) const {
//            if (GID<other.GID){
//                return true;
//            }else if(GID>other.GID){
//                return false;
//            }else if(coordinate<other.coordinate){
//                return true;
//            }else{return false;}
//        }
};


/* 断面定義　temporary */

enum class PLANEMODEL { NONSTRUCT_MODEL = 0, WALL_MODEL, ZWALL_MODEL,SLAB_MODEL };                    // 面材モデル化タイプ（非構造、耐力壁、雑壁、スラブ）
enum class PLANESUBMODEL { NONSTRUCT_SUBMODEL = 0, WI_MODEL, WH_MODEL,
                     SI_MODEL, SH_MODEL,PLATE_MODEL,MEMBRANE_MODEL};                    // 面材モデル化サブタイプ

/* 部材データの諸元 */

enum class PARTINGLEN { PART_FULL = 0, PART_INTERNAL };               // 端部長さの基準（全長、内法）
enum class PARTINGRULE { PART_DEFAULT = 0, PART_VALUE, PART_RATIO };  // 端部長さの決め方（デフォルト、値指定、比率指定）
enum class FACERULE { FACE_AUTO = 0, FACE_VALUE };                    // フェイス長さの決め方（自動、値指定）

struct POST3D_CLASS_DLLSPEC MEMBERPARTITION {  // 部材区切り位置
    explicit MEMBERPARTITION(
        qreal _len_fi = 0.0,
        qreal _len_fj = 0.0,
        qreal _len_i = 0.0,
        qreal _len_j = 0.0,
        qreal _len_sum = 0.0 )
        : lenFi(_len_fi), lenFj(_len_fj),
          lenI(_len_i), lenJ(_len_j),
          lenSum(_len_sum) { }

    qreal lenFi;        // i端フェイス位置 mm （i端を原点とする長さ座標系）
    qreal lenFj;        // j端フェイス位置 mm （i端を原点とする長さ座標系）
    qreal lenI;         // i端 端部位置 mm （i端を原点とする長さ座標系）
    qreal lenJ;         // j端 端部位置 mm （i端を原点とする長さ座標系）
    qreal lenSum;       // j端 mm （i端を原点とする長さ座標系）
};

struct POST3D_CLASS_DLLSPEC
    MEMBERVALUES {   // 部材ごとの設定値　コードアングル、寄り長さ、開口符号など
    explicit MEMBERVALUES(
        DATATYPE _type = DATATYPE::NODATATYPE,
        QString _name = QString(),
        qreal _angle = 0.0,
        qreal _shy = 0.0,
        qreal _shz = 0.0,
        FACERULE _ifr = FACERULE::FACE_AUTO,
        FACERULE _jfr = FACERULE::FACE_AUTO,
        qreal _if = 0.0,
        qreal _jf = 0.0,
        PARTINGLEN _pl = PARTINGLEN::PART_INTERNAL,
        PARTINGRULE _pr = PARTINGRULE::PART_RATIO,
        qreal _il = 1200.0,
        qreal _jl = 1200.0,
        qreal _ir = 0.25,
        qreal _jr = 0.25,
        QString _wname = QString(),
            QString _iyBane= QString(),
            QString _izBane= QString(),
            QString _jyBane= QString(),
            QString _jzBane= QString()
//            SPRINGTYPE _bane=NOSPRINGTYPE
    )
        : dataType(_type), sectionName(_name),
          codeAngle(_angle), shiftY(_shy), shiftZ(_shz),
          iFaceRule(_ifr), jFaceRule(_jfr),
          iFace(_if), jFace(_jf),
          partingLength(_pl), partingRule(_pr),
          iLength(_il), jLength(_jl),
          iRatio(_ir), jRatio(_jr),
          wallOpenName(_wname),iyBaneName(_iyBane),izBaneName(_izBane),
          jyBaneName(_jyBane),jzBaneName(_jzBane) {} //,springType(_bane){ }

    DATATYPE dataType;          // 部材種類
    QString sectionName;        // 部材符号
    qreal codeAngle;            // コードアングル（rad）
    qreal shiftY;               // 部材軸y方向　寄り長さ（mm）
    qreal shiftZ;               // 部材軸z方向　寄り長さ（mm）
    FACERULE iFaceRule;         // i端フェイス長さの決め方（自動、値指定）
    FACERULE jFaceRule;         // j端フェイス長さの決め方（自動、値指定）
    qreal iFace;                // i端 フェイス長さ（mm）
    qreal jFace;                // j端 フェイス長さ（mm）
    PARTINGLEN partingLength;   // 端部長さの基準（全長、内法）
    PARTINGRULE
    partingRule;    // 端部長さの決め方（デフォルト、値指定、比率指定）
    qreal iLength;              // i端 端部長さ（mm）
    qreal jLength;              // j端　端部長さ（mm）
    qreal iRatio;               // i端 端部長さの比率
    qreal jRatio;               // j端　端部長さの比率

    QString wallOpenName;       // 壁開口符号

    QString iyBaneName;//i端の回転ばね名
    QString izBaneName;//i端の回転ばね名
    QString jyBaneName;//j端の回転ばね名
    QString jzBaneName;//j端の回転ばね名

};

struct POST3D_CLASS_DLLSPEC MEMBERSIZE {  // 部材サイズ
    explicit MEMBERSIZE(
        qreal _sizeB = 0.0,
        qreal _sizeH = 0.0 )
        : sizeB(_sizeB), sizeH(_sizeH) { }

    qreal sizeB;
    qreal sizeH;
};


struct POST3D_CLASS_DLLSPEC LINESHAPE_IN {  // 部材リストからの情報
    explicit LINESHAPE_IN(
        const QString &_sname = QString(),
        const QString &_bname = QString(),
        RENDERSHAPE _type = RENDERSHAPE::SHAPE_BOX,
        qreal _size1 = 0.0,
        qreal _size2 = 0.0,
        qreal _size3 = 0.0,
        qreal _size4 = 0.0,
        qreal _size5 = 0.0,
        qreal _size6 = 0.0,
        PARTINGLEN _plen = PARTINGLEN::PART_FULL,
        PARTINGRULE _prule = PARTINGRULE::PART_RATIO,
        qreal _ilen = 1500.0,
        qreal _jlen = 1500.0,
        qreal _iratio = 0.25,
        qreal _jratio = 0.25,
            qreal _size11 = 0.0,
            qreal _size12 = 0.0,
            qreal _size13 = 0.0,
            qreal _size14 = 0.0,
            qreal _size15 = 0.0,
            qreal _size16 = 0.0
            )
        : sectionName(_sname), baseName(_bname), renderType(_type),
          leftSizeB(_size1), leftSizeH(_size2),
          centerSizeB(_size3), centerSizeH(_size4),
          rightSizeB(_size5), rightSizeH(_size6),
          partingLength(_plen), partingRule(_prule),
          iLength(_ilen), jLength(_jlen),
          iRatio(_iratio), jRatio(_jratio),
          leftSizeB_T(_size11), leftSizeH_T(_size12),
          centerSizeB_T(_size13), centerSizeH_T(_size14),
          rightSizeB_T(_size15), rightSizeH_T(_size16)
    { }

    QString sectionName;        // 断面符号
    QString baseName;           // 柱脚符号
    RENDERSHAPE renderType;     // 断面タイプ
    qreal leftSizeB;            //　左端　幅(mm)
    qreal leftSizeH;            // 左端　高さ(mm)
    qreal centerSizeB;          // 中央 幅(mm)
    qreal centerSizeH;          // 中央 高さ(mm)
    qreal rightSizeB;           // 右端 幅(mm)
    qreal rightSizeH;           // 右端 高さ(mm)
    qreal leftSizeB_T;            //　左端　幅(mm)
    qreal leftSizeH_T;            // 左端　高さ(mm)
    qreal centerSizeB_T;          // 中央 幅(mm)
    qreal centerSizeH_T;          // 中央 高さ(mm)
    qreal rightSizeB_T;           // 右端 幅(mm)
    qreal rightSizeH_T;           // 右端 高さ(mm)

    PARTINGLEN partingLength;   // 端部長さの基準（全長、内法）
    PARTINGRULE
    partingRule;    // 端部長さの決め方（デフォルト、値指定、比率指定）
    qreal iLength;              // i端 端部長さ（mm）
    qreal jLength;              // j端　端部長さ（mm）
    qreal iRatio;               // i端 端部長さの比率
    qreal jRatio;               // j端　端部長さの比率
};


struct POST3D_CLASS_DLLSPEC BASEVALUES {
    explicit BASEVALUES(
        const QString &_name = QString(),
        qreal _bb = 0.0,
        qreal _dd = 0.0,
        qreal _tt = 0.0
    )
        : sectionName(_name), sizeB(_bb),
          sizeD(_dd), sizeT(_tt) { }

    QString sectionName;
    qreal sizeB;
    qreal sizeD;
    qreal sizeT;
};

struct POST3D_CLASS_DLLSPEC LINEPROPERTY {  // 部材リストからの情報 線材
    explicit LINEPROPERTY(
        const QString &_sname = QString(),
        const QString &_bname = QString(),
        STRUCTTYPE _stype = STRUCTTYPE::NOSTRUCTTYPE,
        RENDERSHAPE _itype = RENDERSHAPE::SHAPE_BOX,
        RENDERSHAPE _ctype = RENDERSHAPE::SHAPE_BOX,
        RENDERSHAPE _jtype = RENDERSHAPE::SHAPE_BOX,
        const QString &_imat = QString(),
        const QString &_cmat = QString(),
        const QString &_jmat = QString(),
        qreal _isize1 = 0.0,
        qreal _isize2 = 0.0,
        qreal _isize3 = 0.0,
        qreal _isize4 = 0.0,
        qreal _csize1 = 0.0,
        qreal _csize2 = 0.0,
        qreal _csize3 = 0.0,
        qreal _csize4 = 0.0,
        qreal _jsize1 = 0.0,
        qreal _jsize2 = 0.0,
        qreal _jsize3 = 0.0,
        qreal _jsize4 = 0.0 )
        : sectionName(_sname), baseName(_bname), structType(_stype),
          i_renderType(_itype), c_renderType(_ctype), j_renderType(_jtype),
          i_material(_imat), c_material(_cmat), j_material(_jmat),
          i_B(_isize1), i_H(_isize2), i_t1(_isize3), i_t2(_isize4),
          c_B(_csize1), c_H(_csize2), c_t1(_csize3), c_t2(_csize4),
          j_B(_jsize1), j_H(_jsize2), j_t1(_jsize3), j_t2(_jsize4) { }

    QString sectionName;        // 断面符号
    QString baseName;           // 柱脚符号
    STRUCTTYPE structType;      // 種別
    RENDERSHAPE i_renderType;   // i 断面タイプ
    RENDERSHAPE c_renderType;   // c 断面タイプ
    RENDERSHAPE j_renderType;   // j 断面タイプ
    QString i_material;         // i 材種
    QString c_material;         // c 材種
    QString j_material;         // j 材種
    qreal i_B;                  //　i　幅(mm)
    qreal i_H;                  // i　高さ(mm)
    qreal i_t1;                 //　i　t1(mm)
    qreal i_t2;                 // i　t2(mm)
    qreal c_B;                  //　c　幅(mm)
    qreal c_H;                  // c　高さ(mm)
    qreal c_t1;                 //　c　t1(mm)
    qreal c_t2;                 // c　t2(mm)
    qreal j_B;                  //　j　幅(mm)
    qreal j_H;                  // j　高さ(mm)
    qreal j_t1;                 //　j　t1(mm)
    qreal j_t2;                 // j　t2(mm)
};

struct POST3D_CLASS_DLLSPEC PLANEPROPERTY {  // 部材リストからの情報　面材
    explicit PLANEPROPERTY(
        const QString &_sname = QString(),
        PLANEMODEL _model = PLANEMODEL::NONSTRUCT_MODEL,
        PLANESUBMODEL _smodel = PLANESUBMODEL::NONSTRUCT_SUBMODEL,
        qreal _thick = 0.0,
            STRUCTTYPE _structType = STRUCTTYPE::NOSTRUCTTYPE)
        : sectionName(_sname), modelType(_model), modelSubType(_smodel), thickness(_thick),structType(_structType) { }

    QString sectionName;        // 断面符号
    PLANEMODEL modelType;       // モデル化
    PLANESUBMODEL modelSubType;       // モデル化
    qreal thickness;            //　厚さ(mm)
    STRUCTTYPE structType;      // 種別
};


struct POST3D_CLASS_DLLSPEC SECTIONWEIGHT {          // 断面ごと単位荷重 kN/m kN/m^2
    explicit SECTIONWEIGHT(
        qreal _iw = 0.0,
        qreal _cw = 0.0,
        qreal _jw = 0.0 )
        : i_weight(_iw), c_weight(_cw), j_weight(_jw) { }

    qreal i_weight;   // i端の単位長さ当たり重量kN/m またま　単位面積当たり重量kN/m^2
    qreal c_weight;
    qreal j_weight;
};


/* 荷重種類 */
enum class NODEORFACE {NODE_SECTION = 0, FACE_SECTION};


/* 荷重種類 */
class POST3D_CLASS_DLLSPEC LoadType{
public:
    enum LOADTYPE {LTYPE_DL = 0, LTYPE_LL1, LTYPE_LL2, LTYPE_LL3, LTYPE_LL4, LTYPE_LLE, LTYPE_LL0};
    static LOADTYPE stringToEnum(const QString&);
    static QString enumToString(const LOADTYPE&);
};

/* 荷重座標系（部材荷重）　..全体座標系、部材座標系 */
enum class LCORSYSTEM {LCOR_GLOBAL = 0, LCOR_LOCAL};
/* 荷重方向 ..X,Y,Z */
enum class LOADDIR {LDIR_X = 0, LDIR_Y, LDIR_Z, LDIR_MX, LDIR_MY, LDIR_MZ};


/* 節点荷重 */

struct POST3D_CLASS_DLLSPEC JOINTLOADDIM {
    explicit JOINTLOADDIM(
        qreal _fx = 0.0,
        qreal _fy = 0.0,
        qreal _fz = 0.0,
        qreal _mx = 0.0,
        qreal _my = 0.0,
        qreal _mz = 0.0,
        const QString &_notes = QString()
    )
        : fx(_fx), fy(_fy), fz(_fz),
          mx(_mx), my(_my), mz(_mz), notes(_notes) { }

    JOINTLOADDIM Add(const JOINTLOADDIM& other){
        return JOINTLOADDIM(fx + other.fx, fy + other.fy, fz + other.fz, mx + other.mx, my + other.my, mz + other.mz, notes + other.notes);
    }
    qreal fx;       // kN
    qreal fy;
    qreal fz;
    qreal mx;       // kN・m
    qreal my;
    qreal mz;
    QString notes;  // コメント

    QString writeData() const{
        return QString::number(fx) + ","
                + QString::number(fy) + ","
                + QString::number(fz) + ","
                + QString::number(mx) + ","
                + QString::number(my) + ","
                + QString::number(mz) + ","
                + notes;
    }
};

struct POST3D_CLASS_DLLSPEC JOINTLOADVALUES {
    explicit JOINTLOADVALUES(
        const QString &_name = QString(),
        const QHash<LoadType::LOADTYPE, JOINTLOADDIM> &_values = QHash<LoadType::LOADTYPE, JOINTLOADDIM>()
    )
        : loadName(_name), loadTable(_values) { }

    QString loadName;
    QHash<LoadType::LOADTYPE, JOINTLOADDIM> loadTable;
    QString writeData() const{
        QString text;
        QHashIterator<LoadType::LOADTYPE, JOINTLOADDIM> i(loadTable);
        while (i.hasNext()) {
            i.next();
            text.append(loadName).append(",");
            text.append(QString::number(i.key())).append(",");
            text.append(i.value().writeData());
        }
        return text;
    }
};

/* 部材荷重 */

struct POST3D_CLASS_DLLSPEC MEMBERLOADDIM {
    explicit MEMBERLOADDIM(
        LCORSYSTEM _cor = LCORSYSTEM::LCOR_GLOBAL,
        LOADDIR _dir = LOADDIR::LDIR_Z,
        qreal _wa = 0.0,
        qreal _wb = 0.0,
        const QString &_notes = QString()
    )
        : corSytem(_cor), loadDir(_dir), wa(_wa), wb(_wb), notes(_notes) { }

    LCORSYSTEM corSytem; // 座標系
    LOADDIR loadDir;    // 荷重方向
    qreal wa;           // kN/m
    qreal wb;
    QString notes;      // コメント
    QString writeData() const{
        return QString::number(static_cast<int>(corSytem)) + ","
                + QString::number(static_cast<int>(loadDir)) + ","
                + QString::number(wa) + ","
                + QString::number(wb) + ","
                + notes;
    }};

struct POST3D_CLASS_DLLSPEC MEMBERLOADVALUES {
    explicit MEMBERLOADVALUES(
        const QString &_name = QString(),
        const QHash<LoadType::LOADTYPE, MEMBERLOADDIM> &_values = QHash<LoadType::LOADTYPE, MEMBERLOADDIM>()
    )
        : loadName(_name), loadTable(_values) { }

    QString loadName;
    QHash<LoadType::LOADTYPE, MEMBERLOADDIM> loadTable;
    QString writeData() const{
        QString text;
        QHashIterator<LoadType::LOADTYPE, MEMBERLOADDIM> i(loadTable);
        while (i.hasNext()) {
            i.next();
            text.append(loadName).append(",");
            text.append(QString::number(i.key())).append(",");
            text.append(i.value().writeData());
        }
        return text;
    }};

/* 床荷重 */

struct POST3D_CLASS_DLLSPEC SLABLOADVALUES {
    explicit SLABLOADVALUES(
        const QString &_name = QString(),
        const QString &_notes = QString(),
        qreal _dl = 0.0,
        qreal _ll0 = 0.0,
        qreal _ll1 = 0.0,
        qreal _ll2 = 0.0,
        qreal _ll3 = 0.0,
        qreal _ll4 = 0.0,
        qreal _lle = 0.0
    )
        : loadName(_name), notes(_notes),
          sDL(_dl), sLL0(_ll0), sLL1(_ll1), sLL2(_ll2), sLL3(_ll3), sLL4(_ll4), sLLE(_lle), sLLUnit(1.0) { }

    QString loadName;
    QString notes;  // コメント
    qreal sDL;      // N/m^2
    qreal sLL0;
    qreal sLL1;
    qreal sLL2;
    qreal sLL3;
    qreal sLL4;
    qreal sLLE;
    qreal sLLUnit; // NB3D連携用
    QString writeData() const{
        return loadName
                + "," + QString::number(sDL)
                + "," + QString::number(sLL0)
                + "," + QString::number(sLL1)
                + "," + QString::number(sLL2)
                + "," + QString::number(sLL3)
                + "," + QString::number(sLL4)
                + "," + QString::number(sLLE)
                + "," + notes;
    };
};




/* ＸＹＺ座標 */

struct POST3D_CLASS_DLLSPEC XYZ {
    explicit XYZ( qreal _x = 0.0, qreal _y = 0.0, qreal _z = 0.0 )
        : x(_x), y(_y), z(_z) { }

    qreal x;
    qreal y;
    qreal z;
    XYZ operator+(const XYZ &other) const
    {
        return XYZ(x + other.x, y + other.y, z + other.z);
    }
    XYZ operator-(const XYZ &other) const
    {
        return XYZ(x - other.x, y - other.y, z - other.z);
    }
    XYZ operator*(const qreal &coef) const
    {
        return XYZ(x * coef, y * coef, z * coef);
    }
};

/* 全体座標系　２点 */

struct POST3D_CLASS_DLLSPEC GLOBALLINE {
    explicit GLOBALLINE( const GlobalPoint &_p1 = GlobalPoint(),
                         const GlobalPoint &_p2 = GlobalPoint() )
        : p1(_p1), p2(_p2) { }

    GlobalPoint p1;
    GlobalPoint p2;
};

/* 全体編集の選択制限 */

struct POST3D_CLASS_DLLSPEC LIMITSELECTTERM {
    explicit LIMITSELECTTERM(
        bool _limit = false,
        bool _limitfloor = false,
        bool _flcontain = true,
        const QList<QUuid> &_fls = QList<QUuid>(),
        bool _limitframe = false,
        bool _frcontain = true,
        const QList<QUuid> &_frs = QList<QUuid>(),
        bool _limittype = false,
        const QList<DATATYPE> &_types = QList<DATATYPE>(),
        bool _limitname = false,
        const QStringList &_names = QStringList()
    )
        : isSelectLimited(_limit),
          isLimitFloor(_limitfloor), isFloorContained(_flcontain), limitedFloors(_fls),
          isLimitFrame(_limitframe), isFrameContained(_frcontain), limitedFrames(_frs),
          isLimitDataType(_limittype), limitedDataTypes(_types),
          isLimitName(_limitname), limitedNames(_names) { }

    bool isSelectLimited;
    bool isLimitFloor;
    bool isFloorContained;
    QList<QUuid> limitedFloors;
    bool isLimitFrame;
    bool isFrameContained;
    QList<QUuid> limitedFrames;
    bool isLimitDataType;
    QList<DATATYPE> limitedDataTypes;
    bool isLimitName;
    QStringList limitedNames;

};

/* 階コピー操作の諸元 */

struct POST3D_CLASS_DLLSPEC FLOORCOPYTERM {
    explicit FLOORCOPYTERM(
        const QUuid &_id = QUuid(),
        const QList<QUuid> &_ids = QList<QUuid>(),
        bool _xybase = true,
        bool _upper = true,
        bool _joverwrite = true,
        bool _moverwrite = true,
        bool _limitframe = false,
        const QList<QUuid> &_frs = QList<QUuid>(),
        bool _limittype = false,
        const QList<DATATYPE> &_types = QList<DATATYPE>(),
        bool _limitxy = false,
        const QPointF &_min_xy = QPointF(0.0, 0.0),
        const QPointF &_max_xy = QPointF(0.0, 0.0)
    )
        : sourceFloorID(_id), destFloors(_ids),
          isXYBase(_xybase), isCopyUpperFloor(_upper),
          isOverwriteJoint(_joverwrite), isOverwriteMember(_moverwrite),
          isLimitFrame(_limitframe), limitedFrames(_frs),
          isLimitDataType(_limittype), limitedDataTypes(_types),
          isLimitXYValue(_limitxy), minimumXY(_min_xy), maximumXY(_max_xy) { }

    QUuid sourceFloorID;
    QList<QUuid> destFloors;
    bool isXYBase;
    bool isCopyUpperFloor;
    bool isOverwriteJoint;
    bool isOverwriteMember;
    bool isLimitFrame;
    QList<QUuid> limitedFrames;
    bool isLimitDataType;
    QList<DATATYPE> limitedDataTypes;
    bool isLimitXYValue;
    QPointF minimumXY;
    QPointF maximumXY;
};

/* 節点番号ソート */

enum class JOINTSORTPATTERN { SORTPT_NONE = 0, SORTPT_X, SORTPT_Y, SORTPT_XY };

struct POST3D_CLASS_DLLSPEC JOINTSORTTERM {
    explicit JOINTSORTTERM(
        bool _1storder = true,
        JOINTSORTPATTERN _2thpattern = JOINTSORTPATTERN::SORTPT_NONE,
        bool _2thorder = true,
        JOINTSORTPATTERN _3rdpattern = JOINTSORTPATTERN::SORTPT_NONE,
        bool _3rdorder = true )
        : firstOrder(_1storder),
          secondPattern(_2thpattern), secondOrder(_2thorder),
          thirdPattern(_3rdpattern), thirdOrder(_3rdorder) { }

    bool firstOrder;
    JOINTSORTPATTERN secondPattern;
    bool secondOrder;
    JOINTSORTPATTERN thirdPattern;
    bool thirdOrder;
};

/* 応力組み合わせ設定 */
struct POST3D_CLASS_DLLSPEC STRESSCOMBISETTINGS {
    explicit STRESSCOMBISETTINGS(){}
    QStringList combi;
    QHash<QString,QList<qreal>> coef;
    QHash<QString,QStringList> stress;
    QList<int> step;

    static STRESSCOMBISETTINGS GetDefault(){
        qDebug()<<"getDefault";
        STRESSCOMBISETTINGS set;
        set.combi<<u8"CaseDL"<<u8"CaseE1"<<u8"CaseE2"
            <<u8"CaseU1"<<u8"CaseU2"<<u8"CaseU3"<<u8"CaseU4"<<u8"CaseU5"<<u8"CaseU6"<<u8"CaseU7"<<u8"CaseU8";
        set.step<<0<<1<<1<<3<<3<<3<<3<<3<<3<<3<<3;
        set.coef.insert(set.combi.at(0),QList<qreal>()<<1.0<<1.0);
        set.coef.insert(set.combi.at(1),QList<qreal>()<<1.0);
        set.coef.insert(set.combi.at(2),QList<qreal>()<<1.0);
        set.coef.insert(set.combi.at(3),QList<qreal>()<<1.0<<1.0<<1.0);
        set.coef.insert(set.combi.at(4),QList<qreal>()<<1.0<<1.0<<1.0);
        set.coef.insert(set.combi.at(5),QList<qreal>()<<1.0<<1.0<<1.0);
        set.coef.insert(set.combi.at(6),QList<qreal>()<<1.0<<1.0<<1.0);
        set.coef.insert(set.combi.at(7),QList<qreal>()<<1.0<<1.0<<1.0);
        set.coef.insert(set.combi.at(8),QList<qreal>()<<1.0<<1.0<<1.0);
        set.coef.insert(set.combi.at(9),QList<qreal>()<<1.0<<1.0<<1.0);
        set.coef.insert(set.combi.at(10),QList<qreal>()<<1.0<<1.0<<1.0);
        set.stress.insert(set.combi.at(0),QStringList()<<u8"RSTDL"<<u8"RSTLL1");
        set.stress.insert(set.combi.at(1),QStringList()<<u8"RSTE1");
        set.stress.insert(set.combi.at(2),QStringList()<<u8"RSTE2");
        set.stress.insert(set.combi.at(3),QStringList()<<u8"RSTDL"<<u8"RSTLL1"<<u8"RSTU1");
        set.stress.insert(set.combi.at(4),QStringList()<<u8"RSTDL"<<u8"RSTLL1"<<u8"RSTU2");
        set.stress.insert(set.combi.at(5),QStringList()<<u8"RSTDL"<<u8"RSTLL1"<<u8"RSTU3");
        set.stress.insert(set.combi.at(6),QStringList()<<u8"RSTDL"<<u8"RSTLL1"<<u8"RSTU4");
        set.stress.insert(set.combi.at(7),QStringList()<<u8"RSTDL"<<u8"RSTLL1"<<u8"RSTU5");
        set.stress.insert(set.combi.at(8),QStringList()<<u8"RSTDL"<<u8"RSTLL1"<<u8"RSTU6");
        set.stress.insert(set.combi.at(9),QStringList()<<u8"RSTDL"<<u8"RSTLL1"<<u8"RSTU7");
        set.stress.insert(set.combi.at(10),QStringList()<<u8"RSTDL"<<u8"RSTLL1"<<u8"RSTU8");
        return set;
    }
};

/* 解析用グループ定義*/
class POST3D_CLASS_DLLSPEC GroupSetting{
public:
    explicit GroupSetting(const QString& _group, const QList<QUuid>& _list, const QString& _comment)
        :group(_group), list(_list), comment(_comment){}
    QString group;
    QList<QUuid> list;
    QString comment;
    bool isSame(const GroupSetting& value) const{
        if (group != value.group)
            return false;

        if (list.count() != value.list.count())
            return false;

        foreach(auto l, list){
            if (!value.list.contains(l))
                return false;
        }

        if (comment != value.comment)
            return false;

        return true;
    }
};
} // namespace post3dapp

#endif
