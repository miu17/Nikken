#ifndef COMMON2DVIEWSETTINGWIDGET_H
#define COMMON2DVIEWSETTINGWIDGET_H

#include <QWidget>
#include "viewsetting_partswidget.h"
#include "define_analysis2dsettings.h"
#include "base_setting_widget.h"

namespace post3dapp{
class AppVersion;

class OutputFigureCommon2DWidget : public BaseSettingWidget
{
    Q_OBJECT
public:
    OutputFigureCommon2DWidget(const QString&, const QString&, QWidget *parent = 0);
    ~OutputFigureCommon2DWidget(){}

    virtual void writeBinaryFile(QDataStream&) const;
    virtual void readBinaryFile(QDataStream&, const AppVersion&);
    virtual void saveSettings(QSettings&);
    virtual void readSettings(QSettings&);
    QString TabName;
public slots:
    virtual void initializeSettings();

protected:
    const QString englishTabName;
    TypicalColorComboBox *scaleColorCombobox;
    FiveSizeComboBox *scaleSizeCombobox;
    QCheckBox *gridStringCheck;
    QCheckBox *floorStringCheck;

    TypicalColorComboBox *jointColorCombobox;
    FiveSizeComboBox *jointSizeCombobox;

    TypicalColorComboBox *lineRCColorCombobox;
    TypicalColorComboBox *lineSRCColorCombobox;
    TypicalColorComboBox *lineSColorCombobox;
    TypicalColorComboBox *lineCFTColorCombobox;
    TypicalColorComboBox *lineDamperColorCombobox;
    TypicalColorComboBox *lineIsoColorCombobox;

    TypicalColorComboBox *planeRCColorCombobox;
    TypicalColorComboBox *planeSColorCombobox;
    TypicalColorComboBox *planeDamperColorCombobox;

    TypicalColorComboBox *supportColorCombobox;

    TypicalColorComboBox *valueColorCombobox;
    FiveSizeComboBox *valueSizeCombobox;

    TypicalColorComboBox *selectColorCombobox;

    TypicalColorComboBox *noteColorCombobox;
    FiveSizeComboBox *noteSizeCombobox;

    QVBoxLayout *vlay;
};
} // namespace post3dapp

#endif // COMMON2DVIEWSETTINGWIDGET_H
