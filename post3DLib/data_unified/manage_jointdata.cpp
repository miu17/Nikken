#include "manage_jointdata.h"

#include <QDebug>

#include "calc_vector2d.h"
#include "factory_unifieddata.h"
#include "fixed_data.h"
#include "manage_memberdata.h"
#include "unified_inputdata.h"
#include "version.h"

namespace post3dapp{
ManageJointData::ManageJointData(UnifiedDataType isout, QObject *parent)
    : QObject(parent), isOutput(isout)
{
}

ManageJointData::ManageJointData(const ManageJointData& joint, UnifiedDataType isout, QObject *parent)
    : QObject(parent), isOutput(isout)
{
    Q_FOREACH(auto j, joint.jointList){
        JointData *jd = new JointData(*j);
        jointList.append(jd);
        uuidTable.insert(jd->getUuid(), jd);
    }
}

void ManageJointData::clearData()
{
    Q_FOREACH (JointData *jd, jointList) delete jd;
    jointList.clear();
    uuidTable.clear();
}

bool ManageJointData::checkData( QDataStream &infile, const LibraryVersion &ver ) const
{
    if ( infile.atEnd() ) return false;
    int jcount;
    infile >> jcount;

    qDebug()<<"jcount="<<jcount;
    QUuid id;
    qreal xx, yy, zz;
    QList<QUuid> fl_list, fr_list;

    for (int i = 0; i < jcount ; i++) {
        if ( infile.atEnd() ) return false;
        infile >> id;
        if ( infile.atEnd() ) return false;
        infile >> xx;
        if ( infile.atEnd() ) return false;
        infile >> yy;
        if ( infile.atEnd() ) return false;
        infile >> zz;
        if ( infile.atEnd() ) return false;
        infile >> fl_list;
        if ( infile.atEnd() ) return false;
        infile >> fr_list;
    }
    return true;
}

void ManageJointData::readData( QDataStream &infile, const LibraryVersion &ver )
{
    int jcount;
    infile >> jcount;
    for (int i = 0; i < jcount ; i++) {
        JointData *jd = new JointData(infile, ver,isOutput);

        jointList.append(jd);
        uuidTable.insert(jd->getUuid(), jd);
    }
}
void ManageJointData::readData( QTextStream &infile, const LibraryVersion &ver )
{
    int jcount;
    jcount = infile.readLine(0).split(",").at(0).toInt();
    infile.readLine(0);

    for (int i = 0; i < jcount ; i++) {
        JointData *jd = new JointData(infile, ver,isOutput);

        qDebug()<<"before append jd";
        jointList.append(jd);
        qDebug()<<"after";
        uuidTable.insert(jd->getUuid(), jd);
        qDebug()<<"insert uuid";
    }
}

void ManageJointData::writeP3dData( QDataStream &outfile) const
{
    outfile << jointList.count();
    Q_FOREACH (JointData *jd, jointList){
        jd->writeP3dData(outfile);
    }
}


void ManageJointData::writePsvData(QTextStream &outfile) const
{
    outfile << jointList.count() << "\n";
    outfile << u8"UUID" << "\t" << u8"X" << "\t" << u8"Y" << "\t" << u8"Z" << "\n";
    Q_FOREACH (JointData *jd, jointList){
        jd->writePsvData(outfile);
    }
}

QString ManageJointData::writeNsvData(QSharedPointer<N3DUuidTable> table) const
{
    int jointCount = 0;
    QString tempJoint;
    Q_FOREACH (JointData *jd, jointList){
        if (!jd->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData()))
            continue;
        tempJoint.append(jd->writeNsvData(table));
        jointCount++;
    }
    QString text;
    text.append("*JOINT, ").append(QString::number(jointCount).append("\n"));
    text.append(u8"#ID, X(m), Y(m), Z(m), FLOOR数, FLOORID(FLOOR数繰り返し), FRAME数, FRAMEID(FRAME数繰り返し)\n");
    text.append(tempJoint);
    return text;
}

QHash<QUuid, QUuid> ManageJointData::readAdditionalData(QDataStream &infile, const LibraryVersion &ver,
                                                        const QHash<QUuid, QString> &fidToName,
                                                        const FILEREADPATTERN &fpt)
{
    Q_UNUSED(ver);
    int jcount;
    infile >> jcount;

    QHash<QUuid, QUuid> idToIds;
    QList<GlobalPoint> new_joints;
    QList<QUuid> new_jids;

    for (int i = 0; i < jcount ; i++) {

        QUuid jid;
        XYZ xyz;
        QList<QUuid> fllist, frlist;
        infile >> jid >> xyz.x >> xyz.y >> xyz.z >> fllist >> frlist;

        if ( uuidTable.contains(jid) ) {
            JointData *jd = uuidTable.value(jid);
            if ( fpt.priorList != fpt.priorJID ) {
                jd->setXYZ(xyz);
                QList<QUuid> o_fllist, o_frlist;
                Q_FOREACH ( QUuid uid, fllist ) {
                    QUuid o_uid =  FactoryUnifiedData::getInstance(isOutput)->nameToID( fidToName.value(uid), DATATYPE::TPFLOOR );
                    if ( !o_uid.isNull() ) o_fllist.append(o_uid);
                }
                Q_FOREACH ( QUuid uid, frlist ) {
                    QUuid o_uid =  FactoryUnifiedData::getInstance(isOutput)->nameToID( fidToName.value(uid), DATATYPE::TPFRAMEP );
                    if ( !o_uid.isNull() ) o_frlist.append(o_uid);
                }
                jd->replaceAttachedFloor(o_fllist);
                jd->replaceAttachedFrame(o_frlist);
            }
            if ( !fpt.addedID.isNull() ) jd->appendAttachedFloor(fpt.addedID);
            continue;
        }

        QList<QUuid> o_fllist, o_frlist;
        Q_FOREACH ( QUuid uid, fllist ) {
            QUuid o_uid =  FactoryUnifiedData::getInstance(isOutput)->nameToID( fidToName.value(uid), DATATYPE::TPFLOOR );
            if ( !o_uid.isNull() ) o_fllist.append(o_uid);
        }
        if ( !fpt.addedID.isNull() ) o_fllist.append(fpt.addedID);
        Q_FOREACH ( QUuid uid, frlist ) {
            QUuid o_uid =  FactoryUnifiedData::getInstance(isOutput)->nameToID( fidToName.value(uid), DATATYPE::TPFRAMEP );
            if ( !o_uid.isNull() ) o_frlist.append(o_uid);
        }

        GlobalPoint gp( xyz.x, xyz.y, xyz.z );
        gp.appendAttachedFloor(o_fllist);
        gp.appendAttachedFrame(o_frlist);
        new_joints.append(gp);
        new_jids.append(jid);
    }

    for ( int i = 0; i < new_joints.count(); i++ ) {

        GlobalPoint gp = new_joints.at(i);
        JointData *s_jd = nullptr;

        int maxj_index = 0;
        Q_FOREACH ( JointData *jd, jointList ) {
            qreal ss = 0.0;
            qreal vx = jd->xPos() - gp.x();
            qreal vy = jd->yPos() - gp.y();
            qreal vz = jd->zPos() - gp.z();
            ss += vx * vx;
            ss += vy * vy;
            ss += vz * vz;
            if ( qSqrt(ss) < 1.0e-3 ) {
                s_jd = jd;
                break;
            }
            if ( jd->getJointNo(UnifiedDataType::Input) > maxj_index){
                maxj_index = jd->getJointNo(UnifiedDataType::Input);
            }
        }

        if ( s_jd == nullptr ) { // 同一座標が見つからなかった場合
            JointData *jd = new JointData(gp, new_jids.at(i), isOutput);
            jointList.append(jd);
            uuidTable.insert(new_jids.at(i), jd);
        } else if ( fpt.priorJXYZ ==
                    FILEPRIOR::PRIOR_AB ) { // 同一座標が見つかって、重複させたままの場合
            JointData *jd = new JointData(gp, new_jids.at(i), isOutput);
            jointList.append(jd);
            uuidTable.insert(new_jids.at(i), jd);
        } else if ( fpt.priorList !=
                    fpt.priorJXYZ ) { // 同一座標が見つかって、追加ファイル優先の場合
            JointData *jd = new JointData(gp, new_jids.at(i), isOutput);
            jointList.append(jd);
            uuidTable.insert(new_jids.at(i), jd);
            Q_FOREACH ( MemberData *md, s_jd->getRelatedMembers() )
                md->replaceJoint(s_jd, jd);
            removeJoint(s_jd);
        } else { // 同一座標が見つかって、元ファイル優先の場合
            idToIds.insert( new_jids.at(i), s_jd->getUuid() );
            if ( !fpt.addedID.isNull() ) s_jd->appendAttachedFloor(fpt.addedID);
        }

    }

    return idToIds;
}


JointData *ManageJointData::createJoint(const GlobalPoint &gp, const QUuid &id)
{
    JointData *jd = new JointData(gp, id, isOutput);
    jointList.append(jd);
    uuidTable.insert(jd->getUuid(), jd);
    return jd;
}

JointData *ManageJointData::createJoint(const GlobalPoint &gp, const int jointName, const QUuid &id)
{
//    Q_FOREACH(JointData* jd, jointList)
//        if(jd->isNearlyPoint(gp,1.0e-5)) return jd;
    int maxj_index = 0;
    int jName = jointName;
    if(jointName == 0){
        Q_FOREACH(JointData* jd,jointList){
            int jindex = jd->getAnalysisJointNo();
            if(jindex > maxj_index)maxj_index = jindex;
        }
        jName = maxj_index + 1;
    }
    JointData *jd = new JointData(gp, jName, id, isOutput);
    jointList.append(jd);
    uuidTable.insert(jd->getUuid(), jd);
    return jd;
}

/*
JointData *ManageJointData::createJoint(const GlobalPoint &gp, const QUuid &id,
                                        const int &jointName)
{
//    Q_FOREACH(JointData* jd, jointList)
//        if(jd->isNearlyPoint(gp,1.0e-5)) return jd;
    int maxj_index = 0;
    int jName = jointName;
    if(jointName == 0){
        Q_FOREACH(JointData* jd,jointList){
            int jindex = jd->getAnalysisJointNo();
            if(jindex > maxj_index)maxj_index = jindex;
        }
        jName = maxj_index + 1;
    }
    JointData *jd = new JointData(gp, jName, id, isOutput);
    jointList.append(jd);
    uuidTable.insert(jd->getUuid(), jd);
    return jd;
}
*/

JointData *ManageJointData::nearlyJoint(const GlobalPoint &gp, qreal eps) const
{
    Q_FOREACH (JointData *jd, jointList)
        if (jd->isNearlyPoint(gp, eps)) return jd;
    return nullptr;
}

bool ManageJointData::isExistedJoint(JointData *jd)
{
    return jointList.contains(jd);
}

bool ManageJointData::isExistedID(const QUuid &id) const
{
    if ( id.isNull() ) return false;
    return uuidTable.contains(id);
}

int ManageJointData::indexOfJoint(JointData *jd) const
{
    return jointList.indexOf(jd);
}

JointData *ManageJointData::jointIndexToPointer(int idx) const
{
    return ( idx > jointList.count() - 1 || idx < 0 ) ? NULL : jointList.at(idx);
}

bool ManageJointData::removeJoint(JointData *jd)
{
    if (!jointList.contains(jd)) return false;
    jointList.removeOne(jd);
    uuidTable.remove(jd->getUuid());
    delete jd;
    return true;
}

bool ManageJointData::checkMovedPosition(const QList<JointData *> &move_jdlist,
                                         const QList<GlobalPoint> &move_gplist)
{
    Q_FOREACH (JointData *jd, jointList) {
        if (move_jdlist.contains(jd)) continue;
        Q_FOREACH (GlobalPoint gp, move_gplist)
            if (jd->isNearlyPoint(gp, 1.0e-5)) return false;
    }
    return true;
}

bool ManageJointData::changeJoint(JointData *jd, const GlobalPoint &gp)
{
    if (!jointList.contains(jd)) return false;
    jd->setXYZ(XYZ(gp.x(), gp.y(), gp.z()));
    jd->clearAttachedFloor();
    jd->clearAttachedFrame();
    jd->appendAttachedFloor(gp.getAttachedFloorList());
    jd->appendAttachedFrame(gp.getAttachedFrameList());
    return true;
}

QList<JointData *> ManageJointData::simplifiedMemberJoints( const QList<JointData *> &jdlist,
                                                            bool isClosed ) const
{
    QList<JointData *> s_joints;
    if ( isClosed ) {
        for ( int i = 0; i < jdlist.count(); i++ ) {
            JointData *j1 = ( i == 0 ) ? jdlist.last() : jdlist.at(i - 1);
            JointData *j2 = jdlist.at(i);
            JointData *j3 = ( i == jdlist.count() - 1 ) ? jdlist.first() : jdlist.at(i + 1);
            VEC3D v1( j1->xPos(), j1->yPos(), j1->zPos() );
            VEC3D v2( j2->xPos(), j2->yPos(), j2->zPos() );
            VEC3D v3( j3->xPos(), j3->yPos(), j3->zPos() );
            VEC3D q1 = CalcVector3D::minus(v2, v1);
            VEC3D q2 = CalcVector3D::minus(v3, v2);
            VEC3D vv = CalcVector3D::cross(q1, q2);
            if ( CalcVector3D::norm(vv) > 1.0e-3 ) s_joints.append(j2);
        }
    } else {
        s_joints.append(jdlist.first());
        for ( int i = 1; i < jdlist.count() - 1; i++ ) {
            JointData *j1 = jdlist.at(i - 1);
            JointData *j2 = jdlist.at(i);
            JointData *j3 = jdlist.at(i + 1);
            VEC3D v1( j1->xPos(), j1->yPos(), j1->zPos() );
            VEC3D v2( j2->xPos(), j2->yPos(), j2->zPos()  );
            VEC3D v3( j3->xPos(), j3->yPos(), j3->zPos()  );
            VEC3D q1 = CalcVector3D::minus(v2, v1);
            VEC3D q2 = CalcVector3D::minus(v3, v2);
            VEC3D vv = CalcVector3D::cross(q1, q2);
            if ( CalcVector3D::norm(vv) > 1.0e-3 ) s_joints.append(j2);
        }
        s_joints.append(jdlist.last());
    }
    return s_joints;
}

void ManageJointData::removeJointAttachments(DATATYPE dtype, const QList<QUuid> &idlist)
{
    if ( dtype == DATATYPE::TPFLOOR ) {
        Q_FOREACH ( JointData *jd, jointList ) jd->removeAttachedFloor(idlist);
    } else if ( dtype == DATATYPE::TPFRAMEP ) {
        Q_FOREACH ( JointData *jd, jointList ) jd->removeAttachedFrame(idlist);
    }
}

QList<JointData *> ManageJointData::jointListOfAll() const
{
    return jointList;
}

QList<JointData *> ManageJointData::jointListOfFloor(const QUuid &fl_id) const
{
    QList<JointData *> jlist;
    Q_FOREACH (JointData *jd, jointList)
        if (jd->containsFloor(fl_id)) jlist.append(jd);
    return jlist;
}

QList<JointData *> ManageJointData::jointListOfFloor(const QList<QUuid> &fl_idlist) const
{
    QList<JointData *> jlist;
    Q_FOREACH (JointData *jd, jointList)
        if (jd->containsFloor(fl_idlist)) jlist.append(jd);
    return jlist;
}

QList<JointData *> ManageJointData::jointListOfFrame(const QUuid &fr_id) const
{
    QList<JointData *> jlist;
    Q_FOREACH (JointData *jd, jointList)
        if (jd->containsFrame(fr_id)) jlist.append(jd);
    return jlist;
}

QList<JointData *> ManageJointData::jointListOfFrame(const QList<QUuid> &fr_idlist) const
{
    QList<JointData *> jlist;
    Q_FOREACH (JointData *jd, jointList)
        if (jd->containsFrame(fr_idlist)) jlist.append(jd);
    return jlist;
}

bool ManageJointData::replaceJointIndexes( const QList<JointData *> &jdlist )
{
    if ( jdlist.count() != jointList.count() ) return false;
    Q_FOREACH ( JointData *jd, jdlist ) {
        if ( !jointList.contains(jd) ) return false;
    }
    jointList = jdlist;
    return true;
}

QList<JointData *> ManageJointData::jointIDsToPointers(const QList<QUuid> &idlist) const
{
    QList<JointData *> jlist;
    Q_FOREACH (QUuid id, idlist) jlist.append( jointIDToPointer(id) );
    return jlist;
}

JointData *ManageJointData::jointIDToPointer( const QUuid &id ) const
{
    if ( id.isNull() ) return nullptr;
    return ( uuidTable.contains(id) ) ? uuidTable.value(id) : nullptr ;
}

void ManageJointData::calcCrossPoints(const QUuid &floorID,
                                      const QList<FRAMEPOINTSVALUE> &frameList,
                                      QList<GlobalPoint> &definePoints, QList<GlobalPoint> &crossPoints )
{
    /* 通りの定義点、既存の交点に対し X mm 以内の点であれば同一と見なす機能 後で追加  */
    qreal eps = 0.10; // 10cm 以内
    qreal hh =  FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(floorID);

    QHash< QUuid, QList<QPointF> > tmpLineTable;
    Q_FOREACH ( FRAMEPOINTSVALUE fp, frameList ) {
        QList<QPointF> plist = fp.pointList;
        if ( plist.count() < 2 ) continue;
        if ( fp.frameType == FrameType::FRAMETYPE::CLCIRCLE || fp.frameType == FrameType::FRAMETYPE::CLVARIABLE ) plist.append(plist.first());
        tmpLineTable.insert(fp.UUID, plist);

        Q_FOREACH (QPointF pp, plist) {
            int ii = indexOfGlobalPoint(definePoints, pp, 1.0e-5);
            if ( ii >= 0 ) {
                GlobalPoint gp = definePoints.at(ii);
                gp.appendAttachedFrame(fp.UUID);
                definePoints.replace(ii, gp);
            } else {
                GlobalPoint gp(pp.x(), pp.y(), hh);
                gp.setElvX(pp.x());
                gp.setElvY(pp.y());
                gp.appendAttachedFloor(floorID);
                gp.appendAttachedFrame(fp.UUID);
                definePoints.append(gp);
            }
        }
    }

    QList<QUuid> o_idlist;
    QList<QUuid> n_idlist = tmpLineTable.keys();

    Q_FOREACH (QUuid my_id, n_idlist) {
        if ( o_idlist.contains(my_id) ) continue;
        o_idlist.append(my_id);
        QList<QPointF> myplist = tmpLineTable.value(my_id);

        Q_FOREACH (QUuid p_id, n_idlist) {
            if ( o_idlist.contains(p_id) ) continue;
            QList<QPointF> o_plist = tmpLineTable.value(p_id);

            QList<QPointF> c_plist = crossPointList(myplist, o_plist);
            Q_FOREACH (QPointF pp, c_plist) {
                int ii = indexOfGlobalPoint(crossPoints, pp, eps);
                if ( ii >= 0 ) {
                    GlobalPoint gp = crossPoints.at(ii);
                    gp.appendAttachedFrame(my_id);
                    gp.appendAttachedFrame(p_id);
                    crossPoints.replace(ii, gp);
                } else {
                    int jj = indexOfGlobalPoint(definePoints, pp, eps);
                    if ( jj >= 0 ) {
                        GlobalPoint gp = definePoints.at(jj);
                        gp.appendAttachedFrame(my_id);
                        gp.appendAttachedFrame(p_id);
                        crossPoints.append(gp);
                    } else {
                        GlobalPoint gp(pp.x(), pp.y(), hh);
                        gp.setElvX(pp.x());
                        gp.setElvY(pp.y());
                        gp.appendAttachedFloor(floorID);
                        gp.appendAttachedFrame(my_id);
                        gp.appendAttachedFrame(p_id);
                        crossPoints.append(gp);
                    }
                }
            }
        }
    }
}

QList<QPointF> ManageJointData::crossPointList( const QList<QPointF> &a_list,
                                                const QList<QPointF> &b_list )
{
    return CalcVector2D::crossPointList(a_list, b_list);
}

int ManageJointData::indexOfGlobalPoint(const QList<GlobalPoint> &gplist, QPointF pp, qreal eps)
{
    int idx = -1;
    for (int i = 0; i < gplist.count(); i++) {
        if ( gplist.at(i).isNearlyPoint(pp, eps) ) {
            idx = i;
            break;
        }
    }
    return idx;
}

void ManageJointData::executeJointSort( const JOINTSORTTERM &term )
{
    QList<JointData *> sorted_joints;
    QMultiHash<JointData *, JointData *> level_joints; // 同一高さの節点

    Q_FOREACH ( JointData *jd, jointList ) {

        qreal hh = jd->zPos();
        int idx = -2;
        for ( int i = 0; i < sorted_joints.count(); i++ ) {
            JointData *j1 = sorted_joints.at(i);
            qreal h1 = j1->zPos();
            if ( qAbs(hh - h1) < 1.0e-5 ) {
                level_joints.insert(j1, jd);
                idx = -1;
                break;
            }
            if ( term.firstOrder ) {
                if ( hh + 1.0e-5 < h1 ) {
                    idx = i;
                    break;
                }
            } else {
                if ( h1 + 1.0e-5 < hh ) {
                    idx = i;
                    break;
                }
            }
        }

        if ( idx == -2 ) {
            sorted_joints.append(jd);
        } else if ( idx > -1 ) {
            sorted_joints.insert(idx, jd);
        }

    }

    Q_FOREACH ( JointData *k_jd, level_joints.uniqueKeys() ) {
        QList<JointData *> c_joints = level_joints.values(k_jd);
        QList<JointData *> s_joints;
        s_joints.append(k_jd);

        for ( int i = 0; i < c_joints.count(); i++ ) {
            JointData *c_jd = c_joints.at(i);
            int s_idx = -1;
            for ( int j = 0; j < s_joints.count(); j++ ) {
                JointData *s_jd = s_joints.at(j);
                if ( term.secondPattern == JOINTSORTPATTERN::SORTPT_X ) {
                    if ( qAbs(c_jd->xPos() - s_jd->xPos()) < 1.0e-5 ) {
                        if ( term.thirdPattern == JOINTSORTPATTERN::SORTPT_Y ) {
                            if ( term.thirdOrder && c_jd->yPos() + 1.0e-5 < s_jd->yPos() ) {
                                s_idx = j;
                                break;
                            } else if ( !term.thirdOrder && s_jd->yPos() + 1.0e-5 < c_jd->yPos() ) {
                                s_idx = j;
                                break;
                            }
                        } else if ( term.thirdPattern == JOINTSORTPATTERN::SORTPT_XY ) {
                            qreal c_xy = c_jd->xPos() + c_jd->yPos();
                            qreal s_xy = s_jd->xPos() + s_jd->yPos();
                            if ( term.thirdOrder && c_xy + 1.0e-5 < s_xy ) {
                                s_idx = j;
                                break;
                            } else if ( !term.thirdOrder && s_xy + 1.0e-5 < c_xy ) {
                                s_idx = j;
                                break;
                            }
                        }
                    }
                    if ( term.secondOrder && c_jd->xPos() + 1.0e-5 < s_jd->xPos() ) {
                        s_idx = j;
                        break;
                    } else if ( !term.secondOrder && s_jd->xPos() + 1.0e-5 < c_jd->xPos() ) {
                        s_idx = j;
                        break;
                    }
                } else if ( term.secondPattern == JOINTSORTPATTERN::SORTPT_Y ) {
                    if ( qAbs(c_jd->yPos() - s_jd->yPos()) < 1.0e-5 ) {
                        if ( term.thirdPattern == JOINTSORTPATTERN::SORTPT_X ) {
                            if ( term.thirdOrder && c_jd->xPos() + 1.0e-5 < s_jd->xPos() ) {
                                s_idx = j;
                                break;
                            } else if ( !term.thirdOrder && s_jd->xPos() + 1.0e-5 < c_jd->xPos() ) {
                                s_idx = j;
                                break;
                            }
                        } else if ( term.thirdPattern == JOINTSORTPATTERN::SORTPT_XY ) {
                            qreal c_xy = c_jd->xPos() + c_jd->yPos();
                            qreal s_xy = s_jd->xPos() + s_jd->yPos();
                            if ( term.thirdOrder && c_xy + 1.0e-5 < s_xy ) {
                                s_idx = j;
                                break;
                            } else if ( !term.thirdOrder && s_xy + 1.0e-5 < c_xy ) {
                                s_idx = j;
                                break;
                            }
                        }
                    }
                    if ( term.secondOrder && c_jd->yPos() + 1.0e-5 < s_jd->yPos() ) {
                        s_idx = j;
                        break;
                    } else if ( !term.secondOrder && s_jd->yPos() + 1.0e-5 < c_jd->yPos() ) {
                        s_idx = j;
                        break;
                    }
                } else if ( term.secondPattern == JOINTSORTPATTERN::SORTPT_XY ) {
                    qreal c_xy = c_jd->xPos() + c_jd->yPos();
                    qreal s_xy = s_jd->xPos() + s_jd->yPos();
                    if ( qAbs(c_xy - s_xy) < 1.0e-5 ) {
                        if ( term.thirdPattern == JOINTSORTPATTERN::SORTPT_X ) {
                            if ( term.thirdOrder && c_jd->xPos() + 1.0e-5 < s_jd->xPos() ) {
                                s_idx = j;
                                break;
                            } else if ( !term.thirdOrder && s_jd->xPos() + 1.0e-5 < c_jd->xPos() ) {
                                s_idx = j;
                                break;
                            }
                        } else if ( term.thirdPattern == JOINTSORTPATTERN::SORTPT_Y ) {
                            if ( term.thirdOrder && c_jd->yPos() + 1.0e-5 < s_jd->yPos() ) {
                                s_idx = j;
                                break;
                            } else if ( !term.thirdOrder && s_jd->yPos() + 1.0e-5 < c_jd->yPos() ) {
                                s_idx = j;
                                break;
                            }
                        }
                    }
                    if ( term.secondOrder && c_xy + 1.0e-5 < s_xy ) {
                        s_idx = j;
                        break;
                    } else if ( !term.secondOrder && s_xy + 1.0e-5 < c_xy ) {
                        s_idx = j;
                        break;
                    }
                } else {
                    if ( term.thirdPattern == JOINTSORTPATTERN::SORTPT_X ) {
                        if ( term.thirdOrder && c_jd->xPos() + 1.0e-5 < s_jd->xPos() ) {
                            s_idx = j;
                            break;
                        } else if ( !term.thirdOrder && s_jd->xPos() + 1.0e-5 < c_jd->xPos() ) {
                            s_idx = j;
                            break;
                        }
                    } else if ( term.thirdPattern == JOINTSORTPATTERN::SORTPT_Y ) {
                        if ( term.thirdOrder && c_jd->yPos() + 1.0e-5 < s_jd->yPos() ) {
                            s_idx = j;
                            break;
                        } else if ( !term.thirdOrder && s_jd->yPos() + 1.0e-5 < c_jd->yPos() ) {
                            s_idx = j;
                            break;
                        }
                    } else if ( term.thirdPattern == JOINTSORTPATTERN::SORTPT_XY ) {
                        qreal c_xy = c_jd->xPos() + c_jd->yPos();
                        qreal s_xy = s_jd->xPos() + s_jd->yPos();
                        if ( term.thirdOrder && c_xy + 1.0e-5 < s_xy ) {
                            s_idx = j;
                            break;
                        } else if ( !term.thirdOrder && s_xy + 1.0e-5 < c_xy ) {
                            s_idx = j;
                            break;
                        }
                    }
                }
            }
            if ( s_idx < 0 ) {
                s_joints.append(c_jd);
            } else {
                s_joints.insert(s_idx, c_jd);
            }
        }

        int p_idx = sorted_joints.indexOf(k_jd);
        QList<JointData *> b_jdlist = sorted_joints.mid(0, p_idx);
        QList<JointData *> p_jdlist = sorted_joints.mid(p_idx + 1);
        sorted_joints = b_jdlist + s_joints + p_jdlist;
    }

    jointList = sorted_joints;
}
QUuid ManageJointData::analysisJointNoToID(int no){
    Q_FOREACH(JointData* jd,jointList){
        if(jd->getAnalysisJointNo() == no)return jd->getUuid();
    }

    return QUuid();
}

///// JointData クラス //////

JointData::JointData(const JointData& jd){
    isOutput = UnifiedDataType::FixedInput;
    jointXYZ = XYZ(jd.xPos(), jd.yPos(), jd.zPos());
    attachedFloor = jd.getAttachedFloorList();
    attachedFrame = jd.getAttachedFrameList();
    jointID = jd.getUuid();
    analysisJointName = jd.analysisJointName;
}

JointData::JointData(const GlobalPoint &gp, const int &_jointName, const QUuid &id, UnifiedDataType isout)
{
    isOutput = isout;
    jointXYZ = XYZ(gp.x(), gp.y(), gp.z());
    attachedFloor = gp.getAttachedFloorList();
    attachedFrame = gp.getAttachedFrameList();
    jointID = ( id.isNull() ) ? QUuid::createUuid() : id ;
    analysisJointName = _jointName;
}

JointData::JointData(const GlobalPoint &gp, const QUuid &id, UnifiedDataType isout)
{
    isOutput = isout;
    jointXYZ = XYZ(gp.x(), gp.y(), gp.z());
    attachedFloor = gp.getAttachedFloorList();
    attachedFrame = gp.getAttachedFrameList();
    jointID = ( id.isNull() ) ? QUuid::createUuid() : id ;
}

JointData::JointData(QDataStream &infile, const LibraryVersion &ver, UnifiedDataType isout)
{
    isOutput = isout;
    infile >> jointID >> jointXYZ.x >> jointXYZ.y >> jointXYZ.z >> attachedFloor >>
           attachedFrame;
    return;
}
JointData::JointData( QTextStream &infile, const LibraryVersion &ver,UnifiedDataType isout)
{
    Q_UNUSED(ver);
    isOutput = isout;
    if (!infile.atEnd()) {
        UnifiedData* instance =  UnifiedInputData::getInInstance();

        QString str = infile.readLine(0);
        qDebug() << str;
        QStringList strlist = str.split("\t");
        jointID = QUuid(strlist.at(0)).isNull() ? QUuid::createUuid() : QUuid(strlist.at(0));
        jointXYZ = XYZ(strlist.at(1).toDouble(),strlist.at(2).toDouble(),strlist.at(3).toDouble());
        str = infile.readLine(0);
        qDebug() << str;
        strlist = str.split("\t");
        for(int i = 1; i <= strlist.at(0).toInt(); i++){
            qDebug()<<"floor loop";
            if(strlist.at(i).isEmpty())continue;
            attachedFloor.append(instance->nameToID(strlist.at(i),DATATYPE::TPFLOOR));
            qDebug()<<"attached floor";
        }
        str = infile.readLine(0);
        qDebug() << str;
        strlist = str.split("\t");
        for(int i = 1; i <= strlist.at(0).toInt(); i++){
            if(strlist.at(i).isEmpty())continue;
            attachedFrame.append(instance->nameToID(strlist.at(i),DATATYPE::TPFRAMEP));
        }

        return;
    }
}

JointData::~JointData()
{
    UnifiedInputData::getInInstance()->removeJointOfMember(this, interiorMemberList);
    UnifiedInputData::getInInstance()->removeJointOfMember(this, composeMemberList);
    foreach (JOINTSPRING *js, jointSpring) {
        jointSpring.removeAll(js);
    }
}

int JointData::getJointNo(UnifiedDataType type) const{
    return FactoryUnifiedData::getInstance(type)->indexOfJoint(const_cast<JointData*>(this)) + 1;
}

void JointData::writeP3dData(QDataStream &outfile) const
{
    outfile << jointID << jointXYZ.x << jointXYZ.y << jointXYZ.z << attachedFloor <<
               attachedFrame;
}

void JointData::writePsvData(QTextStream &outfile) const
{
    const QString tab = "\t";
    outfile << jointID.toString() << tab
            << QString::number(jointXYZ.x) << tab
            << QString::number(jointXYZ.y) << tab
            << QString::number(jointXYZ.z) << "\n";
    outfile << QString::number(attachedFloor.count()) << tab;
    foreach(auto floor, attachedFloor){
        outfile << FactoryUnifiedData::getInstance(isOutput)->idToName(floor, DATATYPE::TPFLOOR) << tab;
    }
    outfile << "\n";
    outfile << attachedFrame.count() << tab;
    foreach(auto frame, attachedFrame){
        outfile << FactoryUnifiedData::getInstance(isOutput)->idToName(frame, DATATYPE::TPFRAMEP) << tab;
    }
    outfile << "\n";
}

QString JointData::writeNsvData(QSharedPointer<N3DUuidTable> table) const{
    QString text;
    text.append(table->GetOrCreateN3dNumber(jointID.toString(), DATATYPE::TPJOINT)).append(",");
    text.append(QString::number(jointXYZ.x)).append(",");
    text.append(QString::number(jointXYZ.y)).append(",");
    text.append(QString::number(jointXYZ.z)).append(",");
    text.append(QString::number(attachedFloor.count())).append(",");
    if (attachedFloor.count() == 0){
        text.append(",");
    }
    else{
        for(int i = 0;i<attachedFloor.count();i++){
            text.append(table->GetOrCreateN3dNumber(attachedFloor.at(i), DATATYPE::TPFLOOR)).append(",");
        }
    }

    text.append(QString::number(attachedFrame.count())).append(",");
    if (attachedFrame.count() == 0){
    }
    else{
        for(int i = 0;i<attachedFrame.count();i++){
            text.append(table->GetOrCreateN3dNumber(attachedFrame.at(i), DATATYPE::TPFRAMEP));
            if (i != attachedFrame.count() - 1){
                text.append(",");
            }
        }
    }
    text.append("\n");
    return text;
}

XYZ JointData::xyz(bool adjusted ) const
{
    if ( !adjusted || attachedFloor.isEmpty() ) return jointXYZ;
    qreal delta =  FactoryUnifiedData::getInstance(isOutput)->getAdjustedFloorLevel( getFirstFloor() );
    return XYZ( jointXYZ.x, jointXYZ.y, jointXYZ.z + delta );
}

qreal JointData::zPos(bool adjusted ) const
{
    if ( !adjusted || attachedFloor.isEmpty() ) return jointXYZ.z;
    qreal delta =  FactoryUnifiedData::getInstance(isOutput)->getAdjustedFloorLevel( getFirstFloor() );
    return jointXYZ.z + delta;
}

GlobalPoint JointData::toGlobalPoint() const
{
    GlobalPoint gp(jointXYZ.x, jointXYZ.y, jointXYZ.z);
    gp.appendAttachedFloor(attachedFloor);
    gp.appendAttachedFrame(attachedFrame);
    return gp;
}

bool JointData::isNearlyPoint(const GlobalPoint &gp, qreal eps) const
{
    return ( qPow((jointXYZ.x - gp.x()), 2.0) + qPow((jointXYZ.y - gp.y()),
                                                     2.0) + qPow((jointXYZ.z - gp.z()), 2.0) < eps * eps );
}

QList<MemberData *> JointData::getCriticalMember() const
{
    JointData *jd = const_cast<JointData *>(this);
    QList<MemberData *> mdlist;
    Q_FOREACH (MemberData *md, composeMemberList) {
        if ( md->isCriticalJoint(jd) && !mdlist.contains(md) ) mdlist.append(md);
    }
    return mdlist;
}

void JointData::appendComposeMember(MemberData *md)
{
    if (!composeMemberList.contains(md)){
        composeMemberList.append(md);
    }
}

void JointData::appendInteriorMember(MemberData *md)
{
    if (!interiorMemberList.contains(md)) interiorMemberList.append(md);
}

void JointData::removeComposeMember(MemberData *md)
{
    if (composeMemberList.contains(md)) composeMemberList.removeOne(md);
}

void JointData::removeInteriorMember(MemberData *md)
{
    if (interiorMemberList.contains(md)) interiorMemberList.removeOne(md);
}

bool JointData::isInteriorJoint() const
{
    return ( !interiorMemberList.isEmpty() ) ;
}

bool JointData::isComposeJoint() const
{
    return ( !composeMemberList.isEmpty() ) ;
}

bool JointData::isIndependentJoint() const
{
    return ( interiorMemberList.isEmpty() && composeMemberList.isEmpty() ) ;
}

void JointData::interiorToCompose(MemberData *md)
{
    removeInteriorMember(md);
    appendComposeMember(md);
}

void JointData::composeToInterior(MemberData *md)
{
    removeComposeMember(md);
    appendInteriorMember(md);
}

void JointData::removeMemberOfJoint(MemberData *md)
{
    removeComposeMember(md);
    removeInteriorMember(md);
}

QList<JointData *> JointData::linkedJoints() const
{
    QList<JointData *> linked_jdlist;

    Q_FOREACH ( MemberData *md, composeMemberList + interiorMemberList ) {
        Q_FOREACH ( JointData *jd, md->aroundJoints(const_cast<JointData *>(this)) ) {
            if ( !linked_jdlist.contains(jd) ) linked_jdlist.append(jd);
        }
    }

    return linked_jdlist;
}

QList<JointData *> JointData::linkedJoints(const QList<DATATYPE> &typeList) const
{
    QList<JointData *> linked_jdlist;

    Q_FOREACH ( MemberData *md, composeMemberList + interiorMemberList ) {
        if ( !typeList.contains(md->dataType()) ) continue;
        Q_FOREACH ( JointData *jd, md->aroundJoints(const_cast<JointData *>(this)) ) {
            if ( !linked_jdlist.contains(jd) ) linked_jdlist.append(jd);
        }
    }

    return linked_jdlist;
}

QList<JointData *> JointData::linkedAnalysisJoints(const UnifiedData& unified) const
{
    QList<JointData *> linked_jdlist;

    Q_FOREACH ( MemberData *md, composeMemberList + interiorMemberList ) {
        Q_FOREACH ( JointData *jd, md->aroundAnalysisJoints(unified, const_cast<JointData *>(this)) ) {
            if ( !linked_jdlist.contains(jd) ) linked_jdlist.append(jd);
        }
    }

    return linked_jdlist;
}


QList<JointData *> JointData::relatedJoints() const
{
    QList<JointData *> related_jdlist;

    Q_FOREACH ( MemberData *md, composeMemberList + interiorMemberList ) {
        Q_FOREACH ( JointData *jd, md->getJointList() ) {
            if ( jd == const_cast<JointData *>(this) ) continue;
            if ( !related_jdlist.contains(jd) ) related_jdlist.append(jd);
        }
    }

    return related_jdlist;
}

QList<JointData *> JointData::relatedJoints(const QList<DATATYPE> &typeList) const
{
    QList<JointData *> related_jdlist;

    Q_FOREACH ( MemberData *md, composeMemberList + interiorMemberList ) {
        if ( !typeList.contains(md->dataType()) ) continue;
        Q_FOREACH ( JointData *jd, md->getJointList() ) {
            if ( jd == const_cast<JointData *>(this) ) continue;
            if ( !related_jdlist.contains(jd) ) related_jdlist.append(jd);
        }
    }

    return related_jdlist;
}

bool JointData::isLoadJoint() const{
    Q_FOREACH(MemberData* md, this->getComposeMembers()){
        if (md->dataType() != DATATYPE::TPJOINTLOAD &&
            md->dataType() != DATATYPE::TPMEMBERLOAD &&
            md->dataType() != DATATYPE::TPSLABLOAD){
            return false;
        }
    }
    return true;
}


// 解析節点として残る点か否か
bool JointData::isAnalysisJoint(const UnifiedData& unified) const
{
    if (this->isIndependentJoint())
        return false;
    if (this->isLoadJoint())
        return false;

    // 解析実行時にエラーで変な値が入ってこない前提
    Q_FOREACH ( MemberData *mm, composeMemberList ) {
        if ( mm->isAnalysisLineMember() ) {
            return true;  // 構成点とする解析線材の場合
        } else if ( mm->isAnalysisClosedMember(unified) && mm->isCornerJoint(this)) {
            return true;
        }
    }

    int main_count = 0;
    Q_FOREACH ( MemberData *mm, interiorMemberList ) {
        if ( mm->isAnalysisLineMember() ) {
            if (mm->isCornerJoint(this))
                return true;

            main_count++;
            if ( main_count > 1 ) {
                return true;    // 従属する主要部材が2つ以上ある場合
            }
        }
    }

    return false;
}

QList<QUuid> JointData::getAttachedFloorList() const
{
    return attachedFloor;
}

QList<QUuid> JointData::getAttachedFrameList() const
{
    return attachedFrame;
}

QUuid JointData::getFirstFloor() const
{
    QUuid a_uid;
    qreal a_hh =  FactoryUnifiedData::getInstance(isOutput)->getMaximumFloorHeight();
    Q_FOREACH ( QUuid uid, attachedFloor ) {
        qreal hh =  FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(uid);
        if ( hh < a_hh + 1.0e-4 ) {
            a_hh = hh;
            a_uid = uid;
        }
    }
    return a_uid;
}

void JointData::appendAttachedFloor(const QUuid &id)
{
    if (!attachedFloor.contains(id)) attachedFloor.append(id);
}

void JointData::appendAttachedFloor(const QList<QUuid> &idlist)
{
    Q_FOREACH (QUuid id, idlist) {
        if (!attachedFloor.contains(id)) attachedFloor.append(id);
    }
}

void JointData::appendAttachedFrame(const QUuid &id)
{
    if (!attachedFrame.contains(id)) attachedFrame.append(id);
}

void JointData::appendAttachedFrame(const QList<QUuid> &idlist)
{
    Q_FOREACH (QUuid id, idlist) {
        if (!attachedFrame.contains(id)) attachedFrame.append(id);
    }
}

void JointData::removeAttachedFloor(const QUuid &id)
{
    if (attachedFloor.contains(id)) attachedFloor.removeOne(id);
}

void JointData::removeAttachedFloor(const QList<QUuid> &idlist)
{
    Q_FOREACH (QUuid id, idlist) {
        if (attachedFloor.contains(id)) attachedFloor.removeOne(id);
    }
}

void JointData::removeAttachedFrame(const QUuid &id)
{
    if (attachedFrame.contains(id)) attachedFrame.removeOne(id);
}

void JointData::removeAttachedFrame(const QList<QUuid> &idlist)
{
    Q_FOREACH (QUuid id, idlist) {
        if (attachedFrame.contains(id)) attachedFrame.removeOne(id);
    }
}

void JointData::replaceAttachedFloor(const QList<QUuid> &idlist)
{
    attachedFloor = idlist;
}

void JointData::replaceAttachedFrame(const QList<QUuid> &idlist)
{
    attachedFrame = idlist;
}

void JointData::clearAttachedFloor()
{
    attachedFloor.clear();
}

void JointData::clearAttachedFrame()
{
    attachedFrame.clear();
}

bool JointData::containsFloor(const QUuid &id) const
{
    return attachedFloor.contains(id);
}

bool JointData::containsFloor(const QList<QUuid> &idlist) const
{
    bool ok = false;
    Q_FOREACH (QUuid id, idlist) {
        if (attachedFloor.contains(id)) {
            ok = true;
            break;
        }
    }
    return ok;
}

bool JointData::containsFrame(const QUuid &id) const
{
    return attachedFrame.contains(id);
}

bool JointData::containsFrame(const QList<QUuid> &idlist) const
{
    bool ok = false;
    Q_FOREACH (QUuid id, idlist) {
        if (attachedFrame.contains(id)) {
            ok = true;
            break;
        }
    }
    return ok;
}

void JointData::clearNotFoundFloor()
{
    for (int i = attachedFloor.count() - 1; i >= 0 ; i--) {
        if (! FactoryUnifiedData::getInstance(isOutput)->isExistedID(attachedFloor.at(i), DATATYPE::TPFLOOR))
            attachedFloor.removeAt(i);
    }
}

void JointData::clearNotFoundFrame()
{
    for (int i = attachedFrame.count() - 1; i >= 0 ; i--) {
        if (! FactoryUnifiedData::getInstance(isOutput)->isExistedID(attachedFrame.at(i), DATATYPE::TPFRAMEP))
            attachedFrame.removeAt(i);
    }
}

} // namespace post3dapp
