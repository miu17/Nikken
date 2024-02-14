#include "define_calculationload3dsettings.h"

#include "typicalcolor_table.h"
namespace post3dapp{

CalculationLoad3DViewSettings::CalculationLoad3DViewSettings(const CalculationLoad3DViewSettings &setting)
{
    isJointVisible = setting.isJointVisible;
    jointColor = setting.jointColor;
    isLineVisible = setting.isLineVisible;
    lineColor = setting.lineColor;
    isPlaneVisible = setting.isPlaneVisible;
    planeColor = setting.planeColor;
    isJnoVisible = setting.isJnoVisible;
    jnoColor = setting.jnoColor;
    jnoSize = setting.jnoSize;
    isNameVisible = setting.isNameVisible;
    nameColor = setting.nameColor;
    nameSize = setting.nameSize;
    isColorBarVisible = setting.isColorBarVisible;
    colorBarTextColor = setting.colorBarTextColor;
    colorBarTextSize = setting.colorBarTextSize;
    valueAlpha = setting.valueAlpha;
    selectColor = setting.selectColor;
    scaleColor = setting.scaleColor;
    isFrScaleVisible = setting.isFrScaleVisible;
    frScaleFloor = setting.frScaleFloor;
    isFlScaleVisible = setting.isFlScaleVisible;
    floorScaleXY = setting.floorScaleXY;
    jloadSize = setting.jloadSize;
    mloadSize = setting.mloadSize;
    isJloadVisible = setting.isJloadVisible;
    jloadMinValue = setting.jloadMinValue;
    jloadMinColor = setting.jloadMinColor;
    jloadMaxColor = setting.jloadMaxColor;
    jloadMaxValue = setting.jloadMaxValue;
    isPloadVisible = setting.isPloadVisible;
    ploadMinValue = setting.ploadMinValue;
    ploadMinColor = setting.ploadMinColor;
    ploadMaxColor = setting.ploadMaxColor;
    ploadMaxValue = setting.ploadMaxValue;
    isWloadVisible = setting.isWloadVisible;
    wloadMinValue = setting.wloadMinValue;
    wloadMinColor = setting.wloadMinColor;
    wloadMaxColor = setting.wloadMaxColor;
    wloadMaxValue = setting.wloadMaxValue;
    isDloadVisible = setting.isDloadVisible;
    dloadMinValue = setting.dloadMinValue;
    dloadMinColor = setting.dloadMinColor;
    dloadMaxColor = setting.dloadMaxColor;
    dloadMaxValue = setting.dloadMaxValue;
    useScaledFont = setting.useScaledFont;
}

CalculationLoad3DViewSettings CalculationLoad3DViewSettings::getDefault()
{
    return CalculationLoad3DViewSettings(
                true,
                QColorConstants::Svg::mediumblue,
                true,
                QColorConstants::Svg::mediumblue,
                false,
                QColorConstants::Svg::navy,
                false,
                QColorConstants::Svg::white,
                1,
                true,
                QColorConstants::Svg::darkorange,
                2,
                true,
                QColorConstants::Svg::forestgreen,
                2,
                200,
                QColorConstants::Svg::lime,
                QColorConstants::Svg::olivedrab,
                true,
                QUuid(),
                true,
                QPointF(-5.0, -5.0),
                2,
                10.0,
                true,
                0.0,
                QColorConstants::Svg::darkred,
                QColorConstants::Svg::gold,
                100.0,
                true,
                0.0,
                QColorConstants::Svg::darkred,
                QColorConstants::Svg::gold,
                100.0,
                true,
                0.0,
                QColorConstants::Svg::purple,
                QColorConstants::Svg::aquamarine,
                50.0,
                true,
                0.0,
                QColorConstants::Svg::darkred,
                QColorConstants::Svg::gold,
                50.0,
                true
                );
}

CalculationLoad3DViewSettings CalculationLoad3DViewSettings::readSettings(const QString &filepath)
{
    QSettings settings(filepath, QSettings::IniFormat);
    settings.beginGroup("CalculationLoad3DViewSettings");
    CalculationLoad3DViewSettings setting(
                settings.value("isJointVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("jointColor").toString()),
                settings.value("isLineVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("lineColor").toString()),
                settings.value("isPlaneVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("planeColor").toString()),
                settings.value("isJnoVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("jnoColor").toString()),
                settings.value("jnoSize").toInt(),
                settings.value("isNameVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("nameColor").toString()),
                settings.value("nameSize").toInt(),
                settings.value("isColorBarVisible").toBool(),
                TypicalColorTable::nameToColor(settings.value("colorBarTextColor").toString()),
                settings.value("colorBarTextSize").toInt(),
                settings.value("valueAlpha").toInt(),
                TypicalColorTable::nameToColor(settings.value("selectColor").toString()),
                TypicalColorTable::nameToColor(settings.value("scaleColor").toString()),
                settings.value("isFrScaleVisible").toBool(),
                QUuid(settings.value("frScaleFloor").toString()),
                settings.value("isFlScaleVisible").toBool(),
                QPointF(settings.value("floorScaleX").toDouble(),settings.value("floorScaleY").toDouble()),
                settings.value("jloadSize").toInt(),
                settings.value("mloadSize").toReal(),
                settings.value("isJloadVisible").toBool(),
                settings.value("jloadMinValue").toReal(),
                TypicalColorTable::nameToColor(settings.value("jloadMinColor").toString()),
                TypicalColorTable::nameToColor(settings.value("jloadMaxColor").toString()),
                settings.value("jloadMaxValue").toReal(),
                settings.value("isPloadVisible").toBool(),
                settings.value("ploadMinValue").toReal(),
                TypicalColorTable::nameToColor(settings.value("ploadMinColor").toString()),
                TypicalColorTable::nameToColor(settings.value("ploadMaxColor").toString()),
                settings.value("ploadMaxValue").toReal(),
                settings.value("isWloadVisible").toBool(),
                settings.value("wloadMinValue").toReal(),
                TypicalColorTable::nameToColor(settings.value("wloadMinColor").toString()),
                TypicalColorTable::nameToColor(settings.value("wloadMaxColor").toString()),
                settings.value("wloadMaxValue").toReal(),
                settings.value("isDloadVisible").toBool(),
                settings.value("dloadMinValue").toReal(),
                TypicalColorTable::nameToColor(settings.value("dloadMinColor").toString()),
                TypicalColorTable::nameToColor(settings.value("dloadMaxColor").toString()),
                settings.value("dloadMaxValue").toReal(),
                settings.value("useScaledFont").toBool()
                );
    settings.endGroup();
    return setting;

}

void CalculationLoad3DViewSettings::saveSettings(const QString &filepath) const
{
    QSettings settings(filepath, QSettings::IniFormat);
    settings.beginGroup("CalculationLoad3DViewSettings");
    settings.setValue("isJointVisible", isJointVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("isLineVisible", isLineVisible);
    settings.setValue("lineColor", TypicalColorTable::colorToName(lineColor));
    settings.setValue("isPlaneVisible", isPlaneVisible);
    settings.setValue("planeColor", TypicalColorTable::colorToName(planeColor));
    settings.setValue("isJnoVisible", isJnoVisible);
    settings.setValue("jnoColor", TypicalColorTable::colorToName(jnoColor));
    settings.setValue("jnoSize", jnoSize);
    settings.setValue("isNameVisible", isNameVisible);
    settings.setValue("nameColor", TypicalColorTable::colorToName(nameColor));
    settings.setValue("nameSize", nameSize);
    settings.setValue("isColorBarVisible", isColorBarVisible);
    settings.setValue("colorBarTextColor", TypicalColorTable::colorToName(colorBarTextColor));
    settings.setValue("colorBarTextSize", colorBarTextSize);
    settings.setValue("valueAlpha", valueAlpha);
    settings.setValue("selectColor", TypicalColorTable::colorToName(selectColor));
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("isFrScaleVisible", isFrScaleVisible);
    settings.setValue("frScaleFloor", frScaleFloor.toString());
    settings.setValue("isFlScaleVisible", isFlScaleVisible);
    settings.setValue("floorScaleX", floorScaleXY.x());
    settings.setValue("floorScaleY", floorScaleXY.y());
    settings.setValue("jloadSize", jloadSize);
    settings.setValue("mloadSize", mloadSize);
    settings.setValue("isJloadVisible", isJloadVisible);
    settings.setValue("jloadMinValue", jloadMinValue);
    settings.setValue("jloadMinColor", TypicalColorTable::colorToName(jloadMinColor));
    settings.setValue("jloadMaxColor", TypicalColorTable::colorToName(jloadMaxColor));
    settings.setValue("jloadMaxValue", jloadMaxValue);
    settings.setValue("isPloadVisible", isPloadVisible);
    settings.setValue("ploadMinValue", ploadMinValue);
    settings.setValue("ploadMinColor", TypicalColorTable::colorToName(ploadMinColor));
    settings.setValue("ploadMaxColor", TypicalColorTable::colorToName(ploadMaxColor));
    settings.setValue("ploadMaxValue", ploadMaxValue);
    settings.setValue("isWloadVisible", isWloadVisible);
    settings.setValue("wloadMinValue", wloadMinValue);
    settings.setValue("wloadMinColor", TypicalColorTable::colorToName(wloadMinColor));
    settings.setValue("wloadMaxColor", TypicalColorTable::colorToName(wloadMaxColor));
    settings.setValue("wloadMaxValue", wloadMaxValue);
    settings.setValue("isDloadVisible", isDloadVisible);
    settings.setValue("dloadMinValue", dloadMinValue);
    settings.setValue("dloadMinColor", TypicalColorTable::colorToName(dloadMinColor));
    settings.setValue("dloadMaxColor", TypicalColorTable::colorToName(dloadMaxColor));
    settings.setValue("dloadMaxValue", dloadMaxValue);
    settings.setValue("useScaledFont", useScaledFont);
    settings.endGroup();
}

void CalculationLoad3DViewSettings::SetUseScaledFont(bool _useScaledFont)
{
    useScaledFont = _useScaledFont;
}

} // namespace post3dapp
