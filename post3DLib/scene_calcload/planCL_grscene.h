#ifndef PLANCL_GRSCENE_H
#define PLANCL_GRSCENE_H

#include "calcload_grscene.h"
#include "calcload_background_floor.h"
#include "define_draw_figure.h"


#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class JointData;
class MemberData;
class UnifiedInputData;
class BaseBackGround;
class CalcLoadGraphicsItem;
class POST3D_CLASS_DLLSPEC PlanCalcLoadGrScene : public CalcLoadGrScene
{
    Q_OBJECT

public:

    PlanCalcLoadGrScene(const CalculationLoad2DViewSettings& setting, const FIGURE_TYPE_PROPERTY&, bool _syncSetting, QObject *parent = nullptr );
    ~PlanCalcLoadGrScene();

    void setStatus(const QUuid & )override;

    CalcLoadBackGroundFloor* getBackGround() const override {return backGround;}
    void SetUseScaledFont(bool useScaledFont) override
    {
        viewSettings.SetUseScaledFont(useScaledFont);
        backGround->SetUseScaledFont(useScaledFont);
    }

private:
    CalcLoadBackGroundFloor* backGround;
    QUuid myFloorID;

    void createJointItems() override;
    void createMemberItems() override;

    CalcLoadGraphicsItem *makeJointItem(JointData *);
    CalcLoadGraphicsItem *makeLineItem(MemberData *, const QList<QPointF> &, bool isValue = false);
};
} // namespace post3dapp
#endif // PLANCL_GRSCENE_H
