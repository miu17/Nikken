#ifndef ELEVATIONCL_GRSCENE_H
#define ELEVATIONCL_GRSCENE_H

#include "calcload_grscene.h"
#include "calcload_background_frame.h"
#include "define_draw_figure.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class JointData;
class MemberData;
class SpringData;
class CalcLoadGraphicsItem;

class POST3D_CLASS_DLLSPEC ElevationCalcLoadGrScene : public CalcLoadGrScene
{
    Q_OBJECT

public:

    ElevationCalcLoadGrScene(const CalculationLoad2DViewSettings& setting, const FIGURE_TYPE_PROPERTY&, bool _syncSetting, QObject *parent = nullptr );
    ~ElevationCalcLoadGrScene();

    void setStatus(const QUuid &uid) override;

    CalcLoadBackGroundFrame* getBackGround() const override{return backGround;}
    void SetUseScaledFont(bool useScaledFont) override
    {
        viewSettings.SetUseScaledFont(useScaledFont);
        backGround->SetUseScaledFont(useScaledFont);
    }

private:
    CalcLoadBackGroundFrame* backGround;
    QUuid myFrameID;

    QPointF jointToPoint(JointData *, qreal nx = 0.0) const;

    void createJointItems() override;
    void createMemberItems() override;

    CalcLoadGraphicsItem *makeJointItem(JointData *, int, const QPointF &);
    CalcLoadGraphicsItem *makeOffJointItem(JointData *, int, const QPointF &);
    CalcLoadGraphicsItem *makeLineItem(MemberData *, const QList<QPointF> &, bool isValue = false);

};
} // namespace post3dapp
#endif // ELEVATIONCL_GRSCENE_H
