#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteLcmqs::Command = u8"*LCMQS";
const QString WriteLcmqs::Japanese = u8"基本応力軸組図";

WriteLcmqs::WriteLcmqs(const QStringList& list):AbstractWrite(list){};

QString WriteLcmqs::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::CMQ;
    option.scenetype = SCENETYPE::ELEVATIONSCENE;
    option.viewdirection = OUTPUTDIR::ODIR_PLOUT;

    line += makeStressFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp

