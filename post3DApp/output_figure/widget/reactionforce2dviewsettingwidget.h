#ifndef REACTIONFORCE2DVIEWSETTINGWIDGET_H
#define REACTIONFORCE2DVIEWSETTINGWIDGET_H

#include "common2dviewsettingwidget.h"
namespace post3dapp{
class AppVersion;
class OutputFigureReactionForce2DWidget : public OutputFigureCommon2DWidget
{
    Q_OBJECT
public:
    OutputFigureReactionForce2DWidget(const QString&, const QString&, QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const override;
    void readBinaryFile(QDataStream&, const AppVersion&) override;

    void saveSettings(QSettings&) override;
    void readSettings(QSettings&) override;

public slots:
    void initializeSettings() override;

private:
    TypicalColorComboBox *reactionMinusColorCombobox;
    TypicalColorComboBox *reactionPlusColorCombobox;

};

} // namespace post3dapp

#endif // REACTIONFORCE2DVIEWSETTINGWIDGET_H
