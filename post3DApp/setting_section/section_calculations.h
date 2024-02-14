#ifndef SECTION_CALCULATIONS_H
#define SECTION_CALCULATIONS_H

#include "define_section_struct.h"
namespace post3dapp{
class SectionCalculations
{
private:

    SectionCalculations(void);

public:

    static qreal calcUnitWeight(const LineCommonParameter &c_param,
                                const LinePartsParameter &p_param,
                                const SectionDataType &type);

    static qreal calcPlaneUnitWeight(const PlaneSectionParameter &param);

///// S /////

    static qreal calcA_S(const LinePartsParameterS &paramS);
    static qreal calcAe_S(const LinePartsParameterS &paramS);
    static qreal calcI_S(const LinePartsParameterS &paramS, int axis);
    static qreal calcZ_S(const LinePartsParameterS &paramS, int axis, qreal I_s);
    static qreal calcZe_S(const LinePartsParameterS &paramS, int axis);
    static qreal calcZp_S(const LinePartsParameterS &paramS, int axis);
    //static qreal calcSrs_S(qreal I,qreal A);//断面二次半径
    static qreal calcWTratio_S(const LinePartsParameterS &paramS, SectionPart::Part part);
    static SectionRank::Rank judgeRank(const LinePartsParameterS &paramS,
                                       DATATYPE mtype, SectionPart::Part part);
    static SectionRank::Rank judgeRankH_SN(const LinePartsParameterS &paramS,
                                           DATATYPE mtype);
//汎用的な呼び出し
    static qreal calcA_S(SteelType::Type, qreal, qreal, qreal, qreal);
    static qreal calcAe_S(SteelType::Type, qreal, qreal, qreal, qreal, qreal, qreal);
    static qreal calcI_S(SteelType::Type, int, qreal, qreal, qreal, qreal);
    static qreal calcZ_S(SteelType::Type, int, qreal, qreal, qreal, qreal, qreal);
    static qreal calcZe_S(SteelType::Type, int, qreal, qreal, qreal, qreal, qreal, qreal);
    static qreal calcZp_S(SteelType::Type, int, qreal, qreal, qreal, qreal);
    static qreal calcSrs_S(qreal, qreal); //断面二次半径
    static qreal calcWTratio_S(SteelType::Type, SectionPart::Part, qreal, qreal, qreal,
                               qreal); //幅厚比
    static SectionRank::Rank judgeRank(SteelType::Type, SectionPart::Part, DATATYPE, qreal, qreal);
    static SectionRank::Rank judgeRankH_SN(DATATYPE, qreal, qreal, qreal, qreal, int); //

///// RC /////
    static qreal calcA_RC(const LinePartsParameterRC &paramRC);
    static qreal calcAs_RC(const LinePartsParameterRC &paramRC, DATATYPE mtype);
    static qreal calcAs_RC(int num, const QString &phi); // 鉄筋本数を直接数えた場合
    static qreal calcAeq_RC(qreal A, qreal As, int n);
    static qreal calcI_RC(const LinePartsParameterRC &paramRC, int axis);
    static qreal calcIs_RC(const LinePartsParameterRC &paramRC, int axis,
                           DATATYPE mtype);
    static qreal calcIeq_RC(qreal I, qreal Is, int n);
    static qreal calcPg_RC(const LinePartsParameterRC &paramRC,
                           DATATYPE mtype);
    static qreal calcD_RC(const LinePartsParameterRC &paramRC,
                          int side, DATATYPE mtype);
    static qreal calcPt_RC(const LinePartsParameterRC &paramRC, int side,
                           DATATYPE mtype);//内部でcalcDを呼んでるので、下の方が多少軽い
    static qreal calcPt_RC(qreal at, qreal d, qreal width); //atはcountTensionBar→calcAs
    static qreal calcPw_RC(const LinePartsParameterRC &paramRC, int axis);

///// SRC /////
    static qreal calcA_SRC(const LinePartsParameterSRC &paramSRC, bool isFilled = false);
    static qreal calcAs_SRC(const LinePartsParameterSRC &paramSRC,
                            DATATYPE mtype);
    static qreal calcAeq_SRC(qreal A, qreal As, int n);
    static qreal calcI_SRC(const LinePartsParameterSRC &paramSRC, int axis, bool isFilled = false);
    static qreal calcIs_SRC(const LinePartsParameterSRC &paramSRC, int axis,
                            DATATYPE mtype);
    static qreal calcIeq_SRC(qreal I, qreal Is, int n);
    static qreal calcPg_SRC(const LinePartsParameterSRC &paramSRC,
                            DATATYPE mtype, bool isFilled = false);
    static qreal calcPt_SRC(const LinePartsParameterSRC &paramSRC, int side,
                            DATATYPE mtype);
    static qreal calcPw_SRC(const LinePartsParameterSRC &paramSRC, int axis);

///// CFT /////
    static qreal calcA_CFT(const LinePartsParameter &paramCFT);
    static qreal calcAs_CFT(const LinePartsParameter &paramCFT);
    static qreal calcAeq_CFT(qreal A, qreal As, int n);
    static qreal calcI_CFT(const LinePartsParameter &paramCFT, int axis);
    static qreal calcIs_CFT(const LinePartsParameter &paramCFT, int axis);
    static qreal calcIeq_CFT(qreal I, qreal Is, int n);

///// WALL /////
    static qreal calcPs_Wall(const PlaneSectionParameter &, PlaneDirection::Direction);
    static qreal calcAr_Wall(const PlaneSectionParameter &, PlaneDirection::Direction,
                             WallOpenSize::Size);
    static qreal calcAt_Wall(const PlaneSectionParameter &, PlaneSide::Side, SectionPart::Part);

///// SLAB /////
    static qreal calcAt_Slab(const PlaneSectionParameter &,
                             PlaneDirection::Direction, PlaneSide::Side, SectionPart::Part, PlanePart::Part);
    static qreal calcMa_Slab(const PlaneSectionParameter &,
                             PlaneDirection::Direction, PlaneSide::Side, SectionPart::Part, PlanePart::Part,
                             bool isLongtimeloading = true);

//汎用小関数
    //断面二次モーメント
    static qreal calcI_rect(qreal w, qreal h); //矩形
    static qreal calcI_circ(qreal D);//円形
    static qreal calcI_typeH(qreal H, qreal B, qreal t1, qreal t2); //H形
    static qreal calcI_typeI(qreal H, qreal B, qreal t1, qreal t2); //I形
    static qreal calcI_typeT(qreal H, qreal B, qreal t1, qreal t2); //T形
    static qreal calcI_lineBar(int numsum, int numside1, int numside2, qreal RK, qreal RY, qreal barA,
                               qreal height, qreal RKu = 0, qreal RKb = 0, qreal RY1 = 0); //鉄筋ライン配置
    static qreal calcI_rectColumnBar(const BarArrangement &main,
                                     const BarArrangement &sub,
                                     qreal RK, qreal RY, qreal barA, qreal height); //鉄筋四角配置
    static qreal calcI_circColumnBar(int barnum, qreal RK, qreal barA, qreal D); //鉄筋円柱配置
    //中立軸関係
    static qreal calcI_axisShift(qreal I, qreal A, qreal d, qreal shift); //"軸"のシフト
    static qreal calcNeutralAxis_typeT(qreal H, qreal B, qreal t1, qreal t2); //T形
    static qreal calcNeutralAxis(qreal A[], qreal d[], int num); //一般形
    //塑性断面係数
    static qreal calcZp_typeT(qreal H, qreal B, qreal t1, qreal t2); //T形
    static qreal calcZp_typeH(qreal H, qreal B, qreal t1, qreal t2); //H形
    static qreal calcZp_typeI(qreal H, qreal B, qreal t1, qreal t2); //I形
    //鉄筋数・断面積・距離
    static qreal getBarArea(int phi);//公称径→断面積
    static qreal getBarArea(const QString &phi);
    static qreal barPhiToArea(int phi);

    static int countColumnBar(const BarArrangement &bar1,
                              const BarArrangement &bar2, ConcreteType::Type cshape);
    static int countGirderBar(const BarArrangement &bar1,
                              const BarArrangement &bar2);
    static int countTensionBar(const LinePartsParameterRC &paramRC, int side,
                               DATATYPE mtype);
    static int getBarDistance(const QString &phi, const QString &pattern);
    static qreal getBarMaxPhi(const PlaneSectionParameter &palam,
                              PlaneDirection::Direction dir, PlaneSide::Side side);
    static qreal getBarMaxPhi(const QString &phi);
    static qreal getDefaultBoltLength(int phi);
    static qreal getDefaultBoltHolePhi(int phi);
    static qreal getDefaultScrewPhi(int phi, bool isRegularPitch = true);

};
} // namespace post3dapp
#endif // SECTION_CALCULATIONS_H
