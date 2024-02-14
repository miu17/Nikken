#ifndef OUTPUTFIGURE_VIEWSETTINGWIDGET_H
#define OUTPUTFIGURE_VIEWSETTINGWIDGET_H

#include <QWidget>

namespace post3dapp{
class AppVersion;
class OutputFigureCustomSettingDialog;
class OutputPaperSetting;

class OutputFigureViewSettingWidget : public QWidget
{
    Q_OBJECT
public:
    OutputFigureViewSettingWidget(QWidget *parent = nullptr);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

    OutputPaperSetting GetPaperSetting() const;

private slots:
    void slotDefaultSettingShow();
    void slotDisplaySettingShow();
    void slotCustomSettingShow();
private:
    QRadioButton* defaultPaperRadioButton;
    QLineEdit* defaultPaperLineEdit;
    QRadioButton* a4RadioButton;
    QRadioButton* scaleRadioButton;
    QLineEdit* scaleLineEdit;

    QRadioButton* defaultRadioButton;
    QRadioButton* displayRadioButton;
    QRadioButton* customRadioButton;
    OutputFigureCustomSettingDialog* customSettingDialog;

};
} // namespace post3dapp

#endif // OUTPUTFIGURE_VIEWSETTINGWIDGET_H
