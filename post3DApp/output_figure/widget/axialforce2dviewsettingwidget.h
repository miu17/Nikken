#ifndef AXIALFORCE2DVIEWSETTINGWIDGET_H
#define AXIALFORCE2DVIEWSETTINGWIDGET_H

#include "common2dviewsettingwidget.h"

namespace post3dapp{
class AppVersion;

class OutputFigureAxialForce2DWidget : public OutputFigureCommon2DWidget
{
    Q_OBJECT
public:
    OutputFigureAxialForce2DWidget(const QString&, const QString&, QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const override;
    void readBinaryFile(QDataStream&, const AppVersion&) override;

    void saveSettings(QSettings&) override;
    void readSettings(QSettings&) override;
public slots:
    void initializeSettings() override;

private:
    TypicalColorComboBox *axialMinusColorCombobox;
    TypicalColorComboBox *axialPlusColorCombobox;

};

} // namespace post3dapp

#endif // AXIALFORCE2DVIEWSETTINGWIDGET_H
