#include <qDebug>

#include "material_tablemodel.h"
#include "define_material_struct.h"
namespace post3dapp{
MaterialTableModel::MaterialTableModel(const MaterialDataType &type,QObject *parent)
    : CustomTableModel(type,parent), myType(type)
{
    default_value_patch = myType.getDefaultValuePatch();
    shapetype_columns = myType.getShapeTypeColumns();
}

MaterialTableModel::MaterialTableModel(const MaterialTableModel& model)
    : CustomTableModel(model)
{
    myType = model.myType;
    shapetype_columns = QList<int>{model.shapetype_columns};
    default_value_patch = QHash<int, QHash<int, QVariant>>{model.default_value_patch};
}


QVariant MaterialTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();


    if ( !index.isValid() ) return QVariant();

    if ( role == Qt::DisplayRole && col == 0 ) return row + 1;

    if ( role == Qt::EditRole ) {
        if ( col == 0 ) return row + 1;
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

        if (allItems.at(row, col).toString().isEmpty()) {

            int sp1 = myType.getSeparateColumn1();
            int rcol = relative_columns.value(col);
            if ( (rcol < sp1) || ( sp1 <= col && col < COLUMNS && sp1 <= rcol && rcol < COLUMNS) ) {
                if (!allItems.isValid(row, col) && allItems.isValid(row, rcol)) {
                    value = allItems.at(row, rcol);
                }
            }

            if ( ! allItems.isValidRow(row) ) return QVariant();
            if ( value.toString().isEmpty() && default_value_patch[row].keys().contains(col) ) {
                return default_value_patch[row].value(col).toString();
            } else if ( value.toString().isEmpty() && default_values.keys().contains(col) ) {
                return default_values.value(col).toString();
            }
            if ( value.toString().isEmpty()) return QVariant();
        }

        if ( string_columns.contains(col) ) return value.toString();
        if ( int_columns.contains(col) ) return value.toInt();

        if ( col == myType.getTaggedColumn("UNITWEIGHT1")
                || col == myType.getTaggedColumn("UNITWEIGHT2")
                || myType.getTaggedColumn("F_STRENGTH") <= col ) {
            return QString("%1").arg(value.toDouble(), 0, 'f', 2);
        } else {
            return QString("%1").arg(value.toDouble(), 0, 'f', 1);
        }
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

bool MaterialTableModel::isBlockedItem(const QModelIndex &index) const
{
    int row = index.row();
    int col = index.column();

    // カテゴリーによるDisable
    if (col < myType.getTaggedColumn("SYMBOL")) return false;
    QString category = allItems.at(row, myType.getTaggedColumn("CATEGORY")).toString();
    if (category.isEmpty()) {
        return false;
    }

    MaterialType::Type type = MaterialType::stringToEnum(category);
    if (myType.getTaggedColumn("SYMBOL") <= col && col <= myType.getTaggedColumn("RANGETO")){
        if (type == MaterialType::Type::CONCRETE || type == MaterialType::Type::STEEL){
            return true;
        }
        else{
            return false;
        }
    }

    // 規格断面によるDisable
    if ( !shapetype_columns.isEmpty() ) {
        QString label = allItems.at(row, myType.getTaggedColumn("LABEL")).toString();
        if ( label.isEmpty() ) return false;
        if ( myType.standardMaterialNameList(category).contains(label) ) return true;
        if (type == MaterialType::Type::STEEL || type == MaterialType::Type::REINFORCE){
           if (myType.getTaggedColumn("A_L") <= col)
               return true;
        }
    }
    return false;
}

void MaterialTableModel::setDataDirect(const QModelIndex &index, const QVariant &value,
                                       bool signalOn)
{
    int  row = index.row();
    int  col = index.column();
    if ( col == 0 || col >= COLUMNS ) return;

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
        if ( !ok || temp < 0) return;
    }

    if ( double_columns.contains(col) ) {
        bool ok;
        double temp = str.toDouble(&ok);
        if ( !ok || temp < 0) return;
    }

    if ( bool_columns.contains(col) ) {
        if ( value.toBool() != true && value.toBool() != false ) return;
    }
    allItems.replaceOneData(row, col, value);
    if ( allItems.rowCount() != data_count ) setDataCount();
    if (signalOn) emit dataChanged(index, index);
}

void MaterialTableModel::setDefaultValuePatch(int row, int col, const QVariant &dvalue)
{
    if (!dvalue.toString().isEmpty()) {
        default_value_patch[row].insert(col, dvalue);
    } else {
        default_value_patch[row].remove(col);
    }
    emit defaultValuePatchChanged(row, col, dvalue);
}

QList<MATERIALVALUES> MaterialTableModel::getMaterialList() const
{
    QList<MATERIALVALUES> mvlist;
    for ( int i = 0; i < data_count; i++ ) {
        if ( !allItems.at(i, myType.getTaggedColumn("LABEL")).toString().isEmpty() ) {
            mvlist.append( CreateMaterialValues( getRowData(i), myType ) );
        }
    }
    return mvlist;
}

MATERIALVALUES MaterialTableModel::CreateMaterialValues(const QList<QVariant> &myValues, const MaterialDataType &dataType)
{
    MATERIALVALUES mv;
    QHash<QString, int> coltag = dataType.getTaggedColumns();

    mv.materialType = MaterialType::stringToEnum( myValues.at(coltag["CATEGORY"]).toString() );
    mv.materialName = myValues.at(coltag["LABEL"]).toString();
    mv.symbol = ( myValues.at(coltag["SYMBOL"]).toString().isEmpty() )
             ? QString("D") : myValues.at(coltag["SYMBOL"]).toString() ;
    mv.barMin = myValues.at(coltag["RANGEFROM"]).toInt();
    mv.barMax = myValues.at(coltag["RANGETO"]).toInt();

    mv.E = myValues.at(coltag["E"]).toDouble();
    mv.G = myValues.at(coltag["G"]).toDouble();
    mv.unitW1 = myValues.at(coltag["UNITWEIGHT1"]).toDouble();
    mv.unitW2 = myValues.at(coltag["UNITWEIGHT2"]).toDouble();
    mv.modularRatio = myValues.at(coltag["MODULARRATIO"]).toDouble();

    mv.F = myValues.at(coltag["F_STRENGTH"]).toDouble();

    mv.Ft_L = myValues.at(coltag["T_L"]).toDouble();
    mv.Ft_S = myValues.at(coltag["T_S"]).toDouble();
    mv.Ft_U = myValues.at(coltag["T_U"]).toDouble();
    mv.Fc_L = myValues.at(coltag["C_L"]).toDouble();
    mv.Fc_S = myValues.at(coltag["C_S"]).toDouble();
    mv.Fc_U = myValues.at(coltag["C_U"]).toDouble();
    mv.Fs_L = myValues.at(coltag["S_L"]).toDouble();
    mv.Fs_S = myValues.at(coltag["S_S"]).toDouble();
    mv.Fs_U = myValues.at(coltag["S_U"]).toDouble();
    mv.Fa_L = myValues.at(coltag["A_L"]).toDouble();
    mv.Fa_S = myValues.at(coltag["A_S"]).toDouble();
    mv.Fa_U = myValues.at(coltag["A_U"]).toDouble();
    mv.Fa_Lu = myValues.at(coltag["UA_L"]).toDouble();
    mv.Fa_Su = myValues.at(coltag["UA_S"]).toDouble();
    return mv;
}
} // namespace post3dapp
