#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"

namespace post3dapp{
const QString WriteHjstrs::Command = u8"*HJSTRS";
const QString WriteHjstrs::Japanese = u8"水平荷重による節点位置架構応力図(架構面内)";

WriteHjstrs::WriteHjstrs(const QStringList& list):AbstractWrite(list){};

QString WriteHjstrs::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY typeOption;
    typeOption.type = OUTFIGTYPE::NSTRESS;
    typeOption.scenetype = SCENETYPE::ELEVATIONSCENE;
    typeOption.viewdirection = OUTPUTDIR::ODIR_PLIN;

    line += makeStressFigure(maker, setting, typeOption);
    line += suffix();
    return line;
}
} // namespace post3dapp
