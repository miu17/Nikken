#include "analysisgroup_glview.h"

#include "unified_data.h"
#include "unified_inputdata.h"
#include "unified_settings.h"
#include "calc_vector2d.h"

#include <QDebug>
#include "manage_memberdata.h"
#include "manage_jointdata.h"

using namespace std;
using namespace qglviewer;

namespace post3dapp{

AnalysisGroupGLScene::AnalysisGroupGLScene( QObject *parent )
    : QObject(parent)
{
    myElementType=ALLELEMENT;

    connect ( UnifiedInputData::getInInstance(), &UnifiedInputData::dataCleared,
              this, &AnalysisGroupGLScene::clearCurrentData);
    connect ( UnifiedInputData::getInInstance(), &UnifiedInputData::dataReaded,
              this, &AnalysisGroupGLScene::initDisplayList);

    connect(UnifiedInputData::getInInstance(), &UnifiedInputData::jointAppended,
            this, &AnalysisGroupGLScene::createJointItems);
    connect(UnifiedInputData::getInInstance(), &UnifiedInputData::jointRemoved,
            this, &AnalysisGroupGLScene::removeJointItems);
    connect(UnifiedInputData::getInInstance(),&UnifiedInputData::jointChanged,
            this, &AnalysisGroupGLScene::changeJointItems);

    connect(UnifiedInputData::getInInstance(),SIGNAL(memberAppended(const QList<MemberData*>&)),
            this,SLOT(createMemberItems(const QList<MemberData*>&)));
    connect(UnifiedInputData::getInInstance(),SIGNAL(memberRemoved(const QList<MemberData*>&,DATATYPE)),
            this,SLOT(removeMemberItems(const QList<MemberData*>&,DATATYPE)));
    connect(UnifiedInputData::getInInstance(), &UnifiedInputData::memberChanged,
            this, &AnalysisGroupGLScene::changeMemberItems);
}

AnalysisGroupGLScene::~AnalysisGroupGLScene()
{
    clearSelectedItems();
    clearCurrentData();
}

void AnalysisGroupGLScene::clearCurrentData()
{
    closedMemberTriangles.clear();
    memberList.clear();
    jointList.clear();
}


void AnalysisGroupGLScene::initDisplayList()
{
    displayLimitedFloor=QUuid();
    displayLimitedFrame=QUuid();
    myElementType=ALLELEMENT;

    reloadViewItems();
}

void AnalysisGroupGLScene::reloadViewItems()
{
    clearCurrentData();
    createMemberItems( UnifiedInputData::getInInstance()->memberListOfAll() );
    calcBoundingVec();
}

void AnalysisGroupGLScene::setLimitedFloor( const QString & str_fl )
{
    displayLimitedFloor=UnifiedInputData::getInInstance()->nameToID(str_fl,DATATYPE::TPFLOOR);
    reloadViewItems();
}

void AnalysisGroupGLScene::setLimitedFrame( const QString & str_fr )
{
    displayLimitedFrame=UnifiedInputData::getInInstance()->nameToID(str_fr,DATATYPE::TPFRAMEP);
    reloadViewItems();
}

void AnalysisGroupGLScene::setLimitedElement( ELEMENTTYPE el )
{
    myElementType=el;
}

void AnalysisGroupGLScene::setSelectedItems( const QList<JointData*> &jdlist,
                                             const QList<MemberData*> &mdlist )
{
    selectedJoints=jdlist;
    selectedMembers=mdlist;
}

void AnalysisGroupGLScene::clearSelectedItems()
{
    selectedMembers.clear();
    selectedJoints.clear();
    emit selectionChanged();
}

bool AnalysisGroupGLScene::isMemberOkToDisplayLimited( MemberData* md ) const
{
    bool fl_ok=true;
    if( !displayLimitedFloor.isNull() ){
        fl_ok=md->containsFloor( displayLimitedFloor );
    }

    bool fr_ok=true;
    if( !displayLimitedFrame.isNull() ){
        fr_ok=md->containsFrame( displayLimitedFrame );
    }

    return ( fl_ok && fr_ok ) ;
}

bool AnalysisGroupGLScene::isJointOkToDisplayLimited( JointData* jd ) const
{
    bool fl_ok=true;
    if( !displayLimitedFloor.isNull() ){
        fl_ok=jd->containsFloor( displayLimitedFloor );
    }

    bool fr_ok=true;
    if( !displayLimitedFrame.isNull() ){
        fr_ok=jd->containsFrame( displayLimitedFrame );
    }

    return ( fl_ok && fr_ok ) ;
}

void AnalysisGroupGLScene::createJointItems( const QList<JointData*>& jdlist )
{
//    qDebug() << "createJointItems" << jdlist.count();
    Q_FOREACH( JointData* jd, jdlist ){
        if( !isJointOkToDisplayLimited(jd) ) continue;
        if( !jd->isAnalysisJoint(*UnifiedInputData::getInInstance()) ) continue;
        if( !jointList.contains(jd) ) jointList.append(jd);
    }
   // childViewer->updateGL();
    calcBoundingVec();
}

void AnalysisGroupGLScene::removeJointItems( const QList<JointData*>& jdlist )
{
//    qDebug() << "removeJointItems" << jdlist.count();
//    clearSelectedItems();
    bool isDisplayChange=false;
    bool isSelectChange=false;

    Q_FOREACH( JointData* jd, jdlist ){
        int idx=jointList.indexOf(jd);
        if( idx>=0 ){
            jointList.removeAt(idx);
            isDisplayChange=true;
        }
        idx=selectedJoints.indexOf(jd);
        if( idx>=0 ){
            selectedJoints.removeAt(idx);
            isSelectChange=true;
        }
    }
    if( isDisplayChange ) {
      //  childViewer->updateGL();
        calcBoundingVec();
    }
    if( isSelectChange ) emit selectionChanged();
}

void AnalysisGroupGLScene::changeJointItems( const QList<JointData*>& jdlist )
{
//   qDebug() << "changeJointItems" << jdlist.count();
    bool isDisplayChange=false;
    bool isSelectChange=false;

    Q_FOREACH( JointData* jd, jdlist ){

        if( !jd->isAnalysisJoint(*UnifiedInputData::getInInstance()) ){
            int idx=selectedJoints.indexOf(jd);
            if( idx>=0 ){
                selectedJoints.removeAt(idx);
                isSelectChange=true;
            }
            idx=jointList.indexOf(jd);
            if( idx>=0 ){
                jointList.removeAt(idx);
                isDisplayChange=true;
            }
            continue;
        }

        int idx=jointList.indexOf(jd);
        bool isOk=isJointOkToDisplayLimited(jd);
        if( idx>=0 ){
            if( !isOk ){
                jointList.removeAt(idx);
                isDisplayChange=true;
            }
        }else{
            if( isOk ){
                jointList.append(jd);
                isDisplayChange=true;
            }
        }
    }

    if( isDisplayChange ) {
       // childViewer->updateGL();
        calcBoundingVec();
    }

    if( isSelectChange ) emit selectionChanged();
}

void AnalysisGroupGLScene::createMemberItems( const QList<MemberData*>& mdlist, bool updateOn )
{
    bool isDisplayChange=false;

    Q_FOREACH( MemberData* md, mdlist ){

        if( !md->isAnalysisMember(*UnifiedInputData::getInInstance()) ) continue;

        isDisplayChange=true;
        recalcAnalysisJointsOfMember(md);

        if( !isMemberOkToDisplayLimited(md) ) continue;
        if( memberList.contains(md) ) continue;

        memberList.append( md );
        if( md->isClosedMember(false) ){
            closedMemberTriangles.insert(md,divideClosedMember(md));
        }
    }

    //if( updateOn && isDisplayChange ) childViewer->updateGL();
}

QList<Vec> AnalysisGroupGLScene::divideClosedMember( MemberData* md ) const
{
    QList<Vec> tmpPoints;
    Q_FOREACH( JointData* jd, md->getAnalysisJointList(*UnifiedInputData::getInInstance()) ){
        tmpPoints.append( Vec( jd->xPos()*1000.0,jd->yPos()*1000.0,jd->zPos(true)*1000.0 ) );
    }
    QList<Vec> dividedPoints;

    int cur_idx,pre_idx,next_idx;
    Vec v0,v1,v2,vc;

    while( tmpPoints.count()>3 ){

        qreal max_len=0.0;
        for( int i=0; i<tmpPoints.count(); i++ ){
            qreal len=qAbs(tmpPoints.at(i).norm());
            if( len>max_len  ){ cur_idx=i; max_len=len; }
        }

        pre_idx=( cur_idx>0 ) ? cur_idx-1 : tmpPoints.count()-1 ;
        next_idx=( cur_idx<tmpPoints.count()-1 ) ? cur_idx+1 : 0 ;

        v0=tmpPoints.at(pre_idx);
        v1=tmpPoints.at(cur_idx);
        v2=tmpPoints.at(next_idx);

        bool isInside=false;
        for( int i=0; i<tmpPoints.count(); i++ ){
            if( i==pre_idx || i==cur_idx || i==next_idx ) continue;
            Vec vv=tmpPoints.at(i);
            Vec a1=(v1-v0)^(v0-vv);
            Vec a2=(v2-v1)^(v1-vv);
            Vec a3=(v0-v2)^(v2-vv);
            if( ( a1*a2 > 0.0001 ) && ( a1*a3 > 0.0001 ) ){ isInside=true; break; }
        }

        if( !isInside ){
            dividedPoints.append(v0);
            dividedPoints.append(v1);
            dividedPoints.append(v2);
            tmpPoints.removeAt(cur_idx);
            continue;
        }

        vc=(v0-v1)^(v2-v1);
        int start_id=cur_idx;

        while( 1 ){

            if( start_id==next_idx ) break;

            cur_idx=next_idx;
            pre_idx=( cur_idx>0 ) ? cur_idx-1 : tmpPoints.count()-1 ;
            next_idx=( cur_idx<tmpPoints.count()-1 ) ? cur_idx+1 : 0 ;

            v0=tmpPoints.at(pre_idx);
            v1=tmpPoints.at(cur_idx);
            v2=tmpPoints.at(next_idx);

            Vec cur_vc=(v0-v1)^(v2-v1);
            if( vc*cur_vc > -1.0e-3 ){

                isInside=false;
                for( int i=0; i<tmpPoints.count(); i++ ){
                    if( i==pre_idx || i==cur_idx || i==next_idx ) continue;
                    Vec vv=tmpPoints.at(i);
                    Vec a1=(v1-v0)^(v0-vv);
                    Vec a2=(v2-v1)^(v1-vv);
                    Vec a3=(v0-v2)^(v2-vv);
                    if( ( a1*a2 > 0.0001 ) && ( a1*a3 > 0.0001 ) ){ isInside=true; break; }
                }

                if( !isInside ){
                    dividedPoints.append(v0);
                    dividedPoints.append(v1);
                    dividedPoints.append(v2);
                    tmpPoints.removeAt(cur_idx);
                    break;
                }

             }

        }

    }

    Q_FOREACH( Vec vv, tmpPoints ) dividedPoints.append(vv);

    return dividedPoints;
}

void AnalysisGroupGLScene::removeMemberItems( const QList<MemberData*>& mdlist,
                                              DATATYPE dtype, bool updateOn )
{
    if( dtype==DATATYPE::TPJOINTLOAD || dtype==DATATYPE::TPMEMBERLOAD || dtype==DATATYPE::TPSLABLOAD || dtype==DATATYPE::TPBEAM ) return;

    bool isDisplayChange=false;

    Q_FOREACH( MemberData* md, mdlist ){
        int idx=memberList.indexOf(md);
        if( idx<0 ) continue;

        isDisplayChange=true;
        removeSelectedMember(md);
        memberList.removeAt(idx);
//        GLuint id=memberDisplayList.at(idx);
//        if( glIsList(id) ) glDeleteLists(id,1);
//        memberDisplayList.removeAt(idx);
        if( dtype!=DATATYPE::TPWALL || dtype!=DATATYPE::TPSLAB ) continue;
        closedMemberTriangles.remove(md);
    }
//    qDebug() << "removeMemberItems end";
    // 処理後にupdate()をかける場合、行方不明のJointData*が発生するおそれ
//    if( updateOn && isDisplayChange ) childViewer->updateGL();
}

void AnalysisGroupGLScene::changeMemberItems( const QList<MemberData*>& mdlist )
{
    QMultiHash<int,MemberData*> removed_mdtable;
    bool isDisplayChange=false;
    Q_FOREACH( MemberData* md, mdlist ){
        DATATYPE dtype=md->dataType();
        if( dtype==DATATYPE::TPJOINTLOAD || dtype==DATATYPE::TPMEMBERLOAD || dtype==DATATYPE::TPSLABLOAD || dtype==DATATYPE::TPBEAM ) continue;
//        if( dtype==TPBEAM || dtype==TPSLAB || dtype==TPUWALL ) continue;
        isDisplayChange=true;
        removed_mdtable.insert(static_cast<int>(dtype),md);
    }

    Q_FOREACH( int t, removed_mdtable.uniqueKeys() ){
        DATATYPE type = static_cast<DATATYPE>(t);
        removeMemberItems( removed_mdtable.values(t), type, false );
        createMemberItems( removed_mdtable.values(t), false );
    }

   // if( isDisplayChange ) childViewer->updateGL();
}

void AnalysisGroupGLScene::calcBoundingVec()
{
    qreal x1=1000000.,y1=1000000.,z1=1000000.;
    qreal x2=-1000000.,y2=-1000000.,z2=-1000000.;

    if( jointList.isEmpty() ){
        x1= -5000.; y1= -5000.; z1= -5000.;
        x2= 15000.; y2= 15000.; z2= 15000.;
    }else{
        Q_FOREACH(JointData* jd, jointList){
            x1=qMin(x1,jd->xPos()*1000.0);
            x2=qMax(x2,jd->xPos()*1000.0);
            y1=qMin(y1,jd->yPos()*1000.0);
            y2=qMax(y2,jd->yPos()*1000.0);
            z1=qMin(z1,jd->zPos(true)*1000.0);
            z2=qMax(z2,jd->zPos(true)*1000.0);
        }
    }

    boundMinVec=Vec(x1,y1,z1);
    boundMaxVec=Vec(x2,y2,z2);
}

void AnalysisGroupGLScene::drawSolid( bool withName )
{
    /*
    bool lineVisible=UnifiedSettings::getInstance()->getOutput3DViewSettings().isLineVisible;
    bool planeVisible=UnifiedSettings::getInstance()->getOutput3DViewSettings().isPlaneVisible;
    QColor lineColor=UnifiedSettings::getInstance()->getOutput3DViewSettings().lineColor;
    QColor planeColor=UnifiedSettings::getInstance()->getOutput3DViewSettings().planeColor;
    QColor selectColor=UnifiedSettings::getInstance()->getOutput3DViewSettings().selectColor;
    */

    bool lineVisible= defaultLineVisible;
    bool planeVisible = defaultPlaneVisible;
    QColor lineColor = defaultLineColor;
    QColor planeColor = defaultPlaneColor;
    QColor selectColor = defaultSelectColor;


    for( int i=0; i < memberList.count() ; i++ ){

        MemberData* md=memberList.at(i);
        bool isSelected= ( !withName && selectedMembers.contains(md) );

        if( md->isLineMember(false) ){

            if( !lineVisible ) continue;
            QColor col=( isSelected ) ? selectColor : lineColor ;
            glColor3f(col.redF(), col.greenF(), col.blueF());

            QList<Vec> vlist;
            Q_FOREACH( JointData* jd, md->getAnalysisJointList(*UnifiedInputData::getInInstance()) ){
                vlist.append( Vec(jd->xPos()*1000.0, jd->yPos()*1000.0, jd->zPos(true)*1000.0) );
            }
            createLineMember(vlist);

        }else if( md->isClosedMember(false) ){

            if( !planeVisible ) continue;
            QColor col=( isSelected ) ? selectColor : planeColor ;
            glColor3f(col.redF(), col.greenF(), col.blueF());

            createClosedMember(closedMemberTriangles.value(md));

        }

    }
}

void AnalysisGroupGLScene::drawMemberText( const Vec& v_r, const Vec& v_u )
{
    if( !UnifiedSettings::getInstance()->getOutput3DViewSettings().getIsMemberNameVisible() ) return;

//    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // int isize = UnifiedSettings::getInstance()->getOutput3DViewSettings().nameSize;
    int isize = defaultNameSize;
    qreal swidth=200.0+qreal(isize)*40.0;

    // カメラベクトルと正対するような座標変換
    GLdouble theta1,theta2;
    Vec v1,v2;
    Vec a1(1.0,0.0,0.0);
    Quaternion q1(a1,v_r);
    theta1=q1.angle()*180.0/M_PI;
    v1=q1.axis();
    qreal v1_norm=v1.norm();
    Vec a2(0.0,0.0,1.0);
    Vec a3=q1.rotate(a2);
    Quaternion q2(a3,v_u);
    theta2=q2.angle()*180.0/M_PI;
    v2=q2.axis();
    qreal v2_norm=v2.norm();

    /*
    bool lineVisible=UnifiedSettings::getInstance()->getOutput3DViewSettings().isLineVisible;
    bool planeVisible=UnifiedSettings::getInstance()->getOutput3DViewSettings().isPlaneVisible;
    QColor nameColor=UnifiedSettings::getInstance()->getOutput3DViewSettings().nameColor;
    */
    bool lineVisible = defaultLineVisible;
    bool planeVisible = defaultPlaneVisible;
    QColor nameColor = defaultNameColor;


    for( int i=0; i < memberList.count() ; i++ ){

        MemberData* md=memberList.at(i);

        if( md->isLineMember(false) && !lineVisible ) continue;
        if( md->isClosedMember(false) && !planeVisible ) continue;
        glColor3f(nameColor.redF(), nameColor.greenF(), nameColor.blueF());

        QString sectionName=md->sectionName();
        if( sectionName.isEmpty() ) sectionName=dummyName(md->dataType());

        qreal shiftX=0.0;
        qreal shiftY=0.0;

        glPushMatrix();

        if( md->isLineMember(false) ){

            HLINESHAPE shape=md->getHLineShape();

            XYZ i_xyz=md->iJoint()->xyz();
            XYZ j_xyz=md->jJoint()->xyz();

            XYZ tr_xyz;
            if( md->dataType()==DATATYPE::TPBRACE ){
                if( i_xyz.z<j_xyz.z ){
                    tr_xyz=XYZ( 750.0*i_xyz.x+250.0*j_xyz.x,750.0*i_xyz.y+250.0*j_xyz.y,750.0*i_xyz.z+250.0*j_xyz.z );
                }else{
                    tr_xyz=XYZ( 250.0*i_xyz.x+750.0*j_xyz.x,250.0*i_xyz.y+750.0*j_xyz.y,250.0*i_xyz.z+750.0*j_xyz.z );
                }
            }else{
                tr_xyz=XYZ( (j_xyz.x+i_xyz.x)*500.0,(j_xyz.y+i_xyz.y)*500.0,(j_xyz.z+i_xyz.z)*500.0 );
            }

            glTranslatef(tr_xyz.x,tr_xyz.y,tr_xyz.z+10.0);

            if( md->dataType()==DATATYPE::TPCOLUMN ){
                shiftX=qMax(shape.leftSizeB,shape.leftSizeH);
                shiftX=qMax(shiftX,shape.rightSizeB);
                shiftX=qMax(shiftX,shape.rightSizeH);
                shiftX=shiftX/2.0+100.0;
                shiftY=-swidth*1.25/2.0;
            }else{
                shiftX=-swidth*sectionName.count()/2.0;
                shiftY=100.0;
            }

        }else if( md->isClosedMember(false) ){

            XYZ tr_xyz(0.0,0.0,0.0);
            QList<JointData*> jdlist=md->getJointList();

            int count=0;
            Q_FOREACH( JointData* jd, jdlist ){
                tr_xyz.x+=jd->xPos();
                tr_xyz.y+=jd->yPos();
                tr_xyz.z+=jd->zPos(true);
                count++;
            }

            tr_xyz.x=tr_xyz.x/qreal(count)*1000.0;
            tr_xyz.y=tr_xyz.y/qreal(count)*1000.0;
            tr_xyz.z=tr_xyz.z/qreal(count)*1000.0;

            glTranslatef(tr_xyz.x,tr_xyz.y,tr_xyz.z+10.0);
            shiftX=-swidth*sectionName.count()/2.0;
            shiftY=100.0;

        }

        if( v2_norm > 0.001 ) glRotatef(theta2,v2.x,v2.y,v2.z);
        if( v1_norm > 0.001 ) glRotatef(theta1,v1.x,v1.y,v1.z);

        drawXZText(sectionName,swidth,shiftX,shiftY);

        glPopMatrix();

    }
    glEnable(GL_LIGHTING);
}

void AnalysisGroupGLScene::drawJointText( const Vec& v_r, const Vec& v_u )
{
    //if( !UnifiedSettings::getInstance()->getOutput3DViewSettings().isJnoVisible ) return;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    //QColor color=UnifiedSettings::getInstance()->getOutput3DViewSettings().jnoColor;
    QColor color = defaultJnoColor;
    glColor3f(color.redF(), color.greenF(), color.blueF());
//    int isize=UnifiedSettings::getInstance()->getOutput3DViewSettings().jnoSize;
    int isize = defaultJnoSize;
    qreal swidth=100.0+qreal(isize)*30.0;

    // カメラベクトルと正対するような座標変換
    GLdouble theta1,theta2;
    Vec v1,v2;
    Vec a1(1.0,0.0,0.0);
    Quaternion q1(a1,v_r);
    theta1=q1.angle()*180.0/M_PI;
    v1=q1.axis();
    qreal v1_norm=v1.norm();
    Vec a2(0.0,0.0,1.0);
    Vec a3=q1.rotate(a2);
    Quaternion q2(a3,v_u);
    theta2=q2.angle()*180./M_PI;
    v2=q2.axis();
    qreal v2_norm=v2.norm();

    Q_FOREACH( JointData* jd, jointList ){

        qreal shiftX=0.0;
        qreal shiftY=100.0;

        QString jno=QString("%1").arg(UnifiedInputData::getInInstance()->indexOfJoint(jd)+1);
        shiftX=-swidth*jno.count()/2.0;

        glPushMatrix();
        glTranslatef(jd->xPos()*1000.0,jd->yPos()*1000.0,jd->zPos(true)*1000.0);

        if( v2_norm > 0.001 ) glRotatef(theta2,v2.x,v2.y,v2.z);
        if( v1_norm > 0.001 ) glRotatef(theta1,v1.x,v1.y,v1.z);

        drawXZText(jno,swidth,shiftX,shiftY);

        glPopMatrix();

    }

    glEnable(GL_LIGHTING);
}

void AnalysisGroupGLScene::drawPoints()
{
    // if( !UnifiedSettings::getInstance()->getOutput3DViewSettings().isJointVisible ) return;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glPointSize(3.0);
    // QColor n_col=UnifiedSettings::getInstance()->getOutput3DViewSettings().jointColor;
    // QColor s_col=UnifiedSettings::getInstance()->getOutput3DViewSettings().selectColor;
    QColor n_col = defaultJointColor;
    QColor s_col = defaultSelectColor;

    Q_FOREACH( JointData* jd, jointList ){

        ( selectedJoints.contains(jd) ) ?
                    glColor3f(s_col.redF(), s_col.greenF(), s_col.blueF()) :
                    glColor3f(n_col.redF(), n_col.greenF(), n_col.blueF()) ;

        glBegin(GL_POINTS);
        glVertex3d(jd->xPos()*1000.0,jd->yPos()*1000.0,jd->zPos(true)*1000.0);
        glEnd();
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void AnalysisGroupGLScene::drawFrameScale( const Vec& v_r, const Vec& v_u )
{
//    if( !UnifiedSettings::getInstance()->getOutput3DViewSettings().isFrScaleVisible ) return;

    //QUuid myFloorID=UnifiedSettings::getInstance()->getOutput3DViewSettings().frScaleFloor;
    // if( myFloorID.isNull() ) return;
    auto floorList = UnifiedInputData::getInInstance()->getFloorList();
    if (floorList.count() == 0)
        return;
    QUuid myFloorID = floorList.first().UUID;

    glDisable(GL_LIGHTING);
    qreal zz=UnifiedInputData::getInInstance()->getFloorHeight(myFloorID)*1000.0;

    // カメラベクトルと正対するような座標変換
    GLdouble theta1,theta2;
    Vec v1,v2;
    Vec a1(1.0,0.0,0.0);
    Quaternion q1(a1,v_r);
    theta1=q1.angle()*180.0/M_PI;
    v1=q1.axis();
    qreal v1_norm=v1.norm();
    Vec a2(0.0,0.0,1.0);
    Vec a3=q1.rotate(a2);
    Quaternion q2(a3,v_u);
    theta2=q2.angle()*180./M_PI;
    v2=q2.axis();
    qreal v2_norm=v2.norm();

    //QColor color=UnifiedSettings::getInstance()->getOutput3DViewSettings().scaleColor;
    QColor color = defaultScaleColor;
    glColor3f(color.redF(), color.greenF(), color.blueF());
    //int isize=UnifiedSettings::getInstance()->getOutput3DViewSettings().nameSize;
    int isize = defaultNameSize;
    qreal swidth=200.0+qreal(isize)*50.0;
    glPointSize(3.0);

    QList<FRAMEPOINTSVALUE> drawFrameList=UnifiedInputData::getInInstance()->getFrameValuesOfFloor(myFloorID);
    QPointF p1,p2,p3;

    Q_FOREACH(FRAMEPOINTSVALUE fp,drawFrameList){

        if( fp.pointList.count()<2 ) continue;

        glBegin(GL_POINTS);
        for( int i=0; i<fp.pointList.count(); i++ ){
            p1=fp.pointList.at(i);
            glVertex3d( p1.x()*1000.0, p1.y()*1000.0, zz );
        }
        glEnd();

        glEnable(GL_LINE_STIPPLE);
        glLineStipple(2,0xAAAA);

        glBegin(GL_LINE_STRIP);
        for( int i=1; i<fp.pointList.count(); i++ ){
            p1=fp.pointList.at(i-1);
            p2=fp.pointList.at(i);
            glVertex3d( p1.x()*1000.0, p1.y()*1000.0, zz );
            glVertex3d( p2.x()*1000.0, p2.y()*1000.0, zz );
        }
        if( fp.frameType==FrameType::FRAMETYPE::CLCIRCLE || fp.frameType==FrameType::FRAMETYPE::CLVARIABLE  ){
            p1=fp.pointList.last();
            p2=fp.pointList.first();
            glVertex3d( p1.x()*1000.0, p1.y()*1000.0, zz );
            glVertex3d( p2.x()*1000.0, p2.y()*1000.0, zz );
        }
        glEnd();

        glDisable(GL_LINE_STIPPLE);

        // 通り名
        glPushMatrix();

        p3=fp.namePoint;
        glTranslatef( p3.x()*1000.0, p3.y()*1000.0, zz );

        if( v2_norm > 0.001 ) glRotatef(theta2,v2.x,v2.y,v2.z);
        if( v1_norm > 0.001 ) glRotatef(theta1,v1.x,v1.y,v1.z);

        glBegin(GL_LINE_LOOP);
        for( int i=0; i<12; i++ ){
            qreal theta=30.0*qreal(i)/180.0*M_PI;
            glVertex3d( 1000.0*qCos(theta), 0.0, 1000.0*qSin(theta) );
        }
        glEnd();

        QString fname=fp.frameName;
        qreal shx=-qreal(fname.count())*swidth/2.0;
        qreal shy=-swidth*1.25/2.0;
        drawXZText(fname,swidth,shx,shy);

        glPopMatrix();

        // 直線通りのスパン（文字＋ライン）
        if( fp.frameType==FrameType::FRAMETYPE::STRAIGHT ){

            p1=fp.straight.spoint;
            p1*=1000.0;
            qreal span=fp.straight.span*1000.0;
            qreal angle=fp.straight.angle/180.0*M_PI;
            QString tstr=QString("%1").arg( span ,0,'f',0 );
            p2=p1+QPointF( span/2.0*qCos(angle), span/2.0*qSin(angle) );

            glPushMatrix();

            glTranslatef( p2.x(), p2.y(), zz );

            if( v2_norm > 0.001 ) glRotatef(theta2,v2.x,v2.y,v2.z);
            if( v1_norm > 0.001 ) glRotatef(theta1,v1.x,v1.y,v1.z);

            shx=-qreal(tstr.count())*swidth*0.8/2.0;
            shy=100.0;
            drawXZText(tstr,swidth*0.8,shx,shy);

            glPopMatrix();

            p2=p1+QPointF( span*qCos(angle), span*qSin(angle) );
            p3=p2+QPointF( 2000.0*qCos(angle+M_PI/2.0), 2000.0*qSin(angle+M_PI/2.0) );

            glBegin(GL_LINE_STRIP);
            glVertex3d( p1.x(), p1.y(), zz );
            glVertex3d( p2.x(), p2.y(), zz );
            glVertex3d( p3.x(), p3.y(), zz );
            glEnd();

            if( fp.straight.isFirst || fp.straight.isLast ){
                p2=p1+QPointF( 2000.0*qCos(angle+M_PI/2.0), 2000.0*qSin(angle+M_PI/2.0) );
                glBegin(GL_LINES);
                glVertex3d( p1.x(), p1.y(), zz );
                glVertex3d( p2.x(), p2.y(), zz );
                glEnd();
            }
        }
    }

    glEnable(GL_LIGHTING);
}

void AnalysisGroupGLScene::drawFloorScale( const qglviewer::Vec& v_r, const qglviewer::Vec& v_u )
{
    //if( !UnifiedSettings::getInstance()->getOutput3DViewSettings().isFlScaleVisible ) return;

    glDisable(GL_LIGHTING);
    //QPointF op=UnifiedSettings::getInstance()->getOutput3DViewSettings().floorScaleXY*1000.0;
    QPointF op = defaultFloorScaleXY * 1000.0;

    // カメラベクトルと正対するような座標変換
    GLdouble theta1,theta2;
    Vec v1,v2;
    Vec a1(1.0,0.0,0.0);
    Quaternion q1(a1,v_r);
    theta1=q1.angle()*180.0/M_PI;
    v1=q1.axis();
    qreal v1_norm=v1.norm();
    Vec a2(0.0,0.0,1.0);
    Vec a3=q1.rotate(a2);
    Quaternion q2(a3,v_u);
    theta2=q2.angle()*180./M_PI;
    v2=q2.axis();
    qreal v2_norm=v2.norm();

    // QColor color=UnifiedSettings::getInstance()->getOutput3DViewSettings().scaleColor;
    QColor color = defaultScaleColor;
    glColor3f(color.redF(), color.greenF(), color.blueF());
    //int isize=UnifiedSettings::getInstance()->getOutput3DViewSettings().nameSize;
    int isize = defaultNameSize;
    qreal swidth=200.0+qreal(isize)*50.0;
    glPointSize(3.0);

    QList<FLOORVALUES> floorList=UnifiedInputData::getInInstance()->getFloorList();
    QStringList nameList;
    QList<qreal> heightList;

    for( int i=0; i<floorList.count(); i++ ){
        FLOORVALUES cur_fv=floorList.at(i);
        if( cur_fv.floorType==FLOORTYPE::FL_CHILD ) continue;
        nameList.append(cur_fv.analysisName);
        heightList.append((cur_fv.floorHeight+cur_fv.adjustedLevel)*1000.0 );
    }

    for( int i=0; i<nameList.count(); i++ ){

        qreal cur_z=heightList.at(i);

        glBegin(GL_POINTS);
        glVertex3d( op.x(), op.y(), cur_z );
        glEnd();

        glPushMatrix();

        glTranslatef( op.x(), op.y(), cur_z );

        if( v2_norm > 0.001 ) glRotatef(theta2,v2.x,v2.y,v2.z);
        if( v1_norm > 0.001 ) glRotatef(theta1,v1.x,v1.y,v1.z);

        glBegin(GL_LINES);
        glVertex3d( 0.0f, 0.0f, 0.0f );
        glVertex3d( -4000.0f, 0.0f, 0.0f );
        glEnd();

        QString floorName=nameList.at(i);
        qreal shx=-2000.0-qreal(floorName.count())*swidth/2.0;
        qreal shy=100.0;
        drawXZText(floorName,swidth,shx,shy);

        glPopMatrix();

        if( i>nameList.count()-2 ) break;

        qreal next_z=heightList.at(i+1);

        glBegin(GL_LINES);
        glVertex3d( op.x(), op.y(), cur_z );
        glVertex3d( op.x(), op.y(), next_z );
        glEnd();

        glPushMatrix();

        glTranslatef( op.x(), op.y(), (cur_z+next_z)/2.0 );

        if( v2_norm > 0.001 ) glRotatef(theta2,v2.x,v2.y,v2.z);
        if( v1_norm > 0.001 ) glRotatef(theta1,v1.x,v1.y,v1.z);

        QString strh=QString("%1").arg( next_z-cur_z ,0,'f',0 );
        shx=-100.0-qreal(strh.count())*swidth*0.8;
        shy=-swidth*1.25*0.8/2.0;
        drawXZText(strh,swidth*0.8,shx,shy);

        glPopMatrix();

    }
}

void AnalysisGroupGLScene::selectPoints(Camera* camera, const QRect& rect, PERSSELECTMODE mode)
{
    QList<JointData*> tmp_jdlist;
    QList<MemberData*> tmp_mdlist;

    Q_FOREACH( JointData* jd, jointList ){
        Vec sp = camera->projectedCoordinatesOf(
                    Vec( jd->xPos()*1000.0, jd->yPos()*1000.0, jd->zPos(true)*1000.0 ) );
        QPoint pp(qRound(sp.x),qRound(sp.y));
        if( rect.contains(pp) ){
            if( mode==PERSSELECTMODE::PERS_ADDSELECT ) appendSelectedJoint(jd);
            if( mode==PERSSELECTMODE::PERS_REMOVESELECT ) removeSelectedJoint(jd);
            if( !tmp_jdlist.contains(jd) ) tmp_jdlist.append(jd);
            Q_FOREACH( MemberData* md, jd->getRelatedMembers() ){
                if( !md->isAnalysisMember(*UnifiedInputData::getInInstance()) ) continue;
                if( !tmp_mdlist.contains(md) ) tmp_mdlist.append(md);
            }
        }
    }

    Q_FOREACH( MemberData* md, tmp_mdlist){
        bool isContain=true;
        Q_FOREACH( JointData* m_jd, md->getAnalysisJointList(*UnifiedInputData::getInInstance()) ){
            if( !tmp_jdlist.contains(m_jd) ){ isContain=false; break; }
        }
        if( isContain ){
            if( mode==PERSSELECTMODE::PERS_ADDSELECT ) appendSelectedMember(md);
            if( mode==PERSSELECTMODE::PERS_REMOVESELECT ) removeSelectedMember(md);
        }
    }

    emit selectionChanged();
}

void AnalysisGroupGLScene::appendSelectedMember(MemberData* md)
{
    if( myElementType!=ALLELEMENT && myElementType!=dataTypeToElementType(md->dataType()) ) return;
    if( !selectedMembers.contains(md) ) selectedMembers.append(md);
}

void AnalysisGroupGLScene::removeSelectedMember(MemberData* md)
{
    if( myElementType!=ALLELEMENT && myElementType!=dataTypeToElementType(md->dataType()) ) return;
    if( selectedMembers.contains(md) ) selectedMembers.removeOne(md);
}

void AnalysisGroupGLScene::appendSelectedJoint(JointData* jd)
{
    if( myElementType!=ALLELEMENT && myElementType!=ELJOINT ) return;
    if( !selectedJoints.contains(jd) ) selectedJoints.append(jd);
}

void AnalysisGroupGLScene::removeSelectedJoint(JointData* jd)
{
    if( myElementType!=ALLELEMENT && myElementType!=ELJOINT ) return;
    if( selectedJoints.contains(jd) ) selectedJoints.removeOne(jd);
}


void AnalysisGroupGLScene::createLineMember(const QList<Vec> &plist)
{
    glDisable(GL_LIGHTING);

    glBegin(GL_LINE_STRIP);
    for( int i=0; i<plist.count(); i++ ){
        Vec vv=plist.at(i);
        glVertex3d( vv.x, vv.y, vv.z );
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void AnalysisGroupGLScene::createClosedMember( const QList<Vec> &tplist)
{
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(hatch);

    glBegin(GL_TRIANGLES);
    Q_FOREACH( Vec vv, tplist ) glVertex3d( vv.x, vv.y, vv.z+3.0 );
    glEnd();

    glDisable(GL_POLYGON_STIPPLE);
}

void AnalysisGroupGLScene::drawXZText(const QString& name, qreal swidth, qreal shx, qreal shy)
{
    QList<RXYLINE> linelist=ascii_table.stringToXY(name,swidth,swidth*1.25,shx,shy);

    glBegin(GL_LINES);
    Q_FOREACH(RXYLINE line,linelist){
        glVertex3d(line.x1,   0.0, line.y1 );
        glVertex3d(line.x2,   0.0, line.y2 );
    }
    glEnd();
}

ELEMENTTYPE AnalysisGroupGLScene::dataTypeToElementType(DATATYPE dtype) const
{
    ELEMENTTYPE etype;
    if(dtype==DATATYPE::TPJOINT){ etype=ELJOINT; }
    else if(dtype==DATATYPE::TPCOLUMN){ etype=ELCOLUMN; }
    else if(dtype==DATATYPE::TPGIRDER){ etype=ELGIRDER; }
    else if(dtype==DATATYPE::TPBRACE){ etype=ELBRACE; }
    else if(dtype==DATATYPE::TPBEAM){ etype=ELBEAM; }
    else if(dtype==DATATYPE::TPSLAB){ etype=ELSLAB; }
    else if(dtype==DATATYPE::TPWALL){ etype=ELWALL; }
    else if(dtype==DATATYPE::TPUWALL){ etype=ELUWALL; }
    else if(dtype==DATATYPE::TPDAMPER){ etype=ELDAMPER; }
    else if(dtype==DATATYPE::TPISO){ etype=ELISO; }
    else if(dtype==DATATYPE::TPSLABLOAD){ etype=ELSLABLOAD; }
    else if(dtype==DATATYPE::TPMEMBERLOAD){ etype=ELMEMBERLOAD; }
    else if(dtype==DATATYPE::TPJOINTLOAD){ etype=ELJOINTLOAD; }
    return etype;
}

QString AnalysisGroupGLScene::dummyName(DATATYPE dtype) const
{
    if(dtype==DATATYPE::TPCOLUMN){ return QString("(dummyC)"); }
    else if(dtype==DATATYPE::TPGIRDER){ return QString("(dummyG)"); }
    else if(dtype==DATATYPE::TPBRACE){ return QString("(dummyV)"); }
    else if(dtype==DATATYPE::TPBEAM){ return QString("(dummyB)"); }
    else if(dtype==DATATYPE::TPSLAB){ return QString("(dummyS)"); }
    else if(dtype==DATATYPE::TPWALL){ return QString("(dummyW)"); }
    else if(dtype==DATATYPE::TPUWALL){ return QString("(dummyU)"); }
    else if(dtype==DATATYPE::TPDAMPER){ return QString("(dummyD)"); }
    else if(dtype==DATATYPE::TPISO){ return QString("(dummyI)"); }
    return QString();
}

void AnalysisGroupGLScene::recalcAnalysisJointsOfMember( MemberData* md )
{
    Q_FOREACH( JointData* jd, md->getAnalysisJointList(*UnifiedInputData::getInInstance()) ){
        if( !jointList.contains(jd)
                && isJointOkToDisplayLimited(jd) ) jointList.append(jd);
    }
}

} // namespace post3dapp
