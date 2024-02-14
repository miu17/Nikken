#ifndef MAPPLANTEMPLATE_H
#define MAPPLANTEMPLATE_H


#include "figure_template_plan.h"
namespace post3dapp{

class OutputGrScene;
class MapPlanTemplate :public FigureCommonTemplatePlan
{
    Q_OBJECT

public:
    MapPlanTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createLineSectionItem(OutputGrScene *scene, MemberData *jd,  const QPointF &pp)override;
    void createPlaneItem(OutputGrScene*, MemberData *, QList<QPointF>,bool)override;
    void preCreateItems(OutputGrScene*)override;
    void createTextItems(OutputGrScene*)override;

    static QList<QList<qreal>> getTMat_Wall(MemberData *md, QString model);
    QVector<qreal> getGlobalForceTotal(){return globalForceTotal;}
    static QList<qreal> getGlobalForce_Wall(QList<QList<qreal>> TMat, QList<qreal>localForce, MemberData *md);
protected:
    AnalysisCommon2DViewSetting* getViewSettings() const override{return static_cast<AnalysisCommon2DViewSetting*>(setting.getSetting(option.type));}
private:
    QVector<qreal> globalForceTotal;
    QVector<qreal> globalStiffTotal;

};
} // namespace post3dapp

#endif // MAPPLANTEMPLATE_H
