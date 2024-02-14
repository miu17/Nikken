#include "abstractwrite.h"

#include "drawoutputfigure.h"
#include "nposk.h"
namespace post3dapp{

AbstractWrite::~AbstractWrite()
{

}

QString AbstractWrite::makeSelfFigure(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting,  FIGURE_TYPE_PROPERTY typeOption) const{
    QString line;
    Q_FOREACH(QString text, list){
        if (text.startsWith(getCommand()))
            continue;

        for (int j = 1; j < 8; j++){
            QString frame = text.mid(j * 10, 10).trimmed();
            if (frame == QString{})
                continue;

            QStringList outGroups;
            FIGURE_PROPERTY option;
            PAPERSETTING paperSetting;
            if (typeOption.scenetype==SCENETYPE::PLANSCENE){
                if (Nposk::outGroupPlan.contains(frame)){
                    outGroups << Nposk::outGroupPlan.value(frame);
                }
                else{
                    outGroups << frame;
                }
                option = Nposk::planOption.value(outGroups.first());
                paperSetting = Nposk::planPaperSetting.value(outGroups.first());
            }else{
                if (Nposk::outGroupElevation.contains(frame)){
                    outGroups << Nposk::outGroupElevation.value(frame);
                }
                else{
                    outGroups << frame;
                }
                option = Nposk::elevationOption.value(outGroups.first());
                paperSetting = Nposk::elevationPaperSetting.value(outGroups.first());
            }
            QString figureName = getCommand() + "_" + frame;
            maker->groupFormats.insert(figureName, paperSetting);
            maker->outGroups.insert(figureName, outGroups);
            maker->figureToGroup.insert(figureName, figureName);
            maker->setFigureClass(new CalculationMakerLib::DrawOutputFigure{figureName, typeOption, option, setting.analysis2DViewSetting});
            line += figure(figureName,
                           setting.figure_size_policy.paperMargin_left,
                           setting.figure_size_policy.paperMargin_top,
                           paperSetting.getPaperSize().width() - (setting.figure_size_policy.paperMargin_left + setting.figure_size_policy.paperMargin_right),
                           paperSetting.getPaperSize().height() - (setting.figure_size_policy.paperMargin_top + setting.figure_size_policy.paperMargin_bottom));        }
    }
    return line;
}


QString AbstractWrite::makeStressFigure(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting, FIGURE_TYPE_PROPERTY typeOption) const{
    QString line;
    QString stress;
    Q_FOREACH(QString text, list){
        if (text.startsWith(getCommand()))
            continue;
        if (text.left(10).trimmed() != "-"){
            stress = text.left(10).trimmed();
        }
        typeOption.stress.clear();
        typeOption.stress << Nposk::stressMap.value(stress);

        for (int j = 1; j < 8; j++){
            QString frame = text.mid(j * 10, 10).trimmed();
            if (frame == QString{})
                continue;

            QStringList outGroups;
            FIGURE_PROPERTY option;
            PAPERSETTING paperSetting;
            if (typeOption.scenetype==SCENETYPE::PLANSCENE){
                if (Nposk::outGroupPlan.contains(frame)){
                    outGroups << Nposk::outGroupPlan.value(frame);
                }
                else{
                    outGroups << frame;
                }
                option = Nposk::planOption.value(outGroups.first());
                paperSetting = Nposk::planPaperSetting.value(outGroups.first());
            }else{
                if (Nposk::outGroupElevation.contains(frame)){
                    outGroups << Nposk::outGroupElevation.value(frame);
                }
                else{
                    outGroups << frame;
                }
                option = Nposk::elevationOption.value(outGroups.first());
                paperSetting = Nposk::elevationPaperSetting.value(outGroups.first());
            }
            QString figureName = getCommand() + "_" + stress + "_" + frame;
            maker->groupFormats.insert(figureName, paperSetting);
            maker->outGroups.insert(figureName, outGroups);
            maker->figureToGroup.insert(figureName, figureName);
            maker->setFigureClass(new CalculationMakerLib::DrawOutputFigure{figureName, typeOption, option, setting.analysis2DViewSetting});
            line += figure(figureName,
                           setting.figure_size_policy.paperMargin_left,
                           setting.figure_size_policy.paperMargin_top,
                           paperSetting.getPaperSize().width() - (setting.figure_size_policy.paperMargin_left + setting.figure_size_policy.paperMargin_right),
                           paperSetting.getPaperSize().height() - (setting.figure_size_policy.paperMargin_top + setting.figure_size_policy.paperMargin_bottom));        }
    }
    return line;
}


QString AbstractWrite::makeModelFigure(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting, FIGURE_TYPE_PROPERTY typeOption) const{
    QString line;
    QString model;
    Q_FOREACH(QString text, list){
        if (text.startsWith(getCommand()))
            continue;
        if (text.left(10).trimmed() != "-"){
            model = text.left(10).trimmed();
        }
        typeOption.model = model;

        for (int j = 1; j < 8; j++){
            QString frame = text.mid(j * 10, 10).trimmed();
            if (frame == QString{})
                continue;
            typeOption.name = frame;

            QStringList outGroups;
            FIGURE_PROPERTY option;
            PAPERSETTING paperSetting;
            if (typeOption.scenetype==SCENETYPE::PLANSCENE){
                if (Nposk::outGroupPlan.contains(frame)){
                    outGroups << Nposk::outGroupPlan.value(frame);
                }
                else{
                    outGroups << frame;
                }
                option = Nposk::planOption.value(outGroups.first());
                paperSetting = Nposk::planPaperSetting.value(outGroups.first());
            }else{
                if (Nposk::outGroupElevation.contains(frame)){
                    outGroups << Nposk::outGroupElevation.value(frame);
                }
                else{
                    outGroups << frame;
                }
                option = Nposk::elevationOption.value(outGroups.first());
                paperSetting = Nposk::elevationPaperSetting.value(outGroups.first());
            }
            QString figureName = getCommand() + "_" + model + "_" + frame;
            maker->groupFormats.insert(figureName, paperSetting);
            maker->outGroups.insert(figureName, outGroups);
            maker->figureToGroup.insert(figureName, figureName);
            maker->setFigureClass(new CalculationMakerLib::DrawOutputFigure{figureName, typeOption, option, setting.analysis2DViewSetting});
            line += figure(figureName,
                           setting.figure_size_policy.paperMargin_left,
                           setting.figure_size_policy.paperMargin_top,
                           paperSetting.getPaperSize().width() - (setting.figure_size_policy.paperMargin_left + setting.figure_size_policy.paperMargin_right),
                           paperSetting.getPaperSize().height() - (setting.figure_size_policy.paperMargin_top + setting.figure_size_policy.paperMargin_bottom));        }
    }
    return line;
}

} // namespace post3dapp
