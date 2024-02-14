#ifndef PLANOUTGRSCENE_H
#define PLANOUTGRSCENE_H

#include "output_grscene.h"
#include "output_background_floor.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class POST3D_CLASS_DLLSPEC PlanOutputGrScene : public OutputGrScene
{
    Q_OBJECT

public:

    PlanOutputGrScene(const Analysis2DViewSettings& setting, bool _syncSetting, const FIGURE_TYPE_PROPERTY& property = FIGURE_TYPE_PROPERTY{}, QObject *parent = nullptr );
    OutputBackGroundFloor* getBackGround() const override{return backGround;}
    void SetUseScaledFont(bool useScaledFont) override
    {
        viewSettings.SetUseScaledFont(useScaledFont);
        backGround->SetUseScaledFont(useScaledFont);
    }


public slots:


signals:

protected:

private:
    OutputBackGroundFloor* backGround;

};
} // namespace post3dapp
#endif
