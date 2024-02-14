#ifndef DEFINE_SECTION_STRUCT_H
#define DEFINE_SECTION_STRUCT_H

#include <QDebug>
#include <QUuid>

#include "define_bar_arrangement.h"
#include "define_unifieddata.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class SectionDataType;
class N3DUuidTable;
/* タイプ定義 */

class POST3D_CLASS_DLLSPEC SteelType
{
public:
    enum class Type {UNDEFINED = -1,
               ST_H = 0, ST_I, ST_BX, ST_P, ST_T1, ST_T2, ST_T3, ST_T4, ST_C1, ST_C2,ST_C3,ST_C4,ST_L,
               ST_WH, ST_WI, ST_WBX, ST_WP, ST_WT1, ST_WT2, ST_WT3, ST_WT4, ST_WC1, ST_WC2, ST_WC3, ST_WC4, ST_WPL, ST_WBR,
               ST_R, ST_BR, ST_CROSS, ST_CROSSH
              };
    static Type stringToEnum(const QString &str);
    static QString enumToString(Type argenum);
    static bool isStandardType(Type argenum);
    static bool isSideways(Type type);
};

class POST3D_CLASS_DLLSPEC ConcreteType
{
public:
    enum class Type {UNDEFINED = -1, RECTANGLE = 0, CIRCLE};
};

class POST3D_CLASS_DLLSPEC SectionPart
{
public:
    enum class Part {UNDEFINED = -1, FLANGE = 0, WEB, TOP, MIDDLE, BOTTOM, EDGE, CENTER};
};

class POST3D_CLASS_DLLSPEC SectionRank
{
public:
    enum class Rank {UNDEFINED = -1, FA = 0, FB, FC, FD};
    static Rank stringToEnum(const QString &str);
    static QString enumToString(Rank argenum);
};

class POST3D_CLASS_DLLSPEC PlaneType
{
public:
    enum class Type {UNDEFINED = -1, NONSTRUCT = 0, WALL, ZWALL, PLATE, MEMBRANE};
    static Type stringToEnum(const QString &str);
    static QString enumToString(const Type&);
};
class POST3D_CLASS_DLLSPEC PlaneType2
{
public:
    enum class Type {UNDEFINED = -1, WH = 0, WI, SH, SI};
    static Type stringToEnum(const QString &str);
};

class POST3D_CLASS_DLLSPEC PlanePart
{
public:
    enum class Part {UNDEFINED = -1, A_PART = 0, B_PART, AB_PART};
};

class POST3D_CLASS_DLLSPEC PlaneSide
{
public:
    enum class Side {UNDEFINED = -1, UPPER = 0, LOWER, OUTER, INNER, LEFT, RIGHT, CENTER};
    static Side stringToEnum(const QString &str);
    static QString enumToString(Side argenum);
};

class POST3D_CLASS_DLLSPEC PlaneDirection
{
public:
    enum class Direction {UNDEFINED = -1, VERTICAL = 0, HORIZONTAL, DIAGONAL, SHORTSIDE, LONGSIDE};
};

class POST3D_CLASS_DLLSPEC WallOpenType
{
public:
    enum class Type {UNDEFINED = -1, NORMAL = 0, SLIT, ANALYSIS};
    enum class VPosition{BOTTOM, MIDDLE, TOP};
    enum class HPosition{LEFT, CENTER, RIGHT};
    static Type stringToEnumType(const QString &str);
    static QString enumToStringType(Type);
    static HPosition stringToEnumH(const QString &str);
    static QString enumToStringH(HPosition);
    static VPosition stringToEnumV(const QString &str);
    static QString enumToStringV(VPosition);
};

class POST3D_CLASS_DLLSPEC WallOpenSize
{
public:
    enum class Size {UNDEFINED = -1, SMALL = 0, LARGE};
};

class POST3D_CLASS_DLLSPEC BaseBoltType
{
public:
    enum class Type {UNDEFINED = -1, RECTANGLE = 0, PARALLEL_LR = 1, PARALLEL_UD = 2, DIAMOND = 3, CIRCLE = 4};
    static Type stringToEnum(const QString &str);
    static QString enumToString(Type);
private:
    static const QString Undefined;
    static const QString Rectangle;
    static const QString ParallelLR;
    static const QString ParallelUD;
    static const QString Diamond;
    static const QString Circle;
};

class POST3D_CLASS_DLLSPEC BasePlateType
{
public:
    enum class Type {UNDEFINED = -1, RECTANGLE = 0, OCTAGON = 1,
               DTYPE_L = 2, DTYPE_R = 3, DTYPE_D = 4, DTYPE_U = 5, CIRCLE = 6
              };
    static Type stringToEnum(const QString &str);
};

class POST3D_CLASS_DLLSPEC SpringType
{
public:
    enum class Type {UNDEFINED = -1, LINEAR = 1, REVERSIBLE_BILINEAR = 2, BILINEAR = 3,
               TRILINEAR = 4, TENSILE_REVERSIBLE_BILINEAR = 8, TENSILE_REVERSIBLE_TRILINEAR = 9,
                     DEGRADING,SLIP,ORIGIN_ORIENTED,TAKEDA,OKAMOTO,DEGRADING_K4eqK3,DEGRADING_K4eq0};
    static Type stringToEnum(const QString &str);
};

class POST3D_CLASS_DLLSPEC DamperType
{
public:
    enum class Type {UNDEFINED = -1, TYPE01 = 1, TYPE02 = 2, TYPE03 = 3, TYPE04 = 4, TYPE05 = 5,
               TYPE06 = 6, TYPE07 = 7, TYPE08 = 8, TYPE09 = 9, TYPE21 = 21, TYPE22 = 22,
               TYPE23 = 23, TYPE24 = 24, TYPE25 = 25, TYPE26 = 26, TYPE27 = 27, TYPE28 = 28,
               TYPE29 = 29, TYPE30 = 30, TYPE31 = 31, TYPE32 = 32, TYPE33 = 33, TYPE34 = 34
              };
    static Type stringToEnum(const QString &str);
    static QString enumToString(Type t);
};

/* 断面パラメーター */

struct POST3D_CLASS_DLLSPEC LineCommonParameter {
    explicit LineCommonParameter(
        qreal finish_ = 0.0,
        bool canti_ = false,
        const QString &conc_ = QString(),
        const QString &fconc_ = QString(),
        qreal ilen_ = 0.0,
        qreal jlen_ = 0.0,
        qreal ihan_ = 0.0,
        qreal jhan_ = 0.0,
        bool istiff_ = false,
        bool jstiff_ = false,
        const QString &base_ = QString(),
        bool comp_ = false,
        qreal directa_ = 0.0,
        qreal directae_ = 0.0,
        qreal directratio_ = 0.0,
        qreal directmass_ = 0.0 )
        : finishLoad(finish_), isCantilever(canti_),
          concMaterial(conc_), filledConcMaterial(fconc_),
          i_length(ilen_), j_length(jlen_), i_haunch(ihan_), j_haunch(jhan_),
          i_hasStiffener(istiff_), j_hasStiffener(jstiff_), basePlate(base_), hasCompResistance(comp_),
          direct_area(directa_), direct_effectiveArea(directae_),
          direct_slenderRatio(directratio_), direct_unitmass(directmass_) {}

    qreal finishLoad;                   // 仕上げ重量 N/m
    bool isCantilever;                  // 一般／片持ち

    QString concMaterial;               // コンクリート材種
    QString filledConcMaterial;         // 中詰コンクリート材種

    qreal i_length;                     // i端長さ　mm　->RCは内法Lo/4で固定
    qreal j_length;                     // j端長さ　mm 片持ちの場合無視 ->RCは内法Lo/4で固定
    qreal i_haunch;                     // i端ハンチ長さ　mm
    qreal j_haunch;                     // j端ハンチ長さ　mm
    bool i_hasStiffener;                // i端補剛あり
    bool j_hasStiffener;                // j端補剛あり

    QString basePlate;                  // 柱脚リスト名

    bool hasCompResistance;             // 圧縮抵抗性
    qreal direct_area;                  // ブレース 直接入力　断面積 cm^2
    qreal direct_effectiveArea;         // 有効断面積 cm^2
    qreal direct_slenderRatio;          // 細長比
    qreal direct_unitmass;              // 単位重量 N/m
};

struct POST3D_CLASS_DLLSPEC LinePartsParameterS {
    explicit LinePartsParameterS(
            const QString &material_ = QString(),
            const QString &webmaterial_ = QString(),
            SteelType::Type steeltype_ = SteelType::Type::UNDEFINED,
            const QString &standardsteel_ = QString(),
            qreal H_ = 0.0,
            qreal B_ = 0.0,
            qreal t1_ = 0.0,
            qreal t2_ = 0.0,
            qreal H_T_ = 0,
            qreal B_T_ = 0,
            qreal t1_T_ = 0,
            qreal t2_T_ = 0
            )
        : steelMaterial(material_),steelWebMaterial(webmaterial_), steelType(steeltype_), standardName(standardsteel_), sizeH(H_), sizeB(B_), t1(t1_), t2(t2_),
          H_T(H_T_), B_T(B_T_), t1_T(t1_T_), t2_T(t2_T_)
            {}

    QString steelMaterial;              // 鉄骨材種
    QString steelWebMaterial;              // 鉄骨材種
    SteelType::Type steelType;          // 鉄骨タイプ
    QString standardName;               // 規格断面名
    qreal sizeH;                        // H mm
    qreal sizeB;                        // B mm
    qreal t1;                           // t1 mm
    qreal t2;                           // t2 mm
    //直交方向鉄骨の諸元
    qreal H_T;                              // H mm
    qreal B_T;                              // B mm
    qreal t1_T;                             // t1 mm
    qreal t2_T;                             // t2 mm

    qreal standardArea() const;         // 規格断面の場合の断面積 mm2
    qreal getSizeB() const; // ○かどうか調べてH, Bのどちらかを返す。
    qreal getSizeB_T() const; // ○かどうか調べてH, Bのどちらかを返す。
};

struct POST3D_CLASS_DLLSPEC LinePartsParameterRC {
    explicit LinePartsParameterRC(
        ConcreteType::Type conctype_ = ConcreteType::Type::UNDEFINED,
        ConcreteType::Type axialconctype_ = ConcreteType::Type::UNDEFINED,
        qreal Dy_ = 0, qreal Dz_ = 0,
        const BarArrangement &mainbar1_ = BarArrangement(),
        const QString &mainbarPhi_ = QString(),
        const BarArrangement &mainbar2_ = BarArrangement(),
        const QString &mainbarPhi2_ = QString(),
        qreal axialCenterY_ = 0, qreal axialCenterZ_ = 0,
        qreal axialDy_ = 0, qreal axialDz_ = 0,
        const BarArrangement &axialbar1_ = BarArrangement(),
        const BarArrangement &axialbar2_ = BarArrangement(),
        const QString &axialbarPhi_ = QString(),
        int hoopY_ = 0, int hoopZ_ = 0,
        const QString &hoopPhi_ = QString(), int hooppitch_ = 0,
        qreal RK_ = 70, qreal RKu_ = 70, qreal RKb_ = 70, qreal RY_ = 70, qreal RY1_ = 70,
        bool bar_readok_ = true)
        : concType(conctype_), axialConcType(axialconctype_), Dy(Dy_), Dz(Dz_),
          mainBarArrange1(mainbar1_), mainBar1(mainbarPhi_), mainBarArrange2(mainbar2_), mainBar2(mainbarPhi2_),
          axialCenterY(axialCenterY_), axialCenterZ(axialCenterZ_),
          axialDy(axialDy_), axialDz(axialDz_),
          axialBarArrange1(axialbar1_), axialBarArrange2(axialbar2_), axialBar(axialbarPhi_),
          hoopY(hoopY_), hoopZ(hoopZ_), hoopBar(hoopPhi_), hoopPitch(hooppitch_),
          RK(RK_), RKu(RKu_), RKb(RKb_), RY(RY_), RY1(RY1_), bar_readOK(bar_readok_) {}

    ConcreteType::Type concType;        // コンクリートタイプ
    ConcreteType::Type axialConcType;   // 芯鉄筋の並び形状

    //主筋
    qreal Dy;                           // Dx mm
    qreal Dz;                           // Dy mm
    BarArrangement mainBarArrange1;     // 上筋の1段目2段目・・・ or X方向の左・右
    BarArrangement mainBarArrange2;     // 下筋　or Y方向　同上
    QString mainBar1;                    // 主筋径　mm
    QString mainBar2;                    // 主筋径　mm

    //芯鉄筋
    qreal axialCenterY;                 // 芯鉄筋位置 X mm
    qreal axialCenterZ;                 // 芯鉄筋位置 Y mm
    qreal axialDy;                      // 芯鉄筋の並び形状　Dx mm
    qreal axialDz;                      // 芯鉄筋の並び形状 Dy mm
    BarArrangement axialBarArrange1;    // X方向芯鉄筋
    BarArrangement axialBarArrange2;    // Y方向芯鉄筋
    QString axialBar;                   // 芯鉄筋径 mm

    //フープ筋
    int hoopY;                          // 左右方向 フープ本数
    int hoopZ;                          // 上下方向 フープ本数
    QString hoopBar;                    // フープ筋径
    int hoopPitch;                      // フープ筋ピッチ mm

    //鉄筋間距離
    qreal RK;                           // 部材表面-主筋芯の距離 mm
    qreal RKu;                          // 部材表面-主筋芯の距離（上面） mm
    qreal RKb;                          // 部材表面-主筋芯の距離（下面） mm
    qreal RY;                           // 寄せ筋・段筋の距離(芯芯) mm
    qreal RY1;                          // 段筋の距離(芯芯、一段目) mm

    //鉄筋チェック用
    bool bar_readOK;                    // 主筋定義が解読できたか否か

    QList<QPointF> barPointList(DATATYPE mtype, bool isAxialBar = false) const;
};

struct POST3D_CLASS_DLLSPEC LinePartsParameterSRC : public virtual LinePartsParameterRC,
    public virtual LinePartsParameterS {
    explicit LinePartsParameterSRC(
        const QString &material_ = QString(),
        const QString &webmaterial_ = QString(),
        const QString &standardName_ = QString(),
        SteelType::Type steelshape_ = SteelType::Type::UNDEFINED,
        qreal H_ = 0,
        qreal B_ = 0,
        qreal t1_ = 0,
        qreal t2_ = 0,
        qreal coordinateX_ = 0,
        qreal coordinateZ_ = 0,
            const QString &standardName_T_ = QString(),
            SteelType::Type steelshape_T_ = SteelType::Type::UNDEFINED,
            qreal H_T_ = 0,
            qreal B_T_ = 0,
            qreal t1_T_ = 0,
            qreal t2_T_ = 0,

        qreal coordinateY_T_ = 0,
        qreal coordinateZ_T_ = 0,

        ConcreteType::Type concshape_ = ConcreteType::Type::UNDEFINED,
        ConcreteType::Type axialconcshape_ = ConcreteType::Type::UNDEFINED,
        qreal Dy_ = 0, qreal Dz_ = 0,
        const BarArrangement &mainbar1_ = BarArrangement(),
        const QString &mainbarPhi_ = QString(),
        const BarArrangement &mainbar2_ = BarArrangement(),
        const QString &mainbarPhi2_ = QString(),
        qreal axialCenterY_ = 0, qreal axialCenterZ_ = 0,
        qreal axialDy_ = 0, qreal axialDz_ = 0,
        const BarArrangement &axialbar1_ = BarArrangement(),
        const BarArrangement &axialbar2_ = BarArrangement(),
        const QString &axialbarPhi_ = QString(),
        int hoopY_ = 0, int hoopZ_ = 0,
        const QString &hoopPhi_ = QString(), int hooppitch_ = 0,
        qreal RK_ = 70, qreal RKu_ = 70, qreal RKb_ = 70, qreal RY_ = 70, qreal RY1_ = 70,
        bool bar_readok_ = true)
        : LinePartsParameterRC(concshape_, axialconcshape_, Dy_, Dz_, mainbar1_, mainbarPhi_, mainbar2_, mainbarPhi2_,
                               axialCenterY_, axialCenterZ_, axialDy_, axialDz_, axialbar1_, axialbar2_, axialbarPhi_,
                               hoopY_, hoopZ_, hoopPhi_, hooppitch_, RK_, RKu_, RKb_, RY_, RY1_, bar_readok_),
          LinePartsParameterS(material_, webmaterial_, steelshape_, standardName_, H_, B_, t1_, t2_,H_T_,B_T_,t1_T_,t2_T_ ),
          steelType_T(steelshape_T_), standardName_T(standardName_T_), cY(coordinateX_), cZ(coordinateZ_),
          cY_T(coordinateY_T_), cZ_T(coordinateZ_T_){}

    //鉄骨位置
    SteelType::Type steelType_T;            // 鉄骨タイプ
    QString standardName_T;                 // 規格断面名
    qreal cY;                               // 鉄骨位置 X mm
    qreal cZ;                               // 鉄骨位置 Y mm
    qreal cY_T;                             // cX mm
    qreal cZ_T;                             // cY mm

    QString writeData(const DATATYPE &, const STRUCTTYPE &) const;
};

typedef LinePartsParameterSRC LinePartsParameter;//SRC inherits all line parameters

struct POST3D_CLASS_DLLSPEC PlaneSectionParameter {  //面部材
    struct Bar {
        explicit Bar(const QString &phi_ = QString(), int pitch_ = 200)
            : pitch(pitch_), phi(phi_) {}
        qreal pitch;
        QString phi;
    };

    struct OpeningBar {
        explicit OpeningBar(const QString &phi_ = QString(), int num_ = 0)
            : num(num_), phi(phi_) {}
        int num;
        QString phi;
    };

    explicit PlaneSectionParameter(
        qreal _finish = 0.0,
        const QString &conc_ = QString(),
        qreal concthickness_ = 0,
        qreal coveru_ = 0,
        qreal coverb_ = 0,
        const QString &steel_ = QString(),
        qreal steelthickness_ = 0,
        PlaneType::Type ptype_ = PlaneType::Type::UNDEFINED,
        PlaneType2::Type ptype2_ = PlaneType2::Type::UNDEFINED,
        bool isdouble_ = true,
        qreal loaddir_ = -1.0,
        const Bar &bar_ = Bar(),
        const OpeningBar &opening_bar_ = OpeningBar() )
        : finishLoad(_finish), concMaterial(conc_), concThickness(concthickness_),
          coverU(coveru_), coverD(coverb_),
          steelMaterial(steel_), steelThickness(steelthickness_),
          planeType(ptype_), planeType2(ptype2_), isDoubleBar(isdouble_),
          loadDir(loaddir_),
          vOpeningBar_small(opening_bar_), hOpeningBar_small(opening_bar_),
          dOpeningBar_small(opening_bar_),
          vOpeningBar_large(opening_bar_), hOpeningBar_large(opening_bar_),
          dOpeningBar_large(opening_bar_),
          vBar(bar_), hBar(bar_),
          vBar_innerTop(bar_), vBar_innerMiddle(bar_),
          vBar_innerBottom(bar_),
          vBar_outerTop(bar_), vBar_outerMiddle(bar_),
          vBar_outerBottom(bar_),
          hBar_innerEdge(bar_), hBar_innerCenter(bar_),
          hBar_outerEdge(bar_), hBar_outerCenter(bar_),
          sBar_upperEdgeA(bar_), sBar_lowerEdgeA(bar_),
          sBar_upperCenterA(bar_), sBar_lowerCenterA(bar_),
          sBar_upperEdgeCenterB(bar_), sBar_lowerEdgeCenterB(bar_),
          lBar_upperEdgeA(bar_), lBar_lowerEdgeA(bar_),
          lBar_upperCenterA(bar_), lBar_lowerCenterA(bar_),
          lBar_upperEdgeCenterB(bar_), lBar_lowerEdgeCenterB(bar_){}


    qreal finishLoad;               // 仕上げ重量 N/m^2
    QString concMaterial;           // コンクリート材種
    qreal concThickness;            // コンクリート厚 mm
    QString steelMaterial;          // 鉄骨材種
    qreal steelThickness;           // 鉄骨厚さ mm
    qreal coverU;                   // スラブ かぶり上 mm
    qreal coverD;                   // スラブ かぶり下 mm
    qreal loadDir;                  // スラブ　荷重方向　0～360度 それ以外なら亀の甲

    PlaneType::Type
    planeType;      // スラブ：板要素,膜要素,非構造　　壁：耐力壁,雑壁,非構造,平板シェル要素,平面応力要素
    PlaneType2::Type planeType2;    // 壁　WH,WI,SH,SI
    bool isDoubleBar;               // ダブル配筋 or シングル配筋
    Bar vBar;                       // 壁　タテ筋
    Bar hBar;                       // 壁　ヨコ筋

    OpeningBar vOpeningBar_small;   // 開口補強筋　小　タテ
    OpeningBar hOpeningBar_small;   // 開口補強筋　小　ヨコ
    OpeningBar dOpeningBar_small;   // 開口補強筋　小　ナナメ
    OpeningBar vOpeningBar_large;   // 開口補強筋　大　タテ
    OpeningBar hOpeningBar_large;   // 開口補強筋　大　ヨコ
    OpeningBar dOpeningBar_large;   // 開口補強筋　大　ナナメ

    Bar vBar_innerTop;              // 地下外壁　内側　上部　タテ筋
    Bar vBar_innerMiddle;           // 地下外壁　内側　中部　タテ筋
    Bar vBar_innerBottom;           // 地下外壁　内側　下部　タテ筋
    Bar vBar_outerTop;              // 地下外壁　外側　上部　タテ筋
    Bar vBar_outerMiddle;           // 地下外壁　外側　中部　タテ筋
    Bar vBar_outerBottom;           // 地下外壁　外側　下部　タテ筋
    Bar hBar_innerEdge;             // 地下外壁　内側　端部　ヨコ筋
    Bar hBar_innerCenter;           // 地下外壁　内側　中部　ヨコ筋
    Bar hBar_outerEdge;             // 地下外壁　外側　端部　ヨコ筋
    Bar hBar_outerCenter;           // 地下外壁　外側　中部　ヨコ筋

    Bar sBar_upperEdgeA;            // スラブ　短辺　上側　端部　Ａ
    Bar sBar_lowerEdgeA;            // スラブ　短辺　下側　端部　Ａ
    Bar sBar_upperCenterA;          // スラブ　短辺　上側　中央　Ａ
    Bar sBar_lowerCenterA;          // スラブ　短辺　下側　中央　Ａ
    Bar sBar_upperEdgeCenterB;      // スラブ　短辺　上側　端部中央　Ｂ
    Bar sBar_lowerEdgeCenterB;      // スラブ　短辺　下側　端部中央　Ｂ
    Bar lBar_upperEdgeA;            // スラブ　長辺　上側　端部　Ａ
    Bar lBar_lowerEdgeA;            // スラブ　長辺　下側　端部　Ａ
    Bar lBar_upperCenterA;          // スラブ　長辺　上側　中央　Ａ
    Bar lBar_lowerCenterA;          // スラブ　長辺　下側　中央　Ａ
    Bar lBar_upperEdgeCenterB;      // スラブ　長辺　上側　端部中央　Ｂ
    Bar lBar_lowerEdgeCenterB;      // スラブ　長辺　下側　端部中央　Ｂ
/*
    qreal vRib_pitch;               //　縦リブピッチ
    qreal vRib_type;                // 縦リブタイプ
    qreal vRib_thickness;           // 縦リブ厚
    qreal vRib_width;               // 縦リブ幅
    qreal hRib_pitch;               // 横リブピッチ
    qreal hRib_type;                // 横リブタイプ
    qreal hRib_thickness;           // 横リブ厚
    qreal hRib_width;               // 横リブ幅
*/
};

struct POST3D_CLASS_DLLSPEC BaseSectionParameter {
    explicit BaseSectionParameter(
        bool isstandard_ = false,
        const QString &productname_ = QString(),
        BasePlateType::Type baseplatetype_ = BasePlateType::Type::RECTANGLE,
        qreal B_ = 0,
        qreal D_ = 0,
        qreal T_ = 0,
        qreal SML_ = 0,
        BaseBoltType::Type boltarrangementtype_ = BaseBoltType::Type::RECTANGLE,
        int boltnx1_ = 0,
        int boltnx2_ = 0,
        int boltnz1_ = 0,
        int boltnz2_ = 0,
        int boltphi_ = 0,
        qreal edgedistancex1_ = 0,
        qreal edgedistancex2_ = 0,
        qreal edgedistancez1_ = 0,
        qreal edgedistancez2_ = 0,
        const QString &boltmaterial_ = QString(),
        const QString &baseplatematerial_ = QString(),
        const QString &conc_ = QString(),
        qreal boltholephi_ = 0,
        qreal screwphi_ = 0,
        bool hasElongationAbility_ = true,
        qreal anchoragearea_ = 0,
        qreal boltlength_ = 0,
        qreal Bc_ = 0,
        qreal Dc_ = 0,
        qreal H_ = 0,
        qreal projectedarea_ = 0,
        qreal B0_ = 0,
        qreal D0 = 0            )
        : isStandardType(isstandard_), productName(productname_), basePlateType(baseplatetype_),
          sizeB(B_), sizeD(D_), sizeT(T_), SML(SML_), boltType(boltarrangementtype_),
          bolt_ny1(boltnx1_), bolt_ny2(boltnx2_), bolt_nz1(boltnz1_), bolt_nz2(boltnz2_), boltPhi(boltphi_),
          edgehy1(edgedistancex1_), edgehy2(edgedistancex2_),
          edgehz1(edgedistancez1_), edgehz2(edgedistancez2_),
          boltMaterial(boltmaterial_), basePlateMaterial(baseplatematerial_), concMaterial(conc_),
          boltHolePhi(boltholephi_), screwPhi(screwphi_), hasElongationAbility(hasElongationAbility_),
          anchorArea(anchoragearea_), anchorLength(boltlength_),
          sizeBc(Bc_), sizeDc(Dc_), sizeH(H_), projectedarea(projectedarea_), B0(B0_), D0(D0) {}

    QList<QPointF> boltPointList() const;

//一般
    bool isStandardType;                    // 既製品か否か
    QString productName;                    // 型番
    QString boltMaterial;                   // ボルト材種
    QString basePlateMaterial;              //　ベースプレート材種
    QString concMaterial;                   // コンクリート材種

//BasePlate
    BasePlateType::Type basePlateType;      // ベースプレート形状タイプ
    qreal sizeB;                            // 幅 mm
    qreal sizeD;                            // せい mm
    qreal sizeT;                            // 厚さ mm
    qreal SML;                              // 隅切り長さ mm
    qreal boltHolePhi;                      // ボルト孔径 mm
    qreal edgehy1;                          // 縁端距離左 mm
    qreal edgehy2;                          // 縁端距離右 mm
    qreal edgehz1;                          // 縁端距離下 mm
    qreal edgehz2;                          // 縁端距離上 mm

//Bolt
    BaseBoltType::Type  boltType;           // ボルト配置タイプ
    int bolt_ny1;                           // ボルト本数左
    int bolt_ny2;                           // ボルト本数右
    int bolt_nz1;                           // ボルト本数下
    int bolt_nz2;                           // ボルト本数上
    int boltPhi;                            // ボルト呼び径 mm
    qreal screwPhi;                         // ねじ部径 mm
    bool hasElongationAbility;              // ボルトの伸び能力
    qreal anchorArea;                       // アンカー一本当たりの定着板の面積 mm^2
    qreal anchorLength;                     // アンカー長さ mm

//BaseConc
    qreal sizeBc;                           // 基礎コンクリート幅 mm
    qreal sizeDc;                           // 基礎コンクリートせい mm
    qreal sizeH;                            // 基礎コンクリート高さ mm
    qreal projectedarea;                    // アンカーのコンクリート端あき検討用投影面積 mm^2
    qreal B0;                               // ベースプレートのSRC表面に対する端あき mm
    qreal D0;                               // ベースプレートのSRC表面に対する端あき mm
};

struct POST3D_CLASS_DLLSPEC SpringParameter {
    explicit SpringParameter(
        SpringType::Type hysteresis_ = SpringType::Type::LINEAR,
        qreal K1_ = 0,
        qreal K2_ = 0,
        qreal K3_ = 0,
        qreal fy1m_ = 0,
            qreal fy1p_ = 0,
            qreal fy2m_ = 0,
    qreal fy2p_ = 0)
        : hysteresis(hysteresis_), K1(K1_), K2(K2_), K3(K3_),
          Fy1m(fy1m_),Fy1p(fy1p_), Fy2m(fy2m_), Fy2p(fy2p_) {}

    SpringType::Type hysteresis;            // 履歴特性
    qreal K1;                               // K1  kN/m
    qreal K2;                               // K2  kN/m
    qreal K3;                               // K3  kN/m
    //バイリニアでは±の降伏点、トリリニアでは対称にしている。
    //Fy1p,Fy2p・・・のように書き直すと、ISOのウィジェット、そのDraw、ファイルの書き出し（＆ソルバー側の読み込み）に影響。
    qreal Fy1m;                              // Fy1- kN
    qreal Fy1p;                              // Fy1+ kN
    qreal Fy2m;                              // Fy2- kN
    qreal Fy2p;                              // Fy2+ kN
};

struct POST3D_CLASS_DLLSPEC DamperParameter {
    explicit DamperParameter(
        DamperType::Type ptype_ = DamperType::Type::UNDEFINED,
        qreal w_ = 0,
        qreal vx_ = 0,
        qreal vy_ = 0,
        qreal vz_ = 0,
        QList<double> parameters_ = QList<double>{},
        qreal p_ = 0,
        qreal dis_ = 0)
        : productType(ptype_), selfWeight(w_),
          vecX(vx_), vecY(vy_), vecZ(vz_),
          parameters(parameters_),
          period(p_), maxDisp(dis_) {}

    DamperType::Type productType;           // ダンパータイプ
    qreal selfWeight;                       // 自重 kN
    qreal vecX;                             // 取り付け方向 X
    qreal vecY;                             // 取り付け方向 Y
    qreal vecZ;                             // 取り付け方向 Z
    QList<double> parameters;               // 可変数パラメータ
    qreal period;                           // 静的解析用固有周期 s
    qreal maxDisp;                          // 静的解析用最大変形　cm
    qreal modelFlag;
    qreal rigidLength;
    qreal damperDirection;
    qreal tAnalysis;
    qreal factor;
    qreal frameRigidity;
};

struct POST3D_CLASS_DLLSPEC IsolatorParameter {
    explicit IsolatorParameter(
        const QString &ptype_ = QString(),
        const QString &pname_ = QString(),
        qreal w_ = 0,
        qreal d_ = 0,
        qreal h_ = 0,
        qreal l_ = 0)
        : productType(ptype_), productName(pname_),
          selfWeight(w_), sizeD(d_), sizeH(h_), settingLevel(l_) {}

    QString productType;                    // 免震種類
    QString productName;                    // 型番
    qreal  selfWeight;                      //　自重 kN
    QString renderShape;                    // 表示形状　→　enumで実装
    qreal sizeD;                            // 表示サイズD mm
    qreal sizeH;                            //　表示サイズH mm
    qreal settingLevel;                     // 設置高さ mm
    qreal displacement;                     // 割線剛性計算変形 mm
    int springNumber;                       // ばね数
    qreal inflectionRatio;                     // 反曲点高さ比
};

struct POST3D_CLASS_DLLSPEC WallOpenParameter {
    explicit WallOpenParameter(
        WallOpenType::Type otype_ = WallOpenType::Type::UNDEFINED,
        bool lr_ = true,
        WallOpenType::HPosition hPosition_ = WallOpenType::HPosition::CENTER,
        bool ud_ = true,
        WallOpenType::VPosition vPosition_ = WallOpenType::VPosition::MIDDLE,
        qreal x_ = 0,
        qreal y_ = 0,
        qreal w_ = 0,
        qreal h_ = 0,
        qreal r_ = 0,
        qreal l_ = 0,
        qreal t_ = 0,
        qreal b_ = 0,
        QString comment_ = QString())
        : openType(otype_), isHorizontalFace(lr_), hPosition(hPosition_),
          isVerticalFace(ud_), vPosition(vPosition_),
          ox(x_), oy(y_), width(w_), height(h_),
          slitR(r_), slitL(l_), slitT(t_), slitB(b_), comment(comment_) {}

    WallOpenType::Type openType;            // 開口種別
    bool isHorizontalFace;                  // 開口が左右方向のフェイス押さえかどうか
    WallOpenType::HPosition hPosition;      // 開口位置 左押さえか中央か右かか
    bool isVerticalFace;                    // 開口が上下方向のフェイス押さえかどうか
    WallOpenType::VPosition vPosition;      // 開口位置　下押さえ中央か上か
    qreal ox;                               // 開口原点ox mm
    qreal oy;                               // 開口原点oy mm
    qreal width;                            // 幅 mm
    qreal height;                           // 高さ mm
    qreal slitR;                            // スリット範囲 right mm
    qreal slitL;                            // スリット範囲 left mm
    qreal slitT;                            // スリット範囲 top mm
    qreal slitB;                            // スリット範囲 bottom mm
    QString comment;                        // コメント
};



/* 断面定義  */

struct POST3D_CLASS_DLLSPEC SectionValues {
    explicit SectionValues(  const QString &_name = QString() ) : sectionName(_name) {}

    virtual void setValues( const QList<QVariant> &, const SectionDataType &, int ) { }

    DATATYPE memberType;
    STRUCTTYPE sectionType;
    QString sectionName;

    QUuid startFloorID;                 // 開始階  柱・大梁・壁
    QUuid endFloorID;                   //　終了階 　柱・大梁・壁

    LineCommonParameter line_param;
    LinePartsParameter i_param;
    LinePartsParameter c_param;
    LinePartsParameter j_param;

    PlaneSectionParameter plane_param;
    BaseSectionParameter base_param;
    WallOpenParameter wopen_param;

    SpringParameter hspring_param;
    SpringParameter vspring_param;

    DamperParameter damper_param;
    IsolatorParameter isolator_param;

    QString writeData(QSharedPointer<N3DUuidTable>) const;
};

struct POST3D_CLASS_DLLSPEC LineMemberSection : public SectionValues {
    explicit LineMemberSection(){}
};

struct POST3D_CLASS_DLLSPEC PlaneMemberSection : public SectionValues {
    explicit PlaneMemberSection(){}
};

struct POST3D_CLASS_DLLSPEC BaseMemberSection : public SectionValues {
    explicit BaseMemberSection(){}
};

struct POST3D_CLASS_DLLSPEC IsolatorSection : public SectionValues {
    explicit IsolatorSection(){}
};

struct POST3D_CLASS_DLLSPEC DamperSection : public SectionValues {
    explicit DamperSection(){}
};

struct POST3D_CLASS_DLLSPEC WallOpenSection : public SectionValues {
    explicit WallOpenSection(){}
};



/*  計算結果  */

struct POST3D_CLASS_DLLSPEC LineOutputData {
    explicit LineOutputData(
        qreal A_ = 0, qreal As_ = 0, qreal Ae_ = 0, qreal Aeq_ = 0,
        qreal Ix_ = 0, qreal Iy_ = 0, qreal Isx_ = 0, qreal Isy_ = 0, qreal Ieqx_ = 0, qreal Ieqy_ = 0,
        qreal Zx_ = 0, qreal Zy_ = 0, qreal Zpx_ = 0, qreal Zpy_ = 0, qreal Zex_ = 0, qreal Zey_ = 0,
        qreal Srsx_ = 0, qreal Srsy_ = 0, qreal wtratiof_ = 0, qreal wtratiow_ = 0,
        qreal slendernessratio_ = 0, qreal unitmass_ = 0,
        SectionRank::Rank rankf_ = SectionRank::Rank::UNDEFINED,
        SectionRank::Rank rankw_ = SectionRank::Rank::UNDEFINED,
        SectionRank::Rank rankSN_ = SectionRank::Rank::UNDEFINED,
        qreal pg_ = 0, qreal pt1_ = 0, qreal pt2_ = 0, qreal pt3_ = 0, qreal pt4_ = 0, qreal pwx_ = 0,
        qreal pwy_ = 0)
        : A(A_), As(As_), Ae(Ae_), Aeq(Aeq_), Ix(Ix_), Iy(Iy_), Isx(Isx_), Isy(Isy_), Ieqx(Ieqx_),
          Ieqy(Ieqy_),
          Zx(Zx_), Zy(Zy_), Zpx(Zpx_), Zpy(Zpy_), Zex(Zex_), Zey(Zey_), Srsx(Srsx_), Srsy(Srsy_),
          wtratiof(wtratiof_), wtratiow(wtratiow_), slendernessRatio(slendernessratio_), unitmass(unitmass_),
          rankF(rankf_), rankW(rankw_), rankSN(rankSN_),
          pg(pg_), pt1(pt1_), pt2(pt2_), pt3(pt3_), pt4(pt4_), pwx(pwx_), pwy(pwy_) {}

    qreal A;     //断面積
    qreal As;    //断面積（S部分）
    qreal Ae;    //有効面積（幅厚比Dランク部材）
    qreal Aeq;   //等価断面積（ヤング係数比に応じてRC換算）
    qreal Ix;    //断面二次モーメントx方向
    qreal Iy;    //断面二次モーメントy方向
    qreal Isx;   //断面二次モーメントx方向（S部分）
    qreal Isy;   //断面二次モーメントy方向（S部分）
    qreal Ieqx;  //断面二次モーメントx方向（ヤング係数比に応じてRC換算）
    qreal Ieqy;  //断面二次モーメントy方向（ヤング係数比に応じてRC換算）
    qreal Zx;    //断面係数x方向
    qreal Zy;    //断面係数y方向
    qreal Zpx;   //塑性断面係数x方向
    qreal Zpy;   //塑性断面係数y方向
    qreal Zex;   //有効断面係数x方向（幅厚比Dランク部材）
    qreal Zey;   //有効断面係数y方向（幅厚比Dランク部材）
    qreal Srsx;  //断面二次半径x方向
    qreal Srsy;  //断面二次半径y方向
    qreal wtratiof;  //幅厚比フランジ
    qreal wtratiow;  //幅厚比ウェブ
    qreal slendernessRatio; //細長比
    qreal unitmass; //単位重量
    SectionRank::Rank rankF; //幅厚比ランクフランジ
    SectionRank::Rank rankW; //幅厚比ランクウェブ
    SectionRank::Rank rankSN;//幅厚比ランク（H形鋼SN部材）
    qreal pg;    //軸鉄筋・鉄骨比
    qreal pt1;   //引張鉄筋比左辺引張
    qreal pt2;   //引張鉄筋比右辺引張
    qreal pt3;   //引張鉄筋比上辺引張
    qreal pt4;   //引張鉄筋比下辺引張
    qreal pwx;   //フープ筋比x方向
    qreal pwy;   //フープ筋比x方向
};

struct POST3D_CLASS_DLLSPEC PlaneOutputData {
    explicit PlaneOutputData(
        qreal Psv_ = 0, qreal Psh_ = 0, qreal Arvs_ = 0, qreal Arhs_ = 0,
        qreal Ards_ = 0, qreal Arvl_ = 0, qreal Arhl_ = 0, qreal Ardl_ = 0, qreal Atot_ = 0,
        qreal Atom_ = 0,
        qreal Atob_ = 0, qreal Atit_ = 0, qreal Atim_ = 0, qreal Atib_ = 0, qreal Atoe_ = 0,
        qreal Atoc_ = 0,
        qreal Atie_ = 0, qreal Atic_ = 0,
        qreal At_sUpperEdgeA_ = 0, qreal At_sUpperCenterA_ = 0, qreal At_sUpperB_ = 0,
        qreal At_sLowerEdgeA_ = 0, qreal At_sLowerCenterA_ = 0, qreal At_sLowerB_ = 0,
        qreal At_lUpperEdgeA_ = 0, qreal At_lUpperCenterA_ = 0, qreal At_lUpperB_ = 0,
        qreal At_lLowerEdgeA_ = 0, qreal At_lLowerCenterA_ = 0, qreal At_lLowerB_ = 0,
        qreal Ma_sUpperEdgeA_ = 0, qreal Ma_sUpperCenterA_ = 0, qreal Ma_sUpperB_ = 0,
        qreal Ma_sLowerEdgeA_ = 0, qreal Ma_sLowerCenterA_ = 0, qreal Ma_sLowerB_ = 0,
        qreal Ma_lUpperEdgeA_ = 0, qreal Ma_lUpperCenterA_ = 0, qreal Ma_lUpperB_ = 0,
        qreal Ma_lLowerEdgeA_ = 0, qreal Ma_lLowerCenterA_ = 0, qreal Ma_lLowerB_ = 0)
        : Psv(Psv_), Psh(Psh_), Arvs(Arvs_), Arhs(Arhs_), Ards(Ards_), Arvl(Arvl_), Arhl(Arhl_),
          Ardl(Ardl_),
          Atot(Atot_), Atom(Atom_), Atob(Atob_), Atit(Atit_), Atim(Atim_), Atib(Atib_), Atoe(Atoe_),
          Atoc(Atoc_), Atie(Atie_), Atic(Atic_),
          At_sUpperEdgeA(At_sUpperEdgeA_), At_sUpperCenterA(At_sUpperCenterA_), At_sUpperB(At_sUpperB_),
          At_sLowerEdgeA(At_sLowerEdgeA_), At_sLowerCenterA(At_sLowerCenterA_), At_sLowerB(At_sLowerB_),
          At_lUpperEdgeA(At_lUpperEdgeA_), At_lUpperCenterA(At_lUpperCenterA_), At_lUpperB(At_lUpperB_),
          At_lLowerEdgeA(At_lLowerEdgeA_), At_lLowerCenterA(At_lLowerCenterA_), At_lLowerB(At_lLowerB_),
          Ma_sUpperEdgeA(Ma_sUpperEdgeA_), Ma_sUpperCenterA(Ma_sUpperCenterA_), Ma_sUpperB(Ma_sUpperB_),
          Ma_sLowerEdgeA(Ma_sLowerEdgeA_), Ma_sLowerCenterA(Ma_sLowerCenterA_), Ma_sLowerB(Ma_sLowerB_),
          Ma_lUpperEdgeA(Ma_lUpperEdgeA_), Ma_lUpperCenterA(Ma_lUpperCenterA_), Ma_lUpperB(Ma_lUpperB_),
          Ma_lLowerEdgeA(Ma_lLowerEdgeA_), Ma_lLowerCenterA(Ma_lLowerCenterA_), Ma_lLowerB(Ma_lLowerB_) {}

    qreal Psv;    //せん断補強筋比　縦
    qreal Psh;    //せん断補強筋比　横
    qreal Arvs;   //開口補強筋断面積　縦　小開口
    qreal Arhs;   //開口補強筋断面積　横　小開口
    qreal Ards;   //開口補強筋断面積　斜　小開口
    qreal Arvl;   //開口補強筋断面積　縦　大開口
    qreal Arhl;   //開口補強筋断面積　横　大開口
    qreal Ardl;   //開口補強筋断面積　斜　大開口
    qreal Atot;   //せん断補強筋断面積　縦　外側　上
    qreal Atom;   //せん断補強筋断面積　縦　外側　中
    qreal Atob;   //せん断補強筋断面積　縦　外側　下
    qreal Atit;   //せん断補強筋断面積　縦　内側　上
    qreal Atim;   //せん断補強筋断面積　縦　内側　中
    qreal Atib;   //せん断補強筋断面積　縦　内側　下
    qreal Atoe;   //せん断補強筋断面積　横　外側　端部
    qreal Atoc;   //せん断補強筋断面積　横　外側　中央部
    qreal Atie;   //せん断補強筋断面積　横　内側　端部
    qreal Atic;   //せん断補強筋断面積　横　内側　中央部

    qreal At_sUpperEdgeA  ;//せん断補強筋断面積　短辺方向　上側　端部　A部
    qreal At_sUpperCenterA;//せん断補強筋断面積　短辺方向　上側　中央部　A部
    qreal At_sUpperB      ;//せん断補強筋断面積　短辺方向　上側　B部
    qreal At_sLowerEdgeA  ;//せん断補強筋断面積　短辺方向　下側　端部　A部
    qreal At_sLowerCenterA;//せん断補強筋断面積　短辺方向　下側　中央部　A部
    qreal At_sLowerB      ;//せん断補強筋断面積　短辺方向　下側　B部
    qreal At_lUpperEdgeA  ;//せん断補強筋断面積　長辺方向　上側　端部　A部
    qreal At_lUpperCenterA;//せん断補強筋断面積　長辺方向　上側　中央部　A部
    qreal At_lUpperB      ;//せん断補強筋断面積　長辺方向　上側　B部
    qreal At_lLowerEdgeA  ;//せん断補強筋断面積　長辺方向　下側　端部　A部
    qreal At_lLowerCenterA;//せん断補強筋断面積　長辺方向　下側　中央部　A部
    qreal At_lLowerB      ;//せん断補強筋断面積　長辺方向　下側　B部

    qreal Ma_sUpperEdgeA  ;//許容曲げモーメント　短辺方向　上側　端部　A部
    qreal Ma_sUpperCenterA;//許容曲げモーメント　短辺方向　上側　中央部　A部
    qreal Ma_sUpperB      ;//許容曲げモーメント　短辺方向　上側　B部
    qreal Ma_sLowerEdgeA  ;//許容曲げモーメント　短辺方向　下側　端部　A部
    qreal Ma_sLowerCenterA;//許容曲げモーメント　短辺方向　下側　中央部　A部
    qreal Ma_sLowerB      ;//許容曲げモーメント　短辺方向　下側　B部
    qreal Ma_lUpperEdgeA  ;//許容曲げモーメント　長辺方向　上側　端部　A部
    qreal Ma_lUpperCenterA;//許容曲げモーメント　長辺方向　上側　中央部　A部
    qreal Ma_lUpperB      ;//許容曲げモーメント　長辺方向　上側　B部
    qreal Ma_lLowerEdgeA  ;//許容曲げモーメント　長辺方向　下側　端部　A部
    qreal Ma_lLowerCenterA;//許容曲げモーメント　長辺方向　下側　中央部　A部
    qreal Ma_lLowerB      ;//許容曲げモーメント　長辺方向　下側　B部
};

} // namespace post3dapp
#endif // DEFINE_SECTION_STRUCT_H
