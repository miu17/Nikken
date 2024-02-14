#ifndef ANALYSIS3DSETTING_DIALOG_H
#define ANALYSIS3DSETTING_DIALOG_H

#include <QDialog>
namespace post3dapp{
class Analysis3DViewSettings;
class FiveSizeComboBox;
class ReferenceCombobox;
class TypicalColorComboBox;

class Analysis3DSettingDialog : public QDialog
{
    Q_OBJECT
public:
    Analysis3DSettingDialog(QWidget *parent = 0);
    ~Analysis3DSettingDialog();

private slots:

    void initializeSettings();
    void uploadViewSettings();

    void slotFileSave();
    void slotFileOpen();

private:
    void createWidget(QVBoxLayout*);
    Analysis3DViewSettings makeSettings() const;
    void applySettings(const Analysis3DViewSettings&);
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

    QSpinBox *valueAlphaSpinbox;
    TypicalColorComboBox *selectColorCombobox;

    TypicalColorComboBox *scaleColorCombobox;
    QCheckBox *frscaleCheck;
    ReferenceCombobox *frscaleFloorCombobox;

    QCheckBox *flscaleCheck;
    QLineEdit *flscaleXLine;
    QLineEdit *flscaleYLine;



};
} // namespace post3dapp

#endif // ANALYSIS3DSETTING_DIALOG_H
