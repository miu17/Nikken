#ifndef GRAPHQDELTA2DVIEWSETTINGWIDGET_H
#define GRAPHQDELTA2DVIEWSETTINGWIDGET_H

#include "base_setting_widget.h"
#include "define_analysis2dsettings.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{
class AppVersion;
class OutputFigureGraphQDelta2DWidget : public BaseSettingWidget
{
    Q_OBJECT
public:
    OutputFigureGraphQDelta2DWidget(const QString&, const QString&, QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

    void initializeSettings();
    void saveSettings(QSettings&);
    void readSettings(QSettings&);

    const QString TabName;
private:
    const QString englishTabName;
    QCheckBox *titleVisibleCheck;
    TypicalColorComboBox *plasticSymbolColorCombobox;
    TypicalColorComboBox *plasticStepColorCombobox;
    TypicalColorComboBox *ruledLineColorCombobox;

};
} // namespace post3dapp

#endif // GRAPHQDELTA2DVIEWSETTINGWIDGET_H
