#ifndef CHECK_PLAN_TEMPLATE_H
#define CHECK_PLAN_TEMPLATE_H
#include "numeric_plan_template.h"
namespace post3dapp{

class CheckPlanTemplate :public NumericPlanTemplate
{
    Q_OBJECT
public:
    CheckPlanTemplate(const Analysis2DViewSettings& set, const FIGURE_TYPE_PROPERTY& prop, QUuid uuid)
        :NumericPlanTemplate(set, prop,uuid)
    {
    }
    bool useGradation() const {return getViewSettings()->getUseGradiationColor();}
    QColor okColor() const {return getViewSettings()->getValueColor();}
    QColor ngColor() const {return getViewSettings()->getNgColor();}
    AnalysisCheck2DViewSetting* getViewSettings() const override {return static_cast<AnalysisCheck2DViewSetting *>(setting.getSetting(option.type));}
private:

};
} // namespace post3dapp
#endif // CHECK_PLAN_TEMPLATE_H
