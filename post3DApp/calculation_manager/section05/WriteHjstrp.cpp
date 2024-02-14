#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteHjstrp::Command = u8"*HJSTRP";
const QString WriteHjstrp::Japanese = u8"水平荷重による節点位置架構応力図(伏図面内)";

WriteHjstrp::WriteHjstrp(const QStringList& list):AbstractWrite(list){};

QString WriteHjstrp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY typeOption;
    typeOption.type = OUTFIGTYPE::NSTRESS;
    typeOption.scenetype = SCENETYPE::PLANSCENE;
    typeOption.viewdirection = OUTPUTDIR::ODIR_PLIN;

    line += makeStressFigure(maker, setting, typeOption);
    line += suffix();
    return line;
}
} // namespace post3dapp
