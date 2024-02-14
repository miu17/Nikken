#ifndef CALCULATIONLOAD2DSETTING_DIALOG_H
#define CALCULATIONLOAD2DSETTING_DIALOG_H

#include <QDialog>

namespace post3dapp{
class CalculationLoad2DViewSettings;
class FiveSizeComboBox;
class TypicalColorComboBox;

class CalculationLoad2DSettingDialog : public QDialog
{
    Q_OBJECT
public:
    CalculationLoad2DSettingDialog(QWidget *parent = 0);
    ~CalculationLoad2DSettingDialog();

private slots:

    void initializeSettings();
    void uploadViewSettings();

    void slotFileSave();
    void slotFileOpen();

private:
    void createWidgets(QVBoxLayout*);
    CalculationLoad2DViewSettings makeSettings() const;
    void applySettings(const CalculationLoad2DViewSettings&);
    void saveSettings(const QString&) const;
    void readSettings(const QString&);

    QCheckBox *directionCheckbox;

    TypicalColorComboBox *jointColorCombobox;
    QCheckBox *jointStringCheck;
    FiveSizeComboBox *jointSizeCombobox;

    TypicalColorComboBox *lineColorCombobox;
    QCheckBox *lineStringCheck;
    FiveSizeComboBox *lineSizeCombobox;

    TypicalColorComboBox *planeColorCombobox;
    QCheckBox *planeStringCheck;
    FiveSizeComboBox *planeSizeCombobox;

    TypicalColorComboBox *selectColorCombobox;
    TypicalColorComboBox *scaleColorCombobox;

    FiveSizeComboBox *stringSizeCombobox;

    QCheckBox *jCheck;
    TypicalColorComboBox *jColorCombobox;
    FiveSizeComboBox *jSizeCombobox;

    QCheckBox *pCheck;
    TypicalColorComboBox *pColorCombobox;
    FiveSizeComboBox *pSizeCombobox;

    QCheckBox *wCheck;
    TypicalColorComboBox *wColorCombobox;
    QDoubleSpinBox *wScaleSpin;

    QCheckBox *dCheck;
    TypicalColorComboBox *dColorCombobox;
    QDoubleSpinBox *dScaleSpin;

};
} // namespace post3dapp

#endif // CALCULATIONLOAD2DSETTING_DIALOG_H
