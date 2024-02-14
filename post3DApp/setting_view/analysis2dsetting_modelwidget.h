#ifndef ANALYSIS2DSETTING_MODELWIDGET_H
#define ANALYSIS2DSETTING_MODELWIDGET_H

#include "base_setting_widget.h"

namespace post3dapp{
class Analysis2DViewSettings;
class AnalysisBaseViewSettings;
class FiveSizeComboBox;
class TypicalColorComboBox;

class Analysis2DSettingModelWidget : public BaseSettingWidget
{
    Q_OBJECT
public:
    Analysis2DSettingModelWidget(const QString&, const QString&, QWidget *parent = 0);
    QHash<int, AnalysisBaseViewSettings*> getSettings() const;
    void applySettings(const Analysis2DViewSettings&);

    const QString TabName;
public slots:
    void initializeSettings();

protected:
    const QString englishTabName;
    TypicalColorComboBox *scaleColorCombobox;
    FiveSizeComboBox *scaleSizeCombobox;
    QCheckBox *gridStringCheck;
    QCheckBox *floorStringCheck;

    TypicalColorComboBox *jointColorCombobox;
    FiveSizeComboBox *jointSizeCombobox;

    TypicalColorComboBox *lineRCColorCombobox;
    TypicalColorComboBox *lineSRCColorCombobox;
    TypicalColorComboBox *lineSColorCombobox;
    TypicalColorComboBox *lineCFTColorCombobox;
    TypicalColorComboBox *lineDamperColorCombobox;
    TypicalColorComboBox *lineIsoColorCombobox;

    TypicalColorComboBox *planeRCColorCombobox;
    TypicalColorComboBox *planeSColorCombobox;
    TypicalColorComboBox *planeDamperColorCombobox;

    TypicalColorComboBox *supportColorCombobox;

    TypicalColorComboBox *valueColorCombobox;
    FiveSizeComboBox *valueSizeCombobox;

    TypicalColorComboBox *selectColorCombobox;

    TypicalColorComboBox *noteColorCombobox;
    FiveSizeComboBox *noteSizeCombobox;

    QCheckBox *columnMarkCheck;
    QCheckBox *girderMarkCheck;
    QCheckBox *braceMarkCheck;
    QCheckBox *wallMarkCheck;
    QCheckBox *steelWallMarkCheck;
    QCheckBox *foundationWallMarkCheck;
    QCheckBox *slabMarkCheck;
    QCheckBox *damperMarkCheck;
    QCheckBox *isoMarkCheck;

    FiveSizeComboBox *markSizeCombobox;

    QCheckBox *jointNumberCheck;
    TypicalColorComboBox *jointNumberColorCombobox;
    FiveSizeComboBox *jointNumberSizeCombobox;

    QCheckBox *boundaryConditionCheck;
    TypicalColorComboBox *boundaryConditionColorCombobox;
    FiveSizeComboBox *boundaryConditionSizeCombobox;

    QCheckBox* directionCheckbox;

    TypicalColorComboBox *zwallColorCombobox;
    TypicalColorComboBox *rigidRangeColorCombobox;
    TypicalColorComboBox *panelZoneColorCombobox;

};
} // namespace post3dapp

#endif // ANALYSIS2DSETTING_MODELWIDGET_H
