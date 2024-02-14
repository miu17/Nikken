#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteMrls::Command = u8"*MRLS";
const QString WriteMrls::Japanese = u8"剛域長軸組図";

WriteMrls::WriteMrls(const QStringList& list):AbstractWrite(list){};

QString WriteMrls::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::RIGIDLENGTH;
    option.scenetype = SCENETYPE::ELEVATIONSCENE;

    line += makeModelFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp
