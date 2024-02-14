#include "calcload_grscene.h"

#include "base_background.h"
#include "calcloadgraphicsitem.h"
#include "calcloaditem_plane.h"
#include "calcloaditem_lplane.h"
#include "fixed_data.h"
#include "manage_memberdata.h"
#include "parameters.h"
#include "unified_inputdata.h"
#include "unified_settings.h"

namespace post3dapp{

CalcLoadGrScene::CalcLoadGrScene(const CalculationLoad2DViewSettings& setting,
                                 const FIGURE_TYPE_PROPERTY& figureTypeProperty,
                                 bool _syncSetting,
                                 QObject *parent)
    : CustomDrawGrScene(_syncSetting, parent), viewSettings(setting), figureProperty(figureTypeProperty)
{
     this->setSceneRect(PARAMETERS::scene_x_origin, PARAMETERS::scene_y_origin,
                                    PARAMETERS::scene_width, PARAMETERS::scene_height);

     if (syncSetting){
         connect( UnifiedSettings::getInstance(), &UnifiedSettings::calc2D_SettingChanged,
                  this, &CalcLoadGrScene::updateViewSettings );
     }

}

void CalcLoadGrScene::updateViewSettings()
{
    viewSettings = UnifiedSettings::getInstance()->getCalc2DViewSettings();
    // 通芯を再描画するには、上位にシグナルを投げる
    emit resetSceneView();
}

void CalcLoadGrScene::resetItems()
{
    qDebug() << "resetItems";
    clearItems();
    createJointItems();
    createMemberItems();
    update();
}

void CalcLoadGrScene::updateFigureTypeProperty(const FIGURE_TYPE_PROPERTY &property)
{
    figureProperty = property;
    this->resetItems();
}

void CalcLoadGrScene::clearItems()
{
    Q_FOREACH ( CalcLoadGraphicsItem *item, createdItems ) delete item;
    createdItems.clear();
}

void CalcLoadGrScene::drawBackGround(QPainter *painter)
{
    getBackGround()->drawWhole(painter);
}

CalcLoadGraphicsItem* CalcLoadGrScene::makePlaneItem(MemberData *md, const QList<QPointF> &points, bool isNormalDirection)
{
    CalcLoadGraphicsItem *item;
    if ( !isNormalDirection ) {
        item = new CalcLoadPlaneItem(md->dataType(), md->getUuid(), getViewSettings(), points);
    } else {
        item = new CalcLoadLinePlaneItem(md->dataType(), md->getUuid(), getViewSettings(),points);
    }

    item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    item->setPos(points.first());
    item->setZValue(-200.0);
    item->setEnabled(true);
    item->setSelected(false);
    return item;
}


}
