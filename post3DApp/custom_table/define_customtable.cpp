#include "define_customtable.h"

#include "define_defaultvalue.h"
namespace post3dapp{
CustomTableDataType::CustomTableDataType()
    : nameCol(-1), uuidCol(-1) { }


QStringList CustomTableDataType::getHeaderStrings() const
{
    return headerList;
}

QList<int> CustomTableDataType::getStringColumns() const
{
    return stringColumns;
}
QList<int> CustomTableDataType::getIntColumns() const
{
    return intColumns;
}
QList<int> CustomTableDataType::getDoubleColumns() const
{
    return doubleColumns;
}
QList<int> CustomTableDataType::getDisableColumns() const
{
    return disableColumns;
}
QHash<int, QVariant> CustomTableDataType::getDefaultValues() const
{
    return defaultValues;
}
QHash<int, int> CustomTableDataType::getRelativeColumns() const
{
    return relativeColumns;
}
QHash<int, QStringList> CustomTableDataType::getLimitedStrings() const
{
    return limitedStrings;
}
QHash<int, DATATYPE> CustomTableDataType::getReferenceColumns() const
{
    return referenceColumns;
}

QHash<QString, int> CustomTableDataType::getTaggedColumns() const
{
    return taggedColumnNumber;
}
int CustomTableDataType::getTaggedColumn(const QString &tag) const
{
    return taggedColumnNumber[tag];
}

int CustomTableDataType::count() const
{
    return COLUMNS;
}
DATATYPE CustomTableDataType::dataType() const
{
    return myDataType;
}
int CustomTableDataType::nameColumn() const
{
    return nameCol;
}
int CustomTableDataType::uuidColumn() const
{
    return uuidCol;
}
void CustomTableDataType::populateList( CREGULATION *reg )
{
    for (int i = 0; i < COLUMNS ; i++) {

        headerList.append(reg->header);

        if ( reg->cellType == TABLEDATATYPE::TB_STRING ) {
            stringColumns.append(i);
        } else if ( reg->cellType == TABLEDATATYPE::TB_INT ) {
            intColumns.append(i);
        } else if ( reg->cellType == TABLEDATATYPE::TB_DOUBLE ) {
            doubleColumns.append(i);
        }

        if ( reg->defValue.isValid() ) {
            defaultValues.insert( i, reg->defValue);
        }

        if ( reg->relative > 0 ) {
            relativeColumns.insert( i, reg->relative);
        }

        if ( !reg->tag.isEmpty() ) {
            taggedColumnNumber[reg->tag] = i;
        }

        ++reg;
    }
}

QVariant CustomTableDataType::getDefaultValue(const QString& name) const{
    if (taggedColumnNumber.keys().contains(name)){
        int column = getTaggedColumn(name);
        if (defaultValues.contains(column))
            return defaultValues.value(column);
        else
            return QVariant();
    }
    else{
        return QVariant();
    }
}

CREGULATION::CREGULATION(const QString &_header, const QMap<QString, REGULATION> &defaultMap, const QString &_tag, int _relative)
    :header(_header), tag(_tag), relative(_relative)
{
    REGULATION reg = defaultMap.value(_tag);
    cellType = reg.cellType;
    defValue = reg.defValue;
}

} // namespace post3dapp
