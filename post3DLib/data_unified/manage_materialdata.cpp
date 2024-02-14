#include "manage_materialdata.h"

#include <QDebug>
#include <QMessageBox>

#include "define_unifieddata.h"

namespace post3dapp{
const int ManageMaterialData::deIntList[] = {6, 8, 10, 13, 16, 19, 22, 25, 29, 32, 35, 38, 41, 51};
const int ManageMaterialData::roIntList[] = {4, 5, 6, 7, 8, 9, 12, 13, 16, 19, 22, 25, 28, 32};
const int ManageMaterialData::pcIntList[] = {6, 7, 9, 11, 13};

ManageMaterialData::ManageMaterialData(const ManageMaterialData& material, QObject *parent)
    :QObject(parent)
{
    QList<MATERIALVALUES> mvList;
    Q_FOREACH(auto name, material.concreteNameList){
        mvList.append(material.concreteTable.value(name));
    }

    Q_FOREACH(auto name, material.steelNameList){
        mvList.append(material.steelTable.value(name));
    }

    Q_FOREACH(auto name, material.reinforceNameList){
        mvList.append(material.reinforceTable.value(name));
    }
    registerTempData(mvList);
}


QStringList ManageMaterialData::getNameList( DATATYPE dtype ) const
{
    if ( dtype == DATATYPE::TPMACONC ) {
        return concreteNameList;
    } else if ( dtype == DATATYPE::TPMASTEEL ) {
        return steelNameList;
    } else if ( dtype == DATATYPE::TPMAREIN ) {
        return getReinforceList();
    } else if ( dtype == DATATYPE::TPMAREIND ) {
        return getReinforcePairList();
    }
    return QStringList();
}

QStringList ManageMaterialData::getReinforceNameList() const
{
    return reinforceNameList;
}

QStringList ManageMaterialData::getReinforceList() const
{
    QList<int> dlist, elist, flist, slist, rlist, ulist;
    Q_FOREACH ( MATERIALVALUES value, reinforceTable.values() ) {
        QString symbol = value.symbol;
        int st = value.barMin;
        int en = ( value.barMax == 0 ) ? 999 : value.barMax;
        if ( symbol.isEmpty() || symbol == "D" ) {
            for ( int i = 0; i < 14; i++ ) {
                int ii = deIntList[i];
                if ( ii < st || ii > en ) continue;
                if ( !dlist.contains(ii) ) dlist.append(ii);
            }
        } else if ( symbol == "E" ) {
            for ( int i = 0; i < 14; i++ ) {
                int ii = deIntList[i];
                if ( ii < st || ii > en ) continue;
                if ( !elist.contains(ii) ) elist.append(ii);
            }
        } else if ( symbol == "F" ) {
            for ( int i = 0; i < 14; i++ ) {
                int ii = deIntList[i];
                if ( ii < st || ii > en ) continue;
                if ( !flist.contains(ii) ) flist.append(ii);
            }
        } else if ( symbol == "S" ) {
            for ( int i = 0; i < 14; i++ ) {
                int ii = deIntList[i];
                if ( ii < st || ii > en ) continue;
                if ( !slist.contains(ii) ) slist.append(ii);
            }
        } else if ( symbol == "R" ) {
            for ( int i = 0; i < 14; i++ ) {
                int ii = roIntList[i];
                if ( ii < st || ii > en ) continue;
                if ( !rlist.contains(ii) ) rlist.append(ii);
            }
        } else if ( symbol == "U" ) {
            for ( int i = 0; i < 5; i++ ) {
                int ii = pcIntList[i];
                if ( ii < st || ii > en ) continue;
                if ( !ulist.contains(ii) ) ulist.append(ii);
            }
        }
    }
    QStringList reinlist;
    if ( !dlist.isEmpty() ) {
        qSort(dlist.begin(), dlist.end());
        Q_FOREACH ( int ii, dlist ) reinlist.append(QString("D%1").arg(ii));
    }
    if ( !elist.isEmpty() ) {
        qSort(elist.begin(), elist.end());
        Q_FOREACH ( int ii, elist ) reinlist.append(QString("E%1").arg(ii));
    }
    if ( !flist.isEmpty() ) {
        qSort(flist.begin(), flist.end());
        Q_FOREACH ( int ii, flist ) reinlist.append(QString("F%1").arg(ii));
    }
    if ( !slist.isEmpty() ) {
        qSort(slist.begin(), slist.end());
        Q_FOREACH ( int ii, slist ) reinlist.append(QString("S%1").arg(ii));
    }
    if ( !rlist.isEmpty() ) {
        qSort(rlist.begin(), rlist.end());
        Q_FOREACH ( int ii, rlist ) reinlist.append(QString("R%1").arg(ii));
    }
    if ( !ulist.isEmpty() ) {
        qSort(ulist.begin(), ulist.end());
        Q_FOREACH ( int ii, ulist ) reinlist.append(QString("U%1").arg(ii));
    }
    return reinlist;
}

QStringList ManageMaterialData::getReinforcePairList() const
{
    QList<int> dlist, elist, flist, slist;
    Q_FOREACH ( MATERIALVALUES value, reinforceTable.values() ) {
        QString symbol = value.symbol;
        int st = value.barMin;
        int en = ( value.barMax == 0 ) ? 999 : value.barMax;
        if ( symbol.isEmpty() || symbol == "D" ) {
            for ( int i = 0; i < 14; i++ ) {
                int ii = deIntList[i];
                if ( ii < st || ii > en ) continue;
                if ( !dlist.contains(ii) ) dlist.append(ii);
            }
        } else if ( symbol == "E" ) {
            for ( int i = 0; i < 14; i++ ) {
                int ii = deIntList[i];
                if ( ii < st || ii > en ) continue;
                if ( !elist.contains(ii) ) elist.append(ii);
            }
        } else if ( symbol == "F" ) {
            for ( int i = 0; i < 14; i++ ) {
                int ii = deIntList[i];
                if ( ii < st || ii > en ) continue;
                if ( !flist.contains(ii) ) flist.append(ii);
            }
        } else if ( symbol == "S" ) {
            for ( int i = 0; i < 14; i++ ) {
                int ii = deIntList[i];
                if ( ii < st || ii > en ) continue;
                if ( !slist.contains(ii) ) slist.append(ii);
            }
        }
    }
    QStringList reinlist;
    if ( !dlist.isEmpty() ) {
        qSort(dlist.begin(), dlist.end());
        for ( int i = 0; i < dlist.count(); i++ ) {
            if ( i > 0 ) reinlist.append(QString("D%1D%2").arg(dlist.at(i - 1)).arg(dlist.at(i)));
            reinlist.append(QString("D%1").arg(dlist.at(i)));
        }
    }
    if ( !elist.isEmpty() ) {
        qSort(elist.begin(), elist.end());
        for ( int i = 0; i < elist.count(); i++ ) {
            if ( i > 0 ) reinlist.append(QString("E%1E%2").arg(elist.at(i - 1)).arg(elist.at(i)));
            reinlist.append(QString("E%1").arg(elist.at(i)));
        }
    }
    if ( !flist.isEmpty() ) {
        qSort(flist.begin(), flist.end());
        for ( int i = 0; i < flist.count(); i++ ) {
            if ( i > 0 ) reinlist.append(QString("F%1F%2").arg(flist.at(i - 1)).arg(flist.at(i)));
            reinlist.append(QString("F%1").arg(flist.at(i)));
        }
    }
    if ( !slist.isEmpty() ) {
        qSort(slist.begin(), slist.end());
        for ( int i = 0; i < slist.count(); i++ ) {
            if ( i > 0 ) reinlist.append(QString("S%1S%2").arg(slist.at(i - 1)).arg(slist.at(i)));
            reinlist.append(QString("S%1").arg(slist.at(i)));
        }
    }
    return reinlist;
}

MATERIALVALUES ManageMaterialData::getReinforceValue( const QString &str ) const
{
    if ( str.isEmpty() ) return MATERIALVALUES();
    QChar symbol = str.at(0);
    QStringList strlist = str.split(symbol);
    int ii = strlist.at(1).toInt(); // D10D13 などは最初のD10で判定

    Q_FOREACH ( MATERIALVALUES value, reinforceTable.values() ) {
        if ( QString(symbol) != value.symbol ) continue;
        int st = value.barMin;
        int en = ( value.barMax == 0 ) ? 999 : value.barMax;
        if ( ii >= st && ii <= en ) return value;
    }
    return MATERIALVALUES();
}

int ManageMaterialData::dataCount(DATATYPE type) const
{
    if ( type == DATATYPE::TPMACONC ) {
        return concreteNameList.count();
    } else if ( type == DATATYPE::TPMASTEEL ) {
        return steelNameList.count();
    } else if ( type == DATATYPE::TPMAREIN || type == DATATYPE::TPMAREIND ) {
        return reinforceNameList.count();
    }
    return 0;
}

bool ManageMaterialData::isExistedName(const QString &str, DATATYPE dtype) const
{
    return ( getNameList(dtype).contains(str) ) ;
}

bool ManageMaterialData::checkTempData(QWidget *parent, const QList<MATERIALVALUES> &mvlist)
{
    clearTempData();

    QStringList nameList, errNamelist;
    bool undefinedName = false;
    Q_FOREACH ( MATERIALVALUES mv, mvlist ) {
        QString mname = mv.materialName;
        MaterialType::Type mtype = mv.materialType;
        if ( mname.isEmpty() ) {
            undefinedName = true;
            break;
        }
        if ( nameList.contains(mname) ) errNamelist.append(mname);
        nameList.append(mname);
        if ( mtype == MaterialType::Type::CONCRETE ) {
            cash_concreteList.append(mv);
        } else if ( mtype == MaterialType::Type::STEEL ) {
            cash_steelList.append(mv);
        } else {
            cash_reinforceList.append(mv);
        }
    }

    if ( undefinedName ) {
        errorUndefinedName(parent);
        return false;
    }
    if ( !errNamelist.isEmpty() ) {
        errorOverlapNames(parent, errNamelist);
        return false;
    }

    return true;
}

void ManageMaterialData::registerTempData()
{
    clearData();

    Q_FOREACH ( MATERIALVALUES mv, cash_concreteList ) {
        concreteNameList.append(mv.materialName);
        concreteTable.insert(mv.materialName, mv);
    }
    Q_FOREACH ( MATERIALVALUES mv, cash_steelList ) {
        steelNameList.append(mv.materialName);
        steelTable.insert(mv.materialName, mv);
    }
    Q_FOREACH ( MATERIALVALUES mv, cash_reinforceList ) {
        reinforceNameList.append(mv.materialName);
        reinforceTable.insert(mv.materialName, mv);
    }

    clearTempData();
    emit materialChanged();

    /*
        QStringList cashNameList;
        bool isChanged=( concreteNameList.count() != cash_concreteList.count() );
        Q_FOREACH( MATERIALVALUES mv, cash_concreteList ){
            cashNameList.append(mv.materialName);
            if( isChanged ) continue;
            if( !concreteNameList.contains(mv.materialName) ) isChanged=true;
        }
        concreteNameList=cashNameList;
        if( isChanged ) emit materialChanged(MaterialType::CONCRETE,concreteNameList);

        cashNameList.clear();
        isChanged=( steelNameList.count() != cash_steelList.count() );
        Q_FOREACH( MATERIALVALUES mv, cash_steelList ){
            cashNameList.append(mv.materialName);
            if( isChanged ) continue;
            if( !steelNameList.contains(mv.materialName) ) isChanged=true;
        }
        steelNameList=cashNameList;
        if( isChanged ) emit materialChanged(MaterialType::STEEL,steelNameList);
    */


}

void ManageMaterialData::registerTempData(const QList<MATERIALVALUES> &mvlist)
{
    clearData();

    Q_FOREACH ( MATERIALVALUES mv, mvlist ) {
        if ( mv.materialType == MaterialType::Type::CONCRETE ) {
            concreteNameList.append(mv.materialName);
            concreteTable.insert(mv.materialName, mv);
        } else if ( mv.materialType == MaterialType::Type::STEEL ) {
            steelNameList.append(mv.materialName);
            steelTable.insert(mv.materialName, mv);
        } else {
            reinforceNameList.append(mv.materialName);
            reinforceTable.insert(mv.materialName, mv);
        }
    }
}

void ManageMaterialData::clearData()
{
    concreteNameList.clear();
    concreteTable.clear();
    steelNameList.clear();
    steelTable.clear();
    reinforceNameList.clear();
    reinforceTable.clear();
}

void ManageMaterialData::clearTempData()
{
    cash_concreteList.clear();
    cash_steelList.clear();
    cash_reinforceList.clear();
}

MATERIALVALUES ManageMaterialData::getMaterialValue( DATATYPE mtype, const QString &mname ) const
{
    if ( mtype == DATATYPE::TPMACONC ) {
        if ( concreteTable.contains(mname) ) return concreteTable.value(mname);
    } else if ( mtype == DATATYPE::TPMASTEEL ) {
        if ( steelTable.contains(mname) ) return steelTable.value(mname);
    } else if ( mtype == DATATYPE::TPMAREIN || mtype == DATATYPE::TPMAREIND ) {
        if ( reinforceTable.contains(mname) ) return reinforceTable.value(mname);
    }
    return MATERIALVALUES();
}

void ManageMaterialData::errorUndefinedName(QWidget *parent)
{
    QMessageBox::critical( parent, u8"エラー -- 材料名の未定義", u8"材料名が定義されていません", QMessageBox::Ok);
}

void ManageMaterialData::errorOverlapNames(QWidget *parent, const QStringList &names)
{
    QMessageBox::critical( parent, u8"エラー -- 材料名の重複", u8"材料名が重複しています\n" + names.join(","), QMessageBox::Ok);
}

QString ManageMaterialData::writeData() const{
    QString text;
    text.append("*MATERIAL").append("\n");
    text.append("**CONCRETE").append(",").append(QString::number(concreteNameList.count())).append("\n");
    text.append(u8"#材種名, E(N/mm2), G(N/mm2), 単位重量1(kN/m3), 単位荷重2(kN/m3), ヤング係数比, F値(N/mm2), 引張長期(N/mm2), 引張短期(N/mm2), 引張終局(N/mm2), 圧縮長期(N/mm2), 圧縮短期(N/mm2), 圧縮終局(N/mm2), せん断長期(N/mm2), せん断短期(N/mm2), せん断終局(N/mm2), 付着長期(N/mm2), 付着短期(N/mm2), 付着終局(N/mm2), 付着上端長期(N/mm2), 付着上端短期(N/mm2)").append("\n");
    Q_FOREACH(MATERIALVALUES mv, concreteTable.values()){
        text.append(mv.writeData()).append("\n");
    }
    text.append("**STEEL").append(",").append(QString::number(steelNameList.count())).append("\n");
    text.append(u8"材種名, E(N/mm2), G(N/mm2), 単位重量1(kN/m3), F値(N/mm2), 引張長期(N/mm2), 引張短期(N/mm2), 引張終局(N/mm2), 圧縮長期(N/mm2), 圧縮短期(N/mm2), 圧縮終局(N/mm2), せん断長期(N/mm2), せん断短期(N/mm2), せん断終局(N/mm2)").append("\n");
    Q_FOREACH(MATERIALVALUES mv, steelTable.values()){
        text.append(mv.writeData()).append("\n");
    }
    text.append("**REINFORCE").append(",").append(QString::number(reinforceNameList.count())).append("\n");
    text.append(u8"#材種名, 鉄筋シンボル記号, 鉄筋範囲D～, 鉄筋範囲～D, E(N/mm2), 引張長期(N/mm2), 引張短期(N/mm2), 引張終局(N/mm2), 圧縮長期(N/mm2), 圧縮短期(N/mm2), 圧縮終局(N/mm2), せん断長期(N/mm2), せん断短期(N/mm2), せん断終局(N/mm2)").append("\n");
    Q_FOREACH(MATERIALVALUES mv, reinforceTable.values()){
        text.append(mv.writeData()).append("\n");
    }

    return text;
}


/*


QStringList ManageMaterialData::getNameList(DATATYPE type) const
{
    if( type==TPJOINTLOAD ){ return jointLoadNameList; }
    else if( type==TPMEMBERLOAD ){ return memberLoadNameList; }
    else if( type==TPSLABLOAD ){ return slabLoadNameList; }
    return QStringList();
}

bool ManageJoadData::checkJointLoadData(QWidget* parent,const QList<JOINTLOADVALUES> & loadlist)
{
    cash_jLoadList=loadlist;
    return true;
}

bool ManageJoadData::checkMemberLoadData(QWidget* parent,const QList<MEMBERLOADVALUES> & loadlist)
{
    cash_mLoadList=loadlist;
    return true;
}

bool ManageJoadData::checkSlabLoadData(QWidget* parent,const QList<SLABLOADVALUES> & loadlist)
{
    cash_sLoadList=loadlist;
    return true;
}

void ManageJoadData::registerJointLoadData()
{
    QStringList cashNameList;
    QStringList changedList,deletedList;

    // 既存データとキャッシュデータの差分把握

    for(int i=0; i<cash_jLoadList.count(); i++){
        JOINTLOADVALUES jv=cash_jLoadList.at(i);
        cashNameList.append(jv.loadName);
        if( !jointLoadNameList.contains(jv.loadName) ){
            changedList.append( jv.loadName );
        }else{
            QHash<LOADTYPE,JOINTLOADDIM> n_jt=jv.loadTable;
            QHash<LOADTYPE,JOINTLOADDIM> o_jt=jointLoadTable.value(jv.loadName).loadTable;
            if( n_jt.count() != o_jt.count() ){
                changedList.append( jv.loadName );
                continue;
            }
            bool isChanged=false;
            Q_FOREACH( LOADTYPE ltype, n_jt.keys() ){
                if( !o_jt.contains(ltype) ){ isChanged=true; break; }
                JOINTLOADDIM n_dim=n_jt.value(ltype);
                JOINTLOADDIM o_dim=o_jt.value(ltype);
                if( n_dim.notes!=o_dim.notes
                        || qAbs(n_dim.fx-o_dim.fx)>1.0e-5 || qAbs(n_dim.fy-o_dim.fy)>1.0e-5
                        || qAbs(n_dim.fz-o_dim.fz)>1.0e-5 || qAbs(n_dim.mx-o_dim.mx)>1.0e-5
                        || qAbs(n_dim.my-o_dim.my)>1.0e-5 || qAbs(n_dim.mz-o_dim.mz)>1.0e-5 ){
                    isChanged=true;
                    break;
                }
            }
            if( isChanged ) changedList.append( jv.loadName );
        }
    }

    for( int i=0; i<jointLoadNameList.count() ; i++){
        QString name=jointLoadNameList.at(i);
        if( !cashNameList.contains(name) ) deletedList.append(name);
    }

    // 既存データのクリア
    jointLoadTable.clear();
    jointLoadNameList.clear();

    // キャッシュデータの登録

    if( !cash_jLoadList.isEmpty() ){
        Q_FOREACH(JOINTLOADVALUES jv, cash_jLoadList){
            jointLoadTable.insert(jv.loadName,jv);
            jointLoadNameList.append(jv.loadName);
        }
    }

    if( deletedList.count()>0 ) emit loadDeleted(TPJOINTLOAD,deletedList);
    if( changedList.count()>0 ) emit loadChanged(TPJOINTLOAD,changedList);

    cash_jLoadList.clear();
}

void ManageJoadData::registerMemberLoadData()
{
    QStringList cashNameList;
    QStringList changedList,deletedList;

    // 既存データとキャッシュデータの差分把握

    for(int i=0; i<cash_mLoadList.count(); i++){
        MEMBERLOADVALUES mv=cash_mLoadList.at(i);
        cashNameList.append(mv.loadName);
        if( !memberLoadNameList.contains(mv.loadName) ){
            changedList.append( mv.loadName );
        }else{
            QHash<LOADTYPE,MEMBERLOADDIM> n_mt=mv.loadTable;
            QHash<LOADTYPE,MEMBERLOADDIM> o_mt=memberLoadTable.value(mv.loadName).loadTable;
            if( n_mt.count() != o_mt.count() ){
                changedList.append( mv.loadName );
                continue;
            }
            bool isChanged=false;
            Q_FOREACH( LOADTYPE ltype, n_mt.keys() ){
                if( !o_mt.contains(ltype) ){ isChanged=true; break; }
                MEMBERLOADDIM n_dim=n_mt.value(ltype);
                MEMBERLOADDIM o_dim=o_mt.value(ltype);
                if( n_dim.notes!=o_dim.notes
                        || n_dim.corSytem!=o_dim.corSytem || n_dim.loadDir!=o_dim.loadDir
                        || qAbs(n_dim.wa-o_dim.wa)>1.0e-5 || qAbs(n_dim.wb-o_dim.wb)>1.0e-5 ){
                    isChanged=true;
                    break;
                }
            }
            if( isChanged ) changedList.append( mv.loadName );
        }
    }

    for( int i=0; i<memberLoadNameList.count() ; i++){
        QString name=memberLoadNameList.at(i);
        if( !cashNameList.contains(name) ) deletedList.append(name);
    }

    // 既存データのクリア
    memberLoadTable.clear();
    memberLoadNameList.clear();

    // キャッシュデータの登録

    if( !cash_mLoadList.isEmpty() ){

        Q_FOREACH(MEMBERLOADVALUES mv, cash_mLoadList){
            memberLoadTable.insert(mv.loadName,mv);
            memberLoadNameList.append(mv.loadName);
        }
    }

    if( deletedList.count()>0 ) emit loadDeleted(TPMEMBERLOAD,deletedList);
    if( changedList.count()>0 ) emit loadChanged(TPMEMBERLOAD,changedList);

    cash_mLoadList.clear();
}

void ManageJoadData::registerSlabLoadData()
{
    QStringList cashNameList;
    QStringList changedList,deletedList;

    // 既存データとキャッシュデータの差分把握

    for(int i=0; i<cash_sLoadList.count(); i++){
        SLABLOADVALUES sv=cash_sLoadList.at(i);
        cashNameList.append(sv.loadName);
        if( !slabLoadNameList.contains(sv.loadName) ){
            changedList.append( sv.loadName );
        }else{
            SLABLOADVALUES o_sv=slabLoadTable.value(sv.loadName);
            if( sv.notes!=o_sv.notes
                    || qAbs(sv.sDL-o_sv.sDL)>1.0e-5 || qAbs(sv.sLL0-o_sv.sLL0)>1.0e-5
                    || qAbs(sv.sLL1-o_sv.sLL1)>1.0e-5 || qAbs(sv.sLL2-o_sv.sLL2)>1.0e-5
                    || qAbs(sv.sLL3-o_sv.sLL3)>1.0e-5 || qAbs(sv.sLL4-o_sv.sLL4)>1.0e-5
                    || qAbs(sv.sLLE-o_sv.sLLE)>1.0e-5 ){
                changedList.append( sv.loadName );
                break;
            }
         }
    }

    for( int i=0; i<slabLoadNameList.count() ; i++){
        QString name=slabLoadNameList.at(i);
        if( !cashNameList.contains(name) ) deletedList.append(name);
    }

    // 既存データのクリア
    slabLoadTable.clear();
    slabLoadNameList.clear();

    // キャッシュデータの登録

    if( !cash_sLoadList.isEmpty() ){

        Q_FOREACH(SLABLOADVALUES sv, cash_sLoadList){
            slabLoadTable.insert(sv.loadName,sv);
            slabLoadNameList.append(sv.loadName);
        }
    }

    if( deletedList.count()>0 ) emit loadDeleted(TPSLABLOAD,deletedList);
    if( changedList.count()>0 ) emit loadChanged(TPSLABLOAD,changedList);

    cash_sLoadList.clear();
}

void ManageJoadData::registerJointLoadData(const QList<JOINTLOADVALUES> & loadlist)
{
    jointLoadTable.clear();
    jointLoadNameList.clear();
    cash_jLoadList.clear();

    Q_FOREACH(JOINTLOADVALUES sv, loadlist){
        jointLoadTable.insert(sv.loadName,sv);
        jointLoadNameList.append(sv.loadName);
    }
}

void ManageJoadData::registerMemberLoadData(const QList<MEMBERLOADVALUES> & loadlist)
{
    memberLoadTable.clear();
    memberLoadNameList.clear();
    cash_mLoadList.clear();

    Q_FOREACH(MEMBERLOADVALUES sv, loadlist){
        memberLoadTable.insert(sv.loadName,sv);
        memberLoadNameList.append(sv.loadName);
    }
}

void ManageJoadData::registerSlabLoadData(const QList<SLABLOADVALUES> & loadlist)
{
    slabLoadTable.clear();
    slabLoadNameList.clear();
    cash_sLoadList.clear();

    Q_FOREACH(SLABLOADVALUES sv, loadlist){
        slabLoadTable.insert(sv.loadName,sv);
        slabLoadNameList.append(sv.loadName);
    }
}

void ManageJoadData::clearData()
{
    slabLoadTable.clear();
    slabLoadNameList.clear();
    memberLoadTable.clear();
    memberLoadNameList.clear();
    jointLoadTable.clear();
    jointLoadNameList.clear();
}

void ManageJoadData::clearTempData()
{
    cash_jLoadList.clear();
    cash_mLoadList.clear();
    cash_sLoadList.clear();
}

JOINTLOADVALUES ManageJoadData::getJointLoadValue( const QString &str ) const
{
    return ( jointLoadTable.contains(str) ) ? jointLoadTable.value(str) : JOINTLOADVALUES();
}

MEMBERLOADVALUES ManageJoadData::getMemberLoadValue( const QString &str ) const
{
    return ( memberLoadTable.contains(str) ) ? memberLoadTable.value(str) : MEMBERLOADVALUES();
}

SLABLOADVALUES ManageJoadData::getSlabLoadValue( const QString &str ) const
{
    return ( slabLoadTable.contains(str) ) ? slabLoadTable.value(str) : SLABLOADVALUES();
}
*/
} // namespace post3dapp
