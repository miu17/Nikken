#include "define_section_struct.h"

#include <QtGui>

#include "n3d_uuidtable.h"
#include "unified_calcload.h"
namespace post3dapp{
SteelType::Type SteelType::stringToEnum(const QString &str)
{
    Type retenum = Type::UNDEFINED;
    if (str == "H" ) {
        retenum = Type::ST_H ;
    } else if (str == "I" ) {
        retenum = Type::ST_I ;
    } else if (str == "BX") {
        retenum = Type::ST_BX;
    } else if (str == "P" ) {
        retenum = Type::ST_P ;
    } else if (str == "T1" ) {
        retenum = Type::ST_T1 ;
    } else if (str == "T2" ) {
        retenum = Type::ST_T2 ;
    } else if (str == "T3" ) {
        retenum = Type::ST_T3 ;
    } else if (str == "T4" ) {
        retenum = Type::ST_T4 ;
    } else if (str == "C1" ) {
        retenum = Type::ST_C1 ;
    } else if (str == "C2" ) {
        retenum = Type::ST_C2 ;
    } else if (str == "C3" ) {
        retenum = Type::ST_C3 ;
    } else if (str == "C4" ) {
        retenum = Type::ST_C4 ;
    } else if (str == "L" ) {
        retenum = Type::ST_L ;
    } else if (str == "WH") {
        retenum = Type::ST_WH;
    } else if (str == "WI") {
        retenum = Type::ST_WI;
    } else if (str == "WBX") {
        retenum = Type::ST_WBX;
    } else if (str == "WP") {
        retenum = Type::ST_WP;
    } else if (str == "WT1") {
        retenum = Type::ST_WT1;
    } else if (str == "WT2") {
        retenum = Type::ST_WT2;
    } else if (str == "WT3") {
        retenum = Type::ST_WT3;
    } else if (str == "WT4") {
        retenum = Type::ST_WT4;
    } else if (str == "WC1" ) {
        retenum = Type::ST_WC1 ;
    } else if (str == "WC2" ) {
        retenum = Type::ST_WC2 ;
    } else if (str == "WC3" ) {
        retenum = Type::ST_WC3 ;
    } else if (str == "WC4" ) {
        retenum = Type::ST_WC4 ;
    } else if (str == "WPL") {
        retenum = Type::ST_WPL;
    } else if (str == "WBR") {
        retenum = Type::ST_WBR;
    } else if (str == "CRS") {
        retenum = Type::ST_CROSS;
    } else if (str == "CRH") {
        retenum = Type::ST_CROSSH;
    } else if (str == ""){
        retenum = Type::UNDEFINED;
    } else{
        retenum = UnifiedCalcLoad::getStandardType(str);
    }
    return retenum;
}

QString SteelType::enumToString(Type stype)
{
    QString retstr;
    if ( stype == Type::ST_H ) {
        retstr = "H" ;
    } else if ( stype == Type::ST_I ) {
        retstr = "I" ;
    } else if ( stype == Type::ST_BX) {
        retstr = "BX";
    } else if ( stype == Type::ST_P ) {
        retstr = "P" ;
    } else if ( stype == Type::ST_T1 ) {
        retstr = "T1" ;
    } else if ( stype == Type::ST_T2 ) {
        retstr = "T2" ;
    } else if ( stype == Type::ST_T3 ) {
        retstr = "T3" ;
    } else if ( stype == Type::ST_T4 ) {
        retstr = "T4" ;
    } else if ( stype == Type::ST_C1 ) {
        retstr = "C" ;
    } else if ( stype == Type::ST_L ) {
        retstr = "L" ;
    } else if ( stype == Type::ST_WPL) {
        retstr = "WPL";
    } else if ( stype == Type::ST_WBR) {
        retstr = "WBR";
    } else if ( stype == Type::ST_WH) {
        retstr = "WH";
    } else if ( stype == Type::ST_WI) {
        retstr = "WI";
    } else if ( stype == Type::ST_WBX) {
        retstr = "WBX";
    } else if ( stype == Type::ST_WP) {
        retstr = "WP";
    } else if ( stype == Type::ST_WT1) {
        retstr = "WT1";
    } else if ( stype == Type::ST_WT2) {
        retstr = "WT2";
    } else if ( stype == Type::ST_WT3) {
        retstr = "WT1";
    } else if ( stype == Type::ST_WT4) {
        retstr = "WT1";
    } else if ( stype == Type::ST_WC1 ) {
        retstr = "WC1" ;
    } else if ( stype == Type::ST_CROSS ) {
        retstr = "CRS" ;
    } else if ( stype == Type::ST_CROSSH ) {
        retstr = "CRH" ;
    }
    return retstr;
}

bool SteelType::isStandardType(Type stype)
{
    return ( stype == Type::ST_H || stype == Type::ST_I  ||
             stype == Type::ST_BX || stype == Type::ST_P || stype == Type::ST_L ||
             stype == Type::ST_T1 || stype == Type::ST_T2 || stype == Type::ST_T3 || stype == Type::ST_T4 ||
             stype == Type::ST_C1 || stype == Type::ST_C2 || stype == Type::ST_C3 || stype == Type::ST_C4);
}

bool SteelType::isSideways(Type type)
{
    return (type == Type::ST_I || type == Type::ST_WI ||
            type == Type::ST_T3 || type == Type::ST_WT3 || type == Type::ST_T4 || type == Type::ST_WT4 ||
            type == Type::ST_C3 || type == Type::ST_WC3 || type == Type::ST_T4 || type == Type::ST_WC4) ;
}

PlaneType::Type PlaneType::stringToEnum(const QString &str)
{
    Type retenum = Type::UNDEFINED;
    if ( str == u8"非構造モデル" ) {
        retenum = Type::NONSTRUCT;
    } else if ( str == u8"耐力壁" ) {
        retenum = Type::WALL;
    } else if ( str == u8"雑壁" ) {
        retenum = Type::ZWALL;
    } else if ( str == u8"平板シェル要素" ) {
        retenum = Type::PLATE;
    } else if ( str == u8"平面応力要素" ) {
        retenum = Type::MEMBRANE;
    }
    return retenum;
}

QString PlaneType::enumToString(const Type &type){
    QString retString;
    if (type == PlaneType::Type::NONSTRUCT)
        retString = u8"非構造モデル";
    else if (type == PlaneType::Type::WALL)
        retString = u8"耐力壁";
    else if (type == PlaneType::Type::ZWALL)
        retString = u8"雑壁";
    else if (type == PlaneType::Type::PLATE)
        retString = u8"平板シェル要素";
    else if (type == PlaneType::Type::MEMBRANE)
        retString = u8"平面応力要素";
    return retString;
}


PlaneType2::Type PlaneType2::stringToEnum(const QString &str)
{
    Type retenum = Type::UNDEFINED;
    if ( str =="WH" ) {
        retenum = Type::WH;
    } else if ( str == "WI" ) {
        retenum = Type::WI;
    } else if ( str == "SH" ) {
        retenum = Type::SH;
    } else if ( str == "SI" ) {
        retenum = Type::SI;
    }
    return retenum;
}

PlaneSide::Side PlaneSide::stringToEnum(const QString &str)
{
    Side retenum = Side::UNDEFINED;
    if (str == u8"内側" ) {
        retenum = Side::INNER ;
    } else if (str == u8"外側" ) {
        retenum = Side::OUTER ;
    } else if (str == u8"上" ) {
        retenum = Side::UPPER ;
    } else if (str == u8"下" ) {
        retenum = Side::LOWER ;
    } else if (str == u8"左" ) {
        retenum = Side::LEFT ;
    } else if (str == u8"右" ) {
        retenum = Side::RIGHT ;
    } else if (str == u8"中央") {
        retenum = Side::CENTER ;
    }
    return retenum;
}

QString PlaneSide::enumToString(Side argenum)
{
    QString retstr;
    if (argenum == Side::INNER ) {
        retstr = u8"内側" ;
    } else if (argenum == Side::OUTER ) {
        retstr = u8"外側" ;
    } else if (argenum == Side::UPPER ) {
        retstr =  u8"上" ;
    } else if (argenum == Side::LOWER ) {
        retstr = u8"下" ;
    } else if (argenum == Side::LEFT ) {
        retstr = u8"左" ;
    } else if (argenum == Side::RIGHT ) {
        retstr = u8"右" ;
    } else if (argenum == Side::CENTER ) {
        retstr = u8"中央" ;
    }
    return retstr;
}

SectionRank::Rank SectionRank::stringToEnum(const QString &str)
{
    Rank retenum = Rank::UNDEFINED;
    if (str == "FA") {
        retenum = Rank::FA;
    } else if (str == "FB") {
        retenum = Rank::FB;
    } else if (str == "FC") {
        retenum = Rank::FC;
    } else if (str == "FD") {
        retenum = Rank::FD;
    }
    return retenum;
}

QString SectionRank::enumToString(Rank argenum)
{
    QString retstr;
    if (argenum == Rank::FA) {
        retstr = "FA";
    } else if (argenum == Rank::FB) {
        retstr = "FB";
    } else if (argenum == Rank::FC) {
        retstr = "FC";
    } else if (argenum == Rank::FD) {
        retstr = "FD";
    }
    return retstr;
}

WallOpenType::Type WallOpenType::stringToEnumType(const QString &str)
{
    Type retenum = Type::UNDEFINED;
    if     (str == u8"通常開口") {
        retenum = Type::NORMAL;
    } else if (str == u8"スリット") {
        retenum = Type::SLIT;
    } else if (str == u8"解析開口"){
        retenum = Type::ANALYSIS;
    }
    return retenum;
}

QString WallOpenType::enumToStringType(Type type)
{
    if(type == WallOpenType::Type::NORMAL) {
        return u8"通常開口";
    } else if (type == WallOpenType::Type::SLIT){
        return u8"スリット";
    } else{
        return u8"解析開口";
    }
}

WallOpenType::HPosition WallOpenType::stringToEnumH(const QString &str){
    if (str == u8"左") {
        return HPosition::LEFT;
    } else if (str == u8"水平中央") {
        return HPosition::CENTER;
    } else{
        return HPosition::RIGHT;
    }
}
QString WallOpenType::enumToStringH(HPosition h){
    if(h == WallOpenType::HPosition::LEFT) {
        return u8"左";
    } else if (h == WallOpenType::HPosition::CENTER){
        return u8"水平中央";
    } else{
        return u8"右";
    }
}
WallOpenType::VPosition WallOpenType::stringToEnumV(const QString &str){
    if (str == u8"上") {
        return VPosition::TOP;
    } else if (str == u8"鉛直中央") {
        return VPosition::MIDDLE;
    } else{
        return VPosition::BOTTOM;
    }
}
QString WallOpenType::enumToStringV(VPosition v){
    if(v == WallOpenType::VPosition::TOP) {
        return u8"上";
    } else if (v == WallOpenType::VPosition::MIDDLE){
        return u8"鉛直中央";
    } else{
        return u8"下";
    }
}

const QString BaseBoltType::Undefined = "";
const QString BaseBoltType::Rectangle = u8"長方形";
const QString BaseBoltType::ParallelLR = u8"左右平行";
const QString BaseBoltType::ParallelUD = u8"上下平行";
const QString BaseBoltType::Diamond = u8"角なし";
const QString BaseBoltType::Circle = u8"円形";


BaseBoltType::Type BaseBoltType::stringToEnum(const QString &str)
{
    Type retenum = Type::UNDEFINED;
    if (str == Rectangle) {
        retenum = Type::RECTANGLE;
    } else if (str == ParallelLR) {
        retenum = Type::PARALLEL_LR;
    } else if (str == ParallelUD) {
        retenum = Type::PARALLEL_UD;
    } else if (str == Diamond) {
        retenum = Type::DIAMOND;
    } else if (str == Circle) {
        retenum = Type::CIRCLE;
    }
    return retenum;
}

QString BaseBoltType::enumToString(Type type){
    switch(type){
    case BaseBoltType::Type::RECTANGLE:
        return Rectangle;
    case BaseBoltType::Type::PARALLEL_LR:
        return ParallelLR;
    case BaseBoltType::Type::PARALLEL_UD:
        return ParallelUD;
    case BaseBoltType::Type::DIAMOND:
        return Diamond;
    case BaseBoltType::Type::CIRCLE:
        return Circle;
    default:
        return Undefined;
    }
}

BasePlateType::Type BasePlateType::stringToEnum(const QString &str)
{
    Type retenum = Type::UNDEFINED;
    if (str == u8"長方形") {
        retenum = Type::RECTANGLE;
    } else if (str == u8"八角形") {
        retenum = Type::OCTAGON;
    } else if (str == u8"右隅切") {
        retenum = Type::DTYPE_L;
    } else if (str == u8"左隅切") {
        retenum = Type::DTYPE_R;
    } else if (str == u8"下隅切") {
        retenum = Type::DTYPE_U;
    } else if (str == u8"上隅切") {
        retenum = Type::DTYPE_D;
    } else if (str == u8"円形") {
        retenum = Type::CIRCLE;
    }
    return retenum;
}


SpringType::Type SpringType::stringToEnum(const QString &str)
{
    Type retenum = Type::UNDEFINED;
    if (str == u8"線形"){
        return retenum = Type::LINEAR;
    } else if (str == u8"リバーシブルバイリニア") {
        return retenum = Type::REVERSIBLE_BILINEAR;
    } else if (str == u8"バイリニア") {
        return retenum = Type::BILINEAR;
    } else if (str == u8"トリリニア") {
        return retenum = Type::TRILINEAR;
    } else if (str == u8"リバーシブルバイリニア(引張)") {
        return retenum = Type::TENSILE_REVERSIBLE_BILINEAR;
    } else if (str == u8"リバーシブルトリリニア(引張)") {
        return retenum = Type::TENSILE_REVERSIBLE_TRILINEAR;
    } else if (str == u8"劣化") {
        return retenum = Type::DEGRADING;
    } else if (str == u8"スリップ") {
        return retenum = Type::SLIP;
    } else if (str == u8"原点指向") {
        return retenum = Type::ORIGIN_ORIENTED;
    } else if (str == u8"武田モデル") {
        return retenum = Type::TAKEDA;
    } else if (str == u8"岡本モデル") {
        return retenum = Type::OKAMOTO;
    } else if (str == u8"劣化（K4=K3）") {
        return retenum = Type::DEGRADING_K4eqK3;
    } else if (str == u8"劣化（K4=0）") {
        return retenum = Type::DEGRADING_K4eq0;
    }
    return retenum;
}

DamperType::Type DamperType::stringToEnum(const QString &str)
{
    Type retenum = Type::UNDEFINED;
    if     (str == u8"1:線形ダンパー Kc=0") {
        retenum = Type::TYPE01;
    } else if (str == u8"2:粘性制振壁 ADC旧式 Kc=0") {
        retenum = Type::TYPE02;
    } else if (str == u8"3:2段階粘性減衰係数１ Kc=0") {
        retenum = Type::TYPE03;
    } else if (str == u8"4:2段階粘性減衰係数２ Kc=0") {
        retenum = Type::TYPE04;
    } else if (str == u8"5:線形ダンパー Kc≠0") {
        retenum = Type::TYPE05;
    } else if (str == u8"6:粘性制振壁 ADC旧式 Kc≠0") {
        retenum = Type::TYPE06;
    } else if (str == u8"7:2段階粘性減衰係数１ Kc≠0") {
        retenum = Type::TYPE07;
    } else if (str == u8"8:2段階粘性減衰係数２ Kc≠0") {
        retenum = Type::TYPE08;
    } else if (str == u8"9:GMS（一般化マクスウェルモデル）") {
        retenum = Type::TYPE09;
    } else if (str == u8"21:鋼材系バイリニア 剛性入力") {
        retenum = Type::TYPE21;
    } else if (str == u8"22:鋼材系バイリニア 構成入力") {
        retenum = Type::TYPE22;
    } else if (str == u8"23:鋼材系トリリニア 剛性入力") {
        retenum = Type::TYPE23;
    } else if (str == u8"24:鋼材系トリリニア 構成入力") {
        retenum = Type::TYPE24;
    } else if (str == u8"25:オイルダンパー") {
        retenum = Type::TYPE25;
    } else if (str == u8"26:粘性（CVα乗）ダンパー") {
        retenum = Type::TYPE26;
    } else if (str == u8"27:VEM　住友3M") {
        retenum = Type::TYPE27;
    } else if (str == u8"28:Hidaxダンパー 鹿島建設") {
        retenum = Type::TYPE28;
    } else if (str == u8"29:粘性制振壁 オイレス工業") {
        retenum = Type::TYPE29;
    } else if (str == u8"30:粘性制振壁 免制振デバイス") {
        retenum = Type::TYPE30;
    } else if (str == u8"31:付加質量") {
        retenum = Type::TYPE31;
    } else if (str == u8"32:SRI高硬度ダンパー 住友ゴム") {
        retenum = Type::TYPE32;
    } else if (str == u8"33:笠井モデル") {
        retenum = Type::TYPE33;
    } else if (str == u8"34:3段階線形粘性減衰係数") {
        retenum = Type::TYPE34;
    }
    return retenum;
}

QString DamperType::enumToString(Type tp)
{
    QString retstr;
    if     (tp == Type::TYPE01) {
        retstr = u8"1:線形ダンパー Kc=0";
    } else if (tp == Type::TYPE02) {
        retstr = u8"2:粘性制振壁 ADC旧式 Kc=0";
    } else if (tp == Type::TYPE03) {
        retstr = u8"3:2段階粘性減衰係数１ Kc=0";
    } else if (tp == Type::TYPE04) {
        retstr = u8"4:2段階粘性減衰係数２ Kc=0";
    } else if (tp == Type::TYPE05) {
        retstr = u8"5:線形ダンパー Kc≠0";
    } else if (tp == Type::TYPE06) {
        retstr = u8"6:粘性制振壁 ADC旧式 Kc≠0";
    } else if (tp == Type::TYPE07) {
        retstr = u8"7:2段階粘性減衰係数１ Kc≠0";
    } else if (tp == Type::TYPE08) {
        retstr = u8"8:2段階粘性減衰係数２ Kc≠0";
    } else if (tp == Type::TYPE09) {
        retstr = u8"9:GMS（一般化マクスウェルモデル）";
    } else if (tp == Type::TYPE21) {
        retstr = u8"21:鋼材系バイリニア 剛性入力";
    } else if (tp == Type::TYPE22) {
        retstr = u8"22:鋼材系バイリニア 構成入力";
    } else if (tp == Type::TYPE23) {
        retstr = u8"23:鋼材系トリリニア 剛性入力";
    } else if (tp == Type::TYPE24) {
        retstr = u8"24:鋼材系トリリニア 構成入力";
    } else if (tp == Type::TYPE25) {
        retstr = u8"25:オイルダンパー";
    } else if (tp == Type::TYPE26) {
        retstr = u8"26:粘性（CVα乗）ダンパー";
    } else if (tp == Type::TYPE27) {
        retstr = u8"27:VEM　住友3M";
    } else if (tp == Type::TYPE28) {
        retstr = u8"28:Hidaxダンパー 鹿島建設";
    } else if (tp == Type::TYPE29) {
        retstr = u8"29:粘性制振壁 オイレス工業";
    } else if (tp == Type::TYPE30) {
        retstr = u8"30:粘性制振壁 免制振デバイス";
    } else if (tp == Type::TYPE31) {
        retstr = u8"31:付加質量";
    } else if (tp == Type::TYPE32) {
        retstr = u8"32:SRI高硬度ダンパー 住友ゴム";
    } else if (tp == Type::TYPE33) {
        retstr = u8"33:笠井モデル";
    } else if (tp == Type::TYPE34) {
        retstr = u8"34:3段階線形粘性減衰係数";
    }
    return retstr;
}


QList<QPointF> LinePartsParameterRC::barPointList(DATATYPE mtype, bool isAxialBar) const
{
    if (mtype != DATATYPE::TPCOLUMN  && isAxialBar) {
        return QList<QPointF>();   //invalid
    }

    //y正方向＝画面上方向として座標を返すので、描画するときは正負注意
    QList<QPointF> points;
    ConcreteType::Type cc = isAxialBar ? axialConcType : concType;

    if (cc == ConcreteType::Type::CIRCLE) {
        const qreal PI = 4 * qAtan(1.0);
        const qreal R = isAxialBar ? axialDy / 2 : Dy / 2 - RK;
        const qreal ox = isAxialBar ? axialCenterY : 0.0;
        const qreal oy = isAxialBar ? axialCenterZ : 0.0;
        const int num = isAxialBar ? axialBarArrange1.numsum[0] : mainBarArrange1.numsum[0];
        for (int i = 0; i < num; ++i) {
            points << QPointF(ox + R * qCos(i * 2 * PI / num), oy + R * qSin(i * 2 * PI / num));
        }
    } else if (cc == ConcreteType::Type::RECTANGLE && mtype == DATATYPE::TPCOLUMN) {
        const qreal ox = isAxialBar ? axialCenterY : 0.0;
        const qreal oy = isAxialBar ? axialCenterZ : 0.0;
        const qreal dx = isAxialBar ? axialDy : Dy ;
        const qreal dy = isAxialBar ? axialDz : Dz ;
        const qreal rk = isAxialBar ? 0 : RK;
        const qreal ry = RY;

        const QList<int> num1T = isAxialBar ? axialBarArrange1.numleft  : mainBarArrange1.numleft;
        const QList<int> num1S = isAxialBar ? axialBarArrange1.numsum   : mainBarArrange1.numsum;
        const QList<int> num1B = isAxialBar ? axialBarArrange1.numright : mainBarArrange1.numright;
        const QList<int> num2L = isAxialBar ? axialBarArrange2.numleft  : mainBarArrange2.numleft;
        const QList<int> num2S = isAxialBar ? axialBarArrange2.numsum   : mainBarArrange2.numsum;
        const QList<int> num2R = isAxialBar ? axialBarArrange2.numright : mainBarArrange2.numright;
        int fillL, fillR, fillT, fillB;
        qreal b2b;//bar to bar

        //左辺
        for (int i = 1; i < num1T[0]; ++i) {
            points << QPointF(ox - dx / 2 + rk, oy + dy / 2 - rk - i * ry);
        }
        for (int i = 1; i < num1B[0]; ++i) {
            points << QPointF(ox - dx / 2 + rk, oy - dy / 2 + rk + i * ry);
        }
        fillT = qMax(1, num1T[0]);
        fillB = qMax(1, num1B[0]);
        b2b = (dy - (fillT + fillB - 2) * ry - 2 * rk) / (num1S[0] - num1T[0] - num1B[0] + 1);
        for (int i = 0; i < num1S[0] - num1T[0] - num1B[0]; ++i) {
            points << QPointF(ox - dx / 2 + rk, oy + dy / 2 - rk - (fillT - 1)*ry - (i + 1)*b2b);
        }
        //右辺
        for (int i = 1; i < num1T[1]; ++i) {
            points << QPointF(ox + dx / 2 - rk, oy + dy / 2 - rk - i * ry);
        }
        for (int i = 1; i < num1B[1]; ++i) {
            points << QPointF(ox + dx / 2 - rk, oy - dy / 2 + rk + i * ry);
        }
        fillT = qMax(1, num1T[1]);
        fillB = qMax(1, num1B[1]);
        b2b = (dy - (fillT + fillB - 2) * ry - 2 * rk) / (num1S[1] - num1T[1] - num1B[1] + 1);
        for (int i = 0; i < num1S[1] - num1T[1] - num1B[1]; ++i) {
            points << QPointF(ox + dx / 2 - rk, oy + dy / 2 - rk - (fillT - 1)*ry - (i + 1)*b2b);
        }
        //上辺
        for (int i = 0; i < num2L[0]; ++i) {
            points << QPointF(ox - dx / 2 + rk + i * ry, oy + dy / 2 - rk);
        }
        for (int i = 0; i < num2R[0]; ++i) {
            points << QPointF(ox + dx / 2 - rk - i * ry, oy + dy / 2 - rk);
        }
        fillL = qMax(1, num2L[0]);
        fillR = qMax(1, num2R[0]);
        b2b = (dx - (fillL + fillR - 2) * ry - 2 * rk) / (num2S[0] - num2L[0] - num2R[0] + 1);
        for (int i = 0; i < num2S[0] - num2L[0] - num2R[0]; ++i) {
            points << QPointF(ox - dx / 2 + rk + (fillL - 1)*ry + (i + 1)*b2b, oy + dy / 2 - rk);
        }
        //下辺
        for (int i = 0; i < num2L[1]; ++i) {
            points << QPointF(ox - dx / 2 + rk + i * ry, oy - dy / 2 + rk);
        }
        for (int i = 0; i < num2R[1]; ++i) {
            points << QPointF(ox + dx / 2 - rk - i * ry, oy - dy / 2 + rk);
        }
        fillL = qMax(1, num2L[1]);
        fillR = qMax(1, num2R[1]);
        b2b = (dx - (fillL + fillR - 2) * ry - 2 * rk) / (num2S[1] - num2L[1] - num2R[1] + 1);
        for (int i = 0; i < num2S[1] - num2L[1] - num2R[1]; ++i) {
            points << QPointF(ox - dx / 2 + rk + (fillL - 1)*ry + (i + 1)*b2b, oy - dy / 2 + rk);
        }

    } else if (cc == ConcreteType::Type::RECTANGLE && (mtype == DATATYPE::TPGIRDER || mtype == DATATYPE::TPBEAM)) {
        const qreal ox = isAxialBar ? axialCenterY : 0.0;
        const qreal oy = isAxialBar ? axialCenterZ : 0.0;
        const qreal dx = isAxialBar ? axialDy : Dy ;
        const qreal dy = isAxialBar ? axialDz : Dz ;

        const QList<int> num1L = isAxialBar ? axialBarArrange1.numleft  : mainBarArrange1.numleft;
        const QList<int> num1S = isAxialBar ? axialBarArrange1.numsum   : mainBarArrange1.numsum;
        const QList<int> num1R = isAxialBar ? axialBarArrange1.numright : mainBarArrange1.numright;
        const QList<int> num2L = isAxialBar ? axialBarArrange2.numleft  : mainBarArrange2.numleft;
        const QList<int> num2S = isAxialBar ? axialBarArrange2.numsum   : mainBarArrange2.numsum;
        const QList<int> num2R = isAxialBar ? axialBarArrange2.numright : mainBarArrange2.numright;
        int fillL, fillR;
        qreal b2b;

        //上端筋
        for (int dan = 0; dan < num1S.size(); ++dan) {
            for (int i = 0; i < num1L[dan]; ++i) {
                if (dan == 0)points << QPointF(ox - dx / 2 + RK + i * RY, oy + dy / 2 - RKu);
                if (dan >= 1)points << QPointF(ox - dx / 2 + RK + i * RY, oy + dy / 2 - RKu - RY1 - RY * (dan - 1));
            }
            for (int i = 0; i < num1R[dan]; ++i) {
                if (dan == 0)points << QPointF(ox + dx / 2 - RK - i * RY, oy + dy / 2 - RKu);
                if (dan >= 1)points << QPointF(ox + dx / 2 - RK - i * RY, oy + dy / 2 - RKu - RY1 - RY * (dan - 1));
            }
            fillL = qMax(1, num1L[dan]);
            fillR = qMax(1, num1R[dan]);
            b2b = (dx - (fillL + fillR - 2) * RY - 2 * RK) / (num1S[dan] - num1L[dan] - num1R[dan] + 1);
            for (int i = 0; i < num1S[dan] - num1L[dan] - num1R[dan]; ++i) {
                if (dan == 0)points << QPointF(ox - dx / 2 + RK + (fillL - 1)*RY + (i + 1)*b2b, oy + dy / 2 - RKu);
                if (dan >= 1)points << QPointF(ox - dx / 2 + RK + (fillL - 1)*RY + (i + 1)*b2b,
                                                   oy + dy / 2 - RKu - RY1 - RY * (dan - 1));
            }
        }
        //下端筋
        for (int dan = 0; dan < num2S.size(); ++dan) {
            for (int i = 0; i < num2L[dan]; ++i) {
                if (dan == 0)points << QPointF(ox - dx / 2 + RK + i * RY, oy - dy / 2 + RKb);
                if (dan >= 1)points << QPointF(ox - dx / 2 + RK + i * RY, oy - dy / 2 + RKb + RY1 + RY * (dan - 1));
            }
            for (int i = 0; i < num2R[dan]; ++i) {
                if (dan == 0)points << QPointF(ox + dx / 2 - RK - i * RY, oy - dy / 2 + RKb);
                if (dan >= 1)points << QPointF(ox + dx / 2 - RK - i * RY, oy - dy / 2 + RKb + RY1 + RY * (dan - 1));
            }
            fillL = qMax(1, num2L[dan]);
            fillR = qMax(1, num2R[dan]);
            b2b = (dx - (fillL + fillR - 2) * RY - 2 * RK) / (num2S[dan] - num2L[dan] - num2R[dan] + 1);
            for (int i = 0; i < num2S[dan] - num2L[dan] - num2R[dan]; ++i) {
                if (dan == 0)points << QPointF(ox - dx / 2 + RK + (fillL - 1)*RY + (i + 1)*b2b, oy - dy / 2 + RKb);
                if (dan >= 1)points << QPointF(ox - dx / 2 + RK + (fillL - 1)*RY + (i + 1)*b2b,
                                                   oy - dy / 2 + RKb + RY1 + RY * (dan - 1));
            }
        }
    }
    return points;
}

QList<QPointF> BaseSectionParameter::boltPointList() const
{
//y正方向＝画面上方向として座標を返すので、描画するときは正負注意
    QList<QPointF> points;

    if ( boltType == BaseBoltType::Type::CIRCLE ) {
        const qreal PI = 4 * qAtan(1.0);
        const qreal R = sizeB / 2 - edgehy1;
        for (int i = 0; i < bolt_ny1; ++i) {
            points << QPointF(R * qCos(i * 2 * PI / bolt_ny1), R * qSin(i * 2 * PI / bolt_ny1));
        }
        return points;
    } else {
        //const qreal hz2 = edgehz2;

        if (boltType == BaseBoltType::Type::RECTANGLE) {

            //ボルト数のチェック
            if (bolt_ny1 < 1 || bolt_ny2 < 1 || bolt_nz1 < 1 || bolt_nz2 < 1) {
                return points;
            }
            if (bolt_ny1 == 1 && bolt_ny2 != 1) {
                return points;
            }
            if (bolt_ny2 == 1 && bolt_ny1 != 1) {
                return points;
            }
            if (bolt_nz1 == 1 && bolt_nz2 != 1) {
                return points;
            }
            if (bolt_nz2 == 1 && bolt_nz1 != 1) {
                return points;
            }

            //上辺
            for (int i = 0; i < bolt_nz2; ++i) {
                qreal b2b = bolt_nz2 == 1 ? 0 : (sizeB - edgehy1 - edgehy2 ) / (bolt_nz2 - 1);
                points << QPointF(-sizeB / 2 + edgehy1 + i * b2b, sizeD / 2 - edgehz2);
            }
            //下辺
            for (int i = 0; i < bolt_nz1; ++i) {
                qreal b2b = bolt_nz1 == 1 ? 0 : (sizeB - edgehy1 - edgehy2 ) / (bolt_nz1 - 1);
                points << QPointF(-sizeB / 2 + edgehy1 + i * b2b, -sizeD / 2 + edgehz1);
            }
            //左辺
            for (int i = 1; i < bolt_ny1 - 1; ++i) {
                qreal b2b = bolt_ny1 == 1 ? 0 : (sizeD - edgehz1 - edgehz2) / (bolt_ny1 - 1);
                points << QPointF(-sizeB / 2 + edgehy1, -sizeD / 2 + edgehz1 + i * b2b);
            }
            //右辺
            for (int i = 1; i < bolt_ny2 - 1; ++i) {
                qreal b2b = bolt_ny2 == 1 ? 0 : (sizeD - edgehz1 - edgehz2) / (bolt_ny2 - 1);
                points << QPointF(sizeB / 2 - edgehy2 , -sizeD / 2 + edgehz1 + i * b2b);
            }
        } else if (boltType == BaseBoltType::Type::PARALLEL_LR) {
            //ボルト数のチェック
            if (bolt_ny1 < 1 || bolt_ny2 < 1 || bolt_nz1 < 1 || bolt_nz2 < 1) {
                return points;
            }
            if (bolt_ny1 == 1 && bolt_ny2 != 1) {
                return points;
            }
            if (bolt_ny2 == 1 && bolt_ny1 != 1) {
                return points;
            }
            if (bolt_nz1 == 1 && bolt_nz2 != 1) {
                return points;
            }
            if (bolt_nz2 == 1 && bolt_nz1 != 1) {
                return points;
            }

            //上辺
            for (int i = 0; i < bolt_nz2; ++i) {
                qreal b2bx = bolt_nz2 == 1 ? 0 : (sizeB - edgehy1 - edgehy2 ) / (bolt_nz2 - 1);
                qreal b2by = bolt_nz2 == 1 ? 0 : (edgehz1 - edgehz2) / (bolt_nz2 - 1);
                points << QPointF(-sizeB / 2 + edgehy1 + i * b2bx, sizeD / 2 - edgehz1 + i * b2by);
            }
            //下辺
            for (int i = 0; i < bolt_nz1; ++i) {
                qreal b2bx = bolt_nz1 == 1 ? 0 : (sizeB - edgehy1 - edgehy2 ) / (bolt_nz1 - 1);
                qreal b2by = bolt_nz1 == 1 ? 0 : (edgehz2 - edgehz1) / (bolt_nz1 - 1);
                points << QPointF(-sizeB / 2 + edgehy1 + i * b2bx, -sizeD / 2 + edgehz1 + i * b2by);
            }
            //左辺
            for (int i = 1; i < bolt_ny1 - 1; ++i) {
                qreal b2b = bolt_ny1 == 1 ? 0 : (sizeD - edgehz1 - edgehz1) / (bolt_ny1 - 1);
                points << QPointF(-sizeB / 2 + edgehy1, -sizeD / 2 + edgehz1 + i * b2b);
            }
            //右辺
            for (int i = 1; i < bolt_ny2 - 1; ++i) {
                qreal b2b = bolt_ny2 == 1 ? 0 : (sizeD - edgehz2 - edgehz2) / (bolt_ny2 - 1);
                points << QPointF(sizeB / 2 - edgehy2 , -sizeD / 2 + edgehz2 + i * b2b);
            }

        } else if (boltType == BaseBoltType::Type::PARALLEL_UD) {
            //ボルト数のチェック
            if (bolt_ny1 < 1 || bolt_ny2 < 1 || bolt_nz1 < 1 || bolt_nz2 < 1) {
                return points;
            }
            if (bolt_ny1 == 1 && bolt_ny2 != 1) {
                return points;
            }
            if (bolt_ny2 == 1 && bolt_ny1 != 1) {
                return points;
            }
            if (bolt_nz1 == 1 && bolt_nz2 != 1) {
                return points;
            }
            if (bolt_nz2 == 1 && bolt_nz1 != 1) {
                return points;
            }

            //上辺
            for (int i = 0; i < bolt_nz2; ++i) {
                qreal b2b = bolt_nz2 == 1 ? 0 : (sizeB - edgehy2  - edgehy2 ) / (bolt_nz2 - 1);
                points << QPointF(-sizeB / 2 + edgehy2  + i * b2b, sizeD / 2 - edgehz2);
            }
            //下辺
            for (int i = 0; i < bolt_nz1; ++i) {
                qreal b2b = bolt_nz1 == 1 ? 0 : (sizeB - edgehy1 - edgehy1) / (bolt_nz1 - 1);
                points << QPointF(-sizeB / 2 + edgehy1 + i * b2b, -sizeD / 2 + edgehz1);
            }
            //左辺
            for (int i = 1; i < bolt_ny1 - 1; ++i) {
                qreal b2bx = bolt_ny1 == 1 ? 0 : (edgehy2  - edgehy1) / (bolt_ny1 - 1);
                qreal b2by = bolt_ny1 == 1 ? 0 : (sizeD - edgehz1 - edgehz2) / (bolt_ny1 - 1);
                points << QPointF(-sizeB / 2 + edgehy1 + i * b2bx, -sizeD / 2 + edgehz1 + i * b2by);
            }
            //右辺
            for (int i = 1; i < bolt_ny2 - 1; ++i) {
                qreal b2bx = bolt_ny2 == 1 ? 0 : (edgehy1 - edgehy2 ) / (bolt_ny2 - 1);
                qreal b2by = bolt_ny2 == 1 ? 0 : (sizeD - edgehz1 - edgehz2) / (bolt_ny2 - 1);
                points << QPointF(sizeB / 2 - edgehy1 + i * b2bx, -sizeD / 2 + edgehz1 + i * b2by);
            }

        } else if (boltType == BaseBoltType::Type::DIAMOND) {
            //ボルト数のチェックなし

            //上辺
            for (int i = 0; i < bolt_nz2; ++i) {
                qreal b2b = bolt_nz2 == 1 ? 0 : (sizeB - edgehy2  - edgehy2 ) / (bolt_nz2 - 1);
                points << QPointF(-sizeB / 2 + edgehy2  + i * b2b, sizeD / 2 - edgehz1);
            }
            //下辺
            for (int i = 0; i < bolt_nz1; ++i) {
                qreal b2b = bolt_nz1 == 1 ? 0 : (sizeB - edgehy2  - edgehy2 ) / (bolt_nz1 - 1);
                points << QPointF(-sizeB / 2 + edgehy2  + i * b2b, -sizeD / 2 + edgehz1);
            }
            //左辺
            for (int i = 0; i < bolt_ny1; ++i) {
                qreal b2b = bolt_ny1 == 1 ? 0 : (sizeD - edgehz2 - edgehz2) / (bolt_ny1 - 1);
                points << QPointF(-sizeB / 2 + edgehy1, -sizeD / 2 + edgehz2 + i * b2b);
            }
            //右辺
            for (int i = 0; i < bolt_ny2; ++i) {
                qreal b2b = bolt_ny2 == 1 ? 0 : (sizeD - edgehz2 - edgehz2) / (bolt_ny2 - 1);
                points << QPointF(sizeB / 2 - edgehy1, -sizeD / 2 + edgehz2 + i * b2b);
            }

        }
        return points;
    }

}

QString SectionValues::writeData(QSharedPointer<N3DUuidTable> table) const{
    QString sec;
    switch(this->memberType){
        case DATATYPE::TPCOLUMN:
        sec.append(sectionName).append(",");
        sec.append(table == nullptr ? startFloorID.toString() : table->GetOrCreateN3dNumber(startFloorID, DATATYPE::TPFLOOR)).append(",");
        sec.append(table == nullptr ? endFloorID.toString() : table->GetOrCreateN3dNumber(endFloorID, DATATYPE::TPFLOOR)).append(",");
        sec.append(QString::number(line_param.finishLoad)).append(",");
        if (sectionType == STRUCTTYPE::STP_RC){
            sec.append(line_param.concMaterial).append(",");
            sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(j_param.writeData(this->memberType, this->sectionType));
        }else if (sectionType == STRUCTTYPE::STP_S){
            sec.append(QString::number(line_param.i_length)).append(",");
            sec.append(line_param.basePlate).append(",");
            sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(j_param.writeData(this->memberType, this->sectionType));
        }else if (sectionType == STRUCTTYPE::STP_SRC){
            sec.append(line_param.concMaterial).append(",");
            sec.append(line_param.filledConcMaterial).append(",");
            sec.append(QString::number(line_param.i_length)).append(",");
            sec.append(line_param.basePlate).append(",");
            sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(j_param.writeData(this->memberType, this->sectionType));
        }else if (sectionType == STRUCTTYPE::STP_CFT){
            sec.append(line_param.filledConcMaterial).append(",");
            sec.append(QString::number(line_param.i_length)).append(",");
            sec.append(line_param.basePlate).append(",");
            sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(j_param.writeData(this->memberType, this->sectionType));
        }
        break;
    case DATATYPE::TPGIRDER:
        sec.append(sectionName).append(",");
        sec.append(table == nullptr ? startFloorID.toString() : table->GetOrCreateN3dNumber(startFloorID, DATATYPE::TPFLOOR)).append(",");
        sec.append(table == nullptr ? endFloorID.toString() : table->GetOrCreateN3dNumber(endFloorID, DATATYPE::TPFLOOR)).append(",");
        sec.append(QString::number(line_param.isCantilever)).append(",");
        sec.append(QString::number(line_param.finishLoad)).append(",");
        if (sectionType == STRUCTTYPE::STP_RC){
            sec.append(line_param.concMaterial).append(",");
            sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(c_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(j_param.writeData(this->memberType, this->sectionType));
        }else if (sectionType == STRUCTTYPE::STP_S){
            sec.append(QString::number(line_param.i_length)).append(",");
            sec.append(QString::number(line_param.j_length)).append(",");
            sec.append(QString::number(line_param.i_hasStiffener)).append(",");
            sec.append(QString::number(line_param.j_hasStiffener)).append(",");
            sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(c_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(j_param.writeData(this->memberType, this->sectionType));
        }else if (sectionType == STRUCTTYPE::STP_SRC){
            sec.append(line_param.concMaterial).append(",");
            sec.append(QString::number(line_param.i_length)).append(",");
            sec.append(QString::number(line_param.j_length)).append(",");
            sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(c_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(j_param.writeData(this->memberType, this->sectionType));
        }
        break;
    case DATATYPE::TPBEAM:
        sec.append(sectionName).append(",");
        sec.append(QString::number(line_param.isCantilever)).append(",");
        sec.append(QString::number(line_param.finishLoad)).append(",");
        if (sectionType == STRUCTTYPE::STP_RC){
            sec.append(line_param.concMaterial).append(",");
            sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(c_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(j_param.writeData(this->memberType, this->sectionType));
        }else if (sectionType == STRUCTTYPE::STP_S){
            sec.append(QString::number(line_param.i_length)).append(",");
            sec.append(QString::number(line_param.j_length)).append(",");
            sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(c_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(j_param.writeData(this->memberType, this->sectionType));
        }else if (sectionType == STRUCTTYPE::STP_SRC){
            sec.append(line_param.concMaterial).append(",");
            sec.append(QString::number(line_param.i_length)).append(",");
            sec.append(QString::number(line_param.j_length)).append(",");
            sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(c_param.writeData(this->memberType, this->sectionType)).append(",");
            sec.append(j_param.writeData(this->memberType, this->sectionType));
        }
        break;
    case DATATYPE::TPBRACE:
        sec.append(sectionName).append(",");
        sec.append(table == nullptr ? startFloorID.toString() : table->GetOrCreateN3dNumber(startFloorID, DATATYPE::TPFLOOR)).append(",");
        sec.append(table == nullptr ? endFloorID.toString() : table->GetOrCreateN3dNumber(endFloorID, DATATYPE::TPFLOOR)).append(",");
        sec.append(QString::number(line_param.finishLoad)).append(",");
        sec.append(QString::number(line_param.hasCompResistance)).append(",");
        sec.append(QString::number(line_param.i_length)).append(",");
        sec.append(QString::number(line_param.j_length)).append(",");
        sec.append(QString::number(line_param.direct_area)).append(",");
        sec.append(QString::number(line_param.direct_effectiveArea)).append(",");
        sec.append(QString::number(line_param.direct_slenderRatio)).append(",");
        sec.append(QString::number(line_param.direct_unitmass)).append(",");
        sec.append(i_param.writeData(this->memberType, this->sectionType)).append(",");
        sec.append(c_param.writeData(this->memberType, this->sectionType)).append(",");
        sec.append(j_param.writeData(this->memberType, this->sectionType));
        break;
    case DATATYPE::TPSLAB:
        sec.append(sectionName).append(",");
        sec.append(PlaneType::enumToString(plane_param.planeType)).append(",");
        sec.append(plane_param.concMaterial).append(",");
        sec.append(QString::number(plane_param.concThickness)).append(",");
        sec.append(plane_param.steelMaterial).append(",");
        sec.append(QString::number(plane_param.steelThickness)).append(",");
        sec.append(QString::number(plane_param.finishLoad)).append(",");
        sec.append(QString::number(plane_param.loadDir)).append(",");
        sec.append(plane_param.sBar_upperEdgeA.phi).append(",");
        sec.append(QString::number(plane_param.sBar_upperEdgeA.pitch)).append(",");
        sec.append(plane_param.sBar_lowerEdgeA.phi).append(",");
        sec.append(QString::number(plane_param.sBar_lowerEdgeA.pitch)).append(",");
        sec.append(plane_param.sBar_upperCenterA.phi).append(",");
        sec.append(QString::number(plane_param.sBar_upperCenterA.pitch)).append(",");
        sec.append(plane_param.sBar_lowerCenterA.phi).append(",");
        sec.append(QString::number(plane_param.sBar_lowerCenterA.pitch)).append(",");
        sec.append(plane_param.sBar_upperEdgeCenterB.phi).append(",");
        sec.append(QString::number(plane_param.sBar_upperEdgeCenterB.pitch)).append(",");
        sec.append(plane_param.sBar_lowerEdgeCenterB.phi).append(",");
        sec.append(QString::number(plane_param.sBar_lowerEdgeCenterB.pitch)).append(",");
        sec.append(plane_param.lBar_upperEdgeA.phi).append(",");
        sec.append(QString::number(plane_param.lBar_upperEdgeA.pitch)).append(",");
        sec.append(plane_param.lBar_lowerEdgeA.phi).append(",");
        sec.append(QString::number(plane_param.lBar_lowerEdgeA.pitch)).append(",");
        sec.append(plane_param.lBar_upperCenterA.phi).append(",");
        sec.append(QString::number(plane_param.lBar_upperCenterA.pitch)).append(",");
        sec.append(plane_param.lBar_lowerCenterA.phi).append(",");
        sec.append(QString::number(plane_param.lBar_lowerCenterA.pitch)).append(",");
        sec.append(plane_param.lBar_upperEdgeCenterB.phi).append(",");
        sec.append(QString::number(plane_param.lBar_upperEdgeCenterB.pitch)).append(",");
        sec.append(plane_param.lBar_lowerEdgeCenterB.phi).append(",");
        sec.append(QString::number(plane_param.lBar_lowerEdgeCenterB.pitch));
        break;
    case DATATYPE::TPWALL:
        if (sectionType == STRUCTTYPE::STP_S){
            sec.append(sectionName).append(",");
            sec.append(table == nullptr ? startFloorID.toString() : table->GetOrCreateN3dNumber(startFloorID, DATATYPE::TPFLOOR)).append(",");
            sec.append(table == nullptr ? endFloorID.toString() : table->GetOrCreateN3dNumber(endFloorID, DATATYPE::TPFLOOR)).append(",");
            sec.append(PlaneType::enumToString(plane_param.planeType)).append(",");
            sec.append(plane_param.steelMaterial).append(",");
            sec.append(QString::number(plane_param.steelThickness)).append(",");
            //sec.append(QString::number(plane_param.finishLoad)); vRibPitch
            //sec.append(QString::number(plane_param.finishLoad)); vRibType
            //sec.append(QString::number(plane_param.finishLoad)); vRibThickness
            //sec.append(QString::number(plane_param.finishLoad)); vRibBreadth
            //sec.append(QString::number(plane_param.finishLoad)); vRibPitch
            //sec.append(QString::number(plane_param.finishLoad)); hRibType
            //sec.append(QString::number(plane_param.finishLoad)); hRibThickness
            //sec.append(QString::number(plane_param.finishLoad)); hRibBreadth
        }else{
            sec.append(sectionName).append(",");
            sec.append(table == nullptr ? startFloorID.toString() : table->GetOrCreateN3dNumber(startFloorID, DATATYPE::TPFLOOR)).append(",");
            sec.append(table == nullptr ? endFloorID.toString() : table->GetOrCreateN3dNumber(endFloorID, DATATYPE::TPFLOOR)).append(",");
            sec.append(PlaneType::enumToString(plane_param.planeType)).append(",");
            sec.append(plane_param.concMaterial).append(",");
            sec.append(QString::number(plane_param.concThickness)).append(",");
            sec.append(plane_param.steelMaterial).append(",");
            sec.append(QString::number(plane_param.steelThickness)).append(",");
            sec.append(QString::number(plane_param.finishLoad)).append(",");
            sec.append(QString::number(plane_param.isDoubleBar)).append(",");
            sec.append(plane_param.vBar.phi).append(",");
            sec.append(QString::number(plane_param.vBar.pitch)).append(",");
            sec.append(plane_param.hBar.phi).append(",");
            sec.append(QString::number(plane_param.hBar.pitch)).append(",");
            sec.append(QString::number(plane_param.vOpeningBar_small.num)).append(",");
            sec.append(plane_param.vOpeningBar_small.phi).append(",");
            sec.append(QString::number(plane_param.hOpeningBar_small.num)).append(",");
            sec.append(plane_param.hOpeningBar_small.phi).append(",");
            sec.append(QString::number(plane_param.dOpeningBar_small.num)).append(",");
            sec.append(plane_param.dOpeningBar_small.phi).append(",");
            sec.append(QString::number(plane_param.vOpeningBar_large.num)).append(",");
            sec.append(plane_param.vOpeningBar_large.phi).append(",");
            sec.append(QString::number(plane_param.hOpeningBar_large.num)).append(",");
            sec.append(plane_param.hOpeningBar_large.phi).append(",");
            sec.append(QString::number(plane_param.dOpeningBar_large.num)).append(",");
            sec.append(plane_param.dOpeningBar_large.phi);
        }
        break;
    case DATATYPE::TPUWALL:
        sec.append(sectionName).append(",");
        sec.append(plane_param.concMaterial).append(",");
        sec.append(QString::number(plane_param.concThickness)).append(",");
        sec.append(plane_param.steelMaterial).append(",");
        sec.append(QString::number(plane_param.steelThickness)).append(",");
        sec.append(QString::number(plane_param.finishLoad)).append(",");
        sec.append(plane_param.vBar_outerTop.phi).append(",");
        sec.append(QString::number(plane_param.vBar_outerTop.pitch)).append(",");
        sec.append(plane_param.vBar_outerMiddle.phi).append(",");
        sec.append(QString::number(plane_param.vBar_outerMiddle.pitch)).append(",");
        sec.append(plane_param.vBar_outerBottom.phi).append(",");
        sec.append(QString::number(plane_param.vBar_outerBottom.pitch)).append(",");
        sec.append(plane_param.hBar_outerEdge.phi).append(",");
        sec.append(QString::number(plane_param.hBar_outerEdge.pitch)).append(",");
        sec.append(plane_param.hBar_outerCenter.phi).append(",");
        sec.append(QString::number(plane_param.hBar_outerCenter.pitch)).append(",");
        sec.append(plane_param.vBar_innerTop.phi).append(",");
        sec.append(QString::number(plane_param.vBar_innerTop.pitch)).append(",");
        sec.append(plane_param.vBar_innerMiddle.phi).append(",");
        sec.append(QString::number(plane_param.vBar_innerMiddle.pitch)).append(",");
        sec.append(plane_param.vBar_innerBottom.phi).append(",");
        sec.append(QString::number(plane_param.vBar_innerBottom.pitch)).append(",");
        sec.append(plane_param.hBar_innerEdge.phi).append(",");
        sec.append(QString::number(plane_param.hBar_innerEdge.pitch)).append(",");
        sec.append(plane_param.hBar_innerCenter.phi).append(",");
        sec.append(QString::number(plane_param.hBar_innerCenter.pitch));
        break;
    case DATATYPE::TPWALLOPEN:
        sec.append(sectionName).append(",");
        sec.append(WallOpenType::enumToStringType(wopen_param.openType)).append(",");
        sec.append(QString::number(wopen_param.isHorizontalFace)).append(",");
        sec.append(QString::number(static_cast<int>(wopen_param.hPosition))).append(",");
        sec.append(QString::number(wopen_param.isVerticalFace)).append(",");
        sec.append(QString::number(static_cast<int>(wopen_param.vPosition))).append(",");
        sec.append(QString::number(wopen_param.ox)).append(",");
        sec.append(QString::number(wopen_param.oy)).append(",");
        sec.append(QString::number(wopen_param.width)).append(",");
        sec.append(QString::number(wopen_param.height)).append(",");
        sec.append(QString::number(wopen_param.slitL)).append(",");
        sec.append(QString::number(wopen_param.slitR)).append(",");
        sec.append(QString::number(wopen_param.slitB)).append(",");
        sec.append(QString::number(wopen_param.slitT)).append(",");
        sec.append(wopen_param.comment);
        break;
    case DATATYPE::TPBASE:
        sec.append(sectionName).append(",");
        if (sectionType == STRUCTTYPE::STP_S){
            sec.append(base_param.isStandardType).append(",");
            sec.append(base_param.productName).append(",");
            sec.append(QString::number(base_param.sizeB)).append(",");
            sec.append(QString::number(base_param.sizeD)).append(",");
            sec.append(QString::number(base_param.sizeT)).append(",");
            sec.append(base_param.basePlateMaterial).append(",");
            sec.append(QString::number(base_param.edgehy1)).append(",");
            sec.append(QString::number(base_param.edgehz1)).append(",");
            sec.append(QString::number(base_param.bolt_ny1)).append(",");
            sec.append(QString::number(base_param.bolt_nz1)).append(",");
            sec.append(QString::number(base_param.boltPhi)).append(",");
            sec.append(base_param.boltMaterial).append(",");
            sec.append(QString::number(base_param.anchorLength));
        }else if (sectionType == STRUCTTYPE::STP_SRC){
            sec.append(QString::number(base_param.B0)).append(",");
            sec.append(QString::number(base_param.D0)).append(",");
            sec.append(base_param.concMaterial).append(",");
            sec.append(QString::number(static_cast<int>(base_param.basePlateType))).append(",");
            sec.append(QString::number(base_param.sizeB)).append(",");
            sec.append(QString::number(base_param.sizeD)).append(",");
            sec.append(QString::number(base_param.sizeT)).append(",");
            sec.append(QString::number(base_param.SML)).append(",");
            sec.append(QString::number(base_param.edgehy1)).append(",");
            sec.append(QString::number(base_param.edgehy2)).append(",");
            sec.append(QString::number(base_param.edgehz1)).append(",");
            sec.append(QString::number(base_param.edgehz2)).append(",");
            sec.append(QString::number(static_cast<int>(base_param.boltType))).append(",");
            sec.append(QString::number(base_param.bolt_ny1)).append(",");
            sec.append(QString::number(base_param.bolt_ny2)).append(",");
            sec.append(QString::number(base_param.bolt_nz1)).append(",");
            sec.append(QString::number(base_param.bolt_nz2)).append(",");
            sec.append(QString::number(base_param.boltPhi)).append(",");
            sec.append(base_param.boltMaterial);
        }
        break;
    case DATATYPE::TPDAMPER:
        sec.append(sectionName).append(",");
        sec.append(QString::number(static_cast<int>(damper_param.productType))).append(",");
        sec.append(QString::number(damper_param.selfWeight)).append(",");
        sec.append(QString::number(damper_param.vecX)).append(",");
        sec.append(QString::number(damper_param.vecY)).append(",");
        sec.append(QString::number(damper_param.vecZ)).append(",");
        Q_FOREACH(auto p, damper_param.parameters){
            sec.append(QString::number(p)).append("/");
        }
        sec.append(",");
        sec.append(QString::number(damper_param.period)).append(",");
        sec.append(QString::number(damper_param.maxDisp)).append(",");
        sec.append(QString::number(damper_param.modelFlag)).append(",");
        sec.append(QString::number(damper_param.rigidLength)).append(",");
        sec.append(QString::number(damper_param.damperDirection)).append(",");
        sec.append(QString::number(damper_param.tAnalysis)).append(",");
        sec.append(QString::number(damper_param.factor)).append(",");
        sec.append(QString::number(damper_param.frameRigidity));
        break;
    case DATATYPE::TPISO:
        sec.append(sectionName).append(",");
        sec.append(isolator_param.productType).append(",");
        sec.append(isolator_param.productName).append(",");
        sec.append(QString::number(static_cast<int>(hspring_param.hysteresis))).append(",");
        sec.append(QString::number(hspring_param.K1)).append(",");
        sec.append(QString::number(hspring_param.K2)).append(",");
        sec.append(QString::number(hspring_param.K3)).append(",");
        sec.append(QString::number(hspring_param.Fy1m)).append(",");
        sec.append(QString::number(hspring_param.Fy1p)).append(",");
        sec.append(QString::number(hspring_param.Fy2m)).append(",");
        sec.append(QString::number(hspring_param.Fy2p)).append(",");
        sec.append(QString::number(static_cast<int>(vspring_param.hysteresis))).append(",");
        sec.append(QString::number(vspring_param.K1)).append(",");
        sec.append(QString::number(vspring_param.K2)).append(",");
        sec.append(QString::number(vspring_param.K3)).append(",");
        sec.append(QString::number(hspring_param.Fy1p)).append(",");
        sec.append(QString::number(vspring_param.Fy2p)).append(",");
        sec.append(QString::number(isolator_param.selfWeight)).append(",");
        sec.append(isolator_param.renderShape).append(",");
        sec.append(QString::number(isolator_param.sizeD)).append(",");
        sec.append(QString::number(isolator_param.sizeH)).append(",");
        sec.append(QString::number(isolator_param.settingLevel)).append(",");
        sec.append(QString::number(isolator_param.displacement)).append(",");
        sec.append(QString::number(isolator_param.springNumber)).append(",");
        sec.append(QString::number(isolator_param.inflectionRatio)).append(",");
        break;
    }
    return sec;
}

qreal LinePartsParameterS::standardArea() const
{
    return UnifiedCalcLoad::getStandardArea(standardName);
}

qreal LinePartsParameterS::getSizeB() const{
    switch(steelType){
    case SteelType::Type::ST_P:
    case SteelType::Type::ST_WP:
    case SteelType::Type::ST_WBR:
        return sizeH;
    default:
        return sizeB;
    }
}

qreal LinePartsParameterS::getSizeB_T() const{
    switch(steelType){
    case SteelType::Type::ST_P:
    case SteelType::Type::ST_WP:
    case SteelType::Type::ST_WBR:
        return H_T;
    default:
        return B_T;
    }
}



QString LinePartsParameterSRC::writeData(const DATATYPE& dataType, const STRUCTTYPE& structType) const{
    QString sec;
    switch(dataType){
    case DATATYPE::TPCOLUMN:
        switch(structType){
        case STRUCTTYPE::STP_RC:
            sec.append(QString::number(Dy)).append(",");
            sec.append(QString::number(Dz)).append(",");
            sec.append(mainBarArrange1.text().trimmed()).append(",");
            sec.append(mainBarArrange2.text().trimmed()).append(",");
            sec.append(mainBar1).append(",");
            sec.append(mainBar2).append(",");
            sec.append(QString::number(axialCenterY)).append(",");
            sec.append(QString::number(axialCenterZ)).append(",");
            sec.append(QString::number(axialDy)).append(",");
            sec.append(QString::number(axialDz)).append(",");
            sec.append(axialBarArrange1.text().trimmed()).append(",");
            sec.append(axialBarArrange2.text().trimmed()).append(",");
            sec.append(axialBar.trimmed()).append(",");
            sec.append(QString::number(hoopY)).append(",");
            sec.append(QString::number(hoopZ)).append(",");
            sec.append(hoopBar.trimmed()).append(",");
            sec.append(QString::number(hoopPitch)).append(",");
            sec.append(QString::number(RK)).append(",");
            sec.append(QString::number(RY));
            break;
        case STRUCTTYPE::STP_S:
            sec.append(steelMaterial.trimmed()).append(",");
            sec.append(SteelType::enumToString(steelType)).append(",");
            sec.append(standardName.trimmed()).append(",");
            sec.append(QString::number(sizeH)).append(",");
            sec.append(QString::number(sizeB)).append(",");
            sec.append(QString::number(t1)).append(",");
            sec.append(QString::number(t2)).append(",");
            sec.append(QString::number(H_T)).append(",");
            sec.append(QString::number(B_T)).append(",");
            sec.append(QString::number(t1_T)).append(",");
            sec.append(QString::number(t2_T));
            break;
        case STRUCTTYPE::STP_SRC:
            sec.append(QString::number(Dy)).append(",");
            sec.append(QString::number(Dz)).append(",");
            sec.append(mainBarArrange1.text().trimmed()).append(",");
            sec.append(mainBarArrange2.text().trimmed()).append(",");
            sec.append(mainBar1).append(",");
            sec.append(QString::number(axialCenterY)).append(",");
            sec.append(QString::number(axialCenterZ)).append(",");
            sec.append(QString::number(axialDy)).append(",");
            sec.append(QString::number(axialDz)).append(",");
            sec.append(axialBarArrange1.text().trimmed()).append(",");
            sec.append(axialBarArrange2.text().trimmed()).append(",");
            sec.append(axialBar.trimmed()).append(",");
            sec.append(QString::number(hoopY)).append(",");
            sec.append(QString::number(hoopZ)).append(",");
            sec.append(hoopBar.trimmed()).append(",");
            sec.append(QString::number(hoopPitch)).append(",");
            sec.append(QString::number(RK)).append(",");
            sec.append(QString::number(RY));
            sec.append(steelMaterial.trimmed()).append(",");
            sec.append(SteelType::enumToString(steelType)).append(",");
            sec.append(standardName.trimmed()).append(",");
            sec.append(QString::number(sizeH)).append(",");
            sec.append(QString::number(sizeB)).append(",");
            sec.append(QString::number(t1)).append(",");
            sec.append(QString::number(t2)).append(",");
            sec.append(QString::number(cY)).append(",");
            sec.append(QString::number(cZ)).append(",");
            sec.append(SteelType::enumToString(steelType_T)).append(",");
            sec.append(standardName_T.trimmed()).append(",");
            sec.append(QString::number(H_T)).append(",");
            sec.append(QString::number(B_T)).append(",");
            sec.append(QString::number(t1_T)).append(",");
            sec.append(QString::number(t2_T)).append(",");
            sec.append(QString::number(cY_T)).append(",");
            sec.append(QString::number(cZ_T));
            break;
        case STRUCTTYPE::STP_CFT:
            sec.append(steelMaterial.trimmed()).append(",");
            sec.append(SteelType::enumToString(steelType)).append(",");
            sec.append(standardName.trimmed()).append(",");
            sec.append(QString::number(sizeH)).append(",");
            sec.append(QString::number(sizeB)).append(",");
            sec.append(QString::number(t1)).append(",");
            sec.append(QString::number(t2));
            break;
        }
        break;
    case DATATYPE::TPGIRDER:
    case DATATYPE::TPBEAM:
        switch(structType){
        case STRUCTTYPE::STP_RC:
            sec.append(QString::number(Dy)).append(",");
            sec.append(QString::number(Dz)).append(",");
            sec.append(mainBarArrange1.text().trimmed()).append(",");
            sec.append(mainBarArrange2.text().trimmed()).append(",");
            sec.append(mainBar1).append(",");
            sec.append(mainBar2).append(",");
            sec.append(QString::number(hoopZ)).append(",");
            sec.append(hoopBar.trimmed()).append(",");
            sec.append(QString::number(hoopPitch)).append(",");
            sec.append(QString::number(RK)).append(",");
            sec.append(QString::number(RKu)).append(",");
            sec.append(QString::number(RKb)).append(",");
            sec.append(QString::number(RY));
            break;
        case STRUCTTYPE::STP_S:
            sec.append(steelMaterial.trimmed()).append(",");
            sec.append(steelWebMaterial.trimmed()).append(",");
            sec.append(SteelType::enumToString(steelType)).append(",");
            sec.append(standardName.trimmed()).append(",");
            sec.append(QString::number(sizeH)).append(",");
            sec.append(QString::number(sizeB)).append(",");
            sec.append(QString::number(t1)).append(",");
            sec.append(QString::number(t2));
            break;
        case STRUCTTYPE::STP_SRC:
            sec.append(QString::number(Dy)).append(",");
            sec.append(QString::number(Dz)).append(",");
            sec.append(mainBarArrange1.text().trimmed()).append(",");
            sec.append(mainBarArrange2.text().trimmed()).append(",");
            sec.append(mainBar1).append(",");
            sec.append(mainBar2).append(",");
            sec.append(QString::number(hoopZ)).append(",");
            sec.append(hoopBar.trimmed()).append(",");
            sec.append(QString::number(hoopPitch)).append(",");
            sec.append(QString::number(RK)).append(",");
            sec.append(QString::number(RKu)).append(",");
            sec.append(QString::number(RKb)).append(",");
            sec.append(QString::number(RY)).append(",");
            sec.append(QString::number(RY1)).append(",");
            sec.append(steelMaterial.trimmed()).append(",");
            sec.append(steelWebMaterial.trimmed()).append(",");
            sec.append(SteelType::enumToString(steelType)).append(",");
            sec.append(standardName.trimmed()).append(",");
            sec.append(QString::number(sizeH)).append(",");
            sec.append(QString::number(sizeB)).append(",");
            sec.append(QString::number(t1)).append(",");
            sec.append(QString::number(t2)).append(",");
            sec.append(QString::number(cY)).append(",");
            sec.append(QString::number(cZ));
            break;
        }
        break;
    case DATATYPE::TPBRACE:
        sec.append(steelMaterial.trimmed()).append(",");
        sec.append(SteelType::enumToString(steelType)).append(",");
        sec.append(standardName.trimmed()).append(",");
        sec.append(QString::number(sizeH)).append(",");
        sec.append(QString::number(sizeB)).append(",");
        sec.append(QString::number(t1)).append(",");
        sec.append(QString::number(t2));
        break;
    }



    return sec;
}
} // namespace post3dapp
