#ifndef STRESSELEVATIONTEMPLATE_H
#define STRESSELEVATIONTEMPLATE_H

#include "figure_template_elevation.h"
namespace post3dapp
{

    class OutputGrScene;
    class StressElevationTemplate : public FigureCommonTemplateElevation
    {
        Q_OBJECT

    public:
        StressElevationTemplate(const Analysis2DViewSettings &, const FIGURE_TYPE_PROPERTY &, QUuid);
        //    void createJointItem(OutputGrScene *scene,OUTFIG_TYPE_OPTION option,JointData *jd, int ino, const QPointF &pp)override;
        void preCreateItems(OutputGrScene *) override;
        void createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp) override;
        void createLineItem(OutputGrScene *, MemberData *, QList<QPointF>) override;
        void createPlaneItem(OutputGrScene *scene, MemberData *md, QList<QPointF> points, bool isNormalDirection) override;

        QColor valueTextColor(DATATYPE, ATTENTION_STATUS) const final;
        bool figureVisible(DATATYPE) const final;

        qreal diagramLoadPerMeter();
        QColor diagramColor();
        bool wallGirderFigureVisible();

        bool qVisible(){return  getViewSettings()->getValueQVisible();}
        bool mVisible(){return  getViewSettings()->getValueMVisible();};
        bool nVisible(){return  getViewSettings()->getValueNVisible();};

    protected:
        AnalysisStress2DViewSetting *getViewSettings() const override { return static_cast<AnalysisStress2DViewSetting *>(setting.getSetting(option.type)); }

    private:
        QList<MemberData *> lineMembersAroundPlane;
    };
} // namespace post3dapp

#endif // STRESSELEVATIONTEMPLATE_H
