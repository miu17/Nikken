#ifndef MATERIALTABLEMODEL_H
#define MATERIALTABLEMODEL_H

#include "customtable_model.h"
#include "define_materialtable.h"
namespace post3dapp{
struct MATERIALVALUES;

class MaterialTableModel : public CustomTableModel
{
    Q_OBJECT

public:

    MaterialTableModel(const MaterialDataType &type, QObject *parent = 0);
    MaterialTableModel(const MaterialTableModel&);

    QVariant data(const QModelIndex &index, int role) const override;

    void setDataDirect(const QModelIndex &index, const QVariant &value, bool signalOn = true) override;

    bool isBlockedItem(const QModelIndex &) const override;

    void setDefaultValuePatch(int, int, const QVariant &);
    QList<MATERIALVALUES> getMaterialList() const;

    static MATERIALVALUES CreateMaterialValues(const QList<QVariant>&, const MaterialDataType&);

public slots:

signals:
    void defaultValuePatchChanged(int, int, QVariant);

protected:

    MaterialDataType myType;

    QList<int> shapetype_columns;
    QHash<int, QHash<int, QVariant> > default_value_patch;
};
} // namespace post3dapp
#endif
