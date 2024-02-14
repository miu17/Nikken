#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteVjstrs::Command = u8"*VJSTRS";
const QString WriteVjstrs::Japanese = u8"鉛直荷重による節点位置架構応力図";

WriteVjstrs::WriteVjstrs(const QStringList& list):AbstractWrite(list){};

QString WriteVjstrs::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::NSTRESS;
    option.scenetype = SCENETYPE::ELEVATIONSCENE;
    option.viewdirection = OUTPUTDIR::ODIR_PLIN;

    line += makeStressFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp
