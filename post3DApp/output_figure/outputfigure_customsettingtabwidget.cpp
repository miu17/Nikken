#include "outputfigure_customsettingtabwidget.h"

#include "app_version.h"
#include "axialforce2dviewsettingwidget.h"
#include "centerofrigidity2dviewsettingwidget.h"
#include "check2dviewsettingwidget.h"
#include "common2dviewsettingwidget.h"
#include "deformation2dviewsettingwidget.h"
#include "graphnmint2dviewsettingwidget.h"
#include "graphqdelta2dviewsettingwidget.h"
#include "horizontalcapacity2dviewsettingwidget.h"
#include "model2dviewsettingwidget.h"
#include "reactionforce2dviewsettingwidget.h"
#include "shearchecku2dviewSettingwidget.h"
#include "stress2dviewsettingwidget.h"

namespace post3dapp{

OutputFigureCustomSettingTabWidget::OutputFigureCustomSettingTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    modelWidget = new OutputFigureModel2DWidget(u8"モデル図", "model");
    this->addTab(modelWidget, modelWidget->TabName);
    memberLengthWidget = new OutputFigureCommon2DWidget(u8"材長図", "memberLength");
    this->addTab(memberLengthWidget, memberLengthWidget->TabName);
    faceLengthWidget = new OutputFigureCommon2DWidget(u8"フェイス長図", "faceLength");
    this->addTab(memberLengthWidget, memberLengthWidget->TabName);
    nodeCoordinateWidget = new OutputFigureCommon2DWidget(u8"座標位置図", "coordinate");
    this->addTab(nodeCoordinateWidget, nodeCoordinateWidget->TabName);
    rigidLengthWidget = new OutputFigureCommon2DWidget(u8"剛域長図", "rigidLength");
    this->addTab(rigidLengthWidget, rigidLengthWidget->TabName);
    sideBucklingLengthWidget = new OutputFigureCommon2DWidget(u8"横座屈長さ図", "sideBucklingLength");
    this->addTab(sideBucklingLengthWidget, sideBucklingLengthWidget->TabName);
    compressionBucklingLengthWidget = new OutputFigureCommon2DWidget(u8"圧縮座屈長さ図", "compressionBucklingLength");
    this->addTab(compressionBucklingLengthWidget, compressionBucklingLengthWidget->TabName);
    nodeWeightWidget = new OutputFigureCommon2DWidget(u8"節点重量図", "nodeWeight");
    this->addTab(nodeWeightWidget, nodeWeightWidget->TabName);
    nodeOuterForceWidget = new OutputFigureCommon2DWidget(u8"節点外力図", "nodeOuterForce");
    this->addTab(nodeOuterForceWidget, nodeOuterForceWidget->TabName);
    nodeOuterMomentWidget = new OutputFigureCommon2DWidget(u8"節点外力M図", "nodeOuterMoment");
    this->addTab(nodeOuterMomentWidget, nodeOuterMomentWidget->TabName);
    rigidityRatioWidget = new OutputFigureCommon2DWidget(u8"剛比図", "rigidityRatio");
    this->addTab(rigidityRatioWidget, rigidityRatioWidget->TabName);
    cmqWidget = new OutputFigureStress2DWidget(u8"CMQ図", "CMQ");
    this->addTab(cmqWidget, cmqWidget->TabName);
    axialForceMapWidget = new OutputFigureCommon2DWidget(u8"軸力マップ", "axialForceMap");
    this->addTab(axialForceMapWidget, axialForceMapWidget->TabName);
    reactionForceWidget = new OutputFigureReactionForce2DWidget(u8"反力図", "reactionForce");
    this->addTab(reactionForceWidget, reactionForceWidget->TabName);
    combinatedReactionForceWidget = new OutputFigureReactionForce2DWidget(u8"組み合わせ反力マップ", "combinatedReactionForce");
    this->addTab(combinatedReactionForceWidget, combinatedReactionForceWidget->TabName);
    shearForceMapWidget = new OutputFigureCommon2DWidget(u8"せん断力マップ", "shearForceMap");
    this->addTab(shearForceMapWidget, shearForceMapWidget->TabName);
    verticalLoadMapWidget = new OutputFigureCommon2DWidget(u8"鉛直荷重マップ", "verticalLoadMap");
    this->addTab(verticalLoadMapWidget, verticalLoadMapWidget->TabName);
    floorDeformationMapWidget = new OutputFigureCommon2DWidget(u8"層間変形図", "floorDeformationMap");
    this->addTab(floorDeformationMapWidget, floorDeformationMapWidget->TabName);
    floorRigidityMapWidget = new OutputFigureCommon2DWidget(u8"層剛性マップ", "floorRigidityMap");
    this->addTab(floorRigidityMapWidget, floorRigidityMapWidget->TabName);
    nodeStressWidget = new OutputFigureStress2DWidget(u8"節点応力図", "nodeStress");
    this->addTab(nodeStressWidget, nodeStressWidget->TabName);
    faceStressWidget = new OutputFigureStress2DWidget(u8"フェイス応力図", "faceStress");
    this->addTab(faceStressWidget, faceStressWidget->TabName);
    axialForceWidget = new OutputFigureAxialForce2DWidget(u8"軸力図", "axialForce");
    this->addTab(axialForceWidget, axialForceMapWidget->TabName);
    torisonMomentWidget = new OutputFigureStress2DWidget(u8"ねじれM図", "torisonMoment");
    this->addTab(torisonMomentWidget, torisonMomentWidget->TabName);
    deformationWidget = new OutputFigureDeformation2DWidget(u8"変形図", "deformation");
    this->addTab(deformationWidget, deformationWidget->TabName);
    displacementWidget = new OutputFigureCommon2DWidget(u8"変位図", "displacement");
    this->addTab(displacementWidget, displacementWidget->TabName);
    allowableStressWidget = new OutputFigureStress2DWidget(u8"許容力図", "allowableStress");
    this->addTab(allowableStressWidget, allowableStressWidget->TabName);
    checkWidget = new OutputFigureCheck2DWidget(u8"断面検討余裕率図", "check");
    this->addTab(checkWidget, checkWidget->TabName);
    shearCheckEWidget = new OutputFigureCheck2DWidget(u8"せん断余裕率図E", "shearCheckE");
    this->addTab(shearCheckEWidget, shearCheckEWidget->TabName);
    shearCheckUWidget = new OutputFigureShearCheckU2DWidget(u8"せん断余裕率図U", "shearCheckU");
    this->addTab(shearCheckUWidget, shearCheckUWidget->TabName);
    plasticRateWidget = new OutputFigureCommon2DWidget(u8"塑性率図", "plasticRate");
    this->addTab(plasticRateWidget, plasticRateWidget->TabName);
    rankWidget = new OutputFigureStress2DWidget(u8"ランク図", "rank");
    this->addTab(rankWidget, rankWidget->TabName);
    yieldStepWidget = new OutputFigureCommon2DWidget(u8"降伏ステップ図", "yieldStep");
    this->addTab(yieldStepWidget, yieldStepWidget->TabName);
    yieldLoadRateWidget = new OutputFigureCommon2DWidget(u8"降伏荷重比図", "yieldLoadRate");
    this->addTab(yieldLoadRateWidget, yieldLoadRateWidget->TabName);
    centerOfRigidityWidget = new OutputFigureCenterOfRigidity2DWidget(u8"層せん断力中心・剛心伏図", "centerOfRigidity");
    this->addTab(centerOfRigidityWidget, centerOfRigidityWidget->TabName);
    graphQDeltaWidget = new OutputFigureGraphQDelta2DWidget(u8"Q-δ図", "QDelta");
    this->addTab(graphQDeltaWidget, graphQDeltaWidget->TabName);
    graphQDeltaFrameWidget = new OutputFigureGraphQDelta2DWidget(u8"Q-δ図(架構)", "QDeltaFrame");
    this->addTab(graphQDeltaFrameWidget, graphQDeltaFrameWidget->TabName);
    graphQDeltaFloorWidget = new OutputFigureGraphQDelta2DWidget(u8"Q-δ図(階)", "QDeltaFloor");
    this->addTab(graphQDeltaFloorWidget, graphQDeltaFloorWidget->TabName);
    graphQThetaWidget = new OutputFigureGraphQDelta2DWidget(u8"Q-Θ図", "QTheta");
    this->addTab(graphQThetaWidget, graphQThetaWidget->TabName);
    graphQThetaFrameWidget = new OutputFigureGraphQDelta2DWidget(u8"Q-Θ図(架構)", "QThetaFrame");
    this->addTab(graphQThetaFrameWidget, graphQThetaFrameWidget->TabName);
    graphQThetaFloorWidget = new OutputFigureGraphQDelta2DWidget(u8"Q-Θ図(階)", "QThetaFloor");
    this->addTab(graphQThetaFloorWidget, graphQThetaFloorWidget->TabName);
    graphHorizontalCapacityWidget = new OutputFigureGraphHorizontalCapacity2dWidget(u8"保有水平耐力図", "horizontalCapacity");
    this->addTab(graphHorizontalCapacityWidget, graphHorizontalCapacityWidget->TabName);
    graphNmintWidget = new OutputFigureGraphNmint2DWidget(u8"NMINT", "NMINT");
    this->addTab(graphNmintWidget, graphNmintWidget->TabName);
}

void OutputFigureCustomSettingTabWidget::writeBinaryFile(QDataStream &outfile) const{
    modelWidget->writeBinaryFile(outfile);
    memberLengthWidget->writeBinaryFile(outfile);
    faceLengthWidget->writeBinaryFile(outfile);
    nodeCoordinateWidget->writeBinaryFile(outfile);
    rigidLengthWidget->writeBinaryFile(outfile);
    sideBucklingLengthWidget->writeBinaryFile(outfile);
    compressionBucklingLengthWidget->writeBinaryFile(outfile);
    nodeWeightWidget->writeBinaryFile(outfile);
    nodeOuterForceWidget->writeBinaryFile(outfile);
    nodeOuterMomentWidget->writeBinaryFile(outfile);
    rigidityRatioWidget->writeBinaryFile(outfile);
    cmqWidget->writeBinaryFile(outfile);
    axialForceMapWidget->writeBinaryFile(outfile);
    reactionForceWidget->writeBinaryFile(outfile);
    combinatedReactionForceWidget->writeBinaryFile(outfile);
    shearForceMapWidget->writeBinaryFile(outfile);
    verticalLoadMapWidget->writeBinaryFile(outfile);
    floorDeformationMapWidget->writeBinaryFile(outfile);
    floorRigidityMapWidget->writeBinaryFile(outfile);
    nodeStressWidget->writeBinaryFile(outfile);
    faceStressWidget->writeBinaryFile(outfile);
    axialForceWidget->writeBinaryFile(outfile);
    torisonMomentWidget->writeBinaryFile(outfile);
    deformationWidget->writeBinaryFile(outfile);
    displacementWidget->writeBinaryFile(outfile);
    allowableStressWidget->writeBinaryFile(outfile);
    checkWidget->writeBinaryFile(outfile);
    shearCheckEWidget->writeBinaryFile(outfile);
    shearCheckUWidget->writeBinaryFile(outfile);
    plasticRateWidget->writeBinaryFile(outfile);
    rankWidget->writeBinaryFile(outfile);
    yieldStepWidget->writeBinaryFile(outfile);
    yieldLoadRateWidget->writeBinaryFile(outfile);
    centerOfRigidityWidget->writeBinaryFile(outfile);
    graphQDeltaWidget->writeBinaryFile(outfile);
    graphQDeltaFrameWidget->writeBinaryFile(outfile);
    graphQDeltaFloorWidget->writeBinaryFile(outfile);
    graphQThetaWidget->writeBinaryFile(outfile);
    graphQThetaFrameWidget->writeBinaryFile(outfile);
    graphQThetaFloorWidget->writeBinaryFile(outfile);
    graphHorizontalCapacityWidget->writeBinaryFile(outfile);
    graphNmintWidget->writeBinaryFile(outfile);
}
void OutputFigureCustomSettingTabWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    modelWidget->readBinaryFile(infile, version);
    memberLengthWidget->readBinaryFile(infile, version);
    faceLengthWidget->readBinaryFile(infile, version);
    nodeCoordinateWidget->readBinaryFile(infile, version);
    rigidLengthWidget->readBinaryFile(infile, version);
    sideBucklingLengthWidget->readBinaryFile(infile, version);
    compressionBucklingLengthWidget->readBinaryFile(infile, version);
    nodeWeightWidget->readBinaryFile(infile, version);
    nodeOuterForceWidget->readBinaryFile(infile, version);
    nodeOuterMomentWidget->readBinaryFile(infile, version);
    rigidityRatioWidget->readBinaryFile(infile, version);
    cmqWidget->readBinaryFile(infile, version);
    axialForceMapWidget->readBinaryFile(infile, version);
    reactionForceWidget->readBinaryFile(infile, version);
    combinatedReactionForceWidget->readBinaryFile(infile, version);
    shearForceMapWidget->readBinaryFile(infile, version);
    verticalLoadMapWidget->readBinaryFile(infile, version);
    floorDeformationMapWidget->readBinaryFile(infile, version);
    floorRigidityMapWidget->readBinaryFile(infile, version);
    nodeStressWidget->readBinaryFile(infile, version);
    faceStressWidget->readBinaryFile(infile, version);
    axialForceWidget->readBinaryFile(infile, version);
    torisonMomentWidget->readBinaryFile(infile, version);
    deformationWidget->readBinaryFile(infile, version);
    displacementWidget->readBinaryFile(infile, version);
    allowableStressWidget->readBinaryFile(infile, version);
    checkWidget->readBinaryFile(infile, version);
    shearCheckEWidget->readBinaryFile(infile, version);
    shearCheckUWidget->readBinaryFile(infile, version);
    plasticRateWidget->readBinaryFile(infile, version);
    rankWidget->readBinaryFile(infile, version);
    yieldStepWidget->readBinaryFile(infile, version);
    yieldLoadRateWidget->readBinaryFile(infile, version);
    centerOfRigidityWidget->readBinaryFile(infile, version);
    graphQDeltaWidget->readBinaryFile(infile, version);
    graphQDeltaFrameWidget->readBinaryFile(infile, version);
    graphQDeltaFloorWidget->readBinaryFile(infile, version);
    graphQThetaWidget->readBinaryFile(infile, version);
    graphQThetaFrameWidget->readBinaryFile(infile, version);
    graphQThetaFloorWidget->readBinaryFile(infile, version);
    graphHorizontalCapacityWidget->readBinaryFile(infile, version);
    graphNmintWidget->readBinaryFile(infile, version);

}

void OutputFigureCustomSettingTabWidget::initializeSettings(){
    modelWidget->initializeSettings();
    memberLengthWidget->initializeSettings();
    faceLengthWidget->initializeSettings();
    nodeCoordinateWidget->initializeSettings();
    rigidLengthWidget->initializeSettings();
    sideBucklingLengthWidget->initializeSettings();
    compressionBucklingLengthWidget->initializeSettings();
    nodeWeightWidget->initializeSettings();
    nodeOuterForceWidget->initializeSettings();
    nodeOuterMomentWidget->initializeSettings();
    rigidityRatioWidget->initializeSettings();
    cmqWidget->initializeSettings();
    axialForceMapWidget->initializeSettings();
    reactionForceWidget->initializeSettings();
    combinatedReactionForceWidget->initializeSettings();
    shearForceMapWidget->initializeSettings();
    verticalLoadMapWidget->initializeSettings();
    floorDeformationMapWidget->initializeSettings();
    floorRigidityMapWidget->initializeSettings();
    nodeStressWidget->initializeSettings();
    faceStressWidget->initializeSettings();
    axialForceWidget->initializeSettings();
    torisonMomentWidget->initializeSettings();
    deformationWidget->initializeSettings();
    displacementWidget->initializeSettings();
    allowableStressWidget->initializeSettings();
    checkWidget->initializeSettings();
    shearCheckEWidget->initializeSettings();
    shearCheckUWidget->initializeSettings();
    plasticRateWidget->initializeSettings();
    rankWidget->initializeSettings();
    yieldStepWidget->initializeSettings();
    yieldLoadRateWidget->initializeSettings();
    centerOfRigidityWidget->initializeSettings();
    graphQDeltaWidget->initializeSettings();
    graphQDeltaFrameWidget->initializeSettings();
    graphQDeltaFloorWidget->initializeSettings();
    graphQThetaWidget->initializeSettings();
    graphQThetaFrameWidget->initializeSettings();
    graphQThetaFloorWidget->initializeSettings();
    graphHorizontalCapacityWidget->initializeSettings();
    graphNmintWidget->initializeSettings();
}

void OutputFigureCustomSettingTabWidget::saveSettings(QSettings& settings){
    /*
    modelWidget->saveSettings(settings);
    memberLengthWidget->saveSettings(settings);
    faceLengthWidget->saveSettings(settings);
    nodeCoordinateWidget->saveSettings(settings);
    rigidLengthWidget->saveSettings(settings);
    sideBucklingLengthWidget->saveSettings(settings);
    compressionBucklingLengthWidget->saveSettings(settings);
    nodeWeightWidget->saveSettings(settings);
    nodeOuterForceWidget->saveSettings(settings);
    nodeOuterMomentWidget->saveSettings(settings);
    rigidityRatioWidget->saveSettings(settings);
    cmqWidget->saveSettings(settings);
    axialForceMapWidget->saveSettings(settings);
    reactionForceWidget->saveSettings(settings);
    combinatedReactionForceWidget->saveSettings(settings);
    shearForceMapWidget->saveSettings(settings);
    verticalLoadMapWidget->saveSettings(settings);
    floorDeformationMapWidget->saveSettings(settings);
    floorRigidityMapWidget->saveSettings(settings);
    nodeStressWidget->saveSettings(settings);
    faceStressWidget->readSettings(settings);
    axialForceWidget->saveSettings(settings);
    torisonMomentWidget->saveSettings(settings);
    deformationWidget->saveSettings(settings);
    displacementWidget->saveSettings(settings);
    allowableStressWidget->saveSettings(settings);
    checkWidget->saveSettings(settings);
    shearCheckEWidget->saveSettings(settings);
    shearCheckUWidget->saveSettings(settings);
    plasticRateWidget->saveSettings(settings);
    rankWidget->saveSettings(settings);
    yieldStepWidget->saveSettings(settings);
    yieldLoadRateWidget->saveSettings(settings);
    centerOfRigidityWidget->saveSettings(settings);
    graphQDeltaWidget->saveSettings(settings);
    graphQDeltaFrameWidget->saveSettings(settings);
    graphQDeltaFloorWidget->saveSettings(settings);
    graphQThetaWidget->saveSettings(settings);
    graphQThetaFrameWidget->saveSettings(settings);
    graphQThetaFloorWidget->saveSettings(settings);
    graphHorizontalCapacityWidget->saveSettings(settings);
    graphNmintWidget->saveSettings(settings);
    */
}
void OutputFigureCustomSettingTabWidget::readSettings(QSettings& settings){
    /*
    modelWidget->readSettings(settings);
    memberLengthWidget->readSettings(settings);
    faceLengthWidget->readSettings(settings);
    nodeCoordinateWidget->readSettings(settings);
    rigidLengthWidget->readSettings(settings);
    sideBucklingLengthWidget->readSettings(settings);
    compressionBucklingLengthWidget->readSettings(settings);
    nodeWeightWidget->readSettings(settings);
    nodeOuterForceWidget->readSettings(settings);
    nodeOuterMomentWidget->readSettings(settings);
    rigidityRatioWidget->readSettings(settings);
    cmqWidget->readSettings(settings);
    axialForceMapWidget->readSettings(settings);
    reactionForceWidget->readSettings(settings);
    combinatedReactionForceWidget->readSettings(settings);
    shearForceMapWidget->readSettings(settings);
    verticalLoadMapWidget->readSettings(settings);
    floorDeformationMapWidget->readSettings(settings);
    floorRigidityMapWidget->readSettings(settings);
    nodeStressWidget->readSettings(settings);
    faceStressWidget->readSettings(settings);
    axialForceWidget->readSettings(settings);
    torisonMomentWidget->readSettings(settings);
    deformationWidget->readSettings(settings);
    displacementWidget->readSettings(settings);
    allowableStressWidget->readSettings(settings);
    checkWidget->readSettings(settings);
    shearCheckEWidget->readSettings(settings);
    shearCheckUWidget->readSettings(settings);
    plasticRateWidget->readSettings(settings);
    rankWidget->readSettings(settings);
    yieldStepWidget->readSettings(settings);
    yieldLoadRateWidget->readSettings(settings);
    centerOfRigidityWidget->readSettings(settings);
    graphQDeltaWidget->readSettings(settings);
    graphQDeltaFrameWidget->readSettings(settings);
    graphQDeltaFloorWidget->readSettings(settings);
    graphQThetaWidget->readSettings(settings);
    graphQThetaFrameWidget->readSettings(settings);
    graphQThetaFloorWidget->readSettings(settings);
    graphHorizontalCapacityWidget->readSettings(settings);
    graphNmintWidget->readSettings(settings);
    */
}


} // namespace post3dapp
