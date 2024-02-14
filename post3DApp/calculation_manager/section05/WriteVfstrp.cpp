#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"

namespace post3dapp{
const QString WriteVfstrp::Command = u8"*VFSTRP";
const QString WriteVfstrp::Japanese = u8"鉛直荷重によるフェイス位置架構応力図(伏図面外)";

WriteVfstrp::WriteVfstrp(const QStringList& list):AbstractWrite(list){};

QString WriteVfstrp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY typeOption;
    typeOption.type = OUTFIGTYPE::FSTRESS;
    typeOption.scenetype = SCENETYPE::PLANSCENE;
    typeOption.viewdirection = OUTPUTDIR::ODIR_PLOUT;


    line += makeStressFigure(maker, setting, typeOption);
    line += suffix();
    return line;
}
} // namespace post3dapp
