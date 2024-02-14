#ifndef CALCULATIONPAPERSETTINGWIDGET_H
#define CALCULATIONPAPERSETTINGWIDGET_H

#include <QWidget>
#include "unified_settings.h"

class QRadioButton;
class QLineEdit;

namespace post3dapp {
class AppVersion;
class CalculationCustomSettingDialog;
class OutputPaperSetting;

class CalculationPaperSettingWidget : public QWidget
{
public:
    CalculationPaperSettingWidget(QWidget *parent = 0);
    OutputPaperSetting getSetting() const;

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

private:

    QRadioButton* defaultPaperRadioButton;
    QLineEdit* defaultPaperLineEdit;
    QRadioButton* a4RadioButton;
    QRadioButton* scaleRadioButton;
    QLineEdit* scaleLineEdit;

    QRadioButton* defaultRadioButton;
    QRadioButton* displayRadioButton;
    QRadioButton* customRadioButton;
    //Analysis2DViewSettings defaultViewSetting;
    //Analysis2DViewSettings displayViewSetting;
    //Analysis2DViewSettings customViewSetting;

    //    OLD_OUTPUT2DVIEWSETTINGS defaultViewSetting;
//    OLD_OUTPUT2DFIGURESETTINGS defaultFigureSetting;
//    INPUT2DVIEWSETTINGS defaultInputViewSetting;
//    OLD_OUTPUT2DVIEWSETTINGS displayViewSetting;
//    OLD_OUTPUT2DFIGURESETTINGS displayFigureSetting;
//    INPUT2DVIEWSETTINGS displayInputViewSetting;
//    OLD_OUTPUT2DVIEWSETTINGS customViewSetting;
//    OLD_OUTPUT2DFIGURESETTINGS customFigureSetting;
//    INPUT2DVIEWSETTINGS customInputViewSetting;

    CalculationCustomSettingDialog* customSettingDialog;




private slots:
    void slotConfirmDefaultSetting() const;
    void slotConfirmDisplaySetting() const;
    void slotSetCustomSetting();

};
} // namespace post3dapp

#endif // CALCULATIONPAPERSETTINGWIDGET_H
