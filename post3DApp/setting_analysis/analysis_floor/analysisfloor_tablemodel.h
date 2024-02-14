#ifndef ANALYSISFLOORTABLEMODEL_H
#define ANALYSISFLOORTABLEMODEL_H

#include "customtable_model.h"
namespace post3dapp{
class AnalysisFloorTableModel : public CustomTableModel
{
    Q_OBJECT

public:

    AnalysisFloorTableModel(const CustomTableDataType &type, QObject *parent = nullptr);
    AnalysisFloorTableModel(const AnalysisFloorTableModel&);

    QVariant data(const QModelIndex &index, int role) const override;
    bool isBlockedItem(const QModelIndex &) const override;

public slots:

signals:

protected:

};
} // namespace post3dapp
#endif
