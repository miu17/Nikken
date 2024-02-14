#ifndef DEFINE_SECTIONTABLE_H
#define DEFINE_SECTIONTABLE_H

#include "define_customtable.h"
#include "standard_section.h"

/* 断面タイプ　抽象クラス */
namespace post3dapp{
class SectionDataType : public CustomTableDataType
{

public:

    SectionDataType();
    int getSeparateColumn1() const;
    int getSeparateColumn2() const;
    int getSeparateColumn3() const;

    QList<int> getShapeTypeColumns() const;
    QList<int> getDirectInputColumns() const;

    STRUCTTYPE getStructType() const;
    DATATYPE getMemberType() const;


    //規格断面　リスト情報
    static QStringList standardSectionFileList(StandardSectionTable::listType type);
    static QStringList standardSectionNameList(const QString &fname);
    static bool standardSectionNameExists(const QString &sname);
    static QString standardSteelSectionName( const QString &bname, const QString &sname );
    static QString standardSteelSectionValue(const QString &sname, StandardSectionTable::SteelFormat fm);
    static QString standardBaseSectionValue(const QString &sname, StandardSectionTable::BaseFormat fm);
    static QString standardIsolatorSectionValue(const QString &sname,
                                                StandardSectionTable::IsolatorFormat fm);

    //行データに対し、指定したテーブルの入力有無のチェック
    bool hasData(const QList<QVariant> &myValues, int tableNo) const;
    QHash<int, QHash<int, QVariant> > getDefaultValuePatch() const;
    QHash<int, QVariant> getLineDefaultValues(int row) const;
    void setDefaultValuePatch(int row, int col, const QVariant &dvalue);

protected:

    STRUCTTYPE mySectionType;

    int spCol1, spCol2, spCol3;
    QList<int> shapeTypeColumns;
    QList<int> directInputColumns;

    static const StandardSectionTable *standardSection;
    QHash<int, QHash<int, QVariant> > defaultValuePatch;

};


/* 柱断面：ＲＣ */
class ColumnRCTable : public SectionDataType
{

public:

    ColumnRCTable();
};


/* 柱断面：Ｓ */
class ColumnSTable : public SectionDataType
{

public:

    ColumnSTable();

};


/* 柱断面：ＳＲＣ */
class ColumnSRCTable : public SectionDataType
{

public:

    ColumnSRCTable();

};

/* 柱断面：ＣＦＴ */
class ColumnCFTTable : public SectionDataType
{

public:

    ColumnCFTTable();
};

/* 梁断面：ＲＣ */
class GirderRCTable : public SectionDataType
{

public:

    GirderRCTable();
};


/* 梁断面：Ｓ */
class GirderSTable : public SectionDataType
{

public:

    GirderSTable();
};


/* 梁断面：ＳＲＣ */
class GirderSRCTable : public SectionDataType
{

public:

    GirderSRCTable();
};


/* ブレース断面：Ｓ */
class BraceSTable : public SectionDataType
{

public:

    BraceSTable();
};


/* 小梁断面：ＲＣ */
class BeamRCTable : public SectionDataType
{

public:

    BeamRCTable();
};


/* 小梁断面：Ｓ */
class BeamSTable : public SectionDataType
{

public:

    BeamSTable();
};


/* 小梁断面：ＳＲＣ */
class BeamSRCTable : public SectionDataType
{

public:

    BeamSRCTable();
};

/* 壁断面：通常 */
class WallRegularTable : public SectionDataType
{

public:

    WallRegularTable();
};

/* 壁断面：鋼板壁 */
class WallSteelTable : public SectionDataType
{

public:

    WallSteelTable();
};

/* 壁断面：地下外壁 */
class WallUnderGroundTable : public SectionDataType
{

public:

    WallUnderGroundTable();
};

/* スラブ断面 */
class SlabTable : public SectionDataType
{

public:

    SlabTable();
};

/* S露出柱脚断面 */
class BaseSTable : public SectionDataType
{

public:

    BaseSTable();
};

/* SRC非埋め込み柱脚断面 */
class BaseSRCTable : public SectionDataType
{

public:

    BaseSRCTable();
};

/* 免震装置 */
class IsolatorTable : public SectionDataType
{

public:

    IsolatorTable();
};

/* 制振装置 */
class DamperTable : public SectionDataType
{

public:

    DamperTable();
};

/* 開口 */
class OpeningTable : public SectionDataType
{

public:

    OpeningTable();
};
} // namespace post3dapp
#endif // DEFINE_SECTIONTABLE_H
