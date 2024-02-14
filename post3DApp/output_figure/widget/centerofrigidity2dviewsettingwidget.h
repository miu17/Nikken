#ifndef CENTEROFRIGIDITY2DVIEWSETTINGWIDGET_H
#define CENTEROFRIGIDITY2DVIEWSETTINGWIDGET_H

#include "common2dviewsettingwidget.h"
namespace post3dapp{
class AppVersion;
class OutputFigureCenterOfRigidity2DWidget : public OutputFigureCommon2DWidget
{
    Q_OBJECT
public:
    OutputFigureCenterOfRigidity2DWidget(const QString&, const QString&, QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const override;
    void readBinaryFile(QDataStream&, const AppVersion&) override;
    void saveSettings(QSettings&) override;
    void readSettings(QSettings&) override;

public slots:
    void initializeSettings() override;

private:
    TypicalColorComboBox *shearForceColorCombobox;
    TypicalColorComboBox *rigidityColorCombobox;
    FiveSizeComboBox *rigidityCombobox;


};

} // namespace post3dapp

#endif // CENTEROFRIGIDITY2DVIEWSETTINGWIDGET_H
