#ifndef OUTPUTFIGURE_CUSTOMSETTINGDIALOG_H
#define OUTPUTFIGURE_CUSTOMSETTINGDIALOG_H

#include <QDialog>

namespace post3dapp{
class AppVersion;
class OutputFigureCustomSettingTabWidget;

class OutputFigureCustomSettingDialog : public QDialog
{
    Q_OBJECT
public:
    OutputFigureCustomSettingDialog(QWidget *parent = 0);

    void writeBinaryFile(QDataStream&) const;
    void readBinaryFile(QDataStream&, const AppVersion&);

private slots:
    void slotFileSave();
    void slotFileOpen();

private:
    OutputFigureCustomSettingTabWidget* customSettingTabWidget;
    void saveSettings(const QString&) const;
    void readSettings(const QString&);
};

} // namespace post3dapp

#endif // OUTPUTFIGURE_CUSTOMSETTINGDIALOG_H
