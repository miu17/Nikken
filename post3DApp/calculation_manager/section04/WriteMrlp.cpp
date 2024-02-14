#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteMrlp::Command = u8"*MRLP";
const QString WriteMrlp::Japanese = u8"剛域長伏図";

WriteMrlp::WriteMrlp(const QStringList& list):AbstractWrite(list){};

QString WriteMrlp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::RIGIDLENGTH;
    option.scenetype = SCENETYPE::PLANSCENE;
    option.viewdirection = OUTPUTDIR::ODIR_PLIN;

    line += makeModelFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp
