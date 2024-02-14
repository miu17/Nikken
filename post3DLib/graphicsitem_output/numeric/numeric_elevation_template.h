#ifndef NUMERICELEVATIONTEMPLATE_H
#define NUMERICELEVATIONTEMPLATE_H

#include "figure_template_elevation.h"

namespace post3dapp{

class OutputGrScene;
class NumericElevationTemplate :public FigureCommonTemplateElevation
{
    Q_OBJECT

public:
    NumericElevationTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void preCreateItems(OutputGrScene*)override;
    void createJointItem(OutputGrScene *scene,JointData *jd, int ino, const QPointF &pp)override;
    void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>)override;
    void createPlaneItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points,bool isNormalDirection )override;
protected:
    virtual AnalysisCommon2DViewSetting* getViewSettings() const override {return static_cast<AnalysisCommon2DViewSetting *>(setting.getSetting(option.type));}
private:
    // TODO このテンプレートはViewSettingによって分けてほしい -> commonSetting系はここ、別のsettingを使っているものはさらに派生して対応
    QList<MemberData *> lineMembersAroundPlane;

};
} // namespace post3dapp

#endif // NUMERICELEVATIONTEMPLATE_H
