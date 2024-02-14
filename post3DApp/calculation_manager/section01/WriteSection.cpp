#include "abstractwrite.h"
#include "drawinputfigure.h"
#include "nposk.h"
#include "typicalcolor_combobox.h"
namespace post3dapp{

const QString WriteSection::Command = u8"*SECTION";
const QString WriteSection::Japanese = u8"軸組図";

WriteSection::WriteSection(const QStringList& list):AbstractWrite(list){};

QString WriteSection::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{

    QString line;
    line += prefix();

    for (int i = 1; i < list.count(); i++){
        for (int j = 0; j < 8; j++){
            QString grid = list.at(i).mid(j * 10, 10).trimmed();
            if (grid == QString{}) continue;

            QStringList outGroups;
            if (Nposk::outGroupElevation.contains(grid)){
                outGroups << Nposk::outGroupElevation.value(grid);
            }
            else{
                outGroups << grid;
            }
            QString figureName = Command + "_" + grid;
            FIGURE_PROPERTY option = Nposk::elevationOption.value(outGroups.first());
            PAPERSETTING paperSetting = Nposk::elevationPaperSetting.value(outGroups.first());
            maker->groupFormats.insert(figureName, paperSetting);
            maker->outGroups.insert(figureName, outGroups);
            maker->figureToGroup.insert(figureName, figureName);
            maker->setFigureClass(new CalculationMakerLib::DrawInputFigure(figureName, false, option, Input2DViewSettings::makeSymbolSetting(setting.input2DViewSetting)));
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
