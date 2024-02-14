#ifndef MANAGELOADDATA_H
#define MANAGELOADDATA_H

#include <QObject>
#include "define_unifieddata.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
//class ManageLoadData;
class POST3D_CLASS_DLLSPEC ManageLoadData : public QObject
{
    Q_OBJECT

public:

    ManageLoadData( QObject *parent = nullptr);
    ManageLoadData(const ManageLoadData&, QObject *parent = nullptr);
    int dataCount(DATATYPE ) const;
    QStringList getNameList(DATATYPE ) const;
    bool isExistedName(const QString &, DATATYPE) const;

    bool checkJointLoadData(QWidget *, const QList<JOINTLOADVALUES> &);
    bool checkMemberLoadData(QWidget *, const QList<MEMBERLOADVALUES> &);
    bool checkSlabLoadData(QWidget *, const QList<SLABLOADVALUES> &);

    void registerJointLoadData();
    void registerMemberLoadData();
    void registerSlabLoadData();

    void registerJointLoadData(const QList<JOINTLOADVALUES> &);
    void registerMemberLoadData(const QList<MEMBERLOADVALUES> &);
    void registerSlabLoadData(const QList<SLABLOADVALUES> &);

    void replaceLoadName(DATATYPE, const QString &, const QString &);

    void clearData();
    void clearTempData();

    /* 荷重情報 */
    JOINTLOADVALUES getJointLoadValue( const QString & ) const;
    MEMBERLOADVALUES getMemberLoadValue( const QString & ) const;
    SLABLOADVALUES getSlabLoadValue( const QString & ) const;

    QString loadNameToLoadProperty(DATATYPE, const QString &) const;

    QString writePsvData(QSharedPointer<N3DUuidTable>) const;
    QList<JOINTLOADVALUES> getJointLoadList() const;
    QList<MEMBERLOADVALUES> getMemberLoadList() const;
    QList<SLABLOADVALUES> getSlabLoadList() const;

public slots:

signals:

    void loadDeleted(DATATYPE, const QStringList &);
    void loadChanged(DATATYPE, const QStringList &);

private:

    QHash<QString, JOINTLOADVALUES> jointLoadTable;
    QMultiHash<QString, MEMBERLOADVALUES> memberLoadTable;
    QHash<QString, SLABLOADVALUES> slabLoadTable;

    /* 定義順を保持　*/

    QStringList jointLoadNameList;
    QStringList memberLoadNameList;
    QStringList slabLoadNameList;

    /* キャッシュ */

    QList<JOINTLOADVALUES> cash_jLoadList;
    QList<MEMBERLOADVALUES> cash_mLoadList;
    QList<SLABLOADVALUES> cash_sLoadList;


    QString loadDirToString( LCORSYSTEM, LOADDIR ) const;


};
} // namespace post3dapp
#endif
