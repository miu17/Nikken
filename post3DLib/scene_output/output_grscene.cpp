#include "output_grscene.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "calc_vector2d.h"
#include "calc_vector3d.h"
#include "elevationOT_grscene.h"
#include "figure_template.h"
#include "figure_template_factory.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "output_background_frame.h"
#include "output_background_floor.h"
#include "parameters.h"
#include "planOT_grscene.h"
#include "unified_analysisdata.h"
#include "unified_output.h"
#include "unified_settings.h"

namespace post3dapp{


OutputGrScene::OutputGrScene(const Analysis2DViewSettings& setting, bool _syncSetting, const FIGURE_TYPE_PROPERTY& property, QObject *parent )
    : CustomDrawGrScene( _syncSetting, parent ), viewSettings(setting), figureProperty(property)
{
    qDebug()<<"OutputGrScene::constractor in";
    this->setSceneRect(PARAMETERS::scene_x_origin, PARAMETERS::scene_y_origin,
                              PARAMETERS::scene_width, PARAMETERS::scene_height);

    if (syncSetting){
        connect( UnifiedSettings::getInstance(), &UnifiedSettings::out2D_SettingChanged,
                 this, &OutputGrScene::updateViewSettings );
    }

    connect( UnifiedFixedData::getInstance()->getPstnData(), &UnifiedOutput::outputDataUpdated,
             this, &OutputGrScene::updateOutputItems );
    qDebug()<<"OutputGrScene::constractor out";
}

OutputGrScene::~OutputGrScene()
{
    clearItems();
}


void post3dapp::OutputGrScene::setFigureTypeProperty(const QUuid &uid, const post3dapp::FIGURE_TYPE_PROPERTY &property)
{
    figureProperty = property;
    myFrameFloorID = uid;

    this->resetItems();
}

void OutputGrScene::updateOutputItems(OUTFIGTYPE otype)
{
    qDebug()<<"OutputGrScene::updateOutputItems in";
    if ( figureProperty.type == otype ) this->resetItems();
    qDebug()<<"OutputGrScene::updateOutputItems out";
}
void OutputGrScene::updateViewSettings()
{
    qDebug()<<"OutputGrScene::updateViewSettings in";

    viewSettings = UnifiedSettings::getInstance()->getOutput2DViewSettings();
    // 通芯を再描画するには、上位にシグナルを投げる
    emit resetSceneView();
    qDebug()<<"OutputGrScene::updateViewSettings out";
}

void OutputGrScene::drawBackGround(QPainter *painter)
{
    qDebug()<<"OutputGrScene::drawBackGround : in";
    if(myFrameFloorID==QUuid()){
        return;
    }
    getBackGround()->drawWhole(painter);
    qDebug()<<"OutputGrScene::drawBackGround : out";
}


void OutputGrScene::clearItems()
{

    Q_FOREACH ( QGraphicsItem *item, items() ) {
        removeItem(item);
    }

}

void OutputGrScene::resetItems()
{
    QColor scaleColor = Qt::black;
    int scaleTextSize = 3;
    bool gridTextVisible = true;
    bool floorTextVisible = true;
    clearItems();
    if (figureProperty.type != OUTFIGTYPE::UNDEFINED){
//        qDebug()<<"OutputGrScene::resetItems() createTemplate";
        auto figureTemplate = FigureTemplateFactory::CreateTemplate(viewSettings, figureProperty, myFrameFloorID);
//        qDebug()<<"OutputGrScene::resetItems() addItems";
        figureTemplate->addItems(this);
        scaleColor = figureTemplate->scaleColor();
        gridTextVisible = figureTemplate->scaleGridTextVisible();
        floorTextVisible = figureTemplate->scaleFloorTextVisible();
        scaleTextSize = figureTemplate->scaleTextSize();
//        qDebug()<<"scale_color"<<scaleColor;
    }

    getBackGround()->resetFramePoints(myFrameFloorID);
    getBackGround()->setColor(scaleColor);
    getBackGround()->setScaleTextSize(scaleTextSize);
    getBackGround()->setScaleTextVisible(floorTextVisible, gridTextVisible);
    update();
}




QPointF OutputGrScene::jointToPoint(JointData *jd, qreal near_x) const
{
    XYZ xyz = UnifiedFixedData::getInstance()->getAnalysisData()->globalXYZToFrameXYZ(myFrameFloorID, jd->xyz(true));

    // 解析階の高さ補正
//    xyz.y=xyz.y+UnifiedAnalysisData::getOutInstance()->getFloorValue( jd->getFirstFloor() ).adjustedLevel;

    if ( qAbs(xyz.x) < 1.0e-5 ) {
        if ( qAbs(near_x) < 1.0e-5 ) return QPointF(0.0, xyz.y * 1000.0);
        qreal cx = UnifiedFixedData::getInstance()->getAnalysisData()->globalZToClosedFrameEndX(myFrameFloorID, jd->zPos()) * 1000.0;
        return ( qAbs(cx - near_x) > qAbs(near_x) ) ? QPointF(0.0, xyz.y * 1000.0)
               : QPointF(cx, xyz.y * 1000.0);
    } else {
        return QPointF(xyz.x * 1000.0, xyz.y * 1000.0);
    }
}


} // namespace post3dapp
