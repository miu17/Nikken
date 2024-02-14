#ifndef OUTPUTGRSCENE_H
#define OUTPUTGRSCENE_H

#include <QGraphicsScene>
#include "customdraw_grscene.h"
#include "define_draw_figure.h"
#include "define_analysis2dsettings.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class JointData;
class MemberData;
class SpringData;
class OutputGraphicsItem;

class POST3D_CLASS_DLLSPEC OutputGrScene : public CustomDrawGrScene
{
    Q_OBJECT

public:

    OutputGrScene(const Analysis2DViewSettings& setting, bool _syncSetting, const FIGURE_TYPE_PROPERTY& property = FIGURE_TYPE_PROPERTY{}, QObject *parent = nullptr );
    ~OutputGrScene();

    void setFigureTypeProperty(const QUuid&, const FIGURE_TYPE_PROPERTY&);
    void drawBackGround(QPainter *paniter);


    OUTFIGTYPE getOutFigureType() const
    {
        return figureProperty.type;
    }
    QUuid getFrameFloorId()const{return myFrameFloorID;}
    Analysis2DViewSettings* getAllSettingsPointer() {return &viewSettings;}
    AnalysisBaseViewSettings* getViewSettings() const {
        return dynamic_cast<AnalysisBaseViewSettings*>(viewSettings.getSetting(figureProperty.type));
    }

    FIGURE_TYPE_PROPERTY getFigureTypeProperty() const override {return figureProperty;}

public slots:

    void resetItems() override;
    void updateOutputItems(OUTFIGTYPE);
    void updateViewSettings();

signals:
    void resetSceneView();

protected:

    QUuid myFrameFloorID;
    Analysis2DViewSettings viewSettings;
    FIGURE_TYPE_PROPERTY figureProperty;

    QPointF jointToPoint(JointData *, qreal nx = 0.0) const;

    void clearItems();


};
} // namespace post3dapp
#endif // OUTPUTGRSCENE_H
