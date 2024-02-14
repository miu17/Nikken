#ifndef FRAMESETTINGSUBTABLEMODEL_H
#define FRAMESETTINGSUBTABLEMODEL_H

#include "customtable_model.h"
namespace post3dapp{
class FrameSettingSubTableModel : public CustomTableModel
{
    Q_OBJECT

public:

    FrameSettingSubTableModel(const CustomTableDataType &type, QObject *parent = 0);
    FrameSettingSubTableModel(const FrameSettingSubTableModel&);

    QVariant data(const QModelIndex &index, int role) const override;
    void setDataDirect(const QModelIndex &index, const QVariant &value, bool signalOn = true) override;
    void setRowDataDirect(int row, const QList<QVariant> &list, bool signal = true) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &index = QModelIndex()) override;
    void removeItems(const QModelIndexList &idxlist) override;
    bool removeRows(int row, int count, const QModelIndex &index = QModelIndex()) override;
    void swapRows(int a_row, int b_row) override;

    void deleteRow(int row);

    bool readTextData(QTextStream &infile, bool overwriteFlag = true,DATATYPE = DATATYPE::TPCOLUMN) override
    {
        bool ok = CustomTableModel::readTextData(infile, overwriteFlag);
        if (ok) setRowCount( allItems.rowCount() );
        return ok;
    }

    void setGroupName(int row, const QString &str1, const QString &str2);
    void setGIDDirect(int row, const QString &gid);

    QList<int> getRowsOfGID(const QString &gid ) const;
    int getGIDCountOfRow( int row ) const;
    qreal getSpanOfRow( int row ) const;
    qreal getSumSpanOfRow( int row ) const;
    qreal getSumAngleOfRow( int row ) const;
    QList<QPointF> getVariablePointsOfRow(int row) const;

    int getTaggedColumn(const QString &tag) const
    {
        return myType.getTaggedColumn(tag);
    }

    QString getFrameName( int row )const
    {
        return allItems.at(row, myType.nameColumn()).toString();
    }
    QString getGIDOfRow( int row ) const
    {
        return allItems.at(row, myType.getTaggedColumn("GID")).toString();
    }
    QString getUUIDOfRow( int row ) const
    {
        return allItems.at(row, myType.uuidColumn()).toString();
    }

    bool isValidRange(int row, int a_col, int b_col) const
    {
        return allItems.isValidRange(row, a_col, b_col);
    }

public slots:

signals:

    void gidDataChanged(const QString &);
    void rowDataChanged(const QString &);

protected:

    bool isBlockedItem(const QModelIndex &) const override;
    QBrush buttonBrush;

private:
    bool isInvalidCell(int, int) const;

};
} // namespace post3dapp
#endif
