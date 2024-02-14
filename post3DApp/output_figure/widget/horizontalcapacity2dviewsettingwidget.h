#ifndef HORIZONTALCAPACITY2DVIEWSETTINGWIDGET_H
#define HORIZONTALCAPACITY2DVIEWSETTINGWIDGET_H
#include <QWidget>

#include "base_setting_widget.h"
#include "define_analysis2dsettings.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{
class AppVersion;
class OutputFigureGraphHorizontalCapacity2dWidget : public BaseSettingWidget
{
    Q_OBJECT
public:
    OutputFigureGraphHorizontalCapacity2dWidget(const QString&, const QString&, QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

    void initializeSettings();
    void saveSettings(QSettings&);
    void readSettings(QSettings&);
    const QString TabName;
private:
    const QString englishTabName;
    QCheckBox *titleVisibleCheck;
    TypicalColorComboBox *requiredHorizontalCapacityColorCombobox;
    TypicalColorComboBox *horizontalCapacityColorCombobox;
    TypicalColorComboBox *ruledLineColorCombobox;

};
} // namespace post3dapp

#endif // HORIZONTALCAPACITY2DVIEWSETTINGWIDGET_H
