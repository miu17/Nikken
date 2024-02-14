#ifndef STRESS2DVIEWSETTINGWIDGET_H
#define STRESS2DVIEWSETTINGWIDGET_H

#include "common2dviewsettingwidget.h"
namespace post3dapp{
class AppVersion;
class OutputFigureStress2DWidget : public OutputFigureCommon2DWidget
{
    Q_OBJECT
public:
    OutputFigureStress2DWidget(const QString&, const QString&, QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const override;
    void readBinaryFile(QDataStream&, const AppVersion&) override;

    void saveSettings(QSettings&) override;
    void readSettings(QSettings&) override;

public slots:
    void initializeSettings() override;



private:
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
};

} // namespace post3dapp

#endif // STRESS2DVIEWSETTINGWIDGET_H
