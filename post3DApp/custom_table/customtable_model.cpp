#include "customtable_model.h"

#include <QtGui>
#include <QHash>
#include <QMessageBox>

#include "unified_inputdata.h"
namespace post3dapp{
CustomTableModel::CustomTableModel(const CustomTableDataType &type, QObject *parent)
    : QAbstractTableModel(parent), myType(type)
{
    row_count = 1;
    data_count = 0;
    filter_status = false;

    QLinearGradient gr1(0.0, 0.0, 0.0, 25.0);
    gr1.setColorAt(0.0, QColor(230, 230, 250));
    gr1.setColorAt(0.15, QColor(250, 250, 255));
    gr1.setColorAt(0.85, QColor(240, 240, 250));
    gr1.setColorAt(1.0, QColor(220, 220, 240));
    vheaderBrush = QBrush(gr1);

    COLUMNS = myType.count();
    header_strings = myType.getHeaderStrings();
    string_columns = myType.getStringColumns();
    int_columns = myType.getIntColumns();
    double_columns = myType.getDoubleColumns();
    disable_columns = myType.getDisableColumns();
    default_values = myType.getDefaultValues();
    limited_strings = myType.getLimitedStrings();
    relative_columns = myType.getRelativeColumns();
    reference_columns = myType.getReferenceColumns();

    QList<int> collist = reference_columns.keys();
    for (int i = 0; i < collist.count(); i++) {
        QStringList namelist;
        namelist.append("");
        setLimitedStrings(collist.at(i), namelist);
    }

    connect ( UnifiedInputData::getInInstance(), &UnifiedData::dataUpdated,
              this, &CustomTableModel::updateLimitedStrings );

    allItems = DoubleList(COLUMNS);
    previousList = allItems;
    previousRows = row_count;
}

CustomTableModel::CustomTableModel(const CustomTableModel& model){
    myType = model.myType;
    COLUMNS = model.COLUMNS;
    row_count = model.row_count;
    data_count = model.data_count;
    filter_status = model.filter_status;
    vheaderBrush = model.vheaderBrush;
    header_strings = model.header_strings;
    string_columns = QList<int>{model.string_columns};
    int_columns = QList<int>{model.int_columns};
    double_columns = QList<int>{model.double_columns};
    bool_columns = QList<int>{model.bool_columns};
    zero_columns = QList<int>{model.zero_columns};
    disable_columns = QList<int>{model.disable_columns};
    relative_columns = QHash<int, int>{model.relative_columns};
    default_values = QHash<int, QVariant>{model.default_values};
    limited_strings = QHash<int, QStringList>{model.limited_strings};
    reference_columns = QHash<int, DATATYPE>{model.reference_columns};
    allItems = DoubleList{model.allItems};
    previousList = DoubleList{model.previousList};
    previousRows = model.previousRows;
    editOn = model.editOn;
}


void CustomTableModel::linkUUID()
{
    int ncol = myType.nameColumn();
    int icol = myType.uuidColumn();
    if ( ncol < 0 || icol < 0 ) return;
    DATATYPE type = myType.dataType();
    for ( int i = 0; i < data_count; i++ ) {
        QString name = allItems.at(i, ncol).toString();
        if ( !name.isEmpty() ) {
            QUuid uu = UnifiedInputData::getInInstance()->nameToID(name, type);
            ( uu.isNull() ) ? allItems.emptyOneData(i, icol) :
                              allItems.replaceOneData(i, icol, QVariant(uu.toString())) ;
        }
    }
    emit dataChanged(index(0, icol), index(data_count - 1, icol));
}

void CustomTableModel::keepCurrentData()
{
    previousList = allItems;
    previousRows = rowCount();
}

void CustomTableModel::backPreviousData()
{
    allItems = previousList;

    beginResetModel();
    setDataCount();
    setRowCount( previousRows );
    endResetModel();
}

void CustomTableModel::updateLimitedStrings(DATATYPE up_type)
{
    QList<int> collist = reference_columns.keys();
    bool detected = false;
    for (int i = 0; i < collist.count() ; i++) {
        DATATYPE type = reference_columns.value(collist.at(i));
        if ( type != up_type ) continue;
        QStringList namelist = UnifiedInputData::getInInstance()->getNameList(type);
        namelist.prepend("");
        setLimitedStrings(collist.at(i), namelist);
        detected = true;
    }
    if ( detected ) {
        beginResetModel();
        clearUnknownID(up_type);
        emit dataChanged(QModelIndex(), QModelIndex());
        endResetModel();
    }
}

int CustomTableModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() ) return 0;
    return row_count;
}

int CustomTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COLUMNS;
}

void CustomTableModel::initializeModel()
{
    beginResetModel();
    previousList.clear();
    previousRows = 0;
    allItems = DoubleList(COLUMNS);
    setDataCount();
    setRowCount( 1 );
    endResetModel();
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if ( !index.isValid() ) return QVariant();

    if ( col == myType.uuidColumn() ) {
        QUuid id = allItems.at(row, col).toString();
        if ( !id.isNull() ) {
            if (role == Qt::BackgroundRole ) return QBrush(QColor(102, 153, 153));
            if (role == Qt::TextColorRole ) return QColor(220, 200, 200);
        }
        if ( role == Qt::DisplayRole )
            return ( id.isNull() ) ? QString() : id.toString() ;
    }

    if ( role == Qt::EditRole ) {
        if ( col == 0 ) return row + 1;

        if ( reference_columns.contains(col) ) {
            QString str = allItems.at(row, col).toString();
            DATATYPE dtype = reference_columns.value(col);
            if ( dtype == DATATYPE::TPFLOOR || dtype == DATATYPE::TPFRAMEG || dtype == DATATYPE::TPFRAMEP ) {
                return UnifiedInputData::getInInstance()->idToName(QUuid(str), reference_columns.value(col));
            }
            return str;
        }

        QVariant value = allItems.at(row, col);
        if ( ! value.isValid() ) return QVariant();
        if ( int_columns.contains(col) ) {
            return value;
        }
        if ( double_columns.contains(col) ) {
            return value;
        }
        return value.toString();
    }

    if ( role == Qt::DisplayRole ) {
        if ( col == 0 )
            return row + 1;

        QVariant value = allItems.at(row, col);

        if ( reference_columns.contains(col) ) {
            QString str = value.toString();
            DATATYPE dtype = reference_columns.value(col);
            if ( dtype == DATATYPE::TPFLOOR || dtype == DATATYPE::TPFRAMEG || dtype == DATATYPE::TPFRAMEP ) {
                return UnifiedInputData::getInInstance()->idToName(QUuid(str), reference_columns.value(col));
            }
            return str;
        }

        if ( !allItems.isValid(row, col) ) {
            if ( !allItems.isValidRow(row) ) return QVariant();
            if ( default_values.contains(col) ) {
                return default_values.value(col).toString();
            }
            if ( ! value.isValid() ) return QVariant();
        }

        if ( string_columns.contains(col) ) {
            return value.toString();
        }
        if ( int_columns.contains(col) ) return value.toInt();
        return QString("%1").arg(value.toDouble(), 0, 'f', 3);
    }

    if (role == Qt::TextAlignmentRole) {
        if ( !string_columns.contains(col) )
            return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        return Qt::AlignCenter;
    }

    if (role == Qt::BackgroundRole ) {
        if ( col == 0 ) return vheaderBrush;
        if ( isDisableItem(index) ) return QBrush(Qt::lightGray);
        return QBrush(Qt::white);
    }

    if (role == Qt::TextColorRole ) {
        if ( col == 0 ) {
            return ( filter_status ) ? QColor(Qt::blue) : QColor(Qt::black);
        } else {
            if (  editOn && !allItems.isValid(row, col) ){
                return QColor(Qt::lightGray);
            }
            if (editOn && isDisableItem(index)){
                return QColor(Qt::lightGray);
            }
            return QColor(Qt::black);
        }
    }

    return QVariant();

}

Qt::ItemFlags CustomTableModel::flags(const QModelIndex &index) const
{
    if ( !index.isValid() || index.column() == 0 )
        return QAbstractItemModel::flags(index);
    if ( index.column() == myType.uuidColumn() ) return Qt::NoItemFlags;
    if ( isDisableItem(index) ) return QAbstractItemModel::flags(index);

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return ( section < header_strings.count() ) ?
                    header_strings.at(section) : QVariant() ;
    }

    return QVariant();
}

bool CustomTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        int  col = index.column();

        if ( reference_columns.contains(col) ) {
            DATATYPE dtype = reference_columns.value(col);
            if ( dtype == DATATYPE::TPFLOOR || dtype == DATATYPE::TPFRAMEG || dtype == DATATYPE::TPFRAMEP ) {
                QUuid id = UnifiedInputData::getInInstance()->nameToID(value.toString(), reference_columns.value(col));
                emit itemEdited(index, QVariant(id));
                return true;
            }
        }


        emit itemEdited(index, value);
        return true;
    }

    return false;
}

void CustomTableModel::setDataDirect(const QModelIndex &index, const QVariant &value, bool signalOn)
{
    int  row = index.row();
    int  col = index.column();
    if ( col == 0 || col >= COLUMNS ) return;
    if ( isBlockedItem(index) ) return;

    if ( reference_columns.contains(col) ) {
        DATATYPE dtype = reference_columns.value(col);
        if ( dtype == DATATYPE::TPFLOOR || dtype == DATATYPE::TPFRAMEG || dtype == DATATYPE::TPFRAMEP ) {
            QUuid id = value.toString();
            ( id.isNull() ) ? allItems.emptyOneData(row, col) :
                              allItems.replaceOneData(row, col, QVariant(id)) ;
            if ( allItems.rowCount() != data_count ) setDataCount();
            if (signalOn) emit dataChanged(index, index);
            return;
        }
    }

    if ( value.toString().isEmpty() && value.toList().isEmpty()) {
        allItems.emptyOneData(row, col);
        if ( allItems.rowCount() != data_count ) setDataCount();
        if (signalOn) emit dataChanged(index, index);
        return;
    }

    QString str = value.toString();
    if ( limited_strings.contains(col) ) {
        if ( ! limited_strings.value(col).contains(str) ) return;
    }

    if ( int_columns.contains(col) ) {
        bool ok;
        str.toInt(&ok);
        if ( !ok) return;
    }

    if ( double_columns.contains(col) ) {
        bool ok;
        str.toDouble(&ok);
        if ( !ok) return;
    }

    if ( bool_columns.contains(col) ) {
        if ( value.toBool() != true && value.toBool() != false ) return;
    }

    allItems.replaceOneData(row, col, value);
    if ( allItems.rowCount() != data_count ) setDataCount();
    if (signalOn) emit dataChanged(index, index);
}

void CustomTableModel::setRowDataDirect(int row, const QList<QVariant> &list,  bool signalOn)
{
    allItems.replaceRowData(row, list);
    if (signalOn) emit dataChanged(index(row, 0), index(row, COLUMNS - 1));
}

bool CustomTableModel::insertRows ( int row, int count, const QModelIndex & )
{
    if (row < 0) return false;
    if (count < 1) return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);
    allItems.insertRows(row, count);
    endInsertRows();

    setRowCount(row_count + count);
    if ( allItems.rowCount() != data_count ) setDataCount();
    return true;
}

bool CustomTableModel::removeRows ( int row, int count, const QModelIndex & )
{
    if (row < 0) return false;
    if (count < 1) return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    allItems.removeRows(row, count);
    endRemoveRows();

    if ( allItems.rowCount() != data_count ) setDataCount();
    setRowCount(row_count - count);
    return true;
}

void CustomTableModel::swapRows(int a_row, int b_row)
{
    if ( b_row > rowCount() - 1 ) return;
    if ( a_row == allItems.rowCount() - 1 && b_row > allItems.rowCount() - 1 ) {
        insertRows(a_row, b_row - a_row);
        removeRows(b_row + 1, b_row - a_row);
    } else {
        allItems.swapRows(a_row, b_row);
    }

    if ( allItems.rowCount() != data_count ) setDataCount();
    ( a_row > b_row ) ? emit dataChanged(index(b_row, 0), index(a_row, COLUMNS - 1))
                      : emit dataChanged(index(a_row, 0), index(b_row, COLUMNS - 1));
}

void CustomTableModel::removeItems(const QModelIndexList &idxlist)
{
    int minRow = rowCount() - 1;
    int maxRow = 0;
    int minCol = columnCount() - 1;
    int maxCol = 0;
    Q_FOREACH (QModelIndex idx, idxlist) {
        if ( !isBlockedItem(idx) ) {
            int row = idx.row();
            int col = idx.column();
            maxRow = qMax(maxRow, row);
            minRow = qMin(minRow, row);
            maxCol = qMax(maxCol, col);
            minCol = qMin(minCol, col);
            allItems.emptyOneData(row, col);
        }
    }

    if ( maxCol < 1 ) return;
    emit dataChanged(index(minRow, minCol), index(maxRow, maxCol));
    if ( allItems.rowCount() != data_count ) setDataCount();
}

void CustomTableModel::removeItem(const QModelIndex &idx)
{
    if ( !isBlockedItem(idx) ) {
        int row = idx.row();
        int col = idx.column();
        allItems.emptyOneData(row, col);
        emit dataChanged(idx, idx);
        if ( allItems.rowCount() != data_count ) setDataCount();
    }
}

void CustomTableModel::setRowCount(int num)
{
    beginResetModel();
    row_count = num;
    emit rowCountChanged(row_count);
    endResetModel();
}

void CustomTableModel::setDataCount()
{
    data_count = allItems.rowCount();
    emit dataCountChanged(data_count);
}


bool CustomTableModel::isDisableItem(const QModelIndex &index) const
{
    if ( disable_columns.contains( index.column()) ) return true;
    return isBlockedItem(index);
}

bool  CustomTableModel::isBlockedItem(const QModelIndex &index) const
{
    int icol = myType.uuidColumn();
    if (icol < 0) return false;
    if (index.column() == icol) return true;

    int row = index.row();
    int col = index.column();
    QUuid id = allItems.at(row, icol).toString();

    if ( editOn == false && !id.isNull() ) return true;
    if ( editOn == true && !id.isNull() ) {
        if ( col == icol ) return true;
        return false;
    }
    return false;
}

void CustomTableModel::clearUnknownID(DATATYPE up_type)
{
    QList<int> collist = reference_columns.keys();

    for ( int i = 0; i < collist.count(); i++ ) {

        int col = collist.at(i);
        DATATYPE dtype = reference_columns.value(col);

        if ( dtype != up_type ) continue;

        for ( int row = 0 ; row < data_count ; row++ ) {
            if ( dtype == DATATYPE::TPFLOOR || dtype == DATATYPE::TPFRAMEG || dtype == DATATYPE::TPFRAMEP ) {
                QUuid id = allItems.at(row, col).toString();
                if ( !UnifiedInputData::getInInstance()->isExistedID(id, dtype) )
                    allItems.emptyOneData(row, col);
            } else {
                QString str = allItems.at(row, col).toString();
                if ( !UnifiedInputData::getInInstance()->isExistedName(str, dtype) )
                    allItems.emptyOneData(row, col);
            }
        }
    }
}

void CustomTableModel::replaceMatchingData( int col, const QString &str_a, const QString &str_b )
{
    beginResetModel();
    allItems.replaceMatchingData(col, str_a, str_b);
    previousList.replaceMatchingData(col, str_a, str_b);
    endResetModel();
}

bool CustomTableModel::checkTextData( QTextStream& infile ) const
{
    bool chk;
    int f_rows,f_cols;

    QStringList line = infile.readLine().split(' ');
    if( line.count() != 2 ) return false;

    f_rows=line[0].toInt(&chk);
    if( !chk ) return false;

    f_cols=line[1].toInt(&chk);
    if( !chk ) return false;

    QStringList file_tags=infile.readLine().split('\t');
    if( file_tags.count()!=f_cols ) return false;

    for(int i=0; i < f_rows ; i++ ){
        line = infile.readLine().split('\t');
        if( line.count()!=f_cols ) return false;
    }
    return true;
}

void CustomTableModel::writeTextData( QTextStream &outfile, const QList<int> &excludeColumns )
{
    Q_UNUSED(excludeColumns);

    QString str = QString("%1,%2").arg(rowCount()).arg(columnCount());
    outfile << str << Qt::endl;
    str = "";

    QHash<QString, int> coltag = myType.getTaggedColumns();
    for (int i = 0; i < columnCount(); i++) {
        if ( i != 0 ) str += ",";
        str += coltag.key(i);
    }
    outfile << str << Qt::endl;
    str = "";

    for (int i = 0; i < rowCount() ; i++ ) {
        QList<QVariant> rlist = allItems.getRowData(i);
        qDebug()<<rlist;
        for ( int j = 0 ; j < columnCount() ; j++ ) {
            if ( j != 0 ) str += ",";
            QString strd = rlist.at(j).toString();
            DATATYPE dtype = myType.dataType();
            if(dtype == DATATYPE::TPFLOOR){
                if(j == 0){
                    QList<QUuid> idlist = UnifiedInputData::getInInstance()->getIDList(DATATYPE::TPFLOOR);
                    strd = QString::number(idlist.indexOf(QUuid(rlist.at(4).toString())));
                }
            }else if(dtype == DATATYPE::TPFRAMEG){
                if(j == 11 || j == 12){
                    QList<QUuid> idlist = UnifiedInputData::getInInstance()->getIDList(DATATYPE::TPFLOOR);
                    strd = QString::number(idlist.indexOf(QUuid(rlist.at(j).toString())));
                }else if(j == 0){
                    QList<QUuid> idlist = UnifiedInputData::getInInstance()->getIDList(DATATYPE::TPFRAMEG);
                    strd = QString::number(idlist.indexOf(QUuid(rlist.at(13).toString())));
                }
            }else if(dtype == DATATYPE::TPFRAMEP){
                if(j == 8){
                    QList<QUuid> idlist = UnifiedInputData::getInInstance()->getIDList(DATATYPE::TPFRAMEG);
                    strd = QString::number(idlist.indexOf(QUuid(rlist.at(j).toString())));
                }else if(j == 0){
                    QList<QUuid> idlist = UnifiedInputData::getInInstance()->getIDList(DATATYPE::TPFRAMEP);
                    strd = QString::number(idlist.indexOf(QUuid(rlist.at(9).toString())));
                }
            }else if(dtype == DATATYPE::TPAFLOOR){
                if(j == 1){
                    strd = UnifiedInputData::getInInstance()->idToName(QUuid(rlist.at(j).toString()),DATATYPE::TPFLOOR);
                }
            }else{
                if((j == 2 || j == 3) && QUuid(rlist.at(j).toString()) != QUuid()){
                    QList<QUuid> idlist = UnifiedInputData::getInInstance()->getIDList(DATATYPE::TPFLOOR);
                    strd = QString::number(idlist.indexOf(QUuid(rlist.at(j).toString())));
                }
             }

            str += strd;
        }
        outfile << str << Qt::endl;
        str = "";
    }
 }

void CustomTableModel::reflectUnifiedData(const DoubleList &doubleList){
    allItems = doubleList;
    setDataCount();
}
bool CustomTableModel::readTextData( QTextStream &infile, bool overwriteFlag ,DATATYPE dtype)
{
    bool chk;
    int f_rows, f_cols;
    static QList<QUuid> framegrouplist;

    QStringList line = infile.readLine().split(',');
    //if ( line.count() != 2 ) return false;

    f_rows = line[0].toInt(&chk);
    if ( !chk ) return false;

    f_cols = line[1].toInt(&chk);
    if ( !chk ) return false;

    beginResetModel();
    QStringList file_tags = infile.readLine().split(',');
    //if ( file_tags.count() != f_cols ) return false;

    QHash<QString, int> coltag = myType.getTaggedColumns();
    QStringList cur_tags;
    for ( int i = 0; i < columnCount(); i++ ) cur_tags.append( coltag.key(i) );

    DoubleList dlist = DoubleList( columnCount() );
    for (int i = 0; i < f_rows ; i++ ) {
        line = infile.readLine().split(',');
        //if ( line.count() != f_cols ) return false;
        QList<QVariant> rlist;
        for (int j = 0; j < columnCount(); j++) {
            int idx = file_tags.indexOf( cur_tags.at(j) );
            ////
            if(file_tags.at(idx)=="FLOOR" ||file_tags.at(idx)=="FLOORFROM"||file_tags.at(idx)=="FLOORTO"){
                if(QUuid(line.at(idx))==QUuid()){
                    QList<QUuid> idlist = UnifiedInputData::getInInstance()->getIDList(DATATYPE::TPFLOOR);
                    if(line.at(idx).toInt() < idlist.count() && line.at(idx).toInt() >= 0){
                        line[idx] = UnifiedInputData::getInInstance()->nameToID(line.at(idx),DATATYPE::TPFLOOR).toString();
                    }else{
                        line[idx] = QUuid::createUuid().toString();
                    }
                }
            }else if(file_tags.at(idx)=="GID"){
                if(dtype == DATATYPE::TPFRAMEP){
                    line[idx] = framegrouplist.value(line.at(idx).toInt()).toString();
                    qDebug()<<line.at(idx);
                }else if(dtype == DATATYPE::TPFRAMEG){//QUuid(line.at(idx))==QUuid()){
//                        QList<QUuid> idlist = FactoryUnifiedData::getInstance(isOutput)->getIDList(TPFRAMEG);
//                        if(line.at(idx).toInt() < idlist.count() && line.at(idx).toInt() >= 0){
//                            line[idx] = idlist.at(line.at(idx).toInt()).toString();
//                        }else{
                    if(QUuid(line.at(idx))==QUuid()){
                        line[idx] = QUuid::createUuid().toString();
                    }
//                        }
                    framegrouplist.insert(line.at(0).toInt(),QUuid(line.at(idx)));
                }
                qDebug()<<"gid="<<line.at(idx);
            }
            ////
            // ファイル中のタグが見つからない場合
            if ( idx < 0 ) {
                rlist.append( QVariant() );
                continue;
            }
            QString str = line.at(idx);
//                if ( reference_columns.contains(j) ) {
//                    DATATYPE dtype = reference_columns.value(j);
//                    if ( dtype == TPFLOOR || dtype == TPFRAMEG || dtype == TPFRAMEP ) {
//                        QUuid uid = FactoryUnifiedData::getInstance(isOutput)->nameToID(str, reference_columns.value(j));
//                        str = ( uid.isNull() ) ? QString() : uid.toString() ;
//                    }
//                }
            ( str.isEmpty() ) ? rlist.append(QVariant()) : rlist.append(str);
        }
        dlist.replaceRowData(i, rlist);
    }

    if ( overwriteFlag ) {
        allItems = dlist;
    } else {
        int icount = allItems.rowCount();
        for (int i = 0; i < dlist.rowCount() ; i++ )
            allItems.replaceRowData(icount + i, dlist.getRowData(i));
    }

    setDataCount();
    if ( data_count > row_count ) setRowCount( data_count );
    endResetModel();

    setDataCount();
    return true;
}

bool CustomTableModel::checkBinaryData( QDataStream &infile ) const
{
    if ( infile.atEnd() ) return false;

    int rows;
    QStringList strlist;
    QList<QVariant> vlist;
    infile >> rows;

    if ( infile.atEnd() ) return false;
    infile >> strlist;
    int cols = strlist.count();

    for ( int i = 0; i < rows; i++ ) {
        if ( infile.atEnd() ) return false;
        infile >> vlist;
        if ( vlist.count() != cols ) return false;
    }
    return true;
}

void CustomTableModel::readBinaryData(QDataStream &infile)
{
    int rows;
    infile >> rows;
    setRowCount(rows);

    QStringList f_tags;
    infile >> f_tags;

    QHash<QString, int> coltag = myType.getTaggedColumns();
    QStringList c_tags;
    for (int i = 0; i < columnCount(); i++) c_tags.append( coltag.key(i) );

    QList<QVariant> vlist;
    for ( int i = 0 ; i < rowCount() ; i++ ) {
        infile >> vlist;
        QList<QVariant> c_vlist;
        for ( int j = 0; j < columnCount(); j++ ) {
            int idx = f_tags.indexOf( c_tags.at(j) );
            ( idx < 0 ) ? c_vlist.append( QVariant() ) : c_vlist.append( vlist.at(idx) ) ;
        }
        allItems.replaceRowData(i, c_vlist);
    }

    setDataCount();
}
void CustomTableModel::writeListTextData( QTextStream& outfile, const QList<int>& excludeColumns )
{
    QString str=QString("%1 %2\n").arg(rowCount()).arg(columnCount());

    QHash<QString,int> coltag=myType.getTaggedColumns();
    for(int i=0; i< columnCount(); i++){
        if( i!=0 ) str += "\t";
        str+=coltag.key(i);
    }
    str += "\n";

    for(int i=0; i < rowCount() ; i++ ){
        QList<QVariant> rlist=allItems.getRowData(i);
        for( int j=0 ; j < columnCount() ; j++ ){
            if( j!=0 ) str += "\t";
            QString strd=rlist.at(j).toString();
            if( reference_columns.contains(j) ){
                DATATYPE dtype=reference_columns.value(j);
                if( dtype==DATATYPE::TPFLOOR || dtype==DATATYPE::TPFRAMEG || dtype==DATATYPE::TPFRAMEP ){
                    strd=UnifiedInputData::getInInstance()->idToName(strd,reference_columns.value(j));
                }
            }
            if( !excludeColumns.contains(j) ) str += strd;
        }
        if( i!=rowCount()-1 ) str += "\n";
    }
    outfile << str << Qt::endl;
}

bool CustomTableModel::readListTextData( QTextStream& infile , bool overwriteFlag )
{
    bool chk;
    int f_rows,f_cols;

    QStringList line = infile.readLine().split(' ');
    if( line.count() != 2 ) return false;

    f_rows=line[0].toInt(&chk);
    if( !chk ) return false;

    f_cols=line[1].toInt(&chk);
    if( !chk ) return false;

    QStringList file_tags=infile.readLine().split('\t');
    if( file_tags.count()!=f_cols ) return false;

    QHash<QString,int> coltag=myType.getTaggedColumns();
    QStringList cur_tags;
    for( int i=0; i< columnCount(); i++ ) cur_tags.append( coltag.key(i) );

    DoubleList dlist=DoubleList( columnCount() );
    for(int i=0; i < f_rows ; i++ ){
        line = infile.readLine().split('\t');
        if( line.count()!=f_cols ) return false;
        QList<QVariant> rlist;
        for(int j=0; j < columnCount(); j++){
            int idx=file_tags.indexOf( cur_tags.at(j) );
            // ファイル中のタグが見つからない場合
            if( idx<0 ){
                rlist.append( QVariant() );
                continue;
            }
            QString str=line.at(idx);
            if( reference_columns.contains(j) ){
                DATATYPE dtype=reference_columns.value(j);
                if( dtype==DATATYPE::TPFLOOR || dtype==DATATYPE::TPFRAMEG || dtype==DATATYPE::TPFRAMEP ){
                    QUuid uid=UnifiedInputData::getInInstance()->nameToID(str,reference_columns.value(j));
                    str = ( uid.isNull() ) ? QString() : uid.toString() ;
                }
            }
            ( str.isEmpty() ) ? rlist.append(QVariant()) : rlist.append(str);
        }
        dlist.replaceRowData(i,rlist);
    }

    beginResetModel();
    if( overwriteFlag ){
        allItems=dlist;
    }else{
        int icount=allItems.rowCount();
        for(int i=0; i < dlist.rowCount() ; i++ )
            allItems.replaceRowData(icount+i,dlist.getRowData(i));
    }

    setDataCount();
    if( data_count > row_count ) setRowCount( data_count );
    endResetModel();

    return true;
}

void CustomTableModel::writeBinaryData(QDataStream &outfile)
{
    outfile << previousRows;

    QHash<QString, int> coltag = myType.getTaggedColumns();
    QStringList tags;
    for (int i = 0; i < columnCount(); i++) tags.append( coltag.key(i) );
    outfile << tags;

    for ( int i = 0 ; i < previousRows ; i++ ) {
        outfile << previousList.getRowData(i);
    }
}

QStringList CustomTableModel::getOutputTextList() const
{
    QStringList list;
    for (int i = 0; i < rowCount(); i++){
        QList<QVariant> rlist = allItems.getRowData(i);
        QString line;
        for(int j = 0; j < columnCount(); j++){
            if (j != 0) line += ",";
            line += rlist.at(j).toString();
        }
        line += "\n";
        list.append(line);
    }
    return list;
}

} // namespace post3dapp
