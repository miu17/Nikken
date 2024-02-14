#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteMphais::Command = u8"*MPHAIS";
const QString WriteMphais::Japanese = u8"剛比・曲げ剛性増大率軸組図";

WriteMphais::WriteMphais(const QStringList& list):AbstractWrite(list){};

QString WriteMphais::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::RIGIDITYPHI;
    option.scenetype = SCENETYPE::ELEVATIONSCENE;

    line += makeModelFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp

