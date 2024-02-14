#include "abstractwrite.h"
#include "drawinputfigure.h"
#include "nposk.h"
#include "typicalcolor_combobox.h"

namespace post3dapp{
const QString WriteLplan::Command = u8"*LPLAN";
const QString WriteLplan::Japanese = u8"荷重伏図";

WriteLplan::WriteLplan(const QStringList& list):AbstractWrite(list){};

QString WriteLplan::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    for (int i = 1; i < list.count(); i++){
        for (int j = 0; j < 8; j++){
            QString floor = list.at(i).mid(j * 10, 10).trimmed();
            if (floor == QString{}) continue;

            QStringList outGroups;
            if (Nposk::outGroupPlan.contains(floor)){
                outGroups << Nposk::outGroupPlan.value(floor);
            }
            else {
                outGroups << floor;
            }

            QString figureName = Command + "_" + floor;
            FIGURE_PROPERTY option = Nposk::planOption.value(outGroups.first());
            PAPERSETTING paperSetting = Nposk::planPaperSetting.value(outGroups.first());
            maker->groupFormats.insert(figureName, paperSetting);
            maker->outGroups.insert(figureName, outGroups);
            maker->figureToGroup.insert(figureName, figureName);
            maker->setFigureClass(new CalculationMakerLib::DrawInputFigure(figureName, true, option, Input2DViewSettings::makeLoadSetting(setting.input2DViewSetting)));
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

