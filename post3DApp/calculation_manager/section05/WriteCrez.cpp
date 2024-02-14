#include "abstractwrite.h"
#include "nposk.h"
#include "define_draw_figure.h"
#include "drawoutputfigure.h"
namespace post3dapp{
const QString WriteCrez::Command = u8"*CREZ";
const QString WriteCrez::Japanese = u8"組み合わせ鉛直方向反力伏図";

WriteCrez::WriteCrez(const QStringList& list):AbstractWrite(list){};

QString WriteCrez::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();


    FIGURE_TYPE_PROPERTY typeOption;
    typeOption.type = OUTFIGTYPE::COMBIREACTMAP;
    typeOption.scenetype = SCENETYPE::PLANSCENE;
    typeOption.viewdirection = OUTPUTDIR::ODIR_PLOUT;

    QMap<QString, QStringList> groupMap;
    // A4縦に収まるかどうかの判定が必要
    Q_FOREACH(QString text, list){
        if (list.startsWith(Command))
            continue;

        int startColumn;
        if (text.left(10).trimmed() == "-"){
            startColumn = 1; // 11カラムから
        }
        else{
            typeOption.stress.clear();
            typeOption.stress << Nposk::stressMap.value(text.left(10).trimmed());
            typeOption.stress << Nposk::stressMap.value(text.mid(10, 10).trimmed());
            typeOption.stress << Nposk::stressMap.value(text.mid(20, 10).trimmed());
            startColumn = 4; // 41カラムから
        }

        for (int j = startColumn; j < 8; j++){
            QString floor = text.mid(j * 10, 10).trimmed();
            if (floor == QString{}) continue;

            QStringList outGroups;
            if (Nposk::outGroupPlan.contains(floor)){
                outGroups << Nposk::outGroupPlan.value(floor);
            }
            else{
                outGroups << floor;
            }
            FIGURE_PROPERTY option = Nposk::planOption.value(outGroups.first());
            PAPERSETTING paperSetting = Nposk::planPaperSetting.value(outGroups.first());
            QString figureName = Command + "_" + floor;
            maker->groupFormats.insert(figureName, paperSetting);
            maker->outGroups.insert(figureName, outGroups);
            maker->figureToGroup.insert(figureName, figureName);
            maker->setFigureClass(new CalculationMakerLib::DrawOutputFigure{figureName, typeOption, option, setting.analysis2DViewSetting});
            line += figure(figureName,
                           setting.figure_size_policy.paperMargin_left,
                           setting.figure_size_policy.paperMargin_top,
                           paperSetting.getPaperSize().width() - (setting.figure_size_policy.paperMargin_left + setting.figure_size_policy.paperMargin_right),
                           paperSetting.getPaperSize().height() - (setting.figure_size_policy.paperMargin_top + setting.figure_size_policy.paperMargin_bottom));
        }
    }

    line += suffix();
    return line;

}
} // namespace post3dapp
