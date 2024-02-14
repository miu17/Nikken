#ifndef NUMERICPLANTEMPLATE_H
#define NUMERICPLANTEMPLATE_H

#include "figure_template_plan.h"

namespace post3dapp{

class OutputGrScene;
class NumericPlanTemplate :public FigureCommonTemplatePlan
{
    Q_OBJECT

public:
    NumericPlanTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)override;
    void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>)override;

protected:
    virtual AnalysisCommon2DViewSetting* getViewSettings() const override {return static_cast<AnalysisCommon2DViewSetting*>(setting.getSetting(option.type));}
private:
    // TODO このテンプレートはViewSettingによって分けてほしい -> commonSetting系はここ、別のsettingを使っているものはさらに派生して対応
};
} // namespace post3dapp

#endif // NUMERICPLANTEMPLATE_H
