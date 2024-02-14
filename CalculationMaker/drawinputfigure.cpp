#include "drawinputfigure.h"
#include "unified_inputdata.h"
#include "unified_settings.h"
#include "fixed_data.h"
#include "draw_figure.h"
#include "input_grscene.h"

namespace CalculationMakerLib{
void DrawInputFigure::draw(QPainter *painter, const QRect &rect, const QString &floor, post3dapp::PAPERDIVIDE separate, int index)
{
    option.outerTargetSize = QSizeF(rect.width(), rect.height());
    post3dapp::FIGURE_SIZE_RESULT result(QPointF(rect.x(), rect.y()));

    post3dapp::FIGURE_TYPE_PROPERTY type;
    type.viewtype = post3dapp::MODELVIEWTYPE::INPUT2DMODELCOPY;
    type.name = floor;
    int tate = index / separate.yoko;
    int yoko = index - tate * separate.yoko;

    if (isPlan()){
        type.scenetype = post3dapp::SCENETYPE::PLANSCENE;
        auto scene = post3dapp::DrawFigure::createCopyScene(post3dapp::UnifiedFixedData::getInstance()->getInputData(), type, viewSetting);
        scene->resetItems();
        post3dapp::DrawFigure::drawForCalcPaper(painter, rect, scene, tate, yoko, result, option);
    }
    else{
        type.scenetype = post3dapp::SCENETYPE::ELEVATIONSCENE;
        auto scene = post3dapp::DrawFigure::createCopyScene(post3dapp::UnifiedFixedData::getInstance()->getInputData(), type, viewSetting);
        scene->resetItems();
        post3dapp::DrawFigure::drawForCalcPaper(painter, rect, scene, tate, yoko,result,option);
    }


}
} // namespace CalculationMakerLib
