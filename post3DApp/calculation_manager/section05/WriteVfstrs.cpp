#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteVfstrs::Command = u8"*VFSTRS";
const QString WriteVfstrs::Japanese = u8"鉛直荷重によるフェイス位置架構応力図(架構面内)";

WriteVfstrs::WriteVfstrs(const QStringList& list):AbstractWrite(list){};

QString WriteVfstrs::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();

    FIGURE_TYPE_PROPERTY typeOption;
    typeOption.type = OUTFIGTYPE::FSTRESS;
    typeOption.scenetype = SCENETYPE::ELEVATIONSCENE;
    typeOption.viewdirection = OUTPUTDIR::ODIR_PLIN;

    line += makeStressFigure(maker, setting, typeOption);
    line += suffix();
    return line;
}
} // namespace post3dapp
