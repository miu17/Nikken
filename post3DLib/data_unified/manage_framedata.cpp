#include "manage_framedata.h"

#include <QDebug>
#include <QMessageBox>

#include "calc_vector2d.h"
#include "factory_unifieddata.h"
#include "unified_data.h"

namespace post3dapp{
ManageFrameData::ManageFrameData(UnifiedDataType isout, QObject *parent)
    : QObject(parent), isOutput(isout)
{
}

ManageFrameData::ManageFrameData(const ManageFrameData& frame, UnifiedDataType isout, QObject *parent)
    : QObject(parent), isOutput(isout)
{
    this->registerTempData(frame.getFrameGroupList(), frame.getFramePointList());
}

/* 名前とUUIDに関する問い合わせ */

QString ManageFrameData::idToName( const QUuid &id ) const
{
    return ( frameNameTable.contains(id) ) ?
           frameNameTable.value(id) : QString() ;
}

QUuid ManageFrameData::nameToID( const QString &str ) const
{
    qDebug() << "nameToId of ManageFrameData";
    return ( frameNameTable.values().contains(str) ) ?
           frameNameTable.key(str) : QUuid() ;
}

QStringList ManageFrameData::idsToNames(const QList<QUuid> &idlist) const
{
    QStringList strlist;
    Q_FOREACH ( QUuid id, idlist ) {
        if ( frameNameTable.contains(id) ) strlist.append( frameNameTable.value(id) );
    }
    return strlist;
}

QList<QUuid> ManageFrameData::namesToIDs(const QStringList &strlist) const
{
    QList<QUuid> idlist;
    Q_FOREACH ( QString str, strlist ) {
        if ( frameNameTable.values().contains(str) ) idlist.append( frameNameTable.key(str) );
    }
    return idlist;
}

bool ManageFrameData::isExistedName( const QString &str ) const
{
    return frameNameTable.values().contains(str) ;
}

bool ManageFrameData::isExistedID( const QUuid &id, DATATYPE dtype) const
{
    return ( dtype == DATATYPE::TPFRAMEP ) ? frameNameTable.contains(id) : gidToGroupHash.contains(id) ;
}
QList<QUuid> ManageFrameData::getGroupIDList() const
{
    return gidToGroupHash.keys() ;
}

QList<QUuid> ManageFrameData::getIDList() const
{
    return frameNameTable.keys() ;
}

QStringList ManageFrameData::getNameList() const
{
    return frameNames ;
}

/* 階または作業グループに属する全通りデータを返す */
QList<FRAMEPOINTSVALUE> ManageFrameData::getFrameValuesOfFloor(const QUuid &fl_id) const
{
    if ( !FactoryUnifiedData::getInstance(isOutput)->isExistedID(fl_id, DATATYPE::TPFLOOR) ) return QList<FRAMEPOINTSVALUE>();

    qreal fl_h = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(fl_id);

    QList<QUuid> gidlist;
    Q_FOREACH (QUuid gid, gidToGroupHash.keys()) {
        if ( withinGroupHeightRange(gid, fl_h ) ) gidlist.append(gid);
    }

    QList<FRAMEPOINTSVALUE> plist;
    Q_FOREACH ( QUuid gid, gidlist ) {
        plist.append(getFrameValuesOfGID(gid));
    }
    return plist;
}

/* 対象通り（全階分）のうち、当該階に属する通りデータを返す */
FRAMEPOINTSVALUE ManageFrameData::getFrameValueOfFloor(const QUuid &fr_id,
                                                        const QUuid &fl_id ) const
{
    if ( !uidToFrameHash.contains(fr_id) ||
            !FactoryUnifiedData::getInstance(isOutput)->isExistedID(fl_id, DATATYPE::TPFLOOR) ) return FRAMEPOINTSVALUE();

    qreal fl_h = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(fl_id);

    Q_FOREACH (QUuid gid, uidToFrameHash.value(fr_id)->GIDList() ) {
        if ( withinGroupHeightRange(gid, fl_h ) )
            return uidToFrameHash.value(fr_id)->frameValueOfGID(gid);
    }

    return FRAMEPOINTSVALUE();
}


/* 指定グループ（GID）に属する全通りデータを返す */
QList<FRAMEPOINTSVALUE> ManageFrameData::getFrameValuesOfGID(const QUuid &gid) const
{
    QList<FRAMEPOINTSVALUE> plist;
    Q_FOREACH (FramePointsData *fp, uidToFrameHash.values()) {
        if (fp->containsGID(gid)) plist.append(fp->frameValueOfGID(gid));
    }
    return plist;
}

/* 対象通り（全階分）のうち、当該階に属する通りの点列を返す 閉じた円、閉じた点列の場合、最初の点を付加  */
QList<QPointF> ManageFrameData::getFramePointsOfFloor(const QUuid &fr_id,
                                                       const QUuid &fl_id) const
{
    if ( !uidToFrameHash.contains(fr_id) ||
            !FactoryUnifiedData::getInstance(isOutput)->isExistedID(fl_id, DATATYPE::TPFLOOR) ) return QList<QPointF>();

    qreal fl_h = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(fl_id);

    Q_FOREACH (QUuid gid, uidToFrameHash.value(fr_id)->GIDList() ) {
        if ( withinGroupHeightRange(gid, fl_h ) ) {
            FRAMEPOINTSVALUE fp = uidToFrameHash.value(fr_id)->frameValueOfGID(gid);
            return ( fp.frameType == FrameType::FRAMETYPE::CLCIRCLE || fp.frameType == FrameType::FRAMETYPE::CLVARIABLE ) ?
                   fp.pointList << fp.pointList.first() : fp.pointList ;
        }
    }

    return QList<QPointF>();
}

/* 対象通り（全階分）のうち、当該階に属する通りの通りタイプを返す 階IDが未指定の場合は0.0の高さ */
FrameType::FRAMETYPE ManageFrameData::getFrameTypeOfFloor(const QUuid &fr_id, const QUuid &fl_id) const
{
    if ( !uidToFrameHash.contains(fr_id)  ) return FrameType::FRAMETYPE::NOFRAMETYPE;

    qreal fl_h = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(fl_id);
    Q_FOREACH (QUuid gid, uidToFrameHash.value(fr_id)->GIDList() ) {
        if ( withinGroupHeightRange(gid, fl_h ) )
            return uidToFrameHash.value(fr_id)->frameValueOfGID(gid).frameType;
    }
    return FrameType::FRAMETYPE::NOFRAMETYPE;
}

/* 指定の高さが通りグループ（GID）の階範囲内にあるか否か */
bool ManageFrameData::withinGroupHeightRange(const QUuid &gid, qreal fl_h) const
{
    FRAMEGROUPVALUE fg = gidToGroupHash.value(gid);

    QUuid sf = fg.startFloorID;
    qreal sf_h = ( sf.isNull() ) ? FactoryUnifiedData::getInstance(isOutput)->getMinimumFloorHeight() :
                 FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(sf);
    QUuid ef = fg.endFloorID;
    qreal ef_h = ( ef.isNull() ) ? FactoryUnifiedData::getInstance(isOutput)->getMaximumFloorHeight() :
                 FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(ef);

    return (  fl_h >= sf_h - 1.0e-5 && fl_h <= ef_h + 1.0e-5  ) ;
}

/* 対象通りが属する階のＩＤリストを返す */
QList<QUuid> ManageFrameData::getFloorListOfFrame(const QUuid &fr_id ) const
{
    if ( !uidToFrameHash.contains(fr_id) ) return QList<QUuid>();

    QList<QUuid> gidlist = uidToFrameHash.value(fr_id)->GIDList();

    QList<QUuid> fl_idlist;
    Q_FOREACH (QUuid gid, gidlist) {
        QList<QUuid> idlist = getFloorListOfGroup(gid );
        Q_FOREACH (QUuid id, idlist)
            if (!fl_idlist.contains(id)) fl_idlist.append(id);
    }

    return fl_idlist;
}

/* 対象グループが属する階のＩＤリストを返す */
QList<QUuid> ManageFrameData::getFloorListOfGroup(const QUuid &gid ) const
{
    if ( !gidToGroupHash.contains(gid) ) return QList<QUuid>();

    FRAMEGROUPVALUE fg = gidToGroupHash.value(gid);

    QUuid sf = fg.startFloorID;
    qreal sf_h = ( sf.isNull() ) ? FactoryUnifiedData::getInstance(isOutput)->getMinimumFloorHeight() :
                 FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(sf);
    QUuid ef = fg.endFloorID;
    qreal ef_h = ( ef.isNull() ) ? FactoryUnifiedData::getInstance(isOutput)->getMaximumFloorHeight() :
                 FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(ef);

    return FactoryUnifiedData::getInstance(isOutput)->getFloorListInRange(sf_h, ef_h);
}

QList<QUuid> ManageFrameData::getFrameRange(const QUuid &id1, const QUuid &id2) const
{
    if ( !frameNameTable.contains(id1) || !frameNameTable.contains(id2) ) return QList<QUuid>();
//    FramePointsData *fp=uidToFrameHash.value(id1)->GIDList()
    return QList<QUuid>();
}

QList<FRAMEGROUPVALUE> ManageFrameData::getFrameGroupList()const{
    QMap<QString, FRAMEGROUPVALUE> list;
    Q_FOREACH(QString name, frameNames){
        FramePointsData *fp = uidToFrameHash.value(nameToID(name));
        Q_FOREACH(FRAMEGROUPVALUE fg, gidToGroupHash.values()){
            if (fp->containsGID(fg.GID)){
                list.insert(fg.groupName, fg);
            }
        }
    }
    return list.values();
}
QList<FRAMEPOINTSVALUE> ManageFrameData::getFramePointList()const{
    QList<FRAMEPOINTSVALUE> list;
    Q_FOREACH(QString name, frameNames){
        FramePointsData *fp = uidToFrameHash.value(nameToID(name));
        Q_FOREACH(FRAMEGROUPVALUE fg, gidToGroupHash.values()){
            if (fp->containsGID(fg.GID)){
                list.append(fp->frameValueOfGID(fg.GID));
            }
        }
    }
    return list;
}


/* 軸組図上のXY,z座標から全体座標系のXYZを返す */
XYZ ManageFrameData::frameXYZToGlobalXYZ(const QUuid &fr_id, XYZ p0) const
{
    QList<QUuid> fl_list = getFloorListOfFrame(fr_id );
    fl_list = FactoryUnifiedData::getInstance(isOutput)->sortFloorOrderOfHeight(fl_list);

    QList<QUuid> idlist = FactoryUnifiedData::getInstance(isOutput)->getNearlyFloorOfHeight(fl_list, p0.y);

    FrameType::FRAMETYPE f_type = getFrameTypeOfFloor(fr_id,QUuid() );
    bool isOpen = ( f_type != FrameType::FRAMETYPE::CLCIRCLE && f_type != FrameType::FRAMETYPE::CLVARIABLE );

    QPointF fp;
    if ( idlist.count() == 1 ) {
        QList<QPointF> plist = getFramePointsOfFloor(fr_id, idlist.first() );
        fp = CalcVector2D::elvXZToGlobalXY(plist, QPointF(p0.x, p0.z), isOpen);
    } else if ( idlist.count() > 1 ) {
        QList<QPointF> plist1 = getFramePointsOfFloor(fr_id, idlist.first() );
        qreal h1 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.first());
        QList<QPointF> plist2 = getFramePointsOfFloor(fr_id, idlist.last() );
        qreal h2 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.last());
        qreal rr = (p0.y - h1) / (h2 - h1);
        QList<QPointF> c_plist = CalcVector2D::combinePointList(plist1, plist2, rr);
        fp = CalcVector2D::elvXZToGlobalXY(c_plist, QPointF(p0.x, p0.z), isOpen);
    }

    return XYZ(fp.x(), fp.y(), p0.y);
}

/* 軸組図座標系のXY,z点列から全体座標系の点列を返す */
QList<XYZ> ManageFrameData::frameXYZToGlobalXYZ(const QUuid &fr_id, const QList<XYZ> &p0_list) const
{
    QList<QUuid> fl_list = getFloorListOfFrame(fr_id );
    fl_list = FactoryUnifiedData::getInstance(isOutput)->sortFloorOrderOfHeight(fl_list);

    FrameType::FRAMETYPE f_type = getFrameTypeOfFloor(fr_id,QUuid() );
    bool isOpen = ( f_type != FrameType::FRAMETYPE::CLCIRCLE && f_type != FrameType::FRAMETYPE::CLVARIABLE );

    QList<XYZ> xyz_list;

    Q_FOREACH ( XYZ p0,  p0_list) {

        QList<QUuid> idlist = FactoryUnifiedData::getInstance(isOutput)->getNearlyFloorOfHeight(fl_list, p0.y);

        QPointF fp;
        if ( idlist.count() == 1 ) {
            QList<QPointF> plist = getFramePointsOfFloor(fr_id, idlist.first() );
            fp = CalcVector2D::elvXZToGlobalXY(plist, QPointF(p0.x, p0.z), isOpen);
        } else if ( idlist.count() > 1 ) {
            QList<QPointF> plist1 = getFramePointsOfFloor(fr_id, idlist.first() );
            qreal h1 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.first());
            QList<QPointF> plist2 = getFramePointsOfFloor(fr_id, idlist.last() );
            qreal h2 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.last());
            qreal rr = (p0.y - h1) / (h2 - h1);
            QList<QPointF> c_plist = CalcVector2D::combinePointList(plist1, plist2, rr);
            fp = CalcVector2D::elvXZToGlobalXY(c_plist, QPointF(p0.x, p0.z), isOpen);
        }

        xyz_list.append(XYZ(fp.x(), fp.y(), p0.y));
    }

    return xyz_list;
}

/* 全体座標系のXYZ座標から軸組図上のXY,z座標を返す */
XYZ ManageFrameData::globalXYZToFrameXYZ(const QUuid &fr_id, XYZ xyz) const
{
    QList<QUuid> fl_list = getFloorListOfFrame(fr_id );
    if ( fl_list.isEmpty() ) return XYZ();
    fl_list = FactoryUnifiedData::getInstance(isOutput)->sortFloorOrderOfHeight(fl_list);

    FrameType::FRAMETYPE f_type = getFrameTypeOfFloor(fr_id,QUuid() );
    bool isOpen = ( f_type != FrameType::FRAMETYPE::CLCIRCLE && f_type != FrameType::FRAMETYPE::CLVARIABLE );

    QList<QUuid> idlist = FactoryUnifiedData::getInstance(isOutput)->getNearlyFloorOfHeight(fl_list, xyz.z);
    QPointF xz;

    if ( idlist.count() == 1 ) {
        QList<QPointF> plist = getFramePointsOfFloor(fr_id, idlist.first() );
        xz = CalcVector2D::globalXYToElvXZ(plist, QPointF(xyz.x, xyz.y), isOpen);
    } else if ( idlist.count() > 1 ) {
        QList<QPointF> plist1 = getFramePointsOfFloor(fr_id, idlist.first() );
        qreal h1 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.first());
        QList<QPointF> plist2 = getFramePointsOfFloor(fr_id, idlist.last() );
        qreal h2 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.last());
        qreal rr = (xyz.z - h1) / (h2 - h1);
        QList<QPointF> c_plist = CalcVector2D::combinePointList(plist1, plist2, rr);
        xz = CalcVector2D::globalXYToElvXZ(c_plist, QPointF(xyz.x, xyz.y), isOpen);
    }

    return XYZ(xz.x(), xyz.z, xz.y());
}

/* 全体座標系の点列から軸組図座標系のXY,z点列を返す */
QList<XYZ> ManageFrameData::globalXYZToFrameXYZ(const QUuid &fr_id, const QList<XYZ> &p0_list) const
{
//    qDebug()<<"init global XYZ to Frame XYZ";
    QList<QUuid> fl_list = getFloorListOfFrame(fr_id );
    if ( fl_list.isEmpty() ) return QList<XYZ>();
    fl_list = FactoryUnifiedData::getInstance(isOutput)->sortFloorOrderOfHeight(fl_list);

    FrameType::FRAMETYPE f_type = getFrameTypeOfFloor(fr_id,QUuid() );
//    qDebug()<<"ftype="<<f_type;
    bool isOpen = ( f_type != FrameType::FRAMETYPE::CLCIRCLE && f_type != FrameType::FRAMETYPE::CLVARIABLE );

    QList<XYZ> fp_list;
    QPointF xz;

    Q_FOREACH ( XYZ xyz, p0_list ) {

        QList<QUuid> idlist = FactoryUnifiedData::getInstance(isOutput)->getNearlyFloorOfHeight(fl_list, xyz.z);

        if ( idlist.count() == 1 ) {
            QList<QPointF> plist = getFramePointsOfFloor(fr_id, idlist.first() );
            xz = CalcVector2D::globalXYToElvXZ(plist, QPointF(xyz.x, xyz.y), isOpen);
        } else if ( idlist.count() > 1 ) {
            QList<QPointF> plist1 = getFramePointsOfFloor(fr_id, idlist.first() );
            qreal h1 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.first());
            QList<QPointF> plist2 = getFramePointsOfFloor(fr_id, idlist.last() );
            qreal h2 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.last());
//            qDebug()<<"h1,h2="<<h1<<h2;
            qreal rr = (xyz.z - h1) / (h2 - h1);
            QList<QPointF> c_plist = CalcVector2D::combinePointList(plist1, plist2, rr);
            xz = CalcVector2D::globalXYToElvXZ(c_plist, QPointF(xyz.x, xyz.y), isOpen);
//            qDebug()<<"xz="<<xz;
        }

        fp_list.append(XYZ(xz.x(), xyz.z, xz.y()));

    }

    return fp_list;
}

/* 全体座標系のある点（XYZ）の通りに対する角度θ（反時計、rad）を返す */
qreal ManageFrameData::globalXYZToFrameAngle(const QUuid &fr_id, XYZ xyz) const
{
    QList<QUuid> fl_list = getFloorListOfFrame(fr_id );
    if ( fl_list.isEmpty() ) return 0.0;
    fl_list = FactoryUnifiedData::getInstance(isOutput)->sortFloorOrderOfHeight(fl_list);

    FrameType::FRAMETYPE f_type = getFrameTypeOfFloor(fr_id,QUuid() );
    bool isOpen = ( f_type != FrameType::FRAMETYPE::CLCIRCLE && f_type != FrameType::FRAMETYPE::CLVARIABLE );

    QList<QUuid> idlist = FactoryUnifiedData::getInstance(isOutput)->getNearlyFloorOfHeight(fl_list, xyz.z);
    qreal angle = 0.0;

    if ( idlist.count() == 1 ) {
        QList<QPointF> plist = getFramePointsOfFloor(fr_id, idlist.first() );
        angle = CalcVector2D::globalXYToElvAngle(plist, QPointF(xyz.x, xyz.y), isOpen);
    } else if ( idlist.count() > 1 ) {
        QList<QPointF> plist1 = getFramePointsOfFloor(fr_id, idlist.first() );
        qreal h1 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.first());
        QList<QPointF> plist2 = getFramePointsOfFloor(fr_id, idlist.last() );
        qreal h2 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.last());
        qreal rr = (xyz.z - h1) / (h2 - h1);
        QList<QPointF> c_plist = CalcVector2D::combinePointList(plist1, plist2, rr);
        angle = CalcVector2D::globalXYToElvAngle(c_plist, QPointF(xyz.x, xyz.y), isOpen);
    }

    return angle;
}

/* 全体座標系Zにおける通りの終点x座標を返す */
qreal ManageFrameData::globalZToClosedFrameEndX(const QUuid &fr_id, qreal z) const
{
    QList<QUuid> fl_list = getFloorListOfFrame(fr_id );
    if ( fl_list.isEmpty() ) return 0.0;
    fl_list = FactoryUnifiedData::getInstance(isOutput)->sortFloorOrderOfHeight(fl_list);

    QList<QUuid> idlist = FactoryUnifiedData::getInstance(isOutput)->getNearlyFloorOfHeight(fl_list, z);
    qreal sum_len = 0.0;

    if ( idlist.count() == 1 ) {
        QList<QPointF> plist = getFramePointsOfFloor(fr_id, idlist.first() );
        for ( int i = 0; i < plist.count() - 1; i++ ) {
            sum_len += CalcVector2D::length( plist.at(i + 1) - plist.at(i) );
        }
    } else if ( idlist.count() > 1 ) {
        QList<QPointF> plist1 = getFramePointsOfFloor(fr_id, idlist.first() );
        qreal h1 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.first());
        QList<QPointF> plist2 = getFramePointsOfFloor(fr_id, idlist.last() );
        qreal h2 = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(idlist.last());
        qreal rr = (z - h1) / (h2 - h1);
        QList<QPointF> c_plist = CalcVector2D::combinePointList(plist1, plist2, rr);
        for ( int i = 0; i < c_plist.count() - 1; i++ ) {
            sum_len += CalcVector2D::length( c_plist.at(i + 1) - c_plist.at(i) );
        }
    }

    return sum_len;
}

bool ManageFrameData::checkTempData(QWidget *parent, const QList<FRAMEGROUPVALUE> &glist,
                                    const QList<FRAMEPOINTSVALUE> &flist)
{
    /* 通りグループのチェック */

    QHash<QString, QList<QUuid> > gnameToFloorIDs;
    QHash<QUuid, QString> gidToGname;

    for ( int i = 0; i < glist.count(); i++ ) {

        FRAMEGROUPVALUE gvalue = glist.at(i);

        QString gname = gvalue.groupName;
        if ( gname.isEmpty() ) {
            errorUndefinedGroupName(parent);
            return false;
        }

        QList<QUuid> fids = gnameToFloorIDs.value(gname);
        QList<QUuid> floors = FactoryUnifiedData::getInstance(isOutput)->getFloorRange(gvalue.startFloorID,
                                                                        gvalue.endFloorID);

        if ( floors.isEmpty() ) {
            // 通りグループの階範囲が正しくない
            errorNotFoundFloorRange(parent, gname);
            return false;
        }

        Q_FOREACH ( QUuid id, floors ) {
            if ( fids.contains(id) ) {
                // 通りグループの階範囲が重複
                errorOverlapFloorRange(parent, gname);
                return false;
            }
            fids.append(id);
        }

        gnameToFloorIDs.insert(gname, fids);
        gidToGname.insert(gvalue.GID, gname);
    }

    /* 通りのチェック */

    QHash<QString, QString> fnameToGname;
    QHash<QString, QList<QUuid> > fnameToGIDs;
    QHash<QString, bool> fnameToFtype;

    for (int i = 0; i < flist.count(); i++) {

        FRAMEPOINTSVALUE fvalue = flist.at(i);

        QString fname = fvalue.frameName;
        if ( fname.isEmpty() ) {
            errorUndefinedFrameName(parent);
            return false;
        }

        if ( fnameToGname.contains(fname) ) {
            if ( gidToGname.value(fvalue.GID) != fnameToGname.value(fname) ) {
                // 別の通りグループで通り名が使われてる
                errorOverlapFrameName(parent, fname);
                return false;
            }
        } else {
            fnameToGname.insert(fname, gidToGname.value(fvalue.GID));
        }

        QList<QUuid> gidlist = fnameToGIDs.value(fname);
        if ( gidlist.contains(fvalue.GID) ) {
            // 同一グループGIDで通り名が複数使われてる
            errorOverlapFrameName(parent, fname);
            return false;
        }
        gidlist.append(fvalue.GID);
        fnameToGIDs.insert(fname, gidlist);

        bool isErr = false;
        bool isOpen = ( fvalue.frameType == FrameType::FRAMETYPE::CLCIRCLE || fvalue.frameType == FrameType::FRAMETYPE::CLVARIABLE );
        if ( fnameToFtype.contains(fname) ) {
            if ( fnameToFtype.value(fname) != isOpen ) isErr = true;
        } else {
            fnameToFtype.insert(fname, isOpen);
        }
        if ( isErr ) {
            // 同一通り名に対し、閉じた通りと開いた通りが混在している
            errorExistOpenClose(parent, fname);
            return false;
        }

        QList<QPointF> ptlist = fvalue.pointList;
        if ( ptlist.count() < 2 ) isErr = true;
        for ( int j = 1; j < ptlist.count() - 1; j++ ) {
            if ( isErr ) break;
            QPointF pp = ptlist.at(j);
            for ( int k = 0; k < j; k++ ) {
                if ( (pp - ptlist.at(k)).manhattanLength() < 1.0e-3 ) {
                    isErr = true;
                    break;
                }
            }
        }
        if ( isErr ) {
            // 通りの定義点が2点以下、または重複している
            errorFramePoints(parent, fname);
            return false;
        }

    }

    cash_groupList = glist;
    cash_pointsList = flist;

    return true;
}

// 登録ボタンを押しての登録
void ManageFrameData::registerTempData()
{
    QList<QUuid> new_idlist;
    QList<QUuid> deleted_idlist;
    Q_FOREACH ( FRAMEPOINTSVALUE fr, cash_pointsList ) {
        if ( !fr.UUID.isNull() ) new_idlist.append(fr.UUID);
    }
    Q_FOREACH ( QUuid id, frameNameTable.keys() ) {
        if ( !new_idlist.contains(id) ) deleted_idlist.append(id);
    }
    if ( !deleted_idlist.isEmpty() ) emit frameDeleted(DATATYPE::TPFRAMEP, deleted_idlist );

    /* 要　簡略化 */
    clearData();

    /* 通りグループデータ */
    Q_FOREACH ( FRAMEGROUPVALUE gr, cash_groupList ) {
        gidToGroupHash.insert(gr.GID, gr);
    }

    /* 通りデータ */

    /* 1. */
    Q_FOREACH ( FRAMEPOINTSVALUE fr, cash_pointsList ) {

        if ( !frameNames.contains( fr.frameName ) )
            frameNames.append( fr.frameName );

        if ( !fr.UUID.isNull() ) {
            if ( frameNameTable.values().contains(fr.frameName) ) {
                QUuid id = frameNameTable.key(fr.frameName);
                FramePointsData *c_fr = uidToFrameHash.value(id);
                c_fr->insertFrameValue(fr);
            } else {
                frameNameTable.insert( fr.UUID, fr.frameName );  /* IDとNameが対応している前提 */
                FramePointsData *c_fr = new FramePointsData(fr.frameName, fr.UUID);
                c_fr->insertFrameValue(fr);
                uidToFrameHash.insert( fr.UUID, c_fr );
            }
        }
    }

    /* 2. */
    Q_FOREACH ( FRAMEPOINTSVALUE fr, cash_pointsList ) {

        if ( fr.UUID.isNull() ) {
            if ( frameNameTable.values().contains(fr.frameName) ) {
                QUuid id = frameNameTable.key(fr.frameName);
                FramePointsData *c_fr = uidToFrameHash.value(id);
                fr.UUID = id;
                c_fr->insertFrameValue(fr);
            } else {
                QUuid id = QUuid::createUuid();
                fr.UUID = id;
                frameNameTable.insert( id, fr.frameName );
                FramePointsData *c_fr = new FramePointsData(fr.frameName, fr.UUID);
                c_fr->insertFrameValue(fr);
                uidToFrameHash.insert( id, c_fr );
            }
        }
    }

    clearTempData();
}

// ファイル読込による登録
void ManageFrameData::registerTempData(const QList<FRAMEGROUPVALUE> &fglist,
                                       const QList<FRAMEPOINTSVALUE> &fplist)
{
    clearData();
    clearTempData();

    Q_FOREACH ( FRAMEGROUPVALUE gr, fglist ) {
        gidToGroupHash.insert(gr.GID, gr);
    }

    Q_FOREACH ( FRAMEPOINTSVALUE fr, fplist ) {

        if ( !frameNames.contains( fr.frameName ) ) frameNames.append( fr.frameName );

        if ( frameNameTable.values().contains(fr.frameName) ) {
            QUuid id = frameNameTable.key(fr.frameName);
            FramePointsData *c_fr = uidToFrameHash.value(id);
            c_fr->insertFrameValue(fr);
        } else {
            frameNameTable.insert( fr.UUID, fr.frameName );  /* IDとNameが対応している前提 */
            FramePointsData *c_fr = new FramePointsData(fr.frameName, fr.UUID);
            c_fr->insertFrameValue(fr);
            uidToFrameHash.insert( fr.UUID, c_fr );
        }

    }

}

void ManageFrameData::clearData()
{
    frameNames.clear();
    frameNameTable.clear();

    Q_FOREACH (FramePointsData *c_fr, uidToFrameHash.values()) delete c_fr;
    gidToGroupHash.clear();
    uidToFrameHash.clear();
}

void ManageFrameData::clearTempData()
{
    cash_groupList.clear();
    cash_pointsList.clear();
}

bool ManageFrameData::comparePointList(const QList<QPointF> &list1, const QList<QPointF> &list2)
{

    if ( list1.count() != list2.count() ) return false;

    for (int i = 0; i < list1.count(); i++) {
        QPointF p1 = list1.at(i);
        QPointF p2 = list2.at(i);
        if ( (p1 - p2).manhattanLength() > 1.0e-5 ) return false;
    }

    return true;
}


void ManageFrameData::errorUndefinedGroupName(QWidget *parent)
{
    QMessageBox::critical(parent, u8"エラー -- 未定義の通りグループ", u8"通りグループが未定義です。", QMessageBox::Ok);
}

void ManageFrameData::errorUndefinedFrameName(QWidget *parent)
{
    QMessageBox::critical(parent, u8"エラー -- 未定義の通り", u8"通り名が未定義です。", QMessageBox::Ok);
}

void ManageFrameData::errorOverlapFloorRange(QWidget *parent, const QString &str)
{
    QMessageBox::critical(parent, u8"エラー -- 階範囲の重複", u8"通りグループの階範囲が重複しています。\n" + str, QMessageBox::Ok);
}

void ManageFrameData::errorOverlapFrameName(QWidget *parent, const QString &str)
{
    QMessageBox::critical(parent, u8"エラー -- 通り名の重複", u8"通り名が別の通りグループで使用されています。\n" + str, QMessageBox::Ok);
}

void ManageFrameData::errorNotFoundFloorRange(QWidget *parent, const QString &str)
{
    QMessageBox::critical( parent, u8"エラー -- 階範囲定義", u8"通りグループの階範囲が不正です。\n" + str, QMessageBox::Ok);
}

void ManageFrameData::errorExistOpenClose(QWidget *parent, const QString &str)
{
    QMessageBox::critical( parent, u8"エラー -- 通り名が混在", u8"開通りと閉通りが混在しています。\n" + str, QMessageBox::Ok);
}

void ManageFrameData::errorFramePoints(QWidget *parent, const QString &str)
{
    QMessageBox::critical( parent, u8"エラー -- 通り定義点", u8"通り定義点が不正です。\n" + str, QMessageBox::Ok);
}

QString ManageFrameData::writeData(QSharedPointer<N3DUuidTable> table) const{
    QString text;
    QList<FRAMEGROUPVALUE> straight;
    QList<FRAMEGROUPVALUE> circle;
    QList<FRAMEGROUPVALUE> radius;
    QList<FRAMEGROUPVALUE> diameter;
    QList<FRAMEGROUPVALUE> variable;
    QList<FRAMEGROUPVALUE> clcircle;
    QList<FRAMEGROUPVALUE> clvariable;

    Q_FOREACH(FRAMEGROUPVALUE fg, gidToGroupHash.values()){
        if (fg.frameType == FrameType::FRAMETYPE::STRAIGHT){
            straight.append(fg);
        }else if (fg.frameType == FrameType::FRAMETYPE::CIRCLE){
            circle.append(fg);
        }else if (fg.frameType == FrameType::FRAMETYPE::RADIUS){
            radius.append(fg);
        }else if (fg.frameType == FrameType::FRAMETYPE::DIAMETER){
            diameter.append(fg);
        }else if (fg.frameType == FrameType::FRAMETYPE::VARIABLE){
            variable.append(fg);
        }else if (fg.frameType == FrameType::FRAMETYPE::CLCIRCLE){
            clcircle.append(fg);
        }else if (fg.frameType == FrameType::FRAMETYPE::CLVARIABLE){
            clvariable.append(fg);
        }
    }

    text.append("*FRAME").append("\n");
    text.append("**STRAIGHT").append(",").append(QString::number(straight.count())).append("\n");
    text.append(u8"#GID, 通りグループ名, 所属通り数, Ox(m), Oy(m), 基準角度, 通り長さ正方向(m), 通り長さ負方向(m), 階範囲(階～), 階範囲(～階)\n");
    text.append(u8"##ID, 通り名, スパン(m)\n");
    Q_FOREACH(FRAMEGROUPVALUE fg, straight){
        text.append(fg.writeData(table)).append("\n");
        QList<FRAMEPOINTSVALUE> values = getFrameValuesOfGID(fg.GID);
        Q_FOREACH(QString name, frameNames){
            Q_FOREACH(FRAMEPOINTSVALUE fp, values){
                if (fp.frameName == name){
                    FRAMEPOINTSVALUE fp = uidToFrameHash.value(frameNameTable.key(name))->frameValueOfGID(fg.GID);
                    text.append(fp.writeData(table)).append("\n");
                }
            }
        }
    }
    text.append("**CIRCLE").append(",").append(QString::number(circle.count())).append("\n");
    text.append(u8"#GID, 通りグループ名, 所属通り数, X(m), Y(m), 開始角度, 終了角度, 分割数, 階範囲(階～), 階範囲(～階)\n");
    text.append(u8"##ID, 通り名, スパン(m)\n");
    Q_FOREACH(FRAMEGROUPVALUE fg, circle){
        text.append(fg.writeData(table)).append("\n");
        QList<FRAMEPOINTSVALUE> values = getFrameValuesOfGID(fg.GID);
        Q_FOREACH(QString name, frameNames){
            Q_FOREACH(FRAMEPOINTSVALUE fp, values){
                if (fp.frameName == name){
                    FRAMEPOINTSVALUE fp = uidToFrameHash.value(frameNameTable.key(name))->frameValueOfGID(fg.GID);
                    text.append(fp.writeData(table)).append("\n");
                }
            }
        }
    }
    text.append("**RADIUS").append(",").append(QString::number(radius.count())).append("\n");
    text.append(u8"#GID, 通りグループ名, 所属通り数, X(m), Y(m), 開始角度, 通り長さ正方向(m), 階範囲(階～), 階範囲(～階)\n");
    text.append(u8"##ID, 通り名, 加算角度(度)\n");
    Q_FOREACH(FRAMEGROUPVALUE fg, radius){
        text.append(fg.writeData(table)).append("\n");
        QList<FRAMEPOINTSVALUE> values = getFrameValuesOfGID(fg.GID);
        Q_FOREACH(QString name, frameNames){
            Q_FOREACH(FRAMEPOINTSVALUE fp, values){
                if (fp.frameName == name){
                    FRAMEPOINTSVALUE fp = uidToFrameHash.value(frameNameTable.key(name))->frameValueOfGID(fg.GID);
                    text.append(fp.writeData(table)).append("\n");
                }
            }
        }
    }
    text.append("**DIAMETER").append(",").append(QString::number(diameter.count())).append("\n");
    text.append(u8"#GID, 通りグループ名, 所属通り数, X(m), Y(m), 開始角度, 通り長さ正方向(m), 通り長さ負方向(m), 階範囲(階～), 階範囲(～階)\n");
    text.append(u8"##ID, 通り名, 加算角度(度)\n");
    Q_FOREACH(FRAMEGROUPVALUE fg, diameter){
        text.append(fg.writeData(table)).append("\n");
        QList<FRAMEPOINTSVALUE> values = getFrameValuesOfGID(fg.GID);
        Q_FOREACH(QString name, frameNames){
            Q_FOREACH(FRAMEPOINTSVALUE fp, values){
                if (fp.frameName == name){
                    FRAMEPOINTSVALUE fp = uidToFrameHash.value(frameNameTable.key(name))->frameValueOfGID(fg.GID);
                    text.append(fp.writeData(table)).append("\n");
                }
            }
        }
    }
    text.append("**VARIABLE").append(",").append(QString::number(variable.count())).append("\n");
    text.append(u8"#GID, 通りグループ名, 所属通り数, 階範囲(階～), 階範囲(～階)\n");
    text.append(u8"##ID, 通り名, (点列数), (X(m), Y(m))繰り返し\n");
    Q_FOREACH(FRAMEGROUPVALUE fg, variable){
        text.append(fg.writeData(table)).append("\n");
        QList<FRAMEPOINTSVALUE> values = getFrameValuesOfGID(fg.GID);
        Q_FOREACH(QString name, frameNames){
            Q_FOREACH(FRAMEPOINTSVALUE fp, values){
                if (fp.frameName == name){
                    FRAMEPOINTSVALUE fp = uidToFrameHash.value(frameNameTable.key(name))->frameValueOfGID(fg.GID);
                    text.append(fp.writeData(table)).append("\n");
                }
            }
        }
    }
    text.append("**CLCIRCLE").append(",").append(QString::number(clcircle.count())).append("\n");
    text.append(u8"#GID, 通りグループ名, 所属通り数, X(m), Y(m), 分割数, 階範囲(階～), 階範囲(～階)\n");
    text.append(u8"##ID, 通り名, スパン(m)\n");
    Q_FOREACH(FRAMEGROUPVALUE fg, clcircle){
        text.append(fg.writeData(table)).append("\n");
        QList<FRAMEPOINTSVALUE> values = getFrameValuesOfGID(fg.GID);
        Q_FOREACH(QString name, frameNames){
            Q_FOREACH(FRAMEPOINTSVALUE fp, values){
                if (fp.frameName == name){
                    FRAMEPOINTSVALUE fp = uidToFrameHash.value(frameNameTable.key(name))->frameValueOfGID(fg.GID);
                    text.append(fp.writeData(table)).append("\n");
                }
            }
        }
    }
    text.append("**CLVARIABLE").append(",").append(QString::number(clvariable.count())).append("\n");
    text.append(u8"#GID, 通りグループ名, 所属通り数, 階範囲(階～), 階範囲(～階)\n");
    text.append(u8"##ID, 通り名, (点列数), (X(m), Y(m))繰り返し\n");
    Q_FOREACH(FRAMEGROUPVALUE fg, clvariable){
        text.append(fg.writeData(table)).append("\n");
        QList<FRAMEPOINTSVALUE> values = getFrameValuesOfGID(fg.GID);
        Q_FOREACH(QString name, frameNames){
            Q_FOREACH(FRAMEPOINTSVALUE fp, values){
                if (fp.frameName == name){
                    FRAMEPOINTSVALUE fp = uidToFrameHash.value(frameNameTable.key(name))->frameValueOfGID(fg.GID);
                    text.append(fp.writeData(table)).append("\n");
                }
            }
        }
    }
    return text;
}

} // namespace post3dapp
