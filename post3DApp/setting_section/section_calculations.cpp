#include "section_calculations.h"

#include <qmath.h>
#include <QDebug>

#include "define_material_struct.h"
#include "define_sectiontable.h"
#include "unified_inputdata.h"

namespace post3dapp{
///// S /////
///// 構造体による呼び出し /////
qreal SectionCalculations::calcA_S(const LinePartsParameterS &paramS)
{
    qDebug()<<"calcA_S init";
    if(paramS.steelType==SteelType::Type::ST_CROSSH){
        qreal A = 0.0;
        A += calcA_S(SteelType::Type::ST_WH, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2);
        qDebug()<<A;
        A += calcA_S(SteelType::Type::ST_WI, paramS.H_T, paramS.B_T, paramS.t1_T, paramS.t2_T);
        qDebug()<<A;
        return A;
    }
    else if(paramS.steelType==SteelType::Type::ST_CROSS){
        qreal A = 0.0;
        A += calcA_S(SteelType::Type::ST_WPL, paramS.sizeH, paramS.t1, 0, 0);
        A += calcA_S(SteelType::Type::ST_WPL, paramS.t2, paramS.sizeB, 0, 0);
        return A;
    }
    qDebug()<<"calcA_S no cross";
    return calcA_S(paramS.steelType, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2);
}

qreal SectionCalculations::calcAe_S(const LinePartsParameterS &paramS)
{
    MATERIALVALUES mv =
        UnifiedInputData::getInInstance()->getMaterialValue(DATATYPE::TPMASTEEL, paramS.steelMaterial);
    return calcAe_S(paramS.steelType, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2, mv.F, mv.F);
}

qreal SectionCalculations::calcI_S(const LinePartsParameterS &paramS, int axis)
{
    if(paramS.steelType==SteelType::Type::ST_CROSSH){
        qreal A = 0.0;
        A += calcI_S(SteelType::Type::ST_WH,axis, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2);
        A += calcI_S(SteelType::Type::ST_WI, axis,paramS.H_T, paramS.B_T, paramS.t1_T, paramS.t2_T);
        return A;
    }
    else if(paramS.steelType==SteelType::Type::ST_CROSS){
        qreal A = 0.0;
        A += calcI_S(SteelType::Type::ST_WPL,axis, paramS.sizeH, paramS.t1, 0, 0);
        A += calcI_S(SteelType::Type::ST_WPL,axis, paramS.t2, paramS.sizeB, 0, 0);
        return A;
    }
    return calcI_S(paramS.steelType, axis, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2);
}

qreal SectionCalculations::calcZ_S(const LinePartsParameterS &paramS, int axis, qreal I_s)
{
    if(paramS.steelType==SteelType::Type::ST_CROSSH){
        qreal A = 0.0;
        A += calcZ_S(SteelType::Type::ST_WH,axis, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2, I_s);
        A += calcZ_S(SteelType::Type::ST_WI, axis,paramS.H_T, paramS.B_T, paramS.t1_T, paramS.t2_T, I_s);
        return A;
    }
    else if(paramS.steelType==SteelType::Type::ST_CROSS){
        qreal A = 0.0;
        A += calcZ_S(SteelType::Type::ST_WPL,axis, paramS.sizeH, paramS.t1, 0, 0, I_s);
        A += calcZ_S(SteelType::Type::ST_WPL,axis, paramS.t2, paramS.sizeB, 0, 0, I_s);
        return A;
    }
    return calcZ_S(paramS.steelType, axis, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2, I_s);
}

qreal SectionCalculations::calcZe_S(const LinePartsParameterS &paramS, int axis)
{
    MATERIALVALUES mv =
        UnifiedInputData::getInInstance()->getMaterialValue(DATATYPE::TPMASTEEL, paramS.steelMaterial);
    return calcZe_S(paramS.steelType, axis, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2, mv.F,
                    mv.F);
}

qreal SectionCalculations::calcZp_S(const LinePartsParameterS &paramS, int axis)
{
    return calcZp_S(paramS.steelType, axis, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2);
}

qreal SectionCalculations::calcWTratio_S(const LinePartsParameterS &paramS, SectionPart::Part part)
{
    return calcWTratio_S(paramS.steelType, part, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2);
}

SectionRank::Rank SectionCalculations::judgeRank(const LinePartsParameterS &paramS,
                                                 DATATYPE mtype, SectionPart::Part part)
{
    qreal wtratio = calcWTratio_S(paramS.steelType, part, paramS.sizeH, paramS.sizeB, paramS.t1,
                                  paramS.t2);
    qreal F = UnifiedInputData::getInInstance()->getMaterialValue(DATATYPE::TPMASTEEL, paramS.steelMaterial).F;
    return judgeRank(paramS.steelType, part, mtype, wtratio, F);
}

SectionRank::Rank SectionCalculations::judgeRankH_SN(const LinePartsParameterS &paramS,
                                                     DATATYPE mtype)
{
    int steel;
    switch (paramS.steelType) {
    case SteelType::Type::ST_WH :
    case SteelType::Type::ST_H  :
    case SteelType::Type::ST_WI :
    case SteelType::Type::ST_I  :
        if     (paramS.steelMaterial.left(5) == "SN490") {
            steel = 490;   //smat.FではなくparamS.steelで判断
        } else if (paramS.steelMaterial.left(5) == "SN400") {
            steel = 400;
        } else {
            return SectionRank::Rank::UNDEFINED;
        }
        break;
    default :
        return SectionRank::Rank::UNDEFINED;
    }
    if ( paramS.t1 < 1.0e-5 || paramS.t2 < 1.0e-5 ) return SectionRank::Rank::UNDEFINED;
    return judgeRankH_SN(mtype, paramS.sizeH, paramS.sizeB, paramS.t1, paramS.t2, steel);
}



///// 内部的な計算 /////
qreal SectionCalculations::calcA_S(SteelType::Type sshape, qreal H, qreal B, qreal t1, qreal t2)
//ss:鋼材形状＝enumで定義,H:高さ,B:幅,t1:ウェブ厚,t2:フランジ厚
{
    qreal area_s;
    switch (sshape) {
    case SteelType::Type::ST_WH:   //WH - HxBxt1xt2
    case SteelType::Type::ST_WI:   //WI - HxBxt1xt2  Hが幅方向
    case SteelType::Type::ST_WC1:   //WC - HxBxt1xt2
    case SteelType::Type::ST_WC2:   //WC - HxBxt1xt2
    case SteelType::Type::ST_WC3:   //WC - HxBxt1xt2
    case SteelType::Type::ST_WC4:   //WC - HxBxt1xt2
        area_s = 2.0 * B * t2 + (H - 2.0 * t2) * t1;
        break;
    case SteelType::Type::ST_WT1:   //WT - HxBxt1xt2
    case SteelType::Type::ST_WT2:   //WT - HxBxt1xt2
    case SteelType::Type::ST_WT3:   //WT - HxBxt1xt2
    case SteelType::Type::ST_WT4:   //WT - HxBxt1xt2
        area_s = B * t2 + (H - t2) * t1;
        break;
    case SteelType::Type::ST_WBX:   //BX - HxBxt1xt2
        area_s = 2.0 * B * t2 + 2.0 * (H - 2.0 * t2) * t1;
        break;
    case SteelType::Type::ST_WP:    //P  - Hx     t1
        area_s = M_PI * t1 * (H - t1);
        break;
    case SteelType::Type::ST_WPL:   //PL - HxB
        area_s = H * B;
        break;
    case SteelType::Type::ST_WBR:    //R  - H
        area_s = M_PI * H * H / 4.0;
        break;
    default :
        return 0.0;
    }
    return area_s;
}

qreal SectionCalculations::calcAe_S(SteelType::Type sshape, qreal H, qreal B,
                                    qreal t1, qreal t2, qreal F1, qreal F2)
//圧縮用有効断面積（告示FCランク以下となる部分を控除）
//JFEのカタログ(2012)は旧S規準（新規準ではヤング率依存）、
//日鉄住金のカタログ(2012)は告示による。
{
    if ( F1 < 1.0e-5 || F2 < 1.0e-5 ) return 0.0;

    qreal Ae, He, Be;
//web
    const qreal JUDGE11 = 15.5 * qSqrt(235 / F1); //圧縮を受ける突出部
    const qreal JUDGE21 = 48.0 * qSqrt(235 / F1); //圧縮を受ける２辺支持部
//  const qreal JUDGE31 = 71.0*qSqrt(235/F1);   //せん断を受ける２辺支持部
//flange
    const qreal JUDGE12 = 15.5 * qSqrt(235 / F2); //
    const qreal JUDGE22 = 48.0 * qSqrt(235 / F2); //
//  const qreal JUDGE32 = 71.0*qSqrt(235/F2);   //

    switch (sshape) {
    case SteelType::Type::ST_WH :
    case SteelType::Type::ST_WI :
        Be = ((0.5 * B / t2 <= JUDGE12) ? 0.5 * B / t2 : JUDGE12) * 2 * t2;
        He = (((H - 2 * t2) / t1 <= JUDGE21) ? (H - 2 * t2) / t1 : JUDGE21) * t1 + 2 * t2;
        break;
    case SteelType::Type::ST_WT1 :
    case SteelType::Type::ST_WT2 :
    case SteelType::Type::ST_WT3 :
    case SteelType::Type::ST_WT4 :
        Be = ((0.5 * B / t2 <= JUDGE12) ? 0.5 * B / t2 : JUDGE12) * 2 * t2;
        He = (((H - t2) / t1 <= JUDGE11) ? (H - t2) / t1 : JUDGE11) * t1 + t2;
        break;
    case SteelType::Type::ST_WC1:
    case SteelType::Type::ST_WC2:
    case SteelType::Type::ST_WC3:
    case SteelType::Type::ST_WC4:
        Be = ((B / t2 <= JUDGE12) ? B / t2 : JUDGE12) * t2;
        He = (((H - 2 * t2) / t1 <= JUDGE21) ? (H - 2 * t2) / t1 : JUDGE21) * t1 + 2 * t2;
        break;
    case SteelType::Type::ST_WBX :
        Be = ((B / t2 <= JUDGE22) ? B / t2 : JUDGE22) * t2;
        He = ((H / t1 <= JUDGE21) ? H / t1 : JUDGE21) * t1;
        break;
    default :
        return 0;
    }
    Ae = calcA_S(sshape, He, Be, t1, t2);
    return Ae;
}

qreal SectionCalculations::calcI_S(SteelType::Type sshape, int axis, qreal H, qreal B,
                                   qreal t1, qreal t2)
//axis= X軸周り…0,Y軸周り…1
{
    qreal I_s = 0;

    if (axis == 0) {
        switch (sshape) {
        case SteelType::Type::ST_WH:   //WH - HxBxt1xt2
        case SteelType::Type::ST_WC1:   //WC - HxBxt1xt2
            I_s = calcI_typeH(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WI:   //WI - HxBxt1xt2  Hが幅方向
            I_s = calcI_typeI(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WT1:   //WT - HxBxt1xt2
            I_s = calcI_typeT(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WT2:   //WT - HxBxt1xt2
            I_s = calcI_typeI(H, B, t1, 0.5 * t2);
            break;
        case SteelType::Type::ST_WBX:   //BX - HxBxt1xt2
            I_s = calcI_typeH(H, B, 2 * t1, t2);
            break;
        case SteelType::Type::ST_WP:    //P  - Hx     t1
            I_s = calcI_circ(H) - calcI_circ(H - 2 * t1);
            break;
        case SteelType::Type::ST_WPL:   //PL - HxB
            I_s = calcI_rect(B, H);
            break;
        case SteelType::Type::ST_WBR:    //R  - H
            I_s = calcI_circ(H);
            break;
        default :
            return 0;
        }
    } else {
        switch (sshape) {
        case SteelType::Type::ST_WH:   //WH - HxBxt1xt2
            I_s = calcI_typeI(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WI:   //WI - HxBxt1xt2  Hが幅方向
            I_s = calcI_typeH(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WT1:   //WT - HxBxt1xt2
            I_s = calcI_typeI(H, B, t1, 0.5 * t2);
            break;
        case SteelType::Type::ST_WT2:   //WT - HxBxt1xt2
            I_s = calcI_typeI(H, B, t1, 0.5 * t2);
            break;
        case SteelType::Type::ST_WC1 :
            I_s = calcI_typeT(B, H, 2 * t2, t1);
            break;
        case SteelType::Type::ST_WBX:   //BX - HxBxt1xt2
            I_s = calcI_typeH(B, H, 2 * t2, t1);
            break;
        case SteelType::Type::ST_WP:    //P  - Hx     t1
            I_s = calcI_circ(H) - calcI_circ(H - 2 * t1);
            break;
        case SteelType::Type::ST_WPL:   //PL - HxB
            I_s = calcI_rect(H, B);
            break;
        case SteelType::Type::ST_WBR:    //R  - H
            I_s = calcI_circ(H);
            break;
        default :
            return 0;
        }
    }
    return I_s;
}


qreal SectionCalculations::calcZ_S(SteelType::Type sshape, int axis, qreal H, qreal B,
                                   qreal t1, qreal t2, qreal I_s)
//T形については中立軸よりの距離が大きい方の値
{
    qreal Z_s = 0;
    qreal na  = 0;
//  I_s = CalcI_S(steelShape ss,int axis,qreal H,qreal B,qreal t1,qreal t2);
    if (axis == 0) {
        switch (sshape) {
        case SteelType::Type::ST_WI:
        case SteelType::Type::ST_WT2:
            Z_s = 2 * I_s / B;
            break;
        case SteelType::Type::ST_WT1:
            na  = calcNeutralAxis_typeT(H, B, t1, t2);
            Z_s = I_s / na;
            break;
        case SteelType::Type::ST_WC1:
        case SteelType::Type::ST_WBX:
        case SteelType::Type::ST_WH:
        case SteelType::Type::ST_WP :
        case SteelType::Type::ST_WPL:
        case SteelType::Type::ST_WBR:
            Z_s = 2 * I_s / H;
            break;
        default :
            return 0;
        }
    } else {
        switch (sshape) {
        case SteelType::Type::ST_WI:
        case SteelType::Type::ST_WP :
        case SteelType::Type::ST_WBR:
            Z_s = 2 * I_s / H;
            break;
        case SteelType::Type::ST_WBX:
        case SteelType::Type::ST_WH:
        case SteelType::Type::ST_WT1:
        case SteelType::Type::ST_WPL:
            Z_s = 2 * I_s / B;
            break;
        case SteelType::Type::ST_WT2:
            na  = calcNeutralAxis_typeT(H, B, t1, t2);
            Z_s = I_s / na;
            break;
        case SteelType::Type::ST_WC1 :
            na  = calcNeutralAxis_typeT(B, H, 2 * t2, t1);
            Z_s = I_s / na;
            break;
        default :
            return 0;
        }
    }
    return Z_s;
}

qreal SectionCalculations::calcZe_S(SteelType::Type sshape, int axis, qreal H, qreal B,
                                    qreal t1, qreal t2, qreal F1, qreal F2)
//曲げ用有効断面積（告示FCランク以下となる部分を控除）
//T形については、T字の下縁が圧縮となる場合の、圧縮側のZ
{
    if ( F1 < 1.0e-5 || F2 < 1.0e-5 ) return 0.0;
    if ( H < 1.0e-5 || B < 1.0e-5 || t1 < 1.0e-5 || t2 < 1.0e-5 ) return 0.0;

    qreal Be, He, Ie, Ze;
//web
    const qreal JUDGE11 = 15.5 * qSqrt(235 / F1); //圧縮を受ける突出部
    const qreal JUDGE21 = 48.0 * qSqrt(235 / F1); //圧縮を受ける２辺支持部
    const qreal JUDGE31 = 71.0 * qSqrt(235 / F1); //せん断を受ける２辺支持部
//flange
    const qreal JUDGE12 = 15.5 * qSqrt(235 / F2); //
    const qreal JUDGE22 = 48.0 * qSqrt(235 / F2); //
    const qreal JUDGE32 = 71.0 * qSqrt(235 / F2); //

    if (axis == 0) {
        switch (sshape) {
        case SteelType::Type::ST_WH:
            Be = (0.5 * B / t2 <= JUDGE12) ? B : JUDGE12 * t2 * 2;
            He = ((H - 2 * t2) / t1 <= JUDGE31) ? H : JUDGE31 * t1 + 2 * t2;
            Ie = calcI_typeH(H, Be, t1, t2) - calcI_rect(t1, H - He);
            Ze = 2 * Ie / H;
            break;
        case SteelType::Type::ST_WI:
            Be = (0.5 * B / t2 <= JUDGE12) ? B : JUDGE12 * t2 * 2;
//            He = ((H-2*t2)/t1 <= JUDGE3) ? H : JUDGE3*t1+2*t2; //応力来ないけど控除する？
            Ie = calcI_typeI(H, Be, t1, t2);
            Ze = 2 * Ie / Be;
            break;
        case SteelType::Type::ST_WT1:
            He = ((H - t2) / t1 <= JUDGE11) ? H : JUDGE11 * t1 + t2;
            Ie = calcI_typeT(He, B, t1, t2);
            Ze = Ie / calcNeutralAxis_typeT(He, B, t1, t2);
            break;
        case SteelType::Type::ST_WT2:
            Be = (0.5 * B / t2 <= JUDGE12) ? B : JUDGE12 * t2 * 2;
            Ie = calcI_typeI(H, Be, t1, 0.5 * t2);
            Ze = 2 * Ie / Be;
            break;
        case SteelType::Type::ST_WC1:
            Be = (B / t2 <= JUDGE12) ? B : JUDGE12 * t2;
            He = ((H - 2 * t2) / t1 <= JUDGE31) ? H : JUDGE31 * t1 + 2 * t2;
            Ie = calcI_typeH(H, Be, t1, t2) - calcI_rect(t1, H - He);
            Ze = 2 * Ie / H;
            break;
        case SteelType::Type::ST_WBX:
            Be = (B / t2 <= JUDGE22) ? B : JUDGE22 * t2;
            He = (H / t1 <= JUDGE31) ? H : JUDGE31 * t1;
            Ie = calcI_typeH(H, Be, 2 * t1, t2) - calcI_rect(2 * t1, H - He);
            Ze = 2 * Ie / H;
            break;
        default:
            return 0;
        }
    } else {
        switch (sshape) {
        case SteelType::Type::ST_WH:
            Be = (0.5 * B / t2 <= JUDGE12) ? B : JUDGE12 * t2 * 2;
//            He = ((H-2*t2)/t1 <= JUDGE3) ? H : JUDGE3*t1+2*t2; //応力来ないけど控除する？
            Ie = calcI_typeI(H, Be, t1, t2);
            Ze = 2 * Ie / Be;
            break;
        case SteelType::Type::ST_WI:
            Be = (0.5 * B / t2 <= JUDGE12) ? B : JUDGE12 * t2 * 2;
            He = ((H - 2 * t2) / t1 <= JUDGE31) ? H : JUDGE31 * t1 + 2 * t2;
            Ie = calcI_typeH(H, Be, t1, t2) - calcI_rect(t1, H - He);
            Ze = 2 * Ie / H;
            break;
        case SteelType::Type::ST_WT1:
            Be = (0.5 * B / t2 <= JUDGE12) ? B : JUDGE12 * t2 * 2;
            Ie = calcI_typeI(H, Be, t1, 0.5 * t2);
            Ze = 2 * Ie / Be;
            break;
        case SteelType::Type::ST_WT2:
            He = ((H - t2) / t1 <= JUDGE11) ? H : JUDGE11 * t1 + t2;
            Ie = calcI_typeT(He, B, t1, t2);
            Ze = Ie / calcNeutralAxis_typeT(He, B, t1, t2);
            break;
        case SteelType::Type::ST_WC1:
            He = ((H - 2 * t2) / t1 <= JUDGE21) ? H : JUDGE21 * t1 + 2 * t2;
            Be = (B / t2 <= JUDGE12) ? B : JUDGE12 * t2;
            Ie = calcI_typeT(Be, He, 2 * t2, t1);
            Ze = Ie / calcNeutralAxis_typeT(Be, He, 2 * t2, t1);
            break;
        case SteelType::Type::ST_WBX:
            Be = (B / t2 <= JUDGE32) ? B : JUDGE32 * t2;
            He = (H / t1 <= JUDGE21) ? H : JUDGE21 * t1;
            Ie = calcI_typeH(B, He, 2 * t2, t1) - calcI_rect(2 * t2, B - Be);
            Ze = 2 * Ie / B;
            break;
        default:
            return 0;
        }
    }
    return Ze;
}

qreal SectionCalculations::calcZp_S(SteelType::Type sshape, int axis, qreal H, qreal B,
                                    qreal t1, qreal t2)
{
    qreal Zp_s = 0;
    if (axis == 0) {
        switch (sshape) {
        case SteelType::Type::ST_WH:
        case SteelType::Type::ST_WC1:
            Zp_s = calcZp_typeH(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WI:
            Zp_s = calcZp_typeI(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WT1:
            Zp_s = calcZp_typeT(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WT2:
            Zp_s = calcZp_typeI(H, B, t1, 0.5 * t2);
            break;
        case SteelType::Type::ST_WBX:
            Zp_s = calcZp_typeH(H, B, 2 * t1, t2);
            break;
        case SteelType::Type::ST_WP :
            Zp_s = 4 * t1 * (t1 * t1 / 3 - t1 * H + H * H);
            break;
        case SteelType::Type::ST_WPL:
            Zp_s = B * H * H / 4.;
            break;
        case SteelType::Type::ST_WBR:
            Zp_s = H * H * H / 6.;
            break;
        default:
            return 0;
        }
    } else {
        switch (sshape) {
        case SteelType::Type::ST_WH:
            Zp_s = calcZp_typeI(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WI:
            Zp_s = calcZp_typeH(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WT1:
            Zp_s = calcZp_typeT(H, B, t1, t2);
            break;
        case SteelType::Type::ST_WT2:
            Zp_s = calcZp_typeI(H, B, t1, 0.5 * t2);
            break;
        case SteelType::Type::ST_WC1:
            Zp_s = calcZp_typeT(B, H, 2 * t2, t1);
            break;
        case SteelType::Type::ST_WBX:
            Zp_s = calcZp_typeH(B, H, 2 * t2, t1);
            break;
        case SteelType::Type::ST_WP :
            Zp_s = 4 * t1 * (t1 * t1 / 3 - t1 * H + H * H);
            break;
        case SteelType::Type::ST_WPL:
            Zp_s = H * B * B / 4.;
            break;
        case SteelType::Type::ST_WBR:
            Zp_s = H * H * H / 6.;
            break;
        default:
            return 0;
        }
    }
    return Zp_s;
}

qreal SectionCalculations::calcSrs_S(qreal I, qreal A) //断面二次半径
{
    const qreal EPS = 0.001;
    return I > EPS ? qSqrt(I / A) : 0.0;
}

qreal SectionCalculations::calcWTratio_S(SteelType::Type sshape, SectionPart::Part part, qreal H,
                                         qreal B,
                                         qreal t1, qreal t2) //幅厚比
{
    if ( t1 < 1.0e-5 || t2 < 1.0e-5 ) return 0.0;

    qreal wtratio;

    if     (sshape == SteelType::Type::ST_WH && part == SectionPart::Part::FLANGE) {
        wtratio = 0.5 * B / t2;
    } else if (sshape == SteelType::Type::ST_WH && part == SectionPart::Part::WEB   ) {
        wtratio = (H - 2 * t2) / t1;
    } else if (sshape == SteelType::Type::ST_WI && part == SectionPart::Part::FLANGE) {
        wtratio = 0.5 * B / t2;
    } else if (sshape == SteelType::Type::ST_WI && part == SectionPart::Part::WEB   ) {
        wtratio = (H - 2 * t2) / t1;
    } else if (sshape == SteelType::Type::ST_WT1 && part == SectionPart::Part::FLANGE) {
        wtratio = 0.5 * B / t2;
    } else if (sshape == SteelType::Type::ST_WT1 && part == SectionPart::Part::WEB   ) {
        wtratio = (H - t2) / t1;
    } else if (sshape == SteelType::Type::ST_WT2 && part == SectionPart::Part::FLANGE) {
        wtratio = 0.5 * B / t2;
    } else if (sshape == SteelType::Type::ST_WT2 && part == SectionPart::Part::WEB   ) {
        wtratio = (H - t2) / t1;
    } else if (sshape == SteelType::Type::ST_WC1 && part == SectionPart::Part::FLANGE) {
        wtratio = B / t2;
    } else if (sshape == SteelType::Type::ST_WC1 && part == SectionPart::Part::WEB   ) {
        wtratio = (H - 2 * t2) / t1;
    } else if (sshape == SteelType::Type::ST_WBX                        ) {
        wtratio = (B / t2 >= H / t1) ? B / t2 : H / t1;
    } else if (sshape == SteelType::Type::ST_WP                         ) {
        wtratio = H / t1;
    } else {
        return -1;
    }

    return wtratio;
}

SectionRank::Rank SectionCalculations::judgeRank(SteelType::Type sshape, SectionPart::Part part,
                                                 DATATYPE mtype, qreal wt_ratio, qreal F)
{
    SectionRank::Rank rank = SectionRank::Rank::UNDEFINED;
    qreal fa, fb, fc;

    if     (sshape == SteelType::Type::ST_WH && mtype == DATATYPE::TPCOLUMN && part == SectionPart::Part::FLANGE) {
        fa = 9.5 ;
        fb = 12  ;
        fc = 15.5;
    } else if (sshape == SteelType::Type::ST_WH && mtype == DATATYPE::TPCOLUMN && part == SectionPart::Part::WEB   ) {
        fa = 43  ;
        fb = 45  ;
        fc = 48  ;
    } else if (sshape == SteelType::Type::ST_WH && mtype == DATATYPE::TPGIRDER && part == SectionPart::Part::FLANGE) {
        fa = 9   ;
        fb = 11  ;
        fc = 15.5;
    } else if (sshape == SteelType::Type::ST_WH && mtype == DATATYPE::TPGIRDER && part == SectionPart::Part::WEB   ) {
        fa = 60  ;
        fb = 65  ;
        fc = 71  ;
    } else if (sshape == SteelType::Type::ST_WBX && mtype == DATATYPE::TPCOLUMN                        ) {
        fa = 33  ;
        fb = 37  ;
        fc = 48  ;
    } else if (sshape == SteelType::Type::ST_WP  && mtype == DATATYPE::TPCOLUMN                        ) {
        fa = 50  ;
        fb = 70  ;
        fc = 100 ;
    } else if (sshape == SteelType::Type::ST_WI && mtype == DATATYPE::TPCOLUMN && part == SectionPart::Part::FLANGE) {
        fa = 9.5 ;
        fb = 12  ;
        fc = 15.5;
    } else if (sshape == SteelType::Type::ST_WI && mtype == DATATYPE::TPCOLUMN && part == SectionPart::Part::WEB   ) {
        fa = 43  ;
        fb = 45  ;
        fc = 48  ;
    } else {
        return rank;   //HとIcolum,BXcolumn以外はreturn -1
    }

    switch (sshape) {
    case SteelType::Type::ST_WP:
        if     (wt_ratio <= fa * (235 / F)) {
            rank = SectionRank::Rank::FA;
        } else if (wt_ratio <= fb * (235 / F)) {
            rank = SectionRank::Rank::FB;
        } else if (wt_ratio <= fc * (235 / F)) {
            rank = SectionRank::Rank::FC;
        } else {
            rank = SectionRank::Rank::FD;
        }
        break;
    default:
        if     (wt_ratio <= fa * qSqrt(235 / F)) {
            rank = SectionRank::Rank::FA;
        } else if (wt_ratio <= fb * qSqrt(235 / F)) {
            rank = SectionRank::Rank::FB;
        } else if (wt_ratio <= fc * qSqrt(235 / F)) {
            rank = SectionRank::Rank::FC;
        } else {
            rank = SectionRank::Rank::FD;
        }
        break;
    }

    return rank;
}

SectionRank::Rank SectionCalculations::judgeRankH_SN(DATATYPE mtype, qreal H, qreal B,
                                                     qreal t1, qreal t2, int steel)
//qreal F1,F2    :235 or 215 or 325 or 295 (t1,t2による)
{
    qreal F1, F2;
    qreal kf[3], kw[3], kc[3];

    if      (steel == 400) { //SN材なので板厚から一意に決まる
        F1   = (t1 <= 40) ? 235 : 215;
        F2   = (t2 <= 40) ? 235 : 215;
    } else if (steel == 490) {
        F1   = (t1 <= 40) ? 325 : 295;
        F2   = (t2 <= 40) ? 325 : 295;
    } else {
        return SectionRank::Rank::UNDEFINED;
    }

    if      (steel == 400 && mtype == DATATYPE::TPGIRDER) {
        kf[0] = 22 ;
        kf[1] = 27 ;
        kf[2] = 32;
        kw[0] = 114 ;
        kw[1] = 175 ;
        kw[2] = 209;
        kc[0] = 100 ;
        kc[1] = 100 ;
        kc[2] = 110;
    } else if (steel == 400                           ) {
        kf[0] = 22 ;
        kf[1] = 27 ;
        kf[2] = 32;
        kw[0] = 71 ;
        kw[1] = 87 ;
        kw[2] = 104;
        kc[0] = 71 ;
        kc[1] = 71 ;
        kc[2] = 74;
    } else if (steel == 490 && mtype == DATATYPE::TPGIRDER) {
        kf[0] = 26 ;
        kf[1] = 33 ;
        kf[2] = 40;
        kw[0] = 118 ;
        kw[1] = 147 ;
        kw[2] = 180;
        kc[0] = 100 ;
        kc[1] = 100 ;
        kc[2] = 110;
    } else if (steel == 490                           ) {
        kf[0] = 26 ;
        kf[1] = 33 ;
        kf[2] = 40;
        kw[0] = 63 ;
        kw[1] = 77 ;
        kw[2] = 94;
        kc[0] = 71 ;
        kc[1] = 71 ;
        kc[2] = 74;
    } else {
        return SectionRank::Rank::UNDEFINED;
    }

    int i;
    for (i = 0; i <= 2; i++) {
        if (qPow(0.5 * B / t2, 2) / qPow(kf[i] / qSqrt(F2 / 98), 2) + qPow((H - 2 * t2) / t1,
                                                                           2) / qPow(kw[i] / qSqrt(F1 / 98), 2) <= 1
                && (H - 2 * t2) / t1 <= kc[i] / qSqrt(F1 / 98)) {
            break;
        }
    }

    SectionRank::Rank rank = static_cast<SectionRank::Rank>(i);
    return rank;
}


///// RC /////
qreal SectionCalculations::calcA_RC(const LinePartsParameterRC &paramRC)
{
    if (false) {}
    else if (paramRC.concType == ConcreteType::Type::RECTANGLE) {
        return paramRC.Dy * paramRC.Dz;
    } else if (paramRC.concType == ConcreteType::Type::CIRCLE   ) {
        return M_PI * paramRC.Dy * paramRC.Dy / 4 ;
    } else {
        return 0;
    }
}
qreal SectionCalculations::calcAs_RC(int num, const QString &phi)
{
    return num * getBarArea(phi);
}
qreal SectionCalculations::calcAs_RC(const LinePartsParameterRC &paramRC,
                                     DATATYPE mtype)
{
    qreal Ambar = getBarArea(paramRC.mainBar1);
    qreal Aabar = getBarArea(paramRC.axialBar);
    qreal As;
    int nummainbar = 0, numaxialbar = 0;
    //柱
    if (mtype == DATATYPE::TPCOLUMN) {
        //主筋の本数
        nummainbar = countColumnBar(paramRC.mainBarArrange1, paramRC.mainBarArrange2, paramRC.concType);
        //芯鉄筋の本数
        numaxialbar = countColumnBar(paramRC.axialBarArrange1, paramRC.axialBarArrange2,
                                     paramRC.axialConcType);
        As = Ambar * nummainbar + Aabar * numaxialbar;
        //梁
    } else if (mtype == DATATYPE::TPGIRDER) {
        nummainbar = countGirderBar(paramRC.mainBarArrange1, paramRC.mainBarArrange2);
        As = Ambar * nummainbar;
        //ブレース
    } else if (mtype == DATATYPE::TPBRACE) {
        //主筋の本数
        nummainbar = countColumnBar(paramRC.mainBarArrange1, paramRC.mainBarArrange2, paramRC.concType);
        //芯鉄筋の本数
        numaxialbar = countColumnBar(paramRC.axialBarArrange1, paramRC.axialBarArrange2,
                                     paramRC.axialConcType);
        As = Ambar * nummainbar + Aabar * numaxialbar;
        //小梁
    } else if (mtype == DATATYPE::TPBEAM) {
        nummainbar = countGirderBar(paramRC.mainBarArrange1, paramRC.mainBarArrange2);
        As = Ambar * nummainbar;
    } else {
        return 0;
    }
    return As;
}

qreal SectionCalculations::calcAeq_RC(qreal A, qreal As, int n)
//RCに置き換えた等価軸断面積。n：ヤング係数比
{
    return A - As + n * As;
}

qreal SectionCalculations::calcI_RC(const LinePartsParameterRC &paramRC,
                                    int axis) //鉄筋等考慮せず
{
    qreal I_rc;
    if (paramRC.concType == ConcreteType::Type::RECTANGLE) {
        I_rc = (axis == 0) ? calcI_rect(paramRC.Dy, paramRC.Dz) : calcI_rect(paramRC.Dz, paramRC.Dy);
    } else if (paramRC.concType == ConcreteType::Type::CIRCLE) {
        I_rc = calcI_circ(paramRC.Dy);
    } else {
        return 0;
    }
    return I_rc;
}
qreal SectionCalculations::calcIs_RC(const LinePartsParameterRC &paramRC,
                                     int axis, DATATYPE mtype) //鉄筋のみ
{
    qreal RY = paramRC.RY;
    qreal RK = paramRC.RK;
    qreal RKu = paramRC.RKu;
    qreal RKb = paramRC.RKb;
    qreal RY1 = paramRC.RY1;

    qreal Ambar = getBarArea(paramRC.mainBar1); //主筋断面積
    qreal Dy = paramRC.Dy;
    qreal Dz = paramRC.Dz;
    qreal Is = 0; //return

    if (mtype == DATATYPE::TPCOLUMN || mtype == DATATYPE::TPBRACE) {
        //mainbar
        if (paramRC.concType == ConcreteType::Type::CIRCLE) {
            Is += calcI_circColumnBar(paramRC.mainBarArrange1.numsum[0], RK, Ambar, Dy);
        } else if (paramRC.concType == ConcreteType::Type::RECTANGLE && axis == 0) {
            Is += calcI_rectColumnBar(paramRC.mainBarArrange2, paramRC.mainBarArrange1, RK, RY, Ambar, Dz);
        } else if (paramRC.concType == ConcreteType::Type::RECTANGLE && axis == 1) {
            Is += calcI_rectColumnBar(paramRC.mainBarArrange1, paramRC.mainBarArrange2, RK, RY, Ambar, Dy);
        } else {
            return 0;
        }

        //axialbar
        qreal Aabar = getBarArea(paramRC.axialBar); //芯鉄筋断面積
        qreal aDy = paramRC.axialDy;
        qreal aDz = paramRC.axialDz;
        qreal Isa = 0;
        if (paramRC.axialConcType == ConcreteType::Type::CIRCLE) {
            Isa += calcI_circColumnBar(paramRC.axialBarArrange1.numsum[0], 0, Aabar, aDy);
        } else if (paramRC.axialConcType == ConcreteType::Type::RECTANGLE && axis == 0) {
            Isa += calcI_rectColumnBar(paramRC.axialBarArrange2, paramRC.axialBarArrange1, 0, RY, Aabar, aDz);
        } else if (paramRC.axialConcType == ConcreteType::Type::RECTANGLE && axis == 1) {
            Isa += calcI_rectColumnBar(paramRC.axialBarArrange1, paramRC.axialBarArrange2, 0, RY, Aabar, aDy);
        } else {
            return 0;
        }
        qreal Asa = Aabar * countColumnBar(paramRC.axialBarArrange1, paramRC.axialBarArrange2,
                                           paramRC.axialConcType);
        Isa = calcI_axisShift(Isa, Asa, 0, -1 * paramRC.axialCenterY);

    } else if ((mtype == DATATYPE::TPGIRDER || mtype == DATATYPE::TPBEAM) && axis == 0) { //X軸回り
        for (int i = 0; i < paramRC.mainBarArrange1.size(); ++i) { //上端筋
            if (i == 0)Is += paramRC.mainBarArrange1.numsum[i] * Ambar * qPow(Dz / 2 - RKu, 2);
            if (i >= 1)Is += paramRC.mainBarArrange1.numsum[i] * Ambar * qPow(Dz / 2 - RKu - RY1 - RY * (i - 1),
                                                                                  2);
        }
        for (int i = 0; i < paramRC.mainBarArrange2.size(); ++i) { //下端筋
            if (i == 0)Is += paramRC.mainBarArrange2.numsum[i] * Ambar * qPow(Dz / 2 - RKb, 2);
            if (i >= 1)Is += paramRC.mainBarArrange2.numsum[i] * Ambar * qPow(Dz / 2 - RKb - RY1 - RY * (i - 1),
                                                                                  2);
        }
    } else if ((mtype == DATATYPE::TPGIRDER || mtype == DATATYPE::TPBEAM) && axis == 1) { //Y軸回り
        for (int i = 0; i < paramRC.mainBarArrange1.size(); ++i) { //上端筋
            Is += calcI_lineBar(paramRC.mainBarArrange1.numsum[i], paramRC.mainBarArrange1.numleft[i],
                                paramRC.mainBarArrange1.numright[i], RK, RY, Ambar, Dy);
        }
        for (int i = 0; i < paramRC.mainBarArrange2.size(); ++i) { //下端筋
            Is += calcI_lineBar(paramRC.mainBarArrange2.numsum[i], paramRC.mainBarArrange2.numleft[i],
                                paramRC.mainBarArrange2.numright[i], RK, RY, Ambar, Dy);
        }
    } else {
        return 0;
    }
    return Is;
}

qreal SectionCalculations::calcIeq_RC(qreal I, qreal Is,
                                      int n) //RC換算等価断面二次モーメント
{
    return I - Is + Is * n;
}


qreal SectionCalculations::calcPg_RC(const LinePartsParameterRC &paramRC,
                                     DATATYPE mtype)
{
//全体長方形、芯鉄筋円形（もしくはその逆）の場合も計算可

    qreal As = calcAs_RC(paramRC, mtype);
    qreal Arc = (paramRC.concType == ConcreteType::Type::RECTANGLE) ? (paramRC.Dy * paramRC.Dz) :
                (M_PI * paramRC.Dy * paramRC.Dy / 4);
    return As / Arc;
}

qreal SectionCalculations::calcD_RC(const LinePartsParameterRC &paramRC, int side,
                                    DATATYPE mtype)//部材表面～引張鉄筋の重心
{
    //・矩形断面のみ対応（円形・T字×）
    //side ==0:左辺引張
    //       1:右辺引張
    //       2:上辺引張
    //       3:下辺引張
    if (paramRC.concType == ConcreteType::Type::RECTANGLE) {} //矩形以外NG
    else {
        return 0;
    }
    qreal RK = paramRC.RK;
    qreal RKu = paramRC.RKu;
    qreal RKb = paramRC.RKb;
    qreal RY = paramRC.RY;
    qreal RY1 = paramRC.RY1;


    qreal tmpD = 0;
    if (mtype == DATATYPE::TPCOLUMN || mtype == DATATYPE::TPBRACE) {
        int nummain;
        QList<int> numsub;
        switch (side) {
        case 0://left
            nummain = paramRC.mainBarArrange1.numsum[0];
            numsub = paramRC.mainBarArrange2.numleft;
            break;
        case 1://right
            nummain = paramRC.mainBarArrange1.numsum[1];
            numsub = paramRC.mainBarArrange2.numright;
            break;
        case 2://top
            nummain = paramRC.mainBarArrange2.numsum[0];
            numsub = paramRC.mainBarArrange1.numleft;
            break;
        case 3://bottom
            nummain = paramRC.mainBarArrange2.numsum[1];
            numsub = paramRC.mainBarArrange1.numright;
            break;
        default:
            return 0;
        }
        tmpD = nummain * RK;
        for (int i = 0; i <= 1; ++i) {
            tmpD += (numsub[i] >= 2) ? (numsub[i] - 1) * (RY * numsub[i] / 2 + RK) : 0;
        }

    } else if ((mtype == DATATYPE::TPGIRDER || mtype == DATATYPE::TPBEAM) && (side == 0 || side == 1)) {
        QList<int> top, bottom;
        switch (side) {
        case 0://left
            top    = paramRC.mainBarArrange1.numleft;
            bottom = paramRC.mainBarArrange2.numleft;
            break;
        case 1://right
            top    = paramRC.mainBarArrange1.numright;
            bottom = paramRC.mainBarArrange2.numright;
            break;
        default:
            return 0;
        }
        for (int i = 0; i < paramRC.mainBarArrange1.size(); ++i) {
            tmpD += ((top[i] - 1) * RY / 2 + RK) * top[i];
        }
        for (int i = 0; i < paramRC.mainBarArrange2.size(); ++i) {
            tmpD += ((bottom[i] - 1) * RY / 2 + RK) * bottom[i];
        }

    } else if ((mtype == DATATYPE::TPGIRDER || mtype == DATATYPE::TPBEAM) && (side == 2 || side == 3)) {
        QList<int> nummain;
        int listsize;
        switch (side) {
        case 2://top
            nummain = paramRC.mainBarArrange1.numsum;
            listsize = paramRC.mainBarArrange1.size();
            for (int i = 0; i < listsize; ++i) {
                if (i == 0)tmpD += (RKu) * nummain[i];
                if (i >= 1)tmpD += (RKu + RY1 + (i - 1) * RY) * nummain[i];
            }
            break;
        case 3://bottom
            nummain = paramRC.mainBarArrange2.numsum;
            listsize = paramRC.mainBarArrange2.size();
            for (int i = 0; i < listsize; ++i) {
                if (i == 0)tmpD += (RKb) * nummain[i];
                if (i >= 1)tmpD += (RKb + RY1 + (i - 1) * RY) * nummain[i];
            }
            break;
        default:
            return 0;
        }

    } else {
        return 0;
    }
    int num = countTensionBar(paramRC, side, mtype);
    return (side == 0 || side == 1) ? paramRC.Dy - tmpD / num : paramRC.Dz - tmpD / num;
}

qreal SectionCalculations::calcPt_RC(qreal at, qreal d, qreal width)
{
    return at / (d * width);
}

qreal SectionCalculations::calcPt_RC(const LinePartsParameterRC &paramRC, int side,
                                     DATATYPE mtype)
{
    if (paramRC.concType != ConcreteType::Type::RECTANGLE) {
        return 0;
    }
    qreal d = calcD_RC(paramRC, side, mtype);
    int num = countTensionBar(paramRC, side, mtype);
    qreal at = calcAs_RC(num, paramRC.mainBar1);
    return (side == 0 || side == 1) ? calcPt_RC(at, d, paramRC.Dz) : calcPt_RC(at, d, paramRC.Dy);
}

qreal SectionCalculations::calcPw_RC(const LinePartsParameterRC &paramRC, int axis)
{
    qreal aw, bx;
    if (axis == 0) {
        aw = paramRC.hoopZ * getBarArea(paramRC.hoopBar);
        bx = paramRC.hoopPitch * paramRC.Dy;
        return aw / bx;
    } else if (axis == 1) {
        aw = paramRC.hoopY * getBarArea(paramRC.hoopBar);
        bx = paramRC.hoopPitch * paramRC.Dz;
        return aw / bx;
    } else {
        return 0;
    }
}

///// SRC /////
qreal SectionCalculations::calcA_SRC(const LinePartsParameterSRC &paramSRC, bool isFilledConc)
{
    //P,BXかつhasFilledConc false→中空
    switch (paramSRC.steelType) {
    case SteelType::Type::ST_P  :
    case SteelType::Type::ST_WP  :
        if (!isFilledConc) {
            return calcA_RC(paramSRC) - M_PI * qPow((paramSRC.sizeH - 2 * paramSRC.t1) / 2, 2);
        }
    case SteelType::Type::ST_BX :
    case SteelType::Type::ST_WBX :
        if (!isFilledConc) {
            return calcA_RC(paramSRC) - (paramSRC.sizeH - 2 * paramSRC.t2) * (paramSRC.sizeB - 2 * paramSRC.t1);
        }
    default:
        return calcA_RC(paramSRC);
    }
}
qreal SectionCalculations::calcAs_SRC(const LinePartsParameterSRC &paramSRC,
                                      DATATYPE mtype)
{
    //重なっている部分ダブルカウント
    qreal As = 0;
    As += calcA_S(paramSRC.steelType, paramSRC.sizeH, paramSRC.sizeB, paramSRC.t1, paramSRC.t2);
    As += calcA_S(paramSRC.steelType_T, paramSRC.H_T, paramSRC.B_T, paramSRC.t1_T, paramSRC.t2_T);
    As += calcAs_RC(paramSRC, mtype);
    return As;
}

qreal SectionCalculations::calcAeq_SRC(qreal A, qreal As, int n)
{
    return A - As + n * As;   //n:ヤング係数比
}

qreal SectionCalculations::calcI_SRC(const LinePartsParameterSRC &paramSRC, int axis,
                                     bool isFilledConc)
{
    switch (paramSRC.steelType) {
    case SteelType::Type::ST_P  :
    case SteelType::Type::ST_WP  :
        if (!isFilledConc) {
            qreal Ac = M_PI * qPow((paramSRC.sizeH - 2 * paramSRC.t1) / 2, 2);
            qreal Ic = calcI_circ(paramSRC.sizeH - 2 * paramSRC.t1);
            qreal shift = (axis == 0) ? -1 * paramSRC.cY : -1 * paramSRC.cZ;
            Ic = calcI_axisShift(Ic, Ac, 0, shift);
            return calcI_RC(paramSRC, axis) - Ic;
        }
    case SteelType::Type::ST_BX :
    case SteelType::Type::ST_WBX :
        if (!isFilledConc && axis == 0) {
            qreal Ac = (paramSRC.sizeH - 2 * paramSRC.t2) * (paramSRC.sizeB - 2 * paramSRC.t1);
            qreal Ic = (axis == 0) ? calcI_rect(paramSRC.sizeB - 2 * paramSRC.t1,
                                                paramSRC.sizeH - 2 * paramSRC.t2) : calcI_rect(paramSRC.sizeH - 2 * paramSRC.t2,
                                                                                               paramSRC.sizeB - 2 * paramSRC.t1);
            qreal shift = (axis == 0) ? -1 * paramSRC.cY : -1 * paramSRC.cZ;
            Ic = calcI_axisShift(Ic, Ac, 0, shift);
            return calcI_RC(paramSRC, axis) - Ic;
        }
    default:
        return calcI_RC(paramSRC, axis);
    }
}

qreal SectionCalculations::calcIs_SRC(const LinePartsParameterSRC &paramSRC,
                                      int axis, DATATYPE mtype)
{
    qreal Is = 0;
    Is += calcI_S(paramSRC.steelType, axis, paramSRC.sizeH, paramSRC.sizeB, paramSRC.t1, paramSRC.t2);
    Is += calcI_S(paramSRC.steelType_T, !axis, paramSRC.H_T, paramSRC.B_T, paramSRC.t1_T,
                  paramSRC.t2_T);
    Is += calcIs_RC(paramSRC, axis, mtype);
    return Is;
}

qreal SectionCalculations::calcIeq_SRC(qreal I, qreal Is, int n)
{
    return I - Is + Is * n;
}

qreal SectionCalculations::calcPg_SRC(const LinePartsParameterSRC &paramSRC,
                                      DATATYPE mtype, bool isFilledConc)
{
    qreal As = calcAs_SRC(paramSRC, mtype);
    qreal A  = calcA_SRC(paramSRC, isFilledConc);
    return As / A;
}

qreal SectionCalculations::calcPt_SRC(const LinePartsParameterSRC &paramSRC, int side,
                                      DATATYPE mtype)
{
    return calcPt_RC(paramSRC, side, mtype);
}

qreal SectionCalculations::calcPw_SRC(const LinePartsParameterSRC &paramSRC, int axis)
{
    return calcPw_RC(paramSRC, axis);
}

///// CFT /////
qreal SectionCalculations::calcA_CFT(const LinePartsParameter &paramCFT)
{
    //PN…STKNのP、PM…STKのP、BP…BCP、BR…BCR,BX
    switch (paramCFT.steelType) {
    case SteelType::Type::ST_P:
    case SteelType::Type::ST_WP:
        return M_PI * paramCFT.sizeH * paramCFT.sizeH / 4 ;
    case SteelType::Type::ST_BX:
    case SteelType::Type::ST_WBX:
        return paramCFT.sizeH * paramCFT.sizeB;
    default:
        return 0;
    }
}

qreal SectionCalculations::calcAs_CFT(const LinePartsParameter &paramCFT)
{
    return calcA_S(paramCFT);
}

qreal SectionCalculations::calcAeq_CFT(qreal A, qreal As, int n)
{
    return A - As + As * n;
}

qreal SectionCalculations::calcI_CFT(const LinePartsParameter &paramCFT, int axis)
{
    switch (paramCFT.steelType) {
    case SteelType::Type::ST_P:
    case SteelType::Type::ST_WP:
        return calcI_circ(paramCFT.sizeH) ;
    case SteelType::Type::ST_BX:
    case SteelType::Type::ST_WBX:
        return axis == 0 ? calcI_rect(paramCFT.sizeB, paramCFT.sizeH) : calcI_rect(paramCFT.sizeH,
                                                                                   paramCFT.sizeB);
    default:
        return 0;
    }
}

qreal SectionCalculations::calcIs_CFT(const LinePartsParameter &paramCFT, int axis)
{
    return calcI_S(paramCFT, axis);
}

qreal SectionCalculations::calcIeq_CFT(qreal I, qreal Is, int n)
{
    return I - Is + Is * n;
}

///// WALL /////
qreal SectionCalculations::calcPs_Wall(const PlaneSectionParameter &palam,
                                       PlaneDirection::Direction dir)
{
    qreal ps;
    int multi = palam.isDoubleBar ? 2 : 1;

    switch (dir) {
    case PlaneDirection::Direction::VERTICAL:
        ps = multi * getBarArea(palam.vBar.phi) / palam.concThickness / palam.vBar.pitch;
        break;
    case PlaneDirection::Direction::HORIZONTAL:
        ps = multi * getBarArea(palam.hBar.phi) / palam.concThickness / palam.hBar.pitch;
        break;
    default:
        ps = 0;
    }
    return ps;
}

qreal SectionCalculations::calcAr_Wall(const PlaneSectionParameter &palam,
                                       PlaneDirection::Direction dir, WallOpenSize::Size size)
{
    qreal ar;
    if (size == WallOpenSize::Size::SMALL) {
        switch (dir) {
        case PlaneDirection::Direction::VERTICAL:
            ar = getBarArea(palam.vOpeningBar_small.phi) * palam.vOpeningBar_small.num;
            break;
        case PlaneDirection::Direction::HORIZONTAL:
            ar = getBarArea(palam.hOpeningBar_small.phi) * palam.hOpeningBar_small.num;
            break;
        case PlaneDirection::Direction::DIAGONAL:
            ar = getBarArea(palam.dOpeningBar_small.phi) * palam.dOpeningBar_small.num;
            break;
        default:
            ar = 0;
        }
    } else if (size == WallOpenSize::Size::LARGE) {
        switch (dir) {
        case PlaneDirection::Direction::VERTICAL:
            ar = getBarArea(palam.vOpeningBar_large.phi) * palam.vOpeningBar_large.num;
            break;
        case PlaneDirection::Direction::HORIZONTAL:
            ar = getBarArea(palam.hOpeningBar_large.phi) * palam.hOpeningBar_large.num;
            break;
        case PlaneDirection::Direction::DIAGONAL:
            ar = getBarArea(palam.dOpeningBar_large.phi) * palam.dOpeningBar_large.num;
            break;
        default:
            ar = 0;
        }
    }
    return ar;
}

qreal SectionCalculations::calcAt_Wall(const PlaneSectionParameter &palam,
                                       PlaneSide::Side inout, SectionPart::Part part)
{
    qreal at;
    if (inout == PlaneSide::Side::INNER) {
        switch (part) {
        case SectionPart::Part::TOP:
            at = getBarArea(palam.vBar_innerTop.phi) * 1000 / palam.vBar_innerTop.pitch;
            break;
        case SectionPart::Part::MIDDLE:
            at = getBarArea(palam.vBar_innerMiddle.phi) * 1000 / palam.vBar_innerMiddle.pitch;
            break;
        case SectionPart::Part::BOTTOM:
            at = getBarArea(palam.vBar_innerBottom.phi) * 1000 / palam.vBar_innerBottom.pitch;
            break;
        case SectionPart::Part::EDGE:
            at = getBarArea(palam.hBar_innerEdge.phi) * 1000 / palam.hBar_innerEdge.pitch;
            break;
        case SectionPart::Part::CENTER:
            at = getBarArea(palam.hBar_innerCenter.phi) * 1000 / palam.hBar_innerCenter.pitch;
            break;
        default:
            at = 0;
        }
    } else if (inout == PlaneSide::Side::OUTER) {
        switch (part) {
        case SectionPart::Part::TOP:
            at = getBarArea(palam.vBar_outerTop.phi) * 1000 / palam.vBar_outerTop.pitch;
            break;
        case SectionPart::Part::MIDDLE:
            at = getBarArea(palam.vBar_outerMiddle.phi) * 1000 / palam.vBar_outerMiddle.pitch;
            break;
        case SectionPart::Part::BOTTOM:
            at = getBarArea(palam.vBar_outerBottom.phi) * 1000 / palam.vBar_outerBottom.pitch;
            break;
        case SectionPart::Part::EDGE:
            at = getBarArea(palam.hBar_outerEdge.phi) * 1000 / palam.hBar_outerEdge.pitch;
            break;
        case SectionPart::Part::CENTER:
            at = getBarArea(palam.hBar_outerCenter.phi) * 1000 / palam.hBar_outerCenter.pitch;
            break;
        default:
            at = 0;
        }
    }

    return at;
}

qreal SectionCalculations::calcAt_Slab(const PlaneSectionParameter &palam,
                                       PlaneDirection::Direction dir, PlaneSide::Side side,
                                       SectionPart::Part part, PlanePart::Part abpart)
{
    qreal at;
    if (false) {
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::UPPER
               && part == SectionPart::Part::EDGE && abpart == PlanePart::Part::A_PART) {
        at = getBarArea(palam.sBar_upperEdgeA.phi) * 1000 / palam.sBar_upperEdgeA.pitch;
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::UPPER
               && part == SectionPart::Part::CENTER && abpart == PlanePart::Part::A_PART) {
        at = getBarArea(palam.sBar_upperCenterA.phi) * 1000 / palam.sBar_upperCenterA.pitch;
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::UPPER
               && abpart == PlanePart::Part::B_PART) {
        at = getBarArea(palam.sBar_upperEdgeCenterB.phi) * 1000 / palam.sBar_upperEdgeCenterB.pitch;
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::LOWER
               && part == SectionPart::Part::EDGE && abpart == PlanePart::Part::A_PART) {
        at = getBarArea(palam.sBar_lowerEdgeA.phi) * 1000 / palam.sBar_lowerEdgeA.pitch;
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::LOWER
               && part == SectionPart::Part::CENTER && abpart == PlanePart::Part::A_PART) {
        at = getBarArea(palam.sBar_lowerCenterA.phi) * 1000 / palam.sBar_lowerCenterA.pitch;
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::LOWER
               && abpart == PlanePart::Part::B_PART) {
        at = getBarArea(palam.sBar_lowerEdgeCenterB.phi) * 1000 / palam.sBar_lowerEdgeCenterB.pitch;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::UPPER && part == SectionPart::Part::EDGE
               && abpart == PlanePart::Part::A_PART) {
        at = getBarArea(palam.lBar_upperEdgeA.phi) * 1000 / palam.lBar_upperEdgeA.pitch;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::UPPER
               && part == SectionPart::Part::CENTER && abpart == PlanePart::Part::A_PART) {
        at = getBarArea(palam.lBar_upperCenterA.phi) * 1000 / palam.lBar_upperCenterA.pitch;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::UPPER
               && abpart == PlanePart::Part::B_PART) {
        at = getBarArea(palam.lBar_upperEdgeCenterB.phi) * 1000 / palam.lBar_upperEdgeCenterB.pitch;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::LOWER && part == SectionPart::Part::EDGE
               && abpart == PlanePart::Part::A_PART) {
        at = getBarArea(palam.lBar_lowerEdgeA.phi) * 1000 / palam.lBar_lowerEdgeA.pitch;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::LOWER
               && part == SectionPart::Part::CENTER && abpart == PlanePart::Part::A_PART) {
        at = getBarArea(palam.lBar_lowerCenterA.phi) * 1000 / palam.lBar_lowerCenterA.pitch;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::LOWER
               && abpart == PlanePart::Part::B_PART) {
        at = getBarArea(palam.lBar_lowerEdgeCenterB.phi) * 1000 / palam.lBar_lowerEdgeCenterB.pitch;
    } else {
        at = 0;
    }
    return at;
}

qreal SectionCalculations::calcMa_Slab(const PlaneSectionParameter &palam,
                                       PlaneDirection::Direction dir, PlaneSide::Side side,
                                       SectionPart::Part part, PlanePart::Part abpart, bool isLongtimeloading)
{
    qreal at;
    qreal d;
    qreal j;  //下側主筋、上側配力筋にスペーサーがあると仮定、鉄筋径＝呼び径で計算
    qreal ft;

    if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::UPPER && part == SectionPart::Part::EDGE
            && abpart == PlanePart::Part::A_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.sBar_upperEdgeA.phi).F;
        at = getBarArea(palam.sBar_upperEdgeA.phi) * 1000 / palam.sBar_upperEdgeA.pitch;
        d  = palam.concThickness - palam.coverU - getBarMaxPhi(palam, PlaneDirection::Direction::SHORTSIDE,
                                                               PlaneSide::Side::UPPER) + getBarMaxPhi(palam.sBar_upperEdgeA.phi) / 2;
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::UPPER
               && part == SectionPart::Part::CENTER && abpart == PlanePart::Part::A_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.sBar_upperCenterA.phi).F;
        at = getBarArea(palam.sBar_upperCenterA.phi) * 1000 / palam.sBar_upperCenterA.pitch;
        d  = palam.concThickness - palam.coverU - getBarMaxPhi(palam, PlaneDirection::Direction::SHORTSIDE,
                                                               PlaneSide::Side::UPPER) + getBarMaxPhi(palam.sBar_upperCenterA.phi) / 2;
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::UPPER
               && abpart == PlanePart::Part::B_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.sBar_upperEdgeCenterB.phi).F;
        at = getBarArea(palam.sBar_upperEdgeCenterB.phi) * 1000 / palam.sBar_upperEdgeCenterB.pitch;
        d  = palam.concThickness - palam.coverU - getBarMaxPhi(palam, PlaneDirection::Direction::SHORTSIDE,
                                                               PlaneSide::Side::UPPER) + getBarMaxPhi(palam.sBar_upperEdgeCenterB.phi) / 2;
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::LOWER
               && part == SectionPart::Part::EDGE && abpart == PlanePart::Part::A_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.sBar_lowerEdgeA.phi).F;
        at = getBarArea(palam.sBar_lowerEdgeA.phi) * 1000 / palam.sBar_lowerEdgeA.pitch;
        d  = palam.concThickness - palam.coverD - getBarMaxPhi(palam.sBar_lowerEdgeA.phi) / 2;
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::LOWER
               && part == SectionPart::Part::CENTER && abpart == PlanePart::Part::A_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.sBar_lowerCenterA.phi).F;
        at = getBarArea(palam.sBar_lowerCenterA.phi) * 1000 / palam.sBar_lowerCenterA.pitch;
        d  = palam.concThickness - palam.coverD - getBarMaxPhi(palam.sBar_lowerCenterA.phi) / 2;
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::LOWER
               && abpart == PlanePart::Part::B_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.sBar_lowerEdgeCenterB.phi).F;
        at = getBarArea(palam.sBar_lowerEdgeCenterB.phi) * 1000 / palam.sBar_lowerEdgeCenterB.pitch;
        d  = palam.concThickness - palam.coverD - getBarMaxPhi(palam.sBar_lowerEdgeCenterB.phi) / 2;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::UPPER && part == SectionPart::Part::EDGE
               && abpart == PlanePart::Part::A_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.lBar_upperEdgeA.phi).F;
        at = getBarArea(palam.lBar_upperEdgeA.phi) * 1000 / palam.lBar_upperEdgeA.pitch;
        d  = palam.concThickness - palam.coverU - getBarMaxPhi(palam, PlaneDirection::Direction::SHORTSIDE,
                                                               PlaneSide::Side::UPPER) - getBarMaxPhi(palam, PlaneDirection::Direction::LONGSIDE,
                                                                                                PlaneSide::Side::UPPER) + getBarMaxPhi(palam.lBar_upperEdgeA.phi) / 2;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::UPPER
               && part == SectionPart::Part::CENTER && abpart == PlanePart::Part::A_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.lBar_upperCenterA.phi).F;
        at = getBarArea(palam.lBar_upperCenterA.phi) * 1000 / palam.lBar_upperCenterA.pitch;
        d  = palam.concThickness - palam.coverU - getBarMaxPhi(palam, PlaneDirection::Direction::SHORTSIDE,
                                                               PlaneSide::Side::UPPER) - getBarMaxPhi(palam, PlaneDirection::Direction::LONGSIDE,
                                                                                                PlaneSide::Side::UPPER) + getBarMaxPhi(palam.lBar_upperCenterA.phi) / 2;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::UPPER
               && abpart == PlanePart::Part::B_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.lBar_upperEdgeCenterB.phi).F;
        at = getBarArea(palam.lBar_upperEdgeCenterB.phi) * 1000 / palam.lBar_upperEdgeCenterB.pitch;
        d  = palam.concThickness - palam.coverU - getBarMaxPhi(palam, PlaneDirection::Direction::SHORTSIDE,
                                                               PlaneSide::Side::UPPER) - getBarMaxPhi(palam, PlaneDirection::Direction::LONGSIDE,
                                                                                                PlaneSide::Side::UPPER) + getBarMaxPhi(palam.lBar_upperEdgeCenterB.phi) / 2;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::LOWER && part == SectionPart::Part::EDGE
               && abpart == PlanePart::Part::A_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.lBar_lowerEdgeA.phi).F;
        at = getBarArea(palam.lBar_lowerEdgeA.phi) * 1000 / palam.lBar_lowerEdgeA.pitch;
        d  = palam.concThickness - palam.coverD - getBarMaxPhi(palam, PlaneDirection::Direction::SHORTSIDE,
                                                               PlaneSide::Side::LOWER) - getBarMaxPhi(palam.lBar_lowerEdgeA.phi) / 2;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::LOWER
               && part == SectionPart::Part::CENTER && abpart == PlanePart::Part::A_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.lBar_lowerCenterA.phi).F;
        at = getBarArea(palam.lBar_lowerCenterA.phi) * 1000 / palam.lBar_lowerCenterA.pitch;
        d  = palam.concThickness - palam.coverD - getBarMaxPhi(palam, PlaneDirection::Direction::SHORTSIDE,
                                                               PlaneSide::Side::LOWER) - getBarMaxPhi(palam.lBar_lowerCenterA.phi) / 2;
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::LOWER
               && abpart == PlanePart::Part::B_PART) {
        ft = UnifiedInputData::getInInstance()->getReinforceValue(palam.lBar_lowerEdgeCenterB.phi).F;
        at = getBarArea(palam.lBar_lowerEdgeCenterB.phi) * 1000 / palam.lBar_lowerEdgeCenterB.pitch;
        d  = palam.concThickness - palam.coverD - getBarMaxPhi(palam, PlaneDirection::Direction::SHORTSIDE,
                                                               PlaneSide::Side::LOWER) - getBarMaxPhi(palam.lBar_lowerEdgeCenterB.phi) / 2;
    } else {
        at = 0.0;
        d = 0.0;
        ft = 0.0;
    }

    if ( isLongtimeloading ) ft = ft / 1.5;
    j = d * 7 / 8;
    return at * ft * j;
}

///////////////////////////////汎用小関数///////////////////////////////////
/////CalcI/////

qreal SectionCalculations::calcI_rect(qreal w, qreal h) //矩形
{
    return w * qPow(h, 3) / 12;
}

qreal SectionCalculations::calcI_circ(qreal D)//円形
{
    return 4 * atan(static_cast<qreal>(1)) * qPow(D, 4) / 64;
}

qreal SectionCalculations::calcI_typeH(qreal H, qreal B, qreal t1, qreal t2) //H形
{
    return calcI_rect(B, H) - calcI_rect(B - t1, H - 2 * t2);
}

qreal SectionCalculations::calcI_typeI(qreal H, qreal B, qreal t1, qreal t2) //I形
{
    return calcI_rect(2 * t2, B) + calcI_rect(H - 2 * t2, t1);
}

qreal SectionCalculations::calcI_typeT(qreal H, qreal B, qreal t1, qreal t2) //T形
{
    qreal y1, y2, retI;

    y1 = calcNeutralAxis_typeT(H, B, t1, t2);
    y2 = H - y1;
    retI = (B * qPow(y2, 3) - (B - t1) * qPow(y2 - t2, 3) + t1 * qPow(y1, 3)) / 3;

    return retI;
}

qreal SectionCalculations::calcI_axisShift(qreal I, qreal A, qreal d,
                                           qreal shift) //"軸"のシフト
//d:現在の軸から見た図形の図心の座標
{
    qreal retI;
    qreal G;
    G = (d - shift) * A;
    retI = I - 2 * shift * G - A * shift * shift;
    return retI;
}

qreal SectionCalculations::calcI_lineBar(int numsum, int numside1, int numside2,
                                         qreal RK_, qreal RY_, qreal barA, qreal height,
                                         qreal RKu_, qreal RKb_, qreal RY1_)
{
    //RC 直線配置鉄筋
    const qreal EPS = 0.0001;
    qreal RK, RY, RKu, RKb, RY1;
    RK = RK_;
    RY = RY_;
    RKu = RKu_;
    RKb = RKb_;
    RY1 = RY1_;
    if (RY1 < EPS)RY1 = RY_;
    if (RKu < EPS)RKu = RK_;
    if (RKb < EPS)RKb = RK_;

    qreal Is = 0;
    int nummiddle = numsum - numside1 - numside2;
    for (int i = 1; i <= numside1; ++i) { //左/上寄せ
        if (i == 1)Is += barA * qPow(height / 2 - RKu, 2);
        if (i >= 2)Is += barA * qPow(height / 2 - RKu - RY1 - (i - 2) * RY, 2);
    }
    for (int i = 1; i <= numside2; ++i) { //右/下寄せ
        if (i == 1)Is += barA * qPow(height / 2 - RKb, 2);
        if (i >= 2)Is += barA * qPow(height / 2 - RKb - RY1 - (i - 2) * RY, 2);
    }
    int completed1 = (numside1 == 0) ? 1 : numside1 ;
    int completed2 = (numside2 == 0) ? 1 : numside2 ;
    for (int i = 1; i <= nummiddle; ++i) { //その他
        qreal dtmp;
        int flg1 = completed1 >= 2 ? 1 : 0 ;
        int flg2 = completed2 >= 2 ? 1 : 0 ;

        dtmp = height / 2 - RKu - RY1 * flg1 - (completed1 - 1 - flg1) * RY
               - i * (height - (RKu + RKb) - RY1 * (flg1 + flg2) - (completed1 + completed2 - 2 - flg1 - flg2) *
                      RY) / (nummiddle + 1);

        Is += barA * qPow(dtmp, 2);
    }
    return Is;
}

qreal SectionCalculations::calcI_rectColumnBar(const BarArrangement &main,
                                               const BarArrangement &sub,
                                               qreal RK, qreal RY, qreal barA, qreal height)
{
    //RC柱　四角配置の鉄筋
//RK : かぶり,   RY : 寄せ
//main : 主方向の鉄筋,  sub : 脇の鉄筋
//以下のコメントは主方向がX方向の場合で辺の位置を記す

    qreal Is = 0;
    int leftend0  = (sub.numleft[0] == 0) ? 0 : 1;
    int rightend0 = (sub.numleft[1] == 0) ? 0 : 1;
    int leftend1  = (sub.numright[0] == 0) ? 0 : 1;
    int rightend1 = (sub.numright[1] == 0) ? 0 : 1;
    Is += barA * (main.numsum[0] - leftend0 - rightend0) * qPow(height / 2 - RK, 2); //左辺
    Is += barA * (main.numsum[1] - leftend1 - rightend1) * qPow(height / 2 - RK, 2); //右辺
    Is += calcI_lineBar(sub.numsum[0], sub.numleft[0], sub.numright[0], RK, RY, barA, height); //上辺
    Is += calcI_lineBar(sub.numsum[1], sub.numleft[1], sub.numright[1], RK, RY, barA, height); //下辺
    return Is;
}

qreal SectionCalculations::calcI_circColumnBar(int barnum, qreal RK, qreal barA, qreal D)
{
    //RC柱　円配置の鉄筋
//barnum : 鉄筋本数, RK : かぶり, barA : 鉄筋面積/本, D : コンクリート径
    return barnum * barA * (D - 2 * RK) * (D - 2 * RK) / 8;
}


/////中立軸計算等/////
qreal SectionCalculations::calcNeutralAxis_typeT(qreal H, qreal B, qreal t1, qreal t2) //T形
//Tの下端を原点としたときの座標
{
    qreal na;
    na = H - 0.5 * (t1 * H * H + (B - t1) * t2 * t2) / (t1 * H + (B - t1) * t2);
    return na;
}

qreal SectionCalculations::calcNeutralAxis(qreal A[], qreal d[], int num) //一般形
//図心の座標dを定義した軸と、図心軸の差
{
    qreal numer = 0;
    qreal denom = 0;
    for (int i = 0; i < num; i++) {
        numer = numer + A[i] * d[i];
        denom = denom * A[i];
    }
    return numer / denom;
}


/////CalcZp/////
qreal SectionCalculations::calcZp_typeT(qreal H, qreal B, qreal t1, qreal t2) //T形
{
    qreal Zp_s;
    qreal na;
    na = calcNeutralAxis_typeT(H, B, t1, t2);
    Zp_s = 0.5 * na * na * t1 + 0.5 * (H - na - t2) * (H - na - t2) * t1 + B * t2 * (H - na - 0.5 * t2);
    return Zp_s;
}

qreal SectionCalculations::calcZp_typeH(qreal H, qreal B, qreal t1, qreal t2) //H形
{
    qreal Zp_s;
    Zp_s = B * t2 * (H - t2) + 0.25 * (H - 2 * t2) * (H - 2 * t2) * t1;
    return Zp_s;
}

qreal SectionCalculations::calcZp_typeI(qreal H, qreal B, qreal t1, qreal t2) //I形
{
    qreal Zp_s;
    Zp_s = 0.5 * B * B * t2 + 0.25 * (H - 2 * t2) * t1 * t1;
    return Zp_s;
}

/////鉄筋断面積
qreal SectionCalculations::barPhiToArea(int phi)
{
    if     (phi == 6 ) {
        return 31.7 ;
    } else if (phi == 10) {
        return 71.3 ;
    } else if (phi == 13) {
        return 127. ;
    } else if (phi == 16) {
        return 199. ;
    } else if (phi == 19) {
        return 287. ;
    } else if (phi == 22) {
        return 387. ;
    } else if (phi == 25) {
        return 507. ;
    } else if (phi == 29) {
        return 642. ;
    } else if (phi == 32) {
        return 794. ;
    } else if (phi == 35) {
        return 957. ;
    } else if (phi == 38) {
        return 1140.;
    } else if (phi == 41) {
        return 1340.;
    } else if (phi == 51) {
        return 2027.;
    }

    return 0;
}

qreal SectionCalculations::getBarArea(const QString &phi)
{
    if ( phi.isEmpty() ) return 0.0;

    QChar symbol = phi.at(0);
    QStringList strlist = phi.split(symbol);
    int pi = strlist.at(1).toInt();
    int pj = ( strlist.count() > 2 ) ? strlist.at(2).toInt() : 0 ;

    qreal bararea;
    if ( symbol == 'D' || symbol == 'E' || symbol == 'F' || symbol == 'S' ) {
        bararea = barPhiToArea(pi);
        if ( pj > 0 ) {
            bararea += barPhiToArea(pj);
            bararea = bararea / 2.0;
        }
    } else {
        bararea = qreal(pi) * qreal(pi) / 4.0 * M_PI;
        if ( pj > 0 ) {
            bararea += qreal(pj) * qreal(pj) / 4.0 * M_PI;
            bararea = bararea / 2.0;
        }
    }
    return bararea;
}

//柱の軸鉄筋の本数（外側のみ/芯のみ）
int SectionCalculations::countColumnBar(const BarArrangement &bar1,
                                        const BarArrangement &bar2,
                                        ConcreteType::Type cshape)
{
    int count = 0;

    if (cshape == ConcreteType::Type::RECTANGLE) {
        for (int i = 0; i < bar1.size(); ++i) {
            count += bar1.numsum[i];
        }
        for (int i = 0; i < bar2.size(); ++i) {
            count += bar2.numsum[i];
        }
        if (bar1.numleft[0] != 0) {
            count--;
        }
        if (bar1.numleft[1] != 0) {
            count--;
        }
        if (bar1.numright[0] != 0) {
            count--;
        }
        if (bar1.numright[1] != 0) {
            count--;
        }
    } else if (cshape == ConcreteType::Type::CIRCLE) {
        count = bar1.numsum[0];
    } else {
        return 0;
    }

    return count;
}

int SectionCalculations::countGirderBar(const BarArrangement &bar1, const BarArrangement &bar2)
{
    int count = 0;
    for (int i = 0; i < bar1.size(); ++i) {
        count += bar1.numsum[i];
    }
    for (int i = 0; i < bar2.size(); ++i) {
        count += bar2.numsum[i];
    }
    return count;
}

int SectionCalculations::countTensionBar(const LinePartsParameterRC &paramRC, int side,
                                         DATATYPE mtype)
{
//・矩形断面のみ対応（円形・T字×）
//side ==0:左辺引張
//       1:右辺引張
//       2:上辺引張
//       3:下辺引張
    if (paramRC.concType != ConcreteType::Type::RECTANGLE) {
        return -1;   //矩形以外NG
    }

    //主筋の本数
    if (mtype != DATATYPE::TPCOLUMN || mtype != DATATYPE::TPBRACE)goto endcornerbar;
    int hasTLbar = paramRC.mainBarArrange2.numleft[0] == 0 ? 0 : 1;
    int hasTRbar = paramRC.mainBarArrange2.numright[0] == 0 ? 0 : 1;
    int hasBLbar = paramRC.mainBarArrange2.numleft[1] == 0 ? 0 : 1;
    int hasBRbar = paramRC.mainBarArrange2.numright[1] == 0 ? 0 : 1;
endcornerbar:

    int num = 0;
    if      ((mtype == DATATYPE::TPCOLUMN || mtype == DATATYPE::TPBRACE) && side == 0) {
        num = paramRC.mainBarArrange1.numsum[0] + paramRC.mainBarArrange2.numleft[0] +
              paramRC.mainBarArrange2.numleft[1] - hasTLbar - hasBLbar; //left
    } else if ((mtype == DATATYPE::TPCOLUMN || mtype == DATATYPE::TPBRACE) && side == 1) {
        num = paramRC.mainBarArrange1.numsum[1] + paramRC.mainBarArrange2.numright[0] +
              paramRC.mainBarArrange2.numright[1] - hasTRbar - hasBRbar; //right
    } else if ((mtype == DATATYPE::TPCOLUMN || mtype == DATATYPE::TPBRACE) && side == 2) {
        num = paramRC.mainBarArrange2.numsum[0] + paramRC.mainBarArrange1.numleft[0] +
              paramRC.mainBarArrange1.numleft[1] - hasTLbar - hasTRbar; //top
    } else if ((mtype == DATATYPE::TPCOLUMN || mtype == DATATYPE::TPBRACE) && side == 3) {
        num = paramRC.mainBarArrange2.numsum[1] + paramRC.mainBarArrange1.numright[0] +
              paramRC.mainBarArrange1.numright[1] - hasBLbar - hasBRbar; //bottom

    } else if ((mtype == DATATYPE::TPGIRDER || mtype == DATATYPE::TPBEAM) && side == 0) {
        for (int i = 0; i < paramRC.mainBarArrange1.size(); ++i) {
            num += paramRC.mainBarArrange1.numleft[i];   //left
        }
        for (int i = 0; i < paramRC.mainBarArrange2.size(); ++i) {
            num += paramRC.mainBarArrange2.numleft[i];
        }
    } else if ((mtype == DATATYPE::TPGIRDER || mtype == DATATYPE::TPBEAM) && side == 1) {
        for (int i = 0; i < paramRC.mainBarArrange1.size(); ++i) {
            num += paramRC.mainBarArrange1.numright[i];   //right
        }
        for (int i = 0; i < paramRC.mainBarArrange2.size(); ++i) {
            num += paramRC.mainBarArrange2.numright[i];
        }
    } else if ((mtype == DATATYPE::TPGIRDER || mtype == DATATYPE::TPBEAM) && side == 2) {
        for (int i = 0; i < paramRC.mainBarArrange1.size(); ++i) {
            num += paramRC.mainBarArrange1.numsum[i];   //top
        }
    } else if ((mtype == DATATYPE::TPGIRDER || mtype == DATATYPE::TPBEAM) && side == 3) {
        for (int i = 0; i < paramRC.mainBarArrange2.size(); ++i) {
            num += paramRC.mainBarArrange2.numsum[i];   //bottom
        }
    } else {
        return 0;
    }
    return num;
}

int SectionCalculations::getBarDistance(const QString &phi, const QString &pattern)
{
    if ( phi.isEmpty() ) return 0;

    QChar symbol = phi.at(0);
    QStringList strlist = phi.split(symbol);
    int pi = strlist.at(1).toInt();

    int a1 = 0, a2 = 0, a3 = 0, b1 = 0, b2 = 0;
    switch (pi) {
    case 10:
        a1 = 65;
        a2 = 80;
        a3 = 70;
        b1 = 50;
        b2 = 150;
        break;
    case 13:
        a1 = 70;
        a2 = 80;
        a3 = 75;
        b1 = 50;
        b2 = 150;
        break;
    case 16:
        a1 = 70;
        a2 = 90;
        a3 = 80;
        b1 = 55;
        b2 = 150;
        break;
    case 19:
        a1 = 70;
        a2 = 95;
        a3 = 80;
        b1 = 60;
        b2 = 150;
        break;
    case 22:
        a1 = 75;
        a2 = 100;
        a3 = 85;
        b1 = 65;
        b2 = 150;
        break;
    case 25:
        a1 = 75;
        a2 = 105;
        a3 = 90;
        b1 = 75;
        b2 = 150;
        break;
    case 29:
        a1 = 80;
        a2 = 110;
        a3 = 95;
        b1 = 85;
        b2 = 165;
        break;
    case 32:
        a1 = 85;
        a2 = 120;
        a3 = 100;
        b1 = 90;
        b2 = 175;
        break;
    case 35:
        a1 = 90;
        a2 = 130;
        a3 = 110;
        b1 = 100;
        b2 = 180;
        break;
    case 38:
        a1 = 95;
        a2 = 140;
        a3 = 115;
        b1 = 105;
        b2 = 190;
        break;
    case 41:
        a1 = 100;
        a2 = 150;
        a3 = 125;
        b1 = 115;
        b2 = 200;
        break;
    case 51:
        a1 = 125;
        a2 = 180;
        a3 = 150;
        b1 = 140;
        b2 = 230;
        break;
    default:
        a1 = 0;
        a2 = 0;
        a3 = 0;
        b1 = 0;
        b2 = 0;
    }
    if (pattern == "a1") {
        return a1;
    } else if (pattern == "a2") {
        return a2;
    } else if (pattern == "a3") {
        return a3;
    } else if (pattern == "b1") {
        return b1;
    } else if (pattern == "b2") {
        return b2;
    } else {
        return 0;
    }
}

qreal SectionCalculations::getBarMaxPhi(const PlaneSectionParameter &palam,
                                        PlaneDirection::Direction dir, PlaneSide::Side side)
{
    qreal maxphi = 0;
    QList<qreal> barphi;
    if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::UPPER) {
        barphi << getBarMaxPhi(palam.sBar_upperCenterA.phi)
               << getBarMaxPhi(palam.sBar_upperEdgeA.phi)
               << getBarMaxPhi(palam.sBar_upperEdgeCenterB.phi);
    } else if (dir == PlaneDirection::Direction::SHORTSIDE && side == PlaneSide::Side::LOWER) {
        barphi << getBarMaxPhi(palam.sBar_lowerCenterA.phi)
               << getBarMaxPhi(palam.sBar_lowerEdgeA.phi)
               << getBarMaxPhi(palam.sBar_lowerEdgeCenterB.phi);
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::UPPER) {
        barphi << getBarMaxPhi(palam.lBar_upperCenterA.phi)
               << getBarMaxPhi(palam.lBar_upperEdgeA.phi)
               << getBarMaxPhi(palam.lBar_upperEdgeCenterB.phi);
    } else if (dir == PlaneDirection::Direction::LONGSIDE && side == PlaneSide::Side::LOWER) {
        barphi << getBarMaxPhi(palam.lBar_lowerCenterA.phi)
               << getBarMaxPhi(palam.lBar_lowerEdgeA.phi)
               << getBarMaxPhi(palam.lBar_lowerEdgeCenterB.phi);
    }

    for (int i = 0; i < barphi.count(); ++i)
        if (barphi.at(i) > maxphi) maxphi = barphi.at(i);
    return maxphi;
}

qreal SectionCalculations::getBarMaxPhi(const QString &phi)
{
    if ( phi.isEmpty() ) return 0.0;

    QChar symbol = phi.at(0);
    QStringList strlist = phi.split(symbol);
    int pi = strlist.at(1).toInt();
    int pj = ( strlist.count() > 2 ) ? strlist.at(2).toInt() : 0 ;

    return ( pj > 0 ) ? qreal(pj) : qreal(pi);
    /*
        qreal maxphi;//呼び径
        if(false){}
        else if(phi ==  "D6"    ){maxphi = 6. ;}
        else if(phi ==  "D6D10" ){maxphi = 10. ;}
        else if(phi ==  "D10"   ){maxphi = 10. ;}
        else if(phi ==  "D10D13"){maxphi = 13. ;}
        else if(phi ==  "D13"   ){maxphi = 13. ;}
        else if(phi ==  "D13D16"){maxphi = 16. ;}
        else if(phi ==  "D16"   ){maxphi = 16. ;}
        else if(phi ==  "D16D19"){maxphi = 19. ;}
        else if(phi ==  "D19"   ){maxphi = 19. ;}
        else if(phi ==  "D19D22"){maxphi = 22. ;}
        else if(phi ==  "D22"   ){maxphi = 22. ;}
        else if(phi ==  "D22D25"){maxphi = 25. ;}
        else if(phi ==  "D25"   ){maxphi = 25. ;}
        else if(phi ==  "D25D29"){maxphi = 29. ;}
        else if(phi ==  "D29"   ){maxphi = 29. ;}
        else if(phi ==  "D29D32"){maxphi = 32. ;}
        else if(phi ==  "D32"   ){maxphi = 32. ;}
        else if(phi ==  "D32D35"){maxphi = 35. ;}
        else if(phi ==  "D35"   ){maxphi = 35. ;}
        else if(phi ==  "D35D38"){maxphi = 38. ;}
        else if(phi ==  "D38"   ){maxphi = 38. ;}
        else if(phi ==  "D38D41"){maxphi = 41. ;}
        else if(phi ==  "D41"   ){maxphi = 41. ;}
        else if(phi ==  "D41D51"){maxphi = 51. ;}
        else if(phi ==  "D51"   ){maxphi = 51. ;}
        else {return 0;}
        return maxphi;
    */
}

qreal SectionCalculations::getDefaultBoltLength(int phi)
{
    if ( phi <= 0 ) return 0;
    return 35.0 * qreal(phi);
}

qreal SectionCalculations::getDefaultBoltHolePhi(int phi)
{
    if ( phi <= 0 ) return 5.0;
    return qreal(phi) + 5.0;
}

qreal SectionCalculations::getDefaultScrewPhi(int phi, bool isRegularPitch)
{
    //isRegularPitch=true : 並目(JIS) , false : 細目(ISO)
    if ( phi <= 0 ) return 6.0;
    qreal pi = qreal(phi);

    qreal screwphi;
    qreal screwheight;
    qreal screwpitch;

    //並目ねじ
    if (isRegularPitch == true) {
        if     (pi <= 1.2) {
            screwpitch = 0.25;
        } else if (pi <= 1.4) {
            screwpitch = 0.30;
        } else if (pi <= 1.8) {
            screwpitch = 0.35;
        } else if (pi <= 2.0) {
            screwpitch = 0.40;
        } else if (pi <= 2.6) {
            screwpitch = 0.45;
        } else if (pi <= 3.0) {
            screwpitch = 0.50;
        } else if (pi <= 3.5) {
            screwpitch = 0.60;
        } else if (pi <= 4.0) {
            screwpitch = 0.70;
        } else if (pi <= 4.5) {
            screwpitch = 0.75;
        } else if (pi <= 5.0) {
            screwpitch = 0.80;
        } else if (pi <= 7.0) {
            screwpitch = 1.00;
        } else if (pi <= 9.0) {
            screwpitch = 1.25;
        } else if (pi <= 11.0) {
            screwpitch = 1.50;
        } else if (pi <= 12.0) {
            screwpitch = 1.75;
        } else if (pi <= 16.0) {
            screwpitch = 2.00;
        } else if (pi <= 22.0) {
            screwpitch = 2.50;
        } else if (pi <= 27.0) {
            screwpitch = 3.00;
        } else if (pi <= 33.0) {
            screwpitch = 3.50;
        } else if (pi <= 39.0) {
            screwpitch = 4.00;
        } else if (pi <= 45.0) {
            screwpitch = 4.50;
        } else if (pi <= 52.0) {
            screwpitch = 5.00;
        } else if (pi <= 60.0) {
            screwpitch = 5.50;
        } else {
            screwpitch = 6.00;
        }
    }
    //細目ねじ
    else if (isRegularPitch == false) {
        if     (pi <= 1.8) {
            screwpitch = 0.20;
        } else if (pi <= 2.2) {
            screwpitch = 0.25;
        } else if (pi <= 3.5) {
            screwpitch = 0.35;
        } else if (pi <= 5.5) {
            screwpitch = 0.50;
        } else if (pi <= 7.0) {
            screwpitch = 0.75;
        } else if (pi <= 9.0) {
            screwpitch = 1.00;
        } else if (pi <= 11.0) {
            screwpitch = 1.25;
        } else if (pi <= 17.0) {
            screwpitch = 1.50;
        } else if (pi <= 28.0) {
            screwpitch = 2.00;
        } else if (pi <= 40.0) {
            screwpitch = 3.00;
        } else if (pi <= 68.0) {
            screwpitch = 4.00;
        } else {
            screwpitch = 6.00;
        }
    }

    screwheight = 0.866025 * screwpitch;
    screwphi = pi - 2.0 * screwheight;
    screwphi = qreal(int((screwphi + 0.0005) * 1000)) / 1000; //小数点以下4位を四捨五入
    return screwphi;
}

////// 高級計算 //////

qreal SectionCalculations::calcUnitWeight(const LineCommonParameter &line_param,
                                          const LinePartsParameter &parts_param,
                                          const SectionDataType &type)
{
    qreal unitweight = 0; //kN/m
    qreal unitweight_s = 7.85 * 9.8066; //kN/m3
    qreal unitweight_rc = 24; //kN/m3
    qreal unitweight_src = 25; //kN/m3
    qreal unitweight_c = 23; //kN/m3
    qreal unitfactor = 1. / 1000; //kN/m3 * mm2 -> N/m

    switch (type.getStructType()) {
    case STRUCTTYPE::STP_S:
        unitweight += calcA_S(parts_param) * unitweight_s * unitfactor;
        break;
    case STRUCTTYPE::STP_RC:
        unitweight += calcA_RC(parts_param) * unitweight_rc * unitfactor;
        break;
    case STRUCTTYPE::STP_SRC:
        unitweight += calcA_SRC(parts_param, !line_param.filledConcMaterial.isEmpty())
                      * unitweight_src * unitfactor;
        break;
    case STRUCTTYPE::STP_CFT:
        qreal mw = UnifiedInputData::getInInstance()->getMaterialValue(
                       DATATYPE::TPMACONC, line_param.filledConcMaterial).unitW1;
        if ( mw > 1.0e-3 ) unitweight_c = mw;
        qreal A_filledconc = 0;
        switch (parts_param.steelType) { //P,BXかつhasFilledConc false→中空
        case SteelType::Type::ST_P  :
            if (!line_param.filledConcMaterial.isEmpty()) {
                A_filledconc = M_PI * qPow((parts_param.sizeH - 2 * parts_param.t1) / 2, 2);
            }
            break;
        case SteelType::Type::ST_BX :
            if (!line_param.filledConcMaterial.isEmpty()) {
                A_filledconc = (parts_param.sizeH - 2 * parts_param.t2) * (parts_param.sizeB - 2 * parts_param.t1);
            }
            break;
        }
        unitweight += calcAs_CFT(parts_param) * unitweight_s * unitfactor; //s
        unitweight += A_filledconc * unitweight_c * unitfactor; //conc
        break;
    }
    return unitweight;
}

qreal SectionCalculations::calcPlaneUnitWeight(const PlaneSectionParameter &param)
{
    qreal unitweight = 0;
    qreal unitweight_s = 7.85 * 9.8066; //kN/m3
    qreal unitweight_rc = 24; //kN/m3
    qreal unitfactor = 1.0; //mm * kN/m3 -> N/m2

    unitweight += param.concThickness * unitweight_rc * unitfactor;
    unitweight += param.steelThickness * unitweight_s * unitfactor;
    return unitweight;
}
} // namespace post3dapp
