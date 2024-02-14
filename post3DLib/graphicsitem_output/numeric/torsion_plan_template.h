#ifndef TORSION_PLAN_TEMPLATE_H
#define TORSION_PLAN_TEMPLATE_H
#include "numeric_plan_template.h"
namespace post3dapp{

class TorsionPlanTemplate :public NumericPlanTemplate
{
    Q_OBJECT
public:
    TorsionPlanTemplate(const Analysis2DViewSettings& set, const FIGURE_TYPE_PROPERTY& prop, QUuid uuid)
        :NumericPlanTemplate(set, prop,uuid){}
protected:
//    OutputStress2DViewSetting* getViewSettings() override {return &viewSetting;}
private:

};
} // namespace post3dapp
#endif // TORSION_PLAN_TEMPLATE_H
