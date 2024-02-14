#ifndef MODEL_ELEVATION_TEMPLATE_H
#define MODEL_ELEVATION_TEMPLATE_H

#include "figure_model_template.h"

namespace post3dapp{

class OutputGrScene;
class ModelElevationTemplate :public FigureModelTemplate
{
    Q_OBJECT

public:
    ModelElevationTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)override;
    void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>)override;
    void createPlaneItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points,bool isNormalDirection )override;
    void createSpringItem(OutputGrScene*, SpringData *, QList<QPointF>)override;

    QColor nameColor(DATATYPE, STRUCTTYPE material, ATTENTION_STATUS) const;
    QColor zwallColor() const;
    QColor rigidColor() const;
    QColor panelColor() const;
    QColor boundaryTextColor() const;
    int boundaryTextSize() const;
    bool boundaryTextVisible() const;

protected:
    void createJointItems(OutputGrScene*)final;
    void createMemberItems(OutputGrScene*)final;

private:
};
} // namespace post3dapp

#endif // MODEL_ELEVATION_TEMPLATE_H
