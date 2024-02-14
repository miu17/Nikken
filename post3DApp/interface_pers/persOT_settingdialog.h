#ifndef PERSOUTPUTSETTINGDIALOG_H
#define PERSOUTPUTSETTINGDIALOG_H

#include <QDialog>
#include "define_glsetting.h"

class QCheckBox;
class QLineEdit;

namespace post3dapp{
class TypicalColorComboBox;
class PersOutputSettingDialog : public QDialog
{
    Q_OBJECT

public:

    PersOutputSettingDialog( QWidget *parent = 0 );

    void setCenterPosition(qreal, qreal, qreal);
    void setCurrentValues();
    PERSSETTINGVALUES currentViewSettings() const
    {
        return mySettings;
    }

protected:

    void closeEvent(QCloseEvent *);

private slots:

    void slotAxisValuesEnable(bool);
    void slotChangeViewSetting();

    void accept()
    {
        emit windowClosed();
        QDialog::accept();
    }
    void reject()
    {
        emit windowClosed();
        QDialog::reject();
    }

signals:

    void windowClosed();
    void viewSettingChanged();

private:

    PERSSETTINGVALUES mySettings;

    TypicalColorComboBox *backColorComboBox;
    QLineEdit *oxLine;
    QLineEdit *oyLine;
    QLineEdit *ozLine;
    QCheckBox *textCheck;
    QCheckBox *jointCheck;
    QCheckBox *axisCheck;
    QLineEdit *axisLine;

    void createLayout();
};
} // namespace post3dapp
#endif

