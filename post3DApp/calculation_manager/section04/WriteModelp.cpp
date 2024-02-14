#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteModelp::Command = u8"*MODELP";
const QString WriteModelp::Japanese = u8"構造モデル化伏図";

WriteModelp::WriteModelp(const QStringList& list):AbstractWrite(list){};

QString WriteModelp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
    option.scenetype = SCENETYPE::PLANSCENE;

    option.type = OUTFIGTYPE::MODEL;
    option.viewdirection = OUTPUTDIR::ODIR_PLIN;

    line += makeModelFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp
