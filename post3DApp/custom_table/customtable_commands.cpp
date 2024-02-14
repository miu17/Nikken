#include "customtable_commands.h"

#include <QtGui>
#include <QDebug>
#include "customtable_model.h"
#include "sortfilter_proxymodel.h"
#include "unified_inputdata.h"

namespace post3dapp{
InsertLineCommand::InsertLineCommand(MySortFilterProxyModel *model, int row,
                                     int count, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myModel = qobject_cast<CustomTableModel *>(model->sourceModel());
    insertedRow = row;
    insertedCount = count;
}

void InsertLineCommand::undo()
{
    myModel->removeRows(insertedRow, insertedCount);
}

void InsertLineCommand::redo()
{
    myModel->insertRows(insertedRow, insertedCount);
}


DeleteLineCommand::DeleteLineCommand(MySortFilterProxyModel *model, int row,
                                     int count, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myModel = qobject_cast<CustomTableModel *>(model->sourceModel());
    deletedRow = row;
    deletedCount = count;
}

void DeleteLineCommand::undo()
{
    myModel->insertRows(deletedRow, deletedCount);
    for (int i = 0 ; i < deletedCount ; i++ ) {
        myModel->setRowDataDirect(i + deletedRow, deletedData.at(i));
    }
}

void DeleteLineCommand::redo()
{
    deletedData.clear();
    for (int i = deletedRow ; i < deletedRow + deletedCount ; i++ ) {
        QList<QVariant> vlist = myModel->getRowData(i);
        deletedData.append(vlist);
    }
    myModel->removeRows(deletedRow, deletedCount);
}


MoveLineCommand::MoveLineCommand(MySortFilterProxyModel *model,
                                 int a_row, int b_row, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myModel = qobject_cast<CustomTableModel *>(model->sourceModel());
    fromRow = a_row;
    toRow = b_row;
}

void MoveLineCommand::undo()
{
    myModel->swapRows(toRow, fromRow);
}

void MoveLineCommand::redo()
{
    myModel->swapRows(fromRow, toRow);
}


PasteCommand::PasteCommand(MySortFilterProxyModel *pr_model, const QModelIndex &cur_idx,
                           const QString &clipstr, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myModel = qobject_cast<CustomTableModel *>(pr_model->sourceModel());
    if ( !cur_idx.isValid() ) return;

    QStringList oneline = clipstr.split('\n');
    int minRow = myModel->rowCount() - 1;
    int maxRow = 0;
    int minCol = myModel->columnCount() - 1;
    int maxCol = 0;
    for (int i = 0; i < oneline.count() - 1; ++i) {
        int row = cur_idx.row() + i;
        QStringList cells = oneline[i].split('\t');
        QModelIndex tmp_idx = pr_model->mapToSource(pr_model->index(row, 0));
        maxRow = qMax(maxRow, tmp_idx.row());
        minRow = qMin(minRow, tmp_idx.row());
        for (int j = 0; j < oneline[i].count('\t') + 1 ; ++j) {
            int col = cur_idx.column() + j;
            if ( row >= pr_model->rowCount() || col >= pr_model->columnCount() ) continue;
            QModelIndex idx = pr_model->mapToSource( pr_model->index(row, col) );

            if ( myModel->isDisableItem(idx) ) continue; ///////////////////////

            maxCol = qMax(maxCol, idx.column());
            minCol = qMin(minCol, idx.column());
            DATATYPE type = myModel->dataType(idx);
            if ( type == DATATYPE::TPFLOOR || type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP ) {
                orgData.insert( idx, myModel->getData(idx.row(), idx.column()) );
                QUuid uid = UnifiedInputData::getInInstance()->nameToID(cells[j], type);
                ( uid.isNull() ) ? newData.insert( idx, QVariant() ) :
                newData.insert( idx, QVariant(uid) );
            } else {
                orgData.insert( idx, myModel->data(idx, Qt::EditRole) );
                newData.insert( idx, QVariant( cells[j] ) );
            }
        }
    }
    minIndex = myModel->index(minRow, minCol);
    maxIndex = myModel->index(maxRow, maxCol);
}

void PasteCommand::undo()
{
    if ( orgData.isEmpty() ) return;

    QModelIndexList idxlist = orgData.keys();
    Q_FOREACH (QModelIndex idx, idxlist) {
        myModel->setDataDirect(idx, orgData.value(idx), false);
    }
    myModel->signalDataChanged(minIndex, maxIndex);
}

void PasteCommand::redo()
{
    if ( newData.isEmpty() ) return;

    QModelIndexList idxlist = newData.keys();
    Q_FOREACH (QModelIndex idx, idxlist) {
        myModel->setDataDirect(idx, newData.value(idx), false);
    }
    myModel->signalDataChanged(minIndex, maxIndex);
}


DeleteCommand::DeleteCommand(MySortFilterProxyModel *pr_model, const QModelIndexList &idxlist,
                             QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myModel = qobject_cast<CustomTableModel *>(pr_model->sourceModel());

    int minRow = myModel->rowCount() - 1;
    int maxRow = 0;
    int minCol = myModel->columnCount() - 1;
    int maxCol = 0;
    Q_FOREACH (QModelIndex idx, idxlist) {
        QModelIndex s_idx = pr_model->mapToSource(idx);
//        ( myModel->dataType(s_idx)==NODATATYPE ) ?
//            orgData.insert( s_idx, myModel->data( s_idx,Qt::EditRole) ) :
//            orgData.insert( s_idx, myModel->getData( s_idx.row(),s_idx.column() ) );

        if ( myModel->isDisableItem(s_idx) ) continue; ///////////////////////

        DATATYPE type = myModel->dataType(s_idx);
        if ( type == DATATYPE::TPFLOOR || type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP ) {
            orgData.insert( s_idx, myModel->getData( s_idx.row(), s_idx.column() ) );
        } else {
            orgData.insert( s_idx, myModel->data( s_idx, Qt::EditRole) );
        }
        maxRow = qMax(maxRow, s_idx.row());
        minRow = qMin(minRow, s_idx.row());
        maxCol = qMax(maxCol, s_idx.column());
        minCol = qMin(minCol, s_idx.column());
    }
    minIndex = myModel->index(minRow, minCol);
    maxIndex = myModel->index(maxRow, maxCol);
}

void DeleteCommand::undo()
{
    QModelIndexList idxlist = orgData.keys();
    Q_FOREACH (QModelIndex idx, idxlist) {
        myModel->setDataDirect(idx, orgData.value(idx), false);
    }
    myModel->signalDataChanged(minIndex, maxIndex);
}

void DeleteCommand::redo()
{
    myModel->removeItems(orgData.keys());
}


SetDataCommand::SetDataCommand(CustomTableModel *model, const QModelIndex &idx,
                               const QVariant &a_var, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myModel = model;
    editIndex = idx;
    orgValue = myModel->data( idx, Qt::EditRole);
    newValue = a_var;
}

void SetDataCommand::undo()
{
    myModel->setDataDirect(editIndex, orgValue);
}

void SetDataCommand::redo()
{
    myModel->setDataDirect(editIndex, newValue);
}

SetRowCountCommand::SetRowCountCommand(CustomTableModel *model, int count, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myModel = model;
    org_count = myModel->rowCount();
    new_count = count;
}

void SetRowCountCommand::undo()
{
    myModel->setRowCount(org_count);
}

void SetRowCountCommand::redo()
{
    myModel->setRowCount(new_count);
}


SetMultiCommand::SetMultiCommand(CustomTableModel *model, const QModelIndexList &idxlist,
                                 const QList<QVariant> &varlist, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    myModel = model;

    int minRow = myModel->rowCount() - 1;
    int maxRow = 0;
    int minCol = myModel->columnCount() - 1;
    int maxCol = 0;
    for ( int i = 0; i < idxlist.count(); i++ ) {
        QModelIndex idx = idxlist.at(i);
//        orgData.insert( idx, myModel->data(idx,Qt::EditRole) );
//        newData.insert( idx, varlist.at(i) );
        maxRow = qMax(maxRow, idx.row());
        minRow = qMin(minRow, idx.row());
        maxCol = qMax(maxCol, idx.column());
        minCol = qMin(minCol, idx.column());
        DATATYPE type = myModel->dataType(idx);
        if ( type == DATATYPE::TPFLOOR || type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP ) {
            orgData.insert( idx, myModel->getData(idx.row(), idx.column()) );
            QUuid uid = UnifiedInputData::getInInstance()->nameToID(varlist.at(i).toString(), type);
            ( uid.isNull() ) ? newData.insert( idx, QVariant() ) :
            newData.insert( idx, QVariant(uid) );
        } else {
            orgData.insert( idx, myModel->data(idx, Qt::EditRole) );
            newData.insert( idx, varlist.at(i) );
        }
    }
    minIndex = myModel->index(minRow, minCol);
    maxIndex = myModel->index(maxRow, maxCol);
}

void SetMultiCommand::undo()
{
    if ( orgData.isEmpty() ) return;

    QModelIndexList idxlist = orgData.keys();
    Q_FOREACH (QModelIndex idx, idxlist) {
        myModel->setDataDirect(idx, orgData.value(idx), false);
    }
    myModel->signalDataChanged(minIndex, maxIndex);
}

void SetMultiCommand::redo()
{
    if ( newData.isEmpty() ) return;

    QModelIndexList idxlist = newData.keys();
    Q_FOREACH (QModelIndex idx, idxlist) {
        myModel->setDataDirect(idx, newData.value(idx), false);
    }
    myModel->signalDataChanged(minIndex, maxIndex);
}
} // namespace post3dapp
