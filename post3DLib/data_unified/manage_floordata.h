#ifndef MANAGEFLOORDATA_H
#define MANAGEFLOORDATA_H

#include <QObject>
#include "define_unifieddata.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class N3DUuidTable;
class POST3D_CLASS_DLLSPEC ManageFloorData : public QObject
{
    Q_OBJECT

public:

    ManageFloorData( QObject *parent = nullptr);
    ManageFloorData(const ManageFloorData&, QObject *parent = nullptr);

    bool checkTempData(QWidget *, const QList<FLOORVALUES> &);

    // 登録ボタンを押しての登録
    void registerTempData();
    // ファイル読込による登録
    void registerTempData(const QList<FLOORVALUES> &);

    void registerAnalysisFloorData(const QList<FLOORVALUES> &, const QUuid &, const QUuid & );


    void clearData();
    void clearTempData();

    QString idToName(const QUuid &) const;
    QUuid nameToID(const QString &) const;
    QStringList idsToNames(const QList<QUuid> &) const;
    QList<QUuid> namesToIDs(const QStringList &) const;
    QList<QUuid> getIDList() const;
    QStringList getNameList() const;

    QString idToAnalysisName(const QUuid &) const;
    QUuid analysisNameToID(const QString &) const;
    QStringList idsToAnalysisNames(const QList<QUuid> &) const;
    QList<QUuid> analysisNamesToIDs(const QStringList &) const;
    QList<QUuid> getAnalysisIDList() const;
    QStringList getAnalysisNameList() const;

    int dataCount(DATATYPE dtype = DATATYPE::TPFLOOR) const;
    bool isExistedName(const QString &str, DATATYPE dtype = DATATYPE::TPFLOOR) const;
    bool isExistedID(const QUuid &str, DATATYPE dtype = DATATYPE::TPFLOOR) const;

    FLOORVALUES getFloorValues(const QUuid &) const;
    QList<FLOORVALUES> getFloorList() const;
    qreal getAdjustedFloorLevel(const QUuid & ) const;
    bool isChildFloor(const QUuid &) const;
    qreal getFloorHeight(const QUuid &) const;
    qreal getMinimumFloorHeight() const;
    qreal getMaximumFloorHeight() const;
    QList<QUuid> getFloorListInRange(qreal hmin, qreal hmax) const;
    QList<QUuid> getNearlyFloorOfHeight(qreal h) const;
    QList<QUuid> getNearlyFloorOfHeight(const QList<QUuid> &idlist, qreal h) const;
    QList<QUuid> sortFloorOrderOfHeight(const QList<QUuid> &idlist) const;
    QUuid getUpperFloorID(const QUuid &fl, int iu = 1) const;
    QList<QUuid> getFloorRange(const QUuid &, const QUuid &) const;
    QUuid getNearlyDownFloorID(qreal hh) const;
    bool withinFloors(const QUuid &fl, const QUuid &f2, qreal hh) const;
    bool withinFloors(const QUuid &f1, const QUuid &f2,
                      const QUuid &f3) const; // f3がf1とf2の間かどうか
    QUuid getFloorOfHeight(qreal hh) const;

    QUuid getBaseLevelFloor() const{return baseLevelFloor;}
    QUuid getRoofTopFloor() const{return roofTopFloor;}

    QString writeData(QSharedPointer<N3DUuidTable>) const;

public slots:

signals:

    void floorDeleted(DATATYPE, const QList<QUuid> & );

private:

    /*高さ順を保持　下の階から*/
    QList<QUuid> floorIDList;

    /* UUID ⇔ 名前の相互検索  */
    QHash<QUuid, QString> floorNameTable;

    /* UUIDと　階情報構造体FLOORVALUES　のHash */
    QHash<QUuid, FLOORVALUES> uidToFloorHash;

    QList<FLOORVALUES> cash_floorList;

    // 基準レベル階
    QUuid baseLevelFloor;

    // 屋上階
    QUuid roofTopFloor;


    void errorUndefinedName(QWidget *parent);
    void errorOverlapNames(QWidget *parent, const QStringList &names);
    void errorOverlapHeights(QWidget *parent, const QStringList &names);
    void errorBaseFloor(QWidget *parent, const QStringList &names);
    int informationNameChanged(QWidget *);
    int informationRemoveJointAttribute(QWidget *);
    int informationAddJointAttribute(QWidget *);


};
} // namespace post3dapp
#endif
