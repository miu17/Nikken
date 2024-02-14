#include "section_tablemodel.h"

#include <QMessageBox>

#include "define_section_struct.h"
#include "section_calculations.h"
#include "unified_inputdata.h"

namespace post3dapp{
SectionTableModel::SectionTableModel(const SectionDataType &type, QObject *parent)
    : CustomTableModel(type,parent), mySectionType(type)
{
    default_value_patch = mySectionType.getDefaultValuePatch();
    shapetype_columns = mySectionType.getShapeTypeColumns();
    if ( mySectionType.getMemberType() == DATATYPE::TPISO ) {
        standardList = SectionDataType::standardSectionFileList(StandardSectionTable::listType::ISOLATOR);
    } else {
        standardList = SectionDataType::standardSectionFileList(StandardSectionTable::listType::STEEL);
    }
}

SectionTableModel::SectionTableModel(const SectionTableModel &model)
    : CustomTableModel(model)
{
    mySectionType = model.mySectionType;
    shapetype_columns = QList<int>{model.shapetype_columns};
    standardList = model.standardList;
    default_value_patch = QHash<int, QHash<int, QVariant> >{model.default_value_patch};
}


QVariant SectionTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if ( !index.isValid() ) return QVariant();

    if ( role == Qt::DisplayRole && col == 0 ) return row + 1;

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
        if ( !value.isValid() ) return QVariant();
        if ( int_columns.contains(col) ) {
            return value;
        }
        if ( double_columns.contains(col) ) {
            return value;
        }
        return value.toString();
    }
    if ( role == Qt::DisplayRole ) {

        QVariant value = allItems.at(row, col);

        if ( reference_columns.contains(col) ) {
            QString str = value.toString();
            DATATYPE dtype = reference_columns.value(col);
            if ( dtype == DATATYPE::TPFLOOR || dtype == DATATYPE::TPFRAMEG || dtype == DATATYPE::TPFRAMEP ) {
                return UnifiedInputData::getInInstance()->idToName(QUuid(str), reference_columns.value(col));
            }
            return str;
        }

        if (allItems.at(row, col).toString().isEmpty()) {

            int sp1 = mySectionType.getSeparateColumn1();
            int sp2 = mySectionType.getSeparateColumn2();
            int sp3 = mySectionType.getSeparateColumn3();
            int viewcount;
            if (sp1 < 0) {
                viewcount = 1;
                sp1 = COLUMNS;
            } else if (sp2 < 0) {
                viewcount = 2;
                sp2 = COLUMNS;
            } else if (sp3 < 0) {
                viewcount = 3;
                sp3 = COLUMNS;
            } else {
                viewcount = 4;
            }

            if ( viewcount >= 3 && col >= sp2 && col < sp3) { //view2が空でview1に値があるとき
                if (!allItems.isValidRange(row, sp2, sp3 - 1)
                        && allItems.isValidRange(row, sp1, sp2 - 1)) {
                    value = allItems.at(row, relative_columns.value(col));
                }
            } else if (viewcount >= 4 && col >= sp3) { //view3が空でview1に値があるとき
                if (!allItems.isValidRange(row, sp3, COLUMNS - 1)
                        && allItems.isValidRange(row, sp1, sp2 - 1)) {
                    value = allItems.at(row, relative_columns.value(col));
                }
            }
            //そのコラムが空でrelativecolumnとコラムが同じviewあるいはmytableviewに所属していてrelativecolumnにデータがあるとき
            //relativecolumnのrelativecolumnは無限ループしそうなので未実装
            int rcol = relative_columns.value(col);
            if ((viewcount >= 1 && rcol < sp1)
                    || (viewcount >= 2 && col >= sp1 && col < sp2 && rcol >= sp1 && rcol < sp2)
                    || (viewcount >= 3 && col >= sp2 && col < sp3 && rcol >= sp2 && rcol < sp3)
                    || (viewcount >= 4 && col >= sp3 && col < COLUMNS && rcol >= sp3 && rcol < COLUMNS)) {
                if (!allItems.isValid(row, col) && allItems.isValid(row, rcol)) {
                    value = allItems.at(row, rcol);
                }
            }

            if ( !allItems.isValidRow(row) ) return QVariant();
            if ( value.toString().isEmpty() && default_value_patch[row].keys().contains(col) ) {
                return default_value_patch[row].value(col).toString();
            } else if ( value.toString().isEmpty() && default_values.keys().contains(col) ) {
                return default_values.value(col).toString();
            }
            if ( value.toString().isEmpty()) return QVariant();
        }

        if ( string_columns.contains(col) ) return value.toString();
        if ( int_columns.contains(col) ) return value.toInt();
        return QString("%1").arg(value.toDouble(), 0, 'f', 1);
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
            return ( filter_status ) ? QVariant(QColor(Qt::blue)) : QVariant(QColor(Qt::black));
        } else {
            if ( ! allItems.isValid(row, col) ) return QVariant(QColor(Qt::lightGray));
            return QVariant(QColor(Qt::black));
        }
    }
    return QVariant();
}

bool SectionTableModel::isBlockedItem(const QModelIndex &index) const
{
    int row = index.row();
    int col = index.column();

    //規格断面によるDisable
    if ( !shapetype_columns.isEmpty() ) {
        QHash<QString, int> coltag = mySectionType.getTaggedColumns();
        if ( shapetype_columns.contains(col - 1) ) {
            QModelIndex idx = index.model()->index(row, col - 1);
            if ( !standardList.contains(idx.data().toString()) ) return true;
        }

        for ( int i = 2; i < 6; i++ ) {
            if ( !shapetype_columns.contains(col - i) ) continue;
            QModelIndex idx = index.model()->index(row, col - i);
            if ( standardList.contains(idx.data().toString()) ) return true;
        }
    }

    return false;
}

void SectionTableModel::setDataDirect(const QModelIndex &index, const QVariant &value,
                                      bool signalOn)
{
    int  row = index.row();
    int  col = index.column();
    if ( col == 0 || col >= COLUMNS ) return;

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

    QString str = value.toString();
    if ( str.isEmpty() && allItems.isValid(row, col) ) {
        allItems.emptyOneData(row, col);
        if ( allItems.rowCount() != data_count ) setDataCount();
        if (signalOn) emit dataChanged(index, index);
        return;
    }

    if ( limited_strings.contains(col) ) {
        if ( !limited_strings.value(col).contains(str) &&
                !shapetype_columns.contains(col - 1) ) return;
    }

    if ( int_columns.contains(col) ) {
        bool ok;
        int temp = str.toInt(&ok);
        if ( !ok) return;
    }

    if ( double_columns.contains(col) ) {
        bool ok;
        double temp = str.toDouble(&ok);
        if ( !ok) return;
    }

    if ( bool_columns.contains(col) ) {
        if ( value.toBool() != true && value.toBool() != false ) return;
    }
    allItems.replaceOneData(row, col, value);
    if ( allItems.rowCount() != data_count ) setDataCount();
    if ( signalOn ) emit dataChanged(index, index);
}

void SectionTableModel::setDefaultValuePatch(int row, int col, const QVariant &dvalue)
{
    if (!dvalue.toString().isEmpty()) {
        default_value_patch[row].insert(col, dvalue);
    } else {
        default_value_patch[row].remove(col);
    }
    emit defaultValuePatchChanged(row, col, dvalue);
}
} // namespace post3dapp
