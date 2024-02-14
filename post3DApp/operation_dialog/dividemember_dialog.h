#ifndef DIVIDEMEMBERDIALOG_H
#define DIVIDEMEMBERDIALOG_H

#include <QDialog>

class QButtonGroup;
class QGroupBox;
class QSpinBox;
namespace post3dapp{
class DivideMemberDialog : public QDialog
{
    Q_OBJECT

public:

    DivideMemberDialog( QWidget *parent = 0 );

    int typeGroupID() const
    {
        return typeGroup->checkedId();
    }
    int divideGroupID() const
    {
        return divideGroup->checkedId();
    }
    int equalSpinValue() const
    {
        return equal_spin->value();
    }
    int ratio1SpinValue() const
    {
        return ratio1_spin->value();
    }
    int ratio2SpinValue() const
    {
        return ratio2_spin->value();
    }
    int iLenSpinValue() const
    {
        return ilen_spin->value();
    }
    int jLenSpinValue() const
    {
        return jlen_spin->value();
    }

public slots:

    void slotChangeDivideBoxStatus();

signals:

private:

    void createLayout();

    QButtonGroup *typeGroup;
    QButtonGroup *divideGroup;
    QGroupBox *divideBox;
    QSpinBox *equal_spin;
    QSpinBox *ratio1_spin;
    QSpinBox *ratio2_spin;
    QSpinBox *ilen_spin;
    QSpinBox *jlen_spin;

    void initializeValues();

    void warningInvalidValue();

};
} // namespace post3dapp
#endif
