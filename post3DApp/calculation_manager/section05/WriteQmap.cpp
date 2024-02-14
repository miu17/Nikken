#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteQmap::Command = u8"*QMAP";
const QString WriteQmap::Japanese = u8"せん断力伏図";

WriteQmap::WriteQmap(const QStringList& list):AbstractWrite(list){};

QString WriteQmap::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY typeOption;
    typeOption.type = OUTFIGTYPE::SHEARMAP;
    typeOption.scenetype = SCENETYPE::PLANSCENE;
    typeOption.viewdirection = OUTPUTDIR::ODIR_PLOUT;

    line += makeStressFigure(maker, setting, typeOption);
    line += suffix();
    return line;
}
} // namespace post3dapp


