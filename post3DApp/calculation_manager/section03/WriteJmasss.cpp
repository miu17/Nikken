#include "abstractwrite.h"
namespace post3dapp{
const QString WriteJmasss::Command = u8"*JMASSS";
const QString WriteJmasss::Japanese = u8"節点重量軸組図";

WriteJmasss::WriteJmasss(const QStringList& list):AbstractWrite(list){};

QString WriteJmasss::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::JOINTMASS;
    option.scenetype = SCENETYPE::ELEVATIONSCENE;
    option.viewdirection = OUTPUTDIR::ODIR_PLOUT;

    line += makeStressFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp

