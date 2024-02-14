#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteHfstrs::Command = u8"*HFSTRS";
const QString WriteHfstrs::Japanese = u8"水平荷重によるフェイス位置架構応力図";

WriteHfstrs::WriteHfstrs(const QStringList& list):AbstractWrite(list){};

QString WriteHfstrs::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::FSTRESS;
    option.scenetype = SCENETYPE::ELEVATIONSCENE;
    option.viewdirection = OUTPUTDIR::ODIR_PLIN;


    line += makeStressFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp

