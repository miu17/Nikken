#ifndef FACELENGTHELEVATIONTEMPLATE_H
#define FACELENGTHELEVATIONTEMPLATE_H

#include "figure_template_elevation.h"

namespace post3dapp{

class OutputGrScene;
class FaceLengthElevationTemplate : public FigureCommonTemplateElevation
{
    Q_OBJECT

public:
    FaceLengthElevationTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>) override;
protected:
    AnalysisCommon2DViewSetting* getViewSettings() const override {return static_cast<AnalysisCommon2DViewSetting*>(setting.getSetting(option.type));}
private:
};

} // namespace post3dapp

#endif // FACELENGTHELEVATIONTEMPLATE_H
