#include "planOT_grscene.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "calc_vector2d.h"
#include "calc_vector3d.h"
#include "fixed_data.h"
#include "output_background_floor.h"
#include "unified_analysisdata.h"
#include "unified_output.h"
#include "unified_settings.h"

namespace post3dapp{
PlanOutputGrScene::PlanOutputGrScene(const Analysis2DViewSettings& setting, bool _syncSetting, const FIGURE_TYPE_PROPERTY& property, QObject *parent )
    : OutputGrScene( setting , _syncSetting, property, parent )
{
    backGround = new OutputBackGroundFloor(setting.UseScaledFont(), UnifiedFixedData::getInstance()->getAnalysisData());
    backGround->setSpanColorMode(false);
    backGround->setDrawCrossMode(true);
    if (property.type == OUTFIGTYPE::UNDEFINED){
        myFrameFloorID = QUuid();
    }
    else{
        myFrameFloorID = UnifiedFixedData::getInstance()->getAnalysisData()->nameToID(property.name,DATATYPE::TPAFLOOR);
    }
}

} // namespace post3dapp
