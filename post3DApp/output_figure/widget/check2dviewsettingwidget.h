#ifndef CHECK2DVIEWSETTING2DWIDGET_H
#define CHECK2DVIEWSETTING2DWIDGET_H

#include "common2dviewsettingwidget.h"
namespace post3dapp{
class AppVersion;
class OutputFigureCheck2DWidget : public OutputFigureCommon2DWidget
{
    Q_OBJECT
public:
    OutputFigureCheck2DWidget(const QString&, const QString&, QWidget *parent = 0);

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

#endif // CHECK2DVIEWSETTING2DWIDGET_H
