#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteReact::Command = u8"*REACT";
const QString WriteReact::Japanese = u8"反力伏図";


WriteReact::WriteReact(const QStringList& list):AbstractWrite(list){};

QString WriteReact::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY typeOption;
    typeOption.type = OUTFIGTYPE::REACTMAP;
    typeOption.scenetype = SCENETYPE::PLANSCENE;
    typeOption.viewdirection = OUTPUTDIR::ODIR_PLOUT;

    line += makeStressFigure(maker, setting, typeOption);
    line += suffix();
    return line;
}
} // namespace post3dapp

