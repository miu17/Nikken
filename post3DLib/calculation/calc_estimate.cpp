#include "calc_estimate.h"

#include "unified_inputdata.h"
namespace post3dapp{
CalculationEstimate::CalculationEstimate()
{

}

QString CalculationEstimate::exec() const{
    QString ret;
    ret += getDeadLoad() + "\n";
    ret += getDeadLoadDetail() + "\n";
    ret += getConcreteVolume() + "\n";
    ret += getShuttering() + "\n";
    ret += getSteel() + "\n";
    ret += getCovering() + "\n";
    ret += getJointLoadForAnalysis() + "\n";
    ret += getReinforcementByAnalysis() + "\n";
    ret += getReinforcementByInput();
    return ret;
}

QString CalculationEstimate::getDeadLoad() const{
    QString ret;
    ret += u8"固定荷重(kN)";
    ret += "\n";
    ret += u8" 階名      床     節点    大梁    小梁  大梁上  小梁上     柱     間柱     壁  ブレ－ス  層合計  LL面積  [/LL面積 ] 上階合計";
    ret += u8"               追加荷重                追加荷重 追加荷重                                           (M2)   (KN/M2)  ";
    ret += "\n";
    Q_FOREACH(auto floor, UnifiedInputData::getInInstance()->getFloorList()){
        ret += floor.floorName + "\n";
        auto members = UnifiedInputData::getInInstance()->memberListOfFloor(floor.UUID);
        Q_FOREACH(auto member, members){
            Q_UNUSED(member);
            // 線部材：CalculationLoad::appendSelfWeight
            // 面部材：CalculationLoad::calcForMemberArea()
            // 節点追加荷重
            // 大梁上・小梁上追加荷重
            // LL面積：スラブの面積？
            ret += "\n";
        }
    }

    ret += "----------------------------------------------------------------------------------------------------------------------------        ";
    ret += "\n";
    ret += " TOTAL  121238.  14910.  99409.  16650.   5263.   1197.  28121.      0.  42965.      0.  329753.  21639. [  15.24 ]                       ";
    ret += "\n";
    ret += " ( % )  ( 36.8) (  4.5) ( 30.1) (  5.0) (  1.6) (   .4) (  8.5) (   .0) ( 13.0) (   .0)  (100.0)                                         ";

    return ret;
}
QString CalculationEstimate::getDeadLoadDetail() const{
    QString ret;
    ret += u8"固定荷重の内訳 (KN)                                     BODY:く体   FL:各リスト仕上  FLL:床仕上リスト  FIRE:耐火被覆";
    ret += "\n";
    ret += u8" 階名            床     節点           大梁    小梁  大梁上  小梁上     柱     間柱     壁  ブレ－ス  層合計  LL面積  [/LL面積 ] 上階合計     ";
    ret += "\n";
    ret += u8"                      追加荷重                      追加荷重 追加荷重                                           (M2)   (KN/M2)           ";
    ret += "\n";
    Q_FOREACH(auto floor, UnifiedInputData::getInInstance()->getFloorList()){
        // 床：BODYはそのまま、FLLは床とは別にスラブ荷重から別途
        // 大梁：仕上げと躯体を別に
        //
    }
    ret += " -----------------------------------------------------------------------------------------------------------------------------------------    ";
    ret += "\n";
    ret += " TOTAL  BODY   90313.          BODY   93242.  15230.                  26579.      0.  37563.      0.                                     ";
    ret += "\n";
    ret += "        FL      8153.          FL      6166.   1420.                   1542.      0.   5402.      0.                                      ";
    ret += "\n";
    ret += "        FLL    22773.          FIRE       0.      0.                      0.      0.      0.      0.                                      ";
    ret += "\n";
    ret += "        TOTAL 121238.  14910.  TOTAL  99409.  16650.   5263.   1197.  28121.      0.  42965.      0.  329753.      0. [  15.24 ]          ";
    ret += "\n";
    ret += "        ( % ) ( 36.8) (  4.5)        ( 30.1) (  5.0) (  1.6) (   .4) (  8.5) (   .0) ( 13.0) (   .0)  (100.0)                              ";
    ret += "\n";

    return ret;
}
QString CalculationEstimate::getConcreteVolume() const{
    QString ret;
    ret += u8"コンクリ－ト体積 (M3)";
    return ret;
}
QString CalculationEstimate::getShuttering() const{
    QString ret;
    ret += u8"型枠面積 (M2)";
    return ret;
}
QString CalculationEstimate::getSteel() const{
    QString ret;
    ret += u8"鉄骨重量 (KN)";
    return ret;
}
QString CalculationEstimate::getCovering() const{
    QString ret;
    ret += u8"鉄骨耐火被覆重量 (KN) ・ 鉄骨表面積 (M2)   ( S造,SC造 部分 )";
    return ret;
}
QString CalculationEstimate::getJointLoadForAnalysis() const{
    QString ret;
    ret += u8"節点鉛直荷重(応力解析用) (KN)";
    return ret;
}
QString CalculationEstimate::getReinforcementByAnalysis() const{
    QString ret;
    ret += u8"解析モデルに基づく鉄筋重量(tf)";
    return ret;
}
QString CalculationEstimate::getReinforcementByInput() const{
    QString ret;
    ret += u8"形状入力情報に基づく鉄筋重量(tf)";
    return ret;
}

} // namespace post3dapp
