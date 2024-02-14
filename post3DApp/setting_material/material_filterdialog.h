#ifndef MATERIALFILTER_DIALOG_H
#define MATERIALFILTER_DIALOG_H

#include <QDialog>
#include "define_materialtable.h"
namespace post3dapp{
class MaterialFilterDialog : public QDialog
{
    Q_OBJECT

public:

    MaterialFilterDialog(const MaterialDataType &, QWidget *parent = 0);

    void resetFilterColumns(const QHash<QString, QList<bool> > &);

    static QList<bool> loadDefaultFilters();
    QHash<QString, QList<bool> > getFilterColumns() const;

private:
    QHash<QString, QList<bool> > myFilterColumns;
    QHash<QString, QList<bool> > defaultFilterColumns;
    QHash<QString, QList<QCheckBox *> > allCheckBoxes;
    const MaterialDataType materialDataType;

    void setFilterColumns(const QHash<QString, QList<bool> > &filter);
    void setAllCheckBoxes(const QHash<QString, QList<bool> > &filter);
    QHash<QString, QList<bool> > readAllCheckBoxes();

private slots:

    void slot_accept();
    void slot_reject();
    void slot_saveDefault();
    void slot_loadDefault();

    void slot_allOn();
    void slot_allOff();

protected:

    void checkAllColumns(bool);
};
} // namespace post3dapp
#endif
