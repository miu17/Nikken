#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H

#include <QAbstractTableModel>
#include <QBrush>
#include "define_customtable.h"
#include "doublelist.h"

class QTextStream;

namespace post3dapp{
enum class DATATYPE;


class CustomTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    CustomTableModel(const CustomTableDataType &type, QObject *parent = 0);
    CustomTableModel(const CustomTableModel&);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual void setDataDirect(const QModelIndex &index, const QVariant &value, bool signal = true);
    virtual void setRowDataDirect(int row, const QList<QVariant> &list, bool signal = true);

    virtual bool insertRows(int row, int count, const QModelIndex &index = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex &index = QModelIndex());
    virtual void swapRows(int a_row, int b_row);
    virtual void removeItems(const QModelIndexList &idxlist);
    virtual void removeItem(const QModelIndex &idx);

    /* setData() は不可　removeItem() は可 */
    virtual bool isDisableItem(const QModelIndex &) const;
    /* allItem への直接操作以外では編集不可 */
    virtual bool isBlockedItem(const QModelIndex &) const;

    QVariant getData(int row, int col) const
    {
        return allItems.at(row, col);
    }
    QList<QVariant> getRowData(int row) const
    {
        return allItems.getRowData(row);
    }
    QList<QVariant> getColumnData(int col) const
    {
        return allItems.getColumnData(col);
    }

    DATATYPE dataType(const QModelIndex &idx) const
    {
        return (reference_columns.contains(idx.column())) ?
               reference_columns.value(idx.column()) : DATATYPE::NODATATYPE;
    }

    void setLimitedStrings(int col, const QStringList &strlist)
    {
        limited_strings.insert(col, strlist);
    }

    virtual bool checkTextData( QTextStream & ) const;
    virtual bool readTextData(QTextStream &, bool overwriteFlag = true , DATATYPE dtype =DATATYPE::TPCOLUMN);
    virtual void writeTextData(QTextStream &, const QList<int> &excludeColumns = QList<int>() );
    virtual bool readListTextData( QTextStream &, bool overwriteFlag = true );
    void reflectUnifiedData(const DoubleList &);
    virtual void writeListTextData(QTextStream &, const QList<int> &excludeColumns = QList<int>() );

    virtual bool checkBinaryData( QDataStream & ) const;
    virtual void readBinaryData( QDataStream & );
    virtual void writeBinaryData( QDataStream & );

    void setRowCount(int);
    void setDataCount();
    int dataCount()
    {
        return data_count;
    }
    void initializeModel();

    void signalDataChanged(const QModelIndex &a_idx, const QModelIndex &b_idx)
    {
        emit dataChanged(a_idx, b_idx);
    }
    void signalDataChanged(int row)
    {
        emit dataChanged(index(row, 0), index(row, COLUMNS - 1));
    }

    void setFilterStatus(bool flg)
    {
        beginResetModel();
        filter_status = flg;
        endResetModel();
    }

    void linkUUID();
    void setEditOn(bool ok)
    {
        beginResetModel();
        editOn = ok;
        endResetModel();
    }
    bool isEditOn() const
    {
        return editOn;
    }

    virtual bool isRegisterRow(int row) const
    {
        return ( allItems.at(row, myType.uuidColumn()).isValid() );
    }
    bool existRegisterRow() const
    {
        return ( allItems.isValidColumn(myType.uuidColumn()) );
    }

    void keepCurrentData();
    void backPreviousData();

    void replaceMatchingData(int, const QString &, const QString &);

    virtual QStringList getOutputTextList() const;

public slots:

private slots:

    void updateLimitedStrings(DATATYPE);

signals:

    void rowCountChanged(int);
    void dataCountChanged(int);
    void itemEdited(const QModelIndex &, const QVariant &);

protected:

    CustomTableDataType myType;

    int COLUMNS;
    int row_count;
    int data_count;
    bool filter_status;
    QBrush vheaderBrush;

    QStringList header_strings;

    QList<int> string_columns;
    QList<int> int_columns;
    QList<int> double_columns;
    QList<int> bool_columns;
    QList<int> zero_columns;
    QList<int> disable_columns;
    QHash<int, int> relative_columns;
    QHash<int, QVariant> default_values;
    QHash<int, QStringList> limited_strings;
    QHash<int, DATATYPE> reference_columns;

    DoubleList allItems;
    DoubleList previousList;
    int previousRows;
    bool editOn;

    void clearUnknownID(DATATYPE);
};
} // namespace post3dapp
#endif
