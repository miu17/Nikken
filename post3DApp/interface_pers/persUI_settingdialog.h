#ifndef PERSINTERFACESETTINGDIALOG_H
#define PERSINTERFACESETTINGDIALOG_H

#include <QDialog>
#include "define_glsetting.h"

class QButtonGroup;
class QCheckBox;
class QLineEdit;
class QSpinBox;

namespace post3dapp{
class TypicalColorComboBox;
class PersInterfaceSettingDialog : public QDialog
{
    Q_OBJECT

public:

    PersInterfaceSettingDialog( QWidget *parent = 0 );

    void setCenterPosition(qreal, qreal, qreal);
    void setCurrentValues();
    PERSSETTINGVALUES currentViewSettings() const
    {
        return mySettings;
    }

protected:

    void closeEvent(QCloseEvent *);

private slots:

    void slotLight1Enable(bool);
    void slotLight2Enable(bool);
    void slotLight3Enable(bool);
    void slotClippingValuesEnable(bool);
    void slotGridValuesEnable(bool);
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

    QButtonGroup *drawingStyleButtons;
    TypicalColorComboBox *backColorComboBox;
    QLineEdit *oxLine;
    QLineEdit *oyLine;
    QLineEdit *ozLine;
    QCheckBox *light1Check;
    QLineEdit *l1xLine;
    QLineEdit *l1yLine;
    QLineEdit *l1zLine;
    QCheckBox *light2Check;
    QLineEdit *l2xLine;
    QLineEdit *l2yLine;
    QLineEdit *l2zLine;
    QCheckBox *light3Check;
    QLineEdit *l3xLine;
    QLineEdit *l3yLine;
    QLineEdit *l3zLine;
    QCheckBox *clippingCheck;
    QLineEdit *caLine;
    QLineEdit *cbLine;
    QLineEdit *ccLine;
    QLineEdit *cdLine;
    QCheckBox *gridCheck;
    QLineEdit *gxLine;
    QLineEdit *gyLine;
    QLineEdit *gzLine;
    QLineEdit *ptLine;
    QSpinBox *ixSpin;
    QSpinBox *iySpin;
    QCheckBox *textCheck;
    QCheckBox *jointCheck;
    QCheckBox *axisCheck;
    QLineEdit *axisLine;

    void createLayout();
};
} // namespace post3dapp
#endif

