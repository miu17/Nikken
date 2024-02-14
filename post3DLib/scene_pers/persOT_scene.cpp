#include "persOT_scene.h"

#include <QDebug>

#include "calc_vector2d.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "persOT_glview.h"
#include "persOT_partsdraw.h"
#include "unified_analysisdata.h"
#include "unified_data.h"
#include "unified_output.h"
#include "unified_settings.h"


namespace post3dapp{
PersOutputScene::PersOutputScene( QObject *parent )
    : QObject(parent)
{
    connect ( UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::dataCleared,
              this, &PersOutputScene::clearCurrentData );
    connect ( UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::dataReaded,
              this, &PersOutputScene::initDisplayList );

    connect(UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::jointAppended,
            this, &PersOutputScene::createJointItems);
    connect(UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::jointRemoved,
            this, &PersOutputScene::removeJointItems);
    connect(UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::jointChanged,
            this, &PersOutputScene::changeJointItems);

    connect(UnifiedFixedData::getInstance()->getAnalysisData(), SIGNAL(memberAppended(const QList<MemberData *> &)),
            this, SLOT(createMemberItems(const QList<MemberData *> &)));
    connect(UnifiedFixedData::getInstance()->getAnalysisData(), SIGNAL(memberRemoved(const QList<MemberData *> &, DATATYPE)),
            this, SLOT(removeMemberItems(const QList<MemberData *> &, DATATYPE)));
    connect(UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::memberChanged,
            this, &PersOutputScene::changeMemberItems);

}

void PersOutputScene::setChildViewer( PersOutputGLViewer *viewer )
{
    childViewer = viewer;
    connect(UnifiedSettings::getInstance(), &UnifiedSettings::out3D_SettingChanged,
            childViewer, QOverload<void>::of(&PersOutputGLViewer::update) );
}

PersOutputScene::~PersOutputScene()
{
    clearCurrentData();
}

void PersOutputScene::clearCurrentData()
{
    clearSelectedItems();
    closedMemberTriangles.clear();
    memberList.clear();
    jointList.clear();
}

void PersOutputScene::initDisplayList()
{
    limitedFloor = QUuid();
    limitedFrame = QUuid();
    myFigureType = OUTFIGTYPE::UNDEFINED;
    caseIndexA = 0;
    caseIndexB = 0;
    caseIndexC = 0;

    reloadViewItems();
}

void PersOutputScene::reloadViewItems()
{
    clearCurrentData();
    createMemberItems( UnifiedFixedData::getInstance()->getAnalysisData()->memberListOfAll() );
    calcBoundingVec();
}

void PersOutputScene::setLimitedFloor( const QString &str_fl )
{
    limitedFloor = UnifiedFixedData::getInstance()->getAnalysisData()->nameToID(str_fl, DATATYPE::TPAFLOOR);
    reloadViewItems();
}

void PersOutputScene::setLimitedFrame( const QString &str_fr )
{
    limitedFrame = UnifiedFixedData::getInstance()->getAnalysisData()->nameToID(str_fr, DATATYPE::TPFRAMEP);
    reloadViewItems();
}

void PersOutputScene::setFigureType( OUTFIGTYPE otype )
{
    if ( otype != myFigureType ) {
        caseIndexA = 0;
        caseIndexB = 0;
        caseIndexC = 0;
    }

    myFigureType = otype;
    childViewer->update();
}

void PersOutputScene::changeCaseIndex( int idx_a, int idx_b, int idx_c )
{
    caseIndexA = idx_a;
    caseIndexB = idx_b;
    caseIndexC = idx_c;
    childViewer->update();
}

void PersOutputScene::clearSelectedItems()
{
    selectedMembers.clear();
    selectedJoints.clear();
}


bool PersOutputScene::isMemberOkToDisplayLimited( MemberData *md ) const
{
    bool fl_ok = true;
    if ( !limitedFloor.isNull() ) {
        fl_ok = md->containsFloor( limitedFloor );
    }

    bool fr_ok = true;
    if ( !limitedFrame.isNull() ) {
        fr_ok = md->containsFrame( limitedFrame );
    }

    return ( fl_ok && fr_ok ) ;
}

bool PersOutputScene::isJointOkToDisplayLimited( JointData *jd ) const
{
    bool fl_ok = true;
    if ( !limitedFloor.isNull() ) {
        fl_ok = jd->containsFloor( limitedFloor );
    }

    bool fr_ok = true;
    if ( !limitedFrame.isNull() ) {
        fr_ok = jd->containsFrame( limitedFrame );
    }

    return ( fl_ok && fr_ok ) ;
}

void PersOutputScene::createJointItems( const QList<JointData *> &jdlist )
{
    Q_FOREACH ( JointData *jd, jdlist ) {
        if ( !isJointOkToDisplayLimited(jd) ) continue;
        if ( !jd->isAnalysisJoint(*UnifiedFixedData::getInstance()->getAnalysisData()) ) continue;
        if ( !jointList.contains(jd) ) jointList.append(jd);
    }
    childViewer->update();
    calcBoundingVec();
}

void PersOutputScene::removeJointItems( const QList<JointData *> &jdlist )
{
    bool isDisplayChange = false;
    bool isSelectChange = false;

    Q_FOREACH ( JointData *jd, jdlist ) {
        int idx = jointList.indexOf(jd);
        if ( idx >= 0 ) {
            jointList.removeAt(idx);
            isDisplayChange = true;
        }
        idx = selectedJoints.indexOf(jd);
        if ( idx >= 0 ) {
            selectedJoints.removeAt(idx);
            isSelectChange = true;
        }
    }
    if ( isDisplayChange ) {
        childViewer->update();
        calcBoundingVec();
    }
}

void PersOutputScene::changeJointItems( const QList<JointData *> &jdlist )
{
    bool isDisplayChange = false;
    bool isSelectChange = false;

    Q_FOREACH ( JointData *jd, jdlist ) {

        if ( !jd->isAnalysisJoint(*UnifiedFixedData::getInstance()->getAnalysisData()) ) {
            int idx = selectedJoints.indexOf(jd);
            if ( idx >= 0 ) {
                selectedJoints.removeAt(idx);
                isSelectChange = true;
            }
            idx = jointList.indexOf(jd);
            if ( idx >= 0 ) {
                jointList.removeAt(idx);
                isDisplayChange = true;
            }
            continue;
        }

        int idx = jointList.indexOf(jd);
        bool isOk = isJointOkToDisplayLimited(jd);
        if ( idx >= 0 ) {
            if ( !isOk ) {
                jointList.removeAt(idx);
                isDisplayChange = true;
            }
        } else {
            if ( isOk ) {
                jointList.append(jd);
                isDisplayChange = true;
            }
        }
    }

    if ( isDisplayChange ) {
        childViewer->update();
        calcBoundingVec();
    }
}

void PersOutputScene::createMemberItems( const QList<MemberData *> &mdlist, bool updateOn )
{
    bool isDisplayChange = false;

    Q_FOREACH ( MemberData *md, mdlist ) {

        if ( !md->isAnalysisMember(*UnifiedFixedData::getInstance()->getAnalysisData()) ) continue;

        isDisplayChange = true;
        recalcAnalysisJointsOfMember(md);

        if ( !isMemberOkToDisplayLimited(md) ) continue;
        if ( memberList.contains(md) ) continue;

//        GLuint id = glGenLists(1);
//        qDebug() << "A" << id;
        memberList.append( md );
//        memberDisplayList.append( id );
        if ( md->isClosedMember() ) {
            closedMemberTriangles.insert(md, divideClosedMember(md));
        }
    }

    if ( updateOn && isDisplayChange ) childViewer->update();
}

QList<qglviewer::Vec> PersOutputScene::divideClosedMember( MemberData *md ) const
{
    QList<qglviewer::Vec> tmpPoints;
    Q_FOREACH ( JointData *jd, md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getAnalysisData()) ) {
        tmpPoints.append( qglviewer::Vec( jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos(true) * 1000.0 ) );
    }
    QList<qglviewer::Vec> dividedPoints;

    int cur_idx, pre_idx, next_idx;
    qglviewer::Vec v0, v1, v2, vc;

    while ( tmpPoints.count() > 3 ) {

        qreal max_len = 0.0;
        for ( int i = 0; i < tmpPoints.count(); i++ ) {
            qreal len = qAbs(tmpPoints.at(i).norm());
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

void PersOutputScene::removeMemberItems( const QList<MemberData *> &mdlist,
                                         DATATYPE dtype, bool updateOn )
{
    if ( dtype == DATATYPE::TPJOINTLOAD || dtype == DATATYPE::TPMEMBERLOAD || dtype == DATATYPE::TPSLABLOAD
            || dtype == DATATYPE::TPBEAM ) return;
//    if( dtype==TPBEAM || dtype==TPSLAB || dtype==TPUWALL ) return;

    bool isDisplayChange = false;

    Q_FOREACH ( MemberData *md, mdlist ) {
        int idx = memberList.indexOf(md);
        if ( idx < 0 ) continue;

        isDisplayChange = true;
        removeSelectedMember(md);
        memberList.removeAt(idx);
//        GLuint id=memberDisplayList.at(idx);
//        if( glIsList(id) ) glDeleteLists(id,1);
//        memberDisplayList.removeAt(idx);
        if ( dtype != DATATYPE::TPWALL && dtype != DATATYPE::TPSLAB ) continue;
        closedMemberTriangles.remove(md);
    }
//    qDebug() << "removeMemberItems end";
    // 処理後にupdate()をかける場合、行方不明のJointData*が発生するおそれ
//    if( updateOn && isDisplayChange ) childViewer->updateGL();
}

void PersOutputScene::changeMemberItems( const QList<MemberData *> &mdlist )
{
    QMultiHash<int, MemberData *> removed_mdtable;
    bool isDisplayChange = false;
    Q_FOREACH ( MemberData *md, mdlist ) {
        DATATYPE dtype = md->dataType();
        if ( dtype == DATATYPE::TPJOINTLOAD || dtype == DATATYPE::TPMEMBERLOAD
                || dtype == DATATYPE::TPSLABLOAD || dtype == DATATYPE::TPBEAM ) continue;
//        if( dtype==TPBEAM || dtype==TPSLAB || dtype==TPUWALL ) continue;
        isDisplayChange = true;
        removed_mdtable.insert(static_cast<int>(dtype), md);
    }

    Q_FOREACH ( int type, removed_mdtable.uniqueKeys() ) {
        removeMemberItems( removed_mdtable.values(type), static_cast<DATATYPE>(type), false );
        createMemberItems( removed_mdtable.values(type), false );
    }

    if ( isDisplayChange ) childViewer->update();
}

void PersOutputScene::calcBoundingVec()
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
            z1 = qMin(z1, jd->zPos(true) * 1000.0);
            z2 = qMax(z2, jd->zPos(true) * 1000.0);
        }
    }

    boundMinVec = qglviewer::Vec(x1, y1, z1);
    boundMaxVec = qglviewer::Vec(x2, y2, z2);
}

void PersOutputScene::drawSolid()
{
    bool lineVisible = UnifiedSettings::getInstance()->getCalc3DViewSettings().getIsLineVisible();
    bool planeVisible = UnifiedSettings::getInstance()->getCalc3DViewSettings().getIsPlaneVisible();
    QColor lineColor = UnifiedSettings::getInstance()->getCalc3DViewSettings().getLineColor();
    QColor planeColor = UnifiedSettings::getInstance()->getCalc3DViewSettings().getPlaneColor();
    QColor selectColor = UnifiedSettings::getInstance()->getCalc3DViewSettings().getSelectColor();

    for ( int i = 0; i < memberList.count() ; i++ ) {

        MemberData *md = memberList.at(i);
        bool isSelected = ( selectedMembers.contains(md) );

        if ( md->isLineMember(false) ) {

            if ( !lineVisible ) continue;
            QColor col = ( isSelected ) ? selectColor : lineColor ;
            glColor3f(col.redF(), col.greenF(), col.blueF());

            PersOutputPartsDrawing::drawLineOutput(md, myFigureType,
                                                   caseIndexA, caseIndexB, caseIndexC);

        } else if ( md->isClosedMember(false) ) {

            if ( !planeVisible ) continue;
            QColor col = ( isSelected ) ? selectColor : planeColor ;
            glColor3f(col.redF(), col.greenF(), col.blueF());

            createClosedMember(closedMemberTriangles.value(md));
        }
    }
}

void PersOutputScene::drawOutputScale()
{
    if ( !UnifiedSettings::getInstance()->getCalc3DViewSettings().getIsColorBarVisible() ) return;
    PersOutputPartsDrawing::drawColorBar(childViewer, myFigureType);
}

void PersOutputScene::drawMemberText( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u )
{
    if ( !UnifiedSettings::getInstance()->getCalc3DViewSettings().getIsNameVisible() ) return;

    glDisable(GL_LIGHTING);

    int isize = UnifiedSettings::getInstance()->getCalc3DViewSettings().getNameSize();
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
    theta2 = q2.angle() * 180.0 / M_PI;
    v2 = q2.axis();
    qreal v2_norm = v2.norm();

    bool lineVisible = UnifiedSettings::getInstance()->getCalc3DViewSettings().getIsLineVisible();
    bool planeVisible = UnifiedSettings::getInstance()->getCalc3DViewSettings().getIsPlaneVisible();
    QColor nameColor = UnifiedSettings::getInstance()->getCalc3DViewSettings().getNameColor();


    for ( int i = 0; i < memberList.count() ; i++ ) {

        MemberData *md = memberList.at(i);

        if ( md->isLineMember(false) && !lineVisible ) continue;
        if ( md->isClosedMember(false) && !planeVisible ) continue;
        glColor3f(nameColor.redF(), nameColor.greenF(), nameColor.blueF());

        QString sectionName = md->sectionName();
        if ( sectionName.isEmpty() ) sectionName = dummyName(md->dataType());

        qreal shiftX = 0.0;
        qreal shiftY = 0.0;

        glPushMatrix();

        if ( md->isLineMember(false) ) {

            HLINESHAPE shape = md->getHLineShape();

            XYZ i_xyz = md->iJoint()->xyz(true);
            XYZ j_xyz = md->jJoint()->xyz(true);

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

        } else if ( md->isClosedMember(false) ) {

            XYZ tr_xyz(0.0, 0.0, 0.0);
            QList<JointData *> jdlist = md->getJointList();

            int count = 0;
            Q_FOREACH ( JointData *jd, jdlist ) {
                tr_xyz.x += jd->xPos();
                tr_xyz.y += jd->yPos();
                tr_xyz.z += jd->zPos(true);
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

void PersOutputScene::drawJointText( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u )
{
    if ( !UnifiedSettings::getInstance()->getCalc3DViewSettings().getIsJnoVisible() ) return;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    QColor color = UnifiedSettings::getInstance()->getCalc3DViewSettings().getJnoColor();
    glColor3f(color.redF(), color.greenF(), color.blueF());
    int isize = UnifiedSettings::getInstance()->getCalc3DViewSettings().getJnoSize();
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

        QString jno = QString("%1").arg(UnifiedFixedData::getInstance()->getAnalysisData()->indexOfJoint(jd) + 1);
        shiftX = -swidth * jno.count() / 2.0;

        glPushMatrix();
        glTranslatef(jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos(true) * 1000.0);

        if ( v2_norm > 0.001 ) glRotatef(theta2, v2.x, v2.y, v2.z);
        if ( v1_norm > 0.001 ) glRotatef(theta1, v1.x, v1.y, v1.z);

        drawXZText(jno, swidth, shiftX, shiftY);

        glPopMatrix();

    }

    glEnable(GL_LIGHTING);
}

void PersOutputScene::drawPoints()
{
    if ( !UnifiedSettings::getInstance()->getCalc3DViewSettings().getIsJointVisible() ) return;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glPointSize(3.0);
    QColor n_col = UnifiedSettings::getInstance()->getCalc3DViewSettings().getJointColor();
    QColor s_col = UnifiedSettings::getInstance()->getCalc3DViewSettings().getSelectColor();

    Q_FOREACH ( JointData *jd, jointList ) {

        ( selectedJoints.contains(jd) ) ?
        glColor3f(s_col.redF(), s_col.greenF(), s_col.blueF()) :
        glColor3f(n_col.redF(), n_col.greenF(), n_col.blueF()) ;

        glBegin(GL_POINTS);
        glVertex3d(jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos(true) * 1000.0);
        glEnd();

        PersOutputPartsDrawing::drawJointOutput(jd, myFigureType,
                                                caseIndexA, caseIndexB, caseIndexC);
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void PersOutputScene::drawFrameScale( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u )
{
    if ( !UnifiedSettings::getInstance()->getCalc3DViewSettings().getIsFrScaleVisible() ) return;

    QUuid myFloorID = UnifiedSettings::getInstance()->getCalc3DViewSettings().getFrScaleFloor();
    if ( myFloorID.isNull() ) return;

    glDisable(GL_LIGHTING);
    qreal zz = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorHeight(myFloorID) * 1000.0;

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

    QColor color = UnifiedSettings::getInstance()->getCalc3DViewSettings().getScaleColor();
    glColor3f(color.redF(), color.greenF(), color.blueF());
    int isize = UnifiedSettings::getInstance()->getCalc3DViewSettings().getNameSize();
    qreal swidth = 200.0 + qreal(isize) * 50.0;
    glPointSize(3.0);

    QList<FRAMEPOINTSVALUE> drawFrameList = UnifiedFixedData::getInstance()->getAnalysisData()->getFrameValuesOfFloor(
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

void PersOutputScene::drawFloorScale( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u )
{
    if ( !UnifiedSettings::getInstance()->getCalc3DViewSettings().getIsFlScaleVisible() ) return;

    glDisable(GL_LIGHTING);
    QPointF op = UnifiedSettings::getInstance()->getCalc3DViewSettings().getFloorScaleXY() * 1000.0;

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

    QColor color = UnifiedSettings::getInstance()->getCalc3DViewSettings().getScaleColor();
    glColor3f(color.redF(), color.greenF(), color.blueF());
    int isize = UnifiedSettings::getInstance()->getCalc3DViewSettings().getNameSize();
    qreal swidth = 200.0 + qreal(isize) * 50.0;
    glPointSize(3.0);

    QList<FLOORVALUES> floorList = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList();
    QStringList nameList;
    QList<qreal> heightList;

    for ( int i = 0; i < floorList.count(); i++ ) {
        FLOORVALUES cur_fv = floorList.at(i);
        if ( cur_fv.floorType == FLOORTYPE::FL_CHILD ) continue;
        nameList.append(cur_fv.analysisName);
        heightList.append((cur_fv.floorHeight + cur_fv.adjustedLevel) * 1000.0 );
    }

    for ( int i = 0; i < nameList.count(); i++ ) {

        qreal cur_z = heightList.at(i);

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

        QString floorName = nameList.at(i);
        qreal shx = -2000.0 - qreal(floorName.count()) * swidth / 2.0;
        qreal shy = 100.0;
        drawXZText(floorName, swidth, shx, shy);

        glPopMatrix();

        if ( i > nameList.count() - 2 ) break;

        qreal next_z = heightList.at(i + 1);

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

void PersOutputScene::selectPoints(qglviewer::Camera *camera, const QRect &rect, PERSSELECTMODE mode)
{
    QList<JointData *> tmp_jdlist;
    QList<MemberData *> tmp_mdlist;

    Q_FOREACH ( JointData *jd, jointList ) {
        qglviewer::Vec sp = camera->projectedCoordinatesOf(
                     qglviewer::Vec( jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos(true) * 1000.0 ) );
        QPoint pp(qRound(sp.x), qRound(sp.y));
        if ( rect.contains(pp) ) {
            if ( mode == PERSSELECTMODE::PERS_ADDSELECT ) appendSelectedJoint(jd);
            if ( mode == PERSSELECTMODE::PERS_REMOVESELECT ) removeSelectedJoint(jd);
            if ( !tmp_jdlist.contains(jd) ) tmp_jdlist.append(jd);
            Q_FOREACH ( MemberData *md, jd->getRelatedMembers() ) {
                if ( !md->isAnalysisMember(*UnifiedFixedData::getInstance()->getAnalysisData()) ) continue;
                if ( !tmp_mdlist.contains(md) ) tmp_mdlist.append(md);
            }
        }
    }

    Q_FOREACH ( MemberData *md, tmp_mdlist) {
        bool isContain = true;
        Q_FOREACH ( JointData *m_jd, md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getAnalysisData()) ) {
            if ( !tmp_jdlist.contains(m_jd) ) {
                isContain = false;
                break;
            }
        }
        if ( isContain ) {
            if ( mode == PERSSELECTMODE::PERS_ADDSELECT ) appendSelectedMember(md);
            if ( mode == PERSSELECTMODE::PERS_REMOVESELECT ) removeSelectedMember(md);
        }
    }

//    emit selectionChanged();
}

void PersOutputScene::appendSelectedMember(MemberData *md)
{
    if ( !selectedMembers.contains(md) ) selectedMembers.append(md);
}

void PersOutputScene::removeSelectedMember(MemberData *md)
{
    if ( selectedMembers.contains(md) ) selectedMembers.removeOne(md);
}

void PersOutputScene::appendSelectedJoint(JointData *jd)
{
    if ( !selectedJoints.contains(jd) ) selectedJoints.append(jd);
}

void PersOutputScene::removeSelectedJoint(JointData *jd)
{
    if ( selectedJoints.contains(jd) ) selectedJoints.removeOne(jd);
}


void PersOutputScene::createLineMember(const QList<qglviewer::Vec> &plist )
{
    glDisable(GL_LIGHTING);

    glBegin(GL_LINE_STRIP);
    for ( int i = 0; i < plist.count(); i++ ) {
        qglviewer::Vec vv = plist.at(i);
        glVertex3d( vv.x, vv.y, vv.z );
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void PersOutputScene::createClosedMember( const QList<qglviewer::Vec> &tplist )
{
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(hatch);

    glBegin(GL_TRIANGLES);
    Q_FOREACH ( qglviewer::Vec vv, tplist ) glVertex3d( vv.x, vv.y, vv.z + 3.0 );
    glEnd();

    glDisable(GL_POLYGON_STIPPLE);
}

void PersOutputScene::drawXZText(const QString &name, qreal swidth, qreal shx, qreal shy)
{
    QList<RXYLINE> linelist = ascii_table.stringToXY(name, swidth, swidth * 1.25, shx, shy);

    glBegin(GL_LINES);
    Q_FOREACH (RXYLINE line, linelist) {
        glVertex3d(line.x1,   0.0, line.y1 );
        glVertex3d(line.x2,   0.0, line.y2 );
    }
    glEnd();
}

ELEMENTTYPE PersOutputScene::dataTypeToElementType(DATATYPE dtype) const
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

QString PersOutputScene::dummyName(DATATYPE dtype) const
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

void PersOutputScene::recalcAnalysisJointsOfMember( MemberData *md )
{
    Q_FOREACH ( JointData *jd, md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getAnalysisData()) ) {
        if ( !jointList.contains(jd)
                && isJointOkToDisplayLimited(jd) ) jointList.append(jd);
    }
}

} // namespace post3dapp
