#ifndef STRESSPLANTEMPLATE_H
#define STRESSPLANTEMPLATE_H

#include "figure_template_plan.h"
namespace post3dapp{

class StressPlanTemplate:public FigureCommonTemplatePlan
{
    Q_OBJECT

public:
    StressPlanTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)override;
    void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>)override;
    void createPlaneItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points,bool isNormalDirection )override;

    QColor valueTextColor(DATATYPE,ATTENTION_STATUS) const final;
    bool figureVisible(DATATYPE) const final;

    qreal diagramLoadPerMeter();
    QColor diagramColor();
    AnalysisStress2DViewSetting* getViewSettings() const override{return static_cast<AnalysisStress2DViewSetting*>(setting.getSetting(option.type));}

private:

};
}

#endif // STRESSPLANTEMPLATE_H
