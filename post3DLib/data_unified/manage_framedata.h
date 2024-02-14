#ifndef MANAGEFRAMEDATA_H
#define MANAGEFRAMEDATA_H

#include <QObject>
#include "define_unifieddata.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class FramePointsData;
class N3DUuidTable;

class POST3D_CLASS_DLLSPEC ManageFrameData : public QObject
{
    Q_OBJECT

public:

    ManageFrameData(UnifiedDataType, QObject *parent = nullptr);
    ManageFrameData(const ManageFrameData&, UnifiedDataType, QObject *parent = nullptr);

    bool checkTempData(QWidget *, const QList<FRAMEGROUPVALUE> &,
                       const QList<FRAMEPOINTSVALUE> &);
    // 登録ボタンを押しての登録
    void registerTempData();
    // ファイル読込による登録
    void registerTempData(const QList<FRAMEGROUPVALUE> &,
                          const QList<FRAMEPOINTSVALUE> &);

    void clearData();
    void clearTempData();

    QString idToName(const QUuid &) const;
    QUuid nameToID(const QString &) const;
    QStringList idsToNames(const QList<QUuid> &) const;
    QList<QUuid> namesToIDs(const QStringList &) const;
    int dataCount() const
    {
        return frameNames.count();
    }
    bool isExistedName(const QString &str) const;
    bool isExistedID(const QUuid &str, DATATYPE) const;
    QList<QUuid> getGroupIDList() const;
    QList<QUuid> getIDList() const;
    QStringList getNameList() const;

    QList<FRAMEPOINTSVALUE> getFrameValuesOfFloor(const QUuid &fl_id) const;
    FRAMEPOINTSVALUE getFrameValueOfFloor( const QUuid &fr_id, const QUuid &fl_id ) const;
    QList<FRAMEPOINTSVALUE> getFrameValuesOfGID(const QUuid &gid) const;
    QList<QPointF> getFramePointsOfFloor( const QUuid &fr_id, const QUuid &fl_id) const;
    FrameType::FRAMETYPE getFrameTypeOfFloor(const QUuid &fr_id, const QUuid &fl_id = QUuid() ) const;
    bool withinGroupHeightRange(const QUuid &gid, qreal fl_h) const;
    QList<QUuid> getFloorListOfFrame(const QUuid &fr_id ) const;
    QList<QUuid> getFloorListOfGroup( const QUuid &gid) const;
    QList<QUuid> getFrameRange(const QUuid &, const QUuid &) const;

    QList<FRAMEGROUPVALUE> getFrameGroupList()const;
    QList<FRAMEPOINTSVALUE> getFramePointList()const;

    /*座標系の変換関数 xyz XYZ*/
    XYZ frameXYZToGlobalXYZ(const QUuid &fr_id, XYZ p0) const;
    QList<XYZ> frameXYZToGlobalXYZ(const QUuid &fr_id, const QList<XYZ> &p0_list) const;
    XYZ globalXYZToFrameXYZ(const QUuid &fr_id, XYZ xyz) const;
    QList<XYZ> globalXYZToFrameXYZ(const QUuid &fr_id, const QList<XYZ> &p0_list) const;
    qreal globalXYZToFrameAngle(const QUuid &fr_id, XYZ xyz) const;
    qreal globalZToClosedFrameEndX(const QUuid &fr_id, qreal z) const;
    QString writeData(QSharedPointer<N3DUuidTable>) const;

public slots:

signals:

    void frameDeleted(DATATYPE, const QList<QUuid> & );

private:

    /*定義順を保持したいため*/
    QStringList frameNames;

    /* UUID ⇔ 名前の相互検索  */
    QHash<QUuid, QString> frameNameTable;

    /* 通りグループのGIDと　通りグループ　のHash */
    QHash<QUuid, FRAMEGROUPVALUE> gidToGroupHash;

    /* 通りのUUIDと　全階分の通りの情報を集めたクラス　のHash */
    QHash<QUuid, FramePointsData *> uidToFrameHash;

    /* キャッシュ */
    QList<FRAMEGROUPVALUE> cash_groupList;
    QList<FRAMEPOINTSVALUE> cash_pointsList;

    void errorUndefinedGroupName(QWidget *parent);
    void errorUndefinedFrameName(QWidget *parent);
    void errorOverlapFloorRange(QWidget *, const QString &);
    void errorOverlapFrameName(QWidget *, const QString &);
    void errorNotFoundFloorRange(QWidget *, const QString &);
    void errorExistOpenClose(QWidget *, const QString &);
    void errorFramePoints(QWidget *, const QString &);

    bool comparePointList(const QList<QPointF> &, const QList<QPointF> &);
    const UnifiedDataType isOutput;
};

class POST3D_CLASS_DLLSPEC FramePointsData
{

public:

    explicit FramePointsData(
        const QString &name = QString(),
        const QUuid &uuid = QUuid() )
        : pointsName(name), pointsID(uuid) { }

    void insertFrameValue(FRAMEPOINTSVALUE value)
    {
        value.frameName = pointsName;
        value.UUID = pointsID;
        gidTable.insert(value.GID, value);
    }
    QString frameName()const
    {
        return pointsName;
    }
    QUuid frameID()const
    {
        return pointsID;
    }
    QList<FRAMEPOINTSVALUE> frameValues()const
    {
        return gidTable.values();
    }
    FRAMEPOINTSVALUE frameValueOfGID(const QUuid &gid)const
    {
        return gidTable.value(gid);
    }
    QList<QUuid> GIDList()const
    {
        return gidTable.keys();
    }
    bool containsGID(const QUuid &gid) const
    {
        return gidTable.contains(gid);
    }

private:

    QString pointsName;
    QUuid pointsID;
    QHash<QUuid, FRAMEPOINTSVALUE> gidTable;
};

} // namespace post3dapp

#endif

