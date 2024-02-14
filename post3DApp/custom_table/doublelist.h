#ifndef DOUBLELIST_H
#define DOUBLELIST_H

#include <QVariant>
namespace post3dapp{
class DoubleList
{

public:

    explicit DoubleList(int count = 100);
    DoubleList(const DoubleList&);
    ~DoubleList();

    int columnCount() const;
    int rowCount() const;
    QList<QVariant> getRowData(int row) const;
    QList<QVariant> getColumnData(int column) const;
    QVariant at(int row, int column) const;

    void appendRowData(const QList<QVariant> &);
    void replaceRowData(int row, const QList<QVariant> &);

    void insertRows(int row, int count);
    void removeRows(int row, int count);
    void swapRows(int a_row, int b_row);
    void emptyRowData(int row);

    void replaceMatchingData(int column, const QString &, const QString &);
    void emptyColumnData(int column);

    void replaceOneData(int row, int column, const QVariant &);
    void emptyOneData(int row, int column);

    bool isValid(int row, int column) const;
    bool isValidRow(int row) const;
    bool isValidColumn(int column) const;
    bool isValidRange(int row, int a_column, int b_column) const;

    void clear();

private:

    QList< QList<QVariant> > allData;
    int column_count;

    bool isEmptyItem(const QVariant &) const;
    void clearEmptyRow();
    void appendEmptyRow(int row);
    QList<QVariant> emptyList(int count) const;

};
} // namespace post3dapp
#endif
