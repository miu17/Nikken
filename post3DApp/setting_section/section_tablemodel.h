#ifndef SECTIONTABLEMODEL_H
#define SECTIONTABLEMODEL_H

#include "customtable_model.h"
#include "define_sectiontable.h"

namespace post3dapp{
struct SectionValues;
class SectionTableModel : public CustomTableModel
{
    Q_OBJECT

public:

    SectionTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionTableModel(const SectionTableModel &);

    QVariant data(const QModelIndex &index, int role) const override;
    void setDataDirect(const QModelIndex &index, const QVariant &value, bool signalOn = true) override;
    virtual bool isBlockedItem(const QModelIndex &) const override;

    void setDefaultValuePatch(int, int, const QVariant &);

    virtual SectionValues getMemberSection(int row) const = 0;
    virtual QList<SectionValues> getSectionList() const = 0;

public slots:

signals:
    void defaultValuePatchChanged(int, int, QVariant);

protected:
    SectionDataType mySectionType;

    QList<int> shapetype_columns;
    QStringList standardList;
    QHash<int, QHash<int, QVariant> > default_value_patch;
};
} // namespace post3dapp
#endif
