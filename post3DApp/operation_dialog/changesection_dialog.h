#ifndef CHANGESECTIONDIALOG_H
#define CHANGESECTIONDIALOG_H

#include <QDialog>
#include "define_unifieddata.h"

class QLabel;
class QComboBox;
namespace post3dapp{
class ChangeSectionDialog : public QDialog
{
    Q_OBJECT

public:

    ChangeSectionDialog(QWidget *parent = nullptr , UnifiedDataType isout = UnifiedDataType::Input);

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
    UnifiedDataType isOutput;

};
} // namespace post3dapp
#endif
