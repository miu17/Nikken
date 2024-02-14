#include "line_outputtext.h"
#include "abstracttext.h"
namespace post3dapp{

LineOutputText::LineOutputText(const QString &_key, OutputTextStatus _status, const QList<TextDataUnit> &_unit, const QList<OutputTextStatus> &_premiseStatus)
    :AbstractLineData(_key, _unit), myStatus(_status), premiseTypes(_premiseStatus){}

LineOutputText::LineOutputText(OutputTextStatus _status,
                               const QList<TextDataUnit>& _unit,
                               const QList<OutputTextStatus> &_premiseStatus)
    : AbstractLineData(outputTextStatusToCommand(_status), combineDataUnit(_status, _unit)),
      myStatus(_status),
      premiseTypes(_premiseStatus)
{
}

QString LineOutputText::outputTextStatusToCommand(OutputTextStatus status)
{
    switch(status){
    // SECTION Definition
    case OutputTextStatus::Definition_Ccs:      return u8"*CCS";
    case OutputTextStatus::Definition_Outgroupp:return u8"*OUTGROUPP";
    case OutputTextStatus::Definition_Outgroups:return u8"*OUTGROUPS";
    // SECTION Input
    case OutputTextStatus::Input_Outline:       return u8"*OUTLINE";
    case OutputTextStatus::Input_Lsgir:         return u8"*LSGIR";
    case OutputTextStatus::Input_Lscol:         return u8"*LSCOL";
    case OutputTextStatus::Input_Lswal:         return u8"*LSWAL";
    case OutputTextStatus::Input_Lsswa:         return u8"*LSSWA";
    case OutputTextStatus::Input_Lsfwa:         return u8"*LSFWA";
    case OutputTextStatus::Input_Lsbra:         return u8"*LSBRA";
    case OutputTextStatus::Input_Lsbeam:        return u8"*LSBEAM";
    case OutputTextStatus::Input_Lsslab:        return u8"*LSSLAB";
    case OutputTextStatus::Input_Lsopen:        return u8"*LSOPEN";
    case OutputTextStatus::Input_Lsbs:          return u8"*LSBS";
    case OutputTextStatus::Input_Lsbsrc:        return u8"*LSBSRC";
    case OutputTextStatus::Input_Lsdamp:        return u8"*LSDAMP";
    case OutputTextStatus::Input_Lsmen:         return u8"*LSMEN";
    case OutputTextStatus::Input_Ldl:           return u8"*LDL";
    case OutputTextStatus::Input_Ldf:           return u8"*LDF";
    case OutputTextStatus::Input_Ldm:           return u8"*LDM";
    case OutputTextStatus::Input_Ldj:           return u8"*LDJ";
    case OutputTextStatus::Input_Estim:         return u8"*ESTIM";
    case OutputTextStatus::Input_Anacon:        return u8"*ANACON";
    // SECTION Prem
    case OutputTextStatus::Prem_Floor:          return u8"*FLOOR";
    case OutputTextStatus::Prem_Frame:          return u8"*FRAME";
    case OutputTextStatus::Prem_Stt:            return u8"*STT";
    case OutputTextStatus::Prem_Matc:           return u8"*MATC";
    case OutputTextStatus::Prem_Matr:           return u8"*MATR";
    case OutputTextStatus::Prem_Mats:           return u8"*MATS";
    case OutputTextStatus::Prem_Alist:          return u8"*ALIST";
    case OutputTextStatus::Prem_Jnt:            return u8"*JNT";
    case OutputTextStatus::Prem_Index:          return u8"*INDEX";
    case OutputTextStatus::Prem_D3:             return u8"*D3";
    case OutputTextStatus::Prem_Rfloor:         return u8"*RFLOOR";
    case OutputTextStatus::Prem_D4Joint:        return u8"*D4JOINT";
    case OutputTextStatus::Prem_Member:         return u8"*MEMBER";
    case OutputTextStatus::Prem_Wall:           return u8"*WALL";
    case OutputTextStatus::Prem_ZWall:          return u8"*ZWALL";
    case OutputTextStatus::Prem_Slab:           return u8"*SLAB";
    case OutputTextStatus::Prem_Panel:          return u8"*PANEL";
    case OutputTextStatus::Prem_SpringList:     return u8"*SPRINGLIST";
    case OutputTextStatus::Prem_Jspring:        return u8"*JSPRING";
    case OutputTextStatus::Prem_RlForSpring:    return u8"*RLFORSPRING";
    case OutputTextStatus::Prem_AxialSpring:    return u8"*AXIALSPRING";
    case OutputTextStatus::Prem_ShearSpring:    return u8"*SHEARSPRING";
    case OutputTextStatus::Prem_MssSpring:      return u8"*MSSSPRING";
    case OutputTextStatus::Prem_BsSpring:       return u8"*BSSPRING";
    case OutputTextStatus::Prem_TorqueSpring:   return u8"*TORQUESPRING";
    case OutputTextStatus::Prem_WallSpring:     return u8"*WALLSPRING";
    case OutputTextStatus::Prem_MemberEndSpring:return u8"*MEMBERENDSPRING";
    // SECTION Pres
    case OutputTextStatus::Pres_SeismicLoad:    return u8"*SEISMICLOAD";
    case OutputTextStatus::Pres_WindLoad:       return u8"*WINDLOAD";
    case OutputTextStatus::Pres_JointLoad:      return u8"*JOINTLOAD";
    // SECTION Prea
    case OutputTextStatus::Prea_FrameModel:     return u8"*FRAMEMODEL";
    case OutputTextStatus::Prea_QuColumn:       return u8"*QUCOLUMN";
    case OutputTextStatus::Prea_QuGirder:       return u8"*QUGIRDER";
    case OutputTextStatus::Prea_QuWall:         return u8"*QUWALL";
    case OutputTextStatus::Prea_NuBrace:        return u8"*NUBRACE";
    case OutputTextStatus::Prea_Pnlmdl:         return u8"*PNLMDL";
    // SECTION Sols
    case OutputTextStatus::Sols_JointDisp:      return u8"*JOINTDISP";
    case OutputTextStatus::Sols_JointIForce:    return u8"*JOINTIFORCE";
    case OutputTextStatus::Sols_JointReact:     return u8"*JOINTREACT";
    case OutputTextStatus::Sols_MemberJStress:  return u8"*MEMBERJSTRESS";
    case OutputTextStatus::Sols_MemberFStress:  return u8"*MEMBERFSTRESS";
    case OutputTextStatus::Sols_WallStress:     return u8"*WALLSTRESS";
    case OutputTextStatus::Sols_SlabStress:     return u8"*SLABSTRESS";
    case OutputTextStatus::Sols_PanelStress:    return u8"*PANELSTRESS";
    case OutputTextStatus::Sols_JointSpringStr: return u8"*JOINTSPRINGSTR";
    case OutputTextStatus::Sols_AxialSpringStr: return u8"*AXIALSPRINGSTR";
    case OutputTextStatus::Sols_ShearSpringStr: return u8"*SHEARSPRINGSTR";
    case OutputTextStatus::Sols_MssSpringStr:   return u8"*MSSSPRINGSTR";
    case OutputTextStatus::Sols_BsSpringStr:    return u8"*BSSPRINGSTR";
    case OutputTextStatus::Sols_TorqueSpringStr:return u8"*TORQUESPRINGSTR";
    case OutputTextStatus::Sols_WallSpringStr:  return u8"*WALLSPRINGSTR";
    // SECTION Stec
    case OutputTextStatus::Stec_HfDistribution: return u8"*HFDISTRIBUTION";
    case OutputTextStatus::Stec_StoryStiffness: return u8"*STORYSTIFFNESS";
    case OutputTextStatus::Stec_MaximumDeform:  return u8"*MAXIMUMDEFORM";
    case OutputTextStatus::Stec_StfFactor:      return u8"*STFFACTOR";
    case OutputTextStatus::Stec_EccFactor:      return u8"*ECCFACTOR";
    case OutputTextStatus::Stec_FrameStiffness: return u8"*FRAMESTIFFNESS";
    case OutputTextStatus::Stec_FrDistribution: return u8"*FRDISTRIBUTION";
    // SECTION Chck
    case OutputTextStatus::Chck_FrCheckSummary: return u8"*FRCHECKSUMMARY";
    case OutputTextStatus::Chck_CheckSummary1:  return u8"*CHECKSUMMARY1";
    case OutputTextStatus::Chck_CheckSummary2:  return u8"*CHECKSUMMARY2";
    case OutputTextStatus::Chck_ColumnCheck:    return u8"*COLUMNCHECK";
    case OutputTextStatus::Chck_GirderCheck:    return u8"*GIRDERCHECK";
    case OutputTextStatus::Chck_BraceCheck:     return u8"*BRACECHECK";
    case OutputTextStatus::Chck_WallCheck:      return u8"*WALLCHECK";
    // SECTION Qdcv
    case OutputTextStatus::Qdcv_StoryPlastInfo: return u8"*STORYPLASTINFO";
    case OutputTextStatus::Qdcv_QdCurve:        return u8"*QDCURVE";
    case OutputTextStatus::Qdcv_FrQdCurve:      return u8"*FRQDCURVE";
    case OutputTextStatus::Qdcv_FrDistPlast:    return u8"*FRDISTPLAST";
    case OutputTextStatus::Qdcv_QuNeed:         return u8"*QUNEED";
    case OutputTextStatus::Qdcv_UltimateState:  return u8"*ULTIMATESTATE";
    case OutputTextStatus::Qdcv_FrMemUlt:       return u8"*FRMEMULT";
    case OutputTextStatus::Qdcv_FrRank:         return u8"*FRRANK";
    case OutputTextStatus::Qdcv_FrMemR:         return u8"*FRMEMR";
    case OutputTextStatus::Qdcv_GirderRank:     return u8"*GIRDERRANK";
    case OutputTextStatus::Qdcv_ColumnRank:     return u8"*COLUMNRANK";
    // SECTION Result
    case OutputTextStatus::Result_CwArea:       return u8"*CWAREA";
    case OutputTextStatus::Result_Message:      return u8"*MESSAGE";
    case OutputTextStatus::FinalEnd:            return u8"*FINALEND";
    default:
        throw;
    }
}

QString LineOutputText::outputTextStatusToJapanese(OutputTextStatus status)
{
    switch(status){
    // SECTION Definition
    case OutputTextStatus::Definition_Ccs:      return u8"応力解析ケース";
    case OutputTextStatus::Definition_Outgroupp:return u8"*OUTGROUPP";
    case OutputTextStatus::Definition_Outgroups:return u8"*OUTGROUPS";
    // SECTION Input
    case OutputTextStatus::Input_Outline:       return u8"建築物の構造概要";
    case OutputTextStatus::Input_Lsgir:         return u8"大梁リスト";
    case OutputTextStatus::Input_Lscol:         return u8"柱リスト";
    case OutputTextStatus::Input_Lswal:         return u8"壁リスト";
    case OutputTextStatus::Input_Lsswa:         return u8"鋼板壁リスト";
    case OutputTextStatus::Input_Lsfwa:         return u8"地下外壁リスト";
    case OutputTextStatus::Input_Lsbra:         return u8"ブレースリスト";
    case OutputTextStatus::Input_Lsbeam:        return u8"小梁リスト";
    case OutputTextStatus::Input_Lsslab:        return u8"スラブリスト";
    case OutputTextStatus::Input_Lsopen:        return u8"開口リスト";
    case OutputTextStatus::Input_Lsbs:          return u8"S露出柱脚リスト";
    case OutputTextStatus::Input_Lsbsrc:        return u8"SRC非埋込柱脚リスト";
    case OutputTextStatus::Input_Lsdamp:        return u8"ダンパーリスト";
    case OutputTextStatus::Input_Lsmen:         return u8"免震装置リスト";
    case OutputTextStatus::Input_Ldl:           return u8"床荷重リスト(仕上荷重)";
    case OutputTextStatus::Input_Ldf:           return u8"床荷重リスト(積載荷重)";
    case OutputTextStatus::Input_Ldm:           return u8"部材荷重リスト";
    case OutputTextStatus::Input_Ldj:           return u8"節点荷重リスト";
    case OutputTextStatus::Input_Estim:         return u8"資材量重量集計";
    case OutputTextStatus::Input_Anacon:        return u8"解析条件の設定";
    // SECTION Prem
    case OutputTextStatus::Prem_Floor:          return u8"階、耐震計算ルート／通りの出力"; // *CALCの計算ルート部分
    case OutputTextStatus::Prem_Frame:          return u8"架構の代表節点・計算角度の出力";
    case OutputTextStatus::Prem_Stt:            return u8"構造形式・材料";
    case OutputTextStatus::Prem_Matc:           return u8"コンクリートの材料定数・許容応力度・材料強度";
    case OutputTextStatus::Prem_Matr:           return u8"鉄筋の材料定数・許容応力度・材料強度";
    case OutputTextStatus::Prem_Mats:           return u8"鋼材の材料定数・許容応力度・材料強度";
    case OutputTextStatus::Prem_Alist:          return u8"解析部材リスト";
    case OutputTextStatus::Prem_Jnt:            return u8"節点の出力";
    case OutputTextStatus::Prem_Index:          return u8"節点(MATRIX INDEX)の出力";
    case OutputTextStatus::Prem_D3:             return u8"従属変位条件の出力";
    case OutputTextStatus::Prem_Rfloor:         return u8"剛床の特性";
    case OutputTextStatus::Prem_D4Joint:        return u8"剛床節点の出力"; // 計算書にない
    case OutputTextStatus::Prem_Member:         return u8"部材(柱・大梁・ブレース)構成の出力"; // 計算書になし
    case OutputTextStatus::Prem_Wall:           return u8"耐震壁構成の出力"; // 計算書になし
    case OutputTextStatus::Prem_ZWall:          return u8"雑壁構成の出力"; // 計算書になし
    case OutputTextStatus::Prem_Slab:           return u8"スラブ(平面応力要素)構成の出力"; // 計算書になし
    case OutputTextStatus::Prem_Panel:          return u8"接合部パネル構成の出力";
    case OutputTextStatus::Prem_SpringList:     return u8"ばね復元力特性リストの出力";
    case OutputTextStatus::Prem_Jspring:        return u8"節点ばねモデルの出力";
    case OutputTextStatus::Prem_RlForSpring:    return u8"ばね剛端域の出力";
    case OutputTextStatus::Prem_AxialSpring:    return u8"軸ばねモデルの出力"; // 計算書にない
    case OutputTextStatus::Prem_ShearSpring:    return u8"せん断ばねモデルの出力";
    case OutputTextStatus::Prem_MssSpring:      return u8"MSSモデル出力";
    case OutputTextStatus::Prem_BsSpring:       return u8"曲げせん断ばねモデル出力"; // 計算書にない
    case OutputTextStatus::Prem_TorqueSpring:   return u8"回転ばねモデル出力";
    case OutputTextStatus::Prem_WallSpring:     return u8"壁式せん断ばね出力";
    case OutputTextStatus::Prem_MemberEndSpring:return u8"材端ばね出力";
    // SECTION Pres
    case OutputTextStatus::Pres_SeismicLoad:    return u8"地震荷重の出力";
    case OutputTextStatus::Pres_WindLoad:       return u8"風荷重の出力";
    case OutputTextStatus::Pres_JointLoad:      return u8"節点荷重の出力";
    // SECTION Prea
    case OutputTextStatus::Prea_FrameModel:     return u8"架構毎のモデル化条件の出力";
    case OutputTextStatus::Prea_QuColumn:       return u8"柱のせん断耐力の出力";
    case OutputTextStatus::Prea_QuGirder:       return u8"大梁のせん断耐力の出力";
    case OutputTextStatus::Prea_QuWall:         return u8"耐力壁のせん断耐力の出力";
    case OutputTextStatus::Prea_NuBrace:        return u8"ブレースの軸耐力";
    case OutputTextStatus::Prea_Pnlmdl:         return u8"接合部パネルモデルの出力";
    // SECTION Sols
    case OutputTextStatus::Sols_JointDisp:      return u8"節点変位の出力";
    case OutputTextStatus::Sols_JointIForce:    return u8"節点内力の出力";
    case OutputTextStatus::Sols_JointReact:     return u8"節点反力の出力";
    case OutputTextStatus::Sols_MemberJStress:  return u8"節点位置部材応力の出力";
    case OutputTextStatus::Sols_MemberFStress:  return u8"フェイス位置部材応力の出力";
    case OutputTextStatus::Sols_WallStress:     return u8"耐震壁応力の出力";
    case OutputTextStatus::Sols_SlabStress:     return u8"スラブ(平面応力要素)応力の出力";
    case OutputTextStatus::Sols_PanelStress:    return u8"接合部パネル応力の出力";
    case OutputTextStatus::Sols_JointSpringStr: return u8"節点ばね応力の出力";
    case OutputTextStatus::Sols_AxialSpringStr: return u8"軸ばね応力の出力";
    case OutputTextStatus::Sols_ShearSpringStr: return u8"せん断ばね応力の出力";
    case OutputTextStatus::Sols_MssSpringStr:   return u8"MSS応力の出力";
    case OutputTextStatus::Sols_BsSpringStr:    return u8"曲げせん断ばね応力の出力";
    case OutputTextStatus::Sols_TorqueSpringStr:return u8"回転ばね応力の出力";
    case OutputTextStatus::Sols_WallSpringStr:  return u8"壁式せん断ばね応力の出力";
    // SECTION Stec
    case OutputTextStatus::Stec_HfDistribution: return u8"層せん断力分担率の出力";
    case OutputTextStatus::Stec_StoryStiffness: return u8"層剛性の出力";
    case OutputTextStatus::Stec_MaximumDeform:  return u8"最大層間変形の出力";
    case OutputTextStatus::Stec_StfFactor:      return u8"剛性率の出力";
    case OutputTextStatus::Stec_EccFactor:      return u8"偏心率の出力";
    case OutputTextStatus::Stec_FrameStiffness: return u8"架構の層剛性の出力";
    case OutputTextStatus::Stec_FrDistribution: return u8"架構の層せん断力分担の出力";
    // SECTION Chck
    case OutputTextStatus::Chck_FrCheckSummary: return u8"断面検討結果の概要(架構毎の余裕率)";
    case OutputTextStatus::Chck_CheckSummary1:  return u8"部材毎の余裕率";
    case OutputTextStatus::Chck_CheckSummary2:  return u8"リスト毎の余裕率分布";
    case OutputTextStatus::Chck_ColumnCheck:    return u8"柱の断面検討";
    case OutputTextStatus::Chck_GirderCheck:    return u8"大梁の断面検討";
    case OutputTextStatus::Chck_BraceCheck:     return u8"ブレースの断面検討";
    case OutputTextStatus::Chck_WallCheck:      return u8"壁の断面検討";
    // SECTION Qdcv
    case OutputTextStatus::Qdcv_StoryPlastInfo: return u8"弾塑性解析結果の概要の出力";
    case OutputTextStatus::Qdcv_QdCurve:        return u8"弾塑性解析経過の出力";
    case OutputTextStatus::Qdcv_FrQdCurve:      return u8"架構の弾塑性解析経過の出力";
    case OutputTextStatus::Qdcv_FrDistPlast:    return u8"弾塑性解析の架構の層せん断力分担の出力";
    case OutputTextStatus::Qdcv_QuNeed:         return u8"必要保有水平耐力の出力";
    case OutputTextStatus::Qdcv_UltimateState:  return u8"想定終局せん断力の出力";
    case OutputTextStatus::Qdcv_FrMemUlt:       return u8"架構毎の部材終局耐力の出力";
    case OutputTextStatus::Qdcv_FrRank:         return u8"架構毎の部材ランクの概要の出力";
    case OutputTextStatus::Qdcv_FrMemR:         return u8"架構毎の部材角";
    case OutputTextStatus::Qdcv_GirderRank:     return u8"大梁ランクの出力";
    case OutputTextStatus::Qdcv_ColumnRank:     return u8"柱ランクの出力";
    // SECTION Result
    case OutputTextStatus::Result_CwArea:       return u8"壁と柱の水平断面積の出力";
    case OutputTextStatus::Result_Message:      return u8"終了メッセージの出力";
    default:
        throw;
    }
}

bool LineOutputText::isInputStatus(OutputTextStatus status)
{
    switch(status){
    case OutputTextStatus::Input_Outline:       return u8"建築物の構造概要";
    case OutputTextStatus::Input_Lsgir:         return u8"大梁リスト";
    case OutputTextStatus::Input_Lscol:         return u8"柱リスト";
    case OutputTextStatus::Input_Lswal:         return u8"壁リスト";
    case OutputTextStatus::Input_Lsswa:         return u8"鋼板壁リスト";
    case OutputTextStatus::Input_Lsfwa:         return u8"地下外壁リスト";
    case OutputTextStatus::Input_Lsbra:         return u8"ブレースリスト";
    case OutputTextStatus::Input_Lsbeam:        return u8"小梁リスト";
    case OutputTextStatus::Input_Lsslab:        return u8"スラブリスト";
    case OutputTextStatus::Input_Lsopen:        return u8"開口リスト";
    case OutputTextStatus::Input_Lsbs:          return u8"S露出柱脚リスト";
    case OutputTextStatus::Input_Lsbsrc:        return u8"SRC非埋込柱脚リスト";
    case OutputTextStatus::Input_Lsdamp:        return u8"ダンパーリスト";
    case OutputTextStatus::Input_Lsmen:         return u8"免震装置リスト";
    case OutputTextStatus::Input_Ldl:           return u8"床荷重リスト(仕上荷重)";
    case OutputTextStatus::Input_Ldf:           return u8"床荷重リスト(積載荷重)";
    case OutputTextStatus::Input_Ldm:           return u8"部材荷重リスト";
    case OutputTextStatus::Input_Ldj:           return u8"節点荷重リスト";
    case OutputTextStatus::Input_Estim:         return u8"資材量重量集計";
    case OutputTextStatus::Input_Anacon:        return u8"解析条件の設定";
    // SECTION Prem
    case OutputTextStatus::Prem_Floor:          return u8"階、耐震計算ルート／通りの出力"; // *CALCの計算ルート部分
    case OutputTextStatus::Prem_Frame:          return u8"架構の代表節点・計算角度の出力";
    case OutputTextStatus::Prem_Stt:            return u8"構造形式・材料";
    case OutputTextStatus::Prem_Matc:           return u8"コンクリートの材料定数・許容応力度・材料強度";
    case OutputTextStatus::Prem_Matr:           return u8"鉄筋の材料定数・許容応力度・材料強度";
    case OutputTextStatus::Prem_Mats:           return u8"鋼材の材料定数・許容応力度・材料強度";
        return true;
    default:
        return false;
    }
}

bool LineOutputText::MatchKeyword(OutputTextStatus status, const QString& text) const{
    if (premiseTypes.count() > 0 && !premiseTypes.contains(status))
        return false;

    if (key == "" || key == "-"){
        return text.startsWith(key);
    }else{
        auto split = text.trimmed().split(' ');
        return split[0] == key;
    }
}

QList<TextDataUnit> LineOutputText::combineDataUnit(OutputTextStatus status, const QList<TextDataUnit> &unitList)
{
    TextDataUnit unit = TextDataUnit(TEXTTYPE::TYPE_TEXT, 1, outputTextStatusToCommand(status).length());
    QList<TextDataUnit> ret{unit};
    if (unitList.count() != 0)
        ret << unitList;
    return ret;
}

} // namespace post3dapp
