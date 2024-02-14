#include "manage_floordata.h"

#include <QtGui>
#include <QMessageBox>

#include "n3d_uuidtable.h"

namespace post3dapp{
ManageFloorData::ManageFloorData(QObject *parent)
    : QObject(parent)
{
    baseLevelFloor = QUuid();
    roofTopFloor = QUuid();
}

ManageFloorData::ManageFloorData(const ManageFloorData& floor, QObject *parent)
    : QObject(parent)
{
    this->registerTempData(floor.getFloorList());
    this->registerAnalysisFloorData(floor.getFloorList(), floor.baseLevelFloor, floor.roofTopFloor);
}

bool ManageFloorData::checkTempData(QWidget *parent, const QList<FLOORVALUES> &tmpList)
{
    QStringList nameList, nameErrList, heightErrList, changedNameList, changedHeightList;
    QList<qreal> heightList;

    for (int i = 0; i < tmpList.count(); i++) {

        FLOORVALUES fvalue = tmpList.at(i);

        QString name = fvalue.floorName;
        if (name.isEmpty()) {
            errorUndefinedName(parent);
            return false;
        }

        if ( nameList.contains(name) ) nameErrList.append(name);
        nameList.append(name);

        qreal hh = fvalue.floorHeight;
        Q_FOREACH (qreal th, heightList)
            if ( qAbs(th - hh) < 0.001 ) heightErrList.append(name);
        heightList.append(hh);

        if ( !fvalue.UUID.isNull() && floorNameTable.keys().contains(fvalue.UUID) ) {
            FLOORVALUES ovalue = getFloorValues(fvalue.UUID);
            if ( ovalue.floorName != name ) changedNameList.append(name);
            if ( qAbs(ovalue.floorHeight - hh) >= 0.001 ) changedHeightList.append(name);
        }

    }

    /*新たに登録された階、削除された階のチェック*/

    if ( !nameErrList.isEmpty() ) {
        errorOverlapNames(parent, nameErrList);
        return false;
    }
    if ( !heightErrList.isEmpty() ) {
        errorOverlapHeights(parent, heightErrList);
        return false;
    }

    if ( !changedNameList.isEmpty() ) {
        if ( informationNameChanged(parent) == QMessageBox::Cancel ) return false;
    }

    /*  階高が変わっても節点属性・座標を変化させない
        if( !changedHeightList.isEmpty() ){
            if( informationRemoveJointAttribute(parent)==QMessageBox::Cancel ) return false;
            if( informationAddJointAttribute(parent)==QMessageBox::Cancel ) return false;
        }
    */

    cash_floorList = tmpList;
    return true;
}

// 登録ボタンを押しての登録
void ManageFloorData::registerTempData()
{
    QList<QUuid> new_idlist;
    QList<QUuid> deleted_idlist;
    Q_FOREACH (FLOORVALUES tv, cash_floorList) {
        if ( !tv.UUID.isNull() ) new_idlist.append(tv.UUID);
    }
    Q_FOREACH (QUuid id, floorIDList) {
        if ( !new_idlist.contains(id) ) deleted_idlist.append(id);
    }
    if ( !deleted_idlist.isEmpty() ) emit floorDeleted(DATATYPE::TPFLOOR, deleted_idlist);

    clearData();

    Q_FOREACH (FLOORVALUES tv, cash_floorList) {
        if ( tv.UUID.isNull() ) {
            QUuid uu = QUuid::createUuid();
            tv.UUID = uu;
        }
        floorIDList.append(tv.UUID);
        floorNameTable.insert(tv.UUID, tv.floorName);
        uidToFloorHash.insert(tv.UUID, tv);
    }
    clearTempData();
}

void ManageFloorData::registerAnalysisFloorData(const QList<FLOORVALUES> &fvlist,
                                                const QUuid &b_id, const QUuid &t_id )
{
    Q_FOREACH ( FLOORVALUES fv, fvlist ) {
        FLOORVALUES o_fv = uidToFloorHash.value(fv.UUID);
        o_fv.analysisName = fv.analysisName;
        o_fv.adjustedLevel = fv.adjustedLevel;
        o_fv.isChildFloor = fv.isChildFloor;
        o_fv.parentID = fv.parentID;
        uidToFloorHash.insert(fv.UUID, o_fv);
    }
    baseLevelFloor = b_id;
    roofTopFloor = t_id;
}

// ファイル読込による登録
void ManageFloorData::registerTempData(const QList<FLOORVALUES> &flist)
{
    clearData();
    clearTempData();

    Q_FOREACH (FLOORVALUES tv, flist) {
        if ( tv.UUID.isNull() ) {
            QUuid uu = QUuid::createUuid();
            tv.UUID = uu;
        }
        floorIDList.append(tv.UUID);
        floorNameTable.insert(tv.UUID, tv.floorName);
        uidToFloorHash.insert(tv.UUID, tv);
    }
}

void ManageFloorData::clearData()
{
    floorIDList.clear();
    floorNameTable.clear();
    uidToFloorHash.clear();
    baseLevelFloor = QUuid();
    roofTopFloor = QUuid();
}

void ManageFloorData::clearTempData()
{
    cash_floorList.clear();
}

QString ManageFloorData::idToName(const QUuid &id) const
{
    return ( floorIDList.contains(id) ) ?
           floorNameTable.value(id) : QString() ;
}

QUuid ManageFloorData::nameToID(const QString &str) const
{
    qDebug() << "nameToID in ManageFloorData";
    return ( floorNameTable.values().contains(str) ) ?
           floorNameTable.key(str) : QUuid() ;
}

QStringList ManageFloorData::idsToNames(const QList<QUuid> &idlist) const
{
    QStringList strlist;
    Q_FOREACH ( QUuid id, idlist ) {
        if ( floorNameTable.contains(id) ) strlist.append( floorNameTable.value(id) );
    }
    return strlist;
}

QList<QUuid> ManageFloorData::namesToIDs(const QStringList &strlist) const
{
    QList<QUuid> idlist;
    Q_FOREACH ( QString str, strlist ) {
        if ( floorNameTable.values().contains(str) ) idlist.append( floorNameTable.key(str) );
    }
    return idlist;
}

int ManageFloorData::dataCount(DATATYPE dtype) const
{
    return ( dtype == DATATYPE::TPFLOOR ) ? getIDList().count() : getAnalysisIDList().count();
}

bool ManageFloorData::isExistedID(const QUuid &id, DATATYPE dtype ) const
{
    return ( dtype == DATATYPE::TPFLOOR ) ?  floorIDList.contains(id) : getAnalysisIDList().contains(id) ;
}

bool ManageFloorData::isExistedName(const QString &str, DATATYPE dtype ) const
{
    return ( dtype == DATATYPE::TPFLOOR ) ?  floorNameTable.values().contains(str)
           : getAnalysisNameList().contains(str) ;
}

QList<QUuid> ManageFloorData::getIDList() const
{
    return floorIDList;
}

QStringList ManageFloorData::getNameList() const
{
    /* 高さ順を保持したいため、floorNameTable.values()はダメ */
    QStringList nameList;
    Q_FOREACH (QUuid id, floorIDList)
        nameList.append(floorNameTable.value(id));
    return nameList;
}



QString ManageFloorData::idToAnalysisName(const QUuid &id) const
{
    if ( floorIDList.contains(id) ) {
        FLOORVALUES fv = uidToFloorHash.value(id);
        if ( fv.floorType != FLOORTYPE::FL_CHILD ) return fv.analysisName;
    }
    return QString() ;
}

QUuid ManageFloorData::analysisNameToID(const QString &str) const
{
    QUuid uid;
    Q_FOREACH ( QUuid id, floorIDList ) {
        FLOORVALUES fv = uidToFloorHash.value(id);
        if ( fv.analysisName == str && fv.floorType != FLOORTYPE::FL_CHILD ) uid = fv.UUID;
    }
    return uid;
}

QStringList ManageFloorData::idsToAnalysisNames(const QList<QUuid> &idlist) const
{
    QStringList strlist;
    Q_FOREACH ( QUuid id, idlist ) {
        if ( !floorIDList.contains(id) ) continue;
        strlist.append( uidToFloorHash.value(id).analysisName );
    }
    return strlist;
}

QList<QUuid> ManageFloorData::analysisNamesToIDs(const QStringList &strlist) const
{
    QList<int> idxlist;
    for ( int i = 0; i < floorIDList.count(); i++ ) {
        FLOORVALUES fv = uidToFloorHash.value(floorIDList.at(i));
        idxlist.append( strlist.indexOf( fv.analysisName ) );
    }
    QList<QUuid> uidlist;
    for ( int i = 0; i < strlist.count(); i++ ) {
        int j_idx = idxlist.indexOf(i);
        ( j_idx < 0 ) ? uidlist.append( QUuid() ) : uidlist.append( floorIDList.at(j_idx) ) ;
    }
    return uidlist;
}

QList<QUuid> ManageFloorData::getAnalysisIDList() const
{
    QList<QUuid> uidlist;
    for ( int i = 0; i < floorIDList.count(); i++ ) {
        FLOORVALUES fv = uidToFloorHash.value(floorIDList.at(i));
        if ( fv.floorType != FLOORTYPE::FL_CHILD ) uidlist.append(fv.UUID);
    }
    return uidlist;
}

QStringList ManageFloorData::getAnalysisNameList() const
{
    QStringList nameList;
    Q_FOREACH (QUuid id, floorIDList) {
        FLOORVALUES fv = uidToFloorHash.value(id);
        if ( fv.floorType != FLOORTYPE::FL_CHILD ) nameList.append(fv.analysisName);
    }
    return nameList;
}

FLOORVALUES ManageFloorData::getFloorValues(const QUuid &id) const
{
    if ( !isExistedID(id) ) return FLOORVALUES();
    return uidToFloorHash.value(id);
}

QList<FLOORVALUES> ManageFloorData::getFloorList() const
{
    QList<FLOORVALUES> flList;
    Q_FOREACH ( QUuid id, floorIDList ) flList.append( uidToFloorHash.value(id) );
    return flList;
}

qreal ManageFloorData::getAdjustedFloorLevel(const QUuid &uid) const
{
    if ( !isExistedID(uid) ) return 0.0;
    return uidToFloorHash.value(uid).adjustedLevel;
}

bool ManageFloorData::isChildFloor(const QUuid &id) const
{
    if ( !isExistedID(id) ) return false;
    return uidToFloorHash.value(id).isChildFloor;
}

qreal ManageFloorData::getFloorHeight(const QUuid &id) const
{
    if ( !isExistedID(id) ) return 0.0;
    return uidToFloorHash.value(id).floorHeight;
}

qreal ManageFloorData::getMinimumFloorHeight() const
{
    if ( floorIDList.isEmpty() ) return 0.0;
    return uidToFloorHash.value( floorIDList.first() ).floorHeight;
}

qreal ManageFloorData::getMaximumFloorHeight() const
{
    if ( floorIDList.isEmpty() ) return 0.0;
    return uidToFloorHash.value( floorIDList.last() ).floorHeight;
}

/* hmin(m)～hmax(m)の範囲内にある階のIDリストを返す */
QList<QUuid> ManageFloorData::getFloorListInRange(qreal hmin, qreal hmax) const
{
    QList<QUuid> idlist;
    Q_FOREACH ( QUuid id, floorIDList ) {
        qreal hh = uidToFloorHash.value(id).floorHeight;
        if ( hmin - 1.0e-5 < hh && hh < hmax + 1.0e-5 ) idlist.append(id);
    }
    return idlist;
}

/* 高さh(m)の所属階(count=1)、または上下階(count=2)を返す */
QList<QUuid> ManageFloorData::getNearlyFloorOfHeight(qreal h) const
{
    return getNearlyFloorOfHeight(floorIDList, h);
}

/* 対象のIDListに対し、hの所属階(count=1)、または上下階(count=2)を返す　IDListは高さ順にソートされたもの */
QList<QUuid> ManageFloorData::getNearlyFloorOfHeight(const QList<QUuid> &fl_idlist, qreal h) const
{
    QList<QUuid> idlist;
    int idx = -1;
    for (int i = 0; i < fl_idlist.count(); i++) {
        qreal hh = getFloorHeight(fl_idlist.at(i));
        if ( h - 1.0e-5 <= hh && hh <= h + 1.0e-5 ) {
            idlist.append(fl_idlist.at(i));
            return idlist;
        } else if ( h < hh - 1.0e-5 ) {
            idx = i;
            break;
        }
    }
    if (idx >= 1) {
        idlist.append(fl_idlist.at(idx - 1));
        idlist.append(fl_idlist.at(idx));
    } else if (idx == 0) {
        idlist.append(fl_idlist.first());
    } else if (idx < 0) {
        idlist.append(fl_idlist.last());
    }

    return idlist;
}

/* 与えられた階のIDLIstに対して、高さの低い順にソートして返す */
QList<QUuid> ManageFloorData::sortFloorOrderOfHeight(const QList<QUuid> &fl_idlist) const
{
    QList<QUuid> sorted_idlist;

    Q_FOREACH ( QUuid fl_id, floorIDList )
        if ( fl_idlist.contains(fl_id) ) sorted_idlist.append(fl_id);

    return sorted_idlist;
}

QUuid ManageFloorData::getUpperFloorID(const QUuid &fl, int iu) const
{
    int idx = floorIDList.indexOf(fl);
    if ( idx < 0 || idx + iu < 0 || idx + iu > floorIDList.count() - 1 ) return QUuid();
    return floorIDList.at(idx + iu);
}

QList<QUuid> ManageFloorData::getFloorRange(const QUuid &id1, const QUuid &id2) const
{
    QList<QUuid> idlist;

    int idx1 = ( id1.isNull() ) ? 0 : floorIDList.indexOf(id1);
    int idx2 = ( id2.isNull() ) ? floorIDList.count() - 1 : floorIDList.indexOf(id2);
    if ( idx1 < 0 || idx2 < 0 || idx1 > idx2 ) return idlist;

    for ( int i = idx1; i <= idx2; i++ ) idlist.append( floorIDList.at(i) );
    return idlist;
}

QUuid ManageFloorData::getNearlyDownFloorID(qreal hh) const
{
    qreal min_h = getFloorHeight(floorIDList.first());
    if ( hh < min_h - 1.0e-3 ) return QUuid();
//    if ( hh < min_h - 1.0e-3 ) return floorIDList.first();

    QUuid near_fid = floorIDList.first();
    Q_FOREACH ( QUuid uid, floorIDList ) {
        if ( hh < getFloorHeight(uid) - 1.0e-3 ) break;
        near_fid = uid;
    }
    return near_fid;
}

bool ManageFloorData::withinFloors( const QUuid &f1, const QUuid &f2, qreal hh ) const
{
    qreal hmin = -1.0e+10, hmax = 1.0e+10;
    if ( floorIDList.contains(f1) ) hmin = getFloorHeight(f1) - 1.0e-3;
    if ( floorIDList.contains(f2) ) hmax = getFloorHeight(f2) + 1.0e-3;
    return (  hh > hmin && hh < hmax  );
}

bool ManageFloorData::withinFloors( const QUuid &f1, const QUuid &f2, const QUuid &f3) const
{
    qreal hmin = -1.0e+10, hmax = 1.0e+10, hh = -1.0e+8;
    if ( floorIDList.contains(f1) ) hmin = getFloorHeight(f1) - 1.0e-3;
    if ( floorIDList.contains(f2) ) hmax = getFloorHeight(f2) + 1.0e-3;
    if ( floorIDList.contains(f3) ) hh = getFloorHeight(f3);
    return (  hh > hmin && hh < hmax  );
}

QUuid ManageFloorData::getFloorOfHeight(qreal hh) const
{
    Q_FOREACH ( QUuid uid, floorIDList ) {
        if ( qAbs( getFloorHeight(uid) - hh ) < 1.0e-3 ) return uid;
    }
    return QUuid();
}

void ManageFloorData::errorUndefinedName(QWidget *parent)
{

    QString msg = u8"階名が定義されていません";
    QMessageBox::critical( parent, u8"エラー -- 階名の未定義", msg,
                           QMessageBox::Ok);
}

void ManageFloorData::errorOverlapNames(QWidget *parent, const QStringList &names)
{

    QString msg = u8"階名が重複しています\n" + names.join(",");
    QMessageBox::critical( parent, u8"エラー -- 階名の重複", msg,
                           QMessageBox::Ok);
}

void ManageFloorData::errorOverlapHeights(QWidget *parent, const QStringList &names)
{

    QString msg = u8"同一高さの階が定義されています\n" + names.join(",");
    QMessageBox::critical( parent, u8"エラー -- 同一高さの階", msg );
}

void ManageFloorData::errorBaseFloor(QWidget *parent, const QStringList &names)
{

    QString msg = u8"節点を入力済みの階は従属階にできません\n" + names.join(",");
    QMessageBox::critical( parent, u8"エラー -- 基準階の指定", msg );
}

int ManageFloorData::informationNameChanged(QWidget *parent)
{
    return QMessageBox::information(parent,
                                    u8"階名変更の確認",
                                    u8"解析条件の範囲指定で階名が使われています。\n解析条件にも変更を適用しますか？",
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                    QMessageBox::Yes );
}

int ManageFloorData::informationRemoveJointAttribute(QWidget *parent)
{
    return QMessageBox::information(parent,
                                    u8"節点の属性解除の確認",
                                    u8"変更する階が節点の属性として使われています。\n節点の属性を解除しますか？",
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                    QMessageBox::Yes );
}

int ManageFloorData::informationAddJointAttribute(QWidget *parent)
{
    return QMessageBox::information(parent,
                                    u8"節点の属性付加の確認",
                                    u8"変更後の高さに節点が存在します。\n節点の属性に変更階を追加しますか？",
                                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                    QMessageBox::Yes );
}

QString ManageFloorData::writeData(QSharedPointer<N3DUuidTable> table) const{
    QString text;
    text.append(u8"*FLOOR").append(",").append(QString::number(this->dataCount())).append("\n");
    text.append(u8"#ID, 階名, 高さ(m), 階スケールの表示, コンクリート強度(小梁), コンクリート強度(スラブ), 高さ補正±(m), 補助階, 所属階ID\n");
    Q_FOREACH(FLOORVALUES fv, getFloorList()){
        text.append(fv.writeData(table)).append("\n");
    }

    text.append("**BASELEVELFLOOR,").append(table->GetOrCreateN3dNumber(baseLevelFloor, DATATYPE::TPFLOOR)).append("\n");
    text.append("**ROOFTOPFLOOR,").append(table->GetOrCreateN3dNumber(roofTopFloor, DATATYPE::TPFLOOR)).append("\n");

    return text;
}

} // namespace post3dapp
