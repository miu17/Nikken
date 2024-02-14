#include "define_calculationload2dsettings.h"

#include "typicalcolor_table.h"

namespace post3dapp{

CalculationLoad2DViewSettings::CalculationLoad2DViewSettings(const CalculationLoad2DViewSettings &setting)
{
    isDirectionVisible = setting.isDirectionVisible;
    jointColor = setting.jointColor;
    isJointStringVisible = setting.isJointStringVisible;
    jointSize = setting.jointSize;
    lineColor = setting.lineColor;
    isLineStringVisible = setting.isLineStringVisible;
    lineSize = setting.lineSize;
    planeColor = setting.planeColor;
    isPlaneStringVisible = setting.isPlaneStringVisible;
    planeSize = setting.planeSize;
    selectColor = setting.selectColor;
    scaleColor = setting.scaleColor;
    stringSize = setting.stringSize;
    isJloadVisible = setting.isJloadVisible;
    jloadColor = setting.jloadColor;
    jloadSize = setting.jloadSize;
    isPloadVisible = setting.isPloadVisible;
    ploadColor = setting.ploadColor;
    ploadSize = setting.ploadSize;
    isWloadVisible = setting.isWloadVisible;
    wloadColor = setting.wloadColor;
    wloadSize = setting.wloadSize;
    isDloadVisible = setting.isDloadVisible;
    dloadColor = setting.dloadColor;
    dloadSize = setting.dloadSize;
    useScaledFont = setting.useScaledFont;
}

CalculationLoad2DViewSettings CalculationLoad2DViewSettings::getDefault()
{
    return CalculationLoad2DViewSettings(
                true,
                QColorConstants::Svg::black,
                false,
                2,
                QColorConstants::Svg::black,
                true,
                2,
                QColorConstants::Svg::black,
                true,
                2,
                QColorConstants::Svg::lime,
                QColorConstants::Svg::dimgray,
                2,
                true,
                QColorConstants::Svg::navy,
                2,
                true,
                QColorConstants::Svg::olive,
                2,
                true,
                QColorConstants::Svg::mediumvioletred,
                10.0,
                true,
                QColorConstants::Svg::dodgerblue,
                10.0,
                true);
}

post3dapp::CalculationLoad2DViewSettings post3dapp::CalculationLoad2DViewSettings::readSettings(const QString &filepath)
{
    QSettings settings(filepath, QSettings::IniFormat);
    settings.beginGroup("CalculationLoad2DViewSettings");
    CalculationLoad2DViewSettings setting(
                settings.value("isDirectionVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("jointColor").toString()),
                settings.value("isJointStringVisible").toBool(),
                settings.value("jointSize").toInt(),
                TypicalColorTable::nameToColor(settings.value("lineColor").toString()),
                settings.value("isLineStringVisible").toBool(),
                settings.value("lineSize").toInt(),
                TypicalColorTable::nameToColor(settings.value("planeColor").toString()),
                settings.value("isPlaneStringVisible").toBool(),
                settings.value("planeSize").toInt(),
                TypicalColorTable::nameToColor(settings.value("selectColor").toString()),
                TypicalColorTable::nameToColor(settings.value("scaleColor").toString()),
                settings.value("stringSize").toInt(),
                settings.value("isJloadVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("jloadColor").toString()),
                settings.value("jloadSize").toInt(),
                settings.value("isPloadVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("ploadColor").toString()),
                settings.value("ploadSize").toInt(),
                settings.value("isWloadVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("wloadColor").toString()),
                settings.value("wloadSize").toReal(),
                settings.value("isDloadVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("dloadColor").toString()),
                settings.value("dloadSize").toReal(),
                settings.value("useScaledFont").toBool()
                );
    settings.endGroup();
    return setting;
}

void CalculationLoad2DViewSettings::saveSettings(const QString &filepath) const
{
    QSettings settings(filepath, QSettings::IniFormat);
    settings.beginGroup("CalculationLoad2DViewSettings");
    settings.setValue("isDirectionVisible", isDirectionVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("isJointStringVisible", isJointStringVisible);
    settings.setValue("jointSize", jointSize);
    settings.setValue("lineColor", TypicalColorTable::colorToName(lineColor));
    settings.setValue("isLineStringVisible", isLineStringVisible);
    settings.setValue("lineSize", lineSize);
    settings.setValue("planeColor", TypicalColorTable::colorToName(planeColor));
    settings.setValue("isPlaneStringVisible", isPlaneStringVisible);
    settings.setValue("planeSize", planeSize);
    settings.setValue("selectColor", TypicalColorTable::colorToName(selectColor));
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("stringSize", stringSize);
    settings.setValue("isJloadVisible", isJloadVisible);
    settings.setValue("jloadColor", TypicalColorTable::colorToName(jloadColor));
    settings.setValue("jloadSize", jloadSize);
    settings.setValue("isPloadVisible", isPloadVisible);
    settings.setValue("ploadColor", TypicalColorTable::colorToName(ploadColor));
    settings.setValue("ploadSize", ploadSize);
    settings.setValue("isWloadVisible", isWloadVisible);
    settings.setValue("wloadColor", TypicalColorTable::colorToName(wloadColor));
    settings.setValue("wloadSize", wloadSize);
    settings.setValue("isDloadVisible", isDloadVisible);
    settings.setValue("dloadColor", TypicalColorTable::colorToName(dloadColor));
    settings.setValue("dloadSize", dloadSize);
    settings.setValue("useScaledFont", useScaledFont);
    settings.endGroup();
}

void CalculationLoad2DViewSettings::SetUseScaledFont(bool _useScaledFont)
{
    useScaledFont = _useScaledFont;
}

} // namespace post3dapp
