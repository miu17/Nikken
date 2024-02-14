#include "doublelist.h"
#include <QtGui>
namespace post3dapp{
DoubleList::DoubleList( int count )
{
    column_count=count;
}

DoubleList::DoubleList(const DoubleList& list){
    column_count = list.column_count;
    Q_FOREACH(auto l, list.allData){
        allData.append(QList<QVariant>{l});
    }
}


DoubleList::~DoubleList()
{
    allData.clear();
}

int DoubleList::columnCount() const
{
    return column_count;
}

int DoubleList::rowCount() const
{
    return allData.count();
}

QList<QVariant> DoubleList::getRowData(int row) const
{
    if( row >= rowCount() || row < 0 ) return emptyList( columnCount() );
    return allData.at(row);
}

QList<QVariant> DoubleList::getColumnData(int col) const
{
    if( col >= columnCount() || col < 0 ) return emptyList( rowCount() );
    QList<QVariant> cdata;
    Q_FOREACH(QList<QVariant> list, allData ) cdata.append( list.at(col) );
    return cdata;
}

QVariant DoubleList::at(int row, int col) const
{
    if( row >= rowCount() || col >= columnCount() ) return QVariant();
    if( row < 0 || col < 0 ) return QVariant();
    return allData.at(row).at(col);
}

void DoubleList::appendRowData(const QList<QVariant> &list)
{
    if( list.count() != columnCount() ) return;

    allData.append(list);
    Q_FOREACH(QVariant var, list ){ if( !isEmptyItem(var) ) return; }
    clearEmptyRow();
}

void DoubleList::replaceRowData(int row, const QList<QVariant> &list)
{
    if( list.count() != columnCount() ) return;

    int count = 0;
    qCount(list.begin(), list.end(), QVariant(), count);
    if( count == list.count() ) return;

    if( row+1 > rowCount() ) appendEmptyRow(row);
    allData.replace(row,list);
}

void DoubleList::insertRows(int row, int count)
{
    if( row >= rowCount() || row<0 ) return;
    QList<QVariant> list=emptyList( columnCount() );
    for (int i=0 ; i < count ; i++) allData.insert(row,list);
}

void DoubleList::removeRows(int row, int count)
{
    for (int i=row+count-1 ; i >=row ; i--){
        if( i < rowCount() ) allData.removeAt(i);
    }
    clearEmptyRow();
}

void DoubleList::swapRows(int a_row, int b_row)
{
    if( a_row >= rowCount() || b_row >= rowCount() ) return;
    if( a_row < 0 || b_row < 0 ) return;
    allData.swap(a_row,b_row);
    if( a_row == rowCount()-1 || b_row == rowCount()-1 ) clearEmptyRow();
}

void DoubleList::emptyRowData(int row)
{
    if( row >= rowCount() || row<0 ) return;
    allData.replace(row,emptyList( columnCount() ));
    if( row == rowCount()-1 ) clearEmptyRow();
}

void DoubleList::emptyColumnData(int col)
{
    for( int i=0 ; i < rowCount() ; i++ ){
        QList<QVariant> rlist=allData.at(i);
        rlist.replace(col,QVariant());
        allData.replace(i,rlist);
    }
    clearEmptyRow();
}

void DoubleList::replaceOneData(int row, int col, const QVariant &var)
{
    if( col >= columnCount() || col < 0 ) return;
    if( row >= rowCount() || row < 0 ){
        if( isEmptyItem(var) ) return;
        appendEmptyRow(row);
    }

    QList<QVariant> rlist=allData.at(row);
    rlist.replace(col,var);
    allData.replace(row,rlist);
    if( isEmptyItem(var) ) clearEmptyRow();
}

void DoubleList::replaceMatchingData(int col, const QString & str_a, const QString &str_b)
{
    if( col >= columnCount() || col < 0 ) return;
    for( int i=0 ; i < rowCount() ; i++ ){
        if( at(i,col).toString()==str_a ) replaceOneData(i,col,QVariant(str_b));
    }
}

void DoubleList::emptyOneData(int row, int col)
{
    if( row >= rowCount() || row<0 ) return;
    if( col >= columnCount() || col<0 ) return;
//    if( row < rowCount() && col < columnCount() )
    replaceOneData(row,col,QVariant());
}

bool DoubleList::isValid(int row, int col) const
{
//    if( row+1 > rowCount() || col+1 > columnCount() ) return false;
    if( row >= rowCount() || row<0 ) return false;
    if( col >= columnCount() || col<0 ) return false;
    return !isEmptyItem( at(row,col) );
}

bool DoubleList::isValidRow(int row) const
{
    if( row >= rowCount() || row<0 ) return false;
    QList<QVariant> list=getRowData(row);
    Q_FOREACH(QVariant var, list ){ if( !isEmptyItem(var) ) return true; }
    return false;
}

bool DoubleList::isValidColumn(int col) const
{
    if( col >= columnCount() || col<0 ) return false;
    QList<QVariant> list=getColumnData(col);
    Q_FOREACH(QVariant var, list ){ if( !isEmptyItem(var) ) return true; }
    return false;
}

bool DoubleList::isValidRange(int row, int a_col, int b_col) const
{
    if( row >= rowCount() || a_col > b_col ) return false;
    QList<QVariant> list=getRowData(row);
    for( int i=a_col ; i <= b_col ; i++ ){
        if( !isEmptyItem(list[i]) ) return true;
    }
    return false;
}

void DoubleList::clear()
{
    allData.clear();
}


void DoubleList::clearEmptyRow()
{
    for( int i=rowCount()-1 ; i >= 0 ; i-- ){
        if( isValidRow(i) ) return;
        allData.removeAt(i);
    }
}

void DoubleList::appendEmptyRow(int row)
{
    for( int i=rowCount() ; i < row+1 ; i++ )
        allData.append( emptyList( columnCount() ) );
}

QList<QVariant> DoubleList::emptyList(int count) const
{
    QList<QVariant> list;
    for( int i=0 ; i < count ; i++ ) list.append(QVariant());
    return list;
}

bool DoubleList::isEmptyItem(const QVariant & var) const
{
    return ( var.toString().isEmpty() && var.toList().isEmpty() ) ;
}
} // namespace post3dapp
