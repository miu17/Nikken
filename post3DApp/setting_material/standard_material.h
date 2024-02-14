#ifndef STANDARDMATERIAL_H
#define STANDARDMATERIAL_H

#include <QHash>
#include <QStringList>
namespace post3dapp{
class StandardMaterialTable : public QHash<QString, QStringList>
{

public:

    // 以下、ファイル内の読み取り順に列挙型を定義
    enum class format { MT_TP = 0, MT_NM, MT_E, MT_G, MT_U1, MT_U2, MT_N,
                  MT_F, MT_TL, MT_TS, MT_TU, MT_CL, MT_CS, MT_CU, MT_SL, MT_SS, MT_SU,
                  MT_AL, MT_AS, MT_AU, MT_ALU, MT_ASU
                };


    StandardMaterialTable()
    {
        polulateList();
    }

    QStringList getMaterialNameList(const QString &) const;
    bool existsMaterialName( const QString & ) const;
    QString getMaterialValue(const QString &, format) const;

    void updateList()
    {
        polulateList();
    }

protected:

private:

    QHash<QString, QStringList> valueList;

    void polulateList();

};
} // namespace post3dapp
#endif
