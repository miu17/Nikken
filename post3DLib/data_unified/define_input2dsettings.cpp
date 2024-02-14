#include "define_input2dsettings.h"

#include "typicalcolor_table.h"
namespace post3dapp{

Input2DViewSettings::Input2DViewSettings(const Input2DViewSettings &setting)
{
    isPriorIndivisual = setting.isPriorIndivisual;
    isLineDirVisible = setting.isLineDirVisible;
    scaleColor = setting.scaleColor;
    scaleFocusColor = setting.scaleFocusColor;
    jointNameSize = setting.jointNameSize;
    memberNameSize = setting.memberNameSize;
    loadNameSize = setting.loadNameSize;
    StateViewSetting = setting.StateViewSetting;
    TypeViewSetting = setting.TypeViewSetting;
    NameToViewSetting = setting.NameToViewSetting;
    crossElementView = setting.crossElementView;
    downColorBrushSetting = setting.downColorBrushSetting;
    useScaledFont = setting.useScaledFont;
    scaleSize = setting.scaleSize;
}

Input2DViewSettings Input2DViewSettings::getDefault()
{
    Input2DViewSettings setting;
    setting.isPriorIndivisual = true;
    setting.isLineDirVisible = true;
    setting.scaleColor = Qt::red;
    setting.scaleFocusColor = Qt::blue;
    setting.jointNameSize = 4;
    setting.memberNameSize = 2;
    setting.loadNameSize = 2;
    setting.StateViewSetting = QHash<ELEMENTSTATE, COLORBRUSHSETTING>{
        {ELEMENTSTATE::ELDUMMY, COLORBRUSHSETTING{QColorConstants::Svg::green, QColorConstants::Svg::green, 20}},
        {ELEMENTSTATE::ELERROR, COLORBRUSHSETTING{QColorConstants::Svg::magenta, QColorConstants::Svg::magenta, 50}},
        {ELEMENTSTATE::ELSELECT, COLORBRUSHSETTING{QColorConstants::Svg::lime, QColorConstants::Svg::lawngreen, 50}},
        {ELEMENTSTATE::ELINPUT, COLORBRUSHSETTING{QColorConstants::Svg::red, QColorConstants::Svg::orangered, 50}}
    };
    setting.TypeViewSetting = QHash<ELEMENTTYPE, ELEMENTVIEW>{
        {ELEMENTTYPE::ELJOINT, ELEMENTVIEW{true, true, QColorConstants::Svg::darkblue, QColorConstants::Svg::crimson, 255}},
        {ELEMENTTYPE::ELCOLUMN, ELEMENTVIEW{true, true, QColorConstants::Svg::mediumblue, QColorConstants::Svg::turquoise, 50}},
        {ELEMENTTYPE::ELGIRDER, ELEMENTVIEW{true, true, QColorConstants::Svg::indigo, QColorConstants::Svg::lightsteelblue, 50}},
        {ELEMENTTYPE::ELBRACE, ELEMENTVIEW{true, true, QColorConstants::Svg::purple, QColorConstants::Svg::mediumorchid, 50}},
        {ELEMENTTYPE::ELBEAM, ELEMENTVIEW{true, true, QColorConstants::Svg::dimgray, Qt::NoBrush}},
        {ELEMENTTYPE::ELSLAB, ELEMENTVIEW{true, true, QColorConstants::Svg::darkgreen, QColorConstants::Svg::cadetblue, 30}},
        {ELEMENTTYPE::ELWALL, ELEMENTVIEW{true, true, QColorConstants::Svg::darkolivegreen, QColorConstants::Svg::yellowgreen, 30}},
        {ELEMENTTYPE::ELUWALL, ELEMENTVIEW{true, true, QColorConstants::Svg::darkolivegreen, QColorConstants::Svg::yellowgreen, 30}},
        {ELEMENTTYPE::ELDAMPER, ELEMENTVIEW{true, true, QColorConstants::Svg::darkred, QColorConstants::Svg::tomato, 50}},
        {ELEMENTTYPE::ELISO, ELEMENTVIEW{true, true, QColorConstants::Svg::darkred, QColorConstants::Svg::tomato, 50}},
        {ELEMENTTYPE::ELJOINTLOAD, ELEMENTVIEW{true, true, QColorConstants::Svg::indianred, QColorConstants::Svg::lightcoral, 50}},
        {ELEMENTTYPE::ELMEMBERLOAD, ELEMENTVIEW{true, true, QColorConstants::Svg::orangered, QColorConstants::Svg::lightsalmon, 100}},
        {ELEMENTTYPE::ELSLABLOAD, ELEMENTVIEW{true, true, QColorConstants::Svg::firebrick, QColorConstants::Svg::orange, 20}},
    };
    setting.NameToViewSetting = QMap<QString, COLORBRUSHSETTING>{};
    setting.crossElementView = ELEMENTVIEW{true, true, QColorConstants::Svg::saddlebrown, QColorConstants::Svg::burlywood, 150};
    setting.downColorBrushSetting = DOWNSETTING{true, QColorConstants::Svg::gray, Qt::NoBrush};
    setting.useScaledFont = true;
    setting.scaleSize = 3;
    return setting;
}


int Input2DViewSettings::getElementNameSize(ELEMENTTYPE etype ) const
{
    if ( etype == ELJOINT ) {
        return jointNameSize;
    } else if ( etype == ELCOLUMN || etype == ELGIRDER || etype == ELBRACE ||
                etype == ELBEAM || etype == ELDAMPER || etype == ELISO ||
                etype == ELSLAB || etype == ELWALL || etype == ELUWALL ) {
        return memberNameSize;
    } else if ( etype == ELJOINTLOAD || etype == ELMEMBERLOAD || etype == ELSLABLOAD ) {
        return loadNameSize;
    }
    return memberNameSize;
}

COLORBRUSHSETTING Input2DViewSettings::getElementStateView(ELEMENTSTATE state) const
{
    return StateViewSetting.value(state);
}

bool Input2DViewSettings::isVisibleElement(ELEMENTTYPE etype) const
{
    return TypeViewSetting.value(etype).isVisible;
}

ELEMENTVIEW Input2DViewSettings::getElementView(ELEMENTTYPE etype) const
{
    return TypeViewSetting.value(etype);
}

COLORBRUSHSETTING Input2DViewSettings::getIndividualSetting(const QString &name) const
{
    return NameToViewSetting.value(name);
}

bool Input2DViewSettings::existIndividualSetting(const QString &name) const
{
    return NameToViewSetting.contains(name);
}

void Input2DViewSettings::SetUseScaledFont(bool _useScaledFont)
{
    useScaledFont = _useScaledFont;
}

Input2DViewSettings Input2DViewSettings::makeSymbolSetting(const Input2DViewSettings& old)
{
    Input2DViewSettings setting(old);
    setting.isPriorIndivisual = false;
    setting.overwriteElementType(ELEMENTTYPE::ELJOINT, true, false);
    setting.overwriteElementType(ELEMENTTYPE::ELCOLUMN, true, true);
    setting.overwriteElementType(ELEMENTTYPE::ELGIRDER, true, true);
    setting.overwriteElementType(ELEMENTTYPE::ELBRACE, true, true);
    setting.overwriteElementType(ELEMENTTYPE::ELBEAM, true, true);
    setting.overwriteElementType(ELEMENTTYPE::ELSLAB, true, true);
    setting.overwriteElementType(ELEMENTTYPE::ELWALL, true, true);
    setting.overwriteElementType(ELEMENTTYPE::ELUWALL, true, true);
    setting.overwriteElementType(ELEMENTTYPE::ELDAMPER, true, true);
    setting.overwriteElementType(ELEMENTTYPE::ELISO, true, true);
    setting.overwriteElementType(ELEMENTTYPE::ELJOINTLOAD, false, false);
    setting.overwriteElementType(ELEMENTTYPE::ELMEMBERLOAD, false, false);
    setting.overwriteElementType(ELEMENTTYPE::ELSLABLOAD, false, false);
    setting.crossElementView.isNameVisible = false;
    setting.downColorBrushSetting.isNameVisible = false;
    return setting;
}

void Input2DViewSettings::overwriteElementType(ELEMENTTYPE type, bool visible, bool namevisible){
    auto temp = this->TypeViewSetting[type];
    temp.isVisible = visible;
    temp.isNameVisible = namevisible;
    this->TypeViewSetting[type] = temp;
}

Input2DViewSettings Input2DViewSettings::makeLoadSetting(const Input2DViewSettings& old){
    Input2DViewSettings loadSetting(old);
    loadSetting.isPriorIndivisual = false;
    loadSetting.overwriteElementType(ELEMENTTYPE::ELJOINT, true, false);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELCOLUMN, true, false);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELGIRDER, true, false);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELBRACE, true, false);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELBEAM, true, false);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELSLAB, true, false);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELWALL, true, false);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELUWALL, true, false);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELDAMPER, true, false);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELISO, true, false);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELJOINTLOAD, true, true);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELMEMBERLOAD, true, true);
    loadSetting.overwriteElementType(ELEMENTTYPE::ELSLABLOAD, true, true);
    return loadSetting;
}

Input2DViewSettings Input2DViewSettings::readSettings(const QString &filepath)
{
    Input2DViewSettings set;
    QSettings settings(filepath, QSettings::IniFormat);
    settings.beginGroup("Input2DViewSettings");
    set.isPriorIndivisual = settings.value("isPriorIndivisual").toBool();
    set.isLineDirVisible = settings.value("isLineDirVisible").toBool();
    set.scaleColor = TypicalColorTable::nameToColor(settings.value("scaleColor").toString());
    set.scaleFocusColor = TypicalColorTable::nameToColor(settings.value("scaleFocusColor").toString());
    set.jointNameSize = settings.value("jointNameSize").toInt();
    set.memberNameSize = settings.value("memberNameSize").toInt();
    set.loadNameSize = settings.value("loadNameSize").toInt();
    set.useScaledFont = settings.value("useScaledFont").toBool();
    set.scaleSize = settings.value("scaleSize").toInt();
    settings.endGroup();

    QList<ELEMENTSTATE> stateList{ELDUMMY, ELERROR, ELSELECT, ELINPUT};
    settings.beginReadArray("Input2DViewSettings_State");
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
        ELISO,
        ELJOINTLOAD,
        ELMEMBERLOAD,
        ELSLABLOAD
    };
    settings.beginReadArray("Input2DViewSettings_Type");
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

    settings.beginGroup("Input2DViewSettings_Cross");
    ELEMENTVIEW cross(settings.value("isVisible").toBool(),
                      settings.value("isNameVisible").toBool(),
                      TypicalColorTable::nameToColor(settings.value("lineColor").toString()),
                      TypicalColorTable::nameToColor(settings.value("paintBrushColor").toString()),
                      settings.value("paintBrushAlfa").toInt());
    set.crossElementView = cross;
    settings.endGroup();

    settings.beginGroup("Input2DViewSettings_Down");
    DOWNSETTING down(settings.value("isNameVisible").toBool(),
                     TypicalColorTable::nameToColor(settings.value("lineColor").toString()),
                     TypicalColorTable::nameToColor(settings.value("paintBrushColor").toString()),
                     settings.value("paintBrushAlfa").toInt());
    set.downColorBrushSetting = down;
    settings.endGroup();

    int size = settings.beginReadArray("Input2DViewSettings_Name");
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

void Input2DViewSettings::saveSettings(const QString &filepath) const
{
    QSettings settings(filepath, QSettings::IniFormat);
    settings.beginGroup("Input2DViewSettings");
    settings.setValue("isPriorIndivisual", isPriorIndivisual);
    settings.setValue("isLineDirVisible", isLineDirVisible);
    settings.setValue("scaleColor", TypicalColorTable::colorToName(scaleColor));
    settings.setValue("scaleFocusColor", TypicalColorTable::colorToName(scaleFocusColor));
    settings.setValue("jointNameSize", jointNameSize);
    settings.setValue("memberNameSize", memberNameSize);
    settings.setValue("loadNameSize", loadNameSize);
    settings.setValue("useScaledFont", useScaledFont);
    settings.setValue("scaleSize", scaleSize);
    settings.endGroup();

    QList<ELEMENTSTATE> stateList{ELDUMMY, ELERROR, ELSELECT, ELINPUT};
    settings.beginWriteArray("Input2DViewSettings_State");
    for (int i = 0; i < stateList.count(); i++){
        settings.setArrayIndex(i);
        COLORBRUSHSETTING colb = StateViewSetting.value(stateList.at(i));
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
        ELISO,
        ELJOINTLOAD,
        ELMEMBERLOAD,
        ELSLABLOAD
    };
    settings.beginWriteArray("Input2DViewSettings_Type");
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

    settings.beginGroup("Input2DViewSettings_Cross");
    settings.setValue("isVisible", crossElementView.isVisible);
    settings.setValue("isNameVisible", crossElementView.isNameVisible);
    settings.setValue("lineColor", TypicalColorTable::colorToName(crossElementView.lineColor));
    settings.setValue("paintBrushColor", TypicalColorTable::colorToName(crossElementView.paintBrush.color()));
    settings.setValue("paintBrushAlfa", crossElementView.paintBrush.color().alpha());
    settings.endGroup();

    settings.beginGroup("Input2DViewSettings_Down");
    settings.setValue("isNameVisible", downColorBrushSetting.isNameVisible);
    settings.setValue("lineColor", TypicalColorTable::colorToName(downColorBrushSetting.lineColor));
    settings.setValue("paintBrushColor", TypicalColorTable::colorToName(downColorBrushSetting.paintBrush.color()));
    settings.setValue("paintBrushAlfa", downColorBrushSetting.paintBrush.color().alpha());
    settings.endGroup();

    settings.beginWriteArray("Input2DViewSettings_Name");
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

Input2DViewSettings Input2DViewSettings::makeOutputColorSetting()
{
    return Input2DViewSettings{
        true,
        true,
        Qt::red,
        Qt::blue,
        10,
        10,
        10,
        QHash<ELEMENTSTATE, COLORBRUSHSETTING>{
            {ELEMENTSTATE::ELDUMMY, COLORBRUSHSETTING(Qt::green, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTSTATE::ELERROR, COLORBRUSHSETTING(Qt::magenta, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTSTATE::ELSELECT, COLORBRUSHSETTING(Qt::green, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTSTATE::ELINPUT, COLORBRUSHSETTING(Qt::red, QBrush(QColor(0, 0, 0, 0)))}
        },
        QHash<ELEMENTTYPE, ELEMENTVIEW>{
            {ELEMENTTYPE::ELJOINT, ELEMENTVIEW(true, true, Qt::darkBlue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELCOLUMN, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELGIRDER, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELBRACE, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELBEAM, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELSLAB, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELWALL, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELUWALL, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELDAMPER, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELISO, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELCOLUMN, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELJOINTLOAD, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELMEMBERLOAD, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
            {ELEMENTTYPE::ELSLABLOAD, ELEMENTVIEW(true, true, Qt::blue, QBrush(QColor(0, 0, 0, 0)))},
        },
        ELEMENTVIEW(true, true, Qt::green, QBrush(QColor(0, 0, 0, 0))),
        DOWNSETTING(true, Qt::green, QBrush(QColor(0, 0, 0, 0))),
        QMap<QString, COLORBRUSHSETTING>{},
        false,
        3
    };
}
} // namespace post3dapp
