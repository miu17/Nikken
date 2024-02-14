#include "framesetting_tablemodel.h"

#include <QtGui>
#include <QMessageBox>

#include "unified_inputdata.h"
namespace post3dapp{
FrameSettingTableModel::FrameSettingTableModel(const CustomTableDataType &type,QObject *parent)
    : CustomTableModel(type,parent)
{
    columnTYPE = 2;
}

FrameSettingTableModel::FrameSettingTableModel(const FrameSettingTableModel& model)
    : CustomTableModel(model)
{
    columnTYPE = model.columnTYPE;
    minValueList = QHash<QString, int>{model.minValueList};
}


QVariant FrameSettingTableModel::data(const QModelIndex &index, int role) const
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
    if (role == Qt::TextColorRole){
        if (isInvalidColumn(row, col)){
            return QColor(Qt::lightGray);
        }
    }

    return CustomTableModel::data(index, role);
}

Qt::ItemFlags FrameSettingTableModel::flags(const QModelIndex &index) const
{
    if ( index.column() == myType.getTaggedColumn("GID") ) return Qt::NoItemFlags;
    return CustomTableModel::flags(index);
}

void FrameSettingTableModel::removeItems(const QModelIndexList &idxlist)
{
    QModelIndexList flist;
    Q_FOREACH (QModelIndex idx, idxlist) {
        if (idx.column() < myType.getTaggedColumn("GID") && !isBlockedItem(idx)) flist.append(idx);
        if (!isBlockedItem(idx)) flist.append(idx);
    }
    CustomTableModel::removeItems(flist);
}

void FrameSettingTableModel::setDataDirect(const QModelIndex &idx, const QVariant &value,
                                           bool signalOn)
{
    int row = idx.row();
    int col = idx.column();

    if (isBlockedItem(idx)) return;

    if ( col == myType.getTaggedColumn("FRAMESUM") ) {

        QString gid = allItems.at(row, myType.getTaggedColumn("GID")).toString();
        int count = value.toInt();
        int gcol = myType.getTaggedColumn("GID");

        if ( gid.isEmpty() && count > 0 ) {
            QUuid uu = QUuid::createUuid();
            allItems.replaceOneData(row, gcol, QVariant(uu.toString()));
            minValueList.insert(uu.toString(), 0);
            emit dataChanged(index(row, gcol), index(row, gcol));
        }

        if ( count < minValueList.value(gid) ) return;

        if ( !gid.isEmpty() && count == 0 ) {
            emit gidRemoved(gid);
            allItems.emptyOneData(row, gcol);
            minValueList.remove(gid);
            emit dataChanged(index(row, gcol), index(row, gcol));
        }

    }

    CustomTableModel::setDataDirect(idx, value, signalOn);

    if (idx.column() == columnTYPE && signalOn)
        emit dataChanged(index(row, myType.getTaggedColumn("OX")),
                         index(row, myType.getTaggedColumn("LENGTHMINUS")));
}

void FrameSettingTableModel::setRowDataDirect(int row, const QList<QVariant> &list, bool)
{
    int count = list.at( myType.getTaggedColumn("FRAMESUM") ).toInt();
    if ( count > 0) {
        QString gid = list.at( myType.getTaggedColumn("GID") ).toString();
        minValueList.insert(gid, 0);
    }
    CustomTableModel::setRowDataDirect(row, list);
}


bool FrameSettingTableModel::removeRows ( int row, int count, const QModelIndex &parent )
{
    Q_UNUSED(parent);
    if (row < 0) return false;

    for (int i = row; i < row + count; i++) {
        QString gid = allItems.at(i, myType.getTaggedColumn("GID")).toString();
        minValueList.remove(gid);
        emit gidRemoved(gid);
    }

    allItems.removeRows(row, count);
    if ( allItems.rowCount() != data_count ) setDataCount();
    setRowCount(row_count - count);

    emit dataChanged(index(row, 0), index(row + count - 1, COLUMNS - 1));
    return true;
}

bool  FrameSettingTableModel::isBlockedItem(const QModelIndex &index) const
{
    int row = index.row();
    int col = index.column();
    int icol = myType.getTaggedColumn("GID");
    if ( col == icol ) return true;

    QUuid gid = allItems.at(row, icol).toString();
    if ( !isEditOn() ) {
        if ( UnifiedInputData::getInInstance()->isExistedID(gid, DATATYPE::TPFRAMEG) ) return true;
    }

    if (isInvalidColumn(row, col))
        return true;

    return false;
}

bool FrameSettingTableModel::isRegisterRow(int row) const
{
    int icol = myType.getTaggedColumn("GID");
    QUuid gid = allItems.at(row, icol).toString();
    return UnifiedInputData::getInInstance()->isExistedID(gid, DATATYPE::TPFRAMEG);
}

QList<QVariant> FrameSettingTableModel::getRowDataOfGID(const QString &gid )
{
    QList<QVariant> vlist = allItems.getColumnData(myType.getTaggedColumn("GID"));
    int row = vlist.indexOf(QVariant(gid));
    return getRowData(row);
}

QStringList FrameSettingTableModel::getGIDList() const
{
    QList<QVariant> vlist = allItems.getColumnData(myType.getTaggedColumn("GID"));
    QStringList gidlist;
    Q_FOREACH (QVariant var, vlist) {
        QUuid id = QUuid(var.toString());
        if ( !id.isNull() ) gidlist.append(id.toString());
    }
    return gidlist;
}

QString FrameSettingTableModel::getGIDOfRow( int row ) const
{
    return allItems.at(row, myType.getTaggedColumn("GID")).toString();
}

int FrameSettingTableModel::getRowOfGID( const QString &gid ) const
{
    return allItems.getColumnData(myType.getTaggedColumn("GID")).indexOf(QVariant(gid));
}

FRAMEGROUPVALUE FrameSettingTableModel::getFrameGroupItemOfGID( const QString &gid ) const
{
    int row = getRowOfGID(gid);
    qreal po_x = 0.0;
    if (!allItems.isValid(row, myType.getTaggedColumn("OX"))){
        if (default_values.contains(myType.getTaggedColumn("OX")))
            po_x = myType.getDefaultValue("OX").toDouble();
    }else{
        po_x = allItems.at(row, myType.getTaggedColumn("OX")).toDouble();
    }

    qreal po_y = 0.0;
    if (!allItems.isValid(row, myType.getTaggedColumn("OY"))){
        if (default_values.contains(myType.getTaggedColumn("OX")))
            po_y = myType.getDefaultValue("OY").toDouble();
    }else{
        po_y = allItems.at(row, myType.getTaggedColumn("OY")).toDouble();
    }
    QPointF po(po_x, po_y);

    qreal s_angle = 0.0;
    if (!allItems.isValid(row, myType.getTaggedColumn("STARTANGLE"))){
        if (default_values.contains(myType.getTaggedColumn("ENDANGLE")))
            s_angle =  myType.getDefaultValue("STARTANGLE").toDouble();
    }else{
        s_angle = allItems.at(row, myType.getTaggedColumn("STARTANGLE")).toDouble();
    }

    qreal e_angle = 0.0;
    if (!allItems.isValid(row, myType.getTaggedColumn("ENDANGLE"))){
        if (default_values.contains(myType.getTaggedColumn("ENDANGLE")))
            e_angle =  myType.getDefaultValue("ENDANGLE").toDouble();
    }else{
        e_angle = allItems.at(row, myType.getTaggedColumn("ENDANGLE")).toDouble();
    }

    QString str_ftype = allItems.at(row, myType.getTaggedColumn("FRAMETYPE")).toString();
    int i_type = limited_strings.value(myType.getTaggedColumn("FRAMETYPE")).indexOf(str_ftype) + 1;
    FrameType::FRAMETYPE f_type = static_cast<FrameType::FRAMETYPE>(i_type);
    if ( f_type == FrameType::FRAMETYPE::STRAIGHT ) {
        if ( s_angle / 360.0 > 1.0 || s_angle / 360.0 < -1.0 ) {
            s_angle = s_angle - int(s_angle / 360.0) * 360.0;
        }
        if ( s_angle < -225.0 ) {
            s_angle += 360.0;
        } else if ( s_angle < -45.0 && s_angle >= -225.0 ) {
            s_angle += 180.0;
        } else if ( s_angle <= 315.0 && s_angle > 135.0 ) {
            s_angle -= 180.0;
        } else if ( s_angle > 315.0 ) {
            s_angle -= 360.0;
        }
    } else if ( f_type == FrameType::FRAMETYPE::CIRCLE || f_type == FrameType::FRAMETYPE::RADIUS || f_type == FrameType::FRAMETYPE::DIAMETER ) {
        if ( s_angle / 360.0 > 1.0 ||  s_angle / 360.0 < -1.0 ) {
            s_angle = s_angle - int(s_angle / 360.0) * 360.0;
        }
        if ( e_angle / 360.0 > 1.0 ||  e_angle / 360.0 < -1.0 ) {
            e_angle = e_angle - int(e_angle / 360.0) * 360.0;
        }
    }

    int div = 0;
    if (!allItems.at(row, myType.getTaggedColumn("DIVISIONS")).isValid()){
        if (default_values.contains(myType.getTaggedColumn("DIVISIONS")))
            div = myType.getDefaultValue("DIVISIONS").toInt();
    }else{
        div = allItems.at(row, myType.getTaggedColumn("DIVISIONS")).toInt();
    }

    qreal p_len = 0.0;
    if ( !allItems.at(row, myType.getTaggedColumn("LENGTHPLUS")).isValid() ){
        if (default_values.contains(myType.getTaggedColumn("LENGTHPLUS")))
            p_len = myType.getDefaultValue("LENGTHPLUS").toDouble();
    }
    else{
        p_len = allItems.at(row, myType.getTaggedColumn("LENGTHPLUS")).toDouble();
    }

    qreal m_len = 0.0;
    if ( !allItems.at(row, myType.getTaggedColumn("LENGTHMINUS")).isValid() ){
        if (default_values.contains(myType.getTaggedColumn("LENGTHMINUS")))
            m_len = myType.getDefaultValue("LENGTHMINUS").toDouble();
    }
    else{
        m_len = allItems.at(row, myType.getTaggedColumn("LENGTHMINUS")).toDouble();
    }

    QUuid stid = allItems.at(row, myType.getTaggedColumn("FLOORFROM")).toString();
    QUuid enid = allItems.at(row, myType.getTaggedColumn("FLOORTO")).toString();
//    QString uid=allItems.at(row,myType.getTaggedColumn("UUID")).toString();

    QString name = allItems.at(row, myType.getTaggedColumn("FRAMEGROUP")).toString();
    int sum = allItems.at(row, myType.getTaggedColumn("FRAMESUM")).toInt();
    return FRAMEGROUPVALUE(name, f_type, sum, po, s_angle, e_angle, div,
                           p_len, m_len, stid, enid, gid);
}

void FrameSettingTableModel::setMinValueOfFrameCount(const QString &gid, int value)
{
    minValueList.insert(gid, value);
}

bool FrameSettingTableModel::isInvalidColumn(int row, int col) const{
    int tcol = myType.getTaggedColumn("FRAMETYPE");
    QString ftype = allItems.at(row, tcol).toString();
    FrameType::FRAMETYPE itype = static_cast<FrameType::FRAMETYPE>(limited_strings.value(tcol).indexOf(ftype) + 1);
    if ( itype == FrameType::FRAMETYPE::STRAIGHT ) {
        if ( col == myType.getTaggedColumn("ENDANGLE")
                || col == myType.getTaggedColumn("DIVISIONS") ) return true;
    } else if ( itype == FrameType::FRAMETYPE::CIRCLE ) {
        if ( col >= myType.getTaggedColumn("LENGTHPLUS")
                && col <= myType.getTaggedColumn("LENGTHMINUS") ) return true;
    } else if ( itype == FrameType::FRAMETYPE::RADIUS ) {
        if ( col == myType.getTaggedColumn("ENDANGLE")
                || col == myType.getTaggedColumn("DIVISIONS")
                || col == myType.getTaggedColumn("LENGTHMINUS") ) return true;
    } else if ( itype == FrameType::FRAMETYPE::DIAMETER ) {
        if ( col == myType.getTaggedColumn("ENDANGLE")
                || col == myType.getTaggedColumn("DIVISIONS") ) return true;
    } else if ( itype == FrameType::FRAMETYPE::VARIABLE ) {
        if ( col >= myType.getTaggedColumn("OX")
                && col <= myType.getTaggedColumn("LENGTHMINUS") ) return true;
    } else if ( itype == FrameType::FRAMETYPE::CLCIRCLE ) {
        if ( col == myType.getTaggedColumn("STARTANGLE")
                || col == myType.getTaggedColumn("ENDANGLE")
                || col == myType.getTaggedColumn("LENGTHPLUS")
                || col == myType.getTaggedColumn("LENGTHMINUS") ) return true;
    } else if ( itype == FrameType::FRAMETYPE::CLVARIABLE ) {
        if ( col >= myType.getTaggedColumn("OX")
                && col <= myType.getTaggedColumn("LENGTHMINUS") ) return true;
    }

    return false;
}
} // namespace post3dapp
