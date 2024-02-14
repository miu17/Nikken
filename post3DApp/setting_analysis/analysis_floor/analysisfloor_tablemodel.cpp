#include "analysisfloor_tablemodel.h"

#include <QtGui>
#include <QMessageBox>

namespace post3dapp{
AnalysisFloorTableModel::AnalysisFloorTableModel(const CustomTableDataType &type, QObject *parent)
    : CustomTableModel(type,parent)
{
}

AnalysisFloorTableModel::AnalysisFloorTableModel(const AnalysisFloorTableModel& model)
    :CustomTableModel(model){}

bool AnalysisFloorTableModel::isBlockedItem(const QModelIndex &index) const
{
    int row = index.row();
    int col = index.column();

    if ( col == myType.getTaggedColumn("FLOOR") ) return true;
    if ( col == myType.getTaggedColumn("PARENTFLOOR") ) {
        return allItems.at(row, myType.getTaggedColumn("CHILDFLOOR")).toString() != u8"補助階";
    }

    return CustomTableModel::isBlockedItem(index);
}

QVariant AnalysisFloorTableModel::data(const QModelIndex &index, int role) const
{
    int col = index.column();
    if (col == myType.getTaggedColumn("FLOOR")){
        if (role == Qt::TextColorRole) return QColor(Qt::black);
    }

    return CustomTableModel::data(index, role);
}
} // namespace post3dapp
