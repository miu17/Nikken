#ifndef MAPPLANTEMPLATECOR_H
#define MAPPLANTEMPLATECOR_H


#include "numeric_plan_template.h"
namespace post3dapp{

class OutputGrScene;
class CenterOfRigidityPlanTemplate :public NumericPlanTemplate
{
    Q_OBJECT

public:
    CenterOfRigidityPlanTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createPointItems(OutputGrScene*)override;

    AnalysisCenterOfRigidity2DViewSetting* getViewSettings() const override{return static_cast<AnalysisCenterOfRigidity2DViewSetting*>(setting.getSetting(option.type));}
    void createPointItem(OutputGrScene *scene, QPointF point, QPointF offset, QString str, QColor color, int size);
    void centerOfRigidity(OutputGrScene *scene);
private:

};
} // namespace post3dapp

#endif // MAPPLANTEMPLATECOR_H
