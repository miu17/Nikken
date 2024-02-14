#ifndef CALCULATIONLOAD3DSETTING_DIALOG_H
#define CALCULATIONLOAD3DSETTING_DIALOG_H

#include <QDialog>

namespace post3dapp{
class CalculationLoad3DViewSettings;
class FiveSizeComboBox;
class ReferenceCombobox;
class TypicalColorComboBox;

class CalculationLoad3DSettingDialog : public QDialog
{
    Q_OBJECT
public:
    CalculationLoad3DSettingDialog(QWidget *parent = 0);
    ~CalculationLoad3DSettingDialog();

private slots:

    void initializeSettings();
    void uploadViewSettings();

    void slotFileSave();
    void slotFileOpen();

private:
    void createWidget(QVBoxLayout*);
    CalculationLoad3DViewSettings makeSetting() const;
    void applySettings(const CalculationLoad3DViewSettings&);
    void saveSettings(const QString&) const;
    void readSettings(const QString&);

    QCheckBox *jointCheck;
    TypicalColorComboBox *jointColorCombobox;

    QCheckBox *lineCheck;
    TypicalColorComboBox *lineColorCombobox;

    QCheckBox *planeCheck;
    TypicalColorComboBox *planeColorCombobox;

    QCheckBox *jnoCheck;
    TypicalColorComboBox *jnoColorCombobox;
    FiveSizeComboBox *jnoSizeCombobox;

    QCheckBox *nameCheck;
    TypicalColorComboBox *nameColorCombobox;
    FiveSizeComboBox *nameSizeCombobox;

    QCheckBox *colorBarCheck;
    TypicalColorComboBox *colorBarColorCombobox;
    FiveSizeComboBox *colorBarSizeCombobox;

    TypicalColorComboBox *selectColorCombobox;
    QSpinBox *valueAlphaSpinbox;

    TypicalColorComboBox *scaleColorCombobox;
    QCheckBox *frscaleCheck;
    ReferenceCombobox *frscaleFloorCombobox;

    QCheckBox *flscaleCheck;
    QLineEdit *flscaleXLine;
    QLineEdit *flscaleYLine;

    FiveSizeComboBox *jloadSizeCombobox;

    QDoubleSpinBox *mloadSizeSpin;

    QCheckBox *jloadCheck;
    QDoubleSpinBox *jloadMinSpin;
    TypicalColorComboBox *jloadMinColor;
    QDoubleSpinBox *jloadMaxSpin;
    TypicalColorComboBox *jloadMaxColor;

    QCheckBox *ploadCheck;
    QDoubleSpinBox *ploadMinSpin;
    TypicalColorComboBox *ploadMinColor;
    QDoubleSpinBox *ploadMaxSpin;
    TypicalColorComboBox *ploadMaxColor;

    QCheckBox *wloadCheck;
    QDoubleSpinBox *wloadMinSpin;
    TypicalColorComboBox *wloadMinColor;
    QDoubleSpinBox *wloadMaxSpin;
    TypicalColorComboBox *wloadMaxColor;

    QCheckBox *dloadCheck;
    QDoubleSpinBox *dloadMinSpin;
    TypicalColorComboBox *dloadMinColor;
    QDoubleSpinBox *dloadMaxSpin;
    TypicalColorComboBox *dloadMaxColor;


};

} // namespace post3dapp

#endif // CALCULATIONLOAD3DSETTING_DIALOG_H
