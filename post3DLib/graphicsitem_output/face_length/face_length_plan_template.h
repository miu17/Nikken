#ifndef FACELENGTHPLANTEMPLATE_H
#define FACELENGTHPLANTEMPLATE_H

#include "figure_template_plan.h"

namespace post3dapp{

class OutputGrScene;
class FaceLengthPlanTemplate : public FigureCommonTemplatePlan
{
    Q_OBJECT

public:
    FaceLengthPlanTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>) override;
protected:
    AnalysisCommon2DViewSetting* getViewSettings() const override{return static_cast<AnalysisCommon2DViewSetting*>(setting.getSetting(option.type));}
};

} // namespace post3dapp


#endif // FACELENGTHPLANTEMPLATE_H
