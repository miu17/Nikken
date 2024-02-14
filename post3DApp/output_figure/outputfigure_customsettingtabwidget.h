#ifndef OUTPUTFIGURE_CUSTOMSETTINGTABWIDGET_H
#define OUTPUTFIGURE_CUSTOMSETTINGTABWIDGET_H

#include <QTabWidget>

namespace post3dapp{
class AppVersion;
class OutputFigureAxialForce2DWidget;
class OutputFigureCenterOfRigidity2DWidget;
class OutputFigureCheck2DWidget;
class OutputFigureCommon2DWidget;
class OutputFigureDeformation2DWidget;
class OutputFigureGraphHorizontalCapacity2dWidget;
class OutputFigureGraphNmint2DWidget;
class OutputFigureGraphQDelta2DWidget;
class OutputFigureModel2DWidget;
class OutputFigureReactionForce2DWidget;
class OutputFigureShearCheckU2DWidget;
class OutputFigureStress2DWidget;


class OutputFigureCustomSettingTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    OutputFigureCustomSettingTabWidget(QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

    void saveSettings(QSettings&);
    void readSettings(QSettings&);

public slots:
    void initializeSettings();

private:
    OutputFigureModel2DWidget* modelWidget;
    OutputFigureCommon2DWidget* memberLengthWidget;
    OutputFigureCommon2DWidget* faceLengthWidget;
    OutputFigureCommon2DWidget* nodeCoordinateWidget;
    OutputFigureCommon2DWidget* rigidLengthWidget;
    OutputFigureCommon2DWidget* sideBucklingLengthWidget;
    OutputFigureCommon2DWidget* compressionBucklingLengthWidget;
    OutputFigureCommon2DWidget* nodeWeightWidget;
    OutputFigureCommon2DWidget* nodeOuterForceWidget;
    OutputFigureCommon2DWidget* nodeOuterMomentWidget;
    OutputFigureCommon2DWidget* rigidityRatioWidget;
    OutputFigureStress2DWidget* cmqWidget;
    OutputFigureCommon2DWidget* axialForceMapWidget;
    OutputFigureReactionForce2DWidget* reactionForceWidget;
    OutputFigureReactionForce2DWidget* combinatedReactionForceWidget;
    OutputFigureCommon2DWidget* shearForceMapWidget;
    OutputFigureCommon2DWidget* verticalLoadMapWidget;
    OutputFigureCommon2DWidget* floorDeformationMapWidget;
    OutputFigureCommon2DWidget* floorRigidityMapWidget;
    OutputFigureStress2DWidget* nodeStressWidget;
    OutputFigureStress2DWidget* faceStressWidget;
    OutputFigureAxialForce2DWidget* axialForceWidget;
    OutputFigureStress2DWidget* torisonMomentWidget;
    OutputFigureDeformation2DWidget* deformationWidget;
    OutputFigureCommon2DWidget* displacementWidget;
    OutputFigureStress2DWidget* allowableStressWidget;
    OutputFigureCheck2DWidget* checkWidget;
    OutputFigureCheck2DWidget* shearCheckEWidget;
    OutputFigureShearCheckU2DWidget* shearCheckUWidget;
    OutputFigureCommon2DWidget* plasticRateWidget;
    OutputFigureStress2DWidget* rankWidget;
    OutputFigureCommon2DWidget* yieldStepWidget;
    OutputFigureCommon2DWidget* yieldLoadRateWidget;
    OutputFigureCenterOfRigidity2DWidget* centerOfRigidityWidget;
    OutputFigureGraphQDelta2DWidget* graphQDeltaWidget;
    OutputFigureGraphQDelta2DWidget* graphQDeltaFrameWidget;
    OutputFigureGraphQDelta2DWidget* graphQDeltaFloorWidget;
    OutputFigureGraphQDelta2DWidget* graphQThetaWidget;
    OutputFigureGraphQDelta2DWidget* graphQThetaFrameWidget;
    OutputFigureGraphQDelta2DWidget* graphQThetaFloorWidget;
    OutputFigureGraphHorizontalCapacity2dWidget* graphHorizontalCapacityWidget;
    OutputFigureGraphNmint2DWidget* graphNmintWidget;
};
} // namespace post3dapp

#endif // OUTPUTFIGURE_CUSTOMSETTINGTABWIDGET_H
