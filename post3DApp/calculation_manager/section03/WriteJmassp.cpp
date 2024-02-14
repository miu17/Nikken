#include "abstractwrite.h"
namespace post3dapp{
const QString WriteJmassp::Command = u8"*JMASSP";
const QString WriteJmassp::Japanese = u8"節点重量伏図";

WriteJmassp::WriteJmassp(const QStringList& list):AbstractWrite(list){};

QString WriteJmassp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::JOINTMASS;
    option.scenetype = SCENETYPE::PLANSCENE;
    option.viewdirection = OUTPUTDIR::ODIR_PLOUT;

    line += makeStressFigure(maker, setting, option);
    line += suffix();
    return line;
}

} // namespace post3dapp

