#ifndef SHEARCHECKU2DVIEWSETTINGWIDGET_H
#define SHEARCHECKU2DVIEWSETTINGWIDGET_H

#include "common2dviewsettingwidget.h"
namespace post3dapp{
class AppVersion;
class OutputFigureShearCheckU2DWidget : public OutputFigureCommon2DWidget
{
    Q_OBJECT
public:
    OutputFigureShearCheckU2DWidget(const QString&, const QString&, QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const override;
    void readBinaryFile(QDataStream&, const AppVersion&) override;

    void saveSettings(QSettings&) override;
    void readSettings(QSettings&) override;

public slots:
    void initializeSettings() override;

private:
    TypicalColorComboBox *checkColorCombobox;
    QCheckBox *gradationColorCheck;
};

} // namespace post3dapp

#endif // SHEARCHECKU2DVIEWSETTINGWIDGET_H
