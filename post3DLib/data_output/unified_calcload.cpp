#include "unified_calcload.h"

#include <QDateTime>
#include <QDebug>
#include <QProgressDialog>

#include "define_draw_figure.h"
#include "define_output.h"
#include "fixed_data.h"
#include "inputdatacheck.h"
#include "manage_jointdata.h"

namespace post3dapp{
UnifiedCalcLoad::UnifiedCalcLoad()
{
}


QString UnifiedCalcLoad::executeCalculation(const QString& o_fpath, QProgressDialog* progress, bool writeOutput)
{
    load.deleteCashData();

    if (progress != nullptr){
        progress->setWindowModality( Qt::WindowModal );
        progress->setWindowTitle(u8"荷重拾い計算");
        progress->setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
        progress->setFont( QFont(tr("Microsoft JhengHei"), 9) );
        qApp->processEvents();
        progress->setValue(0);
    }

    InputDataCheck check;
    QString err = check.executeCheck(progress);
    if (!err.isEmpty() ){
        if (progress != nullptr){
            progress->setValue(100);
            progress->close();
        }

        QString msg = u8"入力データにエラーがありました。\n";
        msg += err;
        return msg;
    }
    else{
        if (progress != nullptr){
            progress->setValue(50);
        }
    }

    err = load.executeCalculation(o_fpath, progress);
    if (progress != nullptr){
        progress->setValue(100);
        progress->close();
    }

    if ( !err.isEmpty() ) {
        QString msg = u8"荷重拾いに失敗しました。\n";
        msg += err;
        return msg;
    }

    if (writeOutput){
        err = load.write(o_fpath);
        if (!err.isEmpty()){
            QString msg = u8"荷重書き込みに失敗しました。\n";
            msg += err;
            return msg;
        }
    }

    return QString{};
}


QList<JLOADSUM *> UnifiedCalcLoad::loadOfJoint( JointData* jd ) const
{
    return load.loadOfJoint(jd);
}

bool UnifiedCalcLoad::isLoadedJoint( JointData* jd ) const
{
    return load.isLoadedJoint(jd);
}

bool UnifiedCalcLoad::isLoadedMember( MemberData* md ) const
{
    return load.isLoadeMember(md);
}

QList<MLOADSUM *> UnifiedCalcLoad::loadOfMember( MemberData* md ) const
{
    return load.loadOfMember(md);
}

QHash<QString, double> UnifiedCalcLoad::standardAreaList = QHash<QString, double>();

QHash<QString, SteelType::Type> UnifiedCalcLoad::standardTypeList = QHash<QString, SteelType::Type>();

double UnifiedCalcLoad::getStandardArea(const QString &standardName)
{
    if (standardAreaList.count() == 0){
        QDir dir(":/post3DLib/include_files/standard_steel");
        QFileInfoList ilist = dir.entryInfoList(QDir::Files);
        for (int i = 0; i < ilist.count() ; i++) {
            if ( ilist.at(i).suffix() != "csv" )
                continue;
            QString filename = ilist.at(i).filePath();
            QFile file(filename);
            if ( !file.open(QIODevice::ReadOnly) )
                continue;
            QTextStream in(&file);
            QStringList words;
            while ( !in.atEnd() ) {
                words = in.readLine().split(',', QString::KeepEmptyParts);
                if ( words.count() < 3 )
                    continue;
                QString startCharacter = words.at(0);
                if ( startCharacter == "#" || startCharacter.isEmpty() )
                    continue;
                QString name = startCharacter + QString("-") + words.at(1);
                //QString name = startCharacter + QString("-") + words.at(1) + QString("-") + words.at(2);
                double area = words.at(3).toDouble() * 100.0;
                standardAreaList.insert(name, area);
            }
            file.close();
        }

    }
    if (!standardAreaList.contains(standardName))
        qFatal(QString{standardName + u8"が規格断面に見つかりません"}.toUtf8().data());

    return standardAreaList.value(standardName);
}

SteelType::Type UnifiedCalcLoad::getStandardType(const QString &typeName)
{
    if (standardTypeList.count() == 0){
        QDir dir(":/post3DLib/include_files/standard_steel");
        QFileInfoList ilist = dir.entryInfoList(QDir::Files);
        for (int i = 0; i < ilist.count() ; i++) {
            if ( ilist.at(i).suffix() != "csv" )
                continue;
            QString filename = ilist.at(i).filePath();
            QFile file(filename);
            if ( !file.open(QIODevice::ReadOnly) )
                continue;
            QTextStream in(&file);
            QStringList words;
            while ( !in.atEnd() ) {
                words = in.readLine().split(',', QString::KeepEmptyParts);
                if ( words.count() < 3 )
                    continue;
                QString startCharacter = words.at(0);
                if ( startCharacter == "#" || startCharacter.isEmpty() )
                    continue;
                SteelType::Type type = SteelType::stringToEnum(startCharacter);
                if (type != SteelType::Type::UNDEFINED){
                    QString name = ilist.at(i).baseName();
                    standardTypeList.insert(name, type);
                    break;
                }
            }
            file.close();
        }

    }
    if (!standardTypeList.contains(typeName))
        qFatal(QString{typeName + u8"が見つかりません"}.toUtf8().data());

    return standardTypeList.value(typeName);
}

QString UnifiedCalcLoad::getLoadFileDateTime(const QString& o_fpath) const
{
    QFileInfo info(o_fpath);
    if ( !info.exists() ) return QString();

    QDateTime cdate = info.lastModified();
    return cdate.toString("yyyy/MM/dd hh:mm:ss");
}


void UnifiedCalcLoad::readDataOnMemory(const QString& o_fpath)
{
    load.read(o_fpath);
    emit dataUpdated();
    qDebug()<<"read data done";
}

/*

void UnifiedCalcLoad::updateCalcLoadData( const QMultiHash<MemberData*,MLOADSUM*> & c_mloads,
                                        const QMultiHash<JointData*,JLOADSUM*> & c_jloads )
{
    clearCalcLoadData();

    Q_FOREACH( MemberData* md, c_mloads.uniqueKeys() ){

        Q_FOREACH( MLOADSUM* mload, c_mloads.values(md) ){

            VEC3D gvec;
            if( mload->loadDir==LDIR_X || mload->loadDir==LDIR_MX ){
                gvec=VEC3D(1.0,0.0,0.0);
            }else if( mload->loadDir==LDIR_Y || mload->loadDir==LDIR_MY ){
                gvec=VEC3D(0.0,1.0,0.0);
            }else if( mload->loadDir==LDIR_Z || mload->loadDir==LDIR_MZ ){
                gvec=VEC3D(0.0,0.0,1.0);
            }
            bool isMomentLoad=( mload->loadDir==LDIR_MX || mload->loadDir==LDIR_MY
                                || mload->loadDir==LDIR_MZ );
            VEC3D u_v=md->getUVector();
            VEC3D v_v=md->getVVector();
            VEC3D w_v=md->getWVector();
            VEC3D lvec=CalcVector3D::globalToLocal(u_v,v_v,w_v,VEC3D(),gvec);

            QPointF wa=mload->loadWAB.loadWA;
            QPointF wb=mload->loadWAB.loadWB;
            if( !mload->sumUpLoad.isEmpty() ){
                if( qAbs(lvec.x)>1.0e-5 ){
                    QList<qreal> sload;
                    Q_FOREACH( qreal ll, mload->sumUpLoad ) sload.append( ll*lvec.x );
                    LOADDIR dir= ( isMomentLoad ) ? LDIR_MX : LDIR_X ;
                    appendMemberLoad(md->getUuid(),mload->loadName,mload->loadType,dir,sload);
                }
                if( qAbs(lvec.y)>1.0e-5 ){
                    QList<qreal> sload;
                    Q_FOREACH( qreal ll, mload->sumUpLoad ) sload.append( ll*lvec.y );
                    LOADDIR dir= ( isMomentLoad ) ? LDIR_MY : LDIR_Y ;
                    appendMemberLoad(md->getUuid(),mload->loadName,mload->loadType,dir,sload);
                }
                if( qAbs(lvec.z)>1.0e-5 ){
                    QList<qreal> sload;
                    Q_FOREACH( qreal ll, mload->sumUpLoad ) sload.append( ll*lvec.z );
                    LOADDIR dir= ( isMomentLoad ) ? LDIR_MZ : LDIR_Z ;
                    appendMemberLoad(md->getUuid(),mload->loadName,mload->loadType,dir,sload);
                }
            }else if( !mload->concentratedLoad.isEmpty() ){
                if( qAbs(lvec.x)>1.0e-5 ){
                    QList<QPointF> pload;
                    Q_FOREACH( QPointF pp, mload->concentratedLoad )
                        pload.append( QPointF(pp.x(),pp.y()*lvec.x) );
                    LOADDIR dir= ( isMomentLoad ) ? LDIR_MX : LDIR_X ;
                    appendMemberLoad(md->getUuid(),mload->loadName,mload->loadType,dir,pload);
                }
                if( qAbs(lvec.y)>1.0e-5 ){
                    QList<QPointF> pload;
                    Q_FOREACH( QPointF pp, mload->concentratedLoad )
                        pload.append( QPointF(pp.x(),pp.y()*lvec.y) );
                    LOADDIR dir= ( isMomentLoad ) ? LDIR_MY : LDIR_Y ;
                    appendMemberLoad(md->getUuid(),mload->loadName,mload->loadType,dir,pload);
                }
                if( qAbs(lvec.z)>1.0e-5 ){
                    QList<QPointF> pload;
                    Q_FOREACH( QPointF pp, mload->concentratedLoad )
                        pload.append( QPointF(pp.x(),pp.y()*lvec.z) );
                    LOADDIR dir= ( isMomentLoad ) ? LDIR_MZ : LDIR_Z ;
                    appendMemberLoad(md->getUuid(),mload->loadName,mload->loadType,dir,pload);
                }
            }else if( qAbs(wa.y())>1.0e-3 || qAbs(wb.y())>1.0e-3 ){
                if( qAbs(lvec.x)>1.0e-5 ){
                    WABLOAD wab( QPointF(wa.x(),wa.y()*lvec.x), QPointF(wb.x(),wb.y()*lvec.x) );
                    LOADDIR dir= ( isMomentLoad ) ? LDIR_MX : LDIR_X ;
                    appendMemberLoad(md->getUuid(),mload->loadName,mload->loadType,dir,wab);
                }
                if( qAbs(lvec.y)>1.0e-5 ){
                    WABLOAD wab( QPointF(wa.x(),wa.y()*lvec.y), QPointF(wb.x(),wb.y()*lvec.y) );
                    LOADDIR dir= ( isMomentLoad ) ? LDIR_MY : LDIR_Y ;
                    appendMemberLoad(md->getUuid(),mload->loadName,mload->loadType,dir,wab);
                }
                if( qAbs(lvec.z)>1.0e-5 ){
                    WABLOAD wab( QPointF(wa.x(),wa.y()*lvec.z), QPointF(wb.x(),wb.y()*lvec.z) );
                    LOADDIR dir= ( isMomentLoad ) ? LDIR_MZ : LDIR_Z ;
                    appendMemberLoad(md->getUuid(),mload->loadName,mload->loadType,dir,wab);
                }
            }
        }
    }

    Q_FOREACH( JointData* jd, c_jloads.uniqueKeys() ){
        Q_FOREACH( JLOADSUM* jload, c_jloads.values(jd) ){
            JLOADSUM* jl=new JLOADSUM( jload->loadName, jload->loadType,
                                       jload->loadDir, jload->concentratedLoad );
            totalJointLoads.insert(jd->getUuid(),jl);
        }
    }

    emit outputLoadUpdated();
}
*/

/*
void UnifiedCalcLoad::appendMemberLoad( const QUuid & id, const QString & name, LOADTYPE ltype,
                                      LOADDIR ldir, const QList<qreal>& load )
{
    bool isDetected=false;
    Q_FOREACH( MLOADSUM* ml, totalMemberLoads.values(id) ){
        if( ml->loadType==ltype && ml->loadDir==ldir && ml->loadName==name ){
            QList<qreal> cload=ml->sumUpLoad;
            if( cload.isEmpty() ) continue;
            for( int i=0; i<cload.count() ; i++ ){
                qreal ll=cload.at(i)+load.at(i);
                cload.replace(i,ll);
            }
            ml->sumUpLoad=cload;
            isDetected=true;
            break;
        }
    }
    if( !isDetected ){
        MLOADSUM* ml=new MLOADSUM( name, ltype, ldir, load );
        totalMemberLoads.insert(id,ml);
    }
}

void UnifiedCalcLoad::appendMemberLoad( const QUuid & id, const QString & name, LOADTYPE ltype,
                                      LOADDIR ldir, const QList<QPointF>& load )
{
    bool isDetected=false;
    Q_FOREACH( MLOADSUM* ml, totalMemberLoads.values(id) ){
        if( ml->loadType==ltype && ml->loadDir==ldir && ml->loadName==name ){
            QList<QPointF> cload=ml->concentratedLoad;
            if( cload.isEmpty() ) continue;
            for( int i=0; i<load.count(); i++ ){
                qreal xx=load.at(i).x();
                qreal yy=load.at(i).y();
                bool isOverap=false;
                for( int j=0; j<cload.count(); j++ ){
                    if( qAbs(xx-cload.at(j).x())<1.0e-2 ){
                        qreal cy=yy+cload.at(j).y();
                        cload.replace(j,QPointF(cload.at(j).x(),cy));
                        isOverap=true;
                        break;
                    }
                }
                if( !isOverap ) cload.append( load.at(i) );
            }
            ml->concentratedLoad=cload;
            isDetected=true;
            break;
        }
    }

    if( !isDetected ){
        MLOADSUM* ml=new MLOADSUM( name, ltype, ldir, QList<qreal>(), load );
        totalMemberLoads.insert(id,ml);
    }
}

void UnifiedCalcLoad::appendMemberLoad( const QUuid & id, const QString & name, LOADTYPE ltype,
                                      LOADDIR ldir, const WABLOAD & wab )
{
    bool isDetected=false;
    Q_FOREACH( MLOADSUM* ml, totalMemberLoads.values(id) ){
        if( ml->loadType==ltype && ml->loadDir==ldir && ml->loadName==name ){
            QPointF wa=ml->loadWAB.loadWA;
            QPointF wb=ml->loadWAB.loadWB;
            if( qAbs(wa.y())<1.0e-3 && qAbs(wb.y())<1.0e-3 ) continue;
            if( qAbs(wa.x()-wab.loadWA.x())<1.0e-3 && qAbs(wb.x()-wab.loadWB.x())<1.0e-3 ){
                wa.setY( wa.y()+wab.loadWA.y() );
                wb.setY( wb.y()+wab.loadWB.y() );
                ml->loadWAB=WABLOAD(wa,wb);
                isDetected=true;
                break;
            }
        }
    }

    if( !isDetected ){
        MLOADSUM* ml=new MLOADSUM( name, ltype, ldir, QList<qreal>(), QList<QPointF>(), wab );
        totalMemberLoads.insert(id,ml);
    }
}
*/
} // namespace post3dapp
