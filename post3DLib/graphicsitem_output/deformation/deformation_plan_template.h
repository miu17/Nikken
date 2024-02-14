#ifndef DeformationPLANTEMPLATE_H
#define DeformationPLANTEMPLATE_H

#include <QtMath>
#include "figure_template_plan.h"

namespace post3dapp{

class OutputGrScene;
class DeformationPlanTemplate :public FigureCommonTemplatePlan
{
    Q_OBJECT

public:
    DeformationPlanTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>)override;
    QColor figureColor(DATATYPE dataType,STRUCTTYPE material,ATTENTION_STATUS status) const final;
protected:
    AnalysisDeformation2DViewSetting* getViewSettings() const override{return static_cast<AnalysisDeformation2DViewSetting*>(setting.getSetting(option.type));}
private:
//    qreal diagramLoadPerMeter;
};
} // namespace post3dapp

#endif // DeformationELEVATIONTEMPLATE_H
