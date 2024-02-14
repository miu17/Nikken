#include "drawoutputfigure.h"
#include <QDebug>
#include "define_settings.h"
#include "fixed_data.h"
#include "draw_figure.h"
#include "output_grscene.h"
#include "unified_analysisdata.h"
#include "unified_settings.h"


namespace CalculationMakerLib{

void DrawOutputFigure::draw(QPainter *painter, const QRect &rect, const QString &floor, post3dapp::PAPERDIVIDE separate, int index)
{
    sizeOption.outerTargetSize = QSizeF(rect.width(), rect.height());
    post3dapp::FIGURE_SIZE_RESULT result(QPointF(rect.x(), rect.y()));


    int tate = index / separate.yoko;
    int yoko = index - tate * separate.yoko;

    auto scene = post3dapp::DrawFigure::createOutScene(typeOption, viewSetting);
    scene->resetItems();
    post3dapp::DrawFigure::drawForCalcPaper(painter, rect, scene, tate, yoko, result, sizeOption);
}

bool DrawOutputFigure::isPlan(){
    return typeOption.scenetype == post3dapp::SCENETYPE::PLANSCENE;
}
} // namespace CalculationMakerLib
