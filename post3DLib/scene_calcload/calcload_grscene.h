#ifndef CALCLOADGRSCENE_H
#define CALCLOADGRSCENE_H

#include "customdraw_grscene.h"
#include "define_calculationload2dsettings.h"
#include "define_draw_figure.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class CalcLoadGraphicsItem;
class JointData;
class MemberData;
class POST3D_CLASS_DLLSPEC CalcLoadGrScene : public CustomDrawGrScene
{
    Q_OBJECT

public:

    CalcLoadGrScene(const CalculationLoad2DViewSettings& setting, const FIGURE_TYPE_PROPERTY&, bool _syncSetting, QObject *parent = nullptr );
    ~CalcLoadGrScene(){};

    virtual void setStatus(const QUuid &uid)=0;
    void drawBackGround(QPainter *painter);

    FIGURE_TYPE_PROPERTY getFigureTypeProperty() const final {return figureProperty;}
    CalculationLoad2DViewSettings getViewSettings(){return viewSettings;}

public slots:
    void resetItems() final;
    void updateFigureTypeProperty(const FIGURE_TYPE_PROPERTY&);
    void updateViewSettings();

signals:
    void resetSceneView();

protected:

    void clearItems();
    virtual void createJointItems() = 0;
    virtual void createMemberItems() = 0;
    CalcLoadGraphicsItem *makePlaneItem(MemberData *, const QList<QPointF> &, bool isNormalDirection = false);

    CalculationLoad2DViewSettings viewSettings;
    FIGURE_TYPE_PROPERTY figureProperty;

    QList<CalcLoadGraphicsItem *> createdItems;
    LoadType::LOADTYPE loadType;
    LOADSUMTYPE loadSumType;
    OUTPUTDIR outputDir;

};
}
#endif // CALCLOADGRSCENE_H
