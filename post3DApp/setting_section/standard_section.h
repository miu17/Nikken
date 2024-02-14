#ifndef STANDARDSECTION_H
#define STANDARDSECTION_H

#include <QHash>
#include <QStringList>
namespace post3dapp{
class StandardSectionTable : public QHash<QString, QStringList>
{

public:

    enum class listType {STEEL = 0, COLUMNSRCSTEEL, COLUMNSRCSTEELT, CFT, BASE, ISOLATOR};

    // 以下、ファイル内の読み取り順に列挙型を定義
    enum class SteelFormat { ST_TP = 0, ST_HB, ST_R, ST_A, ST_W,
                       ST_Ix, ST_Iy, ST_ix, ST_iy, ST_Zx, ST_Zy, ST_Cx
                     };
    enum class BaseFormat { BS_NAME = 0 };
    enum class IsolatorFormat { IS_NAME = 0, IS_HS, IS_K1, IS_K2, IS_K3, IS_F1p, IS_F1m, IS_F2p, IS_F2m,
                          IS_Kvc, IS_Kvt1, IS_Kvt2,
                          IS_Ft, IS_PN, IS_G, IS_FG, IS_R, IS_PR, IS_t, IS_Fr
                        };

    StandardSectionTable()
    {
        polulateList();
    }

    QStringList getSectionFileList(listType type = listType::STEEL) const;
    QStringList getSectionNameList(const QString &) const;

    bool existsSectionName( const QString & ) const;

    QString getSteelSectionValue(const QString &, SteelFormat) const;
    QString getBaseSectionValue(const QString &, BaseFormat) const;
    QString getIsolatorSectionValue(const QString &, IsolatorFormat) const;

    QString steelSectionName( const QString &, const QString & ) const;

    void updateList()
    {
        polulateList();
    }

protected:

private:

    QStringList steelFileList;
    QStringList columnSrcSteelFileList;
    QStringList columnSrcSteelTFileList;
    QStringList cftSteelFileList;
    QStringList baseFileList;
    QStringList isolatorFileList;
    QHash<QString, QStringList> valueList;

    void polulateList();

};
} // namespace post3dapp


#endif
