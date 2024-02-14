#include "framesetting_subtablemodel.h"

#include <QtGui>
#include <QMessageBox>

namespace post3dapp{
FrameSettingSubTableModel::FrameSettingSubTableModel(const CustomTableDataType &type,
                                                     QObject *parent)
    : CustomTableModel(type, parent)
{
    row_count = 0;
    previousRows = 0;

    QLinearGradient gr1(0.0, 0.0, 0.0, 25.0);
    gr1.setColorAt(0.0, QColor(100, 100, 220));
    gr1.setColorAt(0.25, QColor(140, 140, 250));
    gr1.setColorAt(0.8, QColor(140, 140, 250));
    gr1.setColorAt(1.0, QColor(100, 100, 220));
    buttonBrush = QBrush(gr1);
}

FrameSettingSubTableModel::FrameSettingSubTableModel(const FrameSettingSubTableModel& model)
    : CustomTableModel(model)
{
    buttonBrush = model.buttonBrush;
}


QVariant FrameSettingSubTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if ( col == myType.getTaggedColumn("GID") ) {
        QUuid id = allItems.at(row, col).toString();
        if ( !id.isNull() ) {
            if (role == Qt::BackgroundRole ) return QBrush(QColor(102, 153, 153));
            if (role == Qt::TextColorRole ) return QColor(220, 200, 200);
        }
        if ( role == Qt::DisplayRole )
            return ( id.isNull() ) ? QString() : id.toString() ;
    }

    if ( col == myType.getTaggedColumn("BUTTON") ) {
        if (role == Qt::DisplayRole ) return u8"点列入力";
        if (role == Qt::BackgroundRole ) return buttonBrush;
        if (role == Qt::TextColorRole ) return QVariant(QColor(Qt::white));
    }

    if ( col == myType.getTaggedColumn("FRAMEGROUP")){
        if (role == Qt::TextColorRole) return QVariant(QColor(Qt::black));
    }

    if ( col == myType.getTaggedColumn("FRAMETYPE")){
        if (role == Qt::TextColorRole) return QVariant(QColor(Qt::black));
    }

    if (isInvalidCell(row, col) && role == Qt::TextColorRole)
         return QColor(Qt::lightGray);

    return CustomTableModel::data(index, role);
}

Qt::ItemFlags FrameSettingSubTableModel::flags(const QModelIndex &index) const
{
    if ( index.column() == myType.getTaggedColumn("BUTTON") ) return Qt::NoItemFlags;
    if ( index.column() == myType.getTaggedColumn("GID") ) return Qt::NoItemFlags;
    if ( index.column() > 0
            && index.column() <= myType.getTaggedColumn("FRAMETYPE") ) return Qt::NoItemFlags;
    return CustomTableModel::flags(index);
}

void FrameSettingSubTableModel::removeItems(const QModelIndexList &idxlist)
{
    QModelIndexList flist;
    Q_FOREACH (QModelIndex idx, idxlist) {
        if ( idx.column() > myType.getTaggedColumn("FRAMETYPE")
                && idx.column() != myType.getTaggedColumn("BUTTON")
                && !isBlockedItem(idx) ) flist.append(idx);
    }
    if (flist.isEmpty())return;
    QString gid = allItems.at(flist.first().row(), myType.getTaggedColumn("GID")).toString();
    CustomTableModel::removeItems(flist);
    emit gidDataChanged(gid);
}

void FrameSettingSubTableModel::setDataDirect(const QModelIndex &index, const QVariant &value,
                                              bool signalOn)
{
    if (isBlockedItem(index)) return;
    QString gid = allItems.at(index.row(), myType.getTaggedColumn("GID")).toString();
    CustomTableModel::setDataDirect(index, value, signalOn);
    emit gidDataChanged(gid);
}

void FrameSettingSubTableModel::setRowDataDirect(int row, const QList<QVariant> &list, bool signal)
{
    QString gid1 = allItems.at(row, myType.getTaggedColumn("GID")).toString();
    CustomTableModel::setRowDataDirect(row, list);
    QString gid2 = allItems.at(row, myType.getTaggedColumn("GID")).toString();
    emit gidDataChanged(gid1);
    emit rowDataChanged(gid1);
    emit gidDataChanged(gid2);
    emit rowDataChanged(gid2);
}

bool FrameSettingSubTableModel::insertRows ( int row, int count, const QModelIndex & )
{
    if (row < 0) return false;

    QVariant var1 = allItems.at(row, myType.getTaggedColumn("FRAMEGROUP"));
    QVariant var2 = allItems.at(row, myType.getTaggedColumn("FRAMETYPE"));
    QVariant var3 = allItems.at(row, myType.getTaggedColumn("GID"));

    allItems.insertRows(row, count);
    setRowCount(row_count + count);
    if ( allItems.rowCount() != data_count ) setDataCount();

    for (int i = 0; i < count; i++) {
        allItems.replaceOneData(row + i, myType.getTaggedColumn("FRAMEGROUP"), var1);
        allItems.replaceOneData(row + i, myType.getTaggedColumn("FRAMETYPE"), var2);
        allItems.replaceOneData(row + i, myType.getTaggedColumn("GID"), var3);
    }

    emit dataChanged(index(row, 0), index(row + count - 1, COLUMNS - 1));
    emit gidDataChanged(var3.toString());
    emit rowDataChanged(var3.toString());
    return true;
}

bool FrameSettingSubTableModel::removeRows ( int row, int count, const QModelIndex &parent )
{
    QString gid = allItems.at(row, myType.getTaggedColumn("GID")).toString();
    bool flg = CustomTableModel::removeRows(row, count, parent );
    emit gidDataChanged(gid);
    emit rowDataChanged(gid);
    return flg;
}

void FrameSettingSubTableModel::deleteRow ( int row )
{
    allItems.removeRows(row, 1);
    if ( allItems.rowCount() != data_count ) setDataCount();
    setRowCount(row_count - 1);
    emit dataChanged(index(row, 0), index(row, COLUMNS - 1));
}

void FrameSettingSubTableModel::swapRows(int a_row, int b_row)
{
    QString gid = allItems.at(a_row, myType.getTaggedColumn("GID")).toString();
    CustomTableModel::swapRows(a_row, b_row);
    emit gidDataChanged(gid);
    emit rowDataChanged(gid);
}

bool  FrameSettingSubTableModel::isBlockedItem(const QModelIndex &idx) const
{
    int col = idx.column();
    int row = idx.row();
    if (col == myType.getTaggedColumn("BUTTON")) return true;
    if (col == myType.getTaggedColumn("GID") ) return true;
    if (col == myType.getTaggedColumn("UUID")) return true;
    if (col > 0 && col <= 2) return true;

    if (isInvalidCell(row, col))
        return true;

    return ( !isEditOn() ) ;
}

void FrameSettingSubTableModel::setGroupName(int row, const QString &str1, const QString &str2)
{
    allItems.replaceOneData(row, myType.getTaggedColumn("FRAMEGROUP"), QVariant(str1));
    allItems.replaceOneData(row, myType.getTaggedColumn("FRAMETYPE"), QVariant(str2));
    emit dataChanged(index(row, myType.getTaggedColumn("FRAMEGROUP")), index(row,
                                                                             myType.getTaggedColumn("FRAMETYPE")));
}

void FrameSettingSubTableModel::setGIDDirect(int row, const QString &gid)
{
    int gcol = myType.getTaggedColumn("GID");
    allItems.replaceOneData(row, gcol, QVariant(gid));
    emit dataChanged(index(row, gcol), index(row, gcol));
}

QList<int> FrameSettingSubTableModel::getRowsOfGID(const QString &gid ) const
{
    QList<QVariant> vlist = allItems.getColumnData(myType.getTaggedColumn("GID"));
    int sum = vlist.count(QVariant(gid));
    int pre_row = 0;
    QList<int> rowlist;
    for (int i = 0; i < sum ; i++) {
        int row = vlist.indexOf(QVariant(gid), pre_row);
        rowlist.append(row);
        pre_row = row + 1;
    }
    return rowlist;
}

int FrameSettingSubTableModel::getGIDCountOfRow( int row ) const
{
    QVariant gid_var = allItems.at(row, myType.getTaggedColumn("GID"));
    QList<QVariant> vlist = allItems.getColumnData(myType.getTaggedColumn("GID"));
    if ( vlist.count() <= row ) return -1;
    int count = 0;
    for (int i = 0; i < row ; i++)
        if (vlist.at(i) == gid_var) count++;
    return count;
}

qreal FrameSettingSubTableModel::getSpanOfRow( int row ) const
{
    QString stype = allItems.at(row, myType.getTaggedColumn("FRAMETYPE")).toString();
    FrameType::FRAMETYPE itype = static_cast<FrameType::FRAMETYPE>(limited_strings.value(myType.getTaggedColumn("FRAMETYPE")).indexOf(stype) + 1);
    if ( itype != FrameType::FRAMETYPE::STRAIGHT && itype != FrameType::FRAMETYPE::CIRCLE && itype != FrameType::FRAMETYPE::CLCIRCLE ) return 0.0;
    return allItems.at(row, myType.getTaggedColumn("SPAN")).toDouble();
}

qreal FrameSettingSubTableModel::getSumSpanOfRow( int row ) const
{
    QString stype = allItems.at(row, myType.getTaggedColumn("FRAMETYPE")).toString();
    FrameType::FRAMETYPE itype = static_cast<FrameType::FRAMETYPE>(limited_strings.value(myType.getTaggedColumn("FRAMETYPE")).indexOf(stype) + 1);
    if ( itype != FrameType::FRAMETYPE::STRAIGHT && itype != FrameType::FRAMETYPE::CIRCLE && itype != FrameType::FRAMETYPE::CLCIRCLE ) return 0.0;

    QVariant gid_var = allItems.at(row, myType.getTaggedColumn("GID"));
    QList<QVariant> vlist = allItems.getColumnData(myType.getTaggedColumn("GID"));
    if ( vlist.count() <= row ) return 0.0;
    qreal span = 0.0;
    for (int i = 0; i < row ; i++)
        if (vlist.at(i) == gid_var) span += allItems.at(i, myType.getTaggedColumn("SPAN")).toDouble();
    if ( itype == FrameType::FRAMETYPE::CIRCLE
            || itype == FrameType::FRAMETYPE::CLCIRCLE ) span += allItems.at(row, myType.getTaggedColumn("SPAN")).toDouble();
    return span;
}

qreal FrameSettingSubTableModel::getSumAngleOfRow( int row ) const
{
    QVariant gid_var = allItems.at(row, myType.getTaggedColumn("GID"));
    QList<QVariant> vlist = allItems.getColumnData(myType.getTaggedColumn("GID"));
    if ( vlist.count() <= row ) return 0.0;
    qreal angle = 0.0;
    for (int i = 0; i < row ; i++)
        if (vlist.at(i) == gid_var) angle = angle + allItems.at(i,
                                                                    myType.getTaggedColumn("ANGLE")).toDouble();
    return angle;
}

QList<QPointF> FrameSettingSubTableModel::getVariablePointsOfRow(int row) const
{

    QList<QPointF> points;
    QString xystring = allItems.at(row, myType.getTaggedColumn("XYPOINTS")).toString();
    QStringList strlist = xystring.split(',');
    if ( strlist.last().isEmpty() ) strlist.removeLast();
    int scount = strlist.count();
    int count = ( scount % 2 == 0 ) ? scount / 2 : (scount + 1) / 2 ;
    bool ok1, ok2;
    for (int i = 0; i < count; i++) {
        qreal xx = strlist.at(i * 2).toDouble(&ok1);
        if (!ok1) xx = 0.0;
        qreal yy = ( i == count - 1 && scount % 2 != 0 ) ?
                   0.0 : strlist.at(i * 2 + 1).toDouble(&ok2);
        if (!ok2) yy = 0.0;
        points.append(QPointF(xx, yy));
    }
    return points;
}

bool FrameSettingSubTableModel::isInvalidCell( int row, int col ) const{
    QString gid = allItems.at(row, myType.getTaggedColumn("GID")).toString();
    int lastRow = -1;
    for (int i = 0; i < allItems.rowCount(); i++){
        if (allItems.at(i, myType.getTaggedColumn("GID")).toString() == gid)
            lastRow = i;
    }
    QString ftype = allItems.at(row, myType.getTaggedColumn("FRAMETYPE")).toString();
    FrameType::FRAMETYPE itype = static_cast<FrameType::FRAMETYPE>(limited_strings.value(myType.getTaggedColumn("FRAMETYPE")).indexOf(ftype) + 1);
    if (itype == FrameType::FRAMETYPE::STRAIGHT) {
        if (col == myType.getTaggedColumn("SPAN") &&row == lastRow)
            return true;
    }else if (itype == FrameType::FRAMETYPE::RADIUS || itype == FrameType::FRAMETYPE::DIAMETER){
        if (col == myType.getTaggedColumn("ANGLE") &&row == lastRow)
            return true;
    }

    return false;
}
} // namespace post3dapp
