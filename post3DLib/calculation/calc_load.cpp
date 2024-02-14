#include "calc_load.h"

#include <QProgressDialog>

#include "calc_vector2d.h"
#include "calc_vector3d.h"
#include "define_section_struct.h"
#include "define_output.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_inputdata.h"

namespace post3dapp{
CalculationLoad::CalculationLoad(QObject *parent) : QObject(parent)
{
}

void CalculationLoad::deleteCashData()
{
    /*
    Q_FOREACH ( MemberData *md, globalMemberLoads.uniqueKeys() ) {
        Q_FOREACH ( MLOADSUM *ml, globalMemberLoads.values(md) ) delete ml;
    }
    globalMemberLoads.clear();
    */
    Q_FOREACH ( JointData *jd, globalJointLoads.uniqueKeys() ) {
        Q_FOREACH ( JLOADSUM *jl, globalJointLoads.values(jd) ) delete jl;
    }
    globalJointLoads.clear();
    Q_FOREACH ( MemberData *md, localMemberLoads.uniqueKeys() ) {
        Q_FOREACH ( MLOADSUM *ml, localMemberLoads.values(md) ) delete ml;
    }
    localMemberLoads.clear();
}

QString CalculationLoad::write(const QString &o_fpath)
{
    QFile o_file(o_fpath);
    if ( !o_file.open(QIODevice::WriteOnly) ) {
        deleteCashData();
        return u8"アウトプットをファイルに書き込めません。";
    }

    QTextStream outfile(&o_file);

    // 節点荷重のファイル保存
    Q_FOREACH ( JointData *jd, globalJointLoads.uniqueKeys() ) {
        if ( jd == nullptr ) continue;
        Q_FOREACH ( JLOADSUM *jload, globalJointLoads.values(jd) ) {
            if (qAbs(jload->concentratedLoad) < 1.0e-3)
                continue;

            outfile << QString("$J,");
            QList<QUuid> jointIdList = UnifiedFixedData::getInstance()->getInputData()->getIDList(DATATYPE::TPJOINT);
            outfile << QString::number(jointIdList.indexOf(jd->getUuid()))<< ",";
            outfile << jd->getUuid().toString() + ",";
            outfile << LoadType::enumToString(jload->loadType) + ",";
            if ( jload->loadName.isEmpty() ) {
                outfile << "(auto),";
            } else {
                outfile << jload->loadName + ",";
            }
            switch ( jload->loadDir ) {
            case LOADDIR::LDIR_X:
                outfile << QString("X,%1").arg(jload->concentratedLoad, 0, 'f', 5);
                break;
            case LOADDIR::LDIR_Y:
                outfile << QString("Y,%1").arg(jload->concentratedLoad, 0, 'f', 5);
                break;
            case LOADDIR::LDIR_Z:
                outfile << QString("Z,%1").arg(jload->concentratedLoad, 0, 'f', 5);
                break;
            case LOADDIR::LDIR_MX:
                outfile << QString("MX,%1").arg(jload->concentratedLoad, 0, 'f', 5);
                break;
            case LOADDIR::LDIR_MY:
                outfile << QString("MY,%1").arg(jload->concentratedLoad, 0, 'f', 5);
                break;
            case LOADDIR::LDIR_MZ:
                outfile << QString("MZ,%1").arg(jload->concentratedLoad, 0, 'f', 5);
                break;
            default:
                break;
            }
            outfile << "\n";
        }
    }

    // 部材荷重のファイル保存
    Q_FOREACH ( MemberData *md, localMemberLoads.uniqueKeys() ) {
        if ( md == NULL ) continue;
        Q_FOREACH ( MLOADSUM *mload, localMemberLoads.values(md) ) {

            outfile << QString("$M,");
            QList<QUuid> memberIdList = UnifiedFixedData::getInstance()->getInputData()->getIDList(DATATYPE::TPCOLUMN);
            outfile << QString::number(md->getMemberNo()) << ",";
            outfile << md->getUuid().toString() + ",";
            outfile << LoadType::enumToString(mload->loadType) + ",";
            if ( mload->loadName.isEmpty() ) {
                outfile << "(auto),";
            } else {
                outfile << mload->loadName + ",";
            }

            bool isMoment = false;
            switch ( mload->loadDir ) {
            case LOADDIR::LDIR_X:
                outfile << QString("x,");
                break;
            case LOADDIR::LDIR_Y:
                outfile << QString("y,");
                break;
            case LOADDIR::LDIR_Z:
                outfile << QString("z,");
                break;
            case LOADDIR::LDIR_MX:
                outfile << QString("mx,");
                isMoment = true;
                break;
            case LOADDIR::LDIR_MY:
                outfile << QString("my,");
                isMoment = true;
                break;
            case LOADDIR::LDIR_MZ:
                outfile << QString("mz,");
                isMoment = true;
                break;
            default:
                break;
            }

            if ( !mload->sumUpLoad.isEmpty() ) {
                QString str = "S,";
                Q_FOREACH ( qreal vv, mload->sumUpLoad ) {
                    str += QString("%1,").arg(vv, 0, 'f', 5);
                }
                str.remove(str.count() - 1, 1);
                str += ",";
                str += "\n";
                outfile << str;
            }
            if ( !mload->concentratedLoad.isEmpty() ) {
                QString str = "C,";
                Q_FOREACH ( QPointF pp, mload->concentratedLoad ) {
                    str += QString("%1,%2,").arg(pp.x(), 0, 'f', 5).arg(pp.y(), 0, 'f', 5);
                }
                str += "\n";
                outfile << str;
            }
            if (  qAbs(mload->loadWAB.loadWA.y()) > 1.0e-3
                    || qAbs(mload->loadWAB.loadWB.y()) > 1.0e-3 ) {
                QString str = "D,";
                str += QString("%1,%2,").arg(mload->loadWAB.loadWA.x(), 0, 'f', 5)
                       .arg(mload->loadWAB.loadWA.y(), 0, 'f', 5);

                str += QString("%1,%2,").arg(mload->loadWAB.loadWB.x(), 0, 'f', 5)
                       .arg(mload->loadWAB.loadWB.y(), 0, 'f', 5);
                str += "\n";
                outfile << str;
            }
        }
    }

    outfile << "\n";

    return QString{};
}

bool CalculationLoad::isLoadedJoint(JointData *jd) const
{
    return globalJointLoads.contains(jd);
}

bool CalculationLoad::isLoadeMember(MemberData *md) const
{
    return localMemberLoads.contains(md);
}

QList<JLOADSUM *> CalculationLoad::loadOfJoint(JointData* jd) const
{
    return globalJointLoads.values(jd);
}

QList<MLOADSUM *> CalculationLoad::loadOfMember(MemberData *md) const
{
    return localMemberLoads.values(md);
}

void CalculationLoad::read(const QString &o_fpath)
{
    qDebug()<<"read data init";
    deleteCashData();
    qDebug()<<"clear output data";

    if ( !QFile::exists(o_fpath) ) return;
    QFile o_file(o_fpath);
    if ( !o_file.open(QIODevice::ReadOnly) ) return;
    QTextStream in(&o_file);
    while ( !in.atEnd() ) {
        QString line = in.readLine();
        if ( line.isEmpty() ) break;
        if ( line.at(0) != '$' ) break;
        QStringList words = line.split(',', QString::SkipEmptyParts);

        if ( words.first() == "$J" ) {

            if ( words.count() < 7 ) break;
            QUuid uid = QUuid(words.at(2));
            LoadType::LOADTYPE ltype = LoadType::stringToEnum(words.at(3));
            QString lname = ( words.at(4) == "(auto)" ) ? QString() : words.at(4);
            LOADDIR ldir;
            if ( words.at(5) == "X" ) {
                ldir = LOADDIR::LDIR_X;
            } else if ( words.at(5) == "Y" ) {
                ldir = LOADDIR::LDIR_Y;
            } else if ( words.at(5) == "Z" ) {
                ldir = LOADDIR::LDIR_Z;
            } else if ( words.at(5) == "MX" ) {
                ldir = LOADDIR::LDIR_MX;
            } else if ( words.at(5) == "MY" ) {
                ldir = LOADDIR::LDIR_MY;
            } else if ( words.at(5) == "MZ" ) {
                ldir = LOADDIR::LDIR_MZ;
            }
            qreal dload = words.at(6).toDouble();
            JointData* jd = UnifiedFixedData::getInstance()->getInputData()->jointIDToPointer(uid);
            JLOADSUM *jl = new JLOADSUM( lname, ltype, ldir, dload );
            globalJointLoads.insert(jd, jl);

        } else if ( words.first() == "$M" ) {

            if ( words.count() < 9 ) break;
            QUuid uid = QUuid(words.at(2));
            LoadType::LOADTYPE ltype = LoadType::stringToEnum(words.at(3));
            QString lname = ( words.at(4) == "(auto)" ) ? QString() : words.at(4);
            LOADDIR ldir;
            if ( words.at(5) == "x" ) {
                ldir = LOADDIR::LDIR_X;
            } else if ( words.at(5) == "y" ) {
                ldir = LOADDIR::LDIR_Y;
            } else if ( words.at(5) == "z" ) {
                ldir = LOADDIR::LDIR_Z;
            } else if ( words.at(5) == "mx" ) {
                ldir = LOADDIR::LDIR_MX;
            } else if ( words.at(5) == "my" ) {
                ldir = LOADDIR::LDIR_MY;
            } else if ( words.at(5) == "mz" ) {
                ldir = LOADDIR::LDIR_MZ;
            }
            QList<qreal> sload;
            QList<QPointF> cload;
            WABLOAD dload;
            if ( words.at(6) == "S" ) {
                for ( int i = 7; i < words.count(); i++ ) sload.append( words.at(i).toDouble() );
            } else if ( words.at(6) == "C" ) {
                int icnt = (words.count() - 6) / 2;
                for ( int i = 0; i < icnt; i++ ) {
                    cload.append( QPointF(words.at(2 * i + 7).toDouble(), words.at(2 * i + 8).toDouble()) );
                }
            } else if ( words.at(6) == "D" ) {
                if ( words.count() < 11 ) break;
                QPointF wa( words.at(7).toDouble(), words.at(8).toDouble() );
                QPointF wb( words.at(9).toDouble(), words.at(10).toDouble() );
                dload = WABLOAD(wa, wb);
            }
            auto md = UnifiedFixedData::getInstance()->getInputData()->memberIDToPointer(uid);
            MLOADSUM *ml = new MLOADSUM( lname, ltype, ldir, sload, cload, dload );
            localMemberLoads.insert(md, ml);
        }
    }
}

QString CalculationLoad::executeCalculation(const QString& o_fpath, QProgressDialog *progressDialog)
{
    QMultiHash<MemberData *, MLOADSUM *> globalMemberLoads;
    Q_UNUSED(o_fpath);
    QString err_msg;

    UnifiedFixedData::getInstance()->getInputData()->recalcMemberCoordinate();

    if (progressDialog != nullptr){
        progressDialog->setValue(1);
    }

    // 小梁の優先度（親子）を計算
    QMultiHash<int, MemberData *> beam_priority; // 小梁の優先度
    QList<MemberData *> main_mdlist = UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPCOLUMN);
    main_mdlist.append( UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPGIRDER) );
    main_mdlist.append( UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPBRACE) ); // Damper,Isoを除く
    QList<MemberData *> all_beams = UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPBEAM);

    // 柱・大梁・ブレース自重加算 分布荷重 appendMemberLoad()
    Q_FOREACH ( MemberData *md, main_mdlist ) appendSelfWeight( md , globalMemberLoads);
    // 小梁自重加算 分布荷重 appendMemberLoad()
    Q_FOREACH ( MemberData *md, all_beams ) appendSelfWeight( md , globalMemberLoads);
    // 制振要素・免震要素　自重加算　解析部材の内分点であれば部材荷重へ　構成点であれば節点荷重
    Q_FOREACH ( MemberData *md, UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPDAMPER) )
        appendSelfWeight( md , globalMemberLoads);
    Q_FOREACH ( MemberData *md, UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPISO) )
        appendSelfWeight( md , globalMemberLoads);

    // 解析部材を親とする小梁の検出　優先度0
    QList<MemberData *> decided_beams;
    int ipr = 0;
    Q_FOREACH ( MemberData *md, main_mdlist ) {

        Q_FOREACH ( JointData *jd, md->getJointList() ) {
            Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                if ( mm->dataType() == DATATYPE::TPBEAM ) {
                    JointData *tj = ( mm->isIJoint(jd) ) ? mm->jJoint() : mm->iJoint();
                    bool tok = false;
                    Q_FOREACH ( MemberData *gm, tj->getRelatedMembers() ) {
                        if ( gm->isAnalysisLineMember() ) {
                            tok = true;
                            break;
                        }
                    }
                    if ( !tok ) { // 親の小梁がいないかどうか（片持ち小梁）
                        tok = true;
                        Q_FOREACH ( MemberData *gm, tj->getInteriorMembers() ) {
                            if ( gm->dataType() == DATATYPE::TPBEAM ) {
                                tok = false;
                                break;
                            }
                        }
                    }
                    if ( tok ) {
                        bool ok = all_beams.removeOne(mm);
                        if ( ok ) beam_priority.insert(ipr, mm);
                    }
                }
            }
        }
    }
    main_mdlist.clear();
    decided_beams.append( beam_priority.values(ipr) );

    // すでに親が見つかった小梁側から子の小梁を探索
    ipr++;
    while ( all_beams.count() > 0 ) {
        QList<MemberData *> pre_beams = beam_priority.values(ipr - 1);
        Q_FOREACH ( MemberData *md, pre_beams ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                Q_FOREACH ( MemberData *mm, jd->getComposeMembers() ) {
                    if ( mm->dataType() == DATATYPE::TPBEAM ) {
                        JointData *tj = ( mm->isIJoint(jd) ) ? mm->jJoint() : mm->iJoint();
                        bool tok = false;
                        Q_FOREACH ( MemberData *gm, tj->getRelatedMembers() ) {
                            if ( gm->isAnalysisLineMember() ) {
                                tok = true;
                                break;
                            }
                            if ( decided_beams.contains(gm) ) {
                                tok = true;
                                break;
                            }
                        }
                        if ( !tok ) { // 親の小梁がいないかどうか（片持ち小梁）
                            tok = true;
                            Q_FOREACH ( MemberData *gm, tj->getInteriorMembers() ) {
                                if ( gm->dataType() == DATATYPE::TPBEAM ) {
                                    tok = false;
                                    break;
                                }
                            }
                        }
                        if ( tok ) {
                            bool ok = all_beams.removeOne(mm);
                            if ( ok ) beam_priority.insert(ipr, mm);
                        }
                    }
                }
            }
        }
        if ( !beam_priority.contains(ipr) ) break;
        decided_beams.append( beam_priority.values(ipr) );
        ipr++;
    }
    decided_beams.clear();

    if ( !all_beams.isEmpty() ) {
        err_msg += u8"小梁の荷重伝達が不可能。\n";
        err_msg += memberInfo(all_beams.first());
        deleteCashData();
        if (progressDialog != nullptr){
            progressDialog->setValue( 100 );
        }
        return err_msg;
    }

    if (progressDialog != nullptr){
        progressDialog->setValue(5);
    }
    /*
        qDebug() << "BEAM 0" << beam_priority.values(0).count();
        qDebug() << "BEAM 1" << beam_priority.values(1).count();
        qDebug() << "BEAM 2" << beam_priority.values(2).count();
        qDebug() << "BEAM 3" << beam_priority.values(3).count();
        qDebug() << "BEAM 4" << beam_priority.values(4).count();
        qDebug() << "BEAM 5" << beam_priority.values(5).count();
    */

    // 入力節点荷重のうち、二次部材にかかるものは集計荷重へ、解析節点にかかるものは個別荷重へ
    QList<MemberData *> jl_list = UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPJOINTLOAD);
    Q_FOREACH ( MemberData *jl, jl_list ) {
        JointData *jj = jl->iJoint();
        if ( jl->sectionName().isEmpty() ) {
            err_msg += u8"部材荷重の符号が未定義。\n";
            err_msg += jointInfo(jj);
            deleteCashData();
            if (progressDialog != nullptr){
                progressDialog->setValue( 100 );
            }
            return err_msg;
        }

        JOINTLOADVALUES jlv = UnifiedFixedData::getInstance()->getInputData()->getJointLoadValue(jl->sectionName());
        if ( jj->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData()) ) {
            Q_FOREACH ( LoadType::LOADTYPE ltp, jlv.loadTable.uniqueKeys() ) {
                QList<JOINTLOADDIM> jdimlist = jlv.loadTable.values(ltp);
                Q_FOREACH ( JOINTLOADDIM jdim, jdimlist ) {
                    if ( qAbs(jdim.fx) > 1.0e-3 )
                        appendJointLoad(jj, jl->sectionName(), ltp, LOADDIR::LDIR_X, jdim.fx );
                    if ( qAbs(jdim.fy) > 1.0e-3 )
                        appendJointLoad(jj, jl->sectionName(), ltp, LOADDIR::LDIR_Y, jdim.fy );
                    if ( qAbs(jdim.fz) > 1.0e-3 )
                        appendJointLoad(jj, jl->sectionName(), ltp, LOADDIR::LDIR_Z, jdim.fz );
                    if ( qAbs(jdim.mx) > 1.0e-3 )
                        appendJointLoad(jj, jl->sectionName(), ltp, LOADDIR::LDIR_MX, jdim.mx );
                    if ( qAbs(jdim.my) > 1.0e-3 )
                        appendJointLoad(jj, jl->sectionName(), ltp, LOADDIR::LDIR_MY, jdim.my );
                    if ( qAbs(jdim.mz) > 1.0e-3 )
                        appendJointLoad(jj, jl->sectionName(), ltp, LOADDIR::LDIR_MZ, jdim.mz );
                }
            }
        } else {
            QList<MemberData *> mmlist;
            Q_FOREACH ( MemberData *mm, jj->getRelatedMembers() ) {
                if ( mm->isAnalysisLineMember() ) {
                    mmlist = QList<MemberData *>() << mm ;
                    break;
                }
            }
            if ( mmlist.isEmpty() ) {
                // 小梁の内分点に対する荷重
                Q_FOREACH ( MemberData *mm, jj->getRelatedMembers() ) {
                    if ( mm->dataType() == DATATYPE::TPBEAM && mm->isInteriorJoint(jj) ) mmlist.append(mm);
                }
                // 小梁の構成点に対する荷重
                if ( mmlist.isEmpty() ) {
                    Q_FOREACH ( MemberData *mm, jj->getRelatedMembers() ) {
                        if ( mm->dataType() == DATATYPE::TPBEAM ) mmlist.append(mm);
                    }
                }
            }
            if ( mmlist.isEmpty() ) {
                err_msg += u8"伝達不可能な節点荷重。\n";
                err_msg += u8"符号:" + jl->sectionName() + ",";
                err_msg += jointInfo(jj);
                deleteCashData();
                if (progressDialog != nullptr){
                    progressDialog->setValue( 100 );
                }
                return err_msg;
            }
            qreal mcount = qreal(mmlist.count());
            QString l_name = ( mmlist.first()->dataType() == DATATYPE::TPBEAM ) ? QString() : jl->sectionName();
            Q_FOREACH ( MemberData *mm, mmlist ) {
                qreal ll = mm->perpLengthOfMember(XYZ(jj->xPos(), jj->yPos(), jj->zPos()));
                Q_FOREACH ( LoadType::LOADTYPE ltp, jlv.loadTable.uniqueKeys() ) {
                    QList<JOINTLOADDIM> jdimlist = jlv.loadTable.values(ltp);
                    Q_FOREACH ( JOINTLOADDIM jdim, jdimlist ) {
                        if ( qAbs(jdim.fx) > 1.0e-3 )
                            appendMemberLoad(mm, l_name, ltp, LOADDIR::LDIR_X, QPointF(ll, jdim.fx / mcount), globalMemberLoads);
                        if ( qAbs(jdim.fy) > 1.0e-3 )
                            appendMemberLoad(mm, l_name, ltp, LOADDIR::LDIR_Y, QPointF(ll, jdim.fy / mcount), globalMemberLoads);
                        if ( qAbs(jdim.fz) > 1.0e-3 )
                            appendMemberLoad(mm, l_name, ltp, LOADDIR::LDIR_Z, QPointF(ll, jdim.fz / mcount), globalMemberLoads);
                        if ( qAbs(jdim.mx) > 1.0e-3 )
                            appendMemberLoad(mm, l_name, ltp, LOADDIR::LDIR_MX, QPointF(ll, jdim.mx / mcount), globalMemberLoads);
                        if ( qAbs(jdim.my) > 1.0e-3 )
                            appendMemberLoad(mm, l_name, ltp, LOADDIR::LDIR_MY, QPointF(ll, jdim.my / mcount), globalMemberLoads);
                        if ( qAbs(jdim.mz) > 1.0e-3 )
                            appendMemberLoad(mm, l_name, ltp, LOADDIR::LDIR_MZ, QPointF(ll, jdim.mz / mcount), globalMemberLoads);
                    }
                }
            }
        }
    }

    // 入力部材荷重のうち、二次部材にかかるものは集計荷重へ
    QList<MemberData *> ml_list = UnifiedFixedData::getInstance()->getInputData()->memberListOfType(DATATYPE::TPMEMBERLOAD);
    Q_FOREACH ( MemberData *ml, ml_list ) {
        QString mload_name = ml->sectionName();
        if ( mload_name.isEmpty() ) {
            err_msg += u8"部材荷重の符号が未定義。\n";
            err_msg += memberInfo(ml);
            deleteCashData();
            if (progressDialog != nullptr){
                progressDialog->setValue( 100 );
            }
            return err_msg;
        }

        qreal tlen = ml->getTotalLength() / 1000.0;
        MEMBERLOADVALUES mlv = UnifiedFixedData::getInstance()->getInputData()->getMemberLoadValue(mload_name);
        QList<JointData *> ml_jdlist = ml->getJointList();

        QList<MemberData *> on_mdlist;
        QList<int> idxlist;
        for ( int i = 0; i < ml_jdlist.count() - 1; i++ ) {
            JointData *j1 = ml_jdlist.at(i);
            JointData *j2 = ml_jdlist.at(i + 1);
            QList<MemberData *> s_mdlist = sharedLineMembers(j1, j2);
            if ( s_mdlist.count() != 1 ) {
                err_msg += u8"部材荷重の入力位置が不適切。\n";
                err_msg += memberInfo(ml);
                deleteCashData();
                if (progressDialog != nullptr){
                    progressDialog->setValue( 100 );
                }
                return err_msg;
            }
            if ( i == 0 ) {
                on_mdlist.append(s_mdlist.first());
                idxlist.append(i + 1);
            } else {
                if ( on_mdlist.last() == s_mdlist.first() ) {
                    idxlist.replace( idxlist.count() - 1, i + 1 );
                } else {
                    on_mdlist.append(s_mdlist.first());
                    idxlist.append(i + 1);
                }
            }
        }

        // 部材荷重が伝達する部材ごとに、部材荷重を分割して振り分ける
        for ( int i = 0; i < on_mdlist.count(); i++ ) {

            MemberData *l_md = on_mdlist.at(i);

            int st_id = ( i == 0 ) ? 0 : idxlist.at(i - 1) ;
            int en_id = idxlist.at(i);
            qreal ilen = ml->getJointLength(st_id) / 1000.0;
            qreal jlen = ml->getJointLength(en_id) / 1000.0;
            JointData *j1 = ml_jdlist.at(st_id);
            JointData *j2 = ml_jdlist.at(en_id);
            qreal x1 = l_md->getJointLength(j1) / 1000.0;
            qreal x2 = l_md->getJointLength(j2) / 1000.0;

            QString l_name = ( l_md->dataType() == DATATYPE::TPBEAM ) ? QString() : mload_name ;

            Q_FOREACH ( LoadType::LOADTYPE ltp, mlv.loadTable.uniqueKeys() ) {
                QList<MEMBERLOADDIM> mdimlist = mlv.loadTable.values(ltp);
                Q_FOREACH ( MEMBERLOADDIM mdim, mdimlist ) {
                    qreal wa = mdim.wa + (mdim.wb - mdim.wa) * ilen / tlen;
                    qreal wb = mdim.wa + (mdim.wb - mdim.wa) * jlen / tlen;
                    if ( mdim.corSytem == LCORSYSTEM::LCOR_GLOBAL ) {
                        WABLOAD wab = ( x1 < x2 ) ? WABLOAD(QPointF(x1, wa), QPointF(x2, wb))
                                      : WABLOAD(QPointF(x2, wb), QPointF(x1, wa));
                        appendMemberLoad(l_md, l_name, ltp, mdim.loadDir, wab, globalMemberLoads);
                    } else {
                        // 部材座標系の分布荷重を全体座標系の分布荷重に
                        // 部材座標系ベクトルはi端を基準としているため、部材が折れ点で構成される場合注意
                        VEC3D lvec;
                        if ( mdim.loadDir == LOADDIR::LDIR_X ) {
                            lvec = VEC3D(1.0, 0.0, 0.0);
                        } else if ( mdim.loadDir == LOADDIR::LDIR_Y ) {
                            lvec = VEC3D(0.0, 1.0, 0.0);
                        } else {
                            lvec = VEC3D(0.0, 0.0, 1.0);
                        }
                        VEC3D u_v = l_md->getUVector();
                        VEC3D v_v = l_md->getVVector();
                        VEC3D w_v = l_md->getWVector();
                        VEC3D gvec = CalcVector3D::localToGlobal(u_v, v_v, w_v, VEC3D(), lvec);
                        if ( qAbs(gvec.x) > 1.0e-5 ) {
                            WABLOAD wab = ( x1 < x2 ) ? WABLOAD(QPointF(x1, wa * gvec.x), QPointF(x2, wb * gvec.x))
                                          : WABLOAD(QPointF(x2, wb * gvec.x), QPointF(x1, wa * gvec.x));
                            appendMemberLoad(l_md, l_name, ltp, LOADDIR::LDIR_X, wab, globalMemberLoads);
                        }
                        if ( qAbs(gvec.y) > 1.0e-5 ) {
                            WABLOAD wab = ( x1 < x2 ) ? WABLOAD(QPointF(x1, wa * gvec.y), QPointF(x2, wb * gvec.y))
                                          : WABLOAD(QPointF(x2, wb * gvec.y), QPointF(x1, wa * gvec.y));
                            appendMemberLoad(l_md, l_name, ltp, LOADDIR::LDIR_Y, wab, globalMemberLoads);
                        }
                        if ( qAbs(gvec.z) > 1.0e-5 ) {
                            WABLOAD wab = ( x1 < x2 ) ? WABLOAD(QPointF(x1, wa * gvec.z), QPointF(x2, wb * gvec.z))
                                          : WABLOAD(QPointF(x2, wb * gvec.z), QPointF(x1, wa * gvec.z));
                            appendMemberLoad(l_md, l_name, ltp, LOADDIR::LDIR_Z, wab, globalMemberLoads);
                        }
                    }
                }
            }
        }
    }

    int t_count = 0;
    int c_count = 0;
    t_count += UnifiedFixedData::getInstance()->getInputData()->getDataCount(DATATYPE::TPSLAB);
    t_count += UnifiedFixedData::getInstance()->getInputData()->getDataCount(DATATYPE::TPWALL);
    t_count += UnifiedFixedData::getInstance()->getInputData()->getDataCount(DATATYPE::TPUWALL);
    t_count += UnifiedFixedData::getInstance()->getInputData()->getDataCount(DATATYPE::TPSLABLOAD);

    // スラブの荷重拾い
    err_msg = calcForMemberArea(DATATYPE::TPSLAB, c_count, t_count, globalMemberLoads, progressDialog);
    if ( !err_msg.isEmpty() ) {
        deleteCashData();
        if (progressDialog != nullptr){
            progressDialog->setValue( 100 );
        }
        return err_msg;
    }

    // 壁の荷重拾い
    err_msg = calcForMemberArea(DATATYPE::TPWALL, c_count, t_count, globalMemberLoads, progressDialog);
    if ( !err_msg.isEmpty() ) {
        deleteCashData();
        if (progressDialog != nullptr){
            progressDialog->setValue( 100 );
        }
        return err_msg;
    }

    // 地下外壁の荷重拾い
    err_msg = calcForMemberArea(DATATYPE::TPUWALL, c_count, t_count, globalMemberLoads, progressDialog);
    if ( !err_msg.isEmpty() ) {
        deleteCashData();
        if (progressDialog != nullptr){
            progressDialog->setValue( 100 );
        }
        return err_msg;
    }

    // 床荷重の荷重拾い
    err_msg = calcForMemberArea(DATATYPE::TPSLABLOAD, c_count, t_count, globalMemberLoads, progressDialog);
    if ( !err_msg.isEmpty() ) {
        deleteCashData();
        if (progressDialog != nullptr){
            progressDialog->setValue( 100 );
        }
        return err_msg;
    }


    // 小梁の荷重伝達
    BOUNDARY ztype;
    QList<int> i_priority = beam_priority.uniqueKeys();
    qSort(i_priority.begin(), i_priority.end());
    QList<MemberData *> done_beams;

    for ( int i = i_priority.count() - 1; i >= 0; i-- ) {

        if ( progressDialog != nullptr && progressDialog->wasCanceled() ) {
            deleteCashData();
            progressDialog->setValue( 100 );
            return u8"キャンセルされました。";
        }

        int ikey = i_priority.at(i);
        QList<MemberData *> cur_beams = beam_priority.values(ikey);
        done_beams.append(cur_beams);

        Q_FOREACH ( MemberData *mm, cur_beams ) {

            JointData *ijd = mm->iJoint();
            JointData *jjd = mm->jJoint();

            bool IisAJ = ( ijd->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData()) );
            bool JisAJ = ( jjd->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData()) );
            QList<MemberData *> i_parents;
            QList<MemberData *> j_parents;
            if ( !IisAJ ) {
                Q_FOREACH ( MemberData *gm, ijd->getRelatedMembers() ) {
                    if ( gm->isLineMember(false) && !done_beams.contains(gm) ) {
                        i_parents.append(gm);
                    }
                }
            }
            if ( !JisAJ ) {
                Q_FOREACH ( MemberData *gm, jjd->getRelatedMembers() ) {
                    if ( gm->isLineMember(false) && !done_beams.contains(gm) ) {
                        j_parents.append(gm);
                    }
                }
            }
            if ( !IisAJ && i_parents.isEmpty() ) {
                ztype = BOUNDARY::FREE_FIX;
            } else if ( !JisAJ && j_parents.isEmpty() ) {
                ztype = BOUNDARY::FIX_FREE;
            } else {
                ztype = BOUNDARY::PIN_PIN;
            }

//            qDebug() << "IisAj  JisAj" <<  IisAJ << JisAJ << i_parents.count() << j_parents.count();
//            qDebug() << mm->sectionName() << FactoryUnifiedData::getInstance(isOutput)->indexOfJoint(ijd)+1  << FactoryUnifiedData::getInstance(isOutput)->indexOfJoint(jjd)+1 ;

            // mmに自重追加　appendMemberLoad(mm,...);

            Q_FOREACH ( MLOADSUM *mload, globalMemberLoads.values(mm) ) {

                VEC3D gvec;
                if ( mload->loadDir == LOADDIR::LDIR_X || mload->loadDir == LOADDIR::LDIR_MX ) {
                    gvec = VEC3D(1.0, 0.0, 0.0);
                } else if ( mload->loadDir == LOADDIR::LDIR_Y || mload->loadDir == LOADDIR::LDIR_MY ) {
                    gvec = VEC3D(0.0, 1.0, 0.0);
                } else if ( mload->loadDir == LOADDIR::LDIR_Z || mload->loadDir == LOADDIR::LDIR_MZ ) {
                    gvec = VEC3D(0.0, 0.0, 1.0);
                }
                bool isMomentLoad = ( mload->loadDir == LOADDIR::LDIR_MX || mload->loadDir == LOADDIR::LDIR_MY
                                      || mload->loadDir == LOADDIR::LDIR_MZ );
                VEC3D u_v = mm->getUVector();
                VEC3D v_v = mm->getVVector();
                VEC3D w_v = mm->getWVector();
                VEC3D lvec = CalcVector3D::globalToLocal(u_v, v_v, w_v, VEC3D(), gvec);
                if ( isMomentLoad && qAbs(lvec.x) > 1.0e-5 ) {
                    err_msg += u8"小梁内分点にねじれモーメントが作用。\n";
                    err_msg += memberInfo(mm);
                    deleteCashData();
                    if (progressDialog != nullptr){
                        progressDialog->setValue( 100 );
                    }
                    return err_msg;
                }

                VEC3D vi, vj; // 端部外力 部材座標系

                if ( isMomentLoad ) {
                    if ( ztype == BOUNDARY::PIN_PIN ) {
                        qreal tlen = mm->getTotalLength() / 1000.0;
                        qreal suml = 0.0;
                        Q_FOREACH ( QPointF pp, mload->concentratedLoad ) suml += pp.y() / tlen;
                        vi.y += -suml * lvec.z;
                        vj.y += suml * lvec.z;
                        vi.z += suml * lvec.y;
                        vj.z += -suml * lvec.y;
                    } // 片持ちの場合 モーメント荷重による反力0 エラーにはしない
                }

                if ( !mload->sumUpLoad.isEmpty() ) {
                    if ( ztype == BOUNDARY::PIN_PIN ) {
                        if ( qAbs(lvec.x) > 1.0e-5 ) {
                            qreal suml = 0.0;
                            Q_FOREACH ( qreal ll, mload->sumUpLoad ) suml += ll;
                            vi.x += suml * lvec.x / 2.0;
                            vj.x += -suml * lvec.x / 2.0;
                        }
                        if ( qAbs(lvec.y) > 1.0e-5 || qAbs(lvec.z) > 1.0e-5 ) {
                            qreal tlen = mm->getTotalLength() / 1000.0;
                            QList<qreal> s_lenlist = mm->getSeparatedLength();
                            QList<qreal> s_loadlist = mload->sumUpLoad;
                            qreal sumli = 0.0, sumlj = 0.0;
                            for ( int j = 0; j < s_lenlist.count(); j++ ) {
                                sumli += s_loadlist.at(j) * (tlen - s_lenlist.at(j)) / tlen;
                                sumlj += s_loadlist.at(j) * s_lenlist.at(j) / tlen;
                            }
                            vi.y += sumli * lvec.y;
                            vj.y += sumlj * lvec.y;
                            vi.z += sumli * lvec.z;
                            vj.z += sumlj * lvec.z;
                        }
                    } else {
                        qreal suml = 0.0;
                        Q_FOREACH ( qreal ll, mload->sumUpLoad ) suml += ll;
                        if ( ztype == BOUNDARY::FIX_FREE ) {
                            vi.x += suml * lvec.x;
                            vi.y += suml * lvec.y;
                            vi.z += suml * lvec.z;
                        } else {
                            vj.x += suml * lvec.x;
                            vj.y += suml * lvec.y;
                            vj.z += suml * lvec.z;
                        }
                    }
                }

                if ( !isMomentLoad && !mload->concentratedLoad.isEmpty() ) {
                    if ( ztype == BOUNDARY::PIN_PIN ) {
                        if ( qAbs(lvec.x) > 1.0e-5 ) {
                            qreal suml = 0.0;
                            Q_FOREACH ( QPointF pp, mload->concentratedLoad ) suml += pp.y();
                            vi.x += suml * lvec.x / 2.0;
                            vj.x += -suml * lvec.x / 2.0;
                        }
                        if ( qAbs(lvec.y) > 1.0e-5 || qAbs(lvec.z) > 1.0e-5 ) {
                            qreal tlen = mm->getTotalLength() / 1000.0;
                            qreal sumli = 0.0, sumlj = 0.0;
                            Q_FOREACH ( QPointF pp, mload->concentratedLoad ) {
                                sumli += pp.y() * (tlen - pp.x()) / tlen;
                                sumlj += pp.y() * pp.x() / tlen;
                            }
                            vi.y += sumli * lvec.y;
                            vj.y += sumlj * lvec.y;
                            vi.z += sumli * lvec.z;
                            vj.z += sumlj * lvec.z;
                        }
                    } else {
                        qreal suml = 0.0;
                        Q_FOREACH ( QPointF pp, mload->concentratedLoad ) suml += pp.y();
                        if ( ztype == BOUNDARY::FIX_FREE ) {
                            vi.x += suml * lvec.x;
                            vi.y += suml * lvec.y;
                            vi.z += suml * lvec.z;
                        } else {
                            vj.x += suml * lvec.x;
                            vj.y += suml * lvec.y;
                            vj.z += suml * lvec.z;
                        }
                    }
                }

                QPointF wa = mload->loadWAB.loadWA;
                QPointF wb = mload->loadWAB.loadWB;
                if ( qAbs(wa.y()) > 1.0e-5 || qAbs(wb.y()) > 1.0e-5 ) {
                    if ( ztype == BOUNDARY::PIN_PIN ) {
                        if ( qAbs(lvec.x) > 1.0e-5 ) {
                            qreal suml = (wa.y() + wb.y()) * (wb.x() - wa.x()) / 2.0;
                            vi.x += suml * lvec.x / 2.0;
                            vj.x += -suml * lvec.x / 2.0;
                        }
                        if ( qAbs(lvec.y) > 1.0e-5 || qAbs(lvec.z) > 1.0e-5 ) {
                            qreal tlen = mm->getTotalLength() / 1000.0;
                            qreal aa = wa.x();
                            qreal bb = wb.x() - wa.x();
                            qreal cc = tlen - wb.x();
                            qreal sumli = bb * (2.0 * bb + 3.0 * cc) * wa.y() / 6.0 / tlen
                                          + bb * (bb + 3.0 * cc) * wb.y() / 6.0 / tlen;
                            qreal sumlj = bb * (3.0 * aa + bb) * wa.y() / 6.0 / tlen
                                          + bb * (3.0 * aa + 2.0 * bb) * wb.y() / 6.0 / tlen;
                            vi.y += sumli * lvec.y;
                            vj.y += sumlj * lvec.y;
                            vi.z += sumli * lvec.z;
                            vj.z += sumlj * lvec.z;
                        }
                    } else {
                        qreal suml = (wa.y() + wb.y()) * (wb.x() - wa.x()) / 2.0;
                        if ( ztype == BOUNDARY::FIX_FREE ) {
                            vi.x += suml * lvec.x;
                            vi.y += suml * lvec.y;
                            vi.z += suml * lvec.z;
                        } else {
                            vj.x += suml * lvec.x;
                            vj.y += suml * lvec.y;
                            vj.z += suml * lvec.z;
                        }
                    }
                }

                VEC3D ri = CalcVector3D::localToGlobal(u_v, v_v, w_v, VEC3D(), vi);
                VEC3D rj = CalcVector3D::localToGlobal(u_v, v_v, w_v, VEC3D(), vj);
//                qDebug() << "ri" << ri.x << ri.y << ri.z;
//                qDebug() << "rj" << rj.x << rj.y << rj.z;

                if ( IisAJ ) {
                    if ( qAbs(ri.x) > 1.0e-3 )
                        appendJointLoad(ijd, QString(), mload->loadType, LOADDIR::LDIR_X, ri.x );
                    if ( qAbs(ri.y) > 1.0e-3 )
                        appendJointLoad(ijd, QString(), mload->loadType, LOADDIR::LDIR_Y, ri.y );
                    if ( qAbs(ri.z) > 1.0e-3 )
                        appendJointLoad(ijd, QString(), mload->loadType, LOADDIR::LDIR_Z, ri.z );
                } else if ( !i_parents.isEmpty() ) {
                    qreal icc = qreal(i_parents.count());
                    if ( qAbs(ri.x) > 1.0e-3 ) {
                        Q_FOREACH ( MemberData *gm, i_parents ) {
                            qreal ll = gm->getJointLength(ijd) / 1000.0;
                            appendMemberLoad(gm, QString(), mload->loadType, LOADDIR::LDIR_X, QPointF(ll, ri.x / icc), globalMemberLoads);
                        }
                    }
                    if ( qAbs(ri.y) > 1.0e-3 ) {
                        Q_FOREACH ( MemberData *gm, i_parents ) {
                            qreal ll = gm->getJointLength(ijd) / 1000.0;
                            appendMemberLoad(gm, QString(), mload->loadType, LOADDIR::LDIR_Y, QPointF(ll, ri.y / icc), globalMemberLoads);
                        }
                    }
                    if ( qAbs(ri.z) > 1.0e-3 ) {
                        Q_FOREACH ( MemberData *gm, i_parents ) {
                            qreal ll = gm->getJointLength(ijd) / 1000.0;
                            appendMemberLoad(gm, QString(), mload->loadType, LOADDIR::LDIR_Z, QPointF(ll, ri.z / icc), globalMemberLoads);
                        }
                    }
                }

                if ( JisAJ ) {
                    if ( qAbs(rj.x) > 1.0e-3 )
                        appendJointLoad(jjd, QString(), mload->loadType, LOADDIR::LDIR_X, rj.x );
                    if ( qAbs(rj.y) > 1.0e-3 )
                        appendJointLoad(jjd, QString(), mload->loadType, LOADDIR::LDIR_Y, rj.y );
                    if ( qAbs(rj.z) > 1.0e-3 )
                        appendJointLoad(jjd, QString(), mload->loadType, LOADDIR::LDIR_Z, rj.z );
                } else if ( !j_parents.isEmpty() ) {
                    qreal icc = qreal(j_parents.count());
                    if ( qAbs(rj.x) > 1.0e-3 ) {
                        Q_FOREACH ( MemberData *gm, j_parents ) {
                            qreal ll = gm->getJointLength(jjd) / 1000.0;
                            appendMemberLoad(gm, QString(), mload->loadType, LOADDIR::LDIR_X, QPointF(ll, rj.x / icc), globalMemberLoads);
                        }
                    }
                    if ( qAbs(rj.y) > 1.0e-3 ) {
                        Q_FOREACH ( MemberData *gm, j_parents ) {
                            qreal ll = gm->getJointLength(jjd) / 1000.0;
                            appendMemberLoad(gm, QString(), mload->loadType, LOADDIR::LDIR_Y, QPointF(ll, rj.y / icc), globalMemberLoads);
                        }
                    }
                    if ( qAbs(rj.z) > 1.0e-3 ) {
                        Q_FOREACH ( MemberData *gm, j_parents ) {
                            qreal ll = gm->getJointLength(jjd) / 1000.0;
                            appendMemberLoad(gm, QString(), mload->loadType, LOADDIR::LDIR_Z, QPointF(ll, rj.z / icc), globalMemberLoads);
                        }
                    }
                }
            }
            Q_FOREACH ( MLOADSUM *ml, globalMemberLoads.values(mm) ) delete ml;
            globalMemberLoads.remove(mm);
        }
    }

    if ( progressDialog != nullptr && progressDialog->wasCanceled() ) {
        deleteCashData();
        progressDialog->setValue( 100 );
        return u8"キャンセルされました。";
    }


    // debug
    /*
        Q_FOREACH( MemberData* md, currentMemberLoads.uniqueKeys() ){
            QList<MLOADSUM*> mlist=currentMemberLoads.values(md);
            Q_FOREACH( MLOADSUM* ml, mlist ){
                QString ltp;
                if( ml->loadType==LTYPE_DL ){ ltp="M DL"; }
                else if( ml->loadType==LTYPE_LL1 ){ ltp="M LL1"; }
                else{ ltp="M LLE"; }
                if( !ml->sumUpLoad.isEmpty() ){
                    qDebug() << ltp << md->sectionName() << ml->loadName << ml->loadDir << "sum" << ml->sumUpLoad;
                }else if( !ml->concentratedLoad.isEmpty() ){
                    qDebug() << ltp << md->sectionName() << ml->loadName << ml->loadDir << "con" << ml->concentratedLoad;
                }else{
                    qDebug() << ltp << md->sectionName() << ml->loadName << ml->loadDir << "dis" << ml->loadWAB.loadWA << ml->loadWAB.loadWB ;
                }
            }
        }

        Q_FOREACH( JointData* jd, currentJointLoads.uniqueKeys() ){
            QList<JLOADSUM*> jlist=currentJointLoads.values(jd);
            Q_FOREACH( JLOADSUM* jl, jlist ){
                QString ltp;
                if( jl->loadType==LTYPE_DL ){ ltp="J DL"; }
                else if( jl->loadType==LTYPE_LL1 ){ ltp="J LL1"; }
                else{ ltp="J LLE"; }
                qDebug() << ltp << FactoryUnifiedData::getInstance(isOutput)->indexOfJoint(jd)+1 << jl->loadName << jl->loadDir << jl->concentratedLoad ;
            }
        }
    */

    // UnifiedOutput::getInstance()->updateCalcLoadData(globalMemberLoads,globalJointLoads);
    // 部材荷重　全体座標系-globalMemberLoads　から 部材座標系-localMemberLoads　への変換
    Q_FOREACH ( MemberData *md, globalMemberLoads.uniqueKeys() ) {

        Q_FOREACH ( MLOADSUM *mload, globalMemberLoads.values(md) ) {

            VEC3D gvec;
            if ( mload->loadDir == LOADDIR::LDIR_X || mload->loadDir == LOADDIR::LDIR_MX ) {
                gvec = VEC3D(1.0, 0.0, 0.0);
            } else if ( mload->loadDir == LOADDIR::LDIR_Y || mload->loadDir == LOADDIR::LDIR_MY ) {
                gvec = VEC3D(0.0, 1.0, 0.0);
            } else if ( mload->loadDir == LOADDIR::LDIR_Z || mload->loadDir == LOADDIR::LDIR_MZ ) {
                gvec = VEC3D(0.0, 0.0, 1.0);
            }
            bool isMomentLoad = ( mload->loadDir == LOADDIR::LDIR_MX || mload->loadDir == LOADDIR::LDIR_MY
                                  || mload->loadDir == LOADDIR::LDIR_MZ );
            VEC3D u_v = md->getUVector();
            VEC3D v_v = md->getVVector();
            VEC3D w_v = md->getWVector();
            VEC3D lvec = CalcVector3D::globalToLocal(u_v, v_v, w_v, VEC3D(), gvec);

            QPointF wa = mload->loadWAB.loadWA;
            QPointF wb = mload->loadWAB.loadWB;
            if ( !mload->sumUpLoad.isEmpty() ) {
                if ( qAbs(lvec.x) > 1.0e-5 ) {
                    QList<qreal> sload;
                    Q_FOREACH ( qreal ll, mload->sumUpLoad ) sload.append( ll *lvec.x );
                    LOADDIR dir = ( isMomentLoad ) ? LOADDIR::LDIR_MX : LOADDIR::LDIR_X ;
                    appendLocalMemberLoad(md, mload->loadName, mload->loadType, dir, sload);
                }
                if ( qAbs(lvec.y) > 1.0e-5 ) {
                    QList<qreal> sload;
                    Q_FOREACH ( qreal ll, mload->sumUpLoad ) sload.append( ll *lvec.y );
                    LOADDIR dir = ( isMomentLoad ) ? LOADDIR::LDIR_MY : LOADDIR::LDIR_Y ;
                    appendLocalMemberLoad(md, mload->loadName, mload->loadType, dir, sload);
                }
                if ( qAbs(lvec.z) > 1.0e-5 ) {
                    QList<qreal> sload;
                    Q_FOREACH ( qreal ll, mload->sumUpLoad ) sload.append( ll *lvec.z );
                    LOADDIR dir = ( isMomentLoad ) ? LOADDIR::LDIR_MZ : LOADDIR::LDIR_Z ;
                    appendLocalMemberLoad(md, mload->loadName, mload->loadType, dir, sload);
                }
            } else if ( !mload->concentratedLoad.isEmpty() ) {
                if ( qAbs(lvec.x) > 1.0e-5 ) {
                    QList<QPointF> pload;
                    Q_FOREACH ( QPointF pp, mload->concentratedLoad )
                        pload.append( QPointF(pp.x(), pp.y()*lvec.x) );
                    LOADDIR dir = ( isMomentLoad ) ? LOADDIR::LDIR_MX : LOADDIR::LDIR_X ;
                    appendLocalMemberLoad(md, mload->loadName, mload->loadType, dir, pload);
                }
                if ( qAbs(lvec.y) > 1.0e-5 ) {
                    QList<QPointF> pload;
                    Q_FOREACH ( QPointF pp, mload->concentratedLoad )
                        pload.append( QPointF(pp.x(), pp.y()*lvec.y) );
                    LOADDIR dir = ( isMomentLoad ) ? LOADDIR::LDIR_MY : LOADDIR::LDIR_Y ;
                    appendLocalMemberLoad(md, mload->loadName, mload->loadType, dir, pload);
                }
                if ( qAbs(lvec.z) > 1.0e-5 ) {
                    QList<QPointF> pload;
                    Q_FOREACH ( QPointF pp, mload->concentratedLoad )
                        pload.append( QPointF(pp.x(), pp.y()*lvec.z) );
                    LOADDIR dir = ( isMomentLoad ) ? LOADDIR::LDIR_MZ : LOADDIR::LDIR_Z ;
                    appendLocalMemberLoad(md, mload->loadName, mload->loadType, dir, pload);
                }
            } else if ( qAbs(wa.y()) > 1.0e-3 || qAbs(wb.y()) > 1.0e-3 ) {
                if ( qAbs(lvec.x) > 1.0e-5 ) {
                    WABLOAD wab( QPointF(wa.x(), wa.y()*lvec.x), QPointF(wb.x(), wb.y()*lvec.x) );
                    LOADDIR dir = ( isMomentLoad ) ? LOADDIR::LDIR_MX : LOADDIR::LDIR_X ;
                    appendLocalMemberLoad(md, mload->loadName, mload->loadType, dir, wab);
                }
                if ( qAbs(lvec.y) > 1.0e-5 ) {
                    WABLOAD wab( QPointF(wa.x(), wa.y()*lvec.y), QPointF(wb.x(), wb.y()*lvec.y) );
                    LOADDIR dir = ( isMomentLoad ) ? LOADDIR::LDIR_MY : LOADDIR::LDIR_Y ;
                    appendLocalMemberLoad(md, mload->loadName, mload->loadType, dir, wab);
                }
                if ( qAbs(lvec.z) > 1.0e-5 ) {
                    WABLOAD wab( QPointF(wa.x(), wa.y()*lvec.z), QPointF(wb.x(), wb.y()*lvec.z) );
                    LOADDIR dir = ( isMomentLoad ) ? LOADDIR::LDIR_MZ : LOADDIR::LDIR_Z ;
                    appendLocalMemberLoad(md, mload->loadName, mload->loadType, dir, wab);
                }
            }
        }
    }

    if (progressDialog != nullptr){
        progressDialog->setValue( 100 );
    }
    return err_msg;
}

QString CalculationLoad::calcForMemberArea( DATATYPE areaType,
                                            int &startCount, const int &sumCount, QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads, QProgressDialog *progressDialog )
{
    QList<MemberData *> memberList = UnifiedFixedData::getInstance()->getInputData()->memberListOfType( areaType );
    QString err_msg;


    // 開口をもつ壁・地下外壁の抽出 -> 開口パスの計算
    QList<MemberData *> openWalls;
    if ( areaType == DATATYPE::TPWALL || areaType == DATATYPE::TPUWALL ) {
        Q_FOREACH ( MemberData *md, memberList )
            if ( md->hasWallOpen() ) openWalls.append(md);
    }
    QList<QPainterPath> openPath; // Local座標系の開口パス
    QList<SLITJOINTS> exceptJoints; // スリットにより除外される節点
    if ( !openWalls.isEmpty() ) err_msg = calcWallOpen(openWalls, openPath, exceptJoints);
    if ( !err_msg.isEmpty() ) return err_msg;

    Q_FOREACH ( MemberData *md, memberList ) { // エリアごとのループ　start

        if ( progressDialog != nullptr && progressDialog->wasCanceled() ) return u8"キャンセルされました。";

        startCount++;
        if (progressDialog != nullptr){
            progressDialog->setValue(5 + startCount * 90 / sumCount); // 残り10%はその他の計算
        }
        QList<JointData *> memberJoints = md->getJointList();

        // 代表平面の法線ベクトル:areaWV, 代表平面の原点:op1, 代表平面のu-v:areaUV
        JointData *f_jd = md->iJoint();
        VEC3D op1( f_jd->xPos(), f_jd->yPos(), f_jd->zPos() );
        VEC3D areaUV = md->getUVector();
        VEC3D areaVV = md->getVVector();
        VEC3D areaWV = md->getWVector();

        // Local座標に変換
        QPolygonF areaPoly;
        Q_FOREACH ( JointData *jj, memberJoints ) {
            VEC3D pp = CalcVector3D::globalToLocal(areaUV, areaVV, areaWV, op1,
                                                   VEC3D(jj->xPos(), jj->yPos(), jj->zPos()));
            areaPoly.append( QPointF( pp.x, pp.y )  );
        }
        QPainterPath areaPath;
        areaPath.addPolygon(areaPoly);

        // 領域関連節点、領域関連部材の検索
        QHash<JointData *, QPointF> areaJoints;
        for ( int i = 0; i < memberJoints.count(); i++ )
            areaJoints.insert( memberJoints.at(i), areaPoly.at(i) );
        QList<MemberData *> areaMembers;

        Q_FOREACH ( JointData *jd, memberJoints ) { // 領域周囲点ループ　start

            JointData *ijd = md->previousJoint(jd);
            JointData *jjd = md->nextJoint(jd);

            bool isIOK = false;
            Q_FOREACH ( MemberData *mm, sharedLineMembers(ijd, jd) ) {
                isIOK = true;
                if ( !areaMembers.contains(mm) ) areaMembers.append(mm);
            }
            bool isJOK = false;
            Q_FOREACH ( MemberData *mm, sharedLineMembers(jd, jjd) ) {
                isJOK = true;
                if ( !areaMembers.contains(mm) ) areaMembers.append(mm);
            }
            bool isClosed = ( isIOK && isJOK ) ;

            QList<JointData *> r1Joints;
            Q_FOREACH ( JointData *jj, jd->linkedJoints() ) {

                if ( areaJoints.contains(jj) ) {
                    Q_FOREACH ( MemberData *mm, sharedLineMembers(jd, jj) ) {
                        if ( !areaMembers.contains(mm) ) areaMembers.append(mm);
                    }
                    continue;
                }
                VEC3D v1( jj->xPos() - op1.x, jj->yPos() - op1.y, jj->zPos() - op1.z );
                qreal v1_n = CalcVector3D::norm(v1);
                if ( v1_n < 1.0e-3 ) continue;
                qreal cos_vn = CalcVector3D::dot(v1, areaWV) / v1_n;
                if ( qAbs(cos_vn) > 0.0175 ) continue; // +-1°以上

                VEC3D vg( jj->xPos(), jj->yPos(), jj->zPos() );
                VEC3D vl = CalcVector3D::globalToLocal(areaUV, areaVV, areaWV, op1, vg);
                QPointF pp(vl.x, vl.y);

                // QPolygonF::containsPoint()はバグ？
                if ( CalcVector2D::isInPolygon(areaPoly, pp) ) {
                    r1Joints.append(jj);
                    areaJoints.insert(jj, pp);
                } else if ( !isClosed ) {
                    areaJoints.insert(jj, pp);
                } else {
                    continue;
                }
                Q_FOREACH ( MemberData *mm, sharedLineMembers(jd, jj) ) {
                    if ( !areaMembers.contains(mm) ) areaMembers.append(mm);
                }
            }

            bool isEpt = r1Joints.isEmpty();
            while ( !isEpt ) {
                QList<JointData *> r2Joints;
                Q_FOREACH ( JointData *j1, r1Joints ) {
                    Q_FOREACH ( JointData *j2, j1->linkedJoints() ) {
                        if ( areaJoints.contains(j2) ) {
                            Q_FOREACH ( MemberData *mm, sharedLineMembers(j1, j2) ) {
                                if ( !areaMembers.contains(mm) ) areaMembers.append(mm);
                            }
                            continue;
                        }
                        VEC3D v2( j2->xPos() - op1.x, j2->yPos() - op1.y, j2->zPos() - op1.z );
                        qreal v2_n = CalcVector3D::norm(v2);
                        if ( v2_n < 1.0e-3 ) continue;
                        qreal cos_vn = CalcVector3D::dot(v2, areaWV) / v2_n;
                        if ( qAbs(cos_vn) > 0.0175 ) continue; // +-1°以上

                        VEC3D vg( j2->xPos(), j2->yPos(), j2->zPos() );
                        VEC3D vl = CalcVector3D::globalToLocal(areaUV, areaVV, areaWV, op1, vg);
                        QPointF pp(vl.x, vl.y);

                        // QPolygonF::containsPoint()はバグ？
                        if ( CalcVector2D::isInPolygon(areaPoly, pp) ) {
                            r2Joints.append(j2);
                        }
                        areaJoints.insert(j2, pp);
                        Q_FOREACH ( MemberData *mm, sharedLineMembers(j1, j2) ) {
                            if ( !areaMembers.contains(mm) ) areaMembers.append(mm);
                        }
                    }
                }
                isEpt = r2Joints.isEmpty();
                r1Joints = r2Joints;
            }

        }   // 領域周囲点ループ　end

        // 領域節点areaJoints のうち 解析節点抽出(壁以外)
        QList<JointData *> areaAJoints;
        Q_FOREACH ( JointData *jj, areaJoints.keys() ) {
            if ( jj->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData()) ) areaAJoints.append(jj);
        }

        // 荷重を流す先がない場合、エラー
        if ( areaMembers.isEmpty() && areaAJoints.isEmpty() ) {
            err_msg += u8"固定荷重の伝達先が見つからない。\n";
            err_msg += memberInfo(md);
            return err_msg;
        }

        // 領域からRC,SRC部材の内法を除去
        Q_FOREACH ( MemberData *mm, areaMembers ) {

            if ( areaType == DATATYPE::TPSLABLOAD ) continue;
            qreal height = mm->getAttachedHeight(*UnifiedFixedData::getInstance()->getInputData());
            STRUCTTYPE stp = UnifiedFixedData::getInstance()->getInputData()->sectionNameToStructureType(mm->dataType(),
                                                                                    mm->sectionName(), height);
            if ( stp != STRUCTTYPE::STP_RC && stp != STRUCTTYPE::STP_SRC ) continue;

            qreal bb = mm->getHLineShape().centerSizeB;
            qreal hh = mm->getHLineShape().centerSizeH;

            VEC3D u_v = mm->getUVector();
            VEC3D v_v = mm->getVVector();
            VEC3D w_v = mm->getWVector();

            XYZ ixyz = mm->iJoint()->xyz();
            VEC3D v_0(ixyz.x, ixyz.y, ixyz.z);
            VEC3D v_1(0.0, bb / 2000.0, 0.0);
            VEC3D v_2(0.0, 0.0, hh / 2000.0);

            v_1 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v_0, v_1);
            v_2 = CalcVector3D::localToGlobal(u_v, v_v, w_v, v_0, v_2);

            v_0 = CalcVector3D::globalToLocal(areaUV, areaVV, areaWV, op1, v_0);
            v_1 = CalcVector3D::globalToLocal(areaUV, areaVV, areaWV, op1, v_1);
            v_2 = CalcVector3D::globalToLocal(areaUV, areaVV, areaWV, op1, v_2);

            qreal l1 = (v_1.x - v_0.x) * (v_1.x - v_0.x) + (v_1.y - v_0.y) * (v_1.y - v_0.y);
            qreal l2 = (v_2.x - v_0.x) * (v_2.x - v_0.x) + (v_2.y - v_0.y) * (v_2.y - v_0.y);

            // 断面を領域に投影した際、断面幅Bが大きいか、断面高さHが大きいか
            bool isLargeB = ( l1 > l2 - 1.0e-5 );

            QList<JointData *> jdlist = mm->getJointList();
            for ( int i = 0; i < jdlist.count() - 1; i++ ) {

                JointData *ijd = jdlist.at(i);
                JointData *jjd = jdlist.at(i + 1);

                if ( !areaJoints.contains(ijd) || !areaJoints.contains(jjd) ) continue;

                QPointF ipt = areaJoints.value(ijd);
                QPointF jpt = areaJoints.value(jjd);

                QPointF vij = (jpt - ipt) / CalcVector2D::length(jpt - ipt);
                vij = ( isLargeB ) ? vij * (bb / 2000.0) : vij * (hh / 2000.0) ;

                QPolygonF r_poly;
                r_poly.append( QPointF( vij.y(), -vij.x() ) + ipt );
                r_poly.append( QPointF( vij.y(), -vij.x() ) + jpt );
                r_poly.append( QPointF( -vij.y(), vij.x() ) + jpt );
                r_poly.append( QPointF( -vij.y(), vij.x() ) + ipt );

                QPainterPath r_path;
                r_path.addPolygon(r_poly);

                areaPath = areaPath.subtracted(r_path);
            }
        }

        // 開口部分を面領域から除去
        int op_idx = ( areaType == DATATYPE::TPWALL || areaType == DATATYPE::TPUWALL ) ? openWalls.indexOf(md) : -1 ;
        if ( op_idx > -1 ) {
            areaPath = areaPath.subtracted( openPath.at(op_idx) );
        }

        areaPath = areaPath.simplified();

        // 領域のバウンディングレクト 25cmユニット分割
        QRectF areaRect = areaPoly.boundingRect();
        qreal unit_w = 0.25; // ユニット1辺　25cm
        int x_count = int((areaRect.width() + 1.0e-5) / unit_w);
        int y_count = int((areaRect.height() + 1.0e-5) / unit_w);
        qreal x_edge = ( areaRect.width() - qreal(x_count) * unit_w ) / 2.0;
        qreal y_edge = ( areaRect.height() - qreal(y_count) * unit_w ) / 2.0;

//        qDebug() << "POLY" << areaPoly;
//        qDebug() << "RECT" << areaRect << x_count << y_count << x_edge << y_edge;

        // 荷重ユニットごとに近い節点、近い部材を計算

        QMultiHash<MemberData *, XYZ> mloads; // XYZ:荷重点x,荷重点y,面積S
        QMultiHash<JointData *, qreal> jloads; // qreal:面積S
        qreal sum_area = 0.0; // 確認用

        // スラブの荷重方向が定義されている場合
        bool isDirFixed = false;
        QPointF ldir_mp;
        if ( areaType == DATATYPE::TPSLAB ) {
            qreal ldir_v = UnifiedFixedData::getInstance()->getInputData()->getSlabLoadDir(md->sectionName());
            isDirFixed = ( ldir_v > -1.0e-5 && ldir_v < 180.0 + 1.0e-5 );
            if ( isDirFixed ) {
                VEC3D v0( op1.x + qCos(ldir_v / 180.0 * M_PI), op1.y + qSin(ldir_v / 180.0 * M_PI), op1.z );
                VEC3D v1 = CalcVector3D::globalToLocal(areaUV, areaVV, areaWV, op1, v0);
                ldir_mp = QPointF(v1.x, v1.y);
            }
        }

        SLITJOINTS slitJoints;
        if ( op_idx > -1 ) slitJoints = exceptJoints.at(op_idx);
        QList<JointData *> slit_allJoints;
        Q_FOREACH ( JointData *jj, slitJoints.topJoints )
            if ( !slit_allJoints.contains(jj) ) slit_allJoints.append(jj);
        Q_FOREACH ( JointData *jj, slitJoints.bottomJoints )
            if ( !slit_allJoints.contains(jj) ) slit_allJoints.append(jj);
        Q_FOREACH ( JointData *jj, slitJoints.leftJoints )
            if ( !slit_allJoints.contains(jj) ) slit_allJoints.append(jj);
        Q_FOREACH ( JointData *jj, slitJoints.rightJoints )
            if ( !slit_allJoints.contains(jj) ) slit_allJoints.append(jj);

        qreal x_ut = areaRect.topLeft().x();
        for ( int ix = 0; ix < x_count + 2; ix++ ) { // 荷重ユニットループ　start
            qreal xw = unit_w;
            if ( ix == 0 || ix == x_count + 1 ) {
                if ( qAbs(x_edge) < 1.0e-3 ) continue;
                xw = x_edge;
            }

            qreal y_ut = areaRect.topLeft().y();
            for ( int iy = 0; iy < y_count + 2; iy++ ) {
                qreal yw = unit_w;
                if ( iy == 0 || iy == y_count + 1 ) {
                    if ( qAbs(y_edge) < 1.0e-3 ) continue;
                    yw = y_edge;
                }

                QPainterPath unit_path;
                unit_path.addRect(x_ut, y_ut, xw, yw);
                unit_path = unit_path.intersected( areaPath );
                unit_path = unit_path.simplified();
                QPolygonF unit_poly = unit_path.toFillPolygon(QTransform());

                qreal u_area = qAbs( CalcVector2D::signedPolygonArea(unit_poly) );
                sum_area += u_area; // 確認用

                if ( u_area > 1.0e-5 ) {

                    QPointF u_center = CalcVector2D::centerOfPolygon(unit_poly);

                    // --近い節点
                    qreal min_jlen = 1.0e+5;
                    QList<JointData *> min_jdlist;
                    Q_FOREACH ( JointData *jj, areaAJoints ) {

                        // スリット領域の節点は除く
                        if ( slit_allJoints.contains(jj) ) continue;

                        QPointF pp = areaJoints.value(jj);
                        if ( isDirFixed ) { // 荷重方向が決まっている場合
                            QPointF p1 = u_center + ldir_mp;
                            qreal dd = CalcVector2D::distance(u_center, p1, pp);
                            if ( dd > 1.0e-3 ) continue;
                        }
                        qreal cur_len = CalcVector2D::length( u_center - pp );
                        if ( cur_len < min_jlen - 1.0e-3 ) min_jdlist.clear();
                        if ( cur_len < min_jlen + 1.0e-3 ) min_jdlist.append(jj);
                        min_jlen = qMin( min_jlen, cur_len );
                    }
                    Q_FOREACH ( JointData *jj, min_jdlist ) {
                        QPointF pp = areaJoints.value(jj);
                        qreal cur_len = CalcVector2D::length( u_center - pp );
                        if ( cur_len > min_jlen + 1.0e-3 / 2.0 ) min_jdlist.removeOne(
                                jj); // 最小から0.5mm 以上は除く
                    }

                    // --近い部材
                    qreal min_mlen = 1.0e+5;
                    QList<MemberData *> min_mdlist;
                    QList<QPointF> min_points;
                    Q_FOREACH ( MemberData *mm, areaMembers ) {

                        QList<JointData *> jdlist = mm->getJointList(); //  mm->getStraightJointList();
                        qreal min_slen = min_mlen;
                        QList<QPointF> near_pt;
                        for ( int i = 0; i < jdlist.count() - 1; i++ ) {
                            JointData *j1 = jdlist.at(i);
                            JointData *j2 = jdlist.at(i + 1);
                            if ( !areaJoints.contains(j1) || !areaJoints.contains(j2) ) continue;

                            // スリット領域の部材は除く
                            if ( slitJoints.topJoints.contains(j1) && slitJoints.topJoints.contains(j2) ) continue;
                            if ( slitJoints.bottomJoints.contains(j1) && slitJoints.bottomJoints.contains(j2) ) continue;
                            if ( slitJoints.leftJoints.contains(j1) && slitJoints.leftJoints.contains(j2) ) continue;
                            if ( slitJoints.rightJoints.contains(j1) && slitJoints.rightJoints.contains(j2) ) continue;

                            QPointF p1 = areaJoints.value(j1);
                            QPointF p2 = areaJoints.value(j2);

                            qreal cur_len = 1.0e+5;
                            if ( isDirFixed ) { // 荷重方向が決まっている場合
                                QLineF l1(u_center, u_center + ldir_mp);
                                QLineF l2(p1, p2);
                                QList<QPointF> cplist = CalcVector2D::intersectSegmentToLine(l1, l2, 1.0e-3);
                                if ( cplist.isEmpty() ) continue;
                                cur_len = CalcVector2D::length( u_center - cplist.first() );
                            } else {
                                cur_len = CalcVector2D::segmentDistance(p1, p2, u_center);
                            }
                            if ( cur_len < min_slen - 1.0e-3 ) near_pt.clear();
                            if ( cur_len < min_slen + 1.0e-3 ) near_pt.append( CalcVector2D::perpPointToSegment(p1, p2,
                                                                                                                    u_center) );

                            min_slen = qMin( min_slen, cur_len );
                        }
                        if ( near_pt.isEmpty() ) continue;
                        if ( min_slen < min_mlen - 1.0e-3 ) {
                            min_mdlist.clear();
                            min_points.clear();
                        }
                        if ( min_slen < min_mlen + 1.0e-3 ) {
                            Q_FOREACH ( QPointF pp, near_pt ) {
                                min_mdlist.append(mm);
                                min_points.append(pp);
                            }
                        }
                        min_mlen = qMin( min_mlen, min_slen );

                    }
                    for ( int i = min_points.count() - 1; i >= 0; i-- ) {
                        qreal cur_len = CalcVector2D::length( min_points.at(i) - u_center );
                        if ( cur_len > min_mlen + 1.0e-3 / 2.0 ) { // 最小から 0.5mm以上は除く
                            min_mdlist.removeAt(i);
                            min_points.removeAt(i);
                        }
                    }

                    if ( min_jlen < min_mlen ) {
                        int icc = min_jdlist.count();
                        Q_FOREACH ( JointData *jj, min_jdlist ) {
                            jloads.insert(jj, u_area / qreal(icc));
                        }
                    } else {
                        int icc = min_mdlist.count();
                        for ( int i = 0; i < icc; i++ ) {
                            MemberData *mm = min_mdlist.at(i);
                            QPointF pp = min_points.at(i);
                            mloads.insert(mm, XYZ(pp.x(), pp.y(), u_area / qreal(icc)));
                        }
                    }
                }
                y_ut += ( iy == 0 ) ? y_edge : unit_w;
            }
            x_ut += ( ix == 0 ) ? x_edge : unit_w;

        }   // 荷重ユニットループ　end

        // 部材、節点に分配された荷重を集計、全体座標系に変換
//        qreal mband=0.5; 部材の分割幅　50cm
        qreal loadCor = 1.0; // 面積にかける係数
        SLABLOADVALUES sl_value; // 床荷重の場合

        if ( areaType == DATATYPE::TPSLABLOAD ) {

            VEC3D w_v(0.0, 0.0, 1.0);
            qreal cos1 = CalcVector3D::dot(w_v, areaWV);

            loadCor = -qAbs(cos1) * 0.001; // 水平投影面積×（ N -> kN ）
            sl_value = UnifiedFixedData::getInstance()->getInputData()->getSlabLoadValue(md->sectionName());

        } else {

            qreal hh = md->getAttachedHeight(*UnifiedFixedData::getInstance()->getInputData());
            SECTIONWEIGHT sw = UnifiedFixedData::getInstance()->getInputData()->sectionNameToUnitWeight(md->dataType(),
                                                                                   md->sectionName(), hh);
            loadCor = -sw.i_weight; // 比重×スラブ厚

        }

        Q_FOREACH ( MemberData *mm, mloads.uniqueKeys() ) {

            QList<qreal> s_lenlist = mm->getSeparatedLength();
            int mct = s_lenlist.count();
            qreal *lsum = new qreal[mct];
            for ( int i = 0; i < mct; i++ ) lsum[i] = 0.0;

            QList<XYZ> xyl_list = mloads.values(mm);
            Q_FOREACH ( XYZ xyl, xyl_list ) {
                VEC3D vv = CalcVector3D::localToGlobal(areaUV, areaVV, areaWV, op1, VEC3D(xyl.x, xyl.y, 0.0));
                qreal ll = mm->perpLengthOfMember(XYZ(vv.x, vv.y, vv.z));

                if ( ll > s_lenlist.at(mct - 1) - 1.0e-5 ) {
                    lsum[mct - 1] += xyl.z;
                } else {
                    for ( int i = 0; i < mct; i++ ) {
                        if ( ll < s_lenlist.at(i) ) {
                            if ( i == 0 ) {
                                lsum[0] += xyl.z;
                            } else {
                                qreal dif = s_lenlist.at(i) - s_lenlist.at(i - 1);
                                lsum[i - 1] += xyl.z * (s_lenlist.at(i) - ll) / dif;
                                lsum[i] += xyl.z * (ll - s_lenlist.at(i - 1)) / dif;
                            }
                            break;
                        }
                    }
                }
            }

            if ( areaType == DATATYPE::TPSLABLOAD ) {

                if ( qAbs(sl_value.sDL) > 1.0e-3 ) {
                    QList<qreal> qsum;
                    for ( int i = 0; i < mct; i++ ) qsum.append( lsum[i]*loadCor * sl_value.sDL );
                    appendMemberLoad(mm, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, qsum, globalMemberLoads);
                }
                if ( qAbs(sl_value.sLL1) > 1.0e-3 ) {
                    QList<qreal> qsum;
                    for ( int i = 0; i < mct; i++ ) qsum.append( lsum[i]*loadCor * sl_value.sLL1 );
                    appendMemberLoad(mm, QString(), LoadType::LTYPE_LL1, LOADDIR::LDIR_Z, qsum, globalMemberLoads);
                }
                if ( qAbs(sl_value.sLL2) > 1.0e-3 ) {
                    QList<qreal> qsum;
                    for ( int i = 0; i < mct; i++ ) qsum.append( lsum[i]*loadCor * sl_value.sLL2 );
                    appendMemberLoad(mm, QString(), LoadType::LTYPE_LL2, LOADDIR::LDIR_Z, qsum, globalMemberLoads);
                }
                if ( qAbs(sl_value.sLL3) > 1.0e-3 ) {
                    QList<qreal> qsum;
                    for ( int i = 0; i < mct; i++ ) qsum.append( lsum[i]*loadCor * sl_value.sLL3 );
                    appendMemberLoad(mm, QString(), LoadType::LTYPE_LL3, LOADDIR::LDIR_Z, qsum, globalMemberLoads);
                }
                if ( qAbs(sl_value.sLL4) > 1.0e-3 ) {
                    QList<qreal> qsum;
                    for ( int i = 0; i < mct; i++ ) qsum.append( lsum[i]*loadCor * sl_value.sLL4 );
                    appendMemberLoad(mm, QString(), LoadType::LTYPE_LL4, LOADDIR::LDIR_Z, qsum, globalMemberLoads);
                }
                if ( qAbs(sl_value.sLLE) > 1.0e-3 ) {
                    QList<qreal> qsum;
                    for ( int i = 0; i < mct; i++ ) qsum.append( lsum[i]*loadCor * sl_value.sLLE );
                    appendMemberLoad(mm, QString(), LoadType::LTYPE_LLE, LOADDIR::LDIR_Z, qsum, globalMemberLoads);
                }

            } else {

                QList<qreal> qsum;
                for ( int i = 0; i < mct; i++ ) qsum.append( lsum[i]*loadCor );
                appendMemberLoad(mm, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, qsum, globalMemberLoads);

            }

            delete [] lsum;
        }

        Q_FOREACH ( JointData *jj, jloads.uniqueKeys() ) {
//            qreal qsum=0.0;
//            Q_FOREACH( qreal qq, jloads.values(jj) ) qsum+=qq*loadCor;
//            appendJointLoad(jj,LTYPE_DL,LDIR_Z,qsum);
//            qDebug() << "JLOAD" << FactoryUnifiedData::getInstance(isOutput)->indexOfJoint(jj)+1 << qsum;

            if ( areaType == DATATYPE::TPSLABLOAD ) {

                if ( qAbs(sl_value.sDL) > 1.0e-3 ) {
                    qreal qsum = 0.0;
                    Q_FOREACH ( qreal qq, jloads.values(jj) ) qsum += qq *loadCor *sl_value.sDL;
                    if (qAbs(qsum) > 1.0e-3) appendJointLoad(jj, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, qsum);
                }
                if ( qAbs(sl_value.sLL1) > 1.0e-3 ) {
                    qreal qsum = 0.0;
                    Q_FOREACH ( qreal qq, jloads.values(jj) ) qsum += qq *loadCor *sl_value.sLL1;
                    if (qAbs(qsum) > 1.0e-3) appendJointLoad(jj, QString(), LoadType::LTYPE_LL1, LOADDIR::LDIR_Z, qsum);
                }
                if ( qAbs(sl_value.sLL2) > 1.0e-3 ) {
                    qreal qsum = 0.0;
                    Q_FOREACH ( qreal qq, jloads.values(jj) ) qsum += qq *loadCor *sl_value.sLL2;
                    if (qAbs(qsum) > 1.0e-3) appendJointLoad(jj, QString(), LoadType::LTYPE_LL2, LOADDIR::LDIR_Z, qsum);
                }
                if ( qAbs(sl_value.sLL3) > 1.0e-3 ) {
                    qreal qsum = 0.0;
                    Q_FOREACH ( qreal qq, jloads.values(jj) ) qsum += qq *loadCor *sl_value.sLL3;
                    if (qAbs(qsum) > 1.0e-3) appendJointLoad(jj, QString(), LoadType::LTYPE_LL3, LOADDIR::LDIR_Z, qsum);
                }
                if ( qAbs(sl_value.sLL4) > 1.0e-3 ) {
                    qreal qsum = 0.0;
                    Q_FOREACH ( qreal qq, jloads.values(jj) ) qsum += qq *loadCor *sl_value.sLL4;
                    if (qAbs(qsum) > 1.0e-3) appendJointLoad(jj, QString(), LoadType::LTYPE_LL4, LOADDIR::LDIR_Z, qsum);
                }
                if ( qAbs(sl_value.sLLE) > 1.0e-3 ) {
                    qreal qsum = 0.0;
                    Q_FOREACH ( qreal qq, jloads.values(jj) ) qsum += qq *loadCor *sl_value.sLLE;
                    if (qAbs(qsum) > 1.0e-3) appendJointLoad(jj, QString(), LoadType::LTYPE_LLE, LOADDIR::LDIR_Z, qsum);
                }

            } else {

                qreal qsum = 0.0;
                Q_FOREACH ( qreal qq, jloads.values(jj) ) qsum += qq *loadCor;
                if (qAbs(qsum) > 1.0e-3) appendJointLoad(jj, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, qsum);

            }
        }


//        qreal psum=0.0;
//        Q_FOREACH( QPolygonF aply, areaFillPolygons )
//            psum+=qAbs( CalcVector2D::signedPolygonArea(aply) );
//        qreal u_area=qAbs( CalcVector2D::signedPolygonArea(unit_poly) );
//        qDebug() << "AREA" << psum << sum_area ;

        /*
                Q_FOREACH( JointData* jj, areaJoints.keys() ){
                    qDebug() << "j" << FactoryUnifiedData::getInstance(isOutput)->indexOfJoint(jj)+1;
                }
                Q_FOREACH( MemberData* mm, areaMembers ){
                    qDebug() << "m" << mm->sectionName();
                }
        */

    }   // エリアごとのループ　end

    return QString();
}

QString CalculationLoad::calcWallOpen( const QList<MemberData *> &mdlist,
                                       QList<QPainterPath> &openPath,
                                       QList<SLITJOINTS> &exceptJoints )
{
    MemberData *err_md = NULL;
    bool isErr = false;

    Q_FOREACH ( MemberData *md, mdlist ) {

        QList<QUuid> frlist = md->getAttachedFrameList();
        if ( frlist.isEmpty() ) {
            isErr = true;
            err_md = md;
            break;
        }
        QUuid fr_id = frlist.first();

        QList<JointData *> jointList = md->getJointList();
        QList<QPointF> pointList; // 通り座標系でのxyz
        Q_FOREACH ( JointData *jd, jointList ) {
            XYZ xyz = UnifiedFixedData::getInstance()->getInputData()->globalXYZToFrameXYZ(fr_id, jd->xyz());
            pointList.append(QPointF(xyz.x * 1000.0, xyz.y * 1000.0));
        }

        // 閉じた円の場合
        FrameType::FRAMETYPE fr_type = UnifiedFixedData::getInstance()->getInputData()->getFrameTypeOfFloor(fr_id);
        if ( fr_type == FrameType::FRAMETYPE::CLCIRCLE || fr_type == FrameType::FRAMETYPE::CLVARIABLE ) {
            for ( int i = 0; i < pointList.count(); i++ ) {
                QPointF pp = pointList.at(i);
                if ( pp.x() < 1.0e-3 ) {
                    int j = ( i == 0 ) ? pointList.count() - 1 : i - 1;
                    qreal xx = 0.0;
                    while ( j != i ) {
                        if ( pointList.at(j).x() >= 1.0e-3 ) {
                            xx = pointList.at(j).x();
                            break;
                        }
                        j = ( j == 0 ) ? pointList.count() - 1 : j - 1;
                    }
                    if ( xx < 1.0e-3 ) {
                        isErr = true;
                        break;
                    }
                    qreal ex = UnifiedFixedData::getInstance()->getInputData()->globalZToClosedFrameEndX(fr_id, pp.y());
                    if ( xx > ex / 2.0 ) {
                        pointList.replace(i, QPointF(ex, pp.y()));
                    }
                }
            }
            if ( isErr ) {
                err_md = md;
                break;
            }
        }

        qreal area = 0.0;
        for ( int i = 0; i < pointList.count(); i++ ) {
            QPointF p1 = pointList.at(i);
            QPointF p2 = ( i == pointList.count() - 1 ) ? pointList.first() : pointList.at(i + 1) ;
            area += p1.x() * p2.y() - p2.x() * p1.y();
        }
        area = area / 2.0;

        int id_lb = 0, id_rb = 0, id_lt = 0, id_rt = 0;
        qreal r1max = -1.0e+5, r1min = 1.0e+5, r2max = -1.0e+5, r2min = 1.0e+5;
        for ( int i = 0; i < pointList.count(); i++ ) {
            QPointF pp = pointList.at(i) ;
            qreal r1 = pp.x() + pp.y();
            if ( r1 > r1max ) {
                id_rt = i;
                r1max = r1;
            }
            if ( r1 < r1min ) {
                id_lb = i;
                r1min = r1;
            }
            qreal r2 = pp.x() - pp.y();
            if ( r2 > r2max ) {
                id_rb = i;
                r2max = r2;
            }
            if ( r2 < r2min ) {
                id_lt = i;
                r2min = r2;
            }
        }

        QList<WallOpenParameter> o_values = UnifiedFixedData::getInstance()->getInputData()->getWallOpenValues(
                                                md->wallOpenName());
        QList<QPolygonF> op_polygons; // 一壁分の開口パス 軸組座標系

        // 開口数ループ
        Q_FOREACH ( WallOpenParameter wp, o_values ) {
            if ( wp.openType != WallOpenType::Type::NORMAL ) continue;

            QPointF o_p, v_x, v_y;
            /* TODO
            if ( wp.isXL && wp.isYB ) {
                o_p = pointList.at(id_lb);
                if ( area > 0.0 ) {
                    QPointF pp = ( id_lb == pointList.count() - 1 ) ? pointList.first() : pointList.at(id_lb + 1);
                    v_x = pp - o_p;
                } else {
                    QPointF pp = ( id_lb == 0 ) ? pointList.last() : pointList.at(id_lb - 1);
                    v_x = pp - o_p;
                }
                v_x /= CalcVector2D::length(v_x);
                v_y = QPointF(-v_x.y(), v_x.x());
            } else if ( wp.isXL && !wp.isYB ) {
                o_p = pointList.at(id_lt);
                if ( area > 0.0 ) {
                    QPointF pp = ( id_lt == 0 ) ? pointList.last() : pointList.at(id_lt - 1);
                    v_x = pp - o_p;
                } else {
                    QPointF pp = ( id_lt == pointList.count() - 1 ) ? pointList.first() : pointList.at(id_lt + 1);
                    v_x = pp - o_p;
                }
                v_x /= CalcVector2D::length(v_x);
                v_y = QPointF(v_x.y(), -v_x.x());
            } else if ( !wp.isXL && wp.isYB ) {
                o_p = pointList.at(id_rb);
                if ( area > 0.0 ) {
                    QPointF pp = ( id_rb == 0 ) ? pointList.last() : pointList.at(id_rb - 1);
                    v_x = pp - o_p;
                } else {
                    QPointF pp = ( id_rb == pointList.count() - 1 ) ? pointList.first() : pointList.at(id_rb + 1);
                    v_x = pp - o_p;
                }
                v_x /= CalcVector2D::length(v_x);
                v_y = QPointF(v_x.y(), -v_x.x());
            } else {
                o_p = pointList.at(id_rt);
                if ( area > 0.0 ) {
                    QPointF pp = ( id_rt == pointList.count() - 1 ) ? pointList.first() : pointList.at(id_rt + 1);
                    v_x = pp - o_p;
                } else {
                    QPointF pp = ( id_rt == 0 ) ? pointList.last() : pointList.at(id_rt - 1);
                    v_x = pp - o_p;
                }
                v_x /= CalcVector2D::length(v_x);
                v_y = QPointF(-v_x.y(), v_x.x());
            }

            */
            QPointF p1 = pointList.at(id_lb);
            QPointF p2 = pointList.at(id_rb);
            QPointF p3 = pointList.at(id_rt);
            QPointF p4 = pointList.at(id_lt);

            if ( wp.width < 1.0e-5 && wp.height > 1.0e-5 ) {
                QLineF l1( o_p + v_y * wp.oy, o_p + v_y * wp.oy + v_x ); // 直線
                QLineF l2( o_p + v_y * (wp.oy + wp.height), o_p + v_y * (wp.oy + wp.height) + v_x ); // 直線
                qreal x1max = -1.0e+5, x1min = 1.0e+5, x2max = -1.0e+5, x2min = 1.0e+5;
                for ( int i = 0; i < pointList.count(); i++ ) {
                    QLineF ll = ( i == pointList.count() - 1 ) ? QLineF(pointList.at(i), pointList.first())
                                : QLineF(pointList.at(i), pointList.at(i + 1));
                    QList<QPointF> plist1 = CalcVector2D::intersectSegmentToLine(l1, ll);
                    QList<QPointF> plist2 = CalcVector2D::intersectSegmentToLine(l2, ll);
                    Q_FOREACH ( QPointF pp, plist1 ) {
                        if ( pp.x() > x1max ) {
                            x1max = pp.x();
                            p2 = pp;
                        }
                        if ( pp.x() < x1min ) {
                            x1min = pp.x();
                            p1 = pp;
                        }
                    }
                    Q_FOREACH ( QPointF pp, plist2 ) {
                        if ( pp.x() > x2max ) {
                            x2max = pp.x();
                            p3 = pp;
                        }
                        if ( pp.x() < x2min ) {
                            x2min = pp.x();
                            p4 = pp;
                        }
                    }
                }
            } else if ( wp.width > 1.0e-5 && wp.height < 1.0e-5 ) {
                QLineF l1( o_p + v_x * wp.ox, o_p + v_x * wp.ox + v_y ); // 直線
                QLineF l2( o_p + v_x * (wp.ox + wp.width), o_p + v_x * (wp.ox + wp.width) + v_y ); // 直線
                qreal y1max = -1.0e+5, y1min = 1.0e+5, y2max = -1.0e+5, y2min = 1.0e+5;
                for ( int i = 0; i < pointList.count(); i++ ) {
                    QLineF ll = ( i == pointList.count() - 1 ) ? QLineF(pointList.at(i), pointList.first())
                                : QLineF(pointList.at(i), pointList.at(i + 1));
                    QList<QPointF> plist1 = CalcVector2D::intersectSegmentToLine(l1, ll);
                    QList<QPointF> plist2 = CalcVector2D::intersectSegmentToLine(l2, ll);
                    Q_FOREACH ( QPointF pp, plist1 ) {
                        if ( pp.y() > y1max ) {
                            y1max = pp.y();
                            p4 = pp;
                        }
                        if ( pp.y() < y1min ) {
                            y1min = pp.y();
                            p1 = pp;
                        }
                    }
                    Q_FOREACH ( QPointF pp, plist2 ) {
                        if ( pp.y() > y2max ) {
                            y2max = pp.y();
                            p3 = pp;
                        }
                        if ( pp.y() < y2min ) {
                            y2min = pp.y();
                            p2 = pp;
                        }
                    }
                }
            } else if ( wp.width > 1.0e-5 && wp.height > 1.0e-5 ) {
                p1 = o_p + v_x * wp.ox + v_y * wp.oy;
                p2 = p1 + v_x * wp.width;
                p3 = p2 + v_y * wp.height;
                p4 = p1 + v_y * wp.height;
            }

            QPolygonF poly;
            poly << p1 << p2 << p3 << p4;
            op_polygons.append( poly );
        }

        // 開口座標を通り座標系からグローバルに変換、グローバルから開口座標系に変換
        QPainterPath op_path;   // 一壁分の開口パス 開口座標系
        if ( !op_polygons.isEmpty() ) {

            JointData *f_jd = md->iJoint();
            VEC3D op1( f_jd->xPos(), f_jd->yPos(), f_jd->zPos() );
            VEC3D areaUV = md->getUVector();
            VEC3D areaVV = md->getVVector();
            VEC3D areaWV = md->getWVector();

            Q_FOREACH ( QPolygonF poly, op_polygons ) {
                QPolygonF poly_w;
                Q_FOREACH ( QPointF pp, poly ) {
                    XYZ xyz = UnifiedFixedData::getInstance()->getInputData()->frameXYZToGlobalXYZ(fr_id, XYZ(pp.x() / 1000.0,
                                                                                         pp.y() / 1000.0));
                    VEC3D vg = VEC3D(xyz.x, xyz.y, xyz.z);
                    VEC3D vl = CalcVector3D::globalToLocal(areaUV, areaVV, areaWV, op1, vg);
                    poly_w << QPointF(vl.x, vl.y);
                }
                op_path.addPolygon(poly_w);
            }
        }
        openPath.append(op_path);  // 全壁に対する開口パス

        // スリット
        qreal s_l = 0.0, s_r = 0.0, s_b = 0.0, s_t = 0.0;
        Q_FOREACH ( WallOpenParameter wp, o_values ) {
            if ( wp.openType != WallOpenType::Type::SLIT ) continue;
            s_l = qMax(s_l, wp.slitL);
            s_r = qMax(s_r, wp.slitR);
            s_b = qMax(s_b, wp.slitB);
            s_t = qMax(s_t, wp.slitT);
        }

        qreal xmax = pointList.first().x();
        qreal xmin = pointList.first().x();
        qreal ymax = pointList.first().y();
        qreal ymin = pointList.first().y();
        Q_FOREACH ( QPointF pp, pointList ) {
            xmax = qMax(xmax, pp.x());
            xmin = qMin(xmin, pp.x());
            ymax = qMax(ymax, pp.y());
            ymin = qMin(ymin, pp.y());
        }

        s_l = xmin + s_l;
        s_r = xmax - s_r;
        s_b = ymin + s_b;
        s_t = ymax - s_t;
//        QRectF crect( QPointF(s_l-0.01,s_b-0.01),QPointF(s_r+0.01,s_t+0.01) );

        QList<JointData *> l_jlist, r_jlist, t_jlist, b_jlist; // 一壁に対するスリット節点
        for ( int i = 0; i < pointList.count(); i++ ) {
            QPointF pp = pointList.at(i);
            qreal xx = pp.x();
            qreal yy = pp.y();
            if ( xx < s_l - 0.01 ) l_jlist.append( jointList.at(i) );
            if ( xx > s_r + 0.01 ) r_jlist.append( jointList.at(i) );
            if ( yy < s_b - 0.01 ) b_jlist.append( jointList.at(i) );
            if ( yy > s_t + 0.01 ) t_jlist.append( jointList.at(i) );
        }
        exceptJoints.append( SLITJOINTS(t_jlist, b_jlist, l_jlist,
                                        r_jlist) ); // 全壁に対するスリット節点
    }

    if ( err_md != NULL ) {
        QString err_msg = u8"開口をもつ壁は通りに属さなければならない。\n";
        err_msg += memberInfo(err_md);
        return err_msg;
    }

    return QString();
}

QList<MemberData *> CalculationLoad::sharedLineMembers( JointData *j1, JointData *j2 ) const
{
    QList<MemberData *> m1_list = j1->getRelatedMembers();
    QList<MemberData *> m2_list = j2->getRelatedMembers();
    QList<MemberData *> s_mdlist;
    Q_FOREACH ( MemberData *md, m1_list ) {
        if ( !md->isLineMember(false) ) continue;
        if ( m2_list.contains(md) ) s_mdlist.append(md);
    }
    return s_mdlist;
}

void CalculationLoad::appendMemberLoad( MemberData *md, const QString &name, LoadType::LOADTYPE ltype,
                                        LOADDIR ldir, const QList<qreal> &load , QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads)
{
    bool isDetected = false;
    Q_FOREACH ( MLOADSUM *ml, globalMemberLoads.values(md) ) {
        if ( ml->loadType == ltype && ml->loadDir == ldir && ml->loadName == name ) {
            QList<qreal> cload = ml->sumUpLoad;
            if ( cload.isEmpty() ) continue;
            for ( int i = 0; i < cload.count() ; i++ ) {
                qreal ll = cload.at(i) + load.at(i);
                cload.replace(i, ll);
            }
            ml->sumUpLoad = cload;
            isDetected = true;
            break;
        }
    }
    if ( !isDetected ) {
        MLOADSUM *ml = new MLOADSUM( name, ltype, ldir, load );
        globalMemberLoads.insert(md, ml);
    }
}

void CalculationLoad::appendMemberLoad( MemberData *md, const QString &name, LoadType::LOADTYPE ltype,
                                        LOADDIR ldir, const QPointF &lp , QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads)
{
    appendMemberLoad(md, name, ltype, ldir, QList<QPointF>() << lp , globalMemberLoads);
}

void CalculationLoad::appendMemberLoad( MemberData *md, const QString &name, LoadType::LOADTYPE ltype,
                                        LOADDIR ldir, const QList<QPointF> &load , QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads)
{
    bool isDetected = false;
    Q_FOREACH ( MLOADSUM *ml, globalMemberLoads.values(md) ) {
        if ( ml->loadType == ltype && ml->loadDir == ldir && ml->loadName == name ) {
            QList<QPointF> cload = ml->concentratedLoad;
            if ( cload.isEmpty() ) continue;
            for ( int i = 0; i < load.count(); i++ ) {
                qreal xx = load.at(i).x();
                qreal yy = load.at(i).y();
                bool isOverap = false;
                for ( int j = 0; j < cload.count(); j++ ) {
                    if ( qAbs(xx - cload.at(j).x()) < 1.0e-2 ) {
                        qreal cy = yy + cload.at(j).y();
                        cload.replace(j, QPointF(cload.at(j).x(), cy));
                        isOverap = true;
                        break;
                    }
                }
                if ( !isOverap ) cload.append( load.at(i) );
            }
            ml->concentratedLoad = cload;
            isDetected = true;
            break;
        }
    }

    if ( !isDetected ) {
        MLOADSUM *ml = new MLOADSUM( name, ltype, ldir, QList<qreal>(), load );
        globalMemberLoads.insert(md, ml);
    }
}

void CalculationLoad::appendMemberLoad( MemberData *md, const QString &name, LoadType::LOADTYPE ltype,
                                        LOADDIR ldir, const WABLOAD &wab , QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads)
{
    bool isDetected = false;
    Q_FOREACH ( MLOADSUM *ml, globalMemberLoads.values(md) ) {
        if ( ml->loadType == ltype && ml->loadDir == ldir && ml->loadName == name ) {
            QPointF wa = ml->loadWAB.loadWA;
            QPointF wb = ml->loadWAB.loadWB;
            if ( qAbs(wa.y()) < 1.0e-3 && qAbs(wb.y()) < 1.0e-3 ) continue;
            if ( qAbs(wa.x() - wab.loadWA.x()) < 1.0e-3 && qAbs(wb.x() - wab.loadWB.x()) < 1.0e-3 ) {
                wa.setY( wa.y() + wab.loadWA.y() );
                wb.setY( wb.y() + wab.loadWB.y() );
                ml->loadWAB = WABLOAD(wa, wb);
                isDetected = true;
                break;
            }
        }
    }

    if ( !isDetected ) {
        MLOADSUM *ml = new MLOADSUM( name, ltype, ldir, QList<qreal>(), QList<QPointF>(), wab );
        globalMemberLoads.insert(md, ml);
    }
}



void CalculationLoad::appendLocalMemberLoad( MemberData *md, const QString &name, LoadType::LOADTYPE ltype,
                                             LOADDIR ldir, const QList<qreal> &load )
{
    bool isDetected = false;
    Q_FOREACH ( MLOADSUM *ml, localMemberLoads.values(md) ) {
        if ( ml->loadType == ltype && ml->loadDir == ldir && ml->loadName == name ) {
            QList<qreal> cload = ml->sumUpLoad;
            if ( cload.isEmpty() ) continue;
            for ( int i = 0; i < cload.count() ; i++ ) {
                qreal ll = cload.at(i) + load.at(i);
                cload.replace(i, ll);
            }
            ml->sumUpLoad = cload;
            isDetected = true;
            break;
        }
    }
    if ( !isDetected ) {
        MLOADSUM *ml = new MLOADSUM( name, ltype, ldir, load );
        localMemberLoads.insert(md, ml);
    }
}

void CalculationLoad::appendLocalMemberLoad( MemberData *md, const QString &name, LoadType::LOADTYPE ltype,
                                             LOADDIR ldir, const QList<QPointF> &load )
{
    bool isDetected = false;
    Q_FOREACH ( MLOADSUM *ml, localMemberLoads.values(md) ) {
        if ( ml->loadType == ltype && ml->loadDir == ldir && ml->loadName == name ) {
            QList<QPointF> cload = ml->concentratedLoad;
            if ( cload.isEmpty() ) continue;
            for ( int i = 0; i < load.count(); i++ ) {
                qreal xx = load.at(i).x();
                qreal yy = load.at(i).y();
                bool isOverap = false;
                for ( int j = 0; j < cload.count(); j++ ) {
                    if ( qAbs(xx - cload.at(j).x()) < 1.0e-2 ) {
                        qreal cy = yy + cload.at(j).y();
                        cload.replace(j, QPointF(cload.at(j).x(), cy));
                        isOverap = true;
                        break;
                    }
                }
                if ( !isOverap ) cload.append( load.at(i) );
            }
            ml->concentratedLoad = cload;
            isDetected = true;
            break;
        }
    }

    if ( !isDetected ) {
        MLOADSUM *ml = new MLOADSUM( name, ltype, ldir, QList<qreal>(), load );
        localMemberLoads.insert(md, ml);
    }
}

void CalculationLoad::appendLocalMemberLoad( MemberData *md, const QString &name, LoadType::LOADTYPE ltype,
                                             LOADDIR ldir, const WABLOAD &wab )
{
    bool isDetected = false;
    Q_FOREACH ( MLOADSUM *ml, localMemberLoads.values(md) ) {
        if ( ml->loadType == ltype && ml->loadDir == ldir && ml->loadName == name ) {
            QPointF wa = ml->loadWAB.loadWA;
            QPointF wb = ml->loadWAB.loadWB;
            if ( qAbs(wa.y()) < 1.0e-3 && qAbs(wb.y()) < 1.0e-3 ) continue;
            if ( qAbs(wa.x() - wab.loadWA.x()) < 1.0e-3 && qAbs(wb.x() - wab.loadWB.x()) < 1.0e-3 ) {
                wa.setY( wa.y() + wab.loadWA.y() );
                wb.setY( wb.y() + wab.loadWB.y() );
                ml->loadWAB = WABLOAD(wa, wb);
                isDetected = true;
                break;
            }
        }
    }

    if ( !isDetected ) {
        MLOADSUM *ml = new MLOADSUM( name, ltype, ldir, QList<qreal>(), QList<QPointF>(), wab );
        localMemberLoads.insert(md, ml);
    }
}

void CalculationLoad::appendJointLoad( JointData *jd, const QString &name, LoadType::LOADTYPE ltype,
                                       LOADDIR ldir, qreal load )
{
//    qDebug() << name << static_cast<int>(ldir) << load;
    bool isDetected = false;
    Q_FOREACH ( JLOADSUM *jl, globalJointLoads.values(jd) ) {
        if ( jl->loadType == ltype && jl->loadDir == ldir && jl->loadName == name ) {
            qreal cload = jl->concentratedLoad;
            jl->concentratedLoad = cload + load;
            isDetected = true;
            break;
        }
    }

    if ( !isDetected ) {
        JLOADSUM *jl = new JLOADSUM( name, ltype, ldir, load );
        globalJointLoads.insert(jd, jl);
    }
}

void CalculationLoad::appendSelfWeight( MemberData *md , QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads)
{
    DATATYPE dtype = md->dataType();

    if ( !md->isLineMember(false) ) return;

    if ( dtype == DATATYPE::TPDAMPER || dtype == DATATYPE::TPISO ) {
        SECTIONWEIGHT secw = UnifiedFixedData::getInstance()->getInputData()->sectionNameToUnitWeight(dtype, md->sectionName());
        qreal ww = secw.i_weight;
        appendJointLoad(md->iJoint(), QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, -ww / 2.0);
        appendJointLoad(md->jJoint(), QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, -ww / 2.0);
        return;
    }

    MEMBERPARTITION partition = md->getMemberPartition();
    qreal hh = md->getAttachedHeight(*UnifiedFixedData::getInstance()->getInputData());

    SECTIONWEIGHT secw = UnifiedFixedData::getInstance()->getInputData()->sectionNameToUnitWeight(md->dataType(),
                                                                             md->sectionName(), hh);
    STRUCTTYPE stp = UnifiedFixedData::getInstance()->getInputData()->sectionNameToStructureType(md->dataType(),
                                                                            md->sectionName(), hh);

    bool i_c = qAbs(secw.i_weight - secw.c_weight) < 1.0e-3;
    bool c_j = qAbs(secw.c_weight - secw.j_weight) < 1.0e-3;

    if ( i_c && c_j ) {
        qreal st_x1 = ( stp == STRUCTTYPE::STP_RC || stp == STRUCTTYPE::STP_SRC ) ? partition.lenFi : 0.0 ;
        qreal st_x2 = ( stp == STRUCTTYPE::STP_RC || stp == STRUCTTYPE::STP_SRC ) ? partition.lenFj : partition.lenSum ;
        WABLOAD wl( QPointF(st_x1 / 1.0e+3, -secw.i_weight), QPointF(st_x2 / 1.0e+3, -secw.i_weight) );
        appendMemberLoad(md, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, wl, globalMemberLoads);
    } else if ( i_c && !c_j ) {
        qreal st_x1 = ( stp == STRUCTTYPE::STP_RC || stp == STRUCTTYPE::STP_SRC ) ? partition.lenFi : 0.0 ;
        qreal st_x2 = partition.lenJ ;
        qreal st_x3 = ( stp == STRUCTTYPE::STP_RC || stp == STRUCTTYPE::STP_SRC ) ? partition.lenFj : partition.lenSum ;
        WABLOAD wl1( QPointF(st_x1 / 1.0e+3, -secw.i_weight), QPointF(st_x2 / 1.0e+3, -secw.i_weight) );
        WABLOAD wl2( QPointF(st_x2 / 1.0e+3, -secw.j_weight), QPointF(st_x3 / 1.0e+3, -secw.j_weight) );
        appendMemberLoad(md, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, wl1, globalMemberLoads);
        appendMemberLoad(md, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, wl2, globalMemberLoads);
    } else if ( !i_c && c_j ) {
        qreal st_x1 = ( stp == STRUCTTYPE::STP_RC || stp == STRUCTTYPE::STP_SRC ) ? partition.lenFi : 0.0 ;
        qreal st_x2 = partition.lenI ;
        qreal st_x3 = ( stp == STRUCTTYPE::STP_RC || stp == STRUCTTYPE::STP_SRC ) ? partition.lenFj : partition.lenSum ;
        WABLOAD wl1( QPointF(st_x1 / 1.0e+3, -secw.i_weight), QPointF(st_x2 / 1.0e+3, -secw.i_weight) );
        WABLOAD wl2( QPointF(st_x2 / 1.0e+3, -secw.j_weight), QPointF(st_x3 / 1.0e+3, -secw.j_weight) );
        appendMemberLoad(md, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, wl1, globalMemberLoads);
        appendMemberLoad(md, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, wl2, globalMemberLoads);
    } else if ( !i_c && !c_j ) {
        qreal st_x1 = ( stp == STRUCTTYPE::STP_RC || stp == STRUCTTYPE::STP_SRC ) ? partition.lenFi : 0.0 ;
        qreal st_x2 = partition.lenI ;
        qreal st_x3 = partition.lenJ ;
        qreal st_x4 = ( stp == STRUCTTYPE::STP_RC || stp == STRUCTTYPE::STP_SRC ) ? partition.lenFj : partition.lenSum ;
        WABLOAD wl1( QPointF(st_x1 / 1.0e+3, -secw.i_weight), QPointF(st_x2 / 1.0e+3, -secw.i_weight) );
        WABLOAD wl2( QPointF(st_x2 / 1.0e+3, -secw.c_weight), QPointF(st_x3 / 1.0e+3, -secw.c_weight) );
        WABLOAD wl3( QPointF(st_x3 / 1.0e+3, -secw.j_weight), QPointF(st_x4 / 1.0e+3, -secw.j_weight) );
        appendMemberLoad(md, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, wl1, globalMemberLoads);
        appendMemberLoad(md, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, wl2, globalMemberLoads);
        appendMemberLoad(md, QString(), LoadType::LTYPE_DL, LOADDIR::LDIR_Z, wl3, globalMemberLoads);
    }
}

QString CalculationLoad::jointInfo(JointData *jd) const
{
    QUuid fid = jd->getFirstFloor();
//    QList<QUuid> idlist=jd->getAttachedFloorList();
    QString str = ( fid.isNull() ) ? "(none)"
                  : UnifiedFixedData::getInstance()->getInputData()->idToName(fid, DATATYPE::TPFLOOR);
    QString err_msg = u8"所属階:" + str + ",";
    err_msg += u8"構成点:" + QString("%1").arg(UnifiedFixedData::getInstance()->getInputData()->indexOfJoint(jd) + 1);
    return err_msg;
}

QString CalculationLoad::memberInfo(MemberData *md) const
{
    QString err_msg = u8"符号:" + md->sectionName() + ",";
    QUuid fid = md->iJoint()->getFirstFloor();
    QString str = ( fid.isNull() ) ? "(none)"
                  : UnifiedFixedData::getInstance()->getInputData()->idToName(fid, DATATYPE::TPFLOOR);
    err_msg += u8"所属階:" + str + ",";
    QString sjd;
    Q_FOREACH ( JointData *jj, md->getJointList() ) {
        sjd += QString("%1-").arg( UnifiedFixedData::getInstance()->getInputData()->indexOfJoint(jj) + 1 );
    }
    sjd = sjd.mid(0, sjd.count() - 1);
    err_msg += u8"構成点:" + sjd;
    return err_msg;
}


} // namespace post3dapp
