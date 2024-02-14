#ifndef CHANGESECTIONDIALOG_H
#define CHANGESECTIONDIALOG_H

#include <QtWidgets>
#include <QDialog>
#include "factory_unifieddata.h"

class QLabel;
class QComboBox;
namespace post3dapp{
class ChangeSectionDialog : public QDialog
{
    Q_OBJECT

public:

    ChangeSectionDialog(QWidget *parent = nullptr , bool isout = false);

    void setCurrentDataType(DATATYPE);

    DATATYPE currentDataType() const;
    QString currentName() const;

public slots:

    void slotChangeSectionComboBox(int);

signals:

private:

    void createLayout();

    QComboBox *elementComboBox;
    QComboBox *sectionComboBox;

    QStringList dataTypeSrings;
    QList<DATATYPE> dataTypes;
    bool isOutput;

};
} // namespace post3dapp
#endif
