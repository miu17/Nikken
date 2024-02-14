#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteHfstrp::Command = u8"*HFSTRP";
const QString WriteHfstrp::Japanese = u8"水平荷重によるフェイス位置架構応力図";

WriteHfstrp::WriteHfstrp(const QStringList& list):AbstractWrite(list){};

QString WriteHfstrp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::FSTRESS;
    option.scenetype = SCENETYPE::PLANSCENE;
    option.viewdirection = OUTPUTDIR::ODIR_PLIN;

    line += makeStressFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp
