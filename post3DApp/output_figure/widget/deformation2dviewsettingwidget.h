#ifndef DEFORMATION2DVIEWSETTINGWIDGET_H
#define DEFORMATION2DVIEWSETTINGWIDGET_H

#include "common2dviewsettingwidget.h"
namespace post3dapp{
class AppVersion;
class OutputFigureDeformation2DWidget : public OutputFigureCommon2DWidget
{
    Q_OBJECT
public:
    OutputFigureDeformation2DWidget(const QString&, const QString&, QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const override;
    void readBinaryFile(QDataStream&, const AppVersion&) override;

    void saveSettings(QSettings&) override;
    void readSettings(QSettings&) override;
public slots:
    void initializeSettings() override;


private:
    TypicalColorComboBox *beforeColorCombobox;
    TypicalColorComboBox *afterColorCombobox;
};
} // namespace post3dapp

#endif // DEFORMATION2DVIEWSETTINGWIDGET_H
