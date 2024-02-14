#include "persUI_scene.h"

#include <QDebug>

#include "calc_vector2d.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_inputdata.h"
#include "unified_settings.h"

namespace post3dapp{
PersInterfaceScene::PersInterfaceScene(ELEMENTTYPE elementType, ADDEDITMODE mode, const Input3DViewSettings& setting, QObject *parent)
    : QObject(parent), viewSettings(setting)
{
    myElementType = elementType;
    currentAddEditMode = mode;
//    oneTime_useNoDisplayList = false;

    connect (  UnifiedInputData::getInInstance(), &UnifiedData::dataCleared,
              this, &PersInterfaceScene::clearCurrentData );
    connect (  UnifiedInputData::getInInstance(), &UnifiedData::dataReaded,
              this, &PersInterfaceScene::initDisplayList );

    connect( UnifiedInputData::getInInstance(), &UnifiedData::jointAppended,
            this, &PersInterfaceScene::initDisplayList);
    connect( UnifiedInputData::getInInstance(), &UnifiedData::jointRemoved,
            this, &PersInterfaceScene::initDisplayList);
    connect( UnifiedInputData::getInInstance(), &UnifiedData::jointChanged,
            this, &PersInterfaceScene::initDisplayList);

    connect(UnifiedInputData::getInInstance(), &UnifiedInputData::memberAppended,
            this, &PersInterfaceScene::initDisplayList);
    connect( UnifiedInputData::getInInstance(), &UnifiedData::memberRemoved,
            this, &PersInterfaceScene::initDisplayList);
    connect( UnifiedInputData::getInInstance(), &UnifiedData::memberChanged,
            this, &PersInterfaceScene::initDisplayList);

    connect(UnifiedSettings::getInstance(), &UnifiedSettings::persLimitedChanged,
            this, &PersInterfaceScene::initDisplayList);
    connect(UnifiedSettings::getInstance(), &UnifiedSettings::in3D_SettingChanged,
            this, &PersInterfaceScene::updateViewSettings);

}

PersInterfaceScene::~PersInterfaceScene()
{
    clearCurrentData();
}
void PersInterfaceScene::updateViewSettings()
{
    viewSettings = UnifiedSettings::getInstance()->getInput3DViewSettings();
    initDisplayList();
}
void PersInterfaceScene::importCurrentStatus(ELEMENTTYPE el_type, ADDEDITMODE ad_mode)
{
    if ( ad_mode != EDITMODE || el_type != myElementType || ad_mode != currentAddEditMode ) {
        clearSelectedItems();
        initDisplayList();
    }
    myElementType = el_type;
    currentAddEditMode = ad_mode;
}

void PersInterfaceScene::appendSelectedJoint(JointData *jd)
{
    if (!selectedJoints.contains(jd))
        selectedJoints.append(jd);
}

void PersInterfaceScene::removeSelectedJoint(JointData *jd)
{
    if (selectedJoints.contains(jd))
        selectedJoints.removeOne(jd);
}

void PersInterfaceScene::appendSelectedMember(MemberData *md)
{
    if (!selectedMembers.contains(md))
        selectedMembers.append(md);
}

void PersInterfaceScene::initializeSelectItems()
{
    clearSelectedItems();
    initDisplayList();
}

void PersInterfaceScene::clearCurrentData()
{
    Q_FOREACH ( GLuint id, memberDisplayList ) {
        if ( glIsList(id) ) glDeleteLists(id, 1);
    }
    selectedMembers.clear();
    selectedJoints.clear();
    closedMemberTriangles.clear();
    memberStates.clear();
    memberDisplayList.clear();
    memberIndexList.clear();
    memberList.clear();
    jointList.clear();    
}

bool PersInterfaceScene::isNotInitialized() const
{
    return memberDisplayList.isEmpty();
}

void PersInterfaceScene::initDisplayList()
{
    qDebug()<<"initDisplayList()";
    clearCurrentData();

    createMemberItems(  UnifiedInputData::getInInstance()->memberListOfAll() );

    if ( viewSettings.getElementView( ELJOINT).isVisible )
        createJointItems(  UnifiedInputData::getInInstance()->jointListOfAll() );

    calcBoundingVec();
    emit dataUpdated();
}

bool PersInterfaceScene::isMemberOkToDisplayLimited( MemberData *md ) const
{
    bool fl_ok = true;
    if ( viewSettings.getFloorLimitedState() == ELCONTAIN ) {
        fl_ok = md->containsFloors( viewSettings.getLimitedFloors() );
    } else if (viewSettings.getFloorLimitedState() == ELTOUCH ) {
        fl_ok = md->touchFloors( viewSettings.getLimitedFloors() );
    }

    bool fr_ok = true;
    if ( viewSettings.getFrameLimitedState() == ELCONTAIN ) {
        fr_ok = md->containsFrames( viewSettings.getLimitedFrames() );
    } else if (viewSettings.getFrameLimitedState() == ELTOUCH ) {
        fr_ok = md->touchFrames( viewSettings.getLimitedFrames() );
    }

    return ( fl_ok && fr_ok ) ;
}

bool PersInterfaceScene::isJointOkToDisplayLimited( JointData *jd ) const
{
    bool fl_ok = true;
    if ( viewSettings.getFloorLimitedState() != ELUNLIMITED ) {
        fl_ok = ( jd->containsFloor( viewSettings.getLimitedFloors() ) );
    }

    bool fr_ok = true;
    if ( viewSettings.getFrameLimitedState() != ELUNLIMITED ) {
        fr_ok = ( jd->containsFrame( viewSettings.getLimitedFrames() ) );
    }

    return ( fl_ok && fr_ok ) ;
}


void PersInterfaceScene::createJointItems( const QList<JointData *> &jdlist )
{
//    qDebug() << "createJointItems" << jdlist.count();
    Q_FOREACH ( JointData *jd, jdlist ) {
        if ( !isJointOkToDisplayLimited(jd) ) continue;
        jointList.append(jd);
    }
    calcBoundingVec();
}

void PersInterfaceScene::removeJointItems( const QList<JointData *> &jdlist )
{
//    qDebug() << "removeJointItems" << jdlist.count();
    clearSelectedItems();
    Q_FOREACH ( JointData *jd, jdlist ) {
        int idx = jointList.indexOf(jd);
        if ( idx >= 0 ) jointList.removeAt(idx);
    }
    calcBoundingVec();
}

void PersInterfaceScene::changeJointItems( const QList<JointData *> &jdlist )
{
//    qDebug() << "changeJointItems" << jdlist.count();
    Q_FOREACH ( JointData *jd, jdlist ) {
        int idx = jointList.indexOf(jd);
        bool isOk = isJointOkToDisplayLimited(jd);
        if ( idx >= 0 ) {
            if ( !isOk ) jointList.removeAt(idx);
        } else {
            if ( isOk ) jointList.append(jd);
        }
    }
    calcBoundingVec();
}

void PersInterfaceScene::createMemberItems( const QList<MemberData *> &mdlist)
{
//    qDebug() << "createMemberItems" << mdlist.count();
    Q_FOREACH ( MemberData *md, mdlist ) {

//        if(mdlist.count()<100)  qDebug() << md;
//        if(mdlist.count()<100)  qDebug() << md->sectionName();
        ELEMENTTYPE etype = dataTypeToElementType( md->dataType() );
        if ( !viewSettings.getElementView( etype).isVisible ) continue;

        if ( !isMemberOkToDisplayLimited(md) ) continue;

        if ( md->isLineMember(false) ) {

            HLINESHAPE shape = md->getHLineShape();
            qreal angle = md->codeAngle();
            qreal h0, hh;
            if ( shape.renderType == RENDERSHAPE::SHAPE_I || shape.renderType == RENDERSHAPE::SHAPE_T2 ) {
                h0 = shape.leftSizeB / 2.0;
                hh = shape.leftSizeH / 2.0 * qAbs(qSin(angle)) + shape.leftSizeB / 2.0 * qAbs(qCos(angle));
            } else if ( shape.renderType == RENDERSHAPE::SHAPE_PIPE ) {
                h0 = shape.leftSizeH / 2.0;
                hh = shape.leftSizeH / 2.0;
            } else {
                h0 = shape.leftSizeH / 2.0;
                hh = shape.leftSizeB / 2.0 * qAbs(qSin(angle)) + shape.leftSizeH / 2.0 * qAbs(qCos(angle));
            }
//            qreal hh=shape.leftSizeB/2.0*qAbs( qSin(angle) )
//                     +shape.leftSizeH/2.0*qAbs( qCos(angle) );
            qreal shy = md->getShiftY();
            qreal shz = md->getShiftZ();
            QList<JointData *> jdlist = md->getJointList();

            int p_idx = 0;
            int c_idx = 1;
            int n_idx = c_idx + 1;
            XYZ p_xyz = jdlist.first()->xyz();
            XYZ c_xyz = jdlist.at(1)->xyz();
            qglviewer::Vec pV( c_xyz.x - p_xyz.x, c_xyz.y - p_xyz.y, c_xyz.z - p_xyz.z );

            while ( n_idx < jdlist.count() ) {

                XYZ n_xyz = jdlist.at(n_idx)->xyz();
                qglviewer::Vec nV( n_xyz.x - c_xyz.x, n_xyz.y - c_xyz.y, n_xyz.z - c_xyz.z );
                qglviewer::Vec vv = pV ^ nV;

                if ( vv.norm() < 0.01 ) {
                    c_xyz = n_xyz;
                    c_idx = n_idx;
                    n_idx++;
                    continue;
                }

                GLuint id = glGenLists(1);

                memberIndexList.append( INDEXPAIR(p_idx, c_idx) );
                memberList.append( md );
                memberDisplayList.append( id );

                glNewList(id, GL_COMPILE);

                bool isVertical=false;
                if(qAbs(n_xyz.x - c_xyz.x)+qAbs(n_xyz.y - c_xyz.y)<0.0001)isVertical=true;

                setLocalMatrix(p_xyz.x * 1000.0, p_xyz.y * 1000.0, p_xyz.z * 1000.0,
                               c_xyz.x * 1000.0, c_xyz.y * 1000.0, c_xyz.z * 1000.0,
                               h0, hh, shy, shz, angle * 180. / M_PI, md->dataType(),isVertical);
                glEndList();

                pV = nV;
                p_idx = c_idx;
                p_xyz = c_xyz;
                c_idx = n_idx;
                c_xyz = n_xyz;
                n_idx++;

            }

            GLuint id = glGenLists(1);

            memberIndexList.append( INDEXPAIR(p_idx, c_idx) );
            memberList.append( md );
            memberDisplayList.append( id );

            glNewList(id, GL_COMPILE);
            bool isVertical=false;
            if(qAbs(p_xyz.x - c_xyz.x)+qAbs(p_xyz.y - c_xyz.y)<0.0001)isVertical=true;
            setLocalMatrix(p_xyz.x * 1000.0, p_xyz.y * 1000.0, p_xyz.z * 1000.0,
                           c_xyz.x * 1000.0, c_xyz.y * 1000.0, c_xyz.z * 1000.0,
                           h0, hh, shy, shz, angle * 180. / M_PI, md->dataType(),isVertical);
            glEndList();

            memberStates.insert(md, checkLineMemberState(md));
        }


        if ( md->isClosedMember(false) ) {

            GLuint id = glGenLists(1);

            memberIndexList.append( INDEXPAIR(0, md->jointCount() - 1) );
            memberList.append( md );
            memberDisplayList.append( id );

            memberStates.insert(md, checkClosedMemberState(md));
            closedMemberTriangles.insert(md, divideClosedMember(md));
        }
    }
}

ELEMENTSTATE PersInterfaceScene::checkLineMemberState( MemberData *md ) const
{
    if ( md->sectionName().isEmpty() ) return ELDUMMY;
    if ( md->getHLineShape().leftSizeB < 1.0e-3 &&
            md->getHLineShape().leftSizeH < 1.0e-3 ) {
        return ELERROR;
    }
    if ( md->getIFaceLength() > md->getJFaceLength() ) {
        return ELERROR;
    }
    return ELNORMAL;
}

ELEMENTSTATE PersInterfaceScene::checkClosedMemberState( MemberData *md ) const
{
    if ( md->sectionName().isEmpty() ) return ELDUMMY;

    if ( md->getPlaneThickness() < 1.0e-3 ) return ELERROR;

    QList<JointData *> jdlist = md->getJointList();

    bool isOnPlane = false;
    qglviewer::Vec a1( jdlist.first()->xPos(), jdlist.first()->yPos(), jdlist.first()->zPos() );
    qglviewer::Vec vv(0.0, 0.0, 0.0);
    for ( int i = 1; i < jdlist.count() - 1; i++ ) {
        qglviewer::Vec a2( jdlist.at(i)->xPos(), jdlist.at(i)->yPos(), jdlist.at(i)->zPos() );
        qglviewer::Vec a3( jdlist.at(i + 1)->xPos(), jdlist.at(i + 1)->yPos(), jdlist.at(i + 1)->zPos() );
        vv = (a2 - a1) ^ (a3 - a1);
        if ( vv.norm() > 1.0e-3 ) {
            isOnPlane = true;
            break;
        }
    }
    if ( !isOnPlane ) return ELERROR;
    for (int i = 1; i < jdlist.count(); i++) {
        qglviewer::Vec aa( jdlist.at(i)->xPos(), jdlist.at(i)->yPos(), jdlist.at(i)->zPos() );
        if ( qAbs( (aa - a1)*vv ) > 1.0e-3 ) {
            isOnPlane = false;
            break;
        }
    }
    if ( !isOnPlane ) return ELERROR;

    QList<QPointF> plist;
    if ( qAbs(vv.x) > qAbs(vv.y) && qAbs(vv.x) > qAbs(vv.z) ) {
        Q_FOREACH ( JointData *jd, jdlist ) plist.append( QPointF(jd->yPos(), jd->zPos()) );
    } else if ( qAbs(vv.y) > qAbs(vv.x) && qAbs(vv.y) > qAbs(vv.z) ) {
        Q_FOREACH ( JointData *jd, jdlist ) plist.append( QPointF(jd->xPos(), jd->zPos()) );
    } else {
        Q_FOREACH ( JointData *jd, jdlist ) plist.append( QPointF(jd->xPos(), jd->yPos()) );
    }

    for ( int i = 0; i < plist.count() - 1; i++ ) {
        QLineF l1 = QLineF(plist.at(i), plist.at(i + 1));
        for ( int j = i + 1; j < plist.count(); j++ ) {
            QLineF l2 = ( j == plist.count() - 1 ) ? QLineF( plist.at(j), plist.first() )
                        : QLineF( plist.at(j), plist.at(j + 1) );
            if ( CalcVector2D::checkSegmentIntersection(l1, l2) ) return ELERROR;
        }
    }
    return ELNORMAL;
}

QList<qglviewer::Vec> PersInterfaceScene::divideClosedMember( MemberData *md ) const
{
    QList<qglviewer::Vec> tmpPoints;
    Q_FOREACH ( JointData *jd, md->getJointList() ) {
//        tmpPoints.append( Vec( jd->xPos()*1000.0,jd->yPos()*1000.0,jd->zPos()*1000.0 ) );
        qglviewer::Vec v1( jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos() * 1000.0 );
        qglviewer::Vec v2( md->getWVector().x * md->getShiftZ(),
                md->getWVector().y * md->getShiftZ(),
                md->getWVector().z * md->getShiftZ() );
        tmpPoints.append( v1 + v2 );
    }
    QList<qglviewer::Vec> dividedPoints;

    int cur_idx, pre_idx, next_idx;
    qglviewer::Vec v0, v1, v2, vc;

    while ( tmpPoints.count() > 3 ) {

        qreal max_len = 0.0;
        for ( int i = 0; i < tmpPoints.count(); i++ ) {
            qreal len = qAbs( tmpPoints.at(i).norm() );
            if ( len > max_len  ) {
                cur_idx = i;
                max_len = len;
            }
        }

        pre_idx = ( cur_idx > 0 ) ? cur_idx - 1 : tmpPoints.count() - 1 ;
        next_idx = ( cur_idx < tmpPoints.count() - 1 ) ? cur_idx + 1 : 0 ;

        v0 = tmpPoints.at(pre_idx);
        v1 = tmpPoints.at(cur_idx);
        v2 = tmpPoints.at(next_idx);

        bool isInside = false;
        for ( int i = 0; i < tmpPoints.count(); i++ ) {
            if ( i == pre_idx || i == cur_idx || i == next_idx ) continue;
            qglviewer::Vec vv = tmpPoints.at(i);
            qglviewer::Vec a1 = (v1 - v0) ^ (v0 - vv);
            qglviewer::Vec a2 = (v2 - v1) ^ (v1 - vv);
            qglviewer::Vec a3 = (v0 - v2) ^ (v2 - vv);
            if ( ( a1 * a2 > 0.0001 ) && ( a1 * a3 > 0.0001 ) ) {
                isInside = true;
                break;
            }
        }

        if ( !isInside ) {
            dividedPoints.append(v0);
            dividedPoints.append(v1);
            dividedPoints.append(v2);
            tmpPoints.removeAt(cur_idx);
            continue;
        }

        vc = (v0 - v1) ^ (v2 - v1);
        int start_id = cur_idx;

        while ( 1 ) {

            if ( start_id == next_idx ) break;

            cur_idx = next_idx;
            pre_idx = ( cur_idx > 0 ) ? cur_idx - 1 : tmpPoints.count() - 1 ;
            next_idx = ( cur_idx < tmpPoints.count() - 1 ) ? cur_idx + 1 : 0 ;

            v0 = tmpPoints.at(pre_idx);
            v1 = tmpPoints.at(cur_idx);
            v2 = tmpPoints.at(next_idx);

            qglviewer::Vec cur_vc = (v0 - v1) ^ (v2 - v1);
            if ( vc * cur_vc > -1.0e-3 ) {

                isInside = false;
                for ( int i = 0; i < tmpPoints.count(); i++ ) {
                    if ( i == pre_idx || i == cur_idx || i == next_idx ) continue;
                    qglviewer::Vec vv = tmpPoints.at(i);
                    qglviewer::Vec a1 = (v1 - v0) ^ (v0 - vv);
                    qglviewer::Vec a2 = (v2 - v1) ^ (v1 - vv);
                    qglviewer::Vec a3 = (v0 - v2) ^ (v2 - vv);
                    if ( ( a1 * a2 > 0.0001 ) && ( a1 * a3 > 0.0001 ) ) {
                        isInside = true;
                        break;
                    }
                }

                if ( !isInside ) {
                    dividedPoints.append(v0);
                    dividedPoints.append(v1);
                    dividedPoints.append(v2);
                    tmpPoints.removeAt(cur_idx);
                    break;
                }

            }

        }

    }

    Q_FOREACH ( qglviewer::Vec vv, tmpPoints ) dividedPoints.append(vv);

    return dividedPoints;
}

void PersInterfaceScene::removeMemberItems( const QList<MemberData *> &mdlist, DATATYPE dtype)
{
    if ( dtype == DATATYPE::TPJOINTLOAD || dtype == DATATYPE::TPMEMBERLOAD || dtype == DATATYPE::TPSLABLOAD )
        return;
    clearSelectedItems();
    Q_FOREACH ( MemberData *md, mdlist ) {
        int count = memberList.count(md);
        if ( count < 1 ) continue;

        removeSelectedMember(md);
        for ( int i = 0; i < count; i++ ) {
            int idx = memberList.indexOf(md);
            memberList.removeAt(idx);
            memberIndexList.removeAt(idx);
            GLuint id = memberDisplayList.at(idx);
            if ( glIsList(id) ) glDeleteLists(id, 1);
            memberDisplayList.removeAt(idx);
        }
        memberStates.remove(md);
        if ( dtype != DATATYPE::TPWALL && dtype != DATATYPE::TPSLAB && dtype != DATATYPE::TPUWALL ) continue;
        closedMemberTriangles.remove(md);
    }
}

void PersInterfaceScene::changeMemberItems( const QList<MemberData *> &mdlist )
{
//    qDebug() << "changeMemberItems" << mdlist.count();
    QMultiHash<int, MemberData *> removed_mdtable;
    Q_FOREACH (MemberData *md, mdlist) {
        DATATYPE dtype = md->dataType();
        if ( dtype == DATATYPE::TPJOINTLOAD || dtype == DATATYPE::TPMEMBERLOAD || dtype == DATATYPE::TPSLABLOAD ) continue;
        removed_mdtable.insert(static_cast<int>(dtype), md);
    }

    Q_FOREACH ( int type, removed_mdtable.uniqueKeys() ) {
        removeMemberItems( removed_mdtable.values(type), static_cast<DATATYPE>(type) );
        createMemberItems( removed_mdtable.values(type));
    }
}

void PersInterfaceScene::calcBoundingVec()
{
    qreal x1 = 1000000., y1 = 1000000., z1 = 1000000.;
    qreal x2 = -1000000., y2 = -1000000., z2 = -1000000.;

    if ( jointList.isEmpty() ) {
        x1 = -5000.;
        y1 = -5000.;
        z1 = -5000.;
        x2 = 15000.;
        y2 = 15000.;
        z2 = 15000.;
    } else {
        Q_FOREACH (JointData *jd, jointList) {
            x1 = qMin(x1, jd->xPos() * 1000.0);
            x2 = qMax(x2, jd->xPos() * 1000.0);
            y1 = qMin(y1, jd->yPos() * 1000.0);
            y2 = qMax(y2, jd->yPos() * 1000.0);
            z1 = qMin(z1, jd->zPos() * 1000.0);
            z2 = qMax(z2, jd->zPos() * 1000.0);
        }
    }

    boundMinVec = qglviewer::Vec(x1, y1, z1);
    boundMaxVec = qglviewer::Vec(x2, y2, z2);
}

void PersInterfaceScene::drawSolid( bool withName, bool useDisplayList, DRAWINGSTYLE dstyle,
                                    bool simplified )
{
//    qDebug()<<"drawSolid()"<<useDisplayList;
//    if ( oneTime_useNoDisplayList) {
//        useDisplayList = false;
//    }

    if ( memberDisplayList.isEmpty() ) return;
    qreal eps = 1.0e-3;

    MemberData *pre_md = nullptr;

    for ( int i = 0; i < memberDisplayList.count() ; i++ ) {

        MemberData *md = memberList.at(i);
        GLuint id = memberDisplayList.at(i);
        bool isSelected = ( !withName && selectedMembers.contains(md) );

        if ( withName && md != pre_md ) glPushName(id);

        if ( md->isLineMember(false) ) {

            HLINESHAPE shape = md->getHLineShape();
            QList<qreal> partList = md->getPartitionLenList();

            INDEXPAIR idx = memberIndexList.at(i);
            qreal st_len = md->getJointLength(idx.indexI);
            qreal en_len = md->getJointLength(idx.indexJ);

            int st_idx = 0, en_idx = 0;
            for ( int j = 1; j < partList.count(); j++ ) {
                qreal len = partList.at(j);
                if ( st_len > len - eps ) st_idx = j;
                if ( en_len > len + eps ) en_idx = j + 1;
            }

            glPushMatrix();

       //     if(true){
            if ( useDisplayList ) {
                glCallList(id);
            } else {
                qreal angle = md->codeAngle();
                qreal h0, hh;
                if ( shape.renderType == RENDERSHAPE::SHAPE_I || shape.renderType == RENDERSHAPE::SHAPE_T2 ) {
                    h0 = shape.leftSizeB / 2.0;
                    hh = shape.leftSizeH / 2.0 * qAbs(qSin(angle)) + shape.leftSizeB / 2.0 * qAbs(qCos(angle));
                } else if ( shape.renderType == RENDERSHAPE::SHAPE_PIPE ) {
                    h0 = shape.leftSizeH / 2.0;
                    hh = shape.leftSizeH / 2.0;
                } else {
                    h0 = shape.leftSizeH / 2.0;
                    hh = shape.leftSizeB / 2.0 * qAbs(qSin(angle)) + shape.leftSizeH / 2.0 * qAbs(qCos(angle));
                }

                qreal shy = md->getShiftY();
                qreal shz = md->getShiftZ();
                XYZ i_xyz = md->jointAt(idx.indexI)->xyz();
                XYZ j_xyz = md->jointAt(idx.indexJ)->xyz();
                bool isVertical=false;
                if(qAbs(i_xyz.x - j_xyz.x)+qAbs(i_xyz.y - j_xyz.y)<0.0001)isVertical=true;
                setLocalMatrix(i_xyz.x * 1000.0, i_xyz.y * 1000.0, i_xyz.z * 1000.0,
                               j_xyz.x * 1000.0, j_xyz.y * 1000.0, j_xyz.z * 1000.0,
                               h0, hh, shy, shz, angle * 180. / M_PI, md->dataType(),isVertical);
            }

            qreal l_first = ( st_idx == 0 ) ? partList.at(1) - st_len : 0.0 ;
            qreal l_last = ( en_idx == 5 ) ? partList.at(4) - st_len : en_len - st_len;

            ELEMENTSTATE e_state = ( memberStates.contains(md) ) ?  memberStates.value(md) : ELNORMAL ;

            if ( e_state == ELERROR || e_state == ELDUMMY ) {
                createLineMember(md->dataType(), md->sectionName(), e_state, RENDERSHAPE::SHAPE_BOX,
                                 l_first, l_last, 100.0, 100.0,
                                 dstyle, isSelected, simplified);
            } else if ( st_idx < 2 ) {
                if ( en_idx == 2 ) {
                    createLineMember(md->dataType(), md->sectionName(), e_state, shape.renderType,
                                     l_first, l_last, shape.leftSizeB, shape.leftSizeH,
                                     dstyle, isSelected, simplified,shape.leftSizeB_T,shape.leftSizeH_T);
                } else if ( en_idx == 3 ) {
                    createLineMember(md->dataType(), md->sectionName(), e_state, shape.renderType,
                                     l_first, partList.at(2) - st_len, shape.centerSizeB, shape.centerSizeH,
                                     dstyle, isSelected, simplified,shape.centerSizeB_T,shape.centerSizeH_T);
                    createLineMember(md->dataType(), md->sectionName(), e_state, shape.renderType,
                                     partList.at(2) - st_len, l_last, shape.rightSizeB, shape.rightSizeH,
                                     dstyle, isSelected, simplified,shape.rightSizeB_T,shape.rightSizeH_T);
                } else if ( en_idx > 3 ) {
                    createLineMember(md->dataType(), md->sectionName(), e_state, shape.renderType,
                                     l_first, partList.at(2) - st_len, shape.leftSizeB, shape.leftSizeH,
                                     dstyle, isSelected, simplified,shape.leftSizeB_T,shape.leftSizeH_T);
                    createLineMember(md->dataType(), md->sectionName(), e_state, shape.renderType,
                                     partList.at(2) - st_len, partList.at(3) - st_len, shape.centerSizeB, shape.centerSizeH,
                                     dstyle, isSelected, simplified,shape.centerSizeB_T,shape.centerSizeH_T);
                    createLineMember(md->dataType(), md->sectionName(), e_state, shape.renderType,
                                     partList.at(3) - st_len, l_last, shape.rightSizeB, shape.rightSizeH,
                                     dstyle, isSelected, simplified,shape.rightSizeB_T,shape.rightSizeH_T);
                }
            } else if ( st_idx == 2 ) {
                if ( en_idx == 3 ) {
                    createLineMember(md->dataType(), md->sectionName(), e_state, shape.renderType,
                                     l_first, l_last, shape.centerSizeB, shape.centerSizeH,
                                     dstyle, isSelected, simplified,shape.centerSizeB_T,shape.centerSizeH_T);
                } else if ( en_idx > 3 ) {
                    createLineMember(md->dataType(), md->sectionName(), e_state, shape.renderType,
                                     l_first, partList.at(3) - st_len, shape.centerSizeB, shape.centerSizeH,
                                     dstyle, isSelected, simplified,shape.centerSizeB_T,shape.centerSizeH_T);
                    createLineMember(md->dataType(), md->sectionName(), e_state, shape.renderType,
                                     partList.at(3) - st_len, l_last, shape.rightSizeB, shape.rightSizeH,
                                     dstyle, isSelected, simplified,shape.rightSizeB_T,shape.rightSizeH_T);
                }
            } else if ( st_idx == 3 ) {
                createLineMember(md->dataType(), md->sectionName(), e_state, shape.renderType,
                                 l_first, l_last, shape.rightSizeB, shape.rightSizeH,
                                 dstyle, isSelected, simplified,shape.rightSizeB_T,shape.rightSizeH_T);
            }

            glPopMatrix();

        }

        if ( md->isClosedMember(false) ) {

            ELEMENTSTATE e_state = ( memberStates.contains(md) ) ?  memberStates.value(md) : ELNORMAL ;

            QList<JointData *> jdlist = md->getJointList();
            QList<qglviewer::Vec> vlist;
            Q_FOREACH ( JointData *jd, jdlist ) {
//                vlist.append( Vec(jd->xPos()*1000.0, jd->yPos()*1000.0, jd->zPos()*1000.0) );
//                qDebug() << md->sectionName() << md->getWVector().x << md->getWVector().y << md->getWVector().z;

                qglviewer::Vec v1( jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos() * 1000.0 );
                qglviewer::Vec v2( md->getWVector().x * md->getShiftZ(),
                        md->getWVector().y * md->getShiftZ(),
                        md->getWVector().z * md->getShiftZ() );
                vlist.append( v1 + v2 );

            }
            createClosedMember(md->dataType(), md->sectionName(), e_state, dstyle, vlist,
                               closedMemberTriangles.value(md), isSelected);

        }

        if ( withName ) {
            MemberData *next_md = ( i < memberDisplayList.count() - 1 ) ? memberList.at(i + 1) : NULL;
            if ( md != next_md ) glPopName();
        }

        pre_md = md;

    }
}

void PersInterfaceScene::drawMemberText( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u )
{
//    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    QColor color = viewSettings.getElementNameColor( ELGIRDER);
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    int isize = viewSettings.getElementNameSize( ELGIRDER);
    qreal swidth = 200.0 + qreal(isize) * 40.0;

    // カメラベクトルと正対するような座標変換
    GLdouble theta1, theta2;
    qglviewer::Vec v1, v2;
    qglviewer::Vec a1(1.0, 0.0, 0.0);
    qglviewer::Quaternion q1(a1, v_r);
    theta1 = q1.angle() * 180.0 / M_PI;
    v1 = q1.axis();
    qreal v1_norm = v1.norm();
    qglviewer::Vec a2(0.0, 0.0, 1.0);
    qglviewer::Vec a3 = q1.rotate(a2);
    qglviewer::Quaternion q2(a3, v_u);
    theta2 = q2.angle() * 180. / M_PI;
    v2 = q2.axis();
    qreal v2_norm = v2.norm();

    MemberData *pre_md = NULL;

    for ( int i = 0; i < memberList.count() ; i++ ) {

        MemberData *md = memberList.at(i);
        if ( md == pre_md ) continue;

        ELEMENTTYPE etype = dataTypeToElementType(md->dataType());
        if ( !viewSettings.getElementView( etype).isNameVisible ) continue;

        ELEMENTSTATE e_state = ( memberStates.contains(md) ) ?  memberStates.value(md) : ELNORMAL ;

        QString sectionName;
        if ( e_state == ELNORMAL ) {
            sectionName = md->sectionName();
            QColor color = viewSettings.getElementNameColor( ELGIRDER);
            glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        } else if ( e_state == ELDUMMY ) {
            sectionName = dummyName(md->dataType());
            setStateLineColor(ELDUMMY);
        } else {
            sectionName = QString("ERROR(") + md->sectionName() + QString(")");
            setStateLineColor(ELERROR);
        }

        if ( md->dataType() == DATATYPE::TPWALL || md->dataType() == DATATYPE::TPUWALL ) {
            if ( !md->wallOpenName().isEmpty() )
                sectionName += QString("(") + md->wallOpenName() + QString(")");
        }

        qreal shiftX = 0.0;
        qreal shiftY = 0.0;

        glPushMatrix();

        if ( md->isLineMember(false) ) {

            HLINESHAPE shape = md->getHLineShape();

            XYZ i_xyz = md->iJoint()->xyz();
            XYZ j_xyz = md->jJoint()->xyz();

            XYZ tr_xyz;
            if ( md->dataType() == DATATYPE::TPBRACE ) {
                if ( i_xyz.z < j_xyz.z ) {
                    tr_xyz = XYZ( 750.0 * i_xyz.x + 250.0 * j_xyz.x, 750.0 * i_xyz.y + 250.0 * j_xyz.y,
                                  750.0 * i_xyz.z + 250.0 * j_xyz.z );
                } else {
                    tr_xyz = XYZ( 250.0 * i_xyz.x + 750.0 * j_xyz.x, 250.0 * i_xyz.y + 750.0 * j_xyz.y,
                                  250.0 * i_xyz.z + 750.0 * j_xyz.z );
                }
            } else {
                tr_xyz = XYZ( (j_xyz.x + i_xyz.x) * 500.0, (j_xyz.y + i_xyz.y) * 500.0,
                              (j_xyz.z + i_xyz.z) * 500.0 );
            }

            glTranslatef(tr_xyz.x, tr_xyz.y, tr_xyz.z + 10.0);

            if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                shiftX = qMax(shape.leftSizeB, shape.leftSizeH);
                shiftX = qMax(shiftX, shape.rightSizeB);
                shiftX = qMax(shiftX, shape.rightSizeH);
                shiftX = shiftX / 2.0 + 100.0;
                shiftY = -swidth * 1.25 / 2.0;
            } else {
                shiftX = -swidth * sectionName.count() / 2.0;
                shiftY = 100.0;
            }

        }

        if ( md->isClosedMember(false) ) {

            XYZ tr_xyz(0.0, 0.0, 0.0);
            QList<JointData *> jdlist = md->getJointList();

            int count = 0;
            Q_FOREACH ( JointData *jd, jdlist ) {
                tr_xyz.x += jd->xPos();
                tr_xyz.y += jd->yPos();
                tr_xyz.z += jd->zPos();
                count++;
            }

            tr_xyz.x = tr_xyz.x / qreal(count) * 1000.0;
            tr_xyz.y = tr_xyz.y / qreal(count) * 1000.0;
            tr_xyz.z = tr_xyz.z / qreal(count) * 1000.0;

            glTranslatef(tr_xyz.x, tr_xyz.y, tr_xyz.z + 10.0);
            shiftX = -swidth * sectionName.count() / 2.0;
            shiftY = 100.0;

        }

        if ( v2_norm > 0.001 ) glRotatef(theta2, v2.x, v2.y, v2.z);
        if ( v1_norm > 0.001 ) glRotatef(theta1, v1.x, v1.y, v1.z);

        drawXZText(sectionName, swidth, shiftX, shiftY);

        glPopMatrix();

    }
    glEnable(GL_LIGHTING);
}

void PersInterfaceScene::drawJointText( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u )
{
    if ( !viewSettings.getElementView( ELJOINT).isNameVisible ) return;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    QColor color = viewSettings.getElementNameColor( ELJOINT);
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    int isize = viewSettings.getElementNameSize( ELJOINT);
    qreal swidth = 100.0 + qreal(isize) * 30.0;

    // カメラベクトルと正対するような座標変換
    GLdouble theta1, theta2;
    qglviewer::Vec v1, v2;
    qglviewer::Vec a1(1.0, 0.0, 0.0);
    qglviewer::Quaternion q1(a1, v_r);
    theta1 = q1.angle() * 180.0 / M_PI;
    v1 = q1.axis();
    qreal v1_norm = v1.norm();
    qglviewer::Vec a2(0.0, 0.0, 1.0);
    qglviewer::Vec a3 = q1.rotate(a2);
    qglviewer::Quaternion q2(a3, v_u);
    theta2 = q2.angle() * 180. / M_PI;
    v2 = q2.axis();
    qreal v2_norm = v2.norm();

    Q_FOREACH ( JointData *jd, jointList ) {

        qreal shiftX = 0.0;
        qreal shiftY = 100.0;

        QString jno = QString("%1").arg( UnifiedInputData::getInInstance()->indexOfJoint(jd) + 1);
        shiftX = -swidth * jno.count() / 2.0;

        glPushMatrix();
        glTranslatef(jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos() * 1000.0);

        if ( v2_norm > 0.001 ) glRotatef(theta2, v2.x, v2.y, v2.z);
        if ( v1_norm > 0.001 ) glRotatef(theta1, v1.x, v1.y, v1.z);

        drawXZText(jno, swidth, shiftX, shiftY);

        glPopMatrix();

    }

    glEnable(GL_LIGHTING);
}

void PersInterfaceScene::drawGrid(qreal ox, qreal oy, qreal oz, qreal pitch, int cx, int cy)
{
    GLdouble xlen = GLdouble(cx) * GLdouble(pitch);
    GLdouble ylen = GLdouble(cy) * GLdouble(pitch);

    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);

    if ( ylen > 1.0e-2 ) {
        for ( int i = 0; i < cx + 1 ; i++ ) {
            GLdouble xx = ox + GLdouble(i) * GLdouble(pitch);
            glVertex3d(xx, oy,      oz );
            glVertex3d(xx, oy + ylen, oz );
        }
    }

    if ( xlen > 1.0e-2 ) {
        for ( int i = 0; i < cy + 1 ; i++ ) {
            GLdouble yy = oy + GLdouble(i) * GLdouble(pitch);
            glVertex3d(ox,      yy, oz );
            glVertex3d(ox + xlen, yy, oz );
        }
    }

    glEnd();
    glEnable(GL_LIGHTING);
}


void PersInterfaceScene::drawPoints()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glPointSize(3.0);
    QColor n_col = viewSettings.getElementView( ELJOINT).lineColor;
    QColor s_col = viewSettings.getElementStateView(
                                                                       ELSELECT).lineColor;

    Q_FOREACH ( JointData *jd, jointList ) {

        ( selectedJoints.contains(jd) ) ?
        glColor4f(s_col.redF(), s_col.greenF(), s_col.blueF(), 1.0) :
        glColor4f(n_col.redF(), n_col.greenF(), n_col.blueF(), 1.0) ;

        glBegin(GL_POINTS);
        glVertex3d(jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos() * 1000.0);
        glEnd();
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void PersInterfaceScene::drawFrameScale( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u )
{
    if ( !viewSettings.getFrameScaleIsChecked() ) return;

    QUuid myFloorID = viewSettings.getFrameScaleFloorID();
    if ( myFloorID.isNull() ) return;

    glDisable(GL_LIGHTING);
    qreal zz =  UnifiedInputData::getInInstance()->getFloorHeight(myFloorID) * 1000.0;

    // カメラベクトルと正対するような座標変換
    GLdouble theta1, theta2;
    qglviewer::Vec v1, v2;
    qglviewer::Vec a1(1.0, 0.0, 0.0);
    qglviewer::Quaternion q1(a1, v_r);
    theta1 = q1.angle() * 180.0 / M_PI;
    v1 = q1.axis();
    qreal v1_norm = v1.norm();
    qglviewer::Vec a2(0.0, 0.0, 1.0);
    qglviewer::Vec a3 = q1.rotate(a2);
    qglviewer::Quaternion q2(a3, v_u);
    theta2 = q2.angle() * 180. / M_PI;
    v2 = q2.axis();
    qreal v2_norm = v2.norm();

    QColor color = viewSettings.getScaleColor();
    glColor3f(color.redF(), color.greenF(), color.blueF());
    int isize = viewSettings.getElementNameSize( ELGIRDER);
    qreal swidth = 200.0 + qreal(isize) * 50.0;
    glPointSize(3.0);

    QList<FRAMEPOINTSVALUE> drawFrameList =  UnifiedInputData::getInInstance()->getFrameValuesOfFloor(
                                                myFloorID);
    QPointF p1, p2, p3;

    Q_FOREACH (FRAMEPOINTSVALUE fp, drawFrameList) {

        if ( fp.pointList.count() < 2 ) continue;

        glBegin(GL_POINTS);
        for ( int i = 0; i < fp.pointList.count(); i++ ) {
            p1 = fp.pointList.at(i);
            glVertex3d( p1.x() * 1000.0, p1.y() * 1000.0, zz );
        }
        glEnd();

        glEnable(GL_LINE_STIPPLE);
        glLineStipple(2, 0xAAAA);

        glBegin(GL_LINE_STRIP);
        for ( int i = 1; i < fp.pointList.count(); i++ ) {
            p1 = fp.pointList.at(i - 1);
            p2 = fp.pointList.at(i);
            glVertex3d( p1.x() * 1000.0, p1.y() * 1000.0, zz );
            glVertex3d( p2.x() * 1000.0, p2.y() * 1000.0, zz );
        }
        if ( fp.frameType == FrameType::FRAMETYPE::CLCIRCLE || fp.frameType == FrameType::FRAMETYPE::CLVARIABLE  ) {
            p1 = fp.pointList.last();
            p2 = fp.pointList.first();
            glVertex3d( p1.x() * 1000.0, p1.y() * 1000.0, zz );
            glVertex3d( p2.x() * 1000.0, p2.y() * 1000.0, zz );
        }
        glEnd();

        glDisable(GL_LINE_STIPPLE);

        // 通り名
        glPushMatrix();

        p3 = fp.namePoint;
        glTranslatef( p3.x() * 1000.0, p3.y() * 1000.0, zz );

        if ( v2_norm > 0.001 ) glRotatef(theta2, v2.x, v2.y, v2.z);
        if ( v1_norm > 0.001 ) glRotatef(theta1, v1.x, v1.y, v1.z);

        glBegin(GL_LINE_LOOP);
        for ( int i = 0; i < 12; i++ ) {
            qreal theta = 30.0 * qreal(i) / 180.0 * M_PI;
            glVertex3d( 1000.0 * qCos(theta), 0.0, 1000.0 * qSin(theta) );
        }
        glEnd();

        QString fname = fp.frameName;
        qreal shx = -qreal(fname.count()) * swidth / 2.0;
        qreal shy = -swidth * 1.25 / 2.0;
        drawXZText(fname, swidth, shx, shy);

        glPopMatrix();

        // 直線通りのスパン（文字＋ライン）
        if ( fp.frameType == FrameType::FRAMETYPE::STRAIGHT ) {

            p1 = fp.straight.spoint;
            p1 *= 1000.0;
            qreal span = fp.straight.span * 1000.0;
            qreal angle = fp.straight.angle / 180.0 * M_PI;
            QString tstr = QString("%1").arg( span, 0, 'f', 0 );
            p2 = p1 + QPointF( span / 2.0 * qCos(angle), span / 2.0 * qSin(angle) );

            glPushMatrix();

            glTranslatef( p2.x(), p2.y(), zz );

            if ( v2_norm > 0.001 ) glRotatef(theta2, v2.x, v2.y, v2.z);
            if ( v1_norm > 0.001 ) glRotatef(theta1, v1.x, v1.y, v1.z);

            shx = -qreal(tstr.count()) * swidth * 0.8 / 2.0;
            shy = 100.0;
            drawXZText(tstr, swidth * 0.8, shx, shy);

            glPopMatrix();

            p2 = p1 + QPointF( span * qCos(angle), span * qSin(angle) );
            p3 = p2 + QPointF( 2000.0 * qCos(angle + M_PI / 2.0), 2000.0 * qSin(angle + M_PI / 2.0) );

            glBegin(GL_LINE_STRIP);
            glVertex3d( p1.x(), p1.y(), zz );
            glVertex3d( p2.x(), p2.y(), zz );
            glVertex3d( p3.x(), p3.y(), zz );
            glEnd();

            if ( fp.straight.isFirst || fp.straight.isLast ) {
                p2 = p1 + QPointF( 2000.0 * qCos(angle + M_PI / 2.0), 2000.0 * qSin(angle + M_PI / 2.0) );
                glBegin(GL_LINES);
                glVertex3d( p1.x(), p1.y(), zz );
                glVertex3d( p2.x(), p2.y(), zz );
                glEnd();
            }
        }
    }

    glEnable(GL_LIGHTING);
}

void PersInterfaceScene::drawFloorScale( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u )
{
    if ( !viewSettings.getFloorScaleIsChecked() ) return;

    glDisable(GL_LIGHTING);
    QPointF op = viewSettings.getFloorScaleXY();

    // カメラベクトルと正対するような座標変換
    GLdouble theta1, theta2;
    qglviewer::Vec v1, v2;
    qglviewer::Vec a1(1.0, 0.0, 0.0);
    qglviewer::Quaternion q1(a1, v_r);
    theta1 = q1.angle() * 180.0 / M_PI;
    v1 = q1.axis();
    qreal v1_norm = v1.norm();
    qglviewer::Vec a2(0.0, 0.0, 1.0);
    qglviewer::Vec a3 = q1.rotate(a2);
    qglviewer::Quaternion q2(a3, v_u);
    theta2 = q2.angle() * 180. / M_PI;
    v2 = q2.axis();
    qreal v2_norm = v2.norm();

    QColor color = viewSettings.getScaleColor();
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    int isize = viewSettings.getElementNameSize( ELGIRDER);
    qreal swidth = 200.0 + qreal(isize) * 50.0;
    glPointSize(3.0);

    QList<FLOORVALUES> floorList =  UnifiedInputData::getInInstance()->getFloorList();
    for ( int i = floorList.count() - 1; i >= 0; i-- )
        if ( !floorList.at(i).displaysFloorScale ) floorList.removeAt(i);

    for ( int i = 0; i < floorList.count(); i++ ) {

        qreal cur_z = floorList.at(i).floorHeight * 1000.0;

        glBegin(GL_POINTS);
        glVertex3d( op.x(), op.y(), cur_z );
        glEnd();

        glPushMatrix();

        glTranslatef( op.x(), op.y(), cur_z );

        if ( v2_norm > 0.001 ) glRotatef(theta2, v2.x, v2.y, v2.z);
        if ( v1_norm > 0.001 ) glRotatef(theta1, v1.x, v1.y, v1.z);

        glBegin(GL_LINES);
        glVertex3d( 0.0f, 0.0f, 0.0f );
        glVertex3d( -4000.0f, 0.0f, 0.0f );
        glEnd();

        QString floorName = floorList.at(i).floorName;
        qreal shx = -2000.0 - qreal(floorName.count()) * swidth / 2.0;
        qreal shy = 100.0;
        drawXZText(floorName, swidth, shx, shy);

        glPopMatrix();

        if ( i > floorList.count() - 2 ) break;

        qreal next_z = floorList.at(i + 1).floorHeight * 1000.0;

        glBegin(GL_LINES);
        glVertex3d( op.x(), op.y(), cur_z );
        glVertex3d( op.x(), op.y(), next_z );
        glEnd();

        glPushMatrix();

        glTranslatef( op.x(), op.y(), (cur_z + next_z) / 2.0 );

        if ( v2_norm > 0.001 ) glRotatef(theta2, v2.x, v2.y, v2.z);
        if ( v1_norm > 0.001 ) glRotatef(theta1, v1.x, v1.y, v1.z);

        QString strh = QString("%1").arg( next_z - cur_z, 0, 'f', 0 );
        shx = -100.0 - qreal(strh.count()) * swidth * 0.8;
        shy = -swidth * 1.25 * 0.8 / 2.0;
        drawXZText(strh, swidth * 0.8, shx, shy);

        glPopMatrix();

    }
}

void PersInterfaceScene::removeSelectedMemberID(GLuint id)
{
    if ( currentAddEditMode != EDITMODE ) return;
    int idx = memberDisplayList.indexOf(id);
    if ( idx < 0 ) return;
    removeSelectedMember( memberList.at(idx) );
}

void PersInterfaceScene::removeSelectedMember(MemberData *md)
{
    if ( myElementType != ALLELEMENT && myElementType != dataTypeToElementType(md->dataType()) ) return;
    if ( selectedMembers.contains(md) ) selectedMembers.removeOne(md);
}

void PersInterfaceScene::clearSelectedItems()
{
    selectedMembers.clear();
    selectedJoints.clear();
    emit selectedItemChanged();
}


void PersInterfaceScene::createLineMember(DATATYPE dtype, const QString &sectionName,
                                          ELEMENTSTATE estate, RENDERSHAPE stype,
                                          qreal l1, qreal l2, qreal bb, qreal hh,
                                          DRAWINGSTYLE dstyle, bool isSelected, bool simplified , qreal bbt, qreal hht)
{
    if ( l2 - l1 < 1.0e-3 ) return;

    bool isColumn = ( dtype == DATATYPE::TPCOLUMN || dtype == DATATYPE::TPDAMPER || dtype == DATATYPE::TPISO );

    if ( dstyle == DRAWINGSTYLE::LINEONLY ) {

        setCurrentLineColor(isSelected, dtype, sectionName, estate);

        glBegin(GL_LINES);
        glDisable(GL_LIGHTING);

        if ( isColumn ) {
            glVertex3d(0.0, 0.0, l1 );
            glVertex3d(0.0, 0.0, l2 );
        } else {
            glVertex3d(l1, 0.0, 0.0 );
            glVertex3d(l2, 0.0, 0.0 );
        }

        glEnable(GL_LIGHTING);
        glEnd();

        return;
    }

    setCurrentSolidColor(isSelected, dtype, sectionName, estate);

    if ( stype == RENDERSHAPE::SHAPE_H ) {
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::SOLIDONLY ) drawHShapeSolid(l1, l2, bb, hh, isColumn);
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::WIREONLY ) {
            setCurrentLineColor(isSelected, dtype, sectionName, estate);
            drawHShapeWire(l1, l2, bb, hh, isColumn);
        }
    } else if ( stype == RENDERSHAPE::SHAPE_I ) {
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::SOLIDONLY ) drawIShapeSolid(l1, l2, bb, hh, isColumn);
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle ==DRAWINGSTYLE::WIREONLY ) {
            setCurrentLineColor(isSelected, dtype, sectionName, estate);
            drawIShapeWire(l1, l2, bb, hh, isColumn);
        }
    } else if ( stype == RENDERSHAPE::SHAPE_BOX ) {
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::SOLIDONLY ) drawBShapeSolid(l1, l2, bb, hh, isColumn);
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::WIREONLY ) {
            setCurrentLineColor(isSelected, dtype, sectionName, estate);
            drawBShapeWire(l1, l2, bb, hh, isColumn);
        }
    } else if ( stype == RENDERSHAPE::SHAPE_T1 ) {
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::SOLIDONLY ) drawT1ShapeSolid(l1, l2, bb, hh, isColumn);
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::WIREONLY ) {
            setCurrentLineColor(isSelected, dtype, sectionName, estate);
            drawT1ShapeWire(l1, l2, bb, hh, isColumn);
        }
    } else if ( stype == RENDERSHAPE::SHAPE_T2 ) {
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::SOLIDONLY ) drawT2ShapeSolid(l1, l2, bb, hh, isColumn);
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::WIREONLY ) {
            setCurrentLineColor(isSelected, dtype, sectionName, estate);
            drawT2ShapeWire(l1, l2, bb, hh, isColumn);
        }
    } else if ( stype == RENDERSHAPE::SHAPE_C1 ) {
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::SOLIDONLY ) drawCShapeSolid(l1, l2, bb, hh, isColumn);
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::WIREONLY ) {
            setCurrentLineColor(isSelected, dtype, sectionName, estate);
            drawCShapeWire(l1, l2, bb, hh, isColumn);
        }
    } else if ( stype == RENDERSHAPE::SHAPE_L ) {
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::SOLIDONLY ) drawLShapeSolid(l1, l2, bb, hh, isColumn);
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::WIREONLY ) {
            setCurrentLineColor(isSelected, dtype, sectionName, estate);
            drawLShapeWire(l1, l2, bb, hh, isColumn);
        }
    } else if ( stype == RENDERSHAPE::SHAPE_CROSS ) {
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::SOLIDONLY ) drawCrossShapeSolid(l1, l2, bb, hh, isColumn);
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::WIREONLY ) {
            setCurrentLineColor(isSelected, dtype, sectionName, estate);
            drawCrossShapeWire(l1, l2, bb, hh, isColumn);
        }
    } else if ( stype == RENDERSHAPE::SHAPE_CROSSH ) {
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::SOLIDONLY ) drawCrossHShapeSolid(l1, l2, bb, hh,bbt,hht, isColumn);
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::WIREONLY ) {
            setCurrentLineColor(isSelected, dtype, sectionName, estate);
            drawCrossHShapeWire(l1, l2, bb, hh,bbt,hht, isColumn);
        }
    } else {
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::SOLIDONLY ) drawPShapeSolid(l1, l2, bb, isColumn, simplified);
        if ( dstyle == DRAWINGSTYLE::SOLIDWIRE || dstyle == DRAWINGSTYLE::WIREONLY ) {
            setCurrentLineColor(isSelected, dtype, sectionName, estate);
            drawPShapeWire(l1, l2, hh,isColumn);
        }
    }
}

void PersInterfaceScene::createClosedMember(DATATYPE dtype, const QString &sectionName,
                                            ELEMENTSTATE estate, DRAWINGSTYLE dstyle,
                                            const QList<qglviewer::Vec> &plist, const QList<qglviewer::Vec> &tplist,
                                            bool isSelected )
{

    setCurrentSolidColor(isSelected, dtype, sectionName, estate);

//    if( dstyle==LINEONLY || dstyle==WIREONLY ){

    /*
            setCurrentLineColor(isSelected,dtype,sectionName,estate);

            glBegin(GL_LINE_LOOP);
            glDisable(GL_LIGHTING);
            for( int i=0; i<plist.count(); i++ ){
                Vec vv=plist.at(i);
                glVertex3d( vv.x, vv.y, vv.z );
            }
            glEnable(GL_LIGHTING);
            glEnd();
    */

//        glEnable(GL_POLYGON_STIPPLE);
//        glPolygonStipple(p_mask);

//        glBegin(GL_TRIANGLES);
//        Q_FOREACH( Vec vv, tplist ) glVertex3d( vv.x, vv.y, vv.z+3.0 );
//        glEnd();

//        glDisable(GL_POLYGON_STIPPLE);

//        return;
//    }

    glBegin(GL_TRIANGLES);
    Q_FOREACH ( qglviewer::Vec vv, tplist ) glVertex3d( vv.x, vv.y, vv.z + 3.0 );
    glEnd();

    if ( dstyle == DRAWINGSTYLE::LINEONLY || dstyle == DRAWINGSTYLE::WIREONLY ) return;

    setCurrentLineColor(isSelected, dtype, sectionName, estate);

    if ( dstyle == DRAWINGSTYLE::SOLIDWIRE ) {
        glBegin(GL_LINE_LOOP);
        for ( int i = 0; i < plist.count(); i++ ) {
            qglviewer::Vec vv = plist.at(i);
            glVertex3d( vv.x, vv.y, vv.z + 5.0 );
        }
        glEnd();
    }
}

void PersInterfaceScene::drawHShapeSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 上フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble( bb / 2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb / 2.0), 0.0 );
    }

    glEnd();

    // 下フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 0.0, -1.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb / 2.0), GLdouble(-hh) );
    }

    glEnd();

    // ウェブ2 Y+方向（柱:X+方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(1.0, 0.0, 0.0);
        glVertex3d(0.1, GLdouble( hh / 2.0), l1 );
        glVertex3d(0.1, GLdouble( hh / 2.0), l2 );
        glVertex3d(0.1, GLdouble(-hh / 2.0), l2 );
        glVertex3d(0.1, GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(l1, 0.1, GLdouble(-hh) );
        glVertex3d(l2, 0.1, GLdouble(-hh) );
        glVertex3d(l2, 0.1, 0.0 );
        glVertex3d(l1, 0.1, 0.0 );
    }

    glEnd();

    // ウェブ1 Y-方向（柱:X-方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(-0.1, GLdouble( hh / 2.0), l1 );
        glVertex3d(-0.1, GLdouble( hh / 2.0), l2 );
        glVertex3d(-0.1, GLdouble(-hh / 2.0), l2 );
        glVertex3d(-0.1, GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(l1, -0.1, GLdouble(-hh) );
        glVertex3d(l2, -0.1, GLdouble(-hh) );
        glVertex3d(l2, -0.1, 0.0 );
        glVertex3d(l1, -0.1, 0.0 );
    }

    glEnd();
}

void PersInterfaceScene::drawHShapeWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 上フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(hh / 2.0 + 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
    }

    glEnd();

    // 下フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
    }

    glEnd();

    // ウェブライン
    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, 0.0, 2.5 );
        glVertex3d(l2 + 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, 0.0, 2.5 );
    }

    glEnd();

    // ウェブ破線
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0F0F);

    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, 0.0, 2.5 );
        glVertex3d(l2 + 2.5, 0.0, 2.5 );
    }

    glEnd();

    glDisable(GL_LINE_STIPPLE);

}

void PersInterfaceScene::drawIShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn )
{
    // 上フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(1.0, 0.0, 0.0);
        glVertex3d(GLdouble(hh / 2.0), GLdouble( bb / 2.0), l1 );
        glVertex3d(GLdouble(hh / 2.0), GLdouble( bb / 2.0), l2 );
        glVertex3d(GLdouble(hh / 2.0), GLdouble(-bb / 2.0), l2 );
        glVertex3d(GLdouble(hh / 2.0), GLdouble(-bb / 2.0), l1 );
    } else {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(l1, GLdouble(hh / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(hh / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(hh / 2.0), -bb );
        glVertex3d(l1, GLdouble(hh / 2.0), -bb );
    }

    glEnd();

    // 下フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(GLdouble(-hh / 2.0), GLdouble(-bb / 2.0), l1 );
        glVertex3d(GLdouble(-hh / 2.0), GLdouble(-bb / 2.0), l2 );
        glVertex3d(GLdouble(-hh / 2.0), GLdouble( bb / 2.0), l2 );
        glVertex3d(GLdouble(-hh / 2.0), GLdouble( bb / 2.0), l1 );
    } else {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(l1, GLdouble(-hh / 2.0), -bb );
        glVertex3d(l2, GLdouble(-hh / 2.0), -bb );
        glVertex3d(l2, GLdouble(-hh / 2.0), 0.0 );
        glVertex3d(l1, GLdouble(-hh / 2.0), 0.0 );
    }

    glEnd();

    // ウェブ2 Y+方向（柱:X+方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(GLdouble(-hh / 2.0), 0.1, l1 );
        glVertex3d(GLdouble(-hh / 2.0), 0.1, l2 );
        glVertex3d(GLdouble( hh / 2.0), 0.1, l2 );
        glVertex3d(GLdouble( hh / 2.0), 0.1, l1 );
    } else {
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(l1, GLdouble(-hh / 2.0), -bb / 2.0 + 0.1 );
        glVertex3d(l2, GLdouble(-hh / 2.0), -bb / 2.0 + 0.1 );
        glVertex3d(l2, GLdouble( hh / 2.0), -bb / 2.0 + 0.1 );
        glVertex3d(l1, GLdouble( hh / 2.0), -bb / 2.0 + 0.1 );
    }

    glEnd();

    // ウェブ1 Y-方向（柱:X-方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(GLdouble( hh / 2.0), -0.1, l1 );
        glVertex3d(GLdouble( hh / 2.0), -0.1, l2 );
        glVertex3d(GLdouble(-hh / 2.0), -0.1, l2 );
        glVertex3d(GLdouble(-hh / 2.0), -0.1, l1 );
    } else {
        glNormal3d(0.0, 0.0, -1.0);
        glVertex3d(l1, GLdouble( hh / 2.0), -bb / 2.0 - 0.1 );
        glVertex3d(l2, GLdouble( hh / 2.0), -bb / 2.0 - 0.1 );
        glVertex3d(l2, GLdouble(-hh / 2.0), -bb / 2.0 - 0.1 );
        glVertex3d(l1, GLdouble(-hh / 2.0), -bb / 2.0 - 0.1 );
    }

    glEnd();
}

void PersInterfaceScene::drawIShapeWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn )
{
    // 上フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(hh / 2.0 + 2.5), GLdouble( bb / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(hh / 2.0 + 2.5), GLdouble( bb / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(hh / 2.0 + 2.5), GLdouble(-bb / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(hh / 2.0 + 2.5), GLdouble(-bb / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(hh / 2.0 + 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(hh / 2.0 + 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(hh / 2.0 + 2.5), -bb - 2.5 );
        glVertex3d(l1 - 2.5, GLdouble(hh / 2.0 + 2.5), -bb - 2.5 );
    }

    glEnd();

    // 下フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), GLdouble(-bb / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), GLdouble(-bb / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), GLdouble( bb / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), GLdouble( bb / 2.0 + 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-hh / 2.0 - 2.5), -bb - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-hh / 2.0 - 2.5), -bb - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-hh / 2.0 - 2.5), 2.5 );
        glVertex3d(l1 - 2.5, GLdouble(-hh / 2.0 - 2.5), 2.5 );
    }

    glEnd();

    // ウェブライン
    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble( hh / 2.0 + 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), 0.0, l2 + 2.5 );
        glVertex3d(GLdouble( hh / 2.0 + 2.5), 0.0, l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-hh / 2.0 - 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l1 - 2.5, GLdouble( hh / 2.0 + 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-hh / 2.0 - 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( hh / 2.0 + 2.5), -bb / 2.0 - 2.5 );
    }

    glEnd();

    // ウェブ破線
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0F0F);

    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), 0.0, l2 + 2.5 );
        glVertex3d(GLdouble( hh / 2.0 + 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble( hh / 2.0 + 2.5), 0.0, l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-hh / 2.0 - 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-hh / 2.0 - 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l1 - 2.5, GLdouble( hh / 2.0 + 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( hh / 2.0 + 2.5), -bb / 2.0 - 2.5 );
    }

    glEnd();

    glDisable(GL_LINE_STIPPLE);
}


void PersInterfaceScene::drawT1ShapeSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 上フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble( bb / 2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb / 2.0), 0.0 );
    }

    glEnd();

    // ウェブ2 Y+方向（柱:X+方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(1.0, 0.0, 0.0);
        glVertex3d(0.1, GLdouble( hh / 2.0), l1 );
        glVertex3d(0.1, GLdouble( hh / 2.0), l2 );
        glVertex3d(0.1, GLdouble(-hh / 2.0), l2 );
        glVertex3d(0.1, GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(l1, 0.1, GLdouble(-hh) );
        glVertex3d(l2, 0.1, GLdouble(-hh) );
        glVertex3d(l2, 0.1, 0.0 );
        glVertex3d(l1, 0.1, 0.0 );
    }

    glEnd();

    // ウェブ1 Y-方向（柱:X-方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(-0.1, GLdouble( hh / 2.0), l1 );
        glVertex3d(-0.1, GLdouble( hh / 2.0), l2 );
        glVertex3d(-0.1, GLdouble(-hh / 2.0), l2 );
        glVertex3d(-0.1, GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(l1, -0.1, GLdouble(-hh) );
        glVertex3d(l2, -0.1, GLdouble(-hh) );
        glVertex3d(l2, -0.1, 0.0 );
        glVertex3d(l1, -0.1, 0.0 );
    }

    glEnd();
}

void PersInterfaceScene::drawT1ShapeWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 上フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(hh / 2.0 + 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
    }

    glEnd();

    // ウェブライン
    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, 0.0, 2.5 );
        glVertex3d(l2 + 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, 0.0, 2.5 );
        glVertex3d(l1 - 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, 0.0, GLdouble(-hh - 2.5) );
    }

    glEnd();

    // ウェブ破線
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0F0F);

    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, 0.0, 2.5 );
        glVertex3d(l2 + 2.5, 0.0, 2.5 );
    }

    glEnd();

    glDisable(GL_LINE_STIPPLE);

}

void PersInterfaceScene::drawT2ShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn )
{
    // 上フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(1.0, 0.0, 0.0);
        glVertex3d(GLdouble(hh / 2.0), GLdouble( bb / 2.0), l1 );
        glVertex3d(GLdouble(hh / 2.0), GLdouble( bb / 2.0), l2 );
        glVertex3d(GLdouble(hh / 2.0), GLdouble(-bb / 2.0), l2 );
        glVertex3d(GLdouble(hh / 2.0), GLdouble(-bb / 2.0), l1 );
    } else {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(l1, GLdouble(hh / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(hh / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(hh / 2.0), -bb );
        glVertex3d(l1, GLdouble(hh / 2.0), -bb );
    }

    glEnd();

    // ウェブ2 Y+方向（柱:X+方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(GLdouble(-hh / 2.0), 0.1, l1 );
        glVertex3d(GLdouble(-hh / 2.0), 0.1, l2 );
        glVertex3d(GLdouble( hh / 2.0), 0.1, l2 );
        glVertex3d(GLdouble( hh / 2.0), 0.1, l1 );
    } else {
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(l1, GLdouble(-hh / 2.0), -bb / 2.0 + 0.1 );
        glVertex3d(l2, GLdouble(-hh / 2.0), -bb / 2.0 + 0.1 );
        glVertex3d(l2, GLdouble( hh / 2.0), -bb / 2.0 + 0.1 );
        glVertex3d(l1, GLdouble( hh / 2.0), -bb / 2.0 + 0.1 );
    }

    glEnd();

    // ウェブ1 Y-方向（柱:X-方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(GLdouble( hh / 2.0), -0.1, l1 );
        glVertex3d(GLdouble( hh / 2.0), -0.1, l2 );
        glVertex3d(GLdouble(-hh / 2.0), -0.1, l2 );
        glVertex3d(GLdouble(-hh / 2.0), -0.1, l1 );
    } else {
        glNormal3d(0.0, 0.0, -1.0);
        glVertex3d(l1, GLdouble( hh / 2.0), -bb / 2.0 - 0.1 );
        glVertex3d(l2, GLdouble( hh / 2.0), -bb / 2.0 - 0.1 );
        glVertex3d(l2, GLdouble(-hh / 2.0), -bb / 2.0 - 0.1 );
        glVertex3d(l1, GLdouble(-hh / 2.0), -bb / 2.0 - 0.1 );
    }

    glEnd();
}

void PersInterfaceScene::drawT2ShapeWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn )
{
    // 上フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(hh / 2.0 + 2.5), GLdouble( bb / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(hh / 2.0 + 2.5), GLdouble( bb / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(hh / 2.0 + 2.5), GLdouble(-bb / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(hh / 2.0 + 2.5), GLdouble(-bb / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(hh / 2.0 + 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(hh / 2.0 + 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(hh / 2.0 + 2.5), -bb - 2.5 );
        glVertex3d(l1 - 2.5, GLdouble(hh / 2.0 + 2.5), -bb - 2.5 );
    }

    glEnd();

    // ウェブライン
    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble( hh / 2.0 + 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), 0.0, l2 + 2.5 );
        glVertex3d(GLdouble( hh / 2.0 + 2.5), 0.0, l2 + 2.5 );
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble(-hh / 2.0 - 2.5), 0.0, l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-hh / 2.0 - 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l1 - 2.5, GLdouble( hh / 2.0 + 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-hh / 2.0 - 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( hh / 2.0 + 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l1 - 2.5, GLdouble(-hh / 2.0 - 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-hh / 2.0 - 2.5), -bb / 2.0 - 2.5 );
    }

    glEnd();

    // ウェブ破線
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0F0F);

    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(GLdouble( hh / 2.0 + 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble( hh / 2.0 + 2.5), 0.0, l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble( hh / 2.0 + 2.5), -bb / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( hh / 2.0 + 2.5), -bb / 2.0 - 2.5 );
    }

    glEnd();

    glDisable(GL_LINE_STIPPLE);
}


void PersInterfaceScene::drawCShapeSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 上フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble( bb / 2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb / 2.0), 0.0 );
    }

    glEnd();

    // 下フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 0.0, -1.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb / 2.0), GLdouble(-hh) );
    }

    glEnd();

    // ウェブ Y方向（柱:X方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble( hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble( hh / 2.0), l2 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(l1, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb / 2.0), 0.0 );
    }

    glEnd();
}

void PersInterfaceScene::drawCShapeWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 上フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(hh / 2.0 + 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
    }

    glEnd();

    // 下フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
    }

    glEnd();

    // ウェブライン
    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
    }

    glEnd();
}

void PersInterfaceScene::drawLShapeSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 下フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 0.0, -1.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb / 2.0), GLdouble(-hh) );
    }

    glEnd();

    // ウェブ Y方向（柱:X方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble( hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble( hh / 2.0), l2 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(l1, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb / 2.0), 0.0 );
    }

    glEnd();
}

void PersInterfaceScene::drawLShapeWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 下フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
    }

    glEnd();

    // ウェブライン
    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
    }

    glEnd();
}

void PersInterfaceScene::drawBShapeSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{

    // 上側面
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble( bb / 2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb / 2.0), 0.0 );
    }

    glEnd();

    // 下側面
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 0.0, -1.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb / 2.0), GLdouble(-hh) );
    }

    glEnd();

    // 左側面
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble( hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble( hh / 2.0), l2 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(l1, GLdouble(bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(bb / 2.0), 0.0 );
        glVertex3d(l1, GLdouble(bb / 2.0), 0.0 );
    }


    glEnd();

    // 右側面
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(1.0, 0.0, 0.0);
        glVertex3d(GLdouble(bb / 2.0), GLdouble( hh / 2.0), l1 );
        glVertex3d(GLdouble(bb / 2.0), GLdouble( hh / 2.0), l2 );
        glVertex3d(GLdouble(bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble(bb / 2.0), GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l1, GLdouble(-bb / 2.0), 0.0 );
    }

    glEnd();


    // 上面
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble( hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble( hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l2 );
    } else {
        glNormal3d(1.0, 0.0, 0.0);
        glVertex3d(l2, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb / 2.0), 0.0 );
    }

    glEnd();

    // 下面
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 0.0, -1.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble( hh / 2.0), l1 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble( hh / 2.0), l1 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb / 2.0), 0.0 );
        glVertex3d(l1, GLdouble(-bb / 2.0), 0.0 );
    }

    glEnd();

}

void PersInterfaceScene::drawBShapeWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{

    // 上側面ライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(hh / 2.0 + 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
    }

    glEnd();

    // 下側面ライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
    }

    glEnd();

    // 左側面ライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble(bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble(bb / 2.0 + 2.5), 2.5 );
        glVertex3d(l1 - 2.5, GLdouble(bb / 2.0 + 2.5), 2.5 );
    }

    glEnd();

    // 右側面ライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(bb / 2.0 + 2.5), GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(bb / 2.0 + 2.5), GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
    }

    glEnd();

}

void PersInterfaceScene::drawPShapeSolid( qreal l1, qreal l2, qreal dd, bool isColumn,
                                          bool simplified )
{
    int NStep = (simplified) ? 6 : 8 ;
    qreal rr = dd / 2.0;

    // 側面
    glBegin(GL_QUAD_STRIP);

    if ( isColumn ) {
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t = 2.0 * M_PI * qreal(i) / qreal(NStep);
            glNormal3d( GLdouble(qCos(t)), GLdouble(qSin(t)), 0.0 );
            glVertex3d( GLdouble(rr * qCos(t)), GLdouble(rr * qSin(t)), l1 );
            glVertex3d( GLdouble(rr * qCos(t)), GLdouble(rr * qSin(t)), l2 );
        }
    } else {
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t = 2.0 * M_PI * qreal(i) / qreal(NStep);
            glNormal3d(0.0, GLdouble(qCos(t)), GLdouble(qSin(t)));
            glVertex3d(l1, GLdouble(rr * qCos(t)), GLdouble(rr * qSin(t) - rr) );
            glVertex3d(l2, GLdouble(rr * qCos(t)), GLdouble(rr * qSin(t) - rr) );
        }
    }

    glEnd();

    // 下面
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 0.0, -1.0);
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t = 2.0 * M_PI * qreal(i) / qreal(NStep);
            glVertex3d(GLdouble(rr * qCos(t)), GLdouble(rr * qSin(t)), l1 );
        }
    } else {
        glNormal3d(0.0, -1.0, 0.0);
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t = 2.0 * M_PI * qreal(i) / qreal(NStep);
            glVertex3d(l1, GLdouble(rr * qCos(t)), GLdouble(rr * qSin(t) - rr) );
        }
    }

    glEnd();

    // 上面
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 0.0, 1.0);
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t = 2.0 * M_PI * qreal(i) / qreal(NStep);
            glVertex3d(GLdouble(rr * qCos(t)), GLdouble(rr * qSin(t)), l2 );
        }
    } else {
        glNormal3d(0.0, 1.0, 0.0);
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t = 2.0 * M_PI * qreal(i) / qreal(NStep);
            glVertex3d(l2, GLdouble(rr * qCos(t)), GLdouble(rr * qSin(t) - rr) );
        }
    }

    glEnd();

}

void PersInterfaceScene::drawPShapeWire( qreal l1, qreal l2, qreal dd, bool isColumn,
                                         bool simplified )
{
    int NStep = (simplified) ? 6 : 8 ;
    qreal rr = dd / 2.0;

    // 側面ライン

    rr = rr + 0.5;

    if ( isColumn ) {
        for (int i = 0; i < NStep; ++i) {
            GLdouble t0 = 2.0 * M_PI * qreal(i) / qreal(NStep);
            GLdouble t1 = 2.0 * M_PI * qreal(i + 1) / qreal(NStep);
            glBegin(GL_LINE_STRIP);
            glVertex3d(GLdouble(rr * qCos(t0)), GLdouble(rr * qSin(t0)), l1 - 2.5 );
            glVertex3d(GLdouble(rr * qCos(t1)), GLdouble(rr * qSin(t1)), l1 - 2.5 );
            glVertex3d(GLdouble(rr * qCos(t1)), GLdouble(rr * qSin(t1)), l2 + 2.5 );
            glVertex3d(GLdouble(rr * qCos(t0)), GLdouble(rr * qSin(t0)), l2 + 2.5 );
            glEnd();
        }
    } else {
        for (int i = 0; i < NStep; ++i) {
            GLdouble t0 = 2.0 * M_PI * qreal(i) / qreal(NStep);
            GLdouble t1 = 2.0 * M_PI * qreal(i + 1) / qreal(NStep);
            glBegin(GL_LINE_STRIP);
            glVertex3d(l1 - 2.5, GLdouble(rr * qCos(t0)), GLdouble(rr * qSin(t0) - rr) );
            glVertex3d(l1 - 2.5, GLdouble(rr * qCos(t1)), GLdouble(rr * qSin(t1) - rr) );
            glVertex3d(l2 + 2.5, GLdouble(rr * qCos(t1)), GLdouble(rr * qSin(t1) - rr) );
            glVertex3d(l2 + 2.5, GLdouble(rr * qCos(t0)), GLdouble(rr * qSin(t0) - rr) );
            glEnd();
        }
    }
}
void PersInterfaceScene::drawCrossShapeSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{

    // ウェブ1 Y-方向（柱:X-方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(0, GLdouble( hh / 2.0), l1 );
        glVertex3d(0, GLdouble( hh / 2.0), l2 );
        glVertex3d(0, GLdouble(-hh / 2.0), l2 );
        glVertex3d(0, GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(l1, 0, GLdouble(-hh) );
        glVertex3d(l2, 0, GLdouble(-hh) );
        glVertex3d(l2, 0, 0.0 );
        glVertex3d(l1, 0, 0.0 );
    }

    glEnd();

    // ウェブ2 X-方向（柱:Y-方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0, 1.0, 0.0);
        glVertex3d( GLdouble( bb / 2.0),0,l1 );
        glVertex3d( GLdouble( bb / 2.0),0,l2 );
        glVertex3d( GLdouble(-bb / 2.0),0,l2 );
        glVertex3d( GLdouble(-bb / 2.0),0,l1 );
    } else {
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(l1,GLdouble(-bb),0 );
        glVertex3d(l2,GLdouble(-bb),0);
        glVertex3d(l2,0, 0);
        glVertex3d(l1,0, 0 );
    }

    glEnd();
}
void PersInterfaceScene::drawCrossShapeWire(qreal l1, qreal l2, qreal bb, qreal hh,bool isColumn)
{

    // ウェブライン
    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, 0.0, 2.5 );
        glVertex3d(l2 + 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, 0.0, 2.5 );
    }

    glEnd();


    // ウェブライン
    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(0.0, l1 - 2.5 , GLdouble( bb / 2.0 + 2.5));
        glVertex3d(0.0, l1 - 2.5, GLdouble(-bb / 2.0 - 2.5) );
        glVertex3d(0.0, l2 + 2.5, GLdouble( bb / 2.0 + 2.5) );
        glVertex3d(0.0, l2 + 2.5, GLdouble(-bb / 2.0 - 2.5) );
    } else {
        glVertex3d( GLdouble(-hh - 2.5), 0.0,l1 - 2.5 );
        glVertex3d(2.5 , 0.0, l1 - 2.5);
        glVertex3d(GLdouble(-hh - 2.5), 0.0, l2 + 2.5 );
        glVertex3d(2.5, 0.0,  l2 + 2.5);
    }

    glEnd();


}

void PersInterfaceScene::drawCrossHShapeSolid(qreal l1, qreal l2, qreal bb, qreal hh,qreal bbt,qreal hht,bool isColumn)
{
    // 上フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb / 2.0), 0.0 );
        glVertex3d(l2, GLdouble( bb / 2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb / 2.0), 0.0 );
    }

    glEnd();

    // 下フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l1 );
        glVertex3d(GLdouble(-bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l2 );
        glVertex3d(GLdouble( bb / 2.0), GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 0.0, -1.0);
        glVertex3d(l1, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb / 2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb / 2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb / 2.0), GLdouble(-hh) );
    }

    glEnd();

    // ウェブ2 Y+方向（柱:X+方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(1.0, 0.0, 0.0);
        glVertex3d(0.1, GLdouble( hh / 2.0), l1 );
        glVertex3d(0.1, GLdouble( hh / 2.0), l2 );
        glVertex3d(0.1, GLdouble(-hh / 2.0), l2 );
        glVertex3d(0.1, GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(l1, 0.1, GLdouble(-hh) );
        glVertex3d(l2, 0.1, GLdouble(-hh) );
        glVertex3d(l2, 0.1, 0.0 );
        glVertex3d(l1, 0.1, 0.0 );
    }

    glEnd();

    // ウェブ1 Y-方向（柱:X-方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(-0.1, GLdouble( hh / 2.0), l1 );
        glVertex3d(-0.1, GLdouble( hh / 2.0), l2 );
        glVertex3d(-0.1, GLdouble(-hh / 2.0), l2 );
        glVertex3d(-0.1, GLdouble(-hh / 2.0), l1 );
    } else {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(l1, -0.1, GLdouble(-hh) );
        glVertex3d(l2, -0.1, GLdouble(-hh) );
        glVertex3d(l2, -0.1, 0.0 );
        glVertex3d(l1, -0.1, 0.0 );
    }

    glEnd();

    //i
    // 上フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(1.0, 0.0, 0.0);
        glVertex3d(GLdouble(hht / 2.0), GLdouble( bbt / 2.0), l1 );
        glVertex3d(GLdouble(hht / 2.0), GLdouble( bbt / 2.0), l2 );
        glVertex3d(GLdouble(hht / 2.0), GLdouble(-bbt / 2.0), l2 );
        glVertex3d(GLdouble(hht / 2.0), GLdouble(-bbt / 2.0), l1 );
    } else {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(l1, GLdouble(hht / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(hht / 2.0), 0.0 );
        glVertex3d(l2, GLdouble(hht / 2.0), -bbt );
        glVertex3d(l1, GLdouble(hht / 2.0), -bbt );
    }

    glEnd();

    // 下フランジ
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(-1.0, 0.0, 0.0);
        glVertex3d(GLdouble(-hht / 2.0), GLdouble(-bbt / 2.0), l1 );
        glVertex3d(GLdouble(-hht / 2.0), GLdouble(-bbt / 2.0), l2 );
        glVertex3d(GLdouble(-hht / 2.0), GLdouble( bbt / 2.0), l2 );
        glVertex3d(GLdouble(-hht / 2.0), GLdouble( bbt / 2.0), l1 );
    } else {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(l1, GLdouble(-hht / 2.0), -bb );
        glVertex3d(l2, GLdouble(-hht / 2.0), -bb );
        glVertex3d(l2, GLdouble(-hht / 2.0), 0.0 );
        glVertex3d(l1, GLdouble(-hht / 2.0), 0.0 );
    }

    glEnd();

    // ウェブ2 Y+方向（柱:X+方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, 1.0, 0.0);
        glVertex3d(GLdouble(-hht / 2.0), 0.1, l1 );
        glVertex3d(GLdouble(-hht / 2.0), 0.1, l2 );
        glVertex3d(GLdouble( hht / 2.0), 0.1, l2 );
        glVertex3d(GLdouble( hht / 2.0), 0.1, l1 );
    } else {
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3d(l1, GLdouble(-hht / 2.0), -bbt / 2.0 + 0.1 );
        glVertex3d(l2, GLdouble(-hht / 2.0), -bbt / 2.0 + 0.1 );
        glVertex3d(l2, GLdouble( hht / 2.0), -bbt / 2.0 + 0.1 );
        glVertex3d(l1, GLdouble( hht / 2.0), -bbt / 2.0 + 0.1 );
    }

    glEnd();

    // ウェブ1 Y-方向（柱:X-方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if ( isColumn ) {
        glNormal3d(0.0, -1.0, 0.0);
        glVertex3d(GLdouble( hht / 2.0), -0.1, l1 );
        glVertex3d(GLdouble( hht / 2.0), -0.1, l2 );
        glVertex3d(GLdouble(-hht / 2.0), -0.1, l2 );
        glVertex3d(GLdouble(-hht / 2.0), -0.1, l1 );
    } else {
        glNormal3d(0.0, 0.0, -1.0);
        glVertex3d(l1, GLdouble( hht / 2.0), -bbt / 2.0 - 0.1 );
        glVertex3d(l2, GLdouble( hht / 2.0), -bbt / 2.0 - 0.1 );
        glVertex3d(l2, GLdouble(-hht / 2.0), -bbt / 2.0 - 0.1 );
        glVertex3d(l1, GLdouble(-hht / 2.0), -bbt / 2.0 - 0.1 );
    }

    glEnd();
}


void PersInterfaceScene::drawCrossHShapeWire(qreal l1, qreal l2, qreal bb, qreal hh, qreal bbt,qreal hht,bool isColumn)
{
    // 上フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(hh / 2.0 + 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), 2.5 );
    }

    glEnd();

    // 下フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble( bb / 2.0 + 2.5), GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble(-bb / 2.0 - 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, GLdouble( bb / 2.0 + 2.5), GLdouble(-hh - 2.5) );
    }

    glEnd();

    // ウェブライン
    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, 0.0, 2.5 );
        glVertex3d(l2 + 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, 0.0, 2.5 );
    }

    glEnd();

    // ウェブ破線
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0F0F);

    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(0.0, GLdouble( hh / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(0.0, GLdouble(-hh / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l2 + 2.5, 0.0, GLdouble(-hh - 2.5) );
        glVertex3d(l1 - 2.5, 0.0, 2.5 );
        glVertex3d(l2 + 2.5, 0.0, 2.5 );
    }

    glEnd();

    glDisable(GL_LINE_STIPPLE);


//i
    // 上フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(hht / 2.0 + 2.5), GLdouble( bbt / 2.0 + 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(hht / 2.0 + 2.5), GLdouble( bbt / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(hht / 2.0 + 2.5), GLdouble(-bbt / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(hht / 2.0 + 2.5), GLdouble(-bbt / 2.0 - 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(hht / 2.0 + 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(hht / 2.0 + 2.5), 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(hht / 2.0 + 2.5), -bbt - 2.5 );
        glVertex3d(l1 - 2.5, GLdouble(hht / 2.0 + 2.5), -bbt - 2.5 );
    }

    glEnd();

    // 下フランジライン
    glBegin(GL_LINE_LOOP);

    if ( isColumn ) {
        glVertex3d(GLdouble(-hht / 2.0 - 2.5), GLdouble(-bbt / 2.0 - 2.5), l1 - 2.5 );
        glVertex3d(GLdouble(-hht / 2.0 - 2.5), GLdouble(-bbt / 2.0 - 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(-hht / 2.0 - 2.5), GLdouble( bbt / 2.0 + 2.5), l2 + 2.5 );
        glVertex3d(GLdouble(-hht / 2.0 - 2.5), GLdouble( bbt / 2.0 + 2.5), l1 - 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-hht / 2.0 - 2.5), -bbt - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-hht / 2.0 - 2.5), -bbt - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-hht / 2.0 - 2.5), 2.5 );
        glVertex3d(l1 - 2.5, GLdouble(-hht / 2.0 - 2.5), 2.5 );
    }

    glEnd();

    // ウェブライン
    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(GLdouble(-hht / 2.0 - 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble( hht / 2.0 + 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble(-hht / 2.0 - 2.5), 0.0, l2 + 2.5 );
        glVertex3d(GLdouble( hht / 2.0 + 2.5), 0.0, l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-hht / 2.0 - 2.5), -bbt / 2.0 - 2.5 );
        glVertex3d(l1 - 2.5, GLdouble( hht / 2.0 + 2.5), -bbt / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-hht / 2.0 - 2.5), -bbt / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( hht / 2.0 + 2.5), -bbt / 2.0 - 2.5 );
    }

    glEnd();

    // ウェブ破線
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0F0F);

    glBegin(GL_LINES);

    if ( isColumn ) {
        glVertex3d(GLdouble(-hht / 2.0 - 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble(-hht / 2.0 - 2.5), 0.0, l2 + 2.5 );
        glVertex3d(GLdouble( hht / 2.0 + 2.5), 0.0, l1 - 2.5 );
        glVertex3d(GLdouble( hht / 2.0 + 2.5), 0.0, l2 + 2.5 );
    } else {
        glVertex3d(l1 - 2.5, GLdouble(-hht / 2.0 - 2.5), -bbt / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble(-hht / 2.0 - 2.5), -bbt / 2.0 - 2.5 );
        glVertex3d(l1 - 2.5, GLdouble( hht / 2.0 + 2.5), -bbt / 2.0 - 2.5 );
        glVertex3d(l2 + 2.5, GLdouble( hht / 2.0 + 2.5), -bbt / 2.0 - 2.5 );
    }

    glEnd();

    glDisable(GL_LINE_STIPPLE);
}

void PersInterfaceScene::drawXZText(const QString &name, qreal swidth, qreal shx, qreal shy)
{
    QList<RXYLINE> linelist = ascii_table.stringToXY(name, swidth, swidth * 1.25, shx, shy);

    glBegin(GL_LINES);
    Q_FOREACH (RXYLINE line, linelist) {
        glVertex3d(line.x1,   0.0, line.y1 );
        glVertex3d(line.x2,   0.0, line.y2 );
    }
    glEnd();
}

void PersInterfaceScene::setLocalMatrix(qreal x1,  qreal y1,  qreal z1,
                                        qreal x2,  qreal y2,  qreal z2,
                                        qreal h0,  qreal h1,  qreal shy, qreal shz,
                                        qreal angle, DATATYPE dtype ,bool isVertical)
{
    //isVertical : （変位ではなく）元の座標でみて垂直かどうかで部材座標系の方向を決定
    qreal xx = x2 - x1;
    qreal yy = y2 - y1;
    qreal zz = z2 - z1;

    qglviewer::Vec a1(1.0, 0.0, 0.0);
    qglviewer::Vec a2(xx, yy, 0.0);
    qglviewer::Vec a3(xx, yy, zz);
    a3.normalize();

    qreal cos1, cos2;
    GLdouble theta1, theta2;
    qglviewer::Vec v1, v2;

    if (  dtype != DATATYPE::TPCOLUMN && dtype != DATATYPE::TPDAMPER && dtype != DATATYPE::TPISO ) {

        if ( a2.norm() > 0.001 ) {
            a2.normalize();
            cos1 = a1 * a2;
            if ( cos1 < -0.9999 ) {
                theta1 = 180.;
                v1 = qglviewer::Vec(0.0, 0.0, 1.0);
            } else if ( cos1 > 0.9999 ) {
                theta1 = 0.0;
                v1 = qglviewer::Vec(0.0, 0.0, 0.0);
            } else {
                theta1 = qAcos(cos1) * 180. / M_PI;
                v1 = a1 ^ a2;
            }
            cos2 = a2 * a3;
            if ( cos2 < -0.9999 ) {
                theta2 = 180.;
                v2 = qglviewer::Vec(-a2.y, a2.x, 0.0);
            } else if ( cos2 > 0.9999 ) {
                theta2 = 0.0;
                v2 = qglviewer::Vec(0.0, 0.0, 0.0);
            } else {
                theta2 = qAcos(cos2) * 180. / M_PI;
                v2 = a2 ^ a3;
            }
        } else {
            theta1 = 0.0;
            v1 = qglviewer::Vec(0.0, 0.0, 0.0);
            theta2 = 90.0;
            v2 = qglviewer::Vec(0.0, -a3.z, 0.0);
        }

        glTranslatef(x1, y1, z1);
        if ( v2.norm() > 0.01 ) glRotatef(theta2, v2.x, v2.y, v2.z);
        if ( v1.norm() > 0.01 ) glRotatef(theta1, v1.x, v1.y, v1.z);
        glTranslatef(0.0, shy, shz);
        glTranslatef(0.0, 0.0, -h1);
        glRotatef(GLdouble(angle), 1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, h0);

    } else {

        a1 = qglviewer::Vec(0.0, 0.0, 1.0);
        cos1 = a1 * a3;
        if ( cos1 < -0.9999 ) {
            theta1 = -180.;
            v1 = qglviewer::Vec(qCos(angle * M_PI / 180.0), qSin(angle * M_PI / 180.0), 0.0);
        } else if ( cos1 > 0.9999 ) {
            theta1 = 0.0;
            v1 = qglviewer::Vec(0.0, 0.0, 0.0);
        } else {
            theta1 = qAcos(cos1) * 180.0 / M_PI;
            v1 = a1 ^ a3;
            if(!isVertical)angle += qAtan2(v1.y,v1.x)*180/M_PI;
        }
        glTranslatef(x1, y1, z1);
        if ( v1.norm() > 0.01 ) glRotatef(theta1, v1.x, v1.y, v1.z);
        glTranslatef(shy, shz, 0.0);
        glRotatef(GLdouble(angle), 0.0, 0.0, 1.0);
    }
}

ELEMENTTYPE PersInterfaceScene::dataTypeToElementType(DATATYPE dtype) const
{
    ELEMENTTYPE etype;
    if (dtype == DATATYPE::TPJOINT) {
        etype = ELJOINT;
    } else if (dtype == DATATYPE::TPCOLUMN) {
        etype = ELCOLUMN;
    } else if (dtype == DATATYPE::TPGIRDER) {
        etype = ELGIRDER;
    } else if (dtype == DATATYPE::TPBRACE) {
        etype = ELBRACE;
    } else if (dtype == DATATYPE::TPBEAM) {
        etype = ELBEAM;
    } else if (dtype == DATATYPE::TPSLAB) {
        etype = ELSLAB;
    } else if (dtype == DATATYPE::TPWALL) {
        etype = ELWALL;
    } else if (dtype == DATATYPE::TPUWALL) {
        etype = ELUWALL;
    } else if (dtype == DATATYPE::TPDAMPER) {
        etype = ELDAMPER;
    } else if (dtype == DATATYPE::TPISO) {
        etype = ELISO;
    } else if (dtype == DATATYPE::TPSLABLOAD) {
        etype = ELSLABLOAD;
    } else if (dtype == DATATYPE::TPMEMBERLOAD) {
        etype = ELMEMBERLOAD;
    } else if (dtype == DATATYPE::TPJOINTLOAD) {
        etype = ELJOINTLOAD;
    }
    return etype;
}

DATATYPE PersInterfaceScene::currentDataType() const
{
    DATATYPE type;
    if (myElementType == ELJOINT) {
        type = DATATYPE::TPJOINT;
    } else if (myElementType == ELCOLUMN) {
        type = DATATYPE::TPCOLUMN;
    } else if (myElementType == ELGIRDER) {
        type = DATATYPE::TPGIRDER;
    } else if (myElementType == ELBRACE) {
        type = DATATYPE::TPBRACE;
    } else if (myElementType == ELBEAM) {
        type = DATATYPE::TPBEAM;
    } else if (myElementType == ELSLAB) {
        type = DATATYPE::TPSLAB;
    } else if (myElementType == ELWALL) {
        type = DATATYPE::TPWALL;
    } else if (myElementType == ELUWALL) {
        type = DATATYPE::TPUWALL;
    } else if (myElementType == ELDAMPER) {
        type = DATATYPE::TPDAMPER;
    } else if (myElementType == ELISO) {
        type = DATATYPE::TPISO;
    } else if (myElementType == ELSLABLOAD) {
        type = DATATYPE::TPSLABLOAD;
    } else if (myElementType == ELMEMBERLOAD) {
        type = DATATYPE::TPMEMBERLOAD;
    } else if (myElementType == ELJOINTLOAD) {
        type = DATATYPE::TPJOINTLOAD;
    } else {
        type = DATATYPE::NODATATYPE;
    }
    return type;
}

QString PersInterfaceScene::dummyName(DATATYPE dtype) const
{
    if (dtype == DATATYPE::TPCOLUMN) {
        return QString("(dummyC)");
    } else if (dtype == DATATYPE::TPGIRDER) {
        return QString("(dummyG)");
    } else if (dtype == DATATYPE::TPBRACE) {
        return QString("(dummyV)");
    } else if (dtype == DATATYPE::TPBEAM) {
        return QString("(dummyB)");
    } else if (dtype == DATATYPE::TPSLAB) {
        return QString("(dummyS)");
    } else if (dtype == DATATYPE::TPWALL) {
        return QString("(dummyW)");
    } else if (dtype == DATATYPE::TPUWALL) {
        return QString("(dummyU)");
    } else if (dtype == DATATYPE::TPDAMPER) {
        return QString("(dummyD)");
    } else if (dtype == DATATYPE::TPISO) {
        return QString("(dummyI)");
    }
    return QString();
}

void PersInterfaceScene::setCurrentLineColor( bool isSelected, DATATYPE dtype,
                                              const QString &sectionName, ELEMENTSTATE estate )
{
    if ( isSelected ) {
        setStateLineColor(ELSELECT);
    } else if ( estate == ELNORMAL ) {
        if ( viewSettings.getIsPriorIndividual() &&
                viewSettings.existIndividualSetting( sectionName) ) {
            QColor color = viewSettings.getIndividualSetting(
                                                                                sectionName).lineColor;
            glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        } else {
            setElementLineColor(dtype);
        }
    } else {
        setStateLineColor(estate);
    }
}

void PersInterfaceScene::setCurrentSolidColor( bool isSelected, DATATYPE dtype,
                                               const QString &sectionName, ELEMENTSTATE estate )
{
    if ( isSelected ) {
        setStateSolidColor(ELSELECT);
    } else if ( estate == ELNORMAL ) {
        if ( viewSettings.getIsPriorIndividual() &&
                viewSettings.existIndividualSetting( sectionName) ) {
            QColor color = viewSettings.getIndividualSetting(
                                                                                sectionName).paintBrush.color();
            glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
        } else {
            setElementSolidColor(dtype);
        }
    } else {
        setStateSolidColor(estate);
    }
}

void PersInterfaceScene::setElementSolidColor(DATATYPE dtype)
{
    ELEMENTTYPE etype = dataTypeToElementType(dtype);
    QColor color = viewSettings.getElementView(
                                                                  etype).paintBrush.color();
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void PersInterfaceScene::setElementLineColor(DATATYPE dtype)
{
    ELEMENTTYPE etype = dataTypeToElementType(dtype);
    QColor color = viewSettings.getElementView( etype).lineColor;
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void PersInterfaceScene::setStateSolidColor(ELEMENTSTATE state)
{
    QColor color = viewSettings.getElementStateView(
                                                                       state).paintBrush.color();
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void PersInterfaceScene::setStateLineColor(ELEMENTSTATE state)
{
    QColor color = viewSettings.getElementStateView( state).lineColor;
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
}

void PersInterfaceScene::setJointSelect( const QList<JointData *> &jdlist )
{
    Q_FOREACH ( JointData *jd, jdlist ) {
        if ( jointList.contains(jd) ) selectedJoints.append(jd);
    }
}

void PersInterfaceScene::setMemberSelect( const QList<MemberData *> &mdlist )
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( memberList.contains(md) ) selectedMembers.append(md);
    }
}

QList<DATATYPE> PersInterfaceScene::selectedDataTypes() const
{
    QList<DATATYPE> dtypes;
    if ( myElementType == ALLELEMENT ) {
        if ( !selectedJoints.isEmpty() ) dtypes.append(DATATYPE::TPJOINT);
        Q_FOREACH ( MemberData *md, selectedMembers ) {
            if ( !dtypes.contains( md->dataType() ) ) dtypes.append( md->dataType() );
        }
        return dtypes;
    } else if ( myElementType == ELJOINT ) {
        if ( !selectedJoints.isEmpty() ) dtypes.append(DATATYPE::TPJOINT);
        return dtypes;
    } else {
        if ( !selectedMembers.isEmpty() ) dtypes.append( currentDataType() );
        return dtypes;
    }
    return dtypes;
}


QStringList PersInterfaceScene::selectedJointsXYZStrings() const
{
    QStringList xyz_str;
    xyz_str.append(  QString("%1").arg( selectedJoints.count() ) );

    if ( selectedJoints.isEmpty() ) return xyz_str;

    JointData *jfirst = selectedJoints.first();
    qreal xc = jfirst->xPos();
    qreal yc = jfirst->yPos();
    qreal zc = jfirst->zPos();
    bool xok = true, yok = true, zok = true;
    qreal eps = 1.0e-3;

    Q_FOREACH (JointData *jd, selectedJoints) {
        if ( jd == jfirst ) continue;
        if ( xok ) xok = ( qAbs( xc - jd->xPos() ) < eps ) ;
        if ( yok ) yok = ( qAbs( yc - jd->yPos() ) < eps ) ;
        if ( zok ) zok = ( qAbs( zc - jd->zPos() ) < eps ) ;
        if ( !xok && !yok && !zok ) break;
    }

    ( xok ) ? xyz_str.append(QString("%1").arg(xc, 0, 'f', 3)) : xyz_str.append("***") ;
    ( yok ) ? xyz_str.append(QString("%1").arg(yc, 0, 'f', 3)) : xyz_str.append("***") ;
    ( zok ) ? xyz_str.append(QString("%1").arg(zc, 0, 'f', 3)) : xyz_str.append("***") ;

    return xyz_str;
}

QString PersInterfaceScene::selectedMembersCodeAngle() const
{
    QString str_angle;
    if ( selectedMembers.isEmpty() ) str_angle;

    bool isFirstDetect = false;
    bool fixed = true;
    qreal ag = 0.0;
    Q_FOREACH ( MemberData *md, selectedMembers ) {
        if ( md->dataType() == DATATYPE::TPSLAB || md->dataType() == DATATYPE::TPWALL || md->dataType() == DATATYPE::TPUWALL
                || md->dataType() == DATATYPE::TPDAMPER || md->dataType() == DATATYPE::TPISO ) continue;
        if ( !isFirstDetect ) {
            ag = md->codeAngle();
            isFirstDetect = true;
            continue;
        }
        if ( qAbs( ag - md->codeAngle() ) > 1.0e-3 ) {
            fixed = false;
            break;
        }
    }
    return ( fixed ) ? QString("%1").arg(ag * 180.0 / M_PI, 0, 'f', 3) : QString("***") ;
}

QStringList PersInterfaceScene::selectedMembersShiftValue() const
{
    QStringList strlist;
    if ( selectedMembers.isEmpty() ) return strlist;

    /* 線材だけ */
    bool isFirstDetect = false;
    bool fixed_y = true, fixed_z = true;
    qreal shy = 0.0;
    qreal shz = 0.0;
    Q_FOREACH ( MemberData *md, selectedMembers ) {
        if ( md->dataType() == DATATYPE::TPDAMPER || md->dataType() == DATATYPE::TPISO ) continue;
        if ( !isFirstDetect ) {
            shy = md->getShiftY();
            shz = md->getShiftZ();
            isFirstDetect = true;
            continue;
        }
        if ( qAbs( shy - md->getShiftY() ) > 1.0e-3 ) {
            fixed_y = false;
        }
        if ( qAbs( shz - md->getShiftZ() ) > 1.0e-3 ) {
            fixed_z = false;
        }
        if ( !fixed_y && !fixed_z ) break;
    }
    QString str_shy = ( fixed_y ) ? QString("%1").arg(shy, 0, 'f', 3) : QString("***") ;
    QString str_shz = ( fixed_z ) ? QString("%1").arg(shz, 0, 'f', 3) : QString("***") ;
    strlist << str_shy << str_shz;
    return strlist;
}

QStringList PersInterfaceScene::selectedMembersFaceValue() const
{
    QStringList strlist;
    if ( selectedMembers.isEmpty() ) return strlist;

    bool isFirstDetect = false;
    FACERULE iface_rule = FACERULE::FACE_AUTO;
    qreal iface_len = 0.0;
    FACERULE jface_rule = FACERULE::FACE_AUTO;
    qreal jface_len = 0.0;
    bool ok_irule = true, ok_ilen = true, ok_jrule = true, ok_jlen = true;
    qreal eps = 1.0e-3;

    Q_FOREACH ( MemberData *md, selectedMembers ) {
        if ( md->dataType() == DATATYPE::TPSLAB || md->dataType() == DATATYPE::TPWALL || md->dataType() == DATATYPE::TPUWALL ) continue;
        if ( !isFirstDetect ) {
            iface_rule = md->getIFaceRule();
            iface_len = md->getIFace();
            jface_rule = md->getJFaceRule();
            jface_len = md->getJFace();
            isFirstDetect = true;
            continue;
        }
        if ( ok_irule ) ok_irule = ( md->getIFaceRule() == iface_rule ) ;
        if ( ok_ilen ) ok_ilen = ( qAbs( iface_len - md->getIFace() ) < eps ) ;
        if ( ok_jrule ) ok_jrule = ( md->getJFaceRule() == jface_rule ) ;
        if ( ok_jlen ) ok_jlen = ( qAbs( jface_len - md->getJFace() ) < eps ) ;
    }

    if ( ok_irule ) {
        if ( iface_rule == FACERULE::FACE_AUTO ) {
            strlist.append("0");
        } else {
            strlist.append("1");
        }
    } else {
        strlist.append("2");
    }
    ( ok_ilen ) ? strlist.append( QString("%1").arg(iface_len, 0, 'f', 3) )
    : strlist.append( QString("***") ) ;

    if ( ok_jrule ) {
        if ( jface_rule == FACERULE::FACE_AUTO ) {
            strlist.append("0");
        } else {
            strlist.append("1");
        }
    } else {
        strlist.append("2");
    }
    ( ok_jlen ) ? strlist.append( QString("%1").arg(jface_len, 0, 'f', 3) )
    : strlist.append( QString("***") ) ;

    return strlist;
}
} // namespace post3dapp
