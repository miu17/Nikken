#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteVjstrp::Command = u8"*VJSTRP";
const QString WriteVjstrp::Japanese = u8"鉛直荷重による節点位置架構応力図(伏図面外)";

WriteVjstrp::WriteVjstrp(const QStringList& list):AbstractWrite(list){};

QString WriteVjstrp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::NSTRESS;
    option.scenetype = SCENETYPE::PLANSCENE;
    option.viewdirection = OUTPUTDIR::ODIR_PLIN;

    line += makeStressFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp

