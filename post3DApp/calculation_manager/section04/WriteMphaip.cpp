#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteMphaip::Command = u8"*MPHAIP";
const QString WriteMphaip::Japanese = u8"剛比・曲げ剛性増大率伏図";

WriteMphaip::WriteMphaip(const QStringList& list):AbstractWrite(list){};

QString WriteMphaip::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::RIGIDITYPHI;
    option.scenetype = SCENETYPE::PLANSCENE;

    line += makeModelFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp
