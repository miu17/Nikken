#ifndef DeformationELEVATIONTEMPLATE_H
#define DeformationELEVATIONTEMPLATE_H

#include <QtMath>
#include "figure_template_elevation.h"

namespace post3dapp{

class OutputGrScene;
class DeformationElevationTemplate :public FigureCommonTemplateElevation
{
    Q_OBJECT

public:
    DeformationElevationTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>)override;

    qreal deformScale();
    AnalysisDeformation2DViewSetting* getViewSettings() const override{return static_cast<AnalysisDeformation2DViewSetting*>(setting.getSetting(option.type));}
    virtual QColor figureColor(DATATYPE dataType,STRUCTTYPE material,ATTENTION_STATUS status) const override;
protected:

private:
//    qreal diagramLoadPerMeter;
};
} // namespace post3dapp

#endif // DeformationELEVATIONTEMPLATE_H
