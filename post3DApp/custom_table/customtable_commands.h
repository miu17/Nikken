#ifndef COMMANDSTABLE_H
#define COMMANDSTABLE_H

#include <QModelIndex>
#include <QUndoCommand>

namespace post3dapp{
class CustomTableModel;
class MySortFilterProxyModel;

class InsertLineCommand : public QUndoCommand
{
public:
    InsertLineCommand(MySortFilterProxyModel *model, int row,
                      int count, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    CustomTableModel *myModel;
    int insertedRow;
    int insertedCount;
};


class DeleteLineCommand : public QUndoCommand
{
public:
    DeleteLineCommand(MySortFilterProxyModel *model, int row,
                      int count, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    CustomTableModel *myModel;
    QList< QList<QVariant> > deletedData;
    int deletedRow;
    int deletedCount;
};


class MoveLineCommand : public QUndoCommand
{
public:
    MoveLineCommand(MySortFilterProxyModel *model,
                    int a_row, int b_row, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    CustomTableModel *myModel;
    int fromRow;
    int toRow;
};


class PasteCommand : public QUndoCommand
{
public:
    PasteCommand(MySortFilterProxyModel *model, const QModelIndex &idx, const QString &str,
                 QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    CustomTableModel *myModel;
    QHash<QModelIndex, QVariant> orgData;
    QHash<QModelIndex, QVariant> newData;
    QModelIndex minIndex;
    QModelIndex maxIndex;
};


class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(MySortFilterProxyModel *model, const QModelIndexList &idxlist,
                  QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    CustomTableModel *myModel;
    QHash<QModelIndex, QVariant> orgData;
    QModelIndex minIndex;
    QModelIndex maxIndex;
};

class SetDataCommand : public QUndoCommand
{
public:
    SetDataCommand(CustomTableModel *model, const QModelIndex &idx,
                   const QVariant &a_var, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    CustomTableModel *myModel;
    QModelIndex editIndex;
    QVariant orgValue;
    QVariant newValue;
};

class SetRowCountCommand : public QUndoCommand
{
public:
    SetRowCountCommand(CustomTableModel *model, int count, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    CustomTableModel *myModel;
    int org_count;
    int new_count;
};

class SetMultiCommand : public QUndoCommand
{
public:
    SetMultiCommand(CustomTableModel *model, const QModelIndexList &idxlist,
                    const QList<QVariant> &vlist, QUndoCommand *parent = nullptr);

    void undo();
    void redo();

private:
    CustomTableModel *myModel;
    QHash<QModelIndex, QVariant> orgData;
    QHash<QModelIndex, QVariant> newData;
    QModelIndex minIndex;
    QModelIndex maxIndex;
};
} // namespace post3dapp
#endif
