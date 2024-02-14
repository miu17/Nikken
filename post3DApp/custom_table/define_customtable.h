#ifndef CUSTOMTABLEDATATYPE_H
#define CUSTOMTABLEDATATYPE_H

#include <QVariant>
#include "define_unifieddata.h"

namespace post3dapp{
enum class TABLEDATATYPE;
struct REGULATION;

struct CREGULATION{
    QString header;
    QString tag;
    TABLEDATATYPE cellType;
    QVariant defValue;
    int relative;

    CREGULATION(const QString& _header,
                const QMap<QString, REGULATION>& defaultMap,
                const QString& _tag,
                int _relative = -1);
};

/* カスタムテーブル　抽象クラス */
class CustomTableDataType
{

public:

    CustomTableDataType();
    QStringList getHeaderStrings()const;
    QList<int> getStringColumns() const;
    QList<int> getIntColumns() const;
    QList<int> getDoubleColumns() const;
    QList<int> getDisableColumns() const;
    QHash<int, QVariant> getDefaultValues() const;
    QHash<int, int> getRelativeColumns() const;
    QHash<int, QStringList> getLimitedStrings() const;
    QHash<int, DATATYPE> getReferenceColumns() const;
    QHash<QString, int> getTaggedColumns() const;
    int getTaggedColumn(const QString &tag) const;
    int count() const;
    DATATYPE dataType() const;
    int nameColumn() const;
    int uuidColumn() const;
    QVariant getDefaultValue(const QString&) const;

protected:

    QStringList headerList;
    int COLUMNS;
    DATATYPE myDataType;
    int nameCol, uuidCol;
    QList<int> stringColumns, intColumns, doubleColumns, disableColumns;
    QHash<int, QVariant> defaultValues;
    QHash<int, int> relativeColumns;
    QHash<int, QStringList> limitedStrings;
    QHash<int, DATATYPE> referenceColumns;
    QHash<QString, int> taggedColumnNumber;

    void populateList( CREGULATION *reg );
};
} // namespace post3dapp
#endif
