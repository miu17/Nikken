#ifndef DEFINE_MATERIALTABLE_H
#define DEFINE_MATERIALTABLE_H

#include "define_customtable.h"
#include "standard_material.h"

namespace post3dapp{

/* 断面タイプ　抽象クラス */
class MaterialDataType : public CustomTableDataType
{

public:

    MaterialDataType();

    int getSeparateColumn1() const;

    QList<int> getShapeTypeColumns() const;

    //規格断面　リスト情報
    static QStringList standardMaterialNameList(const QString &mtype);
    static bool standardMaterialNameExists(const QString &mname);
    static QString standardMaterialValue(const QString &mname, StandardMaterialTable::format fm);

    //行データに対し、指定したテーブルの入力有無のチェック
    bool hasData(const QList<QVariant> &myValues, int tableNo) const;
    QHash<int, QHash<int, QVariant> > getDefaultValuePatch() const;
    QHash<int, QVariant> getLineDefaultValues(int row) const;
    void setDefaultValuePatch(int row, int col, const QVariant &dvalue);

protected:

    int spCol1;
    QList<int> shapeTypeColumns;
    static const StandardMaterialTable *standardMaterial;
    QHash<int, QHash<int, QVariant> > defaultValuePatch;

};


class MaterialTable : public MaterialDataType
{

public:

    MaterialTable();

private:


};
} // namespace post3dapp
#endif
