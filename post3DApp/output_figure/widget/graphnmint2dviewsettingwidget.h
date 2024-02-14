#ifndef GRAPHNMINT2DVIEWSETTINGWIDGET_H
#define GRAPHNMINT2DVIEWSETTINGWIDGET_H
#include "base_setting_widget.h"
#include "define_analysis2dsettings.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{
class AppVersion;
class OutputFigureGraphNmint2DWidget : public BaseSettingWidget
{
    Q_OBJECT
public:
    OutputFigureGraphNmint2DWidget(const QString&, const QString&, QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

    void initializeSettings();
    void saveSettings(QSettings&);
    void readSettings(QSettings&);
    const QString TabName;
private:
    const QString englishTabName;
    QCheckBox *titleVisibleCheck;
    TypicalColorComboBox *axisColorCombobox;
    TypicalColorComboBox *axisTextColorCombobox;
    FiveSizeComboBox *axisTextSizeCombobox;

    QCheckBox *outputLVisibleCheck;
    QCheckBox *outputSVisibleCheck;
    QCheckBox *outputUVisibleCheck;

    TypicalColorComboBox *symbolColorCombobox;
    FiveSizeComboBox *symbolSizeCombobox;

    QCheckBox *isAssignedNValueCheck;
    QDoubleSpinBox *minNValueSpinBox;
    QDoubleSpinBox *maxNValueSpinBox;
    QDoubleSpinBox *pitchNValueSpinBox;

    QCheckBox *isAssignedMyValueCheck;
    QDoubleSpinBox *minMyValueSpinBox;
    QDoubleSpinBox *maxMyValueSpinBox;
    QDoubleSpinBox *pitchMyValueSpinBox;

    QCheckBox *isAssignedMzValueCheck;
    QDoubleSpinBox *minMzValueSpinBox;
    QDoubleSpinBox *maxMzValueSpinBox;
    QDoubleSpinBox *pitchMzValueSpinBox;

};
} // namespace post3dapp

#endif // GRAPHNMINT2DVIEWSETTINGWIDGET_H
