#include "define_input3dsettings.h"

#include "typicalcolor_table.h"
namespace post3dapp{
Input3DViewSettings::Input3DViewSettings(const Input3DViewSettings &setting)
{
    isPriorIndivisual = setting.isPriorIndivisual;
    floorLimitedState = setting.floorLimitedState;
    limitedFloors = setting.limitedFloors;
    frameLimitedState = setting.frameLimitedState;
    limitedFrames = setting.limitedFrames;
    scaleColor = setting.scaleColor;
    frameScaleIsChecked = setting.frameScaleIsChecked;
    frameScaleFloorID = setting.frameScaleFloorID;
    frameScaleIsChecked = setting.floorScaleIsChecked;
    floorScaleXY = setting.floorScaleXY;
    jointNameSize = setting.jointNameSize;
    jointNameColor = setting.jointNameColor;
    memberNameSize = setting.memberNameSize;
    memberNameColor = setting.memberNameColor;
    StateViewSetting = setting.StateViewSetting;
    TypeViewSetting = setting.TypeViewSetting;
    NameToViewSetting = setting.NameToViewSetting;
    useScaledFont = setting.useScaledFont;
}

Input3DViewSettings Input3DViewSettings::getDefault()
{
    Input3DViewSettings setting;
    setting.isPriorIndivisual = true;
    setting.floorLimitedState = ELUNLIMITED;
    setting.limitedFloors = QList<QUuid>{};
    setting.frameLimitedState = ELUNLIMITED;
    setting.limitedFrames = QList<QUuid>{};
    setting.scaleColor = QColorConstants::Svg::olivedrab;
    setting.frameScaleIsChecked = true;
    setting.frameScaleFloorID = QUuid{};
    setting.floorScaleIsChecked = true;
    setting.floorScaleXY = QPointF(-5.0, -5.0);
    setting.jointNameSize = 1;
    setting.jointNameColor = QColorConstants::Svg::blue;
    setting.memberNameSize = 2;
    setting.memberNameColor = QColorConstants::Svg::goldenrod;
    setting.StateViewSetting = QHash<ELEMENTSTATE, COLORBRUSHSETTING>{
        {ELEMENTSTATE::ELDUMMY, COLORBRUSHSETTING{QColorConstants::Svg::green, QColorConstants::Svg::darkolivegreen, 100}},
        {ELEMENTSTATE::ELERROR, COLORBRUSHSETTING{QColorConstants::Svg::magenta, QColorConstants::Svg::magenta, 100}},
        {ELEMENTSTATE::ELSELECT, COLORBRUSHSETTING{QColorConstants::Svg::lime, QColorConstants::Svg::lawngreen, 150}}
    };
    setting.TypeViewSetting = QHash<ELEMENTTYPE, ELEMENTVIEW>{
        {ELEMENTTYPE::ELJOINT, ELEMENTVIEW{true, false, QColorConstants::Svg::blue, Qt::NoBrush}},
        {ELEMENTTYPE::ELCOLUMN, ELEMENTVIEW{true, true, QColorConstants::Svg::midnightblue, QColorConstants::Svg::skyblue, 200}},
        {ELEMENTTYPE::ELGIRDER, ELEMENTVIEW{true, true, QColorConstants::Svg::darkblue, QColorConstants::Svg::lightskyblue, 200}},
        {ELEMENTTYPE::ELBRACE, ELEMENTVIEW{true, true, QColorConstants::Svg::maroon, QColorConstants::Svg::blue, 200}},
        {ELEMENTTYPE::ELBEAM, ELEMENTVIEW{true, true, QColorConstants::Svg::maroon, QColorConstants::Svg::gainsboro, 200}},
        {ELEMENTTYPE::ELSLAB, ELEMENTVIEW{true, true, QColorConstants::Svg::royalblue, QColorConstants::Svg::seashell, 150}},
        {ELEMENTTYPE::ELWALL, ELEMENTVIEW{true, true, QColorConstants::Svg::purple, QColorConstants::Svg::midnightblue, 150}},
        {ELEMENTTYPE::ELUWALL, ELEMENTVIEW{true, true, QColorConstants::Svg::royalblue, QColorConstants::Svg::lightskyblue, 200}},
        {ELEMENTTYPE::ELDAMPER, ELEMENTVIEW{true, true, QColorConstants::Svg::darkcyan, QColorConstants::Svg::orange, 100}},
        {ELEMENTTYPE::ELISO, ELEMENTVIEW{true, true, QColorConstants::Svg::darkcyan, QColorConstants::Svg::orange, 100}},
    };
    setting.NameToViewSetting = QMap<QString, COLORBRUSHSETTING>{};
    setting.useScaledFont = true;
    return setting;
}

Input3DViewSettings Input3DViewSettings::readSettings(const QString &filepath)
{
    Input3DViewSettings set;
    QSettings settings(filepath, QSettings::IniFormat);
    settings.beginGroup("Input3DViewSettings");
    set.isPriorIndivisual = settings.value("isPriorIndivisual").toBool();
    set.floorLimitedState = static_cast<ELEMENTLIMITED>(settings.value("floorLimitedState").toInt());

    QList<QUuid> flUuid;
    auto floors = settings.value("limitedFloors").toString().split(',');
    foreach(auto fl, floors){
        flUuid.append(QUuid(fl));
    }
    set.limitedFloors = flUuid;
    set.frameLimitedState = static_cast<ELEMENTLIMITED>(settings.value("frameLimitedState").toInt());

    QList<QUuid> frUuid;
    auto frames = settings.value("limitedFrames").toString().split(',');
    foreach(auto fr, frames){
        flUuid.append(QUuid(fr));
    }
    set.limitedFrames = frUuid;
    set.scaleColor = settings.value("scaleColor").value<QColor>();
    set.frameScaleIsChecked = settings.value("frameScaleIsChecked").toBool();
    set.frameScaleFloorID = QUuid(settings.value("frameScaleFloorID").toString());
    set.floorScaleIsChecked = settings.value("floorScaleIsChecked").toBool();
    set.floorScaleXY = QPointF(settings.value("floorScaleX").toDouble(), settings.value("floorScaleY").toDouble());
    set.jointNameSize = settings.value("jointNameSize").toInt();
    set.jointNameColor = TypicalColorTable::nameToColor(settings.value("jointNameColor").toString());
    set.memberNameSize = settings.value("memberNameSize").toInt();
    set.memberNameColor = TypicalColorTable::nameToColor(settings.value("memberNameColor").toString());
    set.useScaledFont = settings.value("useScaledFont").toBool();
    settings.endGroup();

    QList<ELEMENTSTATE> stateList{ELDUMMY, ELERROR, ELSELECT};
    settings.beginReadArray("Input3DViewSettings_State");
    for (int i = 0; i < stateList.count(); i++){
        settings.setArrayIndex(i);
        ELEMENTSTATE state = stateList.at(i);
        COLORBRUSHSETTING brush(TypicalColorTable::nameToColor(settings.value("lineColor").toString()),
                                TypicalColorTable::nameToColor(settings.value("paintBrushColor").toString()),
                                settings.value("paintBrushAlfa").toInt());
        set.StateViewSetting.insert(state, brush);
    }
    settings.endArray();

    QList<ELEMENTTYPE> elementList{
        ELJOINT,
        ELCOLUMN,
        ELGIRDER,
        ELBRACE,
        ELBEAM,
        ELSLAB,
        ELWALL,
        ELUWALL,
        ELDAMPER,
        ELISO
    };
    settings.beginReadArray("Input3DViewSettings_Type");
    for (int i = 0; i < elementList.count(); i++){
        settings.setArrayIndex(i);
        ELEMENTTYPE type = elementList.at(i);
        ELEMENTVIEW view(settings.value("isVisible").toBool(),
                         settings.value("isNameVisible").toBool(),
                         TypicalColorTable::nameToColor(settings.value("lineColor").toString()),
                         TypicalColorTable::nameToColor(settings.value("paintBrushColor").toString()),
                         settings.value("paintBrushAlfa").toInt());
        set.TypeViewSetting.insert(type, view);
    }
    settings.endArray();

    int size = settings.beginReadArray("Input3DViewSettings_Name");
    for (int i = 0; i < size; i++){
        settings.setArrayIndex(i);
        QString name = settings.value("name").toString();
        COLORBRUSHSETTING brush(TypicalColorTable::nameToColor(settings.value("lineColor").toString()),
                                TypicalColorTable::nameToColor(settings.value("paintBrushColor").toString()),
                                settings.value("paintBrushAlfa").toInt());
        set.NameToViewSetting.insert(name, brush);
    }
    settings.endArray();
    return set;
}

void Input3DViewSettings::saveSettings(const QString &filepath) const
{
    QSettings settings(filepath, QSettings::IniFormat);
    settings.beginGroup("Input3DViewSettings");
    settings.setValue("isPriorIndivisual", isPriorIndivisual);
    settings.setValue("floorLimitedState", static_cast<int>(floorLimitedState));
    QStringList floors;
    foreach(auto fl, limitedFloors){
        floors.append(fl.toString());
    }
    settings.setValue("limitedFloors", floors.join(','));
    settings.setValue("frameLimitedState", static_cast<int>(frameLimitedState));
    QStringList frames;
    foreach(auto fr, limitedFrames){
        frames.append(fr.toString());
    }
    settings.setValue("limitedFrames", frames.join(','));
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("frameScaleIsChecked", frameScaleIsChecked);
    settings.setValue("frameScaleFloorID", frameScaleFloorID.toString());
    settings.setValue("floorScaleIsChecked", floorScaleIsChecked);
    settings.setValue("floorScaleX", floorScaleXY.x());
    settings.setValue("floorScaleY", floorScaleXY.y());
    settings.setValue("jointNameSize", jointNameSize);
    settings.setValue("jointNameColor", TypicalColorTable::colorToName(jointNameColor));
    settings.setValue("memberNameSize", memberNameSize);
    settings.setValue("memberNameColor", TypicalColorTable::colorToName(memberNameColor));
    settings.setValue("useScaledFont", useScaledFont);
    settings.endGroup();

    const QList<ELEMENTSTATE> stateList{ELDUMMY, ELERROR, ELSELECT};
    settings.beginWriteArray("Input3DViewSettings_State");
    for (int i = 0; i < stateList.count(); ++i) {
        settings.setArrayIndex(i);
        ELEMENTSTATE state = stateList.at(i);
        COLORBRUSHSETTING colb = StateViewSetting.value(state);
        settings.setValue("lineColor", TypicalColorTable::colorToName(colb.lineColor));
        settings.setValue("paintBrushColor", TypicalColorTable::colorToName(colb.paintBrush.color()));
        settings.setValue("paintBrushAlfa", colb.paintBrush.color().alpha());
    }
    settings.endArray();

    QList<ELEMENTTYPE> elementList{
        ELJOINT,
        ELCOLUMN,
        ELGIRDER,
        ELBRACE,
        ELBEAM,
        ELSLAB,
        ELWALL,
        ELUWALL,
        ELDAMPER,
        ELISO
    };
    settings.beginWriteArray("Input3DViewSettings_Type");
    for (int i = 0; i < elementList.count(); i++){
        settings.setArrayIndex(i);
        ELEMENTVIEW ev = TypeViewSetting.value(elementList.at(i));
        settings.setValue("isVisible", ev.isVisible);
        settings.setValue("isNameVisible", ev.isNameVisible);
        settings.setValue("lineColor", TypicalColorTable::colorToName(ev.lineColor));
        settings.setValue("paintBrushColor", TypicalColorTable::colorToName(ev.paintBrush.color()));
        settings.setValue("paintBrushAlfa", ev.paintBrush.color().alpha());
    }
    settings.endArray();

    settings.beginWriteArray("Input3DViewSettings_Name");
    auto keys = NameToViewSetting.keys();
    for (int i = 0; i < NameToViewSetting.keys().count(); i++){
        settings.setArrayIndex(i);
        settings.setValue("name", keys.at(i));
        auto value = NameToViewSetting.value(keys.at(i));
        settings.setValue("lineColor", TypicalColorTable::colorToName(value.lineColor));
        settings.setValue("paintBrushColor", TypicalColorTable::colorToName(value.paintBrush.color()));
        settings.setValue("paintBrushAlfa", value.paintBrush.color().alpha());
    }
    settings.endArray();
}

COLORBRUSHSETTING Input3DViewSettings::getElementStateView(ELEMENTSTATE state) const
{
    return StateViewSetting.value(state);
}

int Input3DViewSettings::getElementNameSize(ELEMENTTYPE etype ) const
{
    if ( etype == ELJOINT ) {
        return jointNameSize;
    } else if ( etype == ELCOLUMN || etype == ELGIRDER || etype == ELBRACE ||
                etype == ELBEAM || etype == ELDAMPER || etype == ELISO ||
                etype == ELSLAB || etype == ELWALL || etype == ELUWALL ) {
        return memberNameSize;
    }
    return memberNameSize;
}

QColor Input3DViewSettings::getElementNameColor( ELEMENTTYPE etype ) const
{
    if ( etype == ELJOINT ) {
        return jointNameColor;
    } else {
        return memberNameColor;
    }
}

ELEMENTVIEW Input3DViewSettings::getElementView(ELEMENTTYPE etype) const
{
    return TypeViewSetting.value(etype);
}

void Input3DViewSettings::SetUseScaledFont(bool _useScaledFont)
{
    useScaledFont = _useScaledFont;
}

COLORBRUSHSETTING Input3DViewSettings::getIndividualSetting(const QString &name) const
{
    return NameToViewSetting.value(name);
}

bool Input3DViewSettings::existIndividualSetting(const QString &name) const
{
    return NameToViewSetting.contains(name);
}

bool Input3DViewSettings::isVisibleElement(ELEMENTTYPE etype) const
{
    return TypeViewSetting.value(etype).isVisible;
}


} // namespace post3dapp
