#include "elevationOT_grscene.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "fixed_data.h"
#include "output_background_frame.h"
#include "unified_analysisdata.h"

namespace post3dapp{

ElevationOutputGrScene::ElevationOutputGrScene(const Analysis2DViewSettings& setting, bool _syncSetting, const FIGURE_TYPE_PROPERTY& property, QObject *parent )
    : OutputGrScene( setting, _syncSetting, property, parent )
{
    qDebug()<<"ElevationOutputGrScene::constractor in";
    qDebug()<<"OutputGrScene::drawBackGround : is elev";
    backGround = new OutputBackGroundFrame(setting.UseScaledFont(), UnifiedFixedData::getInstance()->getAnalysisData());
    if (property.type == OUTFIGTYPE::UNDEFINED){
        myFrameFloorID = QUuid();
    }
    else{
        myFrameFloorID =  UnifiedFixedData::getInstance()->getAnalysisData()->nameToID(property.name,DATATYPE::TPFRAMEP);
    }
    qDebug()<<"ElevationOutputGrScene::constractor out";

}


} // namespace post3dapp
