#include "define_analysis3dsettings.h"

#include "typicalcolor_table.h"
namespace post3dapp{

Analysis3DViewSettings::Analysis3DViewSettings(const Analysis3DViewSettings &setting)
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
    isMemberNameVisible = setting.isMemberNameVisible;
    memberNameColor = setting.memberNameColor;
    memberNameSize = setting.memberNameSize;
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
    useScaledFont = setting.useScaledFont;

    backGroundColor=Qt::lightGray;
    hasShade=false;
    useSmoothing=false;
    isOrthoGraphic=true;
    lineWidth = 2;

    fontSize=2;
     drawLogo = true;
     drawCoordinate= true;
    drawReactionDOF=QVector<bool>(6,true);
     reactScale = 100.0;
     graphScale = 100.0;
     graphScaleType=2;
     nodeOrFace=0;

}

Analysis3DViewSettings Analysis3DViewSettings::getDefault()
{
    Analysis3DViewSettings setting;
    setting.isJointVisible = true;
    setting.jointColor = QColorConstants::Svg::mediumblue;
    setting.isLineVisible = true;
    setting.lineColor = QColorConstants::Svg::mediumblue;
    setting.isPlaneVisible = false;
    setting.planeColor = QColorConstants::Svg::navy;
    setting.isJnoVisible = false;
    setting.jnoColor = QColorConstants::Svg::white;
    setting.jnoSize = 1;
    setting.isMemberNameVisible = true;
    setting.memberNameColor = QColorConstants::Svg::darkorange;
    setting.memberNameSize = 2;
    setting.isColorBarVisible = true;
    setting.colorBarTextColor = QColorConstants::Svg::forestgreen;
    setting.colorBarTextSize = 2;
    setting.valueAlpha = 200;
    setting.selectColor = QColorConstants::Svg::lime;
    setting.scaleColor = QColorConstants::Svg::olivedrab;
    setting.isFrScaleVisible = true;
    setting.frScaleFloor = QUuid{};
    setting.isFlScaleVisible = true;
    setting.floorScaleXY = QPointF{-5.0, -5.0};
    setting.useScaledFont = true;

    setting.backGroundColor=Qt::lightGray;
    setting.hasShade=false;
    setting.useSmoothing=false;
    setting.isOrthoGraphic=true;

    return setting;
}

Analysis3DViewSettings Analysis3DViewSettings::readSettings(const QString &filepath)
{
    Analysis3DViewSettings set;
    QSettings settings(filepath, QSettings::IniFormat);
    settings.beginGroup("Analysis3DViewSettings");
    set.isJointVisible = settings.value("isJointVisible").toBool();
    set.jointColor = TypicalColorTable::nameToColor(settings.value("jointColor").toString());
    set.isLineVisible = settings.value("isLineVisible").toBool();
    set.lineColor = TypicalColorTable::nameToColor(settings.value("lineColor").toString());
    set.isPlaneVisible = settings.value("isPlaneVisible").toBool();
    set.planeColor = TypicalColorTable::nameToColor(settings.value("planeColor").toString());
    set.isJnoVisible = settings.value("isJnoVisible").toBool();
    set.jnoColor = TypicalColorTable::nameToColor(settings.value("jnoColor").toString());
    set.jnoSize = settings.value("jnoSize").toInt();
    set.isMemberNameVisible = settings.value("isMemberNameVisible").toBool();
    set.memberNameColor = TypicalColorTable::nameToColor(settings.value("memberNameColor").toString());
    set.memberNameSize = settings.value("memberNameSize").toInt();
    set.isColorBarVisible = settings.value("isColorBarVisible").toBool();
    set.colorBarTextColor = TypicalColorTable::nameToColor(settings.value("colorBarTextColor").toString());
    set.colorBarTextSize = settings.value("colorBarTextSize").toInt();
    set.valueAlpha = settings.value("valueAlpha").toInt();
    set.selectColor = TypicalColorTable::nameToColor(settings.value("selectColor").toString());
    set.scaleColor = TypicalColorTable::nameToColor(settings.value("scaleColor").toString());
    set.isFrScaleVisible = settings.value("isFrScaleVisible").toBool();
    set.frScaleFloor = QUuid(settings.value("frScaleFloor").toString());
    set.isFlScaleVisible = settings.value("isFlScaleVisible").toBool();
    double x = settings.value("floorScaleX").toDouble();
    double y = settings.value("floorScaleY").toDouble();
    set.floorScaleXY = QPointF(x, y);
    set.useScaledFont = settings.value("useScaledFont").toBool();
    settings.endGroup();
    return set;
}

void Analysis3DViewSettings::saveSettings(const QString &filepath)
{
    QSettings settings(filepath, QSettings::IniFormat);
    settings.beginGroup("Analysis3DViewSettings");
    settings.setValue("isJointVisible", isJointVisible);
    settings.setValue("jointColor", TypicalColorTable::colorToName(jointColor));
    settings.setValue("isLineVisible", isLineVisible);
    settings.setValue("lineColor", TypicalColorTable::colorToName(lineColor));
    settings.setValue("isPlaneVisible", isPlaneVisible);
    settings.setValue("planeColor", TypicalColorTable::colorToName(planeColor));
    settings.setValue("isJnoVisible", isJnoVisible);
    settings.setValue("jnoColor", TypicalColorTable::colorToName(jnoColor));
    settings.setValue("jnoSize", jnoSize);
    settings.setValue("isMemberNameVisible", isMemberNameVisible);
    settings.setValue("memberNameColor", TypicalColorTable::colorToName(memberNameColor));
    settings.setValue("memberNameSize", memberNameSize);
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
    settings.setValue("useScaledFont", useScaledFont);
    settings.endGroup();
}

void Analysis3DViewSettings::SetScaledFont(bool _useScaledFont)
{
    useScaledFont = _useScaledFont;
}

}
