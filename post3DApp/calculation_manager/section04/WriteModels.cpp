#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteModels::Command = u8"*MODELS";
const QString WriteModels::Japanese = u8"構造モデル化軸組図";

WriteModels::WriteModels(const QStringList& list):AbstractWrite(list){};

QString WriteModels::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
    option.scenetype = SCENETYPE::ELEVATIONSCENE;

    option.type = OUTFIGTYPE::MODEL;
    option.viewdirection = OUTPUTDIR::ODIR_PLIN;

    line += makeModelFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp
