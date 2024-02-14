#include "define_analysis2dsettings.h"

#include "define_output.h"
#include "typicalcolor_table.h"
namespace post3dapp{
AnalysisCommon2DViewSetting::AnalysisCommon2DViewSetting(const AnalysisCommon2DViewSetting &setting)
{
    scaleColor = setting.scaleColor;
    scaleSize = setting.scaleSize;
    gridStringVisible = setting.gridStringVisible;
    floorStringVisible = setting.floorStringVisible;
    jointColor = setting.jointColor;
    jointSize = setting.jointSize;
    lineRcColor = setting.lineRcColor;
    lineSrcColor = setting.lineSrcColor;
    lineSColor = setting.lineSColor;
    lineCftColor = setting.lineCftColor;
    lineDamperColor = setting.lineDamperColor;
    lineIsoColor = setting.lineIsoColor;
    planeRcColor = setting.planeRcColor;
    planeSColor = setting.planeSColor;
    planeDamperColor = setting.planeDamperColor;
    supportColor = setting.supportColor;
    valueTextSize = setting.valueTextSize;
    selectedColor = setting.selectedColor;
    noteColor = setting.noteColor;
    noteTextSize = setting.noteTextSize;
    plasticSymbolColor = setting.plasticSymbolColor;
    plasticSymbolSize = setting.plasticSymbolSize;
}

AnalysisCommon2DViewSetting *AnalysisCommon2DViewSetting::getDefault()
{
    return new AnalysisCommon2DViewSetting(
                Qt::black,
                3,
                true,
                true,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                3
                );
}

AnalysisCommon2DViewSetting *AnalysisCommon2DViewSetting::readSettings(QSettings &settings)
{
    AnalysisCommon2DViewSetting* p = new AnalysisCommon2DViewSetting();
    settings.beginGroup("AnalysisCommon2DViewSetting");
    p->scaleColor = TypicalColorTable::nameToColor(settings.value("scaleColor").toString());
    p->scaleSize = settings.value("scaleSize").toInt();
    p->gridStringVisible = settings.value("gridStringVisible").toBool();
    p->floorStringVisible = settings.value("floorStringVisible").toBool();
    p->jointColor = TypicalColorTable::nameToColor(settings.value("jointColor").toString());
    p->jointSize = settings.value("jointSize").toInt();
    p->lineRcColor = TypicalColorTable::nameToColor(settings.value("lineRcColor").toString());
    p->lineSrcColor = TypicalColorTable::nameToColor(settings.value("lineSrcColor").toString());
    p->lineSColor = TypicalColorTable::nameToColor(settings.value("lineSColor").toString());
    p->lineCftColor = TypicalColorTable::nameToColor(settings.value("lineCftColor").toString());
    p->lineDamperColor = TypicalColorTable::nameToColor(settings.value("lineDamperColor").toString());
    p->lineIsoColor = TypicalColorTable::nameToColor(settings.value("lineIsoColor").toString());
    p->planeRcColor = TypicalColorTable::nameToColor(settings.value("planeRcColor").toString());
    p->planeSColor = TypicalColorTable::nameToColor(settings.value("planeSColor").toString());
    p->planeDamperColor = TypicalColorTable::nameToColor(settings.value("planeDamperColor").toString());
    p->supportColor = TypicalColorTable::nameToColor(settings.value("supportColor").toString());
    p->valueColor = TypicalColorTable::nameToColor(settings.value("valueColor").toString());
    p->valueTextSize = settings.value("valueTextSize").toInt();
    p->selectedColor = TypicalColorTable::nameToColor(settings.value("selectedColor").toString());
    p->noteColor = TypicalColorTable::nameToColor(settings.value("noteColor").toString());
    p->noteTextSize = settings.value("noteTextSize").toInt();
    p->plasticSymbolColor = TypicalColorTable::nameToColor(settings.value("plasticSymbolColor").toString());
    p->plasticSymbolSize = settings.value("plasticSymbolSize").toInt();
    settings.endGroup();
    return p;
}

void AnalysisCommon2DViewSetting::saveSettings(QSettings &settings) const
{
    settings.beginGroup("AnalysisCommon2DViewSetting");
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("scaleSize", scaleSize);
    settings.setValue("gridStringVisible", gridStringVisible);
    settings.setValue("floorStringVisible", floorStringVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("jointSize", jointSize);
    settings.setValue("lineRcColor", TypicalColorTable::colorToName(lineRcColor));
    settings.setValue("lineSrcColor", TypicalColorTable::colorToName(lineSrcColor));
    settings.setValue("lineSColor", TypicalColorTable::colorToName(lineSColor));
    settings.setValue("lineCftColor", TypicalColorTable::colorToName(lineCftColor));
    settings.setValue("lineDamperColor", TypicalColorTable::colorToName(lineDamperColor));
    settings.setValue("lineIsoColor", TypicalColorTable::colorToName(lineIsoColor));
    settings.setValue("planeRcColor", TypicalColorTable::colorToName(planeRcColor));
    settings.setValue("planeSColor", TypicalColorTable::colorToName(planeSColor));
    settings.setValue("planeDamperColor", TypicalColorTable::colorToName(planeDamperColor));
    settings.setValue("supportColor", TypicalColorTable::colorToName(supportColor));
    settings.setValue("valueColor", TypicalColorTable::colorToName(valueColor));
    settings.setValue("valueTextSize", valueTextSize);
    settings.setValue("selectedColor", TypicalColorTable::colorToName(selectedColor));
    settings.setValue("noteColor", TypicalColorTable::colorToName(noteColor));
    settings.setValue("noteTextSize", noteTextSize);
    settings.setValue("plasticSymbolColor", TypicalColorTable::colorToName(plasticSymbolColor));
    settings.setValue("plasticSymbolSize", plasticSymbolSize);
    settings.endGroup();
}

AnalysisReactionForce2DViewSetting::AnalysisReactionForce2DViewSetting(const AnalysisReactionForce2DViewSetting &setting)
    :        AnalysisCommon2DViewSetting(setting.scaleColor,
                                       setting.scaleSize,
                                       setting.gridStringVisible,
                                       setting.floorStringVisible,
                                       setting.jointColor,
                                       setting.jointSize,
                                       setting.lineRcColor,
                                       setting.lineSrcColor,
                                       setting.lineSColor,
                                       setting.lineCftColor,
                                       setting.lineDamperColor,
                                       setting.lineIsoColor,
                                       setting.planeRcColor,
                                       setting.planeSColor,
                                       setting.planeDamperColor,
                                       setting.supportColor,
                                       setting.valueColor,
                                       setting.valueTextSize,
                                       setting.selectedColor,
                                       setting.noteColor,
                                       setting.noteTextSize,
                                       setting.plasticSymbolColor,
                                       setting.plasticSymbolSize
                                       )
{
    minusColor = setting.minusColor;
    plusColor = setting.plusColor;
}

AnalysisReactionForce2DViewSetting *AnalysisReactionForce2DViewSetting::getDefault()
{
    return new AnalysisReactionForce2DViewSetting(
                Qt::black,
                3,
                true,
                true,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                3,
                Qt::black,
                Qt::black
                );
}

AnalysisReactionForce2DViewSetting *AnalysisReactionForce2DViewSetting::readSettings(QSettings &settings)
{
    AnalysisReactionForce2DViewSetting* p = new AnalysisReactionForce2DViewSetting();
    settings.beginGroup("AnalysisReactionForce2DViewSetting");
    p->scaleColor = TypicalColorTable::nameToColor(settings.value("scaleColor").toString());
    p->scaleSize = settings.value("scaleSize").toInt();
    p->gridStringVisible = settings.value("gridStringVisible").toBool();
    p->floorStringVisible = settings.value("floorStringVisible").toBool();
    p->jointColor = TypicalColorTable::nameToColor(settings.value("jointColor").toString());
    p->jointSize = settings.value("jointSize").toInt();
    p->lineRcColor = TypicalColorTable::nameToColor(settings.value("lineRcColor").toString());
    p->lineSrcColor = TypicalColorTable::nameToColor(settings.value("lineSrcColor").toString());
    p->lineSColor = TypicalColorTable::nameToColor(settings.value("lineSColor").toString());
    p->lineCftColor = TypicalColorTable::nameToColor(settings.value("lineCftColor").toString());
    p->lineDamperColor = TypicalColorTable::nameToColor(settings.value("lineDamperColor").toString());
    p->lineIsoColor = TypicalColorTable::nameToColor(settings.value("lineIsoColor").toString());
    p->planeRcColor = TypicalColorTable::nameToColor(settings.value("planeRcColor").toString());
    p->planeSColor = TypicalColorTable::nameToColor(settings.value("planeSColor").toString());
    p->planeDamperColor = TypicalColorTable::nameToColor(settings.value("planeDamperColor").toString());
    p->supportColor = TypicalColorTable::nameToColor(settings.value("supportColor").toString());
    p->valueColor = TypicalColorTable::nameToColor(settings.value("valueColor").toString());
    p->valueTextSize = settings.value("valueTextSize").toInt();
    p->selectedColor = TypicalColorTable::nameToColor(settings.value("selectedColor").toString());
    p->noteColor = TypicalColorTable::nameToColor(settings.value("noteColor").toString());
    p->noteTextSize = settings.value("noteTextSize").toInt();
    p->plasticSymbolColor = TypicalColorTable::nameToColor(settings.value("plasticSymbolColor").toString());
    p->plasticSymbolSize = settings.value("plasticSymbolSize").toInt();
    p->minusColor = TypicalColorTable::nameToColor(settings.value("minusColor").toString());
    p->plusColor = TypicalColorTable::nameToColor(settings.value("plusColor").toString());
    settings.endGroup();
    return p;
}

void AnalysisReactionForce2DViewSetting::saveSettings(QSettings &settings) const
{
    settings.beginGroup("AnalysisReactionForce2DViewSetting");
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("scaleSize", scaleSize);
    settings.setValue("gridStringVisible", gridStringVisible);
    settings.setValue("floorStringVisible", floorStringVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("jointSize", jointSize);
    settings.setValue("lineRcColor", TypicalColorTable::colorToName(lineRcColor));
    settings.setValue("lineSrcColor", TypicalColorTable::colorToName(lineSrcColor));
    settings.setValue("lineSColor", TypicalColorTable::colorToName(lineSColor));
    settings.setValue("lineCftColor", TypicalColorTable::colorToName(lineCftColor));
    settings.setValue("lineDamperColor", TypicalColorTable::colorToName(lineDamperColor));
    settings.setValue("lineIsoColor", TypicalColorTable::colorToName(lineIsoColor));
    settings.setValue("planeRcColor", TypicalColorTable::colorToName(planeRcColor));
    settings.setValue("planeSColor", TypicalColorTable::colorToName(planeSColor));
    settings.setValue("planeDamperColor", TypicalColorTable::colorToName(planeDamperColor));
    settings.setValue("supportColor", TypicalColorTable::colorToName(supportColor));
    settings.setValue("valueColor", TypicalColorTable::colorToName(valueColor));
    settings.setValue("valueTextSize", valueTextSize);
    settings.setValue("selectedColor", TypicalColorTable::colorToName(selectedColor));
    settings.setValue("noteColor", TypicalColorTable::colorToName(noteColor));
    settings.setValue("noteTextSize", noteTextSize);
    settings.setValue("plasticSymbolColor", TypicalColorTable::colorToName(plasticSymbolColor));
    settings.setValue("plasticSymbolSize", plasticSymbolSize);
    settings.setValue("minusColor", TypicalColorTable::colorToName(minusColor));
    settings.setValue("plusColor", TypicalColorTable::colorToName(plusColor));
    settings.endGroup();
}

AnalysisAxialForce2DViewSetting::AnalysisAxialForce2DViewSetting(const AnalysisAxialForce2DViewSetting &setting)
    :        AnalysisCommon2DViewSetting(setting.scaleColor,
                                       setting.scaleSize,
                                       setting.gridStringVisible,
                                       setting.floorStringVisible,
                                       setting.jointColor,
                                       setting.jointSize,
                                       setting.lineRcColor,
                                       setting.lineSrcColor,
                                       setting.lineSColor,
                                       setting.lineCftColor,
                                       setting.lineDamperColor,
                                       setting.lineIsoColor,
                                       setting.planeRcColor,
                                       setting.planeSColor,
                                       setting.planeDamperColor,
                                       setting.supportColor,
                                       setting.valueColor,
                                       setting.valueTextSize,
                                       setting.selectedColor,
                                       setting.noteColor,
                                       setting.noteTextSize,
                                       setting.plasticSymbolColor,
                                       setting.plasticSymbolSize
                                       )
{
    axialMinusColor = setting.axialMinusColor;
    axialPlusColor = setting.axialPlusColor;
    axialScale = setting.axialScale;
}

AnalysisAxialForce2DViewSetting *AnalysisAxialForce2DViewSetting::getDefault()
{
    return new AnalysisAxialForce2DViewSetting(
                Qt::black,
                3,
                true,
                true,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                1000.0
                );
}

AnalysisAxialForce2DViewSetting *AnalysisAxialForce2DViewSetting::readSettings(QSettings &settings)
{
    AnalysisAxialForce2DViewSetting* p = new AnalysisAxialForce2DViewSetting();
    settings.beginGroup("AnalysisAxialForce2DViewSetting");
    p->scaleColor = TypicalColorTable::nameToColor(settings.value("scaleColor").toString());
    p->scaleSize = settings.value("scaleSize").toInt();
    p->gridStringVisible = settings.value("gridStringVisible").toBool();
    p->floorStringVisible = settings.value("floorStringVisible").toBool();
    p->jointColor = TypicalColorTable::nameToColor(settings.value("jointColor").toString());
    p->jointSize = settings.value("jointSize").toInt();
    p->lineRcColor = TypicalColorTable::nameToColor(settings.value("lineRcColor").toString());
    p->lineSrcColor = TypicalColorTable::nameToColor(settings.value("lineSrcColor").toString());
    p->lineSColor = TypicalColorTable::nameToColor(settings.value("lineSColor").toString());
    p->lineCftColor = TypicalColorTable::nameToColor(settings.value("lineCftColor").toString());
    p->lineDamperColor = TypicalColorTable::nameToColor(settings.value("lineDamperColor").toString());
    p->lineIsoColor = TypicalColorTable::nameToColor(settings.value("lineIsoColor").toString());
    p->planeRcColor = TypicalColorTable::nameToColor(settings.value("planeRcColor").toString());
    p->planeSColor = TypicalColorTable::nameToColor(settings.value("planeSColor").toString());
    p->planeDamperColor = TypicalColorTable::nameToColor(settings.value("planeDamperColor").toString());
    p->supportColor = TypicalColorTable::nameToColor(settings.value("supportColor").toString());
    p->valueColor = TypicalColorTable::nameToColor(settings.value("valueColor").toString());
    p->valueTextSize = settings.value("valueTextSize").toInt();
    p->selectedColor = TypicalColorTable::nameToColor(settings.value("selectedColor").toString());
    p->noteColor = TypicalColorTable::nameToColor(settings.value("noteColor").toString());
    p->noteTextSize = settings.value("noteTextSize").toInt();
    p->plasticSymbolColor = TypicalColorTable::nameToColor(settings.value("plasticSymbolColor").toString());
    p->plasticSymbolSize = settings.value("plasticSymbolSize").toInt();
    p->axialMinusColor = TypicalColorTable::nameToColor(settings.value("axialMinusColor").toString());
    p->axialPlusColor = TypicalColorTable::nameToColor(settings.value("axialPlusColor").toString());
    p->axialScale = settings.value("axialScale").toDouble();
    settings.endGroup();
    return p;
}

void AnalysisAxialForce2DViewSetting::saveSettings(QSettings &settings) const
{
    settings.beginGroup("AnalysisAxialForce2DViewSetting");
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("scaleSize", scaleSize);
    settings.setValue("gridStringVisible", gridStringVisible);
    settings.setValue("floorStringVisible", floorStringVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("jointSize", jointSize);
    settings.setValue("lineRcColor", TypicalColorTable::colorToName(lineRcColor));
    settings.setValue("lineSrcColor", TypicalColorTable::colorToName(lineSrcColor));
    settings.setValue("lineSColor", TypicalColorTable::colorToName(lineSColor));
    settings.setValue("lineCftColor", TypicalColorTable::colorToName(lineCftColor));
    settings.setValue("lineDamperColor", TypicalColorTable::colorToName(lineDamperColor));
    settings.setValue("lineIsoColor", TypicalColorTable::colorToName(lineIsoColor));
    settings.setValue("planeRcColor", TypicalColorTable::colorToName(planeRcColor));
    settings.setValue("planeSColor", TypicalColorTable::colorToName(planeSColor));
    settings.setValue("planeDamperColor", TypicalColorTable::colorToName(planeDamperColor));
    settings.setValue("supportColor", TypicalColorTable::colorToName(supportColor));
    settings.setValue("valueColor", TypicalColorTable::colorToName(valueColor));
    settings.setValue("valueTextSize", valueTextSize);
    settings.setValue("selectedColor", TypicalColorTable::colorToName(selectedColor));
    settings.setValue("noteColor", TypicalColorTable::colorToName(noteColor));
    settings.setValue("noteTextSize", noteTextSize);
    settings.setValue("plasticSymbolColor", TypicalColorTable::colorToName(plasticSymbolColor));
    settings.setValue("plasticSymbolSize", plasticSymbolSize);
    settings.setValue("axialMinusColor", TypicalColorTable::colorToName(axialMinusColor));
    settings.setValue("axialPlusColor", TypicalColorTable::colorToName(axialPlusColor));
    settings.setValue("axialScale", axialScale);
    settings.endGroup();
}

AnalysisDeformation2DViewSetting::AnalysisDeformation2DViewSetting(const AnalysisDeformation2DViewSetting &setting)
    :        AnalysisCommon2DViewSetting(setting.scaleColor,
                                       setting.scaleSize,
                                       setting.gridStringVisible,
                                       setting.floorStringVisible,
                                       setting.jointColor,
                                       setting.jointSize,
                                       setting.lineRcColor,
                                       setting.lineSrcColor,
                                       setting.lineSColor,
                                       setting.lineCftColor,
                                       setting.lineDamperColor,
                                       setting.lineIsoColor,
                                       setting.planeRcColor,
                                       setting.planeSColor,
                                       setting.planeDamperColor,
                                       setting.supportColor,
                                       setting.valueColor,
                                       setting.valueTextSize,
                                       setting.selectedColor,
                                       setting.noteColor,
                                       setting.noteTextSize,
                                       setting.plasticSymbolColor,
                                       setting.plasticSymbolSize
                                       )
{
    beforeColor = setting.beforeColor;
    afterColor = setting.afterColor;
    deformScale = setting.deformScale;
}

AnalysisDeformation2DViewSetting *AnalysisDeformation2DViewSetting::getDefault()
{
    return new AnalysisDeformation2DViewSetting(
                Qt::black,
                3,
                true,
                true,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                200
                );
}

AnalysisDeformation2DViewSetting *AnalysisDeformation2DViewSetting::readSettings(QSettings &settings)
{
    AnalysisDeformation2DViewSetting* p = new AnalysisDeformation2DViewSetting();
    settings.beginGroup("AnalysisDeformation2DViewSetting");
    p->scaleColor = TypicalColorTable::nameToColor(settings.value("scaleColor").toString());
    p->scaleSize = settings.value("scaleSize").toInt();
    p->gridStringVisible = settings.value("gridStringVisible").toBool();
    p->floorStringVisible = settings.value("floorStringVisible").toBool();
    p->jointColor = TypicalColorTable::nameToColor(settings.value("jointColor").toString());
    p->jointSize = settings.value("jointSize").toInt();
    p->lineRcColor = TypicalColorTable::nameToColor(settings.value("lineRcColor").toString());
    p->lineSrcColor = TypicalColorTable::nameToColor(settings.value("lineSrcColor").toString());
    p->lineSColor = TypicalColorTable::nameToColor(settings.value("lineSColor").toString());
    p->lineCftColor = TypicalColorTable::nameToColor(settings.value("lineCftColor").toString());
    p->lineDamperColor = TypicalColorTable::nameToColor(settings.value("lineDamperColor").toString());
    p->lineIsoColor = TypicalColorTable::nameToColor(settings.value("lineIsoColor").toString());
    p->planeRcColor = TypicalColorTable::nameToColor(settings.value("planeRcColor").toString());
    p->planeSColor = TypicalColorTable::nameToColor(settings.value("planeSColor").toString());
    p->planeDamperColor = TypicalColorTable::nameToColor(settings.value("planeDamperColor").toString());
    p->supportColor = TypicalColorTable::nameToColor(settings.value("supportColor").toString());
    p->valueColor = TypicalColorTable::nameToColor(settings.value("valueColor").toString());
    p->valueTextSize = settings.value("valueTextSize").toInt();
    p->selectedColor = TypicalColorTable::nameToColor(settings.value("selectedColor").toString());
    p->noteColor = TypicalColorTable::nameToColor(settings.value("noteColor").toString());
    p->noteTextSize = settings.value("noteTextSize").toInt();
    p->plasticSymbolColor = TypicalColorTable::nameToColor(settings.value("plasticSymbolColor").toString());
    p->plasticSymbolSize = settings.value("plasticSymbolSize").toInt();
    p->beforeColor = TypicalColorTable::nameToColor(settings.value("beforeColor").toString());
    p->afterColor = TypicalColorTable::nameToColor(settings.value("afterColor").toString());
    p->deformScale = settings.value("deformScale").toDouble();
    settings.endGroup();
    return p;
}

void AnalysisDeformation2DViewSetting::saveSettings(QSettings &settings) const
{
    settings.beginGroup("AnalysisDeformation2DViewSetting");
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("scaleSize", scaleSize);
    settings.setValue("gridStringVisible", gridStringVisible);
    settings.setValue("floorStringVisible", floorStringVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("jointSize", jointSize);
    settings.setValue("lineRcColor", TypicalColorTable::colorToName(lineRcColor));
    settings.setValue("lineSrcColor", TypicalColorTable::colorToName(lineSrcColor));
    settings.setValue("lineSColor", TypicalColorTable::colorToName(lineSColor));
    settings.setValue("lineCftColor", TypicalColorTable::colorToName(lineCftColor));
    settings.setValue("lineDamperColor", TypicalColorTable::colorToName(lineDamperColor));
    settings.setValue("lineIsoColor", TypicalColorTable::colorToName(lineIsoColor));
    settings.setValue("planeRcColor", TypicalColorTable::colorToName(planeRcColor));
    settings.setValue("planeSColor", TypicalColorTable::colorToName(planeSColor));
    settings.setValue("planeDamperColor", TypicalColorTable::colorToName(planeDamperColor));
    settings.setValue("supportColor", TypicalColorTable::colorToName(supportColor));
    settings.setValue("valueColor", TypicalColorTable::colorToName(valueColor));
    settings.setValue("valueTextSize", valueTextSize);
    settings.setValue("selectedColor", TypicalColorTable::colorToName(selectedColor));
    settings.setValue("noteColor", TypicalColorTable::colorToName(noteColor));
    settings.setValue("noteTextSize", noteTextSize);
    settings.setValue("plasticSymbolColor", TypicalColorTable::colorToName(plasticSymbolColor));
    settings.setValue("plasticSymbolSize", plasticSymbolSize);
    settings.setValue("beforeColor", TypicalColorTable::colorToName(beforeColor));
    settings.setValue("afterColor", TypicalColorTable::colorToName(afterColor));
    settings.setValue("deformScale", deformScale);
    settings.endGroup();
}

AnalysisCheck2DViewSetting::AnalysisCheck2DViewSetting(const AnalysisCheck2DViewSetting &setting)
    :        AnalysisCommon2DViewSetting(setting.scaleColor,
                                       setting.scaleSize,
                                       setting.gridStringVisible,
                                       setting.floorStringVisible,
                                       setting.jointColor,
                                       setting.jointSize,
                                       setting.lineRcColor,
                                       setting.lineSrcColor,
                                       setting.lineSColor,
                                       setting.lineCftColor,
                                       setting.lineDamperColor,
                                       setting.lineIsoColor,
                                       setting.planeRcColor,
                                       setting.planeSColor,
                                       setting.planeDamperColor,
                                       setting.supportColor,
                                       setting.valueColor,
                                       setting.valueTextSize,
                                       setting.selectedColor,
                                       setting.noteColor,
                                       setting.noteTextSize,
                                       setting.plasticSymbolColor,
                                       setting.plasticSymbolSize
                                       )
{
    ngColor = setting.ngColor;
    useGradiationColor = setting.useGradiationColor;
}

AnalysisCheck2DViewSetting *AnalysisCheck2DViewSetting::getDefault()
{
    return new AnalysisCheck2DViewSetting(
                Qt::black,
                3,
                true,
                true,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                3,
                Qt::black,
                true
                );
}

AnalysisCheck2DViewSetting *AnalysisCheck2DViewSetting::readSettings(QSettings &settings)
{
    AnalysisCheck2DViewSetting* p = new AnalysisCheck2DViewSetting();
    settings.beginGroup("AnalysisCheck2DViewSetting");
    p->scaleColor = TypicalColorTable::nameToColor(settings.value("scaleColor").toString());
    p->scaleSize = settings.value("scaleSize").toInt();
    p->gridStringVisible = settings.value("gridStringVisible").toBool();
    p->floorStringVisible = settings.value("floorStringVisible").toBool();
    p->jointColor = TypicalColorTable::nameToColor(settings.value("jointColor").toString());
    p->jointSize = settings.value("jointSize").toInt();
    p->lineRcColor = TypicalColorTable::nameToColor(settings.value("lineRcColor").toString());
    p->lineSrcColor = TypicalColorTable::nameToColor(settings.value("lineSrcColor").toString());
    p->lineSColor = TypicalColorTable::nameToColor(settings.value("lineSColor").toString());
    p->lineCftColor = TypicalColorTable::nameToColor(settings.value("lineCftColor").toString());
    p->lineDamperColor = TypicalColorTable::nameToColor(settings.value("lineDamperColor").toString());
    p->lineIsoColor = TypicalColorTable::nameToColor(settings.value("lineIsoColor").toString());
    p->planeRcColor = TypicalColorTable::nameToColor(settings.value("planeRcColor").toString());
    p->planeSColor = TypicalColorTable::nameToColor(settings.value("planeSColor").toString());
    p->planeDamperColor = TypicalColorTable::nameToColor(settings.value("planeDamperColor").toString());
    p->supportColor = TypicalColorTable::nameToColor(settings.value("supportColor").toString());
    p->valueColor = TypicalColorTable::nameToColor(settings.value("valueColor").toString());
    p->valueTextSize = settings.value("valueTextSize").toInt();
    p->selectedColor = TypicalColorTable::nameToColor(settings.value("selectedColor").toString());
    p->noteColor = TypicalColorTable::nameToColor(settings.value("noteColor").toString());
    p->noteTextSize = settings.value("noteTextSize").toInt();
    p->plasticSymbolColor = TypicalColorTable::nameToColor(settings.value("plasticSymbolColor").toString());
    p->plasticSymbolSize = settings.value("plasticSymbolSize").toInt();
    p->ngColor = TypicalColorTable::nameToColor(settings.value("ngColor").toString());
    p->useGradiationColor = settings.value("useGradiationColor").toBool();
    settings.endGroup();
    return p;

}

void AnalysisCheck2DViewSetting::saveSettings(QSettings &settings) const
{
    settings.beginGroup("AnalysisCheck2DViewSetting");
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("scaleSize", scaleSize);
    settings.setValue("gridStringVisible", gridStringVisible);
    settings.setValue("floorStringVisible", floorStringVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("jointSize", jointSize);
    settings.setValue("lineRcColor", TypicalColorTable::colorToName(lineRcColor));
    settings.setValue("lineSrcColor", TypicalColorTable::colorToName(lineSrcColor));
    settings.setValue("lineSColor", TypicalColorTable::colorToName(lineSColor));
    settings.setValue("lineCftColor", TypicalColorTable::colorToName(lineCftColor));
    settings.setValue("lineDamperColor", TypicalColorTable::colorToName(lineDamperColor));
    settings.setValue("lineIsoColor", TypicalColorTable::colorToName(lineIsoColor));
    settings.setValue("planeRcColor", TypicalColorTable::colorToName(planeRcColor));
    settings.setValue("planeSColor", TypicalColorTable::colorToName(planeSColor));
    settings.setValue("planeDamperColor", TypicalColorTable::colorToName(planeDamperColor));
    settings.setValue("supportColor", TypicalColorTable::colorToName(supportColor));
    settings.setValue("valueColor", TypicalColorTable::colorToName(valueColor));
    settings.setValue("valueTextSize", valueTextSize);
    settings.setValue("selectedColor", TypicalColorTable::colorToName(selectedColor));
    settings.setValue("noteColor", TypicalColorTable::colorToName(noteColor));
    settings.setValue("noteTextSize", noteTextSize);
    settings.setValue("plasticSymbolColor", TypicalColorTable::colorToName(plasticSymbolColor));
    settings.setValue("plasticSymbolSize", plasticSymbolSize);
    settings.setValue("ngColor", TypicalColorTable::colorToName(ngColor));
    settings.setValue("useGradiationColor", useGradiationColor);

    settings.endGroup();

}

AnalysisStress2DViewSetting::AnalysisStress2DViewSetting(const AnalysisStress2DViewSetting &setting)
    :        AnalysisCommon2DViewSetting(setting.scaleColor,
                                       setting.scaleSize,
                                       setting.gridStringVisible,
                                       setting.floorStringVisible,
                                       setting.jointColor,
                                       setting.jointSize,
                                       setting.lineRcColor,
                                       setting.lineSrcColor,
                                       setting.lineSColor,
                                       setting.lineCftColor,
                                       setting.lineDamperColor,
                                       setting.lineIsoColor,
                                       setting.planeRcColor,
                                       setting.planeSColor,
                                       setting.planeDamperColor,
                                       setting.supportColor,
                                       setting.valueColor,
                                       setting.valueTextSize,
                                       setting.selectedColor,
                                       setting.noteColor,
                                       setting.noteTextSize,
                                       setting.plasticSymbolColor,
                                       setting.plasticSymbolSize
                                       )
{
    stressColor = setting.stressColor;
    stressScale = setting.stressScale;
    valueNVisible = setting.valueNVisible;
    valueMVisible = setting.valueMVisible;
    valueQVisible = setting.valueQVisible;
    columnStressVisible = setting.columnStressVisible;
    columnStressValueColor = setting.columnStressValueColor;
    girderStressVisible = setting.girderStressVisible;
    girderStressValueColor = setting.girderStressValueColor;
    braceStressVisible = setting.braceStressVisible;
    braceStressValueColor = setting.braceStressValueColor;
    wallStressVisible = setting.wallStressVisible;
    wallStressValueColor = setting.wallStressValueColor;
    slabStressVisible = setting.slabStressVisible;
    slabStressValueColor = setting.slabStressValueColor;
    panelStressVisible = setting.panelStressVisible;
    panelStressValueColor = setting.panelStressValueColor;
    springStressVisible = setting.springStressVisible;
    springStressValueColor = setting.springStressValueColor;
    damperStressVisible = setting.damperStressVisible;
    damperStressValueColor = setting.damperStressValueColor;
    isoStressVisible = setting.isoStressVisible;
    isoStressValueColor = setting.isoStressValueColor;
}

AnalysisStress2DViewSetting *AnalysisStress2DViewSetting::getDefault()
{
    return new AnalysisStress2DViewSetting(
                Qt::black,
                3,
                true,
                true,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                3,
                Qt::black,
                100.0,
                true,
                true,
                true,
                true,
                Qt::black,
                true,
                Qt::black,
                true,
                Qt::black,
                true,
                Qt::black,
                true,
                Qt::black,
                true,
                Qt::black,
                true,
                Qt::black,
                true,
                Qt::black,
                true,
                Qt::black
                );
}

AnalysisStress2DViewSetting *AnalysisStress2DViewSetting::readSettings(QSettings &settings)
{
    AnalysisStress2DViewSetting* p = new AnalysisStress2DViewSetting();
    settings.beginGroup("AnalysisStress2DViewSetting");
    p->scaleColor = TypicalColorTable::nameToColor(settings.value("scaleColor").toString());
    p->scaleSize = settings.value("scaleSize").toInt();
    p->gridStringVisible = settings.value("gridStringVisible").toBool();
    p->floorStringVisible = settings.value("floorStringVisible").toBool();
    p->jointColor = TypicalColorTable::nameToColor(settings.value("jointColor").toString());
    p->jointSize = settings.value("jointSize").toInt();
    p->lineRcColor = TypicalColorTable::nameToColor(settings.value("lineRcColor").toString());
    p->lineSrcColor = TypicalColorTable::nameToColor(settings.value("lineSrcColor").toString());
    p->lineSColor = TypicalColorTable::nameToColor(settings.value("lineSColor").toString());
    p->lineCftColor = TypicalColorTable::nameToColor(settings.value("lineCftColor").toString());
    p->lineDamperColor = TypicalColorTable::nameToColor(settings.value("lineDamperColor").toString());
    p->lineIsoColor = TypicalColorTable::nameToColor(settings.value("lineIsoColor").toString());
    p->planeRcColor = TypicalColorTable::nameToColor(settings.value("planeRcColor").toString());
    p->planeSColor = TypicalColorTable::nameToColor(settings.value("planeSColor").toString());
    p->planeDamperColor = TypicalColorTable::nameToColor(settings.value("planeDamperColor").toString());
    p->supportColor = TypicalColorTable::nameToColor(settings.value("supportColor").toString());
    p->valueColor = TypicalColorTable::nameToColor(settings.value("valueColor").toString());
    p->valueTextSize = settings.value("valueTextSize").toInt();
    p->selectedColor = TypicalColorTable::nameToColor(settings.value("selectedColor").toString());
    p->noteColor = TypicalColorTable::nameToColor(settings.value("noteColor").toString());
    p->noteTextSize = settings.value("noteTextSize").toInt();
    p->plasticSymbolColor = TypicalColorTable::nameToColor(settings.value("plasticSymbolColor").toString());
    p->plasticSymbolSize = settings.value("plasticSymbolSize").toInt();
    p->stressColor = TypicalColorTable::nameToColor(settings.value("stressColor").toString());
    p->stressScale = settings.value("stressScale").toDouble();
    p->valueNVisible = settings.value("valueNVisible").toBool();
    p->valueMVisible = settings.value("valueMVisible").toBool();
    p->valueQVisible = settings.value("valueQVisible").toBool();
    p->columnStressVisible = settings.value("columnStressVisible").toBool();
    p->columnStressValueColor = TypicalColorTable::nameToColor(settings.value("columnStressValueColor").toString());
    p->girderStressVisible = settings.value("girderStressVisible").toBool();
    p->girderStressValueColor = TypicalColorTable::nameToColor(settings.value("girderStressValueColor").toString());
    p->braceStressVisible = settings.value("braceStressVisible").toBool();
    p->braceStressValueColor = TypicalColorTable::nameToColor(settings.value("braceStressValueColor").toString());
    p->wallStressVisible = settings.value("wallStressVisible").toBool();
    p->wallStressValueColor = TypicalColorTable::nameToColor(settings.value("wallStressValueColor").toString());
    p->slabStressVisible = settings.value("slabStressVisible").toBool();
    p->slabStressValueColor = TypicalColorTable::nameToColor(settings.value("slabStressValueColor").toString());
    p->panelStressVisible = settings.value("panelStressVisible").toBool();
    p->panelStressValueColor = TypicalColorTable::nameToColor(settings.value("panelStressValueColor").toString());
    p->springStressVisible = settings.value("springStressVisible").toBool();
    p->springStressValueColor = TypicalColorTable::nameToColor(settings.value("springStressValueColor").toString());
    p->damperStressVisible = settings.value("damperStressVisible").toBool();
    p->damperStressValueColor = TypicalColorTable::nameToColor(settings.value("damperStressValueColor").toString());
    p->isoStressVisible = settings.value("isoStressVisible").toBool();
    p->isoStressValueColor = TypicalColorTable::nameToColor(settings.value("isoStressValueColor").toString());
    settings.endGroup();
    return p;
}

void AnalysisStress2DViewSetting::saveSettings(QSettings &settings) const
{
    settings.beginGroup("AnalysisStress2DViewSetting");
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("scaleSize", scaleSize);
    settings.setValue("gridStringVisible", gridStringVisible);
    settings.setValue("floorStringVisible", floorStringVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("jointSize", jointSize);
    settings.setValue("lineRcColor", TypicalColorTable::colorToName(lineRcColor));
    settings.setValue("lineSrcColor", TypicalColorTable::colorToName(lineSrcColor));
    settings.setValue("lineSColor", TypicalColorTable::colorToName(lineSColor));
    settings.setValue("lineCftColor", TypicalColorTable::colorToName(lineCftColor));
    settings.setValue("lineDamperColor", TypicalColorTable::colorToName(lineDamperColor));
    settings.setValue("lineIsoColor", TypicalColorTable::colorToName(lineIsoColor));
    settings.setValue("planeRcColor", TypicalColorTable::colorToName(planeRcColor));
    settings.setValue("planeSColor", TypicalColorTable::colorToName(planeSColor));
    settings.setValue("planeDamperColor", TypicalColorTable::colorToName(planeDamperColor));
    settings.setValue("supportColor", TypicalColorTable::colorToName(supportColor));
    settings.setValue("valueColor", TypicalColorTable::colorToName(valueColor));
    settings.setValue("valueTextSize", valueTextSize);
    settings.setValue("selectedColor", TypicalColorTable::colorToName(selectedColor));
    settings.setValue("noteColor", TypicalColorTable::colorToName(noteColor));
    settings.setValue("noteTextSize", noteTextSize);
    settings.setValue("plasticSymbolColor", TypicalColorTable::colorToName(plasticSymbolColor));
    settings.setValue("plasticSymbolSize", plasticSymbolSize);
    settings.setValue("stressColor", TypicalColorTable::colorToName(stressColor));
    settings.setValue("stressScale", stressScale);
    settings.setValue("valueNVisible", valueNVisible);
    settings.setValue("valueMVisible", valueMVisible);
    settings.setValue("valueQVisible", valueQVisible);
    settings.setValue("columnStressVisible", columnStressVisible);
    settings.setValue("columnStressValueColor", TypicalColorTable::colorToName(columnStressValueColor));
    settings.setValue("girderStressVisible", girderStressVisible);
    settings.setValue("girderStressValueColor", TypicalColorTable::colorToName(girderStressValueColor));
    settings.setValue("braceStressVisible", braceStressVisible);
    settings.setValue("braceStressValueColor", TypicalColorTable::colorToName(braceStressValueColor));
    settings.setValue("wallStressVisible", wallStressVisible);
    settings.setValue("wallStressValueColor", TypicalColorTable::colorToName(wallStressValueColor));
    settings.setValue("slabStressVisible", slabStressVisible);
    settings.setValue("slabStressValueColor", TypicalColorTable::colorToName(slabStressValueColor));
    settings.setValue("panelStressVisible", panelStressVisible);
    settings.setValue("panelStressValueColor", TypicalColorTable::colorToName(panelStressValueColor));
    settings.setValue("springStressVisible", springStressVisible);
    settings.setValue("springStressValueColor", TypicalColorTable::colorToName(springStressValueColor));
    settings.setValue("damperStressVisible", damperStressVisible);
    settings.setValue("damperStressValueColor", TypicalColorTable::colorToName(damperStressValueColor));
    settings.setValue("isoStressVisible", isoStressVisible);
    settings.setValue("isoStressValueColor", TypicalColorTable::colorToName(isoStressValueColor));

    settings.endGroup();

}

AnalysisCenterOfRigidity2DViewSetting::AnalysisCenterOfRigidity2DViewSetting(const AnalysisCenterOfRigidity2DViewSetting &setting)
    :        AnalysisCommon2DViewSetting(setting.scaleColor,
                                       setting.scaleSize,
                                       setting.gridStringVisible,
                                       setting.floorStringVisible,
                                       setting.jointColor,
                                       setting.jointSize,
                                       setting.lineRcColor,
                                       setting.lineSrcColor,
                                       setting.lineSColor,
                                       setting.lineCftColor,
                                       setting.lineDamperColor,
                                       setting.lineIsoColor,
                                       setting.planeRcColor,
                                       setting.planeSColor,
                                       setting.planeDamperColor,
                                       setting.supportColor,
                                       setting.valueColor,
                                       setting.valueTextSize,
                                       setting.selectedColor,
                                       setting.noteColor,
                                       setting.noteTextSize,
                                       setting.plasticSymbolColor,
                                       setting.plasticSymbolSize
                                       )
{
    shearForceColor = setting.shearForceColor;
    rigidityColor = setting.rigidityColor;
    rigiditySize = setting.rigiditySize;
}

AnalysisCenterOfRigidity2DViewSetting *AnalysisCenterOfRigidity2DViewSetting::getDefault()
{
    return new AnalysisCenterOfRigidity2DViewSetting(
                Qt::black,
                3,
                true,
                true,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                3,
                Qt::black,
                3,
                Qt::black,
                Qt::black,
                3
                );
}

AnalysisCenterOfRigidity2DViewSetting *AnalysisCenterOfRigidity2DViewSetting::readSettings(QSettings &settings)
{
    AnalysisCenterOfRigidity2DViewSetting* p = new AnalysisCenterOfRigidity2DViewSetting();
    settings.beginGroup("AnalysisGraphNmint2DViewSetting");
    p->scaleColor = TypicalColorTable::nameToColor(settings.value("scaleColor").toString());
    p->scaleSize = settings.value("scaleSize").toInt();
    p->gridStringVisible = settings.value("gridStringVisible").toBool();
    p->floorStringVisible = settings.value("floorStringVisible").toBool();
    p->jointColor = TypicalColorTable::nameToColor(settings.value("jointColor").toString());
    p->jointSize = settings.value("jointSize").toInt();
    p->lineRcColor = TypicalColorTable::nameToColor(settings.value("lineRcColor").toString());
    p->lineSrcColor = TypicalColorTable::nameToColor(settings.value("lineSrcColor").toString());
    p->lineSColor = TypicalColorTable::nameToColor(settings.value("lineSColor").toString());
    p->lineCftColor = TypicalColorTable::nameToColor(settings.value("lineCftColor").toString());
    p->lineDamperColor = TypicalColorTable::nameToColor(settings.value("lineDamperColor").toString());
    p->lineIsoColor = TypicalColorTable::nameToColor(settings.value("lineIsoColor").toString());
    p->planeRcColor = TypicalColorTable::nameToColor(settings.value("planeRcColor").toString());
    p->planeSColor = TypicalColorTable::nameToColor(settings.value("planeSColor").toString());
    p->planeDamperColor = TypicalColorTable::nameToColor(settings.value("planeDamperColor").toString());
    p->supportColor = TypicalColorTable::nameToColor(settings.value("supportColor").toString());
    p->valueColor = TypicalColorTable::nameToColor(settings.value("valueColor").toString());
    p->valueTextSize = settings.value("valueTextSize").toInt();
    p->selectedColor = TypicalColorTable::nameToColor(settings.value("selectedColor").toString());
    p->noteColor = TypicalColorTable::nameToColor(settings.value("noteColor").toString());
    p->noteTextSize = settings.value("noteTextSize").toInt();
    p->plasticSymbolColor = TypicalColorTable::nameToColor(settings.value("plasticSymbolColor").toString());
    p->plasticSymbolSize = settings.value("plasticSymbolSize").toInt();
    p->shearForceColor = TypicalColorTable::nameToColor(settings.value("shearForceColor").toString());
    p->rigidityColor = TypicalColorTable::nameToColor(settings.value("rigidityColor").toString());
    p->rigiditySize = settings.value("rigiditySize").toInt();
    settings.endGroup();
    return p;
}

void AnalysisCenterOfRigidity2DViewSetting::saveSettings(QSettings &settings) const
{
    settings.beginGroup("AnalysisCenterOfRigidity2DViewSetting");
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("scaleSize", scaleSize);
    settings.setValue("gridStringVisible", gridStringVisible);
    settings.setValue("floorStringVisible", floorStringVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("jointSize", jointSize);
    settings.setValue("lineRcColor", TypicalColorTable::colorToName(lineRcColor));
    settings.setValue("lineSrcColor", TypicalColorTable::colorToName(lineSrcColor));
    settings.setValue("lineSColor", TypicalColorTable::colorToName(lineSColor));
    settings.setValue("lineCftColor", TypicalColorTable::colorToName(lineCftColor));
    settings.setValue("lineDamperColor", TypicalColorTable::colorToName(lineDamperColor));
    settings.setValue("lineIsoColor", TypicalColorTable::colorToName(lineIsoColor));
    settings.setValue("planeRcColor", TypicalColorTable::colorToName(planeRcColor));
    settings.setValue("planeSColor", TypicalColorTable::colorToName(planeSColor));
    settings.setValue("planeDamperColor", TypicalColorTable::colorToName(planeDamperColor));
    settings.setValue("supportColor", TypicalColorTable::colorToName(supportColor));
    settings.setValue("valueColor", TypicalColorTable::colorToName(valueColor));
    settings.setValue("valueTextSize", valueTextSize);
    settings.setValue("selectedColor", TypicalColorTable::colorToName(selectedColor));
    settings.setValue("noteColor", TypicalColorTable::colorToName(noteColor));
    settings.setValue("noteTextSize", noteTextSize);
    settings.setValue("plasticSymbolColor", TypicalColorTable::colorToName(plasticSymbolColor));
    settings.setValue("plasticSymbolSize", plasticSymbolSize);
    settings.setValue("shearForceColor", TypicalColorTable::colorToName(shearForceColor));
    settings.setValue("rigidityColor", TypicalColorTable::colorToName(rigidityColor));
    settings.setValue("rigiditySize", rigiditySize);

    settings.endGroup();
}

AnalysisModel2DViewSetting::AnalysisModel2DViewSetting(const AnalysisModel2DViewSetting &setting)
{
    scaleColor = setting.scaleColor;
    scaleSize = setting.scaleSize;
    gridStringVisible = setting.gridStringVisible;
    floorStringVisible = setting.floorStringVisible;
    jointColor = setting.jointColor;
    jointSize = setting.jointSize;
    lineRcColor = setting.lineRcColor;
    lineSrcColor = setting.lineSrcColor;
    lineSColor = setting.lineSColor;
    lineCftColor = setting.lineCftColor;
    lineDamperColor = setting.lineDamperColor;
    lineIsoColor = setting.lineIsoColor;
    planeRcColor = setting.planeRcColor;
    planeSColor = setting.planeSColor;
    planeDamperColor = setting.planeDamperColor;
    supportColor = setting.supportColor;
    valueColor = setting.valueColor;
    valueTextSize = setting.valueTextSize;
    selectedColor = setting.selectedColor;
    noteColor = setting.noteColor;
    noteTextSize = setting.noteTextSize;
    columnMarkVisible = setting.columnMarkVisible;
    girderMarkVisible = setting.girderMarkVisible;
    braceMarkVisible = setting.braceMarkVisible;
    wallMarkVisible = setting.wallMarkVisible;
    steelWallMarkVisible = setting.steelWallMarkVisible;
    foundationWallMarkVisible = setting.foundationWallMarkVisible;
    slabMarkVisible = setting.slabMarkVisible;
    damperMarkVisible = setting.damperMarkVisible;
    isoMarkVisible = setting.isoMarkVisible;
    markSize = setting.markSize;
    jointNumberVisible = setting.jointNumberVisible;
    jointNumberColor = setting.jointNumberColor;
    jointNumberSize = setting.jointNumberSize;
    boundaryConditionVisible = setting.boundaryConditionVisible;
    boundaryConditionColor = setting.boundaryConditionColor;
    boundaryConditionSize = setting.boundaryConditionSize;
    directionVisible = setting.directionVisible;
    zwallColor = setting.zwallColor;
    rigidRangeColor = setting.rigidRangeColor;
    panelZoneColor = setting.panelZoneColor;
}

AnalysisModel2DViewSetting *AnalysisModel2DViewSetting::getDefault()
{
    auto p = new AnalysisModel2DViewSetting();
    p->scaleColor = Qt::black;
    p->scaleSize = 3;
    p->gridStringVisible = true;
    p->floorStringVisible = true;
    p->jointColor = Qt::black;
    p->jointSize = 3;
    p->lineRcColor = Qt::black;
    p->lineSrcColor = Qt::black;
    p->lineSColor = Qt::black;
    p->lineCftColor = Qt::black;
    p->lineDamperColor = Qt::black;
    p->lineIsoColor = Qt::black;
    p->planeRcColor = Qt::black;
    p->planeSColor = Qt::black;
    p->planeDamperColor = Qt::black;
    p->supportColor = Qt::black;
    p->valueColor = Qt::black;
    p->valueTextSize = 3;
    p->selectedColor = Qt::black;
    p->noteColor = Qt::black;
    p->noteTextSize = 3;
    p->columnMarkVisible = true;
    p->girderMarkVisible = true;
    p->braceMarkVisible = true;
    p->wallMarkVisible = true;
    p->steelWallMarkVisible = true;
    p->foundationWallMarkVisible = true;
    p->slabMarkVisible = true;
    p->damperMarkVisible = true;
    p->isoMarkVisible = true;
    p->markSize = 3;
    p->jointNumberVisible = true;
    p->jointNumberColor = Qt::black;
    p->jointNumberSize = 3;
    p->boundaryConditionVisible = true;
    p->boundaryConditionColor = Qt::black;
    p->boundaryConditionSize = 3;
    p->directionVisible = true;
    p->zwallColor = Qt::black;
    p->rigidRangeColor = Qt::black;
    p->panelZoneColor = Qt::black;
    return p;
}

AnalysisModel2DViewSetting *AnalysisModel2DViewSetting::readSettings(QSettings &settings)
{
    AnalysisModel2DViewSetting* p = new AnalysisModel2DViewSetting();
    settings.beginGroup("AnalysisModel2DViewSetting");
    p->scaleColor = TypicalColorTable::nameToColor(settings.value("scaleColor").toString());
    p->scaleSize = settings.value("scaleSize").toInt();
    p->gridStringVisible = settings.value("gridStringVisible").toBool();
    p->floorStringVisible = settings.value("floorStringVisible").toBool();
    p->jointColor = TypicalColorTable::nameToColor(settings.value("jointColor").toString());
    p->jointSize = settings.value("jointSize").toInt();
    p->lineRcColor = TypicalColorTable::nameToColor(settings.value("lineRcColor").toString());
    p->lineSrcColor = TypicalColorTable::nameToColor(settings.value("lineSrcColor").toString());
    p->lineSColor = TypicalColorTable::nameToColor(settings.value("lineSColor").toString());
    p->lineCftColor = TypicalColorTable::nameToColor(settings.value("lineCftColor").toString());
    p->lineDamperColor = TypicalColorTable::nameToColor(settings.value("lineDamperColor").toString());
    p->lineIsoColor = TypicalColorTable::nameToColor(settings.value("lineIsoColor").toString());
    p->planeRcColor = TypicalColorTable::nameToColor(settings.value("planeRcColor").toString());
    p->planeSColor = TypicalColorTable::nameToColor(settings.value("planeSColor").toString());
    p->planeDamperColor = TypicalColorTable::nameToColor(settings.value("planeDamperColor").toString());
    p->supportColor = TypicalColorTable::nameToColor(settings.value("supportColor").toString());
    p->valueColor = TypicalColorTable::nameToColor(settings.value("valueColor").toString());
    p->valueTextSize = settings.value("valueTextSize").toInt();
    p->selectedColor = TypicalColorTable::nameToColor(settings.value("selectedColor").toString());
    p->noteColor = TypicalColorTable::nameToColor(settings.value("noteColor").toString());
    p->noteTextSize = settings.value("noteTextSize").toInt();
    p->columnMarkVisible = settings.value("columnMarkVisible").toBool();
    p->girderMarkVisible = settings.value("girderMarkVisible").toBool();
    p->braceMarkVisible = settings.value("braceMarkVisible").toBool();
    p->wallMarkVisible = settings.value("wallMarkVisible").toBool();
    p->steelWallMarkVisible = settings.value("steelWallMarkVisible").toBool();
    p->foundationWallMarkVisible = settings.value("foundationWallMarkVisible").toBool();
    p->slabMarkVisible = settings.value("slabMarkVisible").toBool();
    p->damperMarkVisible = settings.value("damperMarkVisible").toBool();
    p->isoMarkVisible = settings.value("isoMarkVisible").toBool();
    p->markSize = settings.value("markSize").toInt();
    p->jointNumberVisible = settings.value("jointNumberVisible").toBool();
    p->jointNumberColor = TypicalColorTable::nameToColor(settings.value("jointNumberColor").toString());
    p->jointNumberSize = settings.value("jointNumberSize").toInt();
    p->boundaryConditionVisible = settings.value("boundaryConditionVisible").toBool();
    p->boundaryConditionColor = TypicalColorTable::nameToColor(settings.value("boundaryConditionColor").toString());
    p->boundaryConditionSize = settings.value("boundaryConditionSize").toInt();
    p->directionVisible = settings.value("directionVisible").toBool();
    p->zwallColor = TypicalColorTable::nameToColor(settings.value("zwallColor").toString());
    p->rigidRangeColor = TypicalColorTable::nameToColor(settings.value("rigidRangeColor").toString());
    p->panelZoneColor = TypicalColorTable::nameToColor(settings.value("panelZoneColor").toString());
    settings.endGroup();
    return p;
}

void AnalysisModel2DViewSetting::saveSettings(QSettings& settings) const
{
    settings.beginGroup("AnalysisModel2DViewSetting");
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("scaleSize", scaleSize);
    settings.setValue("gridStringVisible", gridStringVisible);
    settings.setValue("floorStringVisible", floorStringVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("jointSize", jointSize);
    settings.setValue("lineRcColor", TypicalColorTable::colorToName(lineRcColor));
    settings.setValue("lineSrcColor", TypicalColorTable::colorToName(lineSrcColor));
    settings.setValue("lineSColor", TypicalColorTable::colorToName(lineSColor));
    settings.setValue("lineCftColor", TypicalColorTable::colorToName(lineCftColor));
    settings.setValue("lineDamperColor", TypicalColorTable::colorToName(lineDamperColor));
    settings.setValue("lineIsoColor", TypicalColorTable::colorToName(lineIsoColor));
    settings.setValue("planeRcColor", TypicalColorTable::colorToName(planeRcColor));
    settings.setValue("planeSColor", TypicalColorTable::colorToName(planeSColor));
    settings.setValue("planeDamperColor", TypicalColorTable::colorToName(planeDamperColor));
    settings.setValue("supportColor", TypicalColorTable::colorToName(supportColor));
    settings.setValue("valueColor", TypicalColorTable::colorToName(valueColor));
    settings.setValue("valueTextSize", valueTextSize);
    settings.setValue("selectedColor", TypicalColorTable::colorToName(selectedColor));
    settings.setValue("noteColor", TypicalColorTable::colorToName(noteColor));
    settings.setValue("noteTextSize", noteTextSize);
    settings.setValue("columnMarkVisible", columnMarkVisible);
    settings.setValue("girderMarkVisible", girderMarkVisible);
    settings.setValue("braceMarkVisible", braceMarkVisible);
    settings.setValue("wallMarkVisible", wallMarkVisible);
    settings.setValue("steelWallMarkVisible", steelWallMarkVisible);
    settings.setValue("foundationWallMarkVisible", foundationWallMarkVisible);
    settings.setValue("slabMarkVisible", slabMarkVisible);
    settings.setValue("damperMarkVisible", damperMarkVisible);
    settings.setValue("isoMarkVisible", isoMarkVisible);
    settings.setValue("markSize", markSize);
    settings.setValue("jointNumberVisible", jointNumberVisible);
    settings.setValue("jointNumberColor", TypicalColorTable::colorToName(jointNumberColor));
    settings.setValue("jointNumberSize", jointNumberSize);
    settings.setValue("boundaryConditionVisible", boundaryConditionVisible);
    settings.setValue("boundaryConditionColor", TypicalColorTable::colorToName(boundaryConditionColor));
    settings.setValue("boundaryConditionSize", boundaryConditionSize);
    settings.setValue("directionVisible", directionVisible);
    settings.setValue("zwallColor", TypicalColorTable::colorToName(zwallColor));
    settings.setValue("rigidRangeColor", TypicalColorTable::colorToName(rigidRangeColor));
    settings.setValue("panelZoneColor", TypicalColorTable::colorToName(panelZoneColor));
    settings.endGroup();
}

AnalysisGraphQDelta2DViewSetting::AnalysisGraphQDelta2DViewSetting(const AnalysisGraphQDelta2DViewSetting &setting)
{
    titleVisible = setting.titleVisible;
    plasticSymbolColor = setting.plasticSymbolColor;
    plasticStepColor = setting.plasticStepColor;
    axisLabelSize = setting.axisLabelSize;
    axisScaleTextSize = setting.axisScaleTextSize;
    titleTextSize = setting.titleTextSize;
    symbolSize = setting.symbolSize;
    symbolLabelSize = setting.symbolLabelSize;
    minmax_auto_q = setting.minmax_auto_q;
    minmax_min_q = setting.minmax_min_q;
    minmax_max_q = setting.minmax_max_q;
    minmax_auto_d = setting.minmax_auto_d;
    minmax_min_d = setting.minmax_min_d;
    minmax_max_d = setting.minmax_max_d;
    minmax_auto_rad = setting.minmax_auto_rad;
    minmax_min_rad = setting.minmax_min_rad;
    minmax_max_rad = setting.minmax_max_rad;
    ruledLineVisible = setting.ruledLineVisible;
    ruledLineColor = setting.ruledLineColor;
    ruledLineStyle = setting.ruledLineStyle;
}

AnalysisGraphQDelta2DViewSetting *AnalysisGraphQDelta2DViewSetting::getDefault()
{
    auto p = new AnalysisGraphQDelta2DViewSetting(
                true,
                Qt::black,
                Qt::black,
                3,
                3,
                3,
                3,
                3,
                true,
                0,
                10000,
                true,
                0,
                20,
                true,
                0,
                0.05,
                true,
                Qt::black,
                Qt::SolidLine
                );
    return p;
}

AnalysisGraphQDelta2DViewSetting *AnalysisGraphQDelta2DViewSetting::readSettings(QSettings& settings)
{
    AnalysisGraphQDelta2DViewSetting* p = new AnalysisGraphQDelta2DViewSetting();
    settings.beginGroup("AnalysisGraphQDelta2DViewSetting");
    p->titleVisible = settings.value("titleVisible").toBool();
    p->plasticSymbolColor = TypicalColorTable::nameToColor(settings.value("plasticSymbolColor").toString());
    p->plasticStepColor = TypicalColorTable::nameToColor(settings.value("plasticStepColor").toString());
    p->axisLabelSize = settings.value("axisLabelSize").toInt();
    p->axisScaleTextSize = settings.value("axisScaleTextSize").toInt();
    p->titleTextSize = settings.value("titleTextSize").toInt();
    p->symbolSize = settings.value("symbolSize").toInt();
    p->symbolLabelSize = settings.value("symbolLabelSize").toInt();
    p->minmax_auto_q = settings.value("minmax_auto_q").toBool();
    p->minmax_min_q = settings.value("minmax_min_q").toDouble();
    p->minmax_max_q = settings.value("minmax_max_q").toDouble();
    p->minmax_auto_d = settings.value("minmax_auto_d").toBool();
    p->minmax_min_d = settings.value("minmax_min_d").toDouble();
    p->minmax_max_d = settings.value("minmax_max_d").toDouble();
    p->minmax_auto_rad = settings.value("minmax_auto_rad").toBool();
    p->minmax_min_rad = settings.value("minmax_min_rad").toDouble();
    p->minmax_max_rad = settings.value("minmax_max_rad").toDouble();
    p->ruledLineVisible = settings.value("ruledLineVisible").toBool();
    p->ruledLineColor = TypicalColorTable::nameToColor(settings.value("ruledLineColor").toString());
    p->ruledLineStyle = static_cast<Qt::PenStyle>(settings.value("ruledLineStyle").toInt());
    settings.endGroup();
    return p;
}

void AnalysisGraphQDelta2DViewSetting::saveSettings(QSettings &settings) const
{
    settings.beginGroup("AnalysisGraphQDelta2DViewSetting");
    settings.setValue("titleVisible", titleVisible);
    settings.setValue("plasticSymbolColor", TypicalColorTable::colorToName(plasticSymbolColor));
    settings.setValue("plasticStepColor", TypicalColorTable::colorToName(plasticStepColor));
    settings.setValue("axisLabelSize", axisLabelSize);
    settings.setValue("axisScaleTextSize", axisScaleTextSize);
    settings.setValue("titleTextSize", titleTextSize);
    settings.setValue("symbolSize", symbolSize);
    settings.setValue("symbolLabelSize", symbolLabelSize);
    settings.setValue("minmax_auto_q", minmax_auto_q);
    settings.setValue("minmax_min_q", minmax_min_q);
    settings.setValue("minmax_max_q", minmax_max_q);
    settings.setValue("minmax_auto_d", minmax_auto_d);
    settings.setValue("minmax_min_d", minmax_min_d);
    settings.setValue("minmax_max_d", minmax_max_d);
    settings.setValue("minmax_auto_rad", minmax_auto_rad);
    settings.setValue("minmax_min_rad", minmax_min_rad);
    settings.setValue("minmax_max_rad", minmax_max_rad);
    settings.setValue("ruledLineVisible", ruledLineVisible);
    settings.setValue("ruledLineColor", TypicalColorTable::colorToName(ruledLineColor));
    settings.setValue("ruledLineStyle", static_cast<int>(ruledLineStyle));
    settings.endGroup();
}


AnalysisGraphHorizontalCapacity2DViewSetting::AnalysisGraphHorizontalCapacity2DViewSetting(const AnalysisGraphHorizontalCapacity2DViewSetting &setting)
{
    titleVisible = setting.titleVisible;
    axisScaleTextSize = setting.axisScaleTextSize;
    axisLabelSize = setting.axisLabelSize;
    titleLabelSize = setting.titleLabelSize;
    minmax_auto_q = setting.minmax_auto_q;
    minmax_min_q = setting.minmax_min_q;
    minmax_max_q = setting.minmax_max_q;
    visible_u = setting.visible_u;
    color_u = setting.color_u;
    style_u = setting.style_u;
    visible_un = setting.visible_un;
    color_un = setting.color_un;
    style_un = setting.style_un;
    visible_ruler = setting.visible_ruler;
    color_ruler = setting.color_ruler;
    style_ruler = setting.style_ruler;
}

AnalysisGraphHorizontalCapacity2DViewSetting *AnalysisGraphHorizontalCapacity2DViewSetting::getDefault()
{
    auto p = new AnalysisGraphHorizontalCapacity2DViewSetting(
                true,
                3,
                3,
                3,
                true,
                0,
                10000,
                true,
                Qt::black,
                Qt::SolidLine,
                true,
                Qt::black,
                Qt::SolidLine,
                true,
                Qt::black,
                Qt::SolidLine
                );
    return p;
}

AnalysisGraphHorizontalCapacity2DViewSetting *AnalysisGraphHorizontalCapacity2DViewSetting::readSettings(QSettings &settings)
{
    AnalysisGraphHorizontalCapacity2DViewSetting* p = new AnalysisGraphHorizontalCapacity2DViewSetting();
    settings.beginGroup("AnalysisGraphHorizontalCapacity2DViewSetting");
    p->titleVisible = settings.value("titleVisible").toBool();
    p->axisScaleTextSize = settings.value("axisScaleTextSize").toInt();
    p->axisLabelSize = settings.value("axisLabelSize").toInt();
    p->titleLabelSize = settings.value("titleLabelSize").toInt();
    p->minmax_auto_q = settings.value("minmax_auto_q").toBool();
    p->minmax_min_q = settings.value("minmax_min_q").toDouble();
    p->minmax_max_q = settings.value("minmax_max_q").toDouble();
    p->visible_u = settings.value("visible_u").toBool();
    p->color_u = TypicalColorTable::nameToColor(settings.value("color_u").toString());
    p->style_u = static_cast<Qt::PenStyle>(settings.value("style_u").toInt());
    p->visible_un = settings.value("visible_un").toBool();
    p->color_un = TypicalColorTable::nameToColor(settings.value("color_un").toString());
    p->style_un = static_cast<Qt::PenStyle>(settings.value("style_un").toInt());
    p->visible_ruler = settings.value("visible_ruler").toBool();
    p->color_ruler = TypicalColorTable::nameToColor(settings.value("color_ruler").toString());
    p->style_ruler = static_cast<Qt::PenStyle>(settings.value("style_ruler").toInt());
    settings.endGroup();
    return p;
}

void AnalysisGraphHorizontalCapacity2DViewSetting::saveSettings(QSettings &settings) const
{
    settings.beginGroup("AnalysisGraphHorizontalCapacity2DViewSetting");
    settings.setValue("titleVisible", titleVisible);
    settings.setValue("axisScaleTextSize", axisScaleTextSize);
    settings.setValue("axisLabelSize", axisLabelSize);
    settings.setValue("titleLabelSize", titleLabelSize);
    settings.setValue("minmax_auto_q", minmax_auto_q);
    settings.setValue("minmax_min_q", minmax_min_q);
    settings.setValue("minmax_max_q", minmax_max_q);
    settings.setValue("visible_u", visible_u);
    settings.setValue("color_u", TypicalColorTable::colorToName(color_u));
    settings.setValue("style_u", static_cast<int>(style_u));
    settings.setValue("visible_un", visible_un);
    settings.setValue("color_un", TypicalColorTable::colorToName(color_un));
    settings.setValue("style_un", static_cast<int>(style_un));
    settings.setValue("visible_ruler", visible_ruler);
    settings.setValue("color_ruler", TypicalColorTable::colorToName(color_ruler));
    settings.setValue("style_ruler", static_cast<int>(style_ruler));
    settings.endGroup();
}

AnalysisGraphNmint2DViewSetting::AnalysisGraphNmint2DViewSetting(const AnalysisGraphNmint2DViewSetting &setting)
{
    titleVisible = setting.titleVisible;
    axisColor = setting.axisColor;
    axisTextColor = setting.axisTextColor;
    symbolColor = setting.symbolColor;
    axisLabelSize = setting.axisLabelSize;
    axisScaleTextSize = setting.axisScaleTextSize;
    titleTextSize = setting.titleTextSize;
    symbolSize = setting.symbolSize;
    symbolLabelSize = setting.symbolLabelSize;
    minmaxAuto_N = setting.minmaxAuto_N;
    minNValue = setting.minNValue;
    maxNValue = setting.maxNValue;
    minmaxAuto_My = setting.minmaxAuto_My;
    minMyValue = setting.minMyValue;
    maxMyValue = setting.maxMyValue;
    minmaxAuto_Mz = setting.minmaxAuto_Mz;
    minMzValue = setting.minMzValue;
    maxMzValue = setting.maxMzValue;
    outputLVisible = setting.outputLVisible;
    outputLColor = setting.outputLColor;
    outputLStyle = setting.outputLStyle;
    outputSVisible = setting.outputSVisible;
    outputSColor = setting.outputSColor;
    outputSStyle = setting.outputSStyle;
    outputUVisible  = setting.outputUVisible;
    outputUColor = setting.outputUColor;
    outputUStyle = setting.outputUStyle;
    koteiVisible = setting.koteiVisible;
    koteiColor = setting.koteiColor;
    koteiStyle = setting.koteiStyle;
    hendouVisible = setting.hendouVisible;
    hendouColor = setting.hendouColor;
    hendouStyle = setting.hendouStyle;
    rulerVisible = setting.rulerVisible;
    rulerColor = setting.rulerColor;
    rulerStyle = setting.rulerStyle;
}

AnalysisGraphNmint2DViewSetting* AnalysisGraphNmint2DViewSetting::getDefault()
{
    auto p = new AnalysisGraphNmint2DViewSetting(
                true,
                Qt::black,
                Qt::black,
                Qt::black,
                3,
                3,
                3,
                3,
                3,
                true,
                -10000,
                10000,
                true,
                -1000,
                1000,
                true,
                -1000,
                1000,
                true,
                Qt::black,
                Qt::SolidLine,
                true,
                Qt::black,
                Qt::SolidLine,
                true,
                Qt::black,
                Qt::SolidLine,
                true,
                Qt::black,
                Qt::SolidLine,
                true,
                Qt::black,
                Qt::SolidLine,
                true,
                Qt::darkGray,
                Qt::SolidLine
                );
    return p;
}

AnalysisGraphNmint2DViewSetting* AnalysisGraphNmint2DViewSetting::readSettings(QSettings &settings)
{
    AnalysisGraphNmint2DViewSetting* p = new AnalysisGraphNmint2DViewSetting();
    settings.beginGroup("AnalysisGraphNmint2DViewSetting");
    p->titleVisible = settings.value("titleVisible").toBool();
    p->axisColor = TypicalColorTable::nameToColor(settings.value("axisColor").toString());
    p->axisTextColor = TypicalColorTable::nameToColor(settings.value("axisTextColor").toString());
    p->symbolColor = TypicalColorTable::nameToColor(settings.value("symbolColor").toString());
    p->axisLabelSize = settings.value("axisLabelSize").toInt();
    p->axisScaleTextSize = settings.value("axisScaleTextSize").toInt();
    p->titleTextSize = settings.value("titleTextSize").toInt();
    p->symbolSize = settings.value("symbolSize").toInt();
    p->symbolLabelSize = settings.value("symbolLabelSize").toInt();
    p->minmaxAuto_N = settings.value("minmaxAuto_N").toBool();
    p->minNValue = settings.value("minNValue").toDouble();
    p->maxNValue = settings.value("maxNValue").toDouble();
    p->minmaxAuto_My = settings.value("minmaxAuto_My").toBool();
    p->minMyValue = settings.value("minMyValue").toDouble();
    p->maxMyValue = settings.value("maxMyValue").toDouble();
    p->minmaxAuto_Mz = settings.value("minmaxAuto_Mz").toBool();
    p->minMzValue = settings.value("minMzValue").toDouble();
    p->maxMzValue = settings.value("maxMzValue").toDouble();
    p->outputLVisible = settings.value("outputLVisible").toBool();
    p->outputLColor = TypicalColorTable::nameToColor(settings.value("outputLColor").toString());
    p->outputLStyle = static_cast<Qt::PenStyle>(settings.value("titleVisible").toInt());
    p->outputSVisible = settings.value("outputSVisible").toBool();
    p->outputSColor = TypicalColorTable::nameToColor(settings.value("outputSColor").toString());
    p->outputSStyle = static_cast<Qt::PenStyle>(settings.value("outputSStyle").toInt());
    p->outputUVisible = settings.value("outputUVisible").toBool();
    p->outputUColor = TypicalColorTable::nameToColor(settings.value("outputUColor").toString());
    p->outputUStyle = static_cast<Qt::PenStyle>(settings.value("outputUStyle").toInt());
    p->koteiVisible = settings.value("koteiVisible").toBool();
    p->koteiColor = TypicalColorTable::nameToColor(settings.value("koteiColor").toString());
    p->koteiStyle = static_cast<Qt::PenStyle>(settings.value("koteiStyle").toInt());
    p->hendouVisible = settings.value("hendouVisible").toBool();
    p->hendouColor = TypicalColorTable::nameToColor(settings.value("hendouColor").toString());
    p->hendouStyle = static_cast<Qt::PenStyle>(settings.value("hendouStyle").toInt());
    p->rulerVisible = settings.value("rulerVisible").toBool();
    p->rulerColor = TypicalColorTable::nameToColor(settings.value("rulerColor").toString());
    p->rulerStyle = static_cast<Qt::PenStyle>(settings.value("rulerStyle").toInt());
    settings.endGroup();
    return p;
}

void AnalysisGraphNmint2DViewSetting::saveSettings(QSettings &settings) const
{
    settings.beginGroup("AnalysisGraphNmint2DViewSetting");
    settings.setValue("titleVisible", titleVisible);
    settings.setValue("axisColor", TypicalColorTable::colorToName(axisColor));
    settings.setValue("axisTextColor", TypicalColorTable::colorToName(axisTextColor));
    settings.setValue("symbolColor", TypicalColorTable::colorToName(symbolColor));
    settings.setValue("axisLabelSize", axisLabelSize);
    settings.setValue("axisScaleTextSize", axisScaleTextSize);
    settings.setValue("titleTextSize", titleTextSize);
    settings.setValue("symbolSize", symbolSize);
    settings.setValue("symbolLabelSize", symbolLabelSize);
    settings.setValue("minmaxAuto_N", minmaxAuto_N);
    settings.setValue("minNValue", minNValue);
    settings.setValue("maxNValue", maxNValue);
    settings.setValue("minmaxAuto_My", minmaxAuto_My);
    settings.setValue("minMyValue", minMyValue);
    settings.setValue("maxMyValue", maxMyValue);
    settings.setValue("minmaxAuto_Mz",minmaxAuto_Mz);
    settings.setValue("minMzValue",minMzValue);

    settings.setValue("maxMzValue", maxMzValue);
    settings.setValue("outputLVisible", outputLVisible);
    settings.setValue("outputLColor", TypicalColorTable::colorToName(outputLColor));
    settings.setValue("outputLStyle", static_cast<int>(outputLStyle));
    settings.setValue("outputSVisible", outputSVisible);
    settings.setValue("outputSColor", TypicalColorTable::colorToName(outputSColor));
    settings.setValue("outputSStyle", static_cast<int>(outputSStyle));
    settings.setValue("outputUVisible", outputUVisible);
    settings.setValue("outputUColor", TypicalColorTable::colorToName(outputUColor));
    settings.setValue("outputUStyle", static_cast<int>(outputUStyle));
    settings.setValue("koteiVisible", koteiVisible);
    settings.setValue("koteiColor", TypicalColorTable::colorToName(koteiColor));
    settings.setValue("koteiStyle", static_cast<int>(koteiStyle));
    settings.setValue("hendouVisible", hendouVisible);
    settings.setValue("hendouColor", TypicalColorTable::colorToName(hendouColor));
    settings.setValue("hendouStyle", static_cast<int>(hendouStyle));
    settings.setValue("rulerVisible", rulerVisible);
    settings.setValue("rulerColor", TypicalColorTable::colorToName(rulerColor));
    settings.setValue("rulerStyle", static_cast<int>(rulerStyle));
    settings.endGroup();

}

Analysis2DViewSettings::Analysis2DViewSettings(const Analysis2DViewSettings &setting)
{
    settings = setting.settings;
    useScaledFont = setting.useScaledFont;
}

Analysis2DViewSettings Analysis2DViewSettings::getDefault()
{
    auto ret = Analysis2DViewSettings();
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::MODEL), AnalysisModel2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::LENGTH), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::FACELENGTH), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::COORDINATE), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::RIGIDLENGTH), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::LATERALBUCKLEN), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::EULERBUCKLEN), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::JOINTMASS), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::OUTERFORCE), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::OUTERMOMENT), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::RIGIDITYPHI), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::CMQ), AnalysisStress2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::AXIALMAP), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::REACTMAP), AnalysisReactionForce2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::COMBIREACTMAP), AnalysisReactionForce2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::SHEARMAP), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::VERTICALLOADMAP), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::LAYERDEFORMMAP), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::LAYERSTIFFMAP), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::GCKCP), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::NSTRESS), AnalysisStress2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::FSTRESS), AnalysisStress2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::RSTRESS), AnalysisStress2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::AXIAL), AnalysisAxialForce2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::TORSION), AnalysisStress2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::DEFORM), AnalysisDeformation2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::DISP), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::CHECKRATIO), AnalysisCheck2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::ALLOWRATIOE), AnalysisCheck2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::ALLOWRATIOU), AnalysisCheck2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::PLASTICRATIO), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::RANK), AnalysisStress2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::ALLOW), AnalysisStress2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::STRENGTH), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::YIELDSTEP), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::YIELDLOADMAG), AnalysisCommon2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::CenterOfRigidity), AnalysisCenterOfRigidity2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QDelta), AnalysisGraphQDelta2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QDeltaFrameFloors), AnalysisGraphQDelta2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QDeltaFloorFrames), AnalysisGraphQDelta2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QTheta), AnalysisGraphQDelta2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QThetaFrameFloors), AnalysisGraphQDelta2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QThetaFloorFrames), AnalysisGraphQDelta2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QuNeedf), AnalysisGraphHorizontalCapacity2DViewSetting::getDefault());
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::NMINT), AnalysisGraphNmint2DViewSetting::getDefault());
    ret.useScaledFont = true;
    return ret;
}

Analysis2DViewSettings Analysis2DViewSettings::readSettings(const QString &filepath)
{
    auto ret = Analysis2DViewSettings();
    QSettings settings(filepath, QSettings::IniFormat);
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::MODEL), AnalysisModel2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::LENGTH), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::FACELENGTH), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::COORDINATE), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::RIGIDLENGTH), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::LATERALBUCKLEN), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::EULERBUCKLEN), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::JOINTMASS), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::OUTERFORCE), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::OUTERMOMENT), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::RIGIDITYPHI), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::CMQ), AnalysisStress2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::AXIALMAP), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::REACTMAP), AnalysisReactionForce2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::COMBIREACTMAP), AnalysisReactionForce2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::SHEARMAP), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::VERTICALLOADMAP), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::LAYERDEFORMMAP), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::LAYERSTIFFMAP), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::GCKCP), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::NSTRESS), AnalysisStress2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::FSTRESS), AnalysisStress2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::RSTRESS), AnalysisStress2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::AXIAL), AnalysisAxialForce2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::TORSION), AnalysisStress2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::DEFORM), AnalysisDeformation2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::DISP), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::CHECKRATIO), AnalysisCheck2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::ALLOWRATIOE), AnalysisCheck2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::ALLOWRATIOU), AnalysisCheck2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::PLASTICRATIO), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::RANK), AnalysisStress2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::ALLOW), AnalysisStress2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::STRENGTH), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::YIELDSTEP), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::YIELDLOADMAG), AnalysisCommon2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::CenterOfRigidity), AnalysisCenterOfRigidity2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QDelta), AnalysisGraphQDelta2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QDeltaFrameFloors), AnalysisGraphQDelta2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QDeltaFloorFrames), AnalysisGraphQDelta2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QTheta), AnalysisGraphQDelta2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QThetaFrameFloors), AnalysisGraphQDelta2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QThetaFloorFrames), AnalysisGraphQDelta2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::QuNeedf), AnalysisGraphHorizontalCapacity2DViewSetting::readSettings(settings));
    ret.settings.insert(static_cast<int>(OUTFIGTYPE::NMINT), AnalysisGraphNmint2DViewSetting::readSettings(settings));

    settings.beginGroup("Analysis2DViewSettings");
    ret.useScaledFont = settings.value("useScaledFont").toBool();
    settings.endGroup();
    return ret;
}

void Analysis2DViewSettings::saveSettings(const QString &filepath)
{
    QSettings qSettings(filepath, QSettings::IniFormat);
    qSettings.beginGroup("Analysis2DViewSettings");
    qSettings.setValue("useScaledFont", useScaledFont);
    qSettings.endGroup();

    auto keys = settings.keys();
    foreach(auto key, keys){
        settings.value(key)->saveSettings(qSettings);
    }

}

AnalysisBaseViewSettings *post3dapp::Analysis2DViewSettings::getSetting(OUTFIGTYPE type) const
{
    return settings.value(static_cast<int>(type));
}

void Analysis2DViewSettings::setSetting(OUTFIGTYPE type, AnalysisBaseViewSettings *viewSetting)
{
    settings.insert(static_cast<int>(type), viewSetting);
}

void Analysis2DViewSettings::SetUseScaledFont(bool _scaledFont)
{
    useScaledFont = _scaledFont;
}










}
