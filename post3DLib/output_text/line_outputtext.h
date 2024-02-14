#ifndef LINEOUTPUTTEXT_H
#define LINEOUTPUTTEXT_H

#include "abstractlinedata.h"
#include "textdataunit.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{

enum class OutputTextStatus{
    None,
    Skip,
    // SECTION Definition
    Definition_Ccs,
    Definition_Outgroupp,
    Definition_Outgroups,
    // SECTION Input
    Input_Outline,
    Input_Lsgir,
    Input_Lscol,
    Input_Lswal,
    Input_Lsswa,
    Input_Lsfwa,
    Input_Lsbra,
    Input_Lsbeam,
    Input_Lsslab,
    Input_Lsopen,
    Input_Lsbs,
    Input_Lsbsrc,
    Input_Lsdamp,
    Input_Lsmen,
    Input_Ldl,
    Input_Ldf,
    Input_Ldm,
    Input_Ldj,
    Input_Estim,
    Input_Anacon,
    // SECTION2 PREM
    Prem_Floor,
    Prem_Stt,
    Prem_Matc,
    Prem_Matr,
    Prem_Mats,
    Prem_Alist,
    Prem_Jnt,
    Prem_Index,
    Prem_D3,
    Prem_Rfloor,
    Prem_D4Joint,
    Prem_Member,
    Prem_Wall,
    Prem_ZWall,
    Prem_Slab,
    Prem_Panel,
    Prem_SpringList,
    Prem_Jspring,
    Prem_RlForSpring,
    Prem_AxialSpring,
    Prem_ShearSpring,
    Prem_MssSpring,
    Prem_BsSpring,
    Prem_TorqueSpring,
    Prem_WallSpring,
    Prem_Frame,
    Prem_MemberEndSpring,
    // SECTION3 PRES
    Pres_SeismicLoad,
    Pres_WindLoad,
    Pres_JointLoad,
    // SECTION4 PREA
    Prea_FrameModel,
    Prea_QuColumn,
    Prea_QuGirder,
    Prea_QuWall,
    Prea_NuBrace,
    Prea_Pnlmdl,
    // SECTION5 SOLS
    Sols_JointDisp,
    Sols_JointIForce,
    Sols_JointReact,
    Sols_MemberJStress,
    Sols_MemberFStress,
    Sols_WallStress,
    Sols_SlabStress,
    Sols_PanelStress,
    Sols_JointSpringStr,
    Sols_AxialSpringStr,
    Sols_ShearSpringStr,
    Sols_MssSpringStr,
    Sols_BsSpringStr,
    Sols_TorqueSpringStr,
    Sols_WallSpringStr,
    // SECTION6 STEC
    Stec_HfDistribution,
    Stec_StoryStiffness,
    Stec_MaximumDeform,
    Stec_StfFactor,
    Stec_EccFactor,
    Stec_FrameStiffness,
    Stec_FrDistribution,
    // Section7 CHCK
    Chck_FrCheckSummary,
    Chck_CheckSummary1,
    Chck_CheckSummary2,
    Chck_ColumnCheck,
    Chck_GirderCheck,
    Chck_BraceCheck,
    Chck_WallCheck,
    // Section8 QDCV
    Qdcv_StoryPlastInfo,
    Qdcv_QdCurve,
    Qdcv_FrQdCurve,
    Qdcv_FrDistPlast,
    Qdcv_QuNeed,
    Qdcv_UltimateState,
    Qdcv_FrMemUlt,
    Qdcv_FrRank,
    Qdcv_FrMemR,
    Qdcv_GirderRank,
    Qdcv_ColumnRank,
    // Section Reuslt
    Result_CwArea,
    Result_Message,
    FinalEnd,
};

class POST3D_CLASS_DLLSPEC LineOutputText : public AbstractLineData
{
public:
    explicit LineOutputText(const QString& _key = QString{},
                        OutputTextStatus _status = OutputTextStatus::None,
                        const QList<TextDataUnit>& _unit = QList<TextDataUnit>(),
                        const QList<OutputTextStatus>& _premiseStatus = QList<OutputTextStatus>());
    explicit LineOutputText(OutputTextStatus _status,
                            const QList<TextDataUnit>& _unit = QList<TextDataUnit>(),
                            const QList<OutputTextStatus>& _premiseStatus = QList<OutputTextStatus>());
    static QString outputTextStatusToCommand(OutputTextStatus);
    static QString outputTextStatusToJapanese(OutputTextStatus);
    static bool isInputStatus(OutputTextStatus);
    OutputTextStatus getStatus() const{return myStatus;}
    bool MatchKeyword(OutputTextStatus, const QString&) const;
private:
    static QList<TextDataUnit> combineDataUnit(OutputTextStatus, const QList<TextDataUnit>&);
    const OutputTextStatus myStatus;
    QList<OutputTextStatus> premiseTypes;
};
} // namespace post3dapp

#endif // LINEOUTPUTTEXT_H
