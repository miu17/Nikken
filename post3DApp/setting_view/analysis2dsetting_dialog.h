#ifndef ANALYSIS2DSETTING_DIALOG_H
#define ANALYSIS2DSETTING_DIALOG_H

#include <QDialog>

namespace post3dapp{
class Analysis2DSettingTabWidget;
class Analysis2DViewSettings;

class Analysis2DSettingDialog : public QDialog
{
    Q_OBJECT
public:
    Analysis2DSettingDialog(QWidget *parent = 0);
    ~Analysis2DSettingDialog();

private slots:
    void uploadViewSettings();

    void slotFileSave();
    void slotFileOpen();

private:
    Analysis2DSettingTabWidget *tabWidget;
    Analysis2DViewSettings makeSetting() const;
    void applySettings(const Analysis2DViewSettings&);
    void saveSettings(const QString&) const;
    void readSettings(const QString&);
};
} // namespace post3dapp

#endif // ANALYSIS2DSETTING_DIALOG_H
