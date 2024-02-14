#ifndef MANAGEMATERIALDATA_H
#define MANAGEMATERIALDATA_H

#include <QObject>
#include <QHash>
#include <QStringList>

#include "define_material_struct.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
enum class DATATYPE;
class N3DUuidTable;
class POST3D_CLASS_DLLSPEC ManageMaterialData : public QObject
{
    Q_OBJECT

public:

    ManageMaterialData( QObject *parent = nullptr) : QObject(parent) {}
    ManageMaterialData( const ManageMaterialData&, QObject *parent = nullptr);

    QStringList getNameList( DATATYPE ) const;
    QStringList getReinforceNameList() const;
    int dataCount( DATATYPE ) const;
    bool isExistedName(const QString &, DATATYPE) const;
    bool checkTempData(QWidget *, const QList<MATERIALVALUES> &);

    // 登録ボタンを押しての登録
    void registerTempData();
    // ファイル読込による登録
    void registerTempData(const QList<MATERIALVALUES> &);

    void clearData();
    void clearTempData();

    MATERIALVALUES getMaterialValue( DATATYPE, const QString & ) const;
    MATERIALVALUES getReinforceValue( const QString & ) const;

    QString writeData() const;

public slots:

signals:

    void materialChanged();

private:

    static const int deIntList[14];
    static const int roIntList[14];
    static const int pcIntList[5];

    QHash<QString, MATERIALVALUES> concreteTable;
    QHash<QString, MATERIALVALUES> steelTable;
    QHash<QString, MATERIALVALUES> reinforceTable;

    /* 定義順を保持　*/

    QStringList concreteNameList;
    QStringList steelNameList;
    QStringList reinforceNameList;

    /* キャッシュ */

    QList<MATERIALVALUES> cash_concreteList;
    QList<MATERIALVALUES> cash_steelList;
    QList<MATERIALVALUES> cash_reinforceList;

    QStringList getReinforceList() const;
    QStringList getReinforcePairList() const;

    void errorUndefinedName(QWidget *parent);
    void errorOverlapNames(QWidget *parent, const QStringList &names);
};
} // namespace post3dapp
#endif
