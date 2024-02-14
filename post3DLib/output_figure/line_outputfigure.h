#ifndef LINEOUTPUTFIGURE_H
#define LINEOUTPUTFIGURE_H

#include "abstractlinedata.h"
#include "textdataunit.h"
#include "define_draw_figure.h"
#include "define_output.h"
#include "define_unifieddata.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
struct FIGURE_TYPE_PROPERTY;
enum class OutputFigureStatus{
        None,
        Skip,
        OutgroupP,
        OutgroupS,
        Ccs,
        Plan,
        Section,
        Lplan,
        Lsection,
        PlanInput,
        SectionInput,
        SumLoadP,
        SumLoadS,
//        SumLoad3d,
        LcmqP,
        LcmqS,
        JmassP,
        JmassS,
        ModelP,
        ModelS,
        LengthP,
        LengthS,
        FaceLengthP,
        FaceLengthS,
        CoordinateP,
        CoordinateS,
        MphaiP,
        MphaiS,
        MrlP,
        MrlS,
        MclP,
        MclS,
        MblP,
        MblS,
        NodeOuterForceP,
        NodeOuterForceS,
        NodeOuterMomentP,
        NodeOuterMomentS,
        JstrP, // ニーズがあればPI(面内), PO(面外)に変更するかも。応力系は他も同じ
        JstrS,
        FstrP,
        FstrS,
        RstrP,
        RstrS,
        Crez,
        AxialForceMap,
        React,
        Qmap,
        VerticalLoadMap,
        TorsionMomentP,
        TorsionMomentS,
        FloorDeformationMap,
        FloorRigidityMap,
        DeformationP,
        DeformationS,
        DisplacementP,
        DisplacementS,
        ChP,
        ChS,
        AllowableStressP,
        AllowableStressS,
        Gckcp,
        Quds,
        QudsCase,
        QudsFloor,
        QudsGrid,
        MemberStrengthP,
        MemberStrengthS,
        AllowRatioEP,
        AllowRatioES,
        AllowRatioUP,
        AllowRatioUS,
        Dscurve,
        DscurveFrame,
        DscurveFloor,
        DsQThetaCurve,
        DsQThetaCurveFrame,
        DsQThetaCurveFloor,
        DsYieldStepP,
        DsYieldStepS,
        DsYieldRatioP,
        DsYieldRatioS,
        DsPlasticRatioP,
        DsPlasticRatioS,
        Dsstr,
        Mrnk,
        Dssh,
        Dsmu,
        Qucurve,
        QucurveFrame,
        QucurveFloor,
        QuQThetaCurve,
        QuQThetaCurveFrame,
        QuQThetaCurveFloor,
        QuYieldStepP,
        QuYieldStepS,
        QuYieldRatioP,
        QuYieldRatioS,
        QuPlasticRatioP,
        QuPlasticRatioS,
        Qustr,
        Qush,
        Dsqucurve,
        Quneedf,
        Nmint,
        FinalEnd
};

class POST3D_CLASS_DLLSPEC LineOutputFigure : public AbstractLineData
{
public:
    explicit LineOutputFigure(const QString& _key = QString{},
                              OutputFigureStatus _status = OutputFigureStatus::None,
                              const QList<TextDataUnit>& _unit = QList<TextDataUnit>(),
                              const QList<OutputFigureStatus>& _premiseStatus = QList<OutputFigureStatus>())
        :AbstractLineData(_key, _unit), myStatus(_status), premiseTypes(_premiseStatus){}
    explicit LineOutputFigure(OutputFigureStatus _status,
                              const QList<TextDataUnit>& _unit = QList<TextDataUnit>(),
                              const QList<OutputFigureStatus>& _premiseStatus = QList<OutputFigureStatus>());
    OutputFigureStatus getStatus() const{return myStatus;}
    bool MatchKeyword(OutputFigureStatus, const QString&);
    virtual void setOption(const QString&){return;}

    static QString outputFigureStatusToCommand(OutputFigureStatus);
    static BASE_TYPE_PROPERTY::DSQU makeDsQu(OutputFigureStatus);
    static OUTFIGTYPE makeOutFigType(OutputFigureStatus);
protected:
    static QList<TextDataUnit> combineDataUnit(OutputFigureStatus, const QList<TextDataUnit>&);
    const OutputFigureStatus myStatus;
    QList<OutputFigureStatus> premiseTypes;
};

class POST3D_CLASS_DLLSPEC SumLoadOutputFigure : public LineOutputFigure
{
public:
    explicit SumLoadOutputFigure(const QString& _key = QString{},
                                 OutputFigureStatus _status = OutputFigureStatus::None,
                                 const QList<TextDataUnit>& _unit = QList<TextDataUnit>(),
                                 const QList<OutputFigureStatus>& _premiseStatus = QList<OutputFigureStatus>())
        :LineOutputFigure(_key, _status, _unit, _premiseStatus){}
    explicit SumLoadOutputFigure(OutputFigureStatus _status,
                                 const QList<TextDataUnit>& _unit = QList<TextDataUnit>(),
                                 const QList<OutputFigureStatus>& _premiseStatus = QList<OutputFigureStatus>())
        :LineOutputFigure(_status, _unit, _premiseStatus){}
    bool hasFormatError(const QString&) const override;
    void setOption(const QString&) override;
    FIGURE_TYPE_PROPERTY* makeProperty() const;
private:
    LoadType::LOADTYPE loadType;
    LOADSUMTYPE loadSumType;
    OUTPUTDIR outputDir;
};

}// namespace post3dapp

#endif // LINEOUTPUTFIGURE_H
