#ifndef VERTICALLOADMAPPLANTEMPLATE_H
#define VERTICALLOADMAPPLANTEMPLATE_H
#include "figure_template_plan.h"

namespace post3dapp{

class VerticalLoadMapPlanTemplate :public FigureCommonTemplatePlan
{
    Q_OBJECT
public:
    VerticalLoadMapPlanTemplate(const Analysis2DViewSettings& setting, const FIGURE_TYPE_PROPERTY& prop, QUuid uuid);
    //        :NumericPlanTemplate(prop,uuid,use_scaledFont,set){}
    void createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)override;
    QVector<QVector<qreal>> getZload()const{return zload;}
protected:
    virtual void preCreateItems(OutputGrScene*)override;
    AnalysisCommon2DViewSetting* getViewSettings() const override {return static_cast<AnalysisCommon2DViewSetting*>(setting.getSetting(option.type));}
private:
    VerticalLoadMapPlanTemplate* myFigureTemplate;
    QVector<QVector<qreal>>zload;

};
} // namespace post3dapp
#endif // VERTICALLOADMAPPLANTEMPLATE_H
