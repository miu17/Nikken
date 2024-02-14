#ifndef MODEL_PLAN_TEMPLATE_H
#define MODEL_PLAN_TEMPLATE_H

#include "figure_model_template.h"

namespace post3dapp{

class OutputGrScene;
class ModelPlanTemplate :public FigureModelTemplate
{
    Q_OBJECT

public:
    ModelPlanTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)override;
    void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>)override;
    void createPlaneItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points,bool isNormalDirection )override;
    void createSpringItem(OutputGrScene*, SpringData *, QList<QPointF>)override;

    QColor nameColor(DATATYPE, STRUCTTYPE material, ATTENTION_STATUS);
    QColor zwallColor();
    QColor rigidColor();
    QColor panelColor();
    QColor boundaryTextColor();
    int boundaryTextSize();
    bool boundaryTextVisible();

protected:
    void createJointItems(OutputGrScene*)final;
    void createMemberItems(OutputGrScene*)final;

private:
};
} // namespace post3dapp

#endif // MODEL_PLAN_TEMPLATE_H
