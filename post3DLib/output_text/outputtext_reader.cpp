#include "outputtext_reader.h"

#include "abstracttext.h"
#include "define_draw_figure.h"
#include "textdataunit.h"

namespace post3dapp{

QList<LineOutputText*> OutputTextReader::conditions =
        QList<LineOutputText*>{
        new LineOutputText(OutputTextStatus::Definition_Ccs),
        new LineOutputText(OutputTextStatus::Definition_Outgroupp),
        new LineOutputText(OutputTextStatus::Definition_Outgroups),
        new LineOutputText("-",
                       OutputTextStatus::Skip,
                       QList<TextDataUnit>{
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 1},
                           TextDataUnit{TEXTTYPE::TYPE_BLANK, 2, 10},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, false},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, false},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80, false}
                       },
                       QList<OutputTextStatus>{OutputTextStatus::Definition_Outgroupp, OutputTextStatus::Definition_Outgroups}),
        new LineOutputText("",
                       OutputTextStatus::Skip,
                       QList<TextDataUnit>{
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, false},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, false},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                           TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80, false}
                       },
                       QList<OutputTextStatus>{OutputTextStatus::Definition_Outgroupp, OutputTextStatus::Definition_Outgroups}),
        new LineOutputText(OutputTextStatus::Input_Outline),
        new LineOutputText(OutputTextStatus::Input_Lsgir),
        new LineOutputText(OutputTextStatus::Input_Lscol),
        new LineOutputText(OutputTextStatus::Input_Lswal),
        new LineOutputText(OutputTextStatus::Input_Lsswa),
        new LineOutputText(OutputTextStatus::Input_Lsfwa),
        new LineOutputText(OutputTextStatus::Input_Lsbra),
        new LineOutputText(OutputTextStatus::Input_Lsbeam),
        new LineOutputText(OutputTextStatus::Input_Lsslab),
        new LineOutputText(OutputTextStatus::Input_Lsopen),
        new LineOutputText(OutputTextStatus::Input_Lsbs),
        new LineOutputText(OutputTextStatus::Input_Lsbsrc),
        new LineOutputText(OutputTextStatus::Input_Lsdamp),
        new LineOutputText(OutputTextStatus::Input_Lsmen),
        new LineOutputText(OutputTextStatus::Input_Ldl),
        new LineOutputText(OutputTextStatus::Input_Ldf),
        new LineOutputText(OutputTextStatus::Input_Ldm),
        new LineOutputText(OutputTextStatus::Input_Ldj),
        new LineOutputText(OutputTextStatus::Input_Estim),
        new LineOutputText(OutputTextStatus::Input_Anacon),
        new LineOutputText(OutputTextStatus::Prem_Floor),
        new LineOutputText(OutputTextStatus::Prem_Frame),
        new LineOutputText(OutputTextStatus::Prem_Stt),
        new LineOutputText(OutputTextStatus::Prem_Matc),
        new LineOutputText(OutputTextStatus::Prem_Matr),
        new LineOutputText(OutputTextStatus::Prem_Mats),
        new LineOutputText(OutputTextStatus::Prem_Alist),
        new LineOutputText(OutputTextStatus::Prem_Jnt),
        new LineOutputText(OutputTextStatus::Prem_Index),
        new LineOutputText(OutputTextStatus::Prem_D3),
        new LineOutputText(OutputTextStatus::Prem_Rfloor),
        new LineOutputText(OutputTextStatus::Prem_D4Joint),
        new LineOutputText(OutputTextStatus::Prem_Member),
        new LineOutputText(OutputTextStatus::Prem_Wall),
        new LineOutputText(OutputTextStatus::Prem_ZWall),
        new LineOutputText(OutputTextStatus::Prem_Slab),
        new LineOutputText(OutputTextStatus::Prem_Panel),
        new LineOutputText(OutputTextStatus::Prem_SpringList),
        new LineOutputText(OutputTextStatus::Prem_Jspring),
        new LineOutputText(OutputTextStatus::Prem_RlForSpring),
        new LineOutputText(OutputTextStatus::Prem_AxialSpring),
        new LineOutputText(OutputTextStatus::Prem_ShearSpring),
        new LineOutputText(OutputTextStatus::Prem_MssSpring),
        new LineOutputText(OutputTextStatus::Prem_BsSpring),
        new LineOutputText(OutputTextStatus::Prem_TorqueSpring),
        new LineOutputText(OutputTextStatus::Prem_WallSpring),
        new LineOutputText(OutputTextStatus::Prem_Frame),
        new LineOutputText(OutputTextStatus::Prem_MemberEndSpring),
        new LineOutputText(OutputTextStatus::Pres_SeismicLoad),
        new LineOutputText(OutputTextStatus::Pres_WindLoad),
        new LineOutputText(OutputTextStatus::Pres_JointLoad),
        new LineOutputText(OutputTextStatus::Prea_FrameModel),
        new LineOutputText(OutputTextStatus::Prea_QuColumn),
        new LineOutputText(OutputTextStatus::Prea_QuGirder),
        new LineOutputText(OutputTextStatus::Prea_QuWall),
        new LineOutputText(OutputTextStatus::Prea_NuBrace),
        new LineOutputText(OutputTextStatus::Prea_Pnlmdl),
        new LineOutputText(OutputTextStatus::Sols_JointDisp),
        new LineOutputText(OutputTextStatus::Sols_JointIForce),
        new LineOutputText(OutputTextStatus::Sols_JointReact),
        new LineOutputText(OutputTextStatus::Sols_MemberJStress),
        new LineOutputText(OutputTextStatus::Sols_MemberFStress),
        new LineOutputText(OutputTextStatus::Sols_WallStress),
        new LineOutputText(OutputTextStatus::Sols_SlabStress),
        new LineOutputText(OutputTextStatus::Sols_PanelStress),
        new LineOutputText(OutputTextStatus::Sols_JointSpringStr),
        new LineOutputText(OutputTextStatus::Sols_AxialSpringStr),
        new LineOutputText(OutputTextStatus::Sols_ShearSpringStr),
        new LineOutputText(OutputTextStatus::Sols_MssSpringStr),
        new LineOutputText(OutputTextStatus::Sols_BsSpringStr),
        new LineOutputText(OutputTextStatus::Sols_TorqueSpringStr),
        new LineOutputText(OutputTextStatus::Sols_WallSpringStr),
        new LineOutputText(OutputTextStatus::Stec_HfDistribution),
        new LineOutputText(OutputTextStatus::Stec_StoryStiffness),
        new LineOutputText(OutputTextStatus::Stec_MaximumDeform),
        new LineOutputText(OutputTextStatus::Stec_StfFactor),
        new LineOutputText(OutputTextStatus::Stec_EccFactor),
        new LineOutputText(OutputTextStatus::Stec_FrameStiffness),
        new LineOutputText(OutputTextStatus::Stec_FrDistribution),
        new LineOutputText(OutputTextStatus::Chck_FrCheckSummary),
        new LineOutputText(OutputTextStatus::Chck_CheckSummary1),
        new LineOutputText(OutputTextStatus::Chck_CheckSummary2),
        new LineOutputText(OutputTextStatus::Chck_ColumnCheck),
        new LineOutputText(OutputTextStatus::Chck_GirderCheck),
        new LineOutputText(OutputTextStatus::Chck_BraceCheck),
        new LineOutputText(OutputTextStatus::Chck_WallCheck),
        new LineOutputText(OutputTextStatus::Qdcv_StoryPlastInfo),
        new LineOutputText(OutputTextStatus::Qdcv_QdCurve),
        new LineOutputText(OutputTextStatus::Qdcv_FrQdCurve),
        new LineOutputText(OutputTextStatus::Qdcv_FrDistPlast),
        new LineOutputText(OutputTextStatus::Qdcv_QuNeed),
        new LineOutputText(OutputTextStatus::Qdcv_UltimateState),
        new LineOutputText(OutputTextStatus::Qdcv_FrMemUlt),
        new LineOutputText(OutputTextStatus::Qdcv_FrRank),
        new LineOutputText(OutputTextStatus::Qdcv_FrMemR),
        new LineOutputText(OutputTextStatus::Qdcv_GirderRank),
        new LineOutputText(OutputTextStatus::Qdcv_ColumnRank),
        new LineOutputText(OutputTextStatus::Result_CwArea),
        new LineOutputText(OutputTextStatus::Result_Message),
};

LineOutputText* OutputTextReader::CheckLineCondition(const QString& text, OutputTextStatus status, QString* errorMessage){
    LineOutputText* condition = MatchLineCondition(text, status, errorMessage);
    if (*errorMessage != QString{} || condition == nullptr)
        return nullptr;

    if (condition->hasFormatError(text)){
        *errorMessage = u8"「" + text.trimmed() + u8"」のフォーマットが異なります。";
        return nullptr;
    }
    return condition;
}

LineOutputText* OutputTextReader::MatchLineCondition(const QString &text, OutputTextStatus status, QString* errorMessage){
    if (text.startsWith("//")){
        *errorMessage = QString{};
        return nullptr;
    }

    if (text.startsWith("*FINALEND")){
        *errorMessage = QString{};
        return new LineOutputText(OutputTextStatus::FinalEnd);
    }

    Q_FOREACH(auto condition, conditions){
        if (condition->MatchKeyword(status, text)){
            *errorMessage = QString{};
            return condition;
        }
    }
    *errorMessage = u8"「" + text.trimmed() + u8"」の行と一致するフォーマットが見つかりません。";
    return nullptr;
}

AbstractText *OutputTextReader::Create(OutputTextStatus status) const
{
    switch(status){
    // SECTION Definition
    case OutputTextStatus::Definition_Ccs:      return new TextCcs(status);
    // SECTION Input
    case OutputTextStatus::Input_Outline:       return new TextOutline(status);
    case OutputTextStatus::Input_Lsgir:         return new TextLsgir(status);
    case OutputTextStatus::Input_Lscol:         return new TextLscol(status);
    case OutputTextStatus::Input_Lswal:         return new TextLswal(status);
    case OutputTextStatus::Input_Lsswa:         return new TextLsswa(status);
    case OutputTextStatus::Input_Lsfwa:         return new TextLsfwa(status);
    case OutputTextStatus::Input_Lsbra:         return new TextLsbra(status);
    case OutputTextStatus::Input_Lsbeam:        return new TextLsbeam(status);
    case OutputTextStatus::Input_Lsslab:        return new TextLsslab(status);
    case OutputTextStatus::Input_Lsopen:        return new TextLsopen(status);
    case OutputTextStatus::Input_Lsbs:          return new TextLsbs(status);
    case OutputTextStatus::Input_Lsbsrc:        return new TextLsbsrc(status);
    case OutputTextStatus::Input_Lsdamp:        return new TextLsdamp(status);
    case OutputTextStatus::Input_Lsmen:         return new TextLsmen(status);
    case OutputTextStatus::Input_Ldl:           return new TextLdl(status);
    case OutputTextStatus::Input_Ldf:           return new TextLdf(status);
    case OutputTextStatus::Input_Ldm:           return new TextLdm(status);
    case OutputTextStatus::Input_Ldj:           return new TextLdj(status);
    case OutputTextStatus::Input_Estim:         return new TextEstim(status);
    case OutputTextStatus::Input_Anacon:        return new TextAnacon(status);
    // SECTION Prem
    case OutputTextStatus::Prem_Floor:          return new TextFloor(status);
    case OutputTextStatus::Prem_Stt:            return new TextStt(status);
    case OutputTextStatus::Prem_Matc:           return new TextMatc(status);
    case OutputTextStatus::Prem_Matr:           return new TextMatr(status);
    case OutputTextStatus::Prem_Mats:           return new TextMats(status);
    case OutputTextStatus::Prem_Alist:          return new TextAlist(status);
    case OutputTextStatus::Prem_Jnt:            return new TextJnt(status);
    case OutputTextStatus::Prem_Index:          return new TextIndex(status);
    case OutputTextStatus::Prem_D3:             return new TextD3(status);
    case OutputTextStatus::Prem_Rfloor:         return new TextRfloor(status);
    case OutputTextStatus::Prem_D4Joint:        return new TextD4Joint(status);
    case OutputTextStatus::Prem_Member:         return new TextMember(status);
    case OutputTextStatus::Prem_Wall:           return new TextWall(status);
    case OutputTextStatus::Prem_ZWall:          return new TextZWall(status);
    case OutputTextStatus::Prem_Slab:           return new TextSlab(status);
    case OutputTextStatus::Prem_Panel:          return new TextPanel(status);
    case OutputTextStatus::Prem_SpringList:     return new TextSpringList(status);
    case OutputTextStatus::Prem_Jspring:        return new TextJspring(status);
    case OutputTextStatus::Prem_RlForSpring:    return new TextRlForSpring(status);
    case OutputTextStatus::Prem_AxialSpring:    return new TextAxialSpring(status);
    case OutputTextStatus::Prem_ShearSpring:    return new TextShearSpring(status);
    case OutputTextStatus::Prem_MssSpring:      return new TextMssSpring(status);
    case OutputTextStatus::Prem_BsSpring:       return new TextBsSpring(status);
    case OutputTextStatus::Prem_TorqueSpring:   return new TextTorqueSpring(status);
    case OutputTextStatus::Prem_WallSpring:     return new TextWallSpring(status);
    case OutputTextStatus::Prem_MemberEndSpring:return new TextMemberEndSpring(status);
    // SECTION Pres
    case OutputTextStatus::Pres_SeismicLoad:    return new TextSeismicLoad(status);
    case OutputTextStatus::Pres_WindLoad:       return new TextWindLoad(status);
    case OutputTextStatus::Pres_JointLoad:      return new TextJointLoad(status);
    // SECTION Prea
    case OutputTextStatus::Prea_FrameModel:     return new TextFrameModel(status);
    case OutputTextStatus::Prea_QuColumn:       return new TextQuColumn(status);
    case OutputTextStatus::Prea_QuGirder:       return new TextQuGirder(status);
    case OutputTextStatus::Prea_QuWall:         return new TextQuWall(status);
    case OutputTextStatus::Prea_NuBrace:        return new TextNuBrace(status);
    case OutputTextStatus::Prea_Pnlmdl:         return new TextPnlmdl(status);
    // SECTION Sols
    case OutputTextStatus::Sols_JointDisp:      return new TextJointDisp(status);
    case OutputTextStatus::Sols_JointIForce:    return new TextJointIForce(status);
    case OutputTextStatus::Sols_JointReact:     return new TextJointReact(status);
    case OutputTextStatus::Sols_MemberJStress:  return new TextMemberJStress(status);
    case OutputTextStatus::Sols_MemberFStress:  return new TextMemberFStress(status);
    case OutputTextStatus::Sols_WallStress:     return new TextWallStress(status);
    case OutputTextStatus::Sols_SlabStress:     return new TextSlabStress(status);
    case OutputTextStatus::Sols_PanelStress:    return new TextPanelStress(status);
    case OutputTextStatus::Sols_JointSpringStr: return new TextJointSpringStr(status);
    case OutputTextStatus::Sols_AxialSpringStr: return new TextAxialSpringStr(status);
    case OutputTextStatus::Sols_ShearSpringStr: return new TextShearSpringStr(status);
    case OutputTextStatus::Sols_MssSpringStr:   return new TextMssSpringStr(status);
    case OutputTextStatus::Sols_BsSpringStr:    return new TextBsSpringStr(status);
    case OutputTextStatus::Sols_TorqueSpringStr:return new TextTorqueSpringStr(status);
    case OutputTextStatus::Sols_WallSpringStr:  return new TextWallSpringStr(status);
    // SECTION Stec
    case OutputTextStatus::Stec_HfDistribution: return new TextHfDistribution(status);
    case OutputTextStatus::Stec_StoryStiffness: return new TextStoryStiffness(status);
    case OutputTextStatus::Stec_MaximumDeform:  return new TextMaximumDeform(status);
    case OutputTextStatus::Stec_StfFactor:      return new TextStfFactor(status);
    case OutputTextStatus::Stec_EccFactor:      return new TextEccFactor(status);
    case OutputTextStatus::Stec_FrameStiffness: return new TextFrameStiffness(status);
    case OutputTextStatus::Stec_FrDistribution: return new TextFrDistribution(status);
    // SECTION Chck
    case OutputTextStatus::Chck_FrCheckSummary: return new TextFrCheckSummary(status);
    case OutputTextStatus::Chck_CheckSummary1:  return new TextCheckSummary1(status);
    case OutputTextStatus::Chck_CheckSummary2:  return new TextCheckSummary2(status);
    case OutputTextStatus::Chck_ColumnCheck:    return new TextColumnCheck(status);
    case OutputTextStatus::Chck_GirderCheck:    return new TextGirderCheck(status);
    case OutputTextStatus::Chck_BraceCheck:     return new TextBraceCheck(status);
    case OutputTextStatus::Chck_WallCheck:      return new TextWallCheck(status);
    // SECTION Qdcv
    case OutputTextStatus::Qdcv_StoryPlastInfo: return new TextStoryPlastInfo(status);
    case OutputTextStatus::Qdcv_QdCurve:        return new TextQdCurve(status);
    case OutputTextStatus::Qdcv_FrQdCurve:      return new TextFrQdCurve(status);
    case OutputTextStatus::Qdcv_FrDistPlast:    return new TextFrDistPlast(status);
    case OutputTextStatus::Qdcv_QuNeed:         return new TextQuNeed(status);
    case OutputTextStatus::Qdcv_UltimateState:  return new TextUltimateState(status);
    case OutputTextStatus::Qdcv_FrMemUlt:       return new TextFrMemUlt(status);
    case OutputTextStatus::Qdcv_FrRank:         return new TextFrRank(status);
    case OutputTextStatus::Qdcv_FrMemR:         return new TextFrMemR(status);
    case OutputTextStatus::Qdcv_GirderRank:     return new TextGirderRank(status);
    case OutputTextStatus::Qdcv_ColumnRank:     return new TextColumnRank(status);
    case OutputTextStatus::Result_CwArea:       return new TextCwArea(status);
    case OutputTextStatus::Result_Message:      return new TextMessage(status);
    default:
        throw;
    }
}

QList<AbstractText *> OutputTextReader::fastParse(const QStringList &lines, QString *errorMessage) const
{
    return parse(lines, errorMessage, false);
}

QList<AbstractText *> OutputTextReader::cliParse(const QStringList &lines, QString *errorMessage, bool hasInput, bool hasOutput) const
{
    return parse(lines, errorMessage, true, hasInput, hasOutput);
}

QList<AbstractText *> OutputTextReader::appParse(const QStringList &lines, QString *errorMessage) const
{
    return parse(lines, errorMessage, true);
}

QList<AbstractText *> OutputTextReader::parse(const QStringList &lines, QString *errorMessage, bool readAll, bool hasInput, bool hasOutput) const
{
    QList<AbstractText *> ret;
    AbstractText* aText = nullptr;
    OutputTextStatus status = OutputTextStatus::None;
    QString lastDefinition;
    QHash<QString, QList<STRESS>> combinationStress;
    Q_FOREACH(QString line, lines){
        auto condition = CheckLineCondition(line, status, errorMessage);
        if (errorMessage != QString{})
            return ret;

        if (condition == nullptr)
            continue;

        if (line.startsWith("*FINALEND")){
            break;
        }

        if (condition->getStatus() != OutputTextStatus::Skip){
            if (aText != nullptr)
                ret.append(aText);
            status = condition->getStatus();
            switch(status){
            case OutputTextStatus::Definition_Ccs:
            //case OutputTextStatus::Definition_Outgroupp:
            //case OutputTextStatus::Definition_Outgroups:
                break;
            default:

                aText = Create(status);
            }
        }else{
            if (!readAll)
                continue;

            switch(status){
            case OutputTextStatus::Definition_Ccs:
                lastDefinition = ParseCcsType(line, lastDefinition, &combinationStress);
                break;
            default:
            // aTextに要素を割り当て
                break;
            }
        }
    }
    if (aText != nullptr)
        ret.append(aText);

    if (readAll)
        *errorMessage = checkText(ret, hasInput, hasOutput);

    return ret;

}

QString OutputTextReader::checkText(const QList<AbstractText *> &abstractText, bool hasInput, bool hasOutput) const
{
    return QString{};
}

bool OutputTextReader::hasFormatError(const QString& plainText){
    if (plainText == QString())
        return false;

    QStringList list = plainText.split("\n");
    OutputTextStatus status = OutputTextStatus::None;
    QString errorMessage;

    Q_FOREACH(auto str, list){
        LineOutputText* condition = CheckLineCondition(str, status, &errorMessage);
        if (errorMessage != QString{})
            return true;

        if (condition->getStatus() != OutputTextStatus::Skip)
            status = condition->getStatus();

        if (status == OutputTextStatus::FinalEnd)
            return false;
    }
    return false;
}

} // namespace post3dapp
