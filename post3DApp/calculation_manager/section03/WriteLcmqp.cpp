#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
namespace post3dapp{
const QString WriteLcmqp::Command = u8"*LCMQP";
const QString WriteLcmqp::Japanese = u8"基本応力伏図";

WriteLcmqp::WriteLcmqp(const QStringList& list):AbstractWrite(list){};

QString WriteLcmqp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();


    FIGURE_TYPE_PROPERTY option;
    option.type = OUTFIGTYPE::CMQ;
    option.scenetype = SCENETYPE::PLANSCENE;
    option.viewdirection = OUTPUTDIR::ODIR_PLOUT;

    line += makeStressFigure(maker, setting, option);
    line += suffix();
    return line;
}
} // namespace post3dapp

