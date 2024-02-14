#ifndef SECTIONLINEBUTTONS_H
#define SECTIONLINEBUTTONS_H

#include "define_sectiontable.h"

class QLabel;
class QLineEdit;
namespace post3dapp{
class SectionLineButtons : public QWidget
{
    Q_OBJECT

public:

    SectionLineButtons( const SectionDataType &, const QList<bool> &, QWidget *parent = nullptr );

    void setLineValues(int row, const QList<QVariant> &values);

    int currentRow() const
    {
        return NoSpinBox->value() - 1;
    }

    void setSpinBoxMinMax(int min, int max)
    {
        NoSpinBox->setMinimum(min);
        NoSpinBox->setMaximum(max);
    }

    void setButtonFocus(int);


public slots:

    void signalChangedValues(int);
    void signalSpinValueChange(int value)
    {
        emit currentLineNumberChanged(value - 1);
    }
    void slotChangedFilter(const QList<bool> &);

signals:

    void lineButtonsDataChanged(int, int, const QString &);
    void currentLineNumberChanged(int);

protected:
    virtual void setStandardLineEdit();
    SectionDataType mySectionType;
    QList<QVariant> myValues;
    QList<QLabel *> myLblist;
    QList<QWidget *> myLelist;

private:
    QSpinBox *NoSpinBox;
    void createButtons();
    void resetEditerValues(int);
    void setFilter(const QList<bool> &);
};
} // namespace post3dapp
#endif
