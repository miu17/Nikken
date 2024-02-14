#include "outputfigure_reader.h"

#include "define_draw_figure.h"
#include "define_npstn.h"
#include "fixed_data.h"
#include "unified_analysisdata.h"
#include "unified_inputdata.h"
#include "unified_output.h"
namespace post3dapp{
QList<QSharedPointer<LineOutputFigure>> OutputFigureReader::conditions =
        QList<QSharedPointer<LineOutputFigure>>{
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::OutgroupP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::OutgroupS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Ccs)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Plan)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Section)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Lplan)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Lsection)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::PlanInput)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::SectionInput)),
        QSharedPointer<SumLoadOutputFigure>(new SumLoadOutputFigure(OutputFigureStatus::SumLoadP,
                                                              QList<TextDataUnit>{
                                                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, LineOutputFigure::outputFigureStatusToCommand(OutputFigureStatus::SumLoadP).length() + 1, 10},
                                                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 13, true},
                                                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, 14, 20},
                                                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                                                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, 27, 30},
                                                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 35, true},
                                                              })),
        QSharedPointer<SumLoadOutputFigure>(new SumLoadOutputFigure(OutputFigureStatus::SumLoadS,
                                                              QList<TextDataUnit>{
                                                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, LineOutputFigure::outputFigureStatusToCommand(OutputFigureStatus::SumLoadS).length() + 1, 10},
                                                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 13, true},
                                                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, 14, 20},
                                                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 26, true},
                                                                  TextDataUnit{TEXTTYPE::TYPE_BLANK, 27, 30},
                                                                  TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 35, true},
                                                              })),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::LcmqP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::LcmqS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::JmassP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::JmassS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::ModelP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::ModelS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::LengthP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::LengthS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::FaceLengthP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::FaceLengthS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::CoordinateP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::CoordinateS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::MphaiP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::MphaiS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::MrlP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::MrlS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::MclP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::MclS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::MblP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::MblS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::NodeOuterForceP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::NodeOuterForceS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::NodeOuterMomentP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::NodeOuterMomentS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::JstrP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::JstrS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::FstrP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::FstrS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::RstrP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::RstrS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Crez)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::AxialForceMap)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::React)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Qmap)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::VerticalLoadMap)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::TorsionMomentP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::TorsionMomentS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::FloorDeformationMap)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::FloorRigidityMap)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DeformationP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DeformationS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DisplacementP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DisplacementS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::ChP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::ChS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::AllowableStressP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::AllowableStressS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Gckcp)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Quds)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QudsCase,
                             QList<TextDataUnit>{
                                 TextDataUnit{TEXTTYPE::TYPE_BLANK, LineOutputFigure::outputFigureStatusToCommand(OutputFigureStatus::QudsCase).length() + 1, 10},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true}
                             },
                             QList<OutputFigureStatus>{
                                      OutputFigureStatus::Quds,
                                      OutputFigureStatus::QudsGrid,
                                  })),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QudsFloor,
                             QList<TextDataUnit>{
                                 TextDataUnit{TEXTTYPE::TYPE_BLANK, LineOutputFigure::outputFigureStatusToCommand(OutputFigureStatus::QudsFloor).length() + 1, 10},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, false},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, false},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80, false},
                             },
                             QList<OutputFigureStatus>{
                                      OutputFigureStatus::QudsCase,
                                  })),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QudsGrid,
                             QList<TextDataUnit>{
                                 TextDataUnit{TEXTTYPE::TYPE_BLANK, LineOutputFigure::outputFigureStatusToCommand(OutputFigureStatus::QudsGrid).length() + 1, 10},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, false},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, false},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                                 TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80, false},
                             },
                             QList<OutputFigureStatus>{
                                      OutputFigureStatus::QudsFloor,
                                  })),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::MemberStrengthP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::MemberStrengthS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::AllowRatioEP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::AllowRatioES)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::AllowRatioUP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::AllowRatioUS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Dscurve)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DscurveFrame)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DscurveFloor)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DsQThetaCurve)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DsQThetaCurveFrame)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DsQThetaCurveFloor)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DsYieldStepP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DsYieldStepS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DsYieldRatioP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DsYieldRatioS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DsPlasticRatioP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::DsPlasticRatioS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Dsstr)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Mrnk)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Dssh)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Dsmu)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Qucurve)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QucurveFrame)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QucurveFloor)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QuQThetaCurve)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QuQThetaCurveFrame)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QuQThetaCurveFloor)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QuYieldStepP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QuYieldStepS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QuYieldRatioP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QuYieldRatioS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QuPlasticRatioP)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::QuPlasticRatioS)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Qustr)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Qush)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Dsqucurve)),
        QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::Quneedf)),
        /* Area指定*/
        QSharedPointer<LineOutputFigure>(new LineOutputFigure("-",
                       OutputFigureStatus::Skip,
                       QList<TextDataUnit>{
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 1},
                                TextDataUnit{TEXTTYPE::TYPE_BLANK, 2, 10},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80, false}},
                       QList<OutputFigureStatus>{
                                OutputFigureStatus::OutgroupP,
                                OutputFigureStatus::OutgroupS,
                                OutputFigureStatus::LcmqP,
                                OutputFigureStatus::LcmqS,
                                OutputFigureStatus::ModelP,
                                OutputFigureStatus::ModelS,
                                OutputFigureStatus::FaceLengthP,
                                OutputFigureStatus::FaceLengthS,
                                OutputFigureStatus::MphaiP,
                                OutputFigureStatus::MphaiS,
                                OutputFigureStatus::MrlP,
                                OutputFigureStatus::MrlS,
                                OutputFigureStatus::NodeOuterForceP,
                                OutputFigureStatus::NodeOuterForceS,
                                OutputFigureStatus::NodeOuterMomentP,
                                OutputFigureStatus::NodeOuterMomentS,
                                OutputFigureStatus::JstrP,
                                OutputFigureStatus::JstrS,
                                OutputFigureStatus::FstrP,
                                OutputFigureStatus::FstrS,
                                OutputFigureStatus::RstrP,
                                OutputFigureStatus::RstrS,
                                OutputFigureStatus::React,
                                OutputFigureStatus::AxialForceMap,
                                OutputFigureStatus::Qmap,
                                OutputFigureStatus::VerticalLoadMap,
                                OutputFigureStatus::TorsionMomentP,
                                OutputFigureStatus::TorsionMomentS,
                                OutputFigureStatus::FloorDeformationMap,
                                OutputFigureStatus::FloorRigidityMap,
                                OutputFigureStatus::DeformationP,
                                OutputFigureStatus::DeformationS,
                                OutputFigureStatus::DisplacementP,
                                OutputFigureStatus::DisplacementS,
                                OutputFigureStatus::ChP,
                                OutputFigureStatus::ChS,
                                OutputFigureStatus::AllowableStressP,
                                OutputFigureStatus::AllowableStressS,
                                OutputFigureStatus::QudsFloor,
                                OutputFigureStatus::QudsGrid,
                            })), // OUTGROUP, STRESS, MODEL, CHECK, QudsFloor, QudsGrid
        QSharedPointer<LineOutputFigure>(new LineOutputFigure("",
                       OutputFigureStatus::Skip,
                       QList<TextDataUnit>{
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10, true},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true}, // 名称と指定で20までは必須
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80, false}},
                       QList<OutputFigureStatus>{
                                OutputFigureStatus::OutgroupP,
                                OutputFigureStatus::OutgroupS,
                                OutputFigureStatus::LcmqP,
                                OutputFigureStatus::LcmqS,
                                OutputFigureStatus::ModelP,
                                OutputFigureStatus::ModelS,
                                OutputFigureStatus::FaceLengthP,
                                OutputFigureStatus::FaceLengthS,
                                OutputFigureStatus::MphaiP,
                                OutputFigureStatus::MphaiS,
                                OutputFigureStatus::MrlP,
                                OutputFigureStatus::MrlS,
                                OutputFigureStatus::NodeOuterForceP,
                                OutputFigureStatus::NodeOuterForceS,
                                OutputFigureStatus::NodeOuterMomentP,
                                OutputFigureStatus::NodeOuterMomentS,
                                OutputFigureStatus::JstrP,
                                OutputFigureStatus::JstrS,
                                OutputFigureStatus::FstrP,
                                OutputFigureStatus::FstrS,
                                OutputFigureStatus::RstrP,
                                OutputFigureStatus::RstrS,
                                OutputFigureStatus::React,
                                OutputFigureStatus::AxialForceMap,
                                OutputFigureStatus::Qmap,
                                OutputFigureStatus::VerticalLoadMap,
                                OutputFigureStatus::TorsionMomentP,
                                OutputFigureStatus::TorsionMomentS,
                                OutputFigureStatus::FloorDeformationMap,
                                OutputFigureStatus::FloorRigidityMap,
                                OutputFigureStatus::DeformationP,
                                OutputFigureStatus::DeformationS,
                                OutputFigureStatus::DisplacementP,
                                OutputFigureStatus::DisplacementS,
                                OutputFigureStatus::ChP,
                                OutputFigureStatus::ChS,
                                OutputFigureStatus::AllowableStressP,
                                OutputFigureStatus::AllowableStressS,
                            })), // OUTGROUP, STRESS, MODEL, CHECK
        QSharedPointer<LineOutputFigure>(new LineOutputFigure("",
                       OutputFigureStatus::Skip,
                       QList<TextDataUnit>{
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10, true},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                                TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 21, 30, true},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, false},
                                TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                                TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false}},
                       QList<OutputFigureStatus>{
                                OutputFigureStatus::Ccs
                            })), // CCS,
        QSharedPointer<LineOutputFigure>(new LineOutputFigure("-",
                       OutputFigureStatus::Skip,
                       QList<TextDataUnit>{
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 1, true},
                                TextDataUnit{TEXTTYPE::TYPE_BLANK, 2, 30, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, false},
                                TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 41, 50, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                                TextDataUnit{TEXTTYPE::TYPE_DOUBLE, 61, 70, false}},
                       QList<OutputFigureStatus>{
                                OutputFigureStatus::Ccs
                            })), // CCS,
        QSharedPointer<LineOutputFigure>(new LineOutputFigure("",
                       OutputFigureStatus::Skip,
                       QList<TextDataUnit>{
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10, true},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, true},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, true},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80, false}
                             },
                       QList<OutputFigureStatus>{
                                OutputFigureStatus::Crez
                            })), // CREZ,
        QSharedPointer<LineOutputFigure>(new LineOutputFigure("-",
                       OutputFigureStatus::Skip,
                       QList<TextDataUnit>{
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 1, true},
                                TextDataUnit{TEXTTYPE::TYPE_BLANK, 2, 30, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, true},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80, false}
                             },
                       QList<OutputFigureStatus>{
                                OutputFigureStatus::Crez
                            })), // CREZ,
        QSharedPointer<LineOutputFigure>(new LineOutputFigure("",
                       OutputFigureStatus::Skip,
                       QList<TextDataUnit>{
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 1, 10, true},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 11, 20, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 21, 30, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 31, 40, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 41, 50, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 51, 60, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 61, 70, false},
                                TextDataUnit{TEXTTYPE::TYPE_TEXT, 71, 80, false}},
                       QList<OutputFigureStatus>{
                                OutputFigureStatus::Plan,
                                OutputFigureStatus::Section,
                                OutputFigureStatus::Lplan,
                                OutputFigureStatus::Lsection,
                                OutputFigureStatus::PlanInput,
                                OutputFigureStatus::SectionInput,
                                OutputFigureStatus::SumLoadP,
                                OutputFigureStatus::SumLoadS,
                                OutputFigureStatus::JmassP,
                                OutputFigureStatus::JmassS,
                                OutputFigureStatus::LengthP,
                                OutputFigureStatus::LengthS,
                                OutputFigureStatus::CoordinateP,
                                OutputFigureStatus::CoordinateS,
                                OutputFigureStatus::MclP,
                                OutputFigureStatus::MclS,
                                OutputFigureStatus::MblP,
                                OutputFigureStatus::MblS,
                                OutputFigureStatus::Gckcp,
                                OutputFigureStatus::DscurveFrame,
                                OutputFigureStatus::DscurveFloor,
                                OutputFigureStatus::DsQThetaCurveFrame,
                                OutputFigureStatus::DsQThetaCurveFloor,
                                OutputFigureStatus::QucurveFrame,
                                OutputFigureStatus::QucurveFloor,
                                OutputFigureStatus::QuQThetaCurveFrame,
                                OutputFigureStatus::QuQThetaCurveFloor,
                            })), // NAME, SUMLOAD
                                 // NMINT
};

QSharedPointer<LineOutputFigure> OutputFigureReader::CheckLineCondition(const QString& text, OutputFigureStatus status, QString* errorMessage){
    QSharedPointer<LineOutputFigure> condition = MatchLineCondition(text, status, errorMessage);
    if (*errorMessage != QString{} || condition == nullptr)
        return nullptr;

    if (condition->hasFormatError(text)){
        *errorMessage = u8"「" + text.trimmed() + u8"」のフォーマットが異なります。";
        return nullptr;
    }

    condition->setOption(text);

    return condition;
}

QSharedPointer<LineOutputFigure> OutputFigureReader::MatchLineCondition(const QString &text, OutputFigureStatus status, QString* errorMessage){
    if (text.startsWith("//")){
        *errorMessage = QString{};
        return nullptr;
    }
    if (text.startsWith("*FINALEND")){
        *errorMessage = QString{};
        return QSharedPointer<LineOutputFigure>(new LineOutputFigure(OutputFigureStatus::FinalEnd));
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

bool OutputFigureReader::hasFormatError(const QString& plainText){
    if (plainText == QString())
        return false;

    QStringList list = plainText.split("\n");
    OutputFigureStatus status = OutputFigureStatus::None;
    QString errorMessage;

    Q_FOREACH(auto str, list){
        QSharedPointer<LineOutputFigure> condition = CheckLineCondition(str, status, &errorMessage);
        if (errorMessage != QString{})
            return true;
        if (condition == nullptr)
            continue;

        if (condition->getStatus() != OutputFigureStatus::Skip)
            status = condition->getStatus();

        if (status == OutputFigureStatus::FinalEnd)
            return false;
    }
    return false;
}

QList<BASE_TYPE_PROPERTY *> OutputFigureReader::fastParse(const QStringList &lines, QString *errorMessage) const
{
    return parse(lines, errorMessage, false);
}

QList<BASE_TYPE_PROPERTY *> OutputFigureReader::cliParse(const QStringList &lines, QString *errorMessage, bool hasInput, bool hasInputCopy, bool hasCalc, bool hasOutput) const
{
    return parse(lines, errorMessage, true, hasInput, hasInputCopy, hasCalc, hasOutput);
}

QList<BASE_TYPE_PROPERTY *> OutputFigureReader::appParse(const QStringList &lines, QString *errorMessage) const
{
    return parse(lines, errorMessage, true);
}

QList<BASE_TYPE_PROPERTY*> OutputFigureReader::parse(const QStringList& lines, QString* errorMessage, bool readAll, bool hasInput, bool hasInputCopy, bool hasCalc, bool hasOutput) const
{
    QList<BASE_TYPE_PROPERTY*> properties;
    FIGURE_TYPE_PROPERTY* fproperty;
    FIGURE_TYPE_PROPERTY* sumLoadProperty;
    OutputFigureStatus status = OutputFigureStatus::None;
    QHash<QString, QList<QString>> outGroupP;
    QHash<QString, QList<QString>> outGroupS;
    QHash<QString, QList<STRESS>> combinationStress;
    QString lastDefinition; // "-"で使用する
    QString qudsCase;
    QList<QString> caseList; // hashで順序がわからなくなるので
    QHash<QString, QList<QString>> caseFloor;
    QHash<QString, QList<QString>> caseGrid;
    foreach(auto line, lines){
        QSharedPointer<LineOutputFigure> condition = OutputFigureReader::CheckLineCondition(line, status, errorMessage);
        if (errorMessage != QString{})
            return properties;

        if (condition == nullptr)
            continue;
        if (line.startsWith("*FINALEND"))
            break;

        if (condition->getStatus() != OutputFigureStatus::Skip){
            status = condition->getStatus();
            switch(status){
            // コマンドだけ又はコマンドの右に指定のある場合。
            case OutputFigureStatus::SumLoadP:
            case OutputFigureStatus::SumLoadS:
            {
                auto sCondition = static_cast<SumLoadOutputFigure*>(condition.get());
                sumLoadProperty = sCondition->makeProperty();
            }break;
            case OutputFigureStatus::QudsCase:
                qudsCase = ParseCase(line);
                caseList.append(qudsCase);
                break;
            case OutputFigureStatus::QudsFloor:
                ParseQudsType(line, qudsCase, outGroupP, &caseFloor);
                break;
            case OutputFigureStatus::QudsGrid:
                ParseQudsType(line, qudsCase, outGroupS, &caseGrid);
                break;
            case OutputFigureStatus::Dscurve:
            case OutputFigureStatus::DsQThetaCurve:
            case OutputFigureStatus::Qucurve:
            case OutputFigureStatus::QuQThetaCurve:
                for(auto caseName : caseList){
                    auto gproperty = new GRAPH_TYPE_PROPERTY(status);
                    gproperty->setDsQu(LineOutputFigure::makeDsQu(status));
                    gproperty->setNameList(caseFloor[caseName]);
                    gproperty->caseName = caseName;
                    properties.append(gproperty);
                }
                break;
            case OutputFigureStatus::MemberStrengthP:
            case OutputFigureStatus::AllowRatioEP:
            case OutputFigureStatus::AllowRatioUP:
            case OutputFigureStatus::DsYieldStepP:
            case OutputFigureStatus::DsYieldRatioP:
            case OutputFigureStatus::DsPlasticRatioP:
            case OutputFigureStatus::QuYieldStepP:
            case OutputFigureStatus::QuYieldRatioP:
            case OutputFigureStatus::QuPlasticRatioP:
                for(auto caseName : caseList){
                    for (auto g : caseFloor[caseName]){
                        fproperty = new FIGURE_TYPE_PROPERTY();
                        fproperty->type = LineOutputFigure::makeOutFigType(status);
                        fproperty->scenetype = SCENETYPE::PLANSCENE;
                        fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                        fproperty->caseName = caseName;
                        fproperty->stress; // TODO stressを作りたいもしくはcaseだけで作図してほしい
                        fproperty->name = g;
                        properties.append(fproperty);
                    }
                }
                break;
            case OutputFigureStatus::MemberStrengthS:
            case OutputFigureStatus::AllowRatioES:
            case OutputFigureStatus::AllowRatioUS:
            case OutputFigureStatus::Dsstr:
            case OutputFigureStatus::Qustr:
            case OutputFigureStatus::Mrnk:
            case OutputFigureStatus::Dssh:
            case OutputFigureStatus::Dsmu:
            case OutputFigureStatus::Qush:
            case OutputFigureStatus::DsYieldStepS:
            case OutputFigureStatus::DsYieldRatioS:
            case OutputFigureStatus::DsPlasticRatioS:
            case OutputFigureStatus::QuYieldStepS:
            case OutputFigureStatus::QuYieldRatioS:
            case OutputFigureStatus::QuPlasticRatioS:
                for(auto caseName : caseList){
                    for (auto g : caseGrid[caseName]){
                        fproperty = new FIGURE_TYPE_PROPERTY();
                        fproperty->type = LineOutputFigure::makeOutFigType(status);
                        fproperty->scenetype = SCENETYPE::ELEVATIONSCENE;
                        fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                        fproperty->caseName = caseName;
                        fproperty->stress; // TODO stressを作りたい
                        fproperty->name = g;
                        properties.append(fproperty);
                    }
                }
                break;
            case OutputFigureStatus::Dsqucurve:
            case OutputFigureStatus::Quneedf:
                for(auto caseName : caseList){
                    auto gproperty = new GRAPH_TYPE_PROPERTY(status);
                    gproperty->setNameList(caseFloor[caseName]);
                    gproperty->caseName = caseName;
                    properties.append(gproperty);
                }
                break;
            case OutputFigureStatus::Nmint:
                throw;
            }
        }
        else{
            switch(status){
            case OutputFigureStatus::OutgroupP:
                lastDefinition = MakeGroup(line, lastDefinition, &outGroupP);
                break;
            case OutputFigureStatus::OutgroupS:
                lastDefinition = MakeGroup(line, lastDefinition, &outGroupS);
                break;
            case OutputFigureStatus::Ccs:
                lastDefinition = ParseCcsType(line, lastDefinition, &combinationStress);
                break;
            case OutputFigureStatus::Plan:
            case OutputFigureStatus::Lplan:
            {
                auto nameList = ParseNameType(line, outGroupP);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->viewtype = MODELVIEWTYPE::INPUT2DMODELCOPY;
                    fproperty->scenetype = SCENETYPE::PLANSCENE;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::Section:
            case OutputFigureStatus::Lsection:
            {
                auto nameList = ParseNameType(line, outGroupS);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->viewtype = MODELVIEWTYPE::INPUT2DMODELCOPY;
                    fproperty->scenetype = SCENETYPE::ELEVATIONSCENE;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::PlanInput:
            {
                auto nameList = ParseNameType(line, outGroupP);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->viewtype = MODELVIEWTYPE::INPUT2DMODEL;
                    fproperty->scenetype = SCENETYPE::PLANSCENE;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::SectionInput:
            {
                auto nameList = ParseNameType(line, outGroupS);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->viewtype = MODELVIEWTYPE::INPUT2DMODEL;
                    fproperty->scenetype = SCENETYPE::ELEVATIONSCENE;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::SumLoadP:
            {
                auto nameList = ParseNameType(line, outGroupP);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY(*sumLoadProperty);
                    fproperty->viewtype = MODELVIEWTYPE::CALC2DMODEL;
                    fproperty->scenetype = SCENETYPE::PLANSCENE;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::SumLoadS:
            {
                auto nameList = ParseNameType(line, outGroupS);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY(*sumLoadProperty);
                    fproperty->viewtype = MODELVIEWTYPE::CALC2DMODEL;
                    fproperty->scenetype = SCENETYPE::ELEVATIONSCENE;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::LcmqP:
            case OutputFigureStatus::JstrP:
            case OutputFigureStatus::FstrP:
            case OutputFigureStatus::RstrP:
            {
                // 面外応力図
                auto nameList = ParseHyphenType(line, lastDefinition, outGroupP);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->scenetype = SCENETYPE::PLANSCENE;
                    fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                    fproperty->viewdirection = OUTPUTDIR::ODIR_PLOUT;
                    fproperty->caseName = lastDefinition;
                    fproperty->stress << combinationStress.value(lastDefinition);
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::LcmqS:
            case OutputFigureStatus::NodeOuterForceS:
            case OutputFigureStatus::NodeOuterMomentS:
            case OutputFigureStatus::JstrS:
            case OutputFigureStatus::FstrS:
            case OutputFigureStatus::RstrS:
            case OutputFigureStatus::TorsionMomentS:
            case OutputFigureStatus::DeformationS:
            case OutputFigureStatus::DisplacementS:
            {
                // 面内応力図
                auto nameList = ParseHyphenType(line, lastDefinition, outGroupS);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->scenetype = SCENETYPE::ELEVATIONSCENE;
                    fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                    fproperty->viewdirection = OUTPUTDIR::ODIR_PLIN;
                    fproperty->caseName = lastDefinition;
                    fproperty->stress << combinationStress.value(lastDefinition);
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::JmassP:
            case OutputFigureStatus::LengthP:
            case OutputFigureStatus::CoordinateP:
            case OutputFigureStatus::MclP:
            case OutputFigureStatus::MblP:
            case OutputFigureStatus::Gckcp:
            {
                // Name
                auto nameList = ParseNameType(line, outGroupP);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->scenetype = SCENETYPE::PLANSCENE;
                    fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::JmassS:
            case OutputFigureStatus::LengthS:
            case OutputFigureStatus::CoordinateS:
            case OutputFigureStatus::MclS:
            case OutputFigureStatus::MblS:
            {
                // Name
                auto nameList = ParseNameType(line, outGroupS);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->scenetype = SCENETYPE::ELEVATIONSCENE;
                    fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::NodeOuterForceP:
            case OutputFigureStatus::NodeOuterMomentP:
            case OutputFigureStatus::React:
            case OutputFigureStatus::AxialForceMap:
            case OutputFigureStatus::Qmap:
            case OutputFigureStatus::VerticalLoadMap:
            case OutputFigureStatus::TorsionMomentP:
            case OutputFigureStatus::FloorDeformationMap:
            case OutputFigureStatus::FloorRigidityMap:
            case OutputFigureStatus::DeformationP:
            case OutputFigureStatus::DisplacementP:
            {
                // 面内
                auto nameList = ParseHyphenType(line, lastDefinition, outGroupP);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->scenetype = SCENETYPE::PLANSCENE;
                    fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                    fproperty->caseName = lastDefinition;
                    fproperty->stress << combinationStress.value(lastDefinition);
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::ModelP:
            case OutputFigureStatus::FaceLengthP:
            case OutputFigureStatus::MphaiP:
            case OutputFigureStatus::MrlP:
            {
                auto nameList = ParseHyphenType(line, lastDefinition, outGroupP);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->scenetype = SCENETYPE::PLANSCENE;
                    fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                    fproperty->model = lastDefinition;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::ModelS:
            case OutputFigureStatus::FaceLengthS:
            case OutputFigureStatus::MphaiS:
            case OutputFigureStatus::MrlS:
            {
                auto nameList = ParseHyphenType(line, lastDefinition, outGroupS);
                for (auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->scenetype = SCENETYPE::ELEVATIONSCENE;
                    fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                    fproperty->model = lastDefinition;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::Crez:
                ParseCrezType(line, lastDefinition, combinationStress, outGroupP, fproperty, &properties);
                break;
            case OutputFigureStatus::ChP:
            case OutputFigureStatus::AllowableStressP:
            {
                auto nameList = ParseHyphenType(line, lastDefinition, outGroupP);
                QList<STRESS> stress;
                if (readAll){
                    auto chck = UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();
                    int n = 0;
                    int stressCount = 0;
                    for (int i = 0; i < chck.namech.count(); i++){
                        if (chck.namech.at(i) == lastDefinition){
                            n = i;
                            stressCount = chck.nrsch.at(i);
                            break;
                        }
                    }
                    for (int i = 0; i < stressCount; i++){
                        STRESS st;
                        st.coef = chck.corsch.at(n).at(i);
                        st.rst = chck.nmrsch.at(n).at(i);
                        stress << st;
                    }
                }
                for(auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->scenetype = SCENETYPE::PLANSCENE;
                    fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                    fproperty->stress << stress;
                    fproperty->caseName = lastDefinition;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::ChS:
            case OutputFigureStatus::AllowableStressS:
            {
                auto nameList = ParseHyphenType(line, lastDefinition, outGroupS);
                QList<STRESS> stress;
                if (readAll){
                    auto chck = UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();
                    int n = 0;
                    int stressCount = 0;
                    for (int i = 0; i < chck.namech.count(); i++){
                        if (chck.namech.at(i) == lastDefinition){
                            n = i;
                            stressCount = chck.nrsch.at(i);
                            break;
                        }
                    }
                    for (int i = 0; i < stressCount; i++){
                        STRESS st;
                        st.coef = chck.corsch.at(n).at(i);
                        st.rst = chck.nmrsch.at(n).at(i);
                        stress << st;
                    }
                }
                for(auto name : nameList){
                    fproperty = new FIGURE_TYPE_PROPERTY();
                    fproperty->type = LineOutputFigure::makeOutFigType(status);
                    fproperty->scenetype = SCENETYPE::ELEVATIONSCENE;
                    fproperty->viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
                    fproperty->stress << stress;
                    fproperty->caseName = lastDefinition;
                    fproperty->name = name;
                    properties.append(fproperty);
                }
            }break;
            case OutputFigureStatus::QudsFloor:
                ParseQudsType(line, qudsCase, outGroupP, &caseFloor);
                break;
            case OutputFigureStatus::QudsGrid:
                ParseQudsType(line, qudsCase, outGroupS, &caseGrid);
                break;
            case OutputFigureStatus::DscurveFloor:
            case OutputFigureStatus::DsQThetaCurveFloor:
            case OutputFigureStatus::QucurveFloor:
            case OutputFigureStatus::QuQThetaCurveFloor:
            {
                auto nameList = ParseNameType(line, outGroupP);
                for (auto name : nameList){
                    for(auto caseName : caseList){
                        auto gproperty = new GRAPH_TYPE_PROPERTY(status);
                        gproperty->setNameList(caseGrid[caseName]);
                        gproperty->caseName = caseName;
                        gproperty->setDsQu(LineOutputFigure::makeDsQu(status));
                        gproperty->setBaseName(name);
                        properties.append(gproperty);
                    }
                }
            }break;
            case OutputFigureStatus::DscurveFrame:
            case OutputFigureStatus::DsQThetaCurveFrame:
            case OutputFigureStatus::QucurveFrame:
            case OutputFigureStatus::QuQThetaCurveFrame:
            {
                auto nameList = ParseNameType(line, outGroupS);
                for (auto name : nameList){
                    for(auto caseName : caseList){
                        auto gproperty = new GRAPH_TYPE_PROPERTY(status);
                        gproperty->setNameList(caseFloor[caseName]);
                        gproperty->caseName = caseName;
                        gproperty->setDsQu(LineOutputFigure::makeDsQu(status));
                        gproperty->setBaseName(name);
                        properties.append(gproperty);
                    }
                }
            }break;
            case OutputFigureStatus::Nmint:
            default:
                throw;
            }
        }
    }

    if (readAll)
        *errorMessage = checkProperties(properties, hasInput, hasInputCopy, hasCalc, hasOutput);

    return properties;
}

void OutputFigureReader::ParseQudsType(const QString& line, const QString& caseName, const QHash<QString, QList<QString>>& group, QHash<QString, QList<QString>>* caseHash) const{
    QList<QString> names;
    for (int i = 1; i * 10 < line.length(); i++){
        QString name = line.mid(i * 10, 10).trimmed();
        if (name == QString{}) continue;

        if (group.contains(name)){
            names.append(group[name]);
        }
        else{
            names.append(name);
        }
    }
    if (caseHash->contains(caseName)){
        auto value = caseHash->value(caseName);
        value.append(names);
        caseHash->insert(caseName, value);
    }else{
        caseHash->insert(caseName, names);
    }
}
QString OutputFigureReader::ParseCrezType(
        const QString& line,
        const QString& lastDefinition,
        const QHash<QString, QList<STRESS>>& combinationStress,
        QHash<QString, QList<QString>>& group,
        const FIGURE_TYPE_PROPERTY* property,
        QList<BASE_TYPE_PROPERTY*>* result) const
{
    // combinationStressName = "VL, VL+E1, VL-E1, VL+E2, VL-E2"
    QString combinationStressName = line.left(10).trimmed();
    if (combinationStressName == "-")
        combinationStressName = lastDefinition;
    else{
        combinationStressName += ",";
        combinationStressName += line.mid(10, 10).trimmed();
        combinationStressName += ",";
        combinationStressName += line.mid(20, 10).trimmed();
        combinationStressName += ",";
        combinationStressName += line.mid(30, 10).trimmed();
        combinationStressName += ",";
        combinationStressName += line.mid(40, 10).trimmed();
    }

    auto split = combinationStressName.split(',');

    for(int i = 5; i * 10 < line.length(); i++){
        QString name = line.mid(i * 10, 10).trimmed();
        if (name == QString{}) continue;
        if (group.contains(name)){
            Q_FOREACH(auto value, group[name]){
                FIGURE_TYPE_PROPERTY* copy = new FIGURE_TYPE_PROPERTY(*property);
                copy->stress << combinationStress.value(split[0]);
                copy->stress << combinationStress.value(split[1]);
                copy->stress << combinationStress.value(split[2]);
                copy->stress << combinationStress.value(split[3]);
                copy->stress << combinationStress.value(split[4]);
                copy->name = value;
                result->append(copy);
            }
        }
        else{
            FIGURE_TYPE_PROPERTY* copy = new FIGURE_TYPE_PROPERTY(*property);
            copy->stress << combinationStress.value(split[0]);
            copy->stress << combinationStress.value(split[1]);
            copy->stress << combinationStress.value(split[2]);
            copy->stress << combinationStress.value(split[3]);
            copy->stress << combinationStress.value(split[4]);
            copy->name = name;
            result->append(copy);
        }
    }

    return combinationStressName;
}

QString OutputFigureReader::checkProperties(const QList<BASE_TYPE_PROPERTY *> &properties, bool hasInput, bool hasInputCopy, bool hasCalc, bool hasOutput) const
{
    if (properties.count() == 0){
        return u8"描画項目が1つもありません。";
    }

    QSet<QString> inputFloorSet;
    QSet<QString> inputFrameSet;
    QSet<QString> copyFloorSet;
    QSet<QString> copyFrameSet;
    QSet<QString> outputFloorSet;
    QSet<QString> outputFrameSet;
    QSet<QString> modelSet;
    QSet<QString> stressSet;
    QSet<QString> rstSet;
    for (auto property : properties){
        if (property->viewtype == MODELVIEWTYPE::INPUT2DMODEL){
            auto type = static_cast<FIGURE_TYPE_PROPERTY*>(property);
            if (type->scenetype == SCENETYPE::PLANSCENE){
                if (type->name != QString{})
                    inputFloorSet.insert(type->name);
            }else if(type->scenetype == SCENETYPE::ELEVATIONSCENE){
                if (type->name != QString{})
                    inputFrameSet.insert(type->name);
            }
        }else if (property->viewtype == MODELVIEWTYPE::INPUT2DMODELCOPY){
            auto type = static_cast<FIGURE_TYPE_PROPERTY*>(property);
            if (type->scenetype == SCENETYPE::PLANSCENE){
                if (type->name != QString{})
                    copyFloorSet.insert(type->name);
            }else if(type->scenetype == SCENETYPE::ELEVATIONSCENE){
                if (type->name != QString{})
                    copyFrameSet.insert(type->name);
            }
        }else if (property->viewtype == MODELVIEWTYPE::CALC2DMODEL){
            auto type = static_cast<FIGURE_TYPE_PROPERTY*>(property);
            if (type->scenetype == SCENETYPE::PLANSCENE){
                if (type->name != QString{})
                    copyFloorSet.insert(type->name);
            }else if(type->scenetype == SCENETYPE::ELEVATIONSCENE){
                if (type->name != QString{})
                    copyFrameSet.insert(type->name);
            }
        }else if (property->viewtype == MODELVIEWTYPE::OUTPUT2DMODEL){
            auto type = static_cast<FIGURE_TYPE_PROPERTY*>(property);
            if (type->scenetype == SCENETYPE::PLANSCENE){
                if (type->name != QString{})
                    outputFloorSet.insert(type->name);
            }else if(type->scenetype == SCENETYPE::ELEVATIONSCENE){
                if (type->name != QString{})
                    outputFrameSet.insert(type->name);
            }

            if (type->model != QString{}){
                modelSet.insert(type->model);
            }

            if (type->stress.count() != 0){
                for(auto stress : type->stress){
                    for (auto s : stress){
                        stressSet.insert(s.rst);
                    }
                }
            }

        }else if (property->viewtype == MODELVIEWTYPE::OUTPUTGRAPH){
            auto type = static_cast<GRAPH_TYPE_PROPERTY*>(property);

        }
    }
    if (hasInput){
        // 階に含まれない
        auto floorValues = UnifiedInputData::getInInstance()->getFloorList();
        QStringList floorList;
        for (auto v : floorValues){
            floorList.append(v.floorName);
        }
        for (auto floor : inputFloorSet){
            if (!floorList.contains(floor))
                return floor + u8"は形状データ(入力中画面)の階名称ではない。";
        }

        // 通りに含まれない
        auto frameValues = UnifiedInputData::getInInstance()->getFramePointList();
        QStringList frameList;
        for (auto v : frameValues){
            frameList.append(v.frameName);
        }
        for (auto frame : inputFrameSet){
            if (!frameList.contains(frame))
                return frame + u8"は形状データ(入力中画面)の通り名称ではない。";
        }
    }

    if (hasInputCopy || hasCalc){
        // 階に含まれない
        auto floorValues = UnifiedFixedData::getInstance()->getInputData()->getFloorList();
        QStringList floorList;
        for (auto v : floorValues){
            floorList.append(v.floorName);
        }
        for (auto floor : copyFloorSet){
            if (!floorList.contains(floor))
                return floor + u8"は形状データの階名称ではない。";
        }

        // 通りに含まれない
        auto frameValues = UnifiedFixedData::getInstance()->getInputData()->getFramePointList();
        QStringList frameList;
        for (auto v : frameValues){
            frameList.append(v.frameName);
        }
        for (auto frame : copyFrameSet){
            if (!frameList.contains(frame))
                return frame + u8"は形状データの通り名称ではない。";
        }
    }

    if (hasOutput){
        // 階に含まれない
        auto floorValues = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList();
        QStringList floorList;
        for (auto v : floorValues){
            floorList.append(v.floorName);
        }
        for (auto floor : outputFloorSet){
            if (!floorList.contains(floor))
                return floor + u8"は解析データの階名称ではない。";
        }

        // 通りに含まれない
        auto frameValues = UnifiedFixedData::getInstance()->getAnalysisData()->getFramePointList();
        QStringList frameList;
        for (auto v : frameValues){
            frameList.append(v.frameName);
        }
        for (auto frame : outputFrameSet){
            if (!frameList.contains(frame))
                return frame + u8"は解析データの通り名称ではない。";
        }

        auto modelList = UnifiedFixedData::getInstance()->getPstnData()->getModelCaseList();
        for (auto model : modelSet){
            if (!modelList.contains(model))
                return model + u8"は解析データのモデル名称ではない。";
        }

        auto stressList = UnifiedFixedData::getInstance()->getPstnData()->getStressCaseList();
        for (auto stress : stressSet){
            if (!stressList.contains(stress))
                return stress + u8"は解析データの荷重名称ではない。";
        }

    }


    // RSTに含まれない


    return QString{};
}
} // namespace post3dapp
