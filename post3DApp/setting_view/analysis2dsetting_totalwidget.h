#ifndef ANALYSIS2DSETTING_TOTALWIDGET_H
#define ANALYSIS2DSETTING_TOTALWIDGET_H

#include <QWidget>

namespace post3dapp{
class Analysis2DViewSettings;
class AnalysisAxialForce2DViewSetting;
class AnalysisBaseViewSettings;
class AnalysisCenterOfRigidity2DViewSetting;
class AnalysisCheck2DViewSetting;
class AnalysisCommon2DViewSetting;
class AnalysisDeformation2DViewSetting;
class AnalysisReactionForce2DViewSetting;
class AnalysisStress2DViewSetting;
class FiveSizeComboBox;
class TypicalColorComboBox;

class Analysis2DSettingTotalWidget : public QWidget
{
public:
    Analysis2DSettingTotalWidget(const QString&, const QString&, QWidget *parent = 0);
    QHash<int, AnalysisBaseViewSettings*> getSettings() const;
    void applySettings(const Analysis2DViewSettings&);
    const QString TabName;
public slots:
    void initializeSettings();

private:
    const QString englishTabName;

    AnalysisCommon2DViewSetting* makeCommon() const;
    AnalysisStress2DViewSetting* makeStress() const;
    AnalysisReactionForce2DViewSetting* makeReaction() const;
    AnalysisAxialForce2DViewSetting* makeAxial() const;
    AnalysisDeformation2DViewSetting* makeDeformation() const;
    AnalysisCheck2DViewSetting* makeCheck() const;
    AnalysisCenterOfRigidity2DViewSetting* makeCenter() const;


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

    TypicalColorComboBox *stressColorCombobox;
    QDoubleSpinBox *stressScaleSpin;

    QCheckBox *NValueCheck;
    QCheckBox *MValueCheck;
    QCheckBox *QValueCheck;

    QCheckBox *columnValueCheck;
    TypicalColorComboBox *stressColumnColorCombobox;

    QCheckBox *girderValueCheck;
    TypicalColorComboBox *stressGirderColorCombobox;

    QCheckBox *braceValueCheck;
    TypicalColorComboBox *stressBraceColorCombobox;

    QCheckBox *wallValueCheck;
    TypicalColorComboBox *stressWallColorCombobox;

    QCheckBox *slabValueCheck;
    TypicalColorComboBox *stressSlabColorCombobox;

    QCheckBox *panelValueCheck;
    TypicalColorComboBox *stressPanelColorCombobox;

    QCheckBox *springValueCheck;
    TypicalColorComboBox *stressSpringColorCombobox;

    QCheckBox *damperValueCheck;
    TypicalColorComboBox *stressDamperColorCombobox;

    QCheckBox *isoValueCheck;
    TypicalColorComboBox *stressIsoColorCombobox;

    TypicalColorComboBox *plasticSymbolColorCombobox;
    FiveSizeComboBox *plasticSymbolSizeCombobox;

    QDoubleSpinBox *axialScaleSpin;
    TypicalColorComboBox *axialMinusColorCombobox;
    TypicalColorComboBox *axialPlusColorCombobox;

    TypicalColorComboBox *reactionMinusColorCombobox;
    TypicalColorComboBox *reactionPlusColorCombobox;

    QSpinBox *deformScaleSpin;
    TypicalColorComboBox *beforeColorCombobox;
    TypicalColorComboBox *afterColorCombobox;

    TypicalColorComboBox *checkColorCombobox;
    QCheckBox *gradationColorCheck;

    TypicalColorComboBox *shearForceColorCombobox;
    TypicalColorComboBox *rigidityColorCombobox;
    FiveSizeComboBox *rigidityCombobox;

};
} // namespace post3dapp

#endif // ANALYSIS2DSETTING_TOTALWIDGET_H
