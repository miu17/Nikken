#include "manage_loaddata.h"

#include <QDebug>
namespace post3dapp{
ManageLoadData::ManageLoadData(QObject *parent)
    : QObject(parent)
{

}

ManageLoadData::ManageLoadData(const ManageLoadData& load, QObject *parent)
    : QObject(parent)
{
    jointLoadTable = load.jointLoadTable;
    jointLoadTable.detach();
    memberLoadTable = load.memberLoadTable;
    memberLoadTable.detach();
    slabLoadTable = load.slabLoadTable;
    slabLoadTable.detach();

    /* 定義順を保持　*/
    jointLoadNameList = load.jointLoadNameList;
    jointLoadNameList.detach();
    memberLoadNameList = load.memberLoadNameList;
    memberLoadNameList.detach();
    slabLoadNameList = load.slabLoadNameList;
    slabLoadNameList.detach();

    /* キャッシュ */
    cash_jLoadList = load.cash_jLoadList;
    cash_jLoadList.detach();
    cash_mLoadList = load.cash_mLoadList;
    cash_mLoadList.detach();
    cash_sLoadList = load.cash_sLoadList;
    cash_sLoadList.detach();
}
int ManageLoadData::dataCount(DATATYPE type) const
{
    if ( type == DATATYPE::TPJOINTLOAD ) {
        return jointLoadNameList.count();
    } else if ( type == DATATYPE::TPMEMBERLOAD ) {
        return memberLoadNameList.count();
    } else if ( type == DATATYPE::TPSLABLOAD ) {
        return slabLoadNameList.count();
    }
    return 0;
}

QStringList ManageLoadData::getNameList(DATATYPE type) const
{
    if ( type == DATATYPE::TPJOINTLOAD ) {
        return jointLoadNameList;
    } else if ( type == DATATYPE::TPMEMBERLOAD ) {
        return memberLoadNameList;
    } else if ( type == DATATYPE::TPSLABLOAD ) {
        return slabLoadNameList;
    }
    return QStringList();
}

bool ManageLoadData::isExistedName(const QString &str, DATATYPE type) const
{
    if ( type == DATATYPE::TPJOINTLOAD ) {
        return jointLoadNameList.contains(str);
    } else if ( type == DATATYPE::TPMEMBERLOAD ) {
        return memberLoadNameList.contains(str);
    } else if ( type == DATATYPE::TPSLABLOAD ) {
        return slabLoadNameList.contains(str);
    }
    return false;
}

/* 節点荷重　チェック */
bool ManageLoadData::checkJointLoadData(QWidget *parent, const QList<JOINTLOADVALUES> &loadlist)
{
    Q_UNUSED(parent);
    cash_jLoadList = loadlist;
    return true;
}

/* 部材荷重　チェック */
bool ManageLoadData::checkMemberLoadData(QWidget *parent, const QList<MEMBERLOADVALUES> &loadlist)
{
    Q_UNUSED(parent);
    cash_mLoadList = loadlist;
    return true;
}

/* 床荷重　チェック */
bool ManageLoadData::checkSlabLoadData(QWidget *parent, const QList<SLABLOADVALUES> &loadlist)
{
    Q_UNUSED(parent);
    cash_sLoadList = loadlist;
    return true;
}

void ManageLoadData::registerJointLoadData()
{
    QStringList cashNameList;
    QStringList changedList, deletedList;

    // 既存データとキャッシュデータの差分把握

    for (int i = 0; i < cash_jLoadList.count(); i++) {
        JOINTLOADVALUES jv = cash_jLoadList.at(i);
        cashNameList.append(jv.loadName);
        if ( !jointLoadNameList.contains(jv.loadName) ) {
            changedList.append( jv.loadName );
        } else {
            QHash<LoadType::LOADTYPE, JOINTLOADDIM> n_jt = jv.loadTable;
            QHash<LoadType::LOADTYPE, JOINTLOADDIM> o_jt = jointLoadTable.value(jv.loadName).loadTable;
            if ( n_jt.count() != o_jt.count() ) {
                changedList.append( jv.loadName );
                continue;
            }
            bool isChanged = false;
            Q_FOREACH ( LoadType::LOADTYPE ltype, n_jt.keys() ) {
                if ( !o_jt.contains(ltype) ) {
                    isChanged = true;
                    break;
                }
                JOINTLOADDIM n_dim = n_jt.value(ltype);
                JOINTLOADDIM o_dim = o_jt.value(ltype);
                if ( n_dim.notes != o_dim.notes
                        || qAbs(n_dim.fx - o_dim.fx) > 1.0e-5 || qAbs(n_dim.fy - o_dim.fy) > 1.0e-5
                        || qAbs(n_dim.fz - o_dim.fz) > 1.0e-5 || qAbs(n_dim.mx - o_dim.mx) > 1.0e-5
                        || qAbs(n_dim.my - o_dim.my) > 1.0e-5 || qAbs(n_dim.mz - o_dim.mz) > 1.0e-5 ) {
                    isChanged = true;
                    break;
                }
            }
            if ( isChanged ) changedList.append( jv.loadName );
        }
    }

    for ( int i = 0; i < jointLoadNameList.count() ; i++) {
        QString name = jointLoadNameList.at(i);
        if ( !cashNameList.contains(name) ) deletedList.append(name);
    }

    // 既存データのクリア
    jointLoadTable.clear();
    jointLoadNameList.clear();

    // キャッシュデータの登録

    if ( !cash_jLoadList.isEmpty() ) {
        Q_FOREACH (JOINTLOADVALUES jv, cash_jLoadList) {
            jointLoadTable.insert(jv.loadName, jv);
            jointLoadNameList.append(jv.loadName);
        }
    }

    if ( deletedList.count() > 0 ) emit loadDeleted(DATATYPE::TPJOINTLOAD, deletedList);
    if ( changedList.count() > 0 ) emit loadChanged(DATATYPE::TPJOINTLOAD, changedList);

    cash_jLoadList.clear();

    /*
        Q_FOREACH( JOINTLOADVALUES jv, jointLoadTable.values() ){
            qDebug() << jv.loadName;
            Q_FOREACH( LOADTYPE ltype, jv.loadTable.keys() ){
                int i1=ltype;
                qDebug() << i1 << jv.loadTable.value(ltype).notes
                       << jv.loadTable.value(ltype).fx << jv.loadTable.value(ltype).fy
                          << jv.loadTable.value(ltype).fz << jv.loadTable.value(ltype).mx
                             << jv.loadTable.value(ltype).my << jv.loadTable.value(ltype).mz;
            }
        }
    */
}

void ManageLoadData::registerMemberLoadData()
{
    QStringList cashNameList;
    QStringList changedList, deletedList;

    // 既存データとキャッシュデータの差分把握

    for (int i = 0; i < cash_mLoadList.count(); i++) {
        MEMBERLOADVALUES mv = cash_mLoadList.at(i);
        cashNameList.append(mv.loadName);
        if ( !memberLoadNameList.contains(mv.loadName) ) {
            changedList.append( mv.loadName );
        } else {
            QHash<LoadType::LOADTYPE, MEMBERLOADDIM> n_mt = mv.loadTable;
            QHash<LoadType::LOADTYPE, MEMBERLOADDIM> o_mt = memberLoadTable.value(mv.loadName).loadTable;
            if ( n_mt.count() != o_mt.count() ) {
                changedList.append( mv.loadName );
                continue;
            }
            bool isChanged = false;
            Q_FOREACH ( LoadType::LOADTYPE ltype, n_mt.keys() ) {
                if ( !o_mt.contains(ltype) ) {
                    isChanged = true;
                    break;
                }
                MEMBERLOADDIM n_dim = n_mt.value(ltype);
                MEMBERLOADDIM o_dim = o_mt.value(ltype);
                if ( n_dim.notes != o_dim.notes
                        || n_dim.corSytem != o_dim.corSytem || n_dim.loadDir != o_dim.loadDir
                        || qAbs(n_dim.wa - o_dim.wa) > 1.0e-5 || qAbs(n_dim.wb - o_dim.wb) > 1.0e-5 ) {
                    isChanged = true;
                    break;
                }
            }
            if ( isChanged ) changedList.append( mv.loadName );
        }
    }

    for ( int i = 0; i < memberLoadNameList.count() ; i++) {
        QString name = memberLoadNameList.at(i);
        if ( !cashNameList.contains(name) ) deletedList.append(name);
    }

    // 既存データのクリア
    memberLoadTable.clear();
    memberLoadNameList.clear();

    // キャッシュデータの登録

    if ( !cash_mLoadList.isEmpty() ) {

        Q_FOREACH (MEMBERLOADVALUES mv, cash_mLoadList) {
            memberLoadTable.insert(mv.loadName, mv);
            memberLoadNameList.append(mv.loadName);
        }
    }

    if ( deletedList.count() > 0 ) emit loadDeleted(DATATYPE::TPMEMBERLOAD, deletedList);
    if ( changedList.count() > 0 ) emit loadChanged(DATATYPE::TPMEMBERLOAD, changedList);

    cash_mLoadList.clear();
    /*
        Q_FOREACH( MEMBERLOADVALUES jv, memberLoadTable.values() ){
            qDebug() << jv.loadName;
            Q_FOREACH( LOADTYPE ltype, jv.loadTable.keys() ){
                int i1=ltype;
                int i2=jv.loadTable.value(ltype).corSytem;
                int i3=jv.loadTable.value(ltype).loadDir;
                qDebug() << i1 << jv.loadTable.value(ltype).notes
                       << i2 << i3
                          << jv.loadTable.value(ltype).wa << jv.loadTable.value(ltype).wb;
            }
        }
    */
}

void ManageLoadData::registerSlabLoadData()
{
    QStringList cashNameList;
    QStringList changedList, deletedList;

    // 既存データとキャッシュデータの差分把握

    for (int i = 0; i < cash_sLoadList.count(); i++) {
        SLABLOADVALUES sv = cash_sLoadList.at(i);
        cashNameList.append(sv.loadName);
        if ( !slabLoadNameList.contains(sv.loadName) ) {
            changedList.append( sv.loadName );
        } else {
            SLABLOADVALUES o_sv = slabLoadTable.value(sv.loadName);
            if ( sv.notes != o_sv.notes
                    || qAbs(sv.sDL - o_sv.sDL) > 1.0e-5 || qAbs(sv.sLL0 - o_sv.sLL0) > 1.0e-5
                    || qAbs(sv.sLL1 - o_sv.sLL1) > 1.0e-5 || qAbs(sv.sLL2 - o_sv.sLL2) > 1.0e-5
                    || qAbs(sv.sLL3 - o_sv.sLL3) > 1.0e-5 || qAbs(sv.sLL4 - o_sv.sLL4) > 1.0e-5
                    || qAbs(sv.sLLE - o_sv.sLLE) > 1.0e-5 ) {
                changedList.append( sv.loadName );
                break;
            }
        }
    }

    for ( int i = 0; i < slabLoadNameList.count() ; i++) {
        QString name = slabLoadNameList.at(i);
        if ( !cashNameList.contains(name) ) deletedList.append(name);
    }

    // 既存データのクリア
    slabLoadTable.clear();
    slabLoadNameList.clear();

    // キャッシュデータの登録

    if ( !cash_sLoadList.isEmpty() ) {

        Q_FOREACH (SLABLOADVALUES sv, cash_sLoadList) {
            slabLoadTable.insert(sv.loadName, sv);
            slabLoadNameList.append(sv.loadName);
        }
    }

    if ( deletedList.count() > 0 ) emit loadDeleted(DATATYPE::TPSLABLOAD, deletedList);
    if ( changedList.count() > 0 ) emit loadChanged(DATATYPE::TPSLABLOAD, changedList);

    cash_sLoadList.clear();
    /*
        Q_FOREACH( SLABLOADVALUES jv, slabLoadTable.values() ){
            qDebug() << jv.loadName << jv.notes
                   << jv.sDL << jv.sLL0 << jv.sLL1 << jv.sLL2 << jv.sLL3 << jv.sLL4 << jv.sLLE;
        }
    */
}

void ManageLoadData::registerJointLoadData(const QList<JOINTLOADVALUES> &loadlist)
{
    jointLoadTable.clear();
    jointLoadNameList.clear();
    cash_jLoadList.clear();

    Q_FOREACH (JOINTLOADVALUES sv, loadlist) {
        jointLoadTable.insert(sv.loadName, sv);
        jointLoadNameList.append(sv.loadName);
    }
}

void ManageLoadData::registerMemberLoadData(const QList<MEMBERLOADVALUES> &loadlist)
{
    memberLoadTable.clear();
    memberLoadNameList.clear();
    cash_mLoadList.clear();

    Q_FOREACH (MEMBERLOADVALUES sv, loadlist) {
        memberLoadTable.insert(sv.loadName, sv);
        memberLoadNameList.append(sv.loadName);
    }
}

void ManageLoadData::registerSlabLoadData(const QList<SLABLOADVALUES> &loadlist)
{
    slabLoadTable.clear();
    slabLoadNameList.clear();
    cash_sLoadList.clear();

    Q_FOREACH (SLABLOADVALUES sv, loadlist) {
        slabLoadTable.insert(sv.loadName, sv);
        slabLoadNameList.append(sv.loadName);
    }
}

void ManageLoadData::replaceLoadName(DATATYPE dtype, const QString &str_a, const QString &str_b)
{
    if ( dtype == DATATYPE::TPJOINTLOAD ) {
        int idx = jointLoadNameList.indexOf(str_a);
        if ( idx < 0 ) return;
        jointLoadNameList.replace(idx, str_b);
        JOINTLOADVALUES lv = jointLoadTable.value(str_a);
        lv.loadName = str_b;
        jointLoadTable.remove(str_a);
        jointLoadTable.insert(str_b, lv);
    } else if ( dtype == DATATYPE::TPMEMBERLOAD ) {
        int idx = memberLoadNameList.indexOf(str_a);
        if ( idx < 0 ) return;
        memberLoadNameList.replace(idx, str_b);
        MEMBERLOADVALUES lv = memberLoadTable.value(str_a);
        lv.loadName = str_b;
        memberLoadTable.remove(str_a);
        memberLoadTable.insert(str_b, lv);
    } else if ( dtype == DATATYPE::TPSLABLOAD ) {
        int idx = slabLoadNameList.indexOf(str_a);
        if ( idx < 0 ) return;
        slabLoadNameList.replace(idx, str_b);
        SLABLOADVALUES lv = slabLoadTable.value(str_a);
        lv.loadName = str_b;
        slabLoadTable.remove(str_a);
        slabLoadTable.insert(str_b, lv);
    }
}

void ManageLoadData::clearData()
{
    slabLoadTable.clear();
    slabLoadNameList.clear();
    memberLoadTable.clear();
    memberLoadNameList.clear();
    jointLoadTable.clear();
    jointLoadNameList.clear();
}

void ManageLoadData::clearTempData()
{
    cash_jLoadList.clear();
    cash_mLoadList.clear();
    cash_sLoadList.clear();
}

/* 荷重情報 */
JOINTLOADVALUES ManageLoadData::getJointLoadValue( const QString &str ) const
{
    return ( jointLoadTable.contains(str) ) ? jointLoadTable.value(str) : JOINTLOADVALUES();
}

MEMBERLOADVALUES ManageLoadData::getMemberLoadValue( const QString &str ) const
{
    return ( memberLoadTable.contains(str) ) ? memberLoadTable.value(str) : MEMBERLOADVALUES();
}

SLABLOADVALUES ManageLoadData::getSlabLoadValue( const QString &str ) const
{
    return ( slabLoadTable.contains(str) ) ? slabLoadTable.value(str) : SLABLOADVALUES();
}

QString ManageLoadData::loadNameToLoadProperty( DATATYPE dtype, const QString &str ) const
{
    QString detail;
    if ( dtype == DATATYPE::TPJOINTLOAD ) {
        if ( !jointLoadTable.contains(str) ) return detail;
        JOINTLOADVALUES lv = jointLoadTable.value(str);
        Q_FOREACH ( LoadType::LOADTYPE ltype, lv.loadTable.keys() ) {
            detail += LoadType::enumToString(ltype) + ":";
            qreal vv = lv.loadTable.value(ltype).fx;
            if ( qAbs(vv) > 1.0e-3 ) detail += QString(" Fx:%1kN,").arg(vv, 0, 'f', 3);
            vv = lv.loadTable.value(ltype).fy;
            if ( qAbs(vv) > 1.0e-3 ) detail += QString(" Fy:%1kN,").arg(vv, 0, 'f', 3);
            vv = lv.loadTable.value(ltype).fz;
            if ( qAbs(vv) > 1.0e-3 ) detail += QString(" Fz:%1kN,").arg(vv, 0, 'f', 3);
            vv = lv.loadTable.value(ltype).mx;
            if ( qAbs(vv) > 1.0e-3 ) detail += QString(" Mx:%1kNm,").arg(vv, 0, 'f', 3);
            vv = lv.loadTable.value(ltype).my;
            if ( qAbs(vv) > 1.0e-3 ) detail += QString(" My:%1kNm,").arg(vv, 0, 'f', 3);
            vv = lv.loadTable.value(ltype).mz;
            if ( qAbs(vv) > 1.0e-3 ) detail += QString(" Mz:%1kNm,").arg(vv, 0, 'f', 3);
            detail += "\n";
        }
    } else if ( dtype == DATATYPE::TPMEMBERLOAD ) {
        if ( !memberLoadTable.contains(str) ) return detail;
        MEMBERLOADVALUES lv = memberLoadTable.value(str);
        Q_FOREACH ( LoadType::LOADTYPE ltype, lv.loadTable.keys() ) {
            detail += LoadType::enumToString(ltype) + ": ";
            detail += loadDirToString(lv.loadTable.value(ltype).corSytem,
                                      lv.loadTable.value(ltype).loadDir) + ",";
            detail += QString(" Wa:%1kN/m,").arg(lv.loadTable.value(ltype).wa, 0, 'f', 3);
            detail += QString(" Wb:%1kN/m,").arg(lv.loadTable.value(ltype).wb, 0, 'f', 3);
            detail += "\n";
        }
    } else if ( dtype == DATATYPE::TPSLABLOAD ) {
        if ( !slabLoadTable.contains(str) ) return detail;
        SLABLOADVALUES lv = slabLoadTable.value(str);
        if ( qAbs(lv.sDL) > 1.0e-3 ) detail += QString("DL:%1N/m2, ").arg(qRound(lv.sDL));
        if ( qAbs(lv.sLL0) > 1.0e-3 ) detail += QString("LL0:%1N/m2, ").arg(qRound(lv.sLL0));
        if ( qAbs(lv.sLL1) > 1.0e-3 ) detail += QString("LL1:%1N/m2, ").arg(qRound(lv.sLL1));
        if ( qAbs(lv.sLL2) > 1.0e-3 ) detail += QString("LL2:%1N/m2, ").arg(qRound(lv.sLL2));
        if ( qAbs(lv.sLL3) > 1.0e-3 ) detail += QString("LL3:%1N/m2, ").arg(qRound(lv.sLL3));
        if ( qAbs(lv.sLL4) > 1.0e-3 ) detail += QString("LL4:%1N/m2, ").arg(qRound(lv.sLL4));
        if ( qAbs(lv.sLLE) > 1.0e-3 ) detail += QString("LLE:%1N/m2, ").arg(qRound(lv.sLLE));
        detail += "\n";
    }
    return detail;
}


QString ManageLoadData::loadDirToString( LCORSYSTEM lsys, LOADDIR ldir ) const
{
    QString str = ( lsys == LCORSYSTEM::LCOR_GLOBAL ) ? QString("GLOBAL-") : QString("LOCAL-");

    if ( ldir == LOADDIR::LDIR_X ) {
        str += ("X");
    } else if ( ldir == LOADDIR::LDIR_Y ) {
        str += ("Y");
    } else if ( ldir == LOADDIR::LDIR_Z ) {
        str += ("Z");
    } else if ( ldir == LOADDIR::LDIR_MX ) {
        str += ("MX");
    } else if ( ldir == LOADDIR::LDIR_MY ) {
        str += ("MY");
    } else if ( ldir == LOADDIR::LDIR_MZ ) {
        str += ("MZ");
    }

    return str;
}

QList<JOINTLOADVALUES> ManageLoadData::getJointLoadList() const{
    QList<JOINTLOADVALUES> ret;
    Q_FOREACH(QString jointLoad, jointLoadNameList){
        ret.append(jointLoadTable[jointLoad]);
    }
    return ret;
}
QList<MEMBERLOADVALUES> ManageLoadData::getMemberLoadList() const{
    QList<MEMBERLOADVALUES> ret;
    Q_FOREACH(QString loadName, memberLoadNameList){
        Q_FOREACH(MEMBERLOADVALUES ml, memberLoadTable.values(loadName)){
            ret.append(ml);
        }
    }
    return ret;
}
QList<SLABLOADVALUES> ManageLoadData::getSlabLoadList() const{
    QList<SLABLOADVALUES> ret;
    Q_FOREACH(QString slabLoad, slabLoadNameList){
        ret.append(slabLoadTable[slabLoad]);
    }
    return ret;
}

QString ManageLoadData::writePsvData(QSharedPointer<N3DUuidTable> table) const{
    QString text;
    text.append("**JOINTLOAD").append(",").append(QString::number(jointLoadNameList.count())).append("\n");
    text.append("NO,NAME,TYPE,FX,FY,FZ,MX,MY,MZ,COMMENT\n");
    Q_FOREACH(JOINTLOADVALUES joint, jointLoadTable.values()){
        text.append(joint.writeData()).append("\n");
    }
    text.append("**MEMBERLOAD").append(",").append(QString::number(memberLoadNameList.count())).append("\n");
    text.append("NO,NAME,TYPE,COORDINATE,DIRECTION,WA,WB,COMMENT\n");
    Q_FOREACH(MEMBERLOADVALUES member, memberLoadTable.values()){
        text.append(member.writeData()).append("\n");
    }
    text.append("**SLABLOAD").append(",").append(QString::number(slabLoadNameList.count())).append("\n");
    text.append("NO,NAME,DL,LL0,LL1,LL2,LL3,LL4,LLE,COMMENT\n");
    Q_FOREACH(SLABLOADVALUES slab, slabLoadTable.values()){
        text.append(slab.writeData()).append("\n");
    }
    return text;
}

} // namespace post3dapp
