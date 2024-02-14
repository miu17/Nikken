#include "output_pers_scene.h"
#include "output_pers_viewer.h"
#include "qmath.h"
#include "handle_npstn.h"
#include "calc_any_point_stress.h"
//#include "define_textpos.h"
#include "strokefont.h"
#include "fixed_data.h"
#include "unified_analysisdata.h"
#include "define_section_struct.h"
#include "qgl.h"
#include "calc_any_point_stress.h"
#include "define_analysis3dsettings.h"
#include "define_draw_figure.h"
#include "define_output.h"
#include "unified_output.h"
#include "unified_settings.h"


#include <QDebug>
#include <QVector3D>

namespace post3dapp{
using namespace std;
using namespace qglviewer;

//void OutputPersScene::importData(const QList<MemberData*>& mdlist,const QList<OpeningSet*>& oplist)
//{
//    totalMembers=mdlist;
////    openingList=oplist;

//    initDisplayList();
//}
//void OutputPersScene::setDispOnOff(int i)
//{
//    if(i==0)drawDisp=true;
//    if(i==1)drawDisp=false;
//}

//void OutputPersScene::setLoadCase(QStringList str,QList<qreal> coef,QList<int> step)
//{
//    loadcase=str;
//    loadcoef=coef;
//    loadstep=step;
//}
//void OutputPersScene::setDispRatio(qreal r)
//{
//    dispRatioXY=r;
//}

//void OutputPersScene::setFloorFrameLimited(int fl_lim, const QStringList & fllist,
//                                     int fr_lim, const QStringList & frlist)
//{
//    floorLimitedStatus=fl_lim;
//    limitedFloors=fllist;
//    frameLimitedStatus=fr_lim;
//    limitedFrames=frlist;
//    initDisplayList();
//}
//void OutputPersScene::setSectionLimited(const QStringList & seclist)
//{
//    limitedSection=seclist;
//    initDisplayList();
//}


//bool OutputPersScene::isMemberOkToDisplayLimited( MemberData* md ) const
//{
////    bool fl_ok=true;
////    if( floorLimitedStatus==1 ){
////        fl_ok=md->belongsListedFloorFully( limitedFloors );
////    }else if(floorLimitedStatus==2 ){
////        fl_ok=md->belongsListedFloorPartly( limitedFloors );
////    }

////    bool fr_ok=true;
////    if( frameLimitedStatus==1 ){
////        fr_ok=md->belongsListedFrameFully( limitedFrames );
////    }else if( frameLimitedStatus==2 ){
////        fr_ok=md->belongsListedFramePartly( limitedFrames );
////    }
////    bool sec_ok=true;
////    if(limitedSection.contains(md->getSection()->sectionName))sec_ok=false;

////    bool zwall_ok=true;
////    if(md->getSection()->memberType==DATATYPE::TPWALL && md->option.isStructuralWall==false && drawZWall==false)zwall_ok=false;
////    //    qDebug()<<md->getSection()->sectionName<<zwall_ok;

////    return ( fl_ok && fr_ok && sec_ok && zwall_ok) ;
//    return true;
//}

//void OutputPersScene::initDisplayList()
//{
//    filteredMembers.clear();

//    Q_FOREACH( MemberData* md, totalMembers ){
//        if( !isMemberOkToDisplayLimited(md) ) continue;
//        filteredMembers.append(md);
//    }
//}
//void OutputPersScene::drawYieldMarks(DRAWINGSTYLE dstyle)
//{
//    qDebug()<<"drawYieldMarks";

//    //この中の処理がなんか重い？ループしてたりする？
//    if(filteredMembers.isEmpty())return;
//    if(dstyle!=LINEONLY)return;
//    for( int i=0; i < filteredMembers.count() ; i++ ){
//        MemberData* md=filteredMembers.at(i);
//        //qDebug()<<md->getSection()->sectionName<<md->option.yieldstate.keys();
//        //荷重の係数!=0の荷重について、YIELD<HINGEで深刻度大の方を優先する
//        bool drawyieldmark_i=false;
//        bool drawyieldmark_j=false;
//        bool drawhingemark_i=false;
//        bool drawhingemark_j=false;
//        bool drawtaxialyieldmark=false;
//        bool drawcaxialyieldmark=false;
//        bool drawshearcrackmark=false;
//        bool drawshearyieldmark=false;

////        for(int i=0;i<loadcase.count();i++){
////            qDebug()<<"loadcase"<<i;
////            if(loadcoef.at(i)==0)continue;
////            if(loadcase.at(i).isEmpty())continue;
////            qDebug()<<loadcase.at(i);
////            if(md->option.yieldstate[loadcase.at(i)].isEmpty())continue;//面部材のyieldstateを読み込めばこの例外は起きないはず
////            OptionalMemberData::YIELD_STATE state;
////            state=md->getYieldState(loadcase.at(i),loadstep.at(i)).at(0);//i
////            if(state==OptionalMemberData::HINGE){drawhingemark_i=true;}
////            if(state==OptionalMemberData::YIELD){drawyieldmark_i=true;}

////            qDebug()<<"istate";

////            state=md->getYieldState(loadcase.at(i),loadstep.at(i)).at(1);//j
////            if(state==OptionalMemberData::HINGE){drawhingemark_j=true;}
////            if(state==OptionalMemberData::YIELD){drawyieldmark_j=true;}

////            qDebug()<<"jstate";

////            state=md->getYieldState(loadcase.at(i),loadstep.at(i)).at(2);//c
////            if(state==OptionalMemberData::AXIAL_YIELD){
////                if(md->getNodeMemberForce(loadcase.at(i)).N>0.0){
////                    drawtaxialyieldmark=true;
////                }else{
////                    drawcaxialyieldmark=true;
////                }
////            }else if(state==OptionalMemberData::SHEAR_YIELD){
////                drawshearyieldmark=true;
////            }else if(state==OptionalMemberData::SHEAR_CRACK){
////                drawshearcrackmark=true;
////            }
////        }
////        qDebug()<<"set yield state";

////        if(!drawyieldmark_i && !drawyieldmark_j && !drawhingemark_i && !drawhingemark_j
////                && !drawtaxialyieldmark && !drawcaxialyieldmark && !drawshearcrackmark && !drawshearyieldmark)continue;

////        SECTIONDIM* sc=md->getSection();
////        bool isColumn=false;
////        if(md->dataType()==DATATYPE::TPCOLUMN)isColumn=true;

////        if( md->dataType()!=DATATYPE::TPWALL ){

////            JointData* ijd=md->iJoint();
////            XYZ i_xyz=ijd->xyz();
////            XYZ i_disp=getCombinationDisp(ijd,dispRatioXY,loadcase,loadcoef,loadstep);
////            i_xyz.x+=i_disp.x;
////            i_xyz.y+=i_disp.y;
////            i_xyz.z+=i_disp.z;
////            JointData* jjd=md->jJoint();
////            XYZ j_xyz=jjd->xyz();
////            XYZ j_disp=getCombinationDisp(jjd,dispRatioXY,loadcase,loadcoef,loadstep);
////            j_xyz.x+=j_disp.x;
////            j_xyz.y+=j_disp.y;
////            j_xyz.z+=j_disp.z;

////            glPushMatrix();

////            bool isVertical=false;
////            if(qAbs(ijd->xPos()-jjd->xPos())+qAbs(ijd->yPos()-jjd->yPos())<0.0001)isVertical=true;

////            setLocalMatrix(i_xyz.x, i_xyz.y, i_xyz.z, j_xyz.x, j_xyz.y, j_xyz.z,
////                           0.0,0.0,0.0,md->dataType(),DIRECTION_Z,isVertical);

////            Vec a1(j_xyz.x-i_xyz.x,j_xyz.y-i_xyz.y,j_xyz.z-i_xyz.z);


////            if(drawyieldmark_i&&!drawhingemark_i){
////                qreal radius=0.25;
////                drawYieldPoint(md->faceLength.at(0)+radius,radius,Qt::lightGray,isColumn);
////            }
////            if(drawyieldmark_j&&!drawhingemark_j){
////                qreal radius=0.25;
////                drawYieldPoint(a1.norm()-md->faceLength.at(1)-radius,radius,Qt::lightGray,isColumn);
////            }
////            if(drawhingemark_i){
////                qreal radius=0.25;
////                drawYieldPoint(md->faceLength.at(0)+radius,radius,Qt::black,isColumn);
////            }
////            if(drawhingemark_j){
////                qreal radius=0.25;
////                drawYieldPoint(a1.norm()-md->faceLength.at(1)-radius,radius,Qt::black,isColumn);
////            }
////            if(drawtaxialyieldmark){
////                bool isTension=true;
////                drawAxialYield(0.0,a1.norm(),isTension);
////            }
////            if(drawcaxialyieldmark){
////                bool isTension=false;
////                drawAxialYield(0.0,a1.norm(),isTension);
////            }
////            if(drawshearcrackmark&&!drawshearyieldmark){
////                drawShearCrackMark(a1.norm()/2.0,isColumn);
////            }
////            if(drawshearyieldmark){
////                drawShearYieldMark(a1.norm()/2.0,isColumn);
////            }
////            glPopMatrix();

////        }else if(md->dataType()==DATATYPE::TPWALL && md->option.isStructuralWall==true){
////            QList<JointData*> jdlist=md->getJointList();
////            QList<XYZ> xyzlist;
////            foreach(JointData* jd,jdlist){
////                XYZ xyz=jd->xyz();
////                XYZ disp=getCombinationDisp(jd,dispRatioXY,loadcase,loadcoef,loadstep);
////                xyz.x+=disp.x;
////                xyz.y+=disp.y;
////                xyz.z+=disp.z;
////                xyzlist.append(xyz);
////            }

////            qreal minx,maxx,miny,maxy,minz,maxz;
////            for(int i=0;i<xyzlist.count();i++){
////                if(i==0){
////                    minx=xyzlist.at(i).x;
////                    maxx=xyzlist.at(i).x;
////                    miny=xyzlist.at(i).y;
////                    maxy=xyzlist.at(i).y;
////                    minz=xyzlist.at(i).z;
////                    maxz=xyzlist.at(i).z;
////                }else{
////                    minx=qMin(minx,xyzlist.at(i).x);
////                    maxx=qMax(maxx,xyzlist.at(i).x);
////                    miny=qMin(miny,xyzlist.at(i).y);
////                    maxy=qMax(maxy,xyzlist.at(i).y);
////                    minz=qMin(minz,xyzlist.at(i).z);
////                    maxz=qMax(maxz,xyzlist.at(i).z);
////                }
////            }
////            XYZ spoint;
////            XYZ epoint;
////            if(md->option.wallDirection==OptionalMemberData::IDATATYPE::TPWALL){
////                spoint=XYZ((minx+maxx)/2.0,(miny+maxy)/2.0,minz);//i端
////                epoint=XYZ((minx+maxx)/2.0,(miny+maxy)/2.0,maxz);//j端
////            }else if(md->option.wallDirection==OptionalMemberData::HDATATYPE::TPWALL && (maxx-minx)>=(maxy-miny)){//X軸上のH壁
////                spoint=XYZ(minx,(miny+maxy)/2.0,(minz+maxz)/2.0);//i端
////                epoint=XYZ(maxx,(miny+maxy)/2.0,(minz+maxz)/2.0);//j端
////            }else if(md->option.wallDirection==OptionalMemberData::HDATATYPE::TPWALL && (maxx-minx)<(maxy-miny)){//Y軸上のH壁
////                spoint=XYZ((minx+maxx)/2.0,miny,(minz+maxz)/2.0);//i端
////                epoint=XYZ((minx+maxx)/2.0,maxy,(minz+maxz)/2.0);//j端
////            }

////            glPushMatrix();

////            setLocalMatrix(spoint.x, spoint.y, spoint.z, epoint.x, epoint.y, epoint.z,
////                           0.0,0.0,0.0,DATATYPE::TPGIRDER,DIRECTION_Z,true);

////            Vec a1(epoint.x-spoint.x,epoint.y-spoint.y,epoint.z-spoint.z);
////            if(drawyieldmark_i&&!drawhingemark_i){
////                qreal radius=0.25;
////                drawYieldPoint(radius,radius,Qt::lightGray,isColumn);
////            }
////            if(drawyieldmark_j&&!drawhingemark_j){
////                qreal radius=0.25;
////                drawYieldPoint(a1.norm()-radius,radius,Qt::lightGray,isColumn);
////            }
////            if(drawhingemark_i){
////                qreal radius=0.25;
////                drawYieldPoint(radius,radius,Qt::black,isColumn);
////            }
////            if(drawhingemark_j){
////                qreal radius=0.25;
////                drawYieldPoint(a1.norm()-radius,radius,Qt::black,isColumn);
////            }
////            if(drawshearcrackmark&&!drawshearyieldmark){
////                drawShearCrackMark(a1.norm()/2.0,isColumn);
////            }
////            if(drawshearyieldmark){
////                drawShearYieldMark(a1.norm()/2.0,isColumn);
////            }

////            glPopMatrix();
////        }
//    }
//}


//void OutputPersScene::drawYieldPoint(qreal center,qreal radius,QColor color,bool isColumn){
//    qDebug()<<"drawYieldPoint";
//    glPushMatrix();
//    if(isColumn){
//        glTranslatef(0.0,0.0,center);
//    }else{
//        glTranslatef(center,0.0,0.0);
//    }
//    float linewidth;
//    glGetFloatv(GL_LINE_WIDTH , &linewidth);
//    glLineWidth(1.0);
//    glColor4f(color.redF(),color.greenF(),color.blueF(),color.alphaF());

//    GLUquadricObj* quadric;
//    quadric = gluNewQuadric();
//    sphereList.append(quadric);

//    gluQuadricDrawStyle(quadric,GLU_FILL);
//    gluSphere(quadric,radius,10,10);

//    gluDeleteQuadric(quadric);

//    glPopMatrix();
//    glLineWidth(linewidth);
//}
//void OutputPersScene::addSphere(XYZ pos,qreal radius,QColor color){
//    qDebug()<<"drawSphere";
//    glPushMatrix();
//    glTranslatef(pos.x,pos.y,pos.z);
//    float linewidth;
//    glGetFloatv(GL_LINE_WIDTH , &linewidth);
//    glLineWidth(1.0);
//    glColor4f(color.redF(),color.greenF(),color.blueF(),color.alphaF());

//    GLUquadricObj* quadric;
//    quadric = gluNewQuadric();
//    sphereList.append(quadric);

//    gluQuadricDrawStyle(quadric,GLU_FILL);
//    gluSphere(quadric,radius,10,10);

//    gluDeleteQuadric(quadric);

//    glPopMatrix();
//    glLineWidth(linewidth);
//}
//void OutputPersScene::delSphere(){
//    foreach(GLUquadricObj* obj,sphereList){
//        delete obj;
//    }

//}

//void OutputPersScene::drawAxialYield(qreal start,qreal end,bool isTension){

//    QColor color;
//    color=Qt::black;
//    const qreal arrowr=0.15;//コーン半径
//    const qreal arrowl=0.5;//コーン長さ
//    Vec ipoint=Vec(0.25*(end-start),0.0,0.0);
//    Vec jpoint=Vec(0.75*(end-start),0.0,0.0);

//    glColor3f(color.redF(),color.greenF(),color.blueF());

//    //isTension
//    if(isTension){
//        glBegin(GL_TRIANGLE_FAN);
//        glVertex3dv(ipoint);
//        int div=12;
//        for(int i=0;i<=div;i++){//コーン部分
//            glVertex3d(ipoint.x+arrowl,arrowr*cos(2*M_PI*i/div),arrowr*sin(2*M_PI*i/div));
//        }
//        glEnd();

//        glBegin(GL_TRIANGLE_FAN);
//        glVertex3dv(jpoint);
//        for(int i=0;i<=div;i++){
//            glVertex3d(jpoint.x-arrowl,arrowr*cos(2*M_PI*i/div),arrowr*sin(2*M_PI*i/div));
//        }
//        glEnd();
//    }
//    //isCompression
//    else{
//        glBegin(GL_TRIANGLE_FAN);
//        glVertex3dv(ipoint);
//        int div=12;
//        for(int i=0;i<=div;i++){//コーン部分
//            glVertex3d(ipoint.x-arrowl,arrowr*cos(2*M_PI*i/div),arrowr*sin(2*M_PI*i/div));
//        }
//        glEnd();

//        glBegin(GL_TRIANGLE_FAN);
//        glVertex3dv(jpoint);
//        for(int i=0;i<=div;i++){
//            glVertex3d(jpoint.x+arrowl,arrowr*cos(2*M_PI*i/div),arrowr*sin(2*M_PI*i/div));
//        }
//        glEnd();

//    }
//}
//void OutputPersScene::drawShearCrackMark(qreal center,bool isColumn){
//    float linewidth;
//    glGetFloatv(GL_LINE_WIDTH , &linewidth);

//    glLineWidth(2.0);
//    glColor3d(0.0,0.0,0.0);

//    if(isColumn){
//        glBegin(GL_LINES);
//        glVertex3d(-0.25,0.0,center);
//        glVertex3d(0.25,0.0,center);
//        glVertex3d(0.0,-0.25,center);
//        glVertex3d(0.0,0.25,center);
//        glEnd();
//    }else{
//        glBegin(GL_LINES);
//        glVertex3d(center,0.0,-0.25);
//        glVertex3d(center,0.0,0.25);
//        glEnd();
//    }
//    glLineWidth(linewidth);
//}
//void OutputPersScene::drawShearYieldMark(qreal center,bool isColumn){
//    const qreal radius=0.2;
//    const qreal markl=0.4;
//    float linewidth;
//    glGetFloatv(GL_LINE_WIDTH , &linewidth);

//    glLineWidth(2.0);
//    glColor3d(0.0,0.0,0.0);

//    if(isColumn){
//        glBegin(GL_LINES);
//        glVertex3d(-markl,0.0,center);
//        glVertex3d(markl,0.0,center);
//        glVertex3d(0.0,-markl,center);
//        glVertex3d(0.0,markl,center);
//        glEnd();

//    }else{
//        glBegin(GL_LINES);
//        glVertex3d(center,0.0,-markl);
//        glVertex3d(center,0.0,markl);
//        glEnd();
//    }
//    drawYieldPoint(center,radius,Qt::black,isColumn);
//    glLineWidth(linewidth);

//}

void OutputPersScene::drawSolid(bool simplified)
{
 //   Analysis3DViewSettings  myViewSettings=UnifiedSettings::getInstance()->getOutput3DViewSettings();
    DRAWINGSTYLE dstyle=static_cast<DRAWINGSTYLE>(widgetState.drawStyle);

    //    if( filteredMembers.isEmpty() ) return;
    UnifiedAnalysisData* uad=UnifiedFixedData::getInstance()->getAnalysisData();


    //  dummy

    qreal dispRatioXY = 1.0;//myViewSettings.getDispRatioXY();
//    FIGURE_TYPE_PROPERTY prop;
    QStringList loadcase = QStringList()<<u8"RSTDL";
    QList<qreal> loadcoef = QList<qreal>()<<1.0;
    QList<int> loadstep = QList<int>()<<1;
    bool drawContour = true;

    totalMembers = uad->memberListOfAll();
    filteredMembers = uad->memberListOfAll();
    //  dummy



    for( int i=0; i < filteredMembers.count() ; i++ ){
        MemberData* md=filteredMembers.at(i);
        JointData* ijd=md->iJoint();
        XYZ i_xyz=ijd->xyz();
        XYZ i_disp=getCombinationDisp(ijd,dispRatioXY,loadcase,loadcoef,loadstep);
        i_xyz.x+=i_disp.x;
        i_xyz.y+=i_disp.y;
        i_xyz.z+=i_disp.z;
        JointData* jjd=md->jJoint();
        XYZ j_xyz=jjd->xyz();
        XYZ j_disp=getCombinationDisp(jjd,dispRatioXY,loadcase,loadcoef,loadstep);
        j_xyz.x+=j_disp.x;
        j_xyz.y+=j_disp.y;
        j_xyz.z+=j_disp.z;


        //        SECTIONDIM* sc=md->getSection();
        SectionValues sc =uad->getSectionValues(md->dataType(),md->sectionName(),i_xyz.z);
        //sc.i_param.sizeH
        //TODO:ICJで描画を分ける。配列を渡すか、そもそも内部で計算させる　→　3DVisualでもやってないので一旦スルー

        if(md->dataType()==DATATYPE::TPCOLUMN
                || md->dataType()==DATATYPE::TPGIRDER
                || md->dataType()==DATATYPE::TPBRACE){
            glPushMatrix();

            //kokokara

            setLocalMatrix(i_xyz.x, i_xyz.y, i_xyz.z, j_xyz.x, j_xyz.y, j_xyz.z,
                           0.,0.,md->codeAngle(),sc.memberType,DIRECTION_Y,false);

            Vec a1(j_xyz.x-i_xyz.x,j_xyz.y-i_xyz.y,j_xyz.z-i_xyz.z);
            createLineMember(md,QColor(),QColor(),
                             0.0,a1.norm(),0.4, 1.2,
                             dstyle,simplified,drawContour);
            glPopMatrix();
        }

        //kokomade dummy

        //            qreal th=md->codeAngle()/180.0*M_PI;
        //            qreal hh=0.0;
        //            qreal hh2=0.0;
        //            bool isVertical=false;
        //            if(qAbs(ijd->xPos()-jjd->xPos())+qAbs(ijd->yPos()-jjd->yPos())<0.0001)isVertical=true;



        //            //方向1
        //            if(sc.sizeH > 0){
        //                if(dstyle==LINEONLY){
        //                    hh=0.0;
        //                    hh2=0.0;
        //                }else{
        //                    hh=sc->sizeB/2.0*qAbs( qSin(th) )+sc->sizeH/2.0*qAbs( qCos(th) );
        //                    hh2= sc->sizeH/2.0;
        //                }
        //                setLocalMatrix(i_xyz.x, i_xyz.y, i_xyz.z, j_xyz.x, j_xyz.y, j_xyz.z,
        //                               hh2,hh,md->codeAngle(),md->dataType(),DIRECTION_Y,isVertical);

        //                Vec a1(j_xyz.x-i_xyz.x,j_xyz.y-i_xyz.y,j_xyz.z-i_xyz.z);
        //                createLineMember(md,QColor(),QColor(),
        //                                 0.0,a1.norm(),sc->sizeB, sc->sizeH,
        //                                 dstyle,simplified,drawContour);
        //            }
        //            //方向2
        //            if(sc->sizeH2 > 0){
        //                if(dstyle==LINEONLY){
        //                    hh=0.0;
        //                    hh2=0.0;
        //                }else{
        //                    hh=sc->sizeB2/2.0*qAbs( qSin(th) )+sc->sizeH2/2.0*qAbs( qCos(th) );
        //                    hh2= sc->sizeH2/2.0;
        //                }
        //                setLocalMatrix(i_xyz.x, i_xyz.y, i_xyz.z, j_xyz.x, j_xyz.y, j_xyz.z,
        //                               hh2,hh,md->codeAngle(),md->dataType(),DIRECTION_Z,isVertical);
        //                Vec a1(j_xyz.x-i_xyz.x,j_xyz.y-i_xyz.y,j_xyz.z-i_xyz.z);
        //                createLineMember(md,QColor(),QColor(),
        //                                 0.0,a1.norm(),sc->sizeB2, sc->sizeH2,
        //                                 dstyle,simplified,drawContour);
        //            }

        //            glPopMatrix();

        //        }else{//wall
        //            //壁だと軸力と曲げとせん断力？平面応力要素などの場合はどのようにデータ持っているのか確認

        //            closedMemberTriangles.insert(md,divideClosedMember(md));
        //            qDebug()<<"getjointlist.count="<<md->getJointList().count();

        //            QList<Vec> vlist;
        //            Q_FOREACH( JointData* jd, md->getJointList() ){
        //                XYZ c_xyz=jd->xyz();
        //                XYZ c_disp=getCombinationDisp(jd,dispRatioXY,loadcase,loadcoef,loadstep);
        //                c_xyz.x+=c_disp.x;
        //                c_xyz.y+=c_disp.y;
        //                c_xyz.z+=c_disp.z;
        //                vlist.append( Vec(c_xyz.x,c_xyz.y,c_xyz.z) );
        //            }
        //            qDebug()<<"vlist.count="<<vlist.count();
        //            //開口の描画
        //            //x+y+z=kに一番近い節点を抽出
        //            qreal min=999999.0;qreal max=-999999.0;
        //            qreal k;
        //            JointData *origin,*xdir,*ydir,*ddir;
        //            foreach(JointData* jd,md->getJointList()){
        //                k=jd->xPos()+jd->yPos()+jd->zPos();
        //                if(k<min){min=k;origin=jd;}
        //            }
        //            qDebug()<<"find opening origin";

        //            //変形前の系について、zが小さい二つを抽出
        //            qreal min1,min2;
        //            JointData* tmp1=md->getJointList().at(0);
        //            JointData* tmp2=md->getJointList().at(1);
        //            JointData *min1jnt,*min2jnt;
        //            if(tmp1->xyz().z < tmp2->xyz().z){
        //                min1=tmp1->xyz().z;
        //                min1jnt=tmp1;
        //                min2=tmp2->xyz().z;
        //                min2jnt=tmp2;
        //            }else{
        //                min1=tmp2->xyz().z;
        //                min1jnt=tmp2;
        //                min2=tmp1->xyz().z;
        //                min2jnt=tmp1;
        //            }
        //            Q_FOREACH(JointData* jd,md->getJointList()){
        //                qreal jntz=jd->xyz().z;
        //                if(jntz<min1){min2=min1;min2jnt=min1jnt;min1=jntz;min1jnt=jd;}
        //                else if(jntz<min2){min2=jntz;min2jnt=jd;}
        //            }
        //            if(min1jnt==origin){
        //                xdir=min2jnt;
        //            }else{
        //                xdir=min1jnt;
        //                origin=min2jnt;
        //            }
        //            qDebug()<<"find 2-z-min-joint";

        //            //(y1-y0)/(x1-x0)の符号よりx軸とy軸のどちらに近いか判定
        //            bool isXframe;//true:x構面、false:y構面
        //            if(abs(min1jnt->xyz().x-min2jnt->xyz().x) <= 0.001){isXframe=false;}
        //            else{
        //                qreal grad=(min1jnt->xyz().y-min2jnt->xyz().y)/(min1jnt->xyz().x-min2jnt->xyz().x);
        //                (-1<grad && grad<1) ? isXframe=true : isXframe=false;
        //            }
        //            qDebug()<<"determin x or y frame";

        //            //z-x=kに一番近い節点を抽出
        //            if(isXframe){
        //                min=999999.0;
        //                foreach(JointData* jd,md->getJointList()){
        //                    k=jd->zPos()-jd->xPos();
        //                    if(k<min){min=k;xdir=jd;}
        //                }
        //            }else{
        //                //z-y=kに一番近い節点を抽出

        //                min=999999.0;
        //                foreach(JointData* jd,md->getJointList()){
        //                    k=jd->zPos()-jd->yPos();
        //                    if(k<min){min=k;xdir=jd;}
        //                }
        //            }
        //            qDebug()<<"find x-dir vector";
        //            //z-x=kに一番tooi節点を抽出//tyuukannsettennarutoki,buzaiwosagasutoNULLninarunodetyuui
        //            if(isXframe){
        //                max=-999999.0;
        //                foreach(JointData* jd,md->getJointList()){
        //                    k=jd->zPos()-jd->xPos();
        //                    if(k>max){max=k;ydir=jd;}
        //                }
        //            }else{
        //                //z-y=kに一番tooi節点を抽出

        //                max=-999999.0;
        //                foreach(JointData* jd,md->getJointList()){
        //                    k=jd->zPos()-jd->yPos();
        //                    if(k>max){max=k;ydir=jd;}
        //                }
        //            }
        //            qDebug()<<"find x-dir vector";
        //            //z+x=kに一番tooi節点を抽出
        //            if(isXframe){
        //                max=-999999.0;
        //                foreach(JointData* jd,md->getJointList()){
        //                    k=jd->zPos()+jd->xPos();
        //                    if(k>max){max=k;ddir=jd;}
        //                }
        //            }else{
        //                //z+y=kに一番tooi節点を抽出

        //                max=-999999.0;
        //                foreach(JointData* jd,md->getJointList()){
        //                    k=jd->zPos()+jd->yPos();
        //                    if(k>max){max=k;ddir=jd;}
        //                }
        //            }
        //            qDebug()<<"find diagonal vector";


        //            //            //二点の座標の大小より原点と部材X軸方向を確定
        //            //            JointData *origin,*xdir;
        //            //            if(isXframe){
        //            //                if(min1jnt->xyz().x < min2jnt->xyz().x){origin=min1jnt;xdir=min2jnt;}
        //            //                else{origin=min2jnt;xdir=min1jnt;}
        //            //            }else if(!isXframe){
        //            //                if(min1jnt->xyz().y < min2jnt->xyz().y){origin=min1jnt;xdir=min2jnt;}
        //            //                else{origin=min2jnt;xdir=min1jnt;}
        //            //            }
        //            //原点からつながる、（先ほど選んだ点でない）もう一点の座標を求める
        //            QList<JointData*> jlist=md->getJointList();
        //            qDebug()<<"jlist.count="<<jlist.count();
        //            int index1=jlist.indexOf(origin);
        //            int index2=jlist.indexOf(xdir);
        //            int index3=jlist.indexOf(ddir);
        //            int index4=jlist.indexOf(ydir);
        //            //            if     (index1==0 && index2==1){index3=2;index4=3;}
        //            //            else if(index1==1 && index2==2){index3=3;index4=0;}
        //            //            else if(index1==2 && index2==3){index3=0;index4=1;}
        //            //            else if(index1==3 && index2==0){index3=1;index4=2;}
        //            //            else if(index1==3 && index2==2){index3=1;index4=0;}
        //            //            else if(index1==2 && index2==1){index3=0;index4=3;}
        //            //            else if(index1==1 && index2==0){index3=3;index4=2;}
        //            //            else if(index1==0 && index2==3){index3=2;index4=1;}
        //            if(jlist.count()<4)continue;
        //            JointData* point0=jlist.at(index1);//原点
        //            JointData* point1=jlist.at(index2);//X方向
        //            JointData* point2=jlist.at(index3);//対角
        //            JointData* point3=jlist.at(index4);//Y方向
        //            JointData* point0_1;
        //            JointData* point0_3;
        //            JointData* point2_1;
        //            JointData* point2_3;
        //            if(point0==jlist.first()){
        //                point0_3=jlist.last();
        //            }else{point0_3=jlist.at(index1-1);}
        //            if(point0==jlist.last()){
        //                point0_1=jlist.first();
        //            }else{point0_1=jlist.at(index1+1);}
        //            if(point2==jlist.first()){
        //                point2_1=jlist.last();
        //            }else{point2_1=jlist.at(index3-1);}
        //            if(point2==jlist.last()){
        //                point2_3=jlist.first();
        //            }else{point2_3=jlist.at(index3+1);}
        //            qDebug()<<"determin 4-joints of wall";
        //            //幅、高さを求める
        //            QVector3D vec0to1=QVector3D(point1->xPos()-point0->xPos(),point1->yPos()-point0->yPos(),point1->zPos()-point0->zPos());
        //            QVector3D vec0to3=QVector3D(point3->xPos()-point0->xPos(),point3->yPos()-point0->yPos(),point3->zPos()-point0->zPos());
        //            qreal width=vec0to1.length();
        //            if(width<0.0000001)continue;
        //            qreal height=QVector3D::crossProduct(vec0to1,vec0to3).length()/width;//平行四辺形の面積÷底辺
        //            qDebug()<<"calc width and height";
        //            MemberData *md01,*md12,*md23,*md30;//壁周囲の部材特定(部材が見つからないとき、NULL)
        //            md01=NULL;
        //            md12=NULL;
        //            md23=NULL;
        //            md30=NULL;
        //            Q_FOREACH(MemberData* md0,point0->getMemberList()){
        //                if(md0->getSection()->memberType != DATATYPE::TPWALL){
        //                    Q_FOREACH(MemberData* md1,point0_1->getMemberList()){
        //                        if(md0==md1){md01=md0;break;}
        //                    }
        //                }
        //            }
        //            Q_FOREACH(MemberData* md2,point2->getMemberList()){
        //                if(md2->getSection()->memberType != DATATYPE::TPWALL){
        //                    Q_FOREACH(MemberData* md1,point2_1->getMemberList()){
        //                        if(md1==md2){md12=md2;break;}
        //                    }
        //                }
        //            }
        //            Q_FOREACH(MemberData* md2,point2->getMemberList()){
        //                if(md2->getSection()->memberType != DATATYPE::TPWALL){
        //                    Q_FOREACH(MemberData* md3,point2_3->getMemberList()){
        //                        if(md2==md3){md23=md2;break;}
        //                    }
        //                }
        //            }
        //            Q_FOREACH(MemberData* md3,point0->getMemberList()){
        //                if(md3->getSection()->memberType != DATATYPE::TPWALL){
        //                    Q_FOREACH(MemberData* md0,point0_3->getMemberList()){
        //                        if(md3==md0){md30=md3;break;}
        //                    }
        //                }
        //            }
        //            qDebug()<<md01<<md12<<md23<<md30;
        //            qDebug()<<"determin members around the wall";
        //            QVector3D originVec=QVector3D(point0->xPos(),point0->yPos(),point0->zPos());
        //            QVector3D unitX,unitY,unitZ;
        //            //変形から、開口の基底ベクトルを求める
        //            XYZ origin_disp=getCombinationDisp(point0,dispRatioXY,loadcase,loadcoef,loadstep);
        //            XYZ xdir_disp=getCombinationDisp(point1,dispRatioXY,loadcase,loadcoef,loadstep);
        //            XYZ ydir_disp=getCombinationDisp(point3,dispRatioXY,loadcase,loadcoef,loadstep);
        //            QVector3D origin_dispVec=QVector3D(origin_disp.x,origin_disp.y,origin_disp.z);

        //            QVector3D vec0to1def =QVector3D(point1->xPos()+xdir_disp.x-point0->xPos()-origin_disp.x,
        //                                            point1->yPos()+xdir_disp.y-point0->yPos()-origin_disp.y,
        //                                            point1->zPos()+xdir_disp.z-point0->zPos()-origin_disp.z);
        //            QVector3D vec0to3def =QVector3D(point3->xPos()+ydir_disp.x-point0->xPos()-origin_disp.x,
        //                                            point3->yPos()+ydir_disp.y-point0->yPos()-origin_disp.y,
        //                                            point3->zPos()+ydir_disp.z-point0->zPos()-origin_disp.z);
        //            unitX=vec0to1def.normalized();
        //            unitX=unitX*vec0to1def.length()/vec0to1.length();
        //            qreal costheta;//変形前の角度
        //            costheta=QVector3D::dotProduct(vec0to1,vec0to3)/vec0to1.length()/vec0to3.length();
        //            qreal theta=acos(costheta);
        //            qreal cosphai;//変形後の角度
        //            cosphai=QVector3D::dotProduct(vec0to1def,vec0to3def)/vec0to1def.length()/vec0to3def.length();
        //            qreal phai=acos(cosphai);
        //            qreal angle=phai/theta*M_PI/2.0;
        //            unitZ=QVector3D::crossProduct(vec0to1def,vec0to3def);
        //            unitZ=unitZ.normalized();
        //            //unitY...unitXをunitZ周りにphai/theta*M_PI/2回転、長さはvec0to3def.len/vec0to3.len
        //            //回転
        //            unitY.setX(unitX.x()*(unitZ.x()*unitZ.x()*(1-cos(angle))          +cos(angle))
        //                       + unitX.y()*(unitZ.x()*unitZ.y()*(1-cos(angle))-unitZ.z()*sin(angle))
        //                       + unitX.z()*(unitZ.z()*unitZ.x()*(1-cos(angle))+unitZ.y()*sin(angle)));
        //            unitY.setY(unitX.x()*(unitZ.x()*unitZ.y()*(1-cos(angle))+unitZ.z()*sin(angle))
        //                       + unitX.y()*(unitZ.y()*unitZ.y()*(1-cos(angle))          +cos(angle))
        //                       + unitX.z()*(unitZ.y()*unitZ.z()*(1-cos(angle))-unitZ.x()*sin(angle)));
        //            unitY.setZ(unitX.x()*(unitZ.z()*unitZ.x()*(1-cos(angle))-unitZ.y()*sin(angle))
        //                       + unitX.y()*(unitZ.y()*unitZ.z()*(1-cos(angle))+unitZ.x()*sin(angle))
        //                       + unitX.z()*(unitZ.z()*unitZ.z()*(1-cos(angle))          +cos(angle)));
        //            unitY=unitY.normalized();
        //            unitY=unitY*vec0to3def.length()/vec0to3.length();
        //            originVec=originVec+origin_dispVec;//変形後の原点

        //            int numloop;
        //            if(dstyle==LINEONLY){
        //                numloop=1;
        //            }else{//壁厚のため、二枚分描画
        //                numloop=2;
        //            }
        //            QList<QList<QVector3D> > vecopfront;//壁表の開口頂点ベクトルリスト（全開口）
        //            QList<QList<QVector3D> > vecopback;//壁裏の開口頂点ベクトルリスト
        //            QList<QVector3D> vecop;
        //            for(int loop=0;loop<numloop;loop++){
        //                vecop.clear();
        //                //ステンシルバッファの設定
        //                glEnable(GL_STENCIL_TEST);
        //                glClear(GL_STENCIL_BUFFER_BIT);
        //                glStencilFunc(GL_ALWAYS,1,0xffffffff);//以下で描く図形のステンシルバッファを常に1に書き換える
        //                glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE);//続く図形でステンシルの値を置き換え
        //                glColorMask(0,0,0,0);//続く図形が色に影響を及ぼさないようにする
        //                glDepthMask(0);//同じく前後情報に影響を及ぼさないようにする
        //                glBegin(GL_QUADS);

        //                //陰影のために法線を定める
        //                if(vlist.count()>=3 && loop==0){
        //                    QVector3D normal;
        //                    for(int jno=1;jno<vlist.count();jno++){
        //                        QVector3D vec1=QVector3D(vlist.at(jno).x,vlist.at(jno).y,vlist.at(1).z)-QVector3D(vlist.at(0).x,vlist.at(0).y,vlist.at(0).z);
        //                        QVector3D vec2=QVector3D(vlist.last().x,vlist.last().y,vlist.last().z)-QVector3D(vlist.at(0).x,vlist.at(0).y,vlist.at(0).z);
        //                        normal=QVector3D::crossProduct(vec1,vec2);
        //                        if(normal.length()>0.0001)break;
        //                    }
        //                    normal.normalize();
        //                    glNormal3d(normal.x(),normal.y(),normal.z());
        //                }
        //                if(vlist.count()>=3 && loop==1){
        //                    QVector3D normal;
        //                    for(int jno=1;jno<vlist.count();jno++){
        //                        QVector3D vec1=QVector3D(vlist.at(jno).x,vlist.at(jno).y,vlist.at(1).z)-QVector3D(vlist.at(0).x,vlist.at(0).y,vlist.at(0).z);
        //                        QVector3D vec2=QVector3D(vlist.last().x,vlist.last().y,vlist.last().z)-QVector3D(vlist.at(0).x,vlist.at(0).y,vlist.at(0).z);
        //                        normal=QVector3D::crossProduct(vec1,vec2);
        //                        if(normal.length()>0.0001)break;
        //                    }
        //                    normal.normalize();
        //                    glNormal3d(-normal.x(),-normal.y(),-normal.z());
        //                }

        //                //開口情報の書き込み
        //                if(md->getOpeningNo()!=-1){
        //                    //厚み分平行移動
        //                    if(loop==0 && dstyle!=LINEONLY){originVec=originVec+unitZ*md->getSection()->sizeH/2.0;}
        //                    else if(loop==1 && dstyle!=LINEONLY){originVec=originVec-unitZ*md->getSection()->sizeH;}
        //                    OpeningSet* opset = openingList.at(md->getOpeningNo());
        //                    for(int i=0;i<opset->openings.count();i++){
        //                        Opening* op=opset->openings.at(i);
        //                        qreal bottommargin=0.0;
        //                        qDebug()<<"bottommargin declare";
        //                        //寸法が入ってると芯々、空欄だと内法 → topmargin等に該当
        //                        qDebug()<<md->ixyz().x<<md->ixyz().y<<md->ixyz().z<<md->no();
        //                        qDebug()<<op->lr<<op->tb<<op->width<<op->height<<op->positionx<<op->positiony;
        //                        if(op->lr==Opening::WFULL && op->tb==Opening::HFULL){
        //                            vecop.append(originVec);
        //                            vecop.append(originVec+unitX*width);
        //                            vecop.append(originVec+unitX*width+unitY*height);
        //                            vecop.append(originVec+unitY*height);
        //                        }else if(op->lr==Opening::WFULL && op->tb==Opening::TOP){
        //                            qreal topmargin=0.0;
        //                            if(op->positiony<0.001 && md23!=NULL) topmargin=md23->getSection()->sizeH;
        //                            vecop.append(originVec+unitY*(height-op->positiony-op->height-topmargin));
        //                            vecop.append(originVec+unitX*width+unitY*(height-op->positiony-op->height-topmargin));
        //                            vecop.append(originVec+unitX*width+unitY*(height-op->positiony-topmargin));
        //                            vecop.append(originVec+unitY*(height-op->positiony-topmargin));
        //                        }else if(op->lr==Opening::WFULL && op->tb==Opening::BOTTOM){
        //                            qDebug()<<"WFULL-BOTTOM";
        //                            if(op->positiony<0.001 && md01 != NULL){
        //                                //                                bottommargin=md01->getSection()->sizeH/2.0;
        //                            }
        //                            vecop.append(originVec+unitY*op->positiony                         +unitY*bottommargin);
        //                            vecop.append(originVec+unitX*width+unitY*op->positiony             +unitY*bottommargin);
        //                            vecop.append(originVec+unitX*width+unitY*(op->positiony+op->height)+unitY*bottommargin);
        //                            vecop.append(originVec+unitY*(op->positiony+op->height)            +unitY*bottommargin);
        //                            qDebug()<<"WFULL-BOTTOM done";
        //                        }else if(op->lr==Opening::LEFT && op->tb==Opening::HFULL){
        //                            qreal leftmargin=0.0;
        //                            if(op->positionx < 0.001 && md30 != NULL){
        //                                leftmargin = isXframe ? md30->getSection()->sizeB : md30->getSection()->sizeH;
        //                            }
        //                            leftmargin=leftmargin/2.0;
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx));
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx+op->width));
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx+op->width)+unitY*height);
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx)+unitY*height);
        //                        }else if(op->lr==Opening::LEFT && op->tb==Opening::TOP){
        //                            qreal topmargin=0.0;
        //                            if(op->positiony<0.001 && md23!=NULL) topmargin=md23->getSection()->sizeH;
        //                            qreal leftmargin=0.0;
        //                            if(op->positionx < 0.001 && md30 != NULL){
        //                                leftmargin = isXframe ? md30->getSection()->sizeB : md30->getSection()->sizeH;
        //                            }
        //                            leftmargin=leftmargin/2.0;
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx)          +unitY*(height-topmargin-op->positiony-op->height));
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx+op->width)+unitY*(height-topmargin-op->positiony-op->height));
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx+op->width)+unitY*(height-topmargin-op->positiony));
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx)          +unitY*(height-topmargin-op->positiony));
        //                        }else if(op->lr==Opening::LEFT && op->tb==Opening::BOTTOM){
        //                            qDebug()<<"LEFT-BOTTOM";
        //                            qreal leftmargin=0.0;
        //                            if(op->positionx < 0.001 && md30 != NULL){
        //                                leftmargin = isXframe ? md30->getSection()->sizeB : md30->getSection()->sizeH;
        //                            }
        //                            if(op->positiony<0.001 && md01 != NULL){
        //                                //                                bottommargin=md01->getSection()->sizeH/2.0;
        //                            }
        //                            leftmargin=leftmargin/2.0;
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx)          +unitY*op->positiony             +unitY*bottommargin);
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx+op->width)+unitY*op->positiony             +unitY*bottommargin);
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx+op->width)+unitY*(op->positiony+op->height)+unitY*bottommargin);
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx)          +unitY*(op->positiony+op->height)+unitY*bottommargin);
        //                            qDebug()<<"LEFT-BOTTOM done";
        //                        }else if(op->lr==Opening::RIGHT && op->tb==Opening::HFULL){
        //                            qreal rightmargin=0.0;
        //                            if(op->positionx < 0.001 && md12 != NULL){
        //                                rightmargin = isXframe ? md12->getSection()->sizeB : md12->getSection()->sizeH;
        //                            }
        //                            rightmargin=rightmargin/2.0;
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx-op->width));
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx));
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx)+unitY*height);
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx-op->width)+unitY*height);
        //                        }else if(op->lr==Opening::RIGHT && op->tb==Opening::TOP){
        //                            qreal topmargin=0.0;
        //                            if(op->positiony<0.001 && md23!=NULL) topmargin=md23->getSection()->sizeH;
        //                            qreal rightmargin=0.0;
        //                            if(op->positionx < 0.001 && md12 != NULL){
        //                                rightmargin = isXframe ? md12->getSection()->sizeB : md12->getSection()->sizeH;
        //                            }
        //                            rightmargin=rightmargin/2.0;
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx-op->width)+unitY*(height-topmargin-op->positiony-op->height));
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx)          +unitY*(height-topmargin-op->positiony-op->height));
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx)          +unitY*(height-topmargin-op->positiony));
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx-op->width)+unitY*(height-topmargin-op->positiony));
        //                        }else if(op->lr==Opening::RIGHT && op->tb==Opening::BOTTOM){
        //                            qDebug()<<"RIGHT-BOTTOM";
        //                            qreal rightmargin=0.0;
        //                            if(op->positionx < 0.001 && md12 != NULL){
        //                                rightmargin = isXframe ? md12->getSection()->sizeB : md12->getSection()->sizeH;
        //                            }
        //                            rightmargin=rightmargin/2.0;
        //                            if(op->positiony<0.001 && md01 != NULL){
        //                                //                                bottommargin=md01->getSection()->sizeH/2.0;
        //                            }
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx-op->width)+unitY*op->positiony+unitY*bottommargin);
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx)          +unitY*op->positiony+unitY*bottommargin);
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx)          +unitY*(op->positiony+op->height)+unitY*bottommargin);
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx-op->width)+unitY*(op->positiony+op->height)+unitY*bottommargin);
        //                            qDebug()<<"RIGHT-BOTTOM done";
        //                        }else if(op->lr==Opening::CENTER && op->tb==Opening::HFULL){
        //                            qreal rightmargin=0.0;
        //                            qreal leftmargin=0.0;
        //                            if( md12 != NULL){
        //                                rightmargin = isXframe ? md12->getSection()->sizeB : md12->getSection()->sizeH;
        //                            }
        //                            if( md30 != NULL){
        //                                leftmargin = isXframe ? md30->getSection()->sizeB : md30->getSection()->sizeH;
        //                            }
        //                            qreal centerpos=0.5*width-0.25*rightmargin+0.25*leftmargin;
        //                            vecop.append(originVec+unitX*(centerpos-op->width/2.0));
        //                            vecop.append(originVec+unitX*(centerpos+op->width/2.0));
        //                            vecop.append(originVec+unitX*(centerpos+op->width/2.0)+unitY*height);
        //                            vecop.append(originVec+unitX*(centerpos-op->width/2.0)+unitY*height);
        //                        }else if(op->lr==Opening::CENTER && op->tb==Opening::BOTTOM){
        //                            qreal rightmargin=0.0;
        //                            qreal leftmargin=0.0;
        //                            if( md12 != NULL){
        //                                rightmargin = isXframe ? md12->getSection()->sizeB : md12->getSection()->sizeH;
        //                            }
        //                            if( md30 != NULL){
        //                                leftmargin = isXframe ? md30->getSection()->sizeB : md30->getSection()->sizeH;
        //                            }
        //                            qreal centerpos=0.5*width-0.25*rightmargin+0.25*leftmargin;
        //                            vecop.append(originVec+unitX*(centerpos-op->width/2.0)+unitY*op->positiony);
        //                            vecop.append(originVec+unitX*(centerpos+op->width/2.0)+unitY*op->positiony);
        //                            vecop.append(originVec+unitX*(centerpos+op->width/2.0)+unitY*(op->positiony+op->height));
        //                            vecop.append(originVec+unitX*(centerpos-op->width/2.0)+unitY*(op->positiony+op->height));
        //                        }else if(op->lr==Opening::CENTER && op->tb==Opening::MIDDLE){
        //                            qreal rightmargin=0.0;
        //                            qreal leftmargin=0.0;
        //                            qreal topmargin=0.0;
        //                            if( md23!=NULL) {
        //                                topmargin = md23->getSection()->sizeH;
        //                            }
        //                            if( md12 != NULL){
        //                                rightmargin = isXframe ? md12->getSection()->sizeB : md12->getSection()->sizeH;
        //                            }
        //                            if( md30 != NULL){
        //                                leftmargin = isXframe ? md30->getSection()->sizeB : md30->getSection()->sizeH;
        //                            }
        //                            qreal centerpos=0.5*width-0.25*rightmargin+0.25*leftmargin;
        //                            qreal middlepos=0.5*height-0.5*topmargin;
        //                            vecop.append(originVec+unitX*(centerpos-op->width/2.0)+unitY*(middlepos-op->height/2.0));
        //                            vecop.append(originVec+unitX*(centerpos+op->width/2.0)+unitY*(middlepos-op->height/2.0));
        //                            vecop.append(originVec+unitX*(centerpos+op->width/2.0)+unitY*(middlepos+op->height/2.0));
        //                            vecop.append(originVec+unitX*(centerpos-op->width/2.0)+unitY*(middlepos+op->height/2.0));

        //                        }else if(op->lr==Opening::CENTER && op->tb==Opening::TOP){
        //                            qreal rightmargin=0.0;
        //                            qreal leftmargin=0.0;
        //                            qreal topmargin=0.0;
        //                            if( md23!=NULL) {
        //                                topmargin = md23->getSection()->sizeH;
        //                            }
        //                            if( md12 != NULL){
        //                                rightmargin = isXframe ? md12->getSection()->sizeB : md12->getSection()->sizeH;
        //                            }
        //                            if( md30 != NULL){
        //                                leftmargin = isXframe ? md30->getSection()->sizeB : md30->getSection()->sizeH;
        //                            }
        //                            qreal centerpos=0.5*width-0.25*rightmargin+0.25*leftmargin;
        //                            vecop.append(originVec+unitX*(centerpos-op->width/2.0)+unitY*(height-op->positiony-topmargin-op->height));
        //                            vecop.append(originVec+unitX*(centerpos+op->width/2.0)+unitY*(height-op->positiony-topmargin-op->height));
        //                            vecop.append(originVec+unitX*(centerpos+op->width/2.0)+unitY*(height-op->positiony-topmargin));
        //                            vecop.append(originVec+unitX*(centerpos-op->width/2.0)+unitY*(height-op->positiony-topmargin));

        //                        }else if(op->lr==Opening::LEFT && op->tb==Opening::MIDDLE){
        //                            qreal rightmargin=0.0;
        //                            qreal leftmargin=0.0;
        //                            qreal topmargin=0.0;
        //                            if( md23!=NULL) {
        //                                topmargin = md23->getSection()->sizeH;
        //                            }
        //                            if( op->positionx < 0.001 && md30 != NULL){
        //                                leftmargin = isXframe ? md30->getSection()->sizeB : md30->getSection()->sizeH;
        //                            }
        //                            qreal middlepos=0.5*height-0.5*topmargin;
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx)          +unitY*(middlepos-op->height/2.0));
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx+op->width)+unitY*(middlepos-op->height/2.0));
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx+op->width)+unitY*(middlepos+op->height/2.0));
        //                            vecop.append(originVec+unitX*(leftmargin+op->positionx)          +unitY*(middlepos+op->height/2.0));

        //                        }else if(op->lr==Opening::RIGHT && op->tb==Opening::MIDDLE){
        //                            qreal rightmargin=0.0;
        //                            qreal topmargin=0.0;
        //                            if( md23!=NULL) {
        //                                topmargin = md23->getSection()->sizeH;
        //                            }
        //                            if( op->positionx < 0.001 && md12 != NULL){
        //                                rightmargin = isXframe ? md12->getSection()->sizeB : md12->getSection()->sizeH;
        //                            }
        //                            qreal middlepos=0.5*height-0.5*topmargin;
        //                            vecop.append(originVec+unitX*(width-op->width-rightmargin-op->positionx)+unitY*(middlepos-op->height/2.0));
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx)          +unitY*(middlepos-op->height/2.0));
        //                            vecop.append(originVec+unitX*(width-rightmargin-op->positionx)          +unitY*(middlepos+op->height/2.0));
        //                            vecop.append(originVec+unitX*(width-op->width-rightmargin-op->positionx)+unitY*(middlepos+op->height/2.0));
        //                        }else if(op->lr==Opening::WFULL && op->tb==Opening::MIDDLE){
        //                            qreal topmargin=0.0;
        //                            if( md23!=NULL) {
        //                                topmargin = md23->getSection()->sizeH;
        //                            }
        //                            qreal middlepos=0.5*height-0.5*topmargin;
        //                            vecop.append(originVec+unitY*(middlepos-op->height/2.0));
        //                            vecop.append(originVec+unitX*width          +unitY*(middlepos-op->height/2.0));
        //                            vecop.append(originVec+unitX*width          +unitY*(middlepos+op->height/2.0));
        //                            vecop.append(originVec+unitY*(middlepos+op->height/2.0));
        //                        }
        //                        if(loop==0){vecopfront<<vecop;}
        //                        else if(loop==1){vecopback<<vecop;}

        //                        //ステンシル
        //                        glVertex3d(vecop.at(0).x(),vecop.at(0).y(),vecop.at(0).z() );
        //                        glVertex3d(vecop.at(1).x(),vecop.at(1).y(),vecop.at(1).z() );
        //                        glVertex3d(vecop.at(2).x(),vecop.at(2).y(),vecop.at(2).z() );
        //                        glVertex3d(vecop.at(3).x(),vecop.at(3).y(),vecop.at(3).z() );
        //                    }
        //                }
        //                glEnd();
        //                glColorMask(1,1,1,1);//色のマスクを解除
        //                glDepthMask(1);//前後情報のマスクを解除
        //                glStencilOp(GL_KEEP,GL_KEEP ,GL_KEEP);//続く図形でステンシルはそのまま
        //                glStencilFunc( GL_NOTEQUAL, 1, 0xffffffff);//ステンシルバッファと値が異なる(!=1)とき、描画(ここでは0で初期化して２値でやってるので=0としても同じ)
        //                if(drawOpening==false){glClear(GL_STENCIL_BUFFER_BIT);}

        //                QList<Vec> lineloopList;
        //                QList<Vec> triangleList;
        //                Vec vecunitZ=Vec(unitZ.x(),unitZ.y(),unitZ.z());

        //                //カラーコンター
        //                qreal ivalue,jvalue;
        //                QList<bool> ftype=forceType["W"];//N,Qy,Mz
        //                //                qDebug()<<"wall color setting";

        //                if(md->option.isStructuralWall==true && drawContour && (ftype.at(0)||ftype.at(1)||ftype.at(2))){//いずれかの応力を描く場合
        //                    MemberForce memberforce;
        //                    if(nodeOrFace==0){memberforce=md->getCombinationNodeMemberForce(loadcoef,loadcase,loadstep);}
        //                    else if(nodeOrFace==1){memberforce=md->getCombinationFaceMemberForce(loadcoef,loadcase,loadstep);}
        //                    else {qDebug() << "node or face is invalid..";}

        //                    //色を取得するための値を決定
        //                    QList<qreal> ivaluelist,jvaluelist;
        //                    if(ftype.at(0)==true){ivaluelist<<memberforce.N;jvaluelist<<memberforce.N;}
        //                    if(ftype.at(1)==true){ivaluelist<<memberforce.iQy;jvaluelist<<memberforce.jQy;}
        //                    if(ftype.at(2)==true){
        //                        ivaluelist<<memberforce.iMz;
        //                        jvaluelist<<memberforce.jMz;
        //                    }
        //                    qSort(ivaluelist.begin(),ivaluelist.end());
        //                    qSort(jvaluelist.begin(),jvaluelist.end());
        //                    if(valueCalcMethod==0){//最大
        //                        ivalue=ivaluelist.last();
        //                        jvalue=jvaluelist.last();
        //                    }else if(valueCalcMethod==1){//最小
        //                        ivalue=ivaluelist.first();
        //                        jvalue=jvaluelist.first();
        //                    }else if(valueCalcMethod==2){//絶対値最大
        //                        if(qAbs(ivaluelist.first()) < qAbs(ivaluelist.last())){ivalue=ivaluelist.last();}
        //                        else{ivalue=ivaluelist.first();}
        //                        if(qAbs(jvaluelist.first()) < qAbs(jvaluelist.last())){jvalue=jvaluelist.last();}
        //                        else{jvalue=jvaluelist.first();}
        //                    }
        //                    //qDebug()<<"wallvalue="<<ivalue<<jvalue;
        //                    if(dstyle==LINEONLY){
        //                        createContouredClosedMember(sc->lineColor,ivalue,jvalue,vlist,closedMemberTriangles.value(md),dstyle,md->option.wallDirection);
        //                    }else if(loop == 0){
        //                        for(int i=0;i<vlist.count();i++){
        //                            Vec vec=vlist.at(i);
        //                            vec=vec+vecunitZ*md->getSection()->sizeH/2.0;
        //                            lineloopList.append(vec);
        //                        }
        //                        for(int i=0;i<closedMemberTriangles.value(md).count();i++){
        //                            Vec vec=closedMemberTriangles.value(md).at(i);
        //                            vec=vec+vecunitZ*md->getSection()->sizeH/2.0;
        //                            triangleList.append(vec);
        //                        }
        //                        createContouredClosedMember(sc->lineColor,ivalue,jvalue,lineloopList,triangleList,dstyle,md->option.wallDirection);
        //                    }else if(loop == 1){
        //                        for(int i=0;i<vlist.count();i++){
        //                            Vec vec=vlist.at(i);
        //                            vec=vec-vecunitZ*md->getSection()->sizeH/2.0;
        //                            lineloopList.append(vec);
        //                        }
        //                        for(int i=0;i<closedMemberTriangles.value(md).count();i++){
        //                            Vec vec=closedMemberTriangles.value(md).at(i);
        //                            vec=vec-vecunitZ*md->getSection()->sizeH/2.0;
        //                            triangleList.append(vec);
        //                        }
        //                        createContouredClosedMember(sc->lineColor,ivalue,jvalue,lineloopList,triangleList,dstyle,md->option.wallDirection);
        //                    }
        //                }else{//応力を描かない場合
        //                    //                    qDebug()<<"not paint wall color";
        //                    if(dstyle==LINEONLY){
        //                        QColor tmpcolor = sc->paintColor;
        //                        tmpcolor.setAlpha(200);
        //                        createClosedMember(sc->lineColor,sc->paintColor,
        //                                           vlist,closedMemberTriangles.value(md),dstyle);
        //                    }else if(loop == 0){
        //                        for(int i=0;i<vlist.count();i++){
        //                            Vec vec=vlist.at(i);
        //                            vec=vec+vecunitZ*md->getSection()->sizeH/2.0;
        //                            lineloopList.append(vec);
        //                        }
        //                        for(int i=0;i<closedMemberTriangles.value(md).count();i++){
        //                            Vec vec=closedMemberTriangles.value(md).at(i);
        //                            vec=vec+vecunitZ*md->getSection()->sizeH/2.0;
        //                            triangleList.append(vec);
        //                        }
        //                        createClosedMember(sc->lineColor,sc->paintColor,
        //                                           lineloopList,triangleList,dstyle);
        //                    }else if(loop == 1){
        //                        for(int i=0;i<vlist.count();i++){
        //                            Vec vec=vlist.at(i);
        //                            vec=vec-vecunitZ*md->getSection()->sizeH/2.0;
        //                            lineloopList.append(vec);
        //                        }
        //                        for(int i=0;i<closedMemberTriangles.value(md).count();i++){
        //                            Vec vec=closedMemberTriangles.value(md).at(i);
        //                            vec=vec-vecunitZ*md->getSection()->sizeH/2.0;
        //                            triangleList.append(vec);
        //                        }
        //                        createClosedMember(sc->lineColor,sc->paintColor,
        //                                           lineloopList,triangleList,dstyle);
        //                    }
        //                    glDisable(GL_STENCIL_TEST);//ステンシル処理の終了
        //                    //                qDebug()<<"setting stencil done";
        //                }
        //            }//壁厚ループ
        //            //開口部の周り部分描画
        //            if(drawOpening==true){
        //                if(dstyle==LINEONLY){
        //                    //                    qDebug()<<"opening outline lineonly";
        //                    QColor lineColor=sc->lineColor;
        //                    glColor4d(lineColor.redF(), lineColor.greenF(), lineColor.blueF(), lineColor.alphaF());

        //                    for(int numop=0;numop<vecopfront.count();numop++){
        //                        QVector3D point0=vecopfront.at(numop).at(0);
        //                        QVector3D point1=vecopfront.at(numop).at(1);
        //                        QVector3D point2=vecopfront.at(numop).at(2);
        //                        QVector3D point3=vecopfront.at(numop).at(3);

        //                        glBegin(GL_LINE_LOOP);
        //                        glVertex3d( point0.x(),point0.y(),point0.z());
        //                        glVertex3d( point1.x(),point1.y(),point1.z());
        //                        glVertex3d( point2.x(),point2.y(),point2.z());
        //                        glVertex3d( point3.x(),point3.y(),point3.z());
        //                        glEnd();
        //                    }
        //                }else if(dstyle==SOLIDWIRE || dstyle==SOLIDONLY || dstyle==WIREONLY){
        //                    //                    qDebug()<<"opening outline polygon";
        //                    for(int numop=0;numop<vecopfront.count();numop++)
        //                    {
        //                        QVector3D f_point0=vecopfront.at(numop).at(0);
        //                        QVector3D f_point1=vecopfront.at(numop).at(1);
        //                        QVector3D f_point2=vecopfront.at(numop).at(2);
        //                        QVector3D f_point3=vecopfront.at(numop).at(3);
        //                        QVector3D b_point0=vecopback.at(numop).at(0);
        //                        QVector3D b_point1=vecopback.at(numop).at(1);
        //                        QVector3D b_point2=vecopback.at(numop).at(2);
        //                        QVector3D b_point3=vecopback.at(numop).at(3);
        //                        //線

        //                        if(dstyle==WIREONLY||dstyle==SOLIDWIRE)
        //                        {
        //                            //                            qDebug()<<"outline";
        //                            QColor lineColor=sc->lineColor;
        //                            glColor4d(lineColor.redF(), lineColor.greenF(), lineColor.blueF(), lineColor.alphaF());
        //                            glBegin(GL_LINE_LOOP);
        //                            glVertex3d( f_point0.x(), f_point0.y(), f_point0.z() );
        //                            glVertex3d( f_point1.x(), f_point1.y(), f_point1.z() );
        //                            glVertex3d( b_point1.x(), b_point1.y(), b_point1.z() );
        //                            glVertex3d( b_point0.x(), b_point0.y(), b_point0.z() );
        //                            glEnd();
        //                            glBegin(GL_LINE_LOOP);
        //                            glVertex3d( f_point1.x(), f_point1.y(), f_point1.z() );
        //                            glVertex3d( f_point2.x(), f_point2.y(), f_point2.z() );
        //                            glVertex3d( b_point2.x(), b_point2.y(), b_point2.z() );
        //                            glVertex3d( b_point1.x(), b_point1.y(), b_point1.z() );
        //                            glEnd();
        //                            glBegin(GL_LINE_LOOP);
        //                            glVertex3d( f_point2.x(), f_point2.y(), f_point2.z() );
        //                            glVertex3d( f_point3.x(), f_point3.y(), f_point3.z() );
        //                            glVertex3d( b_point3.x(), b_point3.y(), b_point3.z() );
        //                            glVertex3d( b_point2.x(), b_point2.y(), b_point2.z() );
        //                            glEnd();
        //                            glBegin(GL_LINE_LOOP);
        //                            glVertex3d( f_point3.x(), f_point3.y(), f_point3.z() );
        //                            glVertex3d( f_point0.x(), f_point0.y(), f_point0.z() );
        //                            glVertex3d( b_point0.x(), b_point0.y(), b_point0.z() );
        //                            glVertex3d( b_point3.x(), b_point3.y(), b_point3.z() );
        //                            glEnd();
        //                        }
        //                        //面
        //                        //                        qDebug()<<"outline surface";

        //                        if(dstyle==SOLIDONLY||dstyle==SOLIDWIRE)
        //                        {
        //                            QVector3D normal1=QVector3D::crossProduct((f_point1-f_point0),(b_point0-f_point0));
        //                            QVector3D normal2=QVector3D::crossProduct((f_point2-f_point1),(b_point1-f_point1));
        //                            QVector3D normal3=QVector3D::crossProduct((f_point3-f_point2),(b_point2-f_point2));
        //                            QVector3D normal4=QVector3D::crossProduct((f_point0-f_point3),(b_point3-f_point3));
        //                            normal1.normalize();normal2.normalize();normal3.normalize();normal4.normalize();
        //                            QColor paintColor=sc->paintColor;
        //                            glColor4d(paintColor.redF(), paintColor.greenF(), paintColor.blueF(), paintColor.alphaF());
        //                            glBegin(GL_QUADS);
        //                            glNormal3d( normal1.x(),normal1.y(),normal1.z());
        //                            glVertex3d( f_point0.x(), f_point0.y(), f_point0.z() );
        //                            glVertex3d( f_point1.x(), f_point1.y(), f_point1.z() );
        //                            glVertex3d( b_point1.x(), b_point1.y(), b_point1.z() );
        //                            glVertex3d( b_point0.x(), b_point0.y(), b_point0.z() );

        //                            glNormal3d( normal2.x(),normal2.y(),normal2.z());
        //                            glVertex3d( f_point1.x(), f_point1.y(), f_point1.z() );
        //                            glVertex3d( f_point2.x(), f_point2.y(), f_point2.z() );
        //                            glVertex3d( b_point2.x(), b_point2.y(), b_point2.z() );
        //                            glVertex3d( b_point1.x(), b_point1.y(), b_point1.z() );

        //                            glNormal3d( normal3.x(),normal3.y(),normal3.z());
        //                            glVertex3d( f_point2.x(), f_point2.y(), f_point2.z() );
        //                            glVertex3d( f_point3.x(), f_point3.y(), f_point3.z() );
        //                            glVertex3d( b_point3.x(), b_point3.y(), b_point3.z() );
        //                            glVertex3d( b_point2.x(), b_point2.y(), b_point2.z() );

        //                            glNormal3d( normal4.x(),normal4.y(),normal4.z());
        //                            glVertex3d( f_point3.x(), f_point3.y(), f_point3.z() );
        //                            glVertex3d( f_point0.x(), f_point0.y(), f_point0.z() );
        //                            glVertex3d( b_point0.x(), b_point0.y(), b_point0.z() );
        //                            glVertex3d( b_point3.x(), b_point3.y(), b_point3.z() );
        //                            glEnd();
        //                        }//描画タイプのIF

        //                        //    }//4面ループ
        //                    }//開口の数ループ
        //                }
        //                //                }//描画タイプのif

        //            }//部材ループ
        //        }//線部材or壁
        //        //節点の描画
        //        if(drawPoint==true){
        //            glDisable(GL_DEPTH_TEST);
        //            //            QFont font;
        //            //            font.setPointSize(9);
        //            //            drawText(i_xyz,font,Qt::red,QString::fromLocal8Bit("●"),true);
        //            //            drawText(j_xyz,font,Qt::red,QString::fromLocal8Bit("●"),true);
        //            addSphere(i_xyz,0.25,Qt::red);
        //            addSphere(j_xyz,0.25,Qt::red);
        //            glEnable(GL_DEPTH_TEST);
        //        }

        //        //        qDebug()<<"drawsolid out";

    }
}
//XYZ OutputPersScene::getJointDisp( JointData* jd, qreal r,QString loadcase) const
//{
//    if(drawDisp==false || loadcase.isEmpty()){
//        return XYZ(0.0,0.0,0.0);
//    }else{
//        XYZ disp=jd->getJointDisp(loadcase);
//        return disp*r;
//    }
//}
XYZ OutputPersScene::getCombinationDisp(JointData* jd,qreal r,QStringList loadcase,QList<qreal> coefload,QList<int> loadstep)const
{
    return XYZ(0,0,0);
    //    if(drawDisp==false){
    //        return XYZ(0.0,0.0,0.0);
    //    }else{
    //        XYZ disp=XYZ(0.0,0.0,0.0);
    //        int numcase=qMin(loadcase.count(),coefload.count());
    //        for(int i=0;i<numcase;i++){
    //            disp=disp+jd->getJointDisp(loadcase.at(i),loadstep.at(i))*coefload.at(i);
    //        }
    //        disp=disp*r;
    //        return disp;
    //    }
}

void OutputPersScene::setLocalMatrix(qreal x1,  qreal y1,  qreal z1,
                                     qreal x2,  qreal y2,  qreal z2,
                                     qreal h0,  qreal h1,  qreal angle, DATATYPE dtype,DIRECTION direction ,bool isVertical)
{
    if(direction == DIRECTION_Y)angle=angle-90;
    qreal xx=x2-x1;
    qreal yy=y2-y1;
    qreal zz=z2-z1;

    Vec a1(1.0,0.0,0.0);
    //    if(zz<0 && dtype==DATATYPE::TPCOLUMN)a1=Vec(-1,0,0);
    Vec a2(xx,yy,0.0);
    Vec a3(xx,yy,zz);
    a3.normalize();

    qreal cos1,cos2;
    GLdouble theta1,theta2;
    Vec v1,v2;
    //柱かどうかで場合を分ける必要あるか？鉛直かどうかで分ければ十分でないか？（この場合、createLineMemberなどのisColumn＝falseの場合だけ考えれば十分。ただし芯合わせ上面合わせの場合分けは必要）
    //isColumnが１：材軸＝x or zのどちらの座標系を用いているか、２：芯合わせか上面合わせか、の2つの意味でつかわれているので分けたほうが明快でないか（現状ブレースが上面合わせになりうまくないのもある。また、そもそも全部材軸をx軸かつ芯合わせで良いのでは）
    //柱梁のYとZの方向の違いがどのあたりで表現されているか確認
    if( dtype!=DATATYPE::TPCOLUMN ){

        if( a2.norm() > 0.001 ){//柱でない、かつ部材が鉛直でない場合
            a2.normalize();
            cos1=a1*a2;
            if( cos1 < -0.9999 ){
                theta1=180.;
                v1=Vec(0.0,0.0,1.0);
            }else if( cos1 > 0.9999 ){
                theta1=0.0;
                v1=Vec(0.0,0.0,0.0);
            }else{
                theta1=qAcos(cos1)*180./M_PI;
                v1=a1^a2;
            }
            cos2=a2*a3;
            if( cos2 < -0.9999 ){
                theta2=180.;
                v2=Vec(-a2.y,a2.x,0.0);
            }else if( cos2 > 0.9999 ){
                theta2=0.0;
                v2=Vec(0.0,0.0,0.0);
            }else{
                theta2=qAcos(cos2)*180./M_PI;
                v2=a2^a3;
            }
        }else{//柱でない、かつ部材が鉛直の場合
            theta1=0.0;
            v1=Vec(0.0,0.0,0.0);
            theta2=90.0;
            v2=Vec(0.0,-1.0,0.0);
        }

        glTranslatef(x1,y1,z1);
        if( v2.norm() > 0.001 ) glRotatef(theta2,v2.x,v2.y,v2.z);
        if( v1.norm() > 0.001 ) glRotatef(theta1,v1.x,v1.y,v1.z);
        glTranslatef(0.0,0.0,-h1);
        glRotatef(GLdouble(angle),1.0,0.0,0.0);
        glTranslatef(0.0,0.0,h0);

    }else{//柱の場合

        a1=Vec(0.0,0.0,1.0);
        cos1=a1*a3;

        if( cos1 < -0.9999 ){
            theta1=-180.;
            v1=Vec(qCos(angle*M_PI/180.0),qSin(angle*M_PI/180.0),0.0);
        }else if (cos1 > 0.9999){
            theta1=0.0;
            v1=Vec(0.0,0.0,0.0);

        }else{//鉛直でない場合
            theta1=qAcos(cos1)*180.0/M_PI;
            v1=a1^a3;
            if(!isVertical)angle += qAtan2(v1.y,v1.x)*180/M_PI;
            //            if(!isVertical && qAbs(angle-90)<0.001)angle = angle+180;
        }
        glTranslatef(x1,y1,z1);
        if( v1.norm() > 0.001 ) glRotatef(theta1,v1.x,v1.y,v1.z);
        glRotatef(GLdouble(angle),0.0,0.0,1.0);
    }
}

void OutputPersScene::createLineMember(MemberData* md, const QColor &lcolor, const QColor &scolor,
                                       qreal l1, qreal l2, qreal bb, qreal hh,
                                       DRAWINGSTYLE dstyle, bool simplified, bool isColored)
{
    QColor linecolor = lcolor;
    QColor surfcolor = scolor;
    //    if(lcolor == QColor()){
    //        linecolor =md->getSection()->lineColor;
    //    }
    //    if(scolor == QColor()){
    //        surfcolor =md->getSection()->paintColor;
    //    }
    DATATYPE stype = md->dataType();
    RENDERSHAPE dshape = md->getHLineShape().renderType;
    //    DATATYPE stype = md->getSection()->memberType;
    //    RENDERSHAPE dshape = md->getSection()->drawingShape;
    //    if( l2-l1 < 1.0e-3 ) return;

    bool isColumn= ( stype==DATATYPE::TPCOLUMN );

    if( dstyle==DRAWINGSTYLE::LINEONLY){
        float linewidth;
        glGetFloatv(GL_LINE_WIDTH , &linewidth);
        glLineWidth(2.0);
        //        glColor4d(col1.redF(), col1.greenF(), col1.blueF(), 0.5);
        glColor4d(linecolor.redF(), linecolor.greenF(), linecolor.blueF(), linecolor.alphaF());

        glBegin(GL_LINES);

        if( isColumn ){
            glVertex3d(0.0, 0.0, l1 );
            glVertex3d(0.0, 0.0, l2 );
        }else{
            glVertex3d(l1, 0.0, 0.0 );
            glVertex3d(l2, 0.0, 0.0 );
        }

        //        glEnable(GL_LIGHTING);
        glEnd();
        glLineWidth(linewidth);


        return;
    }


    if(isColored){
        const int div = 10;
        for (int i=0;i<div;i++){
            QColor color = calcColor(md,i,div);
            qreal ss=l1+(qreal)i*((qreal)l2-(qreal)l1)/(qreal)div;
            qreal ee=l1+(qreal)(i+1)*((qreal)l2-(qreal)l1)/(qreal)div;

            bool isStart=false;
            bool isEnd=false;
            if(i==0)isStart=true;
            if(i==div-1)isEnd=true;
            createLineMemberPart(stype,  dshape,
                                 linecolor,color,
                                 ss, ee,  bb, hh,
                                 dstyle, simplified,
                                 isStart,isEnd);
        }

    }else{
        createLineMemberPart(stype, dshape,
                             linecolor,surfcolor,
                             l1, l2, bb, hh,
                             dstyle, simplified,
                             true,true);

    }




}
QColor OutputPersScene::calcColor(MemberData* md,int i,int div){
    return Qt::red;
    //    SECTIONDIM* sc = md->getSection();

    //    bool isNewPstn = (globalData::get()->npstn != nullptr);

    //    qreal x;
    //    if(!isNewPstn || nodeOrFace == 0){
    //        x=(qreal)i/(qreal)div;
    //    }else{
    //        qreal i_len =calcAnyPointStress::iFaceLen(md->no())/ md->getTotalLength();// npstn->model.dmm1[md->no()][6]  / md->getTotalLength();
    //        qreal c_len = 1.0 - i_len - calcAnyPointStress::jFaceLen(md->no())/ md->getTotalLength();// npstn->model.dmm1[md->no()][7] / md->getTotalLength();
    //        x=i_len + c_len * (qreal)i/(qreal)div;
    //    }
    //    //forceの種類によってスイッチ
    //    qreal value=0.0;
    //    QColor color=QColor();
    //    QList<bool> ftype;//Ax,Qy,Qz,Mx,My,Mz
    //    if(md->dataType()==DATATYPE::TPCOLUMN){
    //        ftype=forceType["C"];
    //    }else if(md->dataType()==DATATYPE::TPGIRDER){
    //        ftype=forceType["G"];
    //    }else if(md->dataType()==DATATYPE::TPBRACE){
    //        ftype=forceType["B"];
    //    }
    //    if(ftype.at(0)||ftype.at(1)||ftype.at(2)||ftype.at(3)||ftype.at(4)||ftype.at(5)){//いずれかの応力を描く場合
    //        MemberForce mf;
    //        if(nodeOrFace==0){mf=md->getCombinationNodeMemberForce(loadcoef,loadcase,loadstep);}
    //        else if(nodeOrFace==1){mf=md->getCombinationFaceMemberForce(loadcoef,loadcase,loadstep);}
    //        else{qDebug()<<"node or face is invalid";}

    //        QList<qreal> valuelist;

    //        if(isNewPstn){
    //            //faceのとき、iPos=1 or 3、中央でiPos=2,節点の時、iPos=0かつpos=0 or 1？確認
    //            QVector<qreal> total(6,0.0);
    //            QVector<qreal> tmp(6,0.0);
    //            //                    qDebug()<<md->no()<<md->getTotalLength()<<x<<loadcase.at(i)<<loadstep.at(i);
    //            if(nodeOrFace == 0){//node
    //                for(int j=0;j<loadcase.count();j++){
    //                    if(loadcase.at(j).isEmpty())continue;
    //                    if(loadstep.at(j)==0)continue;
    //                    //        //stepが0、rstがinvalidだったら0、　stepが負や巨大なとき、最終ステップを返すようにしたい(calcMemberForceで対応)
    //                    tmp = calcAnyPointStress::calcMemberForce(md->no(),0,md->getTotalLength()*x,loadcase.at(j),loadstep.at(j));
    //                    for(int k=0;k<6;k++){
    //                        total[k] += tmp[k] *  loadcoef.at(j);
    //                    }
    //                }

    //            }else if(nodeOrFace == 1){
    //                for(int j=0;j<loadcase.count();j++){
    //                    if(loadcase.at(j).isEmpty())continue;
    //                    if(loadstep.at(j)==0)continue;
    //                    //        //stepが0、rstがinvalidだったら0、　stepが負や巨大なとき、最終ステップを返すようにしたい(calcMemberForceで対応)
    //                    //xの値、フェイス基準にする。描画もフェイスに揃える
    //                    tmp = calcAnyPointStress::calcMemberForce(md->no(),0,md->getTotalLength()*x,loadcase.at(j),loadstep.at(j));
    //                    for(int k=0;k<6;k++){
    //                        total[k] += tmp[k] *  loadcoef.at(j);
    //                    }
    //                }

    //            }
    //            if(ftype.at(0)==true)valuelist<<mf.N+ total.at(0);
    //            if(ftype.at(1)==true)valuelist<<mf.getQy((qreal)i/(qreal)div)+ total.at(1);
    //            if(ftype.at(2)==true)valuelist<<mf.getQz((qreal)i/(qreal)div)+ total.at(2);
    //            if(ftype.at(3)==true)valuelist<<mf.getMx((qreal)i/(qreal)div)+ total.at(3);
    //            if(ftype.at(4)==true)valuelist<<mf.getMy((qreal)i/(qreal)div)+ total.at(4);
    //            if(ftype.at(5)==true)valuelist<<mf.getMz((qreal)i/(qreal)div)+ total.at(5);

    //        }

    //        else{

    //            if(ftype.at(0)==true)valuelist<<mf.N;
    //            if(ftype.at(1)==true)valuelist<<mf.getQy(x);
    //            if(ftype.at(2)==true)valuelist<<mf.getQz(x);
    //            if(ftype.at(3)==true)valuelist<<mf.getMx(x);
    //            if(ftype.at(4)==true)valuelist<<mf.getMy(x);
    //            if(ftype.at(5)==true)valuelist<<mf.getMz(x);
    //        }
    //        qSort(valuelist.begin(),valuelist.end());
    //        if(valueCalcMethod==0){//最大
    //            value=valuelist.last();
    //        }else if(valueCalcMethod==1){//最小
    //            value=valuelist.first();
    //        }else if(valueCalcMethod==2){//絶対値最大
    //            if(qAbs(valuelist.first()) < qAbs(valuelist.last())){value=valuelist.last();}
    //            else{value=valuelist.first();}

    //        }
    //        color=colorpicker.getColor(value);
    //    }else{
    //        color = md->getSection()->paintColor;
    //    }
    //    return color;


}
void OutputPersScene::createLineMemberPart(DATATYPE stype, RENDERSHAPE dshape,
                                           const QColor& linecolor,const QColor& surfcolor,
                                           qreal l1, qreal l2, qreal bb, qreal hh,
                                           DRAWINGSTYLE dstyle, bool simplified,
                                           bool isStart,bool isEnd)
{
    bool isColumn = false;
    if(stype == DATATYPE::TPCOLUMN){
        isColumn = true;
    }
    glColor4d(surfcolor.redF(), surfcolor.greenF(), surfcolor.blueF(), surfcolor.alphaF());
    if(dstyle == DRAWINGSTYLE::LINEONLY){
        float linewidth;
        glGetFloatv(GL_LINE_WIDTH , &linewidth);
        glLineWidth(2.0);
        //        glColor4d(col1.redF(), col1.greenF(), col1.blueF(), 0.5);
        glColor4d(surfcolor.redF(), surfcolor.greenF(), surfcolor.blueF(), surfcolor.alphaF());

        glBegin(GL_LINES);

        if( isColumn ){
            glVertex3d(0.0, 0.0, l1 );
            glVertex3d(0.0, 0.0, l2 );
        }else{
            glVertex3d(l1, 0.0, 0.0 );
            glVertex3d(l2, 0.0, 0.0 );
        }

        //        glEnable(GL_LIGHTING);
        glEnd();
        glLineWidth(linewidth);

    }

    if( dshape==RENDERSHAPE::SHAPE_H || dshape==RENDERSHAPE::SHAPE_CROSSH ){
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::SOLIDONLY ) drawHSteelSolid(l1,l2,bb,hh,isColumn);
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::WIREONLY ){
            glColor4d(linecolor.redF(), linecolor.greenF(), linecolor.blueF(), linecolor.alphaF());
            drawHSteelWire(l1,l2,bb,hh,isColumn,isStart,isEnd);
        }
    }else if( dshape==RENDERSHAPE::SHAPE_BOX ){
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::SOLIDONLY ) drawBoxSolid(l1,l2,bb,hh,isColumn);
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::WIREONLY ){
            glColor4d(linecolor.redF(), linecolor.greenF(), linecolor.blueF(), linecolor.alphaF());
            drawBoxWire(l1,l2,bb,hh,isColumn,isStart,isEnd);
        }
    }else if( dshape==RENDERSHAPE::SHAPE_PIPE){
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::SOLIDONLY ) drawPipeSolid(l1,l2,bb,isColumn,simplified);
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::WIREONLY ){
            glColor4d(linecolor.redF(), linecolor.greenF(), linecolor.blueF(), linecolor.alphaF());
            drawPipeWire(l1,l2,bb,isColumn,simplified,isStart,isEnd);
        }
    }else if(dshape==RENDERSHAPE::SHAPE_T1){
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::SOLIDONLY ) drawTSteelSolid(l1,l2,bb,hh,isColumn);
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::WIREONLY ){
            glColor4d(linecolor.redF(), linecolor.greenF(), linecolor.blueF(), linecolor.alphaF());
            drawTSteelWire(l1,l2,bb,hh,isColumn,isStart,isEnd);
        }
    }else if(dshape==RENDERSHAPE::SHAPE_C1){
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::SOLIDONLY ) drawCSteelSolid(l1,l2,bb,hh,isColumn);
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::WIREONLY ){
            glColor4d(linecolor.redF(), linecolor.greenF(), linecolor.blueF(), linecolor.alphaF());
            drawCSteelWire(l1,l2,bb,hh,isColumn,isStart,isEnd);
        }
    }else if(dshape==RENDERSHAPE::SHAPE_CROSS){
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::SOLIDONLY ) drawCrossSolid(l1,l2,bb,hh,isColumn);
        if( dstyle==DRAWINGSTYLE::SOLIDWIRE || dstyle==DRAWINGSTYLE::WIREONLY ){
            glColor4d(linecolor.redF(), linecolor.greenF(), linecolor.blueF(), linecolor.alphaF());
            drawCrossWire(l1,l2,bb,hh,isColumn,isStart,isEnd);
        }
    }

}

//void OutputPersScene::createContouredClosedMember(const QColor& linecolor, const qreal ivalue, const qreal jvalue,
//                                            const QList<Vec> vertexList, const QList<Vec> triangleList,
//                                            DRAWINGSTYLE dstyle, OptionalMemberData::IDATATYPE::TPWALL_OR_HDATATYPE::TPWALL iorh){
//    Q_UNUSED(dstyle);
//    //    qDebug()<<"createContouredClosedMember"<<ivalue<<jvalue;
//    //    for(int i=0;i<vertexList.count();i++){
//    //        qDebug()<<vertexList.at(i).x<<vertexList.at(i).y<<vertexList.at(i).z;
//    //    }

//    struct CustomJoint{
//        Vec xyz;
//        qreal value;
//    };

//    //外周
//    glColor4d(linecolor.redF(), linecolor.greenF(), linecolor.blueF(), linecolor.alphaF());

//    glBegin(GL_LINE_LOOP);
//    for( int i=0; i<vertexList.count(); i++ ){
//        Vec vv=vertexList.at(i);
//        glVertex3d( vv.x, vv.y, vv.z );
//    }
//    glEnd();

//    //分割点の色と節点座標の設定
//    QList<CustomJoint*> customjointlist;
//    qreal minx,miny,minz;
//    qreal maxx,maxy,maxz;
//    enum DATATYPE::TPWALLDIRECTION{X_DIR=0,Y_DIR,Z_DIR};//通常の壁→Z_DIR、H型壁はX通りとY通りのどっちに近いかで判断


//    for(int i=0;i<triangleList.count();i++){
//        CustomJoint* cjd=new CustomJoint;
//        cjd->xyz=triangleList.at(i);
//        customjointlist.append(cjd);
//        if(i==0){
//            minx=triangleList.at(i).x;
//            maxx=triangleList.at(i).x;
//            miny=triangleList.at(i).y;
//            maxy=triangleList.at(i).y;
//            minz=triangleList.at(i).z;
//            maxz=triangleList.at(i).z;
//        }
//        else{
//            minx=qMin(minx,triangleList.at(i).x);
//            maxx=qMax(maxx,triangleList.at(i).x);
//            miny=qMin(miny,triangleList.at(i).y);
//            maxy=qMax(maxy,triangleList.at(i).y);
//            minz=qMin(minz,triangleList.at(i).z);
//            maxz=qMax(maxz,triangleList.at(i).z);
//        }
//    }
//    DATATYPE::TPWALLDIRECTION dir=Z_DIR;
//    if(iorh==OptionalMemberData::HDATATYPE::TPWALL && (maxx-minx >= maxy-miny)){dir=X_DIR;}
//    else if(iorh==OptionalMemberData::HDATATYPE::TPWALL && (maxx-minx < maxy-miny)){dir=Y_DIR;}
//    qreal mincoordinate,maxcoordinate;
//    foreach(CustomJoint* cjd,customjointlist){
//        qreal coordinate;
//        if(dir==X_DIR){coordinate=cjd->xyz.x;mincoordinate=minx;maxcoordinate=maxx;}
//        else if(dir==Y_DIR){coordinate=cjd->xyz.y;mincoordinate=miny;maxcoordinate=maxy;}
//        else if(dir==Z_DIR){coordinate=cjd->xyz.z;mincoordinate=minz;maxcoordinate=maxz;}

//        if(jvalue==ivalue){cjd->value=ivalue;}
//        else{cjd->value=(coordinate-mincoordinate)/(maxcoordinate-mincoordinate)*(jvalue-ivalue)+ivalue;}//上端がj
//        //        qDebug()<<"triangle vertex value="<<cjd->value;
//    }
//    //    foreach(CustomJoint* cjd,customjointlist){
//    //        qDebug()<<"jointdata";
//    //        qDebug()<<cjd->xyz.x<<cjd->xyz.y<<cjd->xyz.z;
//    //        qDebug()<<cjd->value;
//    //    }

//    QList<QList<CustomJoint*> > dividedtriangle;
//    int numberoftriangle=customjointlist.count()/3;
//    //    qDebug()<<"listcount="<<vertexList.count()<<triangleList.count()<<customjointlist.count();
//    //    qDebug()<<"numtriangle="<<numberoftriangle;
//    for(int i=0;i<numberoftriangle;i++){
//        QList<CustomJoint*> triangle;
//        qreal value0=customjointlist.at(3*i)->value;
//        qreal value1=customjointlist.at(3*i+1)->value;
//        qreal value2=customjointlist.at(3*i+2)->value;
//        //        qDebug()<<"value"<<i<<value0<<value1<<value2;
//        if     (value0 <= value1 && value1 <= value2){triangle<<customjointlist.at(3*i)<<customjointlist.at(3*i+1)<<customjointlist.at(3*i+2);}
//        else if(value0 <= value2 && value2 <= value1){triangle<<customjointlist.at(3*i)<<customjointlist.at(3*i+2)<<customjointlist.at(3*i+1);}
//        else if(value1 <= value0 && value0 <= value2){triangle<<customjointlist.at(3*i+1)<<customjointlist.at(3*i)<<customjointlist.at(3*i+2);}
//        else if(value1 <= value2 && value2 <= value0){triangle<<customjointlist.at(3*i+1)<<customjointlist.at(3*i+2)<<customjointlist.at(3*i);}
//        else if(value2 <= value0 && value0 <= value1){triangle<<customjointlist.at(3*i+2)<<customjointlist.at(3*i)<<customjointlist.at(3*i+1);}
//        else if(value2 <= value1 && value1 <= value0){triangle<<customjointlist.at(3*i+2)<<customjointlist.at(3*i+1)<<customjointlist.at(3*i);}
//        else{qDebug()<<"NULL"<<value0<<value1<<value2;}
//        //        qSort(triangle.begin(),triangle.end());//ソートの挙動おかしいので手動でソート
//        dividedtriangle<<triangle;
//    }
//    foreach(QList<CustomJoint*> triangle,dividedtriangle){
//        qreal minvalue=triangle.at(0)->value;
//        qreal midvalue=triangle.at(1)->value;
//        qreal maxvalue=triangle.at(2)->value;
//        qreal midratio;
//        if(maxvalue-minvalue<0.001){midratio=0;}
//        else{midratio=(midvalue-minvalue)/(maxvalue-minvalue);}
//        Vec midpoint=(1.0-midratio)*triangle.at(0)->xyz+midratio*triangle.at(2)->xyz;
//        //最小値をとる頂点から中間点までのコンター
//        QList<Vec>line1;
//        QList<Vec>line2;
//        int div=10;
//        for(int i=0;i<=div;i++){
//            line1<<(qreal)i/(qreal)div*triangle.at(0)->xyz+(qreal)(div-i)/(qreal)div*midpoint;
//            line2<<(qreal)i/(qreal)div*triangle.at(0)->xyz+(qreal)(div-i)/(qreal)div*triangle.at(1)->xyz;
//        }
//        for(int i=0;i<div;i++){
//            qreal value=(qreal)(i+0.5)/(qreal)div*minvalue+(qreal)(div-(i+0.5))/(qreal)div*midvalue;
//            QColor color=colorpicker.getColor(value);
//            glColor4d(color.redF(),color.greenF(),color.blueF(),color.alphaF());
//            //           if(dstyle==LINEONLY){glColor4d(color.redF(),color.greenF(),color.blueF(),GLfloat(0.3));}
//            glBegin(GL_TRIANGLES);//表面裏面に座標をずらして塗り重ねている
//            glVertex3dv(triangle.at(0)->xyz+i*Vec(0.001,0.001,0.001));
//            glVertex3dv(line1.at(i)+i*Vec(0.001,0.001,0.001));
//            glVertex3dv(line2.at(i)+i*Vec(0.001,0.001,0.001));
//            glEnd();
//            glBegin(GL_TRIANGLES);
//            glVertex3dv(triangle.at(0)->xyz-i*Vec(0.001,0.001,0.001));
//            glVertex3dv(line1.at(i)-i*Vec(0.001,0.001,0.001));
//            glVertex3dv(line2.at(i)-i*Vec(0.001,0.001,0.001));
//            glEnd();
//        }
//        //最大値をとる頂点から中間点までのコンター
//        line1.clear();
//        line2.clear();
//        for(int i=0;i<=div;i++){
//            line1<<(qreal)i/(qreal)div*triangle.at(2)->xyz+(qreal)(div-i)/(qreal)div*midpoint;
//            line2<<(qreal)i/(qreal)div*triangle.at(2)->xyz+(qreal)(div-i)/(qreal)div*triangle.at(1)->xyz;
//        }
//        for(int i=0;i<div;i++){
//            qreal value=(qreal)(i+0.5)/(qreal)div*maxvalue+(qreal)(div-(i+0.5))/(qreal)div*midvalue;
//            //            qDebug()<<value;
//            QColor color=colorpicker.getColor(value);
//            //            if(dstyle==LINEONLY){glColor4d(color.redF(),color.greenF(),color.blueF(),GLfloat(0.3));}塗り重ねてるので透明はうまくない
//            glColor4d(color.redF(),color.greenF(),color.blueF(),color.alphaF());
//            glBegin(GL_TRIANGLES);
//            glVertex3dv(triangle.at(2)->xyz+i*Vec(0.001,0.001,0.001));
//            glVertex3dv(line1.at(i)+i*Vec(0.001,0.001,0.001));
//            glVertex3dv(line2.at(i)+i*Vec(0.001,0.001,0.001));
//            glEnd();
//            glBegin(GL_TRIANGLES);
//            glVertex3dv(triangle.at(2)->xyz-i*Vec(0.001,0.001,0.001));
//            glVertex3dv(line1.at(i)-i*Vec(0.001,0.001,0.001));
//            glVertex3dv(line2.at(i)-i*Vec(0.001,0.001,0.001));
//            glEnd();
//        }
//    }

//    //for debug
//    //    drawText(XYZ(vertexList.at(0).x,vertexList.at(0).y,minz),QFont(),QColor(),QString::number(ivalue));
//    //    drawText(XYZ(vertexList.at(0).x,vertexList.at(0).y,maxz),QFont(),QColor(),QString::number(jvalue));


//}

//void OutputPersScene::createClosedMember(const QColor& col1, const QColor& col2,
//                                   const QList<Vec> &plist, const QList<Vec> &tplist,
//                                   DRAWINGSTYLE dstyle )
////plist...外周節点のリスト
////tplist...三角形に分割されたリスト
//{
//    //    qDebug()<<"createClosedMember";
//    if( dstyle==LINEONLY ){
//        glColor4d(col1.redF(), col1.greenF(), col1.blueF(), col1.alphaF());

//        glBegin(GL_LINE_LOOP);
//        //        glDisable(GL_LIGHTING);
//        for( int i=0; i<plist.count(); i++ ){
//            Vec vv=plist.at(i);
//            glVertex3d( vv.x, vv.y, vv.z );
//        }
//        //        glEnable(GL_LIGHTING);
//        glEnd();

//        //        glDepthMask(GL_FALSE);//半透明についてはdepthTestを行わない＝z深度が奥の部材も描画
//        glDisable(GL_DEPTH_TEST);

//        glColor4d(col2.redF(), col2.greenF(), col2.blueF(), 0.3);
//        glBegin(GL_TRIANGLES);
//        Q_FOREACH( Vec vv, tplist ) glVertex3d( vv.x, vv.y, vv.z+0.003 );
//        glEnd();
//        //        glDepthMask(GL_TRUE);
//        glEnable(GL_DEPTH_TEST);

//        return;
//    }


//    if( dstyle==SOLIDWIRE || dstyle==SOLIDONLY ){
//        glColor4d(col2.redF(), col2.greenF(), col2.blueF(), col2.alphaF());
//        glBegin(GL_TRIANGLES);
//        Q_FOREACH( Vec vv, tplist ) glVertex3d( vv.x, vv.y, vv.z+0.003 );
//        glEnd();
//    }

//    glColor4d(col1.redF(), col1.greenF(), col1.blueF(), col1.alphaF());

//    if( dstyle==SOLIDWIRE || dstyle==WIREONLY ){
//        glBegin(GL_LINE_LOOP);
//        for( int i=0; i<plist.count(); i++ ){
//            Vec vv=plist.at(i);
//            glVertex3d( vv.x, vv.y, vv.z+0.005 );
//        }
//        glEnd();
//    }
//}

void OutputPersScene::drawHSteelSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 上フランジ
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(hh/2.0), l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(hh/2.0), l1 );
    }else{
        glNormal3d(0.0,0.0,1.0);
        glVertex3d(l1, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l2, GLdouble( bb/2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb/2.0), 0.0 );
    }
    glEnd();
    // 上フランジ裏
    //    glCullFace(GL_CW);
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(0.0,-1.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(hh/2.0)-0.001, l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(hh/2.0)-0.001, l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(hh/2.0)-0.001, l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(hh/2.0)-0.001, l1 );
    }else{
        glNormal3d(0.0,0.0,-1.0);
        glVertex3d(l1, GLdouble(-bb/2.0), -0.001 );
        glVertex3d(l2, GLdouble(-bb/2.0), -0.001 );
        glVertex3d(l2, GLdouble( bb/2.0), -0.001 );
        glVertex3d(l1, GLdouble( bb/2.0), -0.001 );
    }
    glEnd();

    // 下フランジ
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(0.0,-1.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(-hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(-hh/2.0), l1 );
    }else{
        glNormal3d(0.0,0.0,-1.0);
        glVertex3d(l1, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb/2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb/2.0), GLdouble(-hh) );
    }
    glEnd();
    //    glCullFace(GL_CCW);
    // 下フランジ裏
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0)+0.001, l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0)+0.001, l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(-hh/2.0)+0.001, l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(-hh/2.0)+0.001, l1 );
    }else{
        glNormal3d(0.0,0.0,1.0);
        glVertex3d(l1, GLdouble(-bb/2.0), GLdouble(-hh)+0.001 );
        glVertex3d(l2, GLdouble(-bb/2.0), GLdouble(-hh)+0.001 );
        glVertex3d(l2, GLdouble( bb/2.0), GLdouble(-hh)+0.001 );
        glVertex3d(l1, GLdouble( bb/2.0), GLdouble(-hh)+0.001 );
    }

    glEnd();


    // ウェブ2 Y+方向（柱:X+方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(1.0,0.0,0.0);
        glVertex3d(0.001, GLdouble( hh/2.0), l1 );
        glVertex3d(0.001, GLdouble( hh/2.0), l2 );
        glVertex3d(0.001, GLdouble(-hh/2.0), l2 );
        glVertex3d(0.001, GLdouble(-hh/2.0), l1 );
    }else{
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(l1, 0.001, GLdouble(-hh) );
        glVertex3d(l2, 0.001, GLdouble(-hh) );
        glVertex3d(l2, 0.001, 0.0 );
        glVertex3d(l1, 0.001, 0.0 );
    }

    glEnd();

    // ウェブ1 Y-方向（柱:X-方向）の法線ベクトル
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(-1.0,0.0,0.0);
        glVertex3d(-0.001, GLdouble( hh/2.0), l1 );
        glVertex3d(-0.001, GLdouble( hh/2.0), l2 );
        glVertex3d(-0.001, GLdouble(-hh/2.0), l2 );
        glVertex3d(-0.001, GLdouble(-hh/2.0), l1 );
    }else{
        glNormal3d(0.0,-1.0,0.0);
        glVertex3d(l1, -0.001, GLdouble(-hh) );
        glVertex3d(l2, -0.001, GLdouble(-hh) );
        glVertex3d(l2, -0.001, 0.0 );
        glVertex3d(l1, -0.001, 0.0 );
    }

    glEnd();

}

void OutputPersScene::drawHSteelWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn,bool isStart,bool isEnd)
{
    // 上フランジライン
    //LINE_LOOPからLINESに変更してisStart、isEndの値によって書き換え

    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l1-margin );
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l2+margin );
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l1-margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l2+margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), margin );
        glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble( bb/2.0+margin), margin );
        glVertex3d(l1-margin, GLdouble( bb/2.0+margin), margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), margin );
            glVertex3d(l1-margin, GLdouble( bb/2.0+margin), margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), margin );
            glVertex3d(l2+margin, GLdouble( bb/2.0+margin), margin );
            glEnd();
        }
    }

    // 下フランジライン
    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l1-margin );
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l2+margin );
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l1-margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l2+margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
        glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
        glVertex3d(l1-margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
            glVertex3d(l1-margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
            glVertex3d(l2+margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
            glEnd();
        }
    }


    // ウェブライン
    if(isColumn){
        glBegin(GL_LINES);
        glVertex3d(0.0, GLdouble( hh/2.0), l1-margin );
        glVertex3d(0.0, GLdouble( hh/2.0), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(0.0, GLdouble(-hh/2.0), l2+margin );
        glVertex3d(0.0, GLdouble(-hh/2.0), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(0.0, GLdouble( hh/2.0), l1-margin );
            glVertex3d(0.0, GLdouble(-hh/2.0), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(0.0, GLdouble( hh/2.0), l2+margin );
            glVertex3d(0.0, GLdouble(-hh/2.0), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, 0.0, GLdouble(-hh) );
        glVertex3d(l2+margin, 0.0, GLdouble(-hh) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l1-margin, 0.0, 0.0 );
        glVertex3d(l2+margin, 0.0, 0.0 );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, 0.0, GLdouble(-hh) );
            glVertex3d(l1-margin, 0.0, 0.0 );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, 0.0, GLdouble(-hh) );
            glVertex3d(l2+margin, 0.0, 0.0 );
            glEnd();
        }
    }

    // ウェブ破線
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0x0F0F);
    if(isColumn){
        glBegin(GL_LINES);
        glVertex3d(0.0, GLdouble( hh/2.0+margin), l1-margin );
        glVertex3d(0.0, GLdouble( hh/2.0+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(0.0, GLdouble(-hh/2.0-margin), l2+margin );
        glVertex3d(0.0, GLdouble(-hh/2.0-margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(0.0, GLdouble( hh/2.0+margin), l1-margin );
            glVertex3d(0.0, GLdouble(-hh/2.0-margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(0.0, GLdouble( hh/2.0+margin), l2+margin );
            glVertex3d(0.0, GLdouble(-hh/2.0-margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, 0.0, GLdouble(-hh-margin) );
        glVertex3d(l2+margin, 0.0, GLdouble(-hh-margin) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l1-margin, 0.0, margin );
        glVertex3d(l2+margin, 0.0, margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, 0.0, GLdouble(-hh-margin) );
            glVertex3d(l1-margin, 0.0, margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, 0.0, GLdouble(-hh-margin) );
            glVertex3d(l2+margin, 0.0, margin );
            glEnd();
        }
    }

    glDisable(GL_LINE_STIPPLE);

}
void OutputPersScene::drawISteelSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 左フランジ
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(-1.0,0.0,0.0);
        glVertex3d(GLdouble(-hh/2.0), GLdouble(bb/2.0), l1 );
        glVertex3d(GLdouble(-hh/2.0), GLdouble(bb/2.0), l2 );
        glVertex3d(GLdouble(-hh/2.0), GLdouble(-bb/2.0), l2 );
        glVertex3d(GLdouble(-hh/2.0), GLdouble(-bb/2.0), l1 );
    }else{
        glNormal3d(0.0,-1.0,0.0);
        glVertex3d(l1, GLdouble(-hh/2.0), GLdouble(-bb) );
        glVertex3d(l2, GLdouble(-hh/2.0), GLdouble(-bb) );
        glVertex3d(l2, GLdouble(-hh/2.0), 0.0 );
        glVertex3d(l1, GLdouble(-hh/2.0), 0.0 );
    }
    glEnd();
    // 左フランジ裏
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(1.0,0.0,0.0);
        glVertex3d(GLdouble(-hh/2.0)+0.001, GLdouble(bb/2.0), l1 );
        glVertex3d(GLdouble(-hh/2.0)+0.001, GLdouble(bb/2.0), l2 );
        glVertex3d(GLdouble(-hh/2.0)+0.001, GLdouble(-bb/2.0), l2 );
        glVertex3d(GLdouble(-hh/2.0)+0.001, GLdouble(-bb/2.0), l1 );
    }else{
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(l1, GLdouble(-hh/2.0)+0.001, GLdouble(-bb) );
        glVertex3d(l2, GLdouble(-hh/2.0)+0.001, GLdouble(-bb) );
        glVertex3d(l2, GLdouble(-hh/2.0)+0.001, 0.0 );
        glVertex3d(l1, GLdouble(-hh/2.0)+0.001, 0.0 );
    }
    glEnd();

    // 右フランジ
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(1.0,0.0,0.0);
        glVertex3d(GLdouble(hh/2.0), GLdouble(bb/2.0), l1 );
        glVertex3d(GLdouble(hh/2.0), GLdouble(bb/2.0), l2 );
        glVertex3d(GLdouble(hh/2.0), GLdouble(-bb/2.0), l2 );
        glVertex3d(GLdouble(hh/2.0), GLdouble(-bb/2.0), l1 );
    }else{
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(l1, GLdouble(hh/2.0), GLdouble(-bb) );
        glVertex3d(l2, GLdouble(hh/2.0), GLdouble(-bb) );
        glVertex3d(l2, GLdouble(hh/2.0), 0.0 );
        glVertex3d(l1, GLdouble(hh/2.0), 0.0 );
    }
    glEnd();
    //裏
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(-1.0,0.0,0.0);
        glVertex3d(GLdouble(hh/2.0)-0.001, GLdouble(bb/2.0), l1 );
        glVertex3d(GLdouble(hh/2.0)-0.001, GLdouble(bb/2.0), l2 );
        glVertex3d(GLdouble(hh/2.0)-0.001, GLdouble(-bb/2.0), l2 );
        glVertex3d(GLdouble(hh/2.0)-0.001, GLdouble(-bb/2.0), l1 );
    }else{
        glNormal3d(0.0,-1.0,0.0);
        glVertex3d(l1, GLdouble(hh/2.0)-0.001, GLdouble(-bb) );
        glVertex3d(l2, GLdouble(hh/2.0)-0.001, GLdouble(-bb) );
        glVertex3d(l2, GLdouble(hh/2.0)-0.001, 0.0 );
        glVertex3d(l1, GLdouble(hh/2.0)-0.001, 0.0 );
    }
    glEnd();

    // ウェブ
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(GLdouble( hh/2.0),0.0, l1 );
        glVertex3d(GLdouble( hh/2.0),0.0, l2 );
        glVertex3d(GLdouble(-hh/2.0),0.0, l2 );
        glVertex3d(GLdouble(-hh/2.0),0.0, l1 );
    }else{
        glNormal3d(0.0,0.0,1.0);
        glVertex3d(l1, GLdouble(-hh/2.0), GLdouble(-bb/2.0) );
        glVertex3d(l2, GLdouble(-hh/2.0), GLdouble(-bb/2.0) );
        glVertex3d(l2, GLdouble(hh/2.0), GLdouble(-bb/2.0) );
        glVertex3d(l1, GLdouble(hh/2.0), GLdouble(-bb/2.0) );
    }
    glEnd();
    // ウェブ裏
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(0.0,-1.0,0.0);
        glVertex3d(GLdouble( hh/2.0),-0.001, l1 );
        glVertex3d(GLdouble( hh/2.0),-0.001, l2 );
        glVertex3d(GLdouble(-hh/2.0),-0.001, l2 );
        glVertex3d(GLdouble(-hh/2.0),-0.001, l1 );
    }else{
        glNormal3d(0.0,0.0,-1.0);
        glVertex3d(l1, GLdouble(-hh/2.0), GLdouble(-bb/2.0)-0.001 );
        glVertex3d(l2, GLdouble(-hh/2.0), GLdouble(-bb/2.0)-0.001 );
        glVertex3d(l2, GLdouble(hh/2.0), GLdouble(-bb/2.0)-0.001 );
        glVertex3d(l1, GLdouble(hh/2.0), GLdouble(-bb/2.0)-0.001 );
    }
    glEnd();

}

void OutputPersScene::drawISteelWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn,bool isStart,bool isEnd)
{
    //編集中
    // 左フランジライン
    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-hh/2.0-margin), GLdouble(bb/2.0+margin), l1-margin );
        glVertex3d(GLdouble(-hh/2.0-margin), GLdouble(bb/2.0+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-hh/2.0-margin), GLdouble(-bb/2.0-margin), l2+margin );
        glVertex3d(GLdouble(-hh/2.0-margin), GLdouble(-bb/2.0-margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-hh/2.0-margin), GLdouble(-bb/2.0-margin), l1-margin );
            glVertex3d(GLdouble(-hh/2.0-margin), GLdouble(bb/2.0+margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-hh/2.0-margin), GLdouble(-bb/2.0-margin), l2+margin );
            glVertex3d(GLdouble(-hh/2.0-margin), GLdouble(bb/2.0+margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-hh/2.0-margin), margin );
        glVertex3d(l2+margin, GLdouble(-hh/2.0-margin), margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble(-hh/2.0-margin), GLdouble(-bb-margin) );
        glVertex3d(l1-margin, GLdouble(-hh/2.0-margin), GLdouble(-bb-margin) );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-hh/2.0-margin), margin );
            glVertex3d(l1-margin, GLdouble(-hh/2.0-margin), GLdouble(-bb-margin) );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-hh/2.0-margin), margin );
            glVertex3d(l2+margin, GLdouble(-hh/2.0-margin), GLdouble(-bb-margin) );
            glEnd();
        }
    }

    // 右フランジライン
    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(hh/2.0+margin), GLdouble(bb/2.0+margin), l1-margin );
        glVertex3d(GLdouble(hh/2.0+margin), GLdouble(bb/2.0+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble(hh/2.0+margin), GLdouble(-bb/2.0-margin), l2+margin );
        glVertex3d(GLdouble(hh/2.0+margin), GLdouble(-bb/2.0-margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(hh/2.0+margin), GLdouble(-bb/2.0-margin), l1-margin );
            glVertex3d(GLdouble(hh/2.0+margin), GLdouble(bb/2.0+margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(hh/2.0+margin), GLdouble(-bb/2.0-margin), l2+margin );
            glVertex3d(GLdouble(hh/2.0+margin), GLdouble(bb/2.0+margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(hh/2.0+margin), margin );
        glVertex3d(l2+margin, GLdouble(hh/2.0+margin), margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble(hh/2.0+margin), GLdouble(-bb-margin) );
        glVertex3d(l1-margin, GLdouble(hh/2.0+margin), GLdouble(-bb-margin) );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(hh/2.0+margin), margin );
            glVertex3d(l1-margin, GLdouble(hh/2.0+margin), GLdouble(-bb-margin) );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(hh/2.0+margin), margin );
            glVertex3d(l2+margin, GLdouble(hh/2.0+margin), GLdouble(-bb-margin) );
            glEnd();
        }
    }


    // ウェブライン
    if(isColumn){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-hh/2.0), 0.0, l1-margin );
        glVertex3d(GLdouble(-hh/2.0), 0.0, l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble( hh/2.0), 0.0, l2+margin );
        glVertex3d(GLdouble( hh/2.0), 0.0, l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble( hh/2.0),0.0, l1-margin );
            glVertex3d(GLdouble(-hh/2.0),0.0, l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble( hh/2.0),0.0, l2+margin );
            glVertex3d(GLdouble(-hh/2.0),0.0, l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-hh/2.0), GLdouble(-bb/2.0) );
        glVertex3d(l2+margin,GLdouble(-hh/2.0), GLdouble(-bb/2.0) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l1-margin,GLdouble(hh/2.0),  GLdouble(-bb/2.0) );
        glVertex3d(l2+margin,GLdouble(hh/2.0),  GLdouble(-bb/2.0) );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin,GLdouble(-hh/2.0), GLdouble(-bb/2.0) );
            glVertex3d(l1-margin,GLdouble(hh/2.0), GLdouble(-bb/2.0) );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin,GLdouble(-hh/2.0), GLdouble(-bb/2.0) );
            glVertex3d(l2+margin,GLdouble(hh/2.0), GLdouble(-bb/2.0) );
            glEnd();
        }
    }


}

void OutputPersScene::drawBoxSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{

    // 上側面
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(hh/2.0), l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(hh/2.0), l1 );
    }else{
        glNormal3d(0.0,0.0,1.0);
        glVertex3d(l1, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l2, GLdouble( bb/2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb/2.0), 0.0 );
    }

    glEnd();

    // 下側面
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(0.0,-1.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(-hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(-hh/2.0), l1 );
    }else{
        glNormal3d(0.0,0.0,-1.0);
        glVertex3d(l1, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb/2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb/2.0), GLdouble(-hh) );
    }

    glEnd();

    // 左側面
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(-1.0,0.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0), l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0), l2 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l2 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l1 );
    }else{
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(l1, GLdouble(bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(bb/2.0), 0.0 );
        glVertex3d(l1, GLdouble(bb/2.0), 0.0 );
    }


    glEnd();

    // 右側面
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(1.0,0.0,0.0);
        glVertex3d(GLdouble(bb/2.0), GLdouble( hh/2.0), l1 );
        glVertex3d(GLdouble(bb/2.0), GLdouble( hh/2.0), l2 );
        glVertex3d(GLdouble(bb/2.0), GLdouble(-hh/2.0), l2 );
        glVertex3d(GLdouble(bb/2.0), GLdouble(-hh/2.0), l1 );
    }else{
        glNormal3d(0.0,-1.0,0.0);
        glVertex3d(l1, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l1, GLdouble(-bb/2.0), 0.0 );
    }

    glEnd();


    // 上面
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(0.0,0.0,1.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble( hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(-hh/2.0), l2 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l2 );
    }else{
        glNormal3d(1.0,0.0,0.0);
        glVertex3d(l2, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb/2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb/2.0), 0.0 );
    }

    glEnd();

    // 下面
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(0.0,0.0,-1.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0), l1 );
        glVertex3d(GLdouble( bb/2.0), GLdouble( hh/2.0), l1 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(-hh/2.0), l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l1 );
    }else{
        glNormal3d(-1.0,0.0,0.0);
        glVertex3d(l1, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb/2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb/2.0), 0.0 );
        glVertex3d(l1, GLdouble(-bb/2.0), 0.0 );
    }

    glEnd();

}

void OutputPersScene::drawBoxWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn,bool isStart,bool isEnd)
{
    // 上側面ライン
    //    glBegin(GL_LINE_LOOP);

    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l1-margin );
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l2+margin );
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l1-margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l2+margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), margin );
        glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble( bb/2.0+margin), margin );
        glVertex3d(l1-margin, GLdouble( bb/2.0+margin), margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), margin );
            glVertex3d(l1-margin, GLdouble( bb/2.0+margin), margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), margin );
            glVertex3d(l2+margin, GLdouble( bb/2.0+margin), margin );
            glEnd();
        }
    }

    //    glEnd();

    // 下側面ライン
    //    glBegin(GL_LINE_LOOP);

    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l1-margin );
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l2+margin );
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l1-margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l2+margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
        glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
        glVertex3d(l1-margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
            glVertex3d(l1-margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
            glVertex3d(l2+margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
            glEnd();
        }
    }

    //    glEnd();

    // 左側面ライン
    //    glBegin(GL_LINE_LOOP);

    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble( hh/2.0+margin), l1-margin );
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble( hh/2.0+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l2+margin );
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble( hh/2.0+margin), l1-margin );
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble( hh/2.0+margin), l2+margin );
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(bb/2.0+margin), GLdouble(-hh-margin) );
        glVertex3d(l2+margin, GLdouble(bb/2.0+margin), GLdouble(-hh-margin) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble(bb/2.0+margin), margin );
        glVertex3d(l1-margin, GLdouble(bb/2.0+margin), margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(bb/2.0+margin), GLdouble(-hh-margin) );
            glVertex3d(l1-margin, GLdouble(bb/2.0+margin), margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(bb/2.0+margin), GLdouble(-hh-margin) );
            glVertex3d(l2+margin, GLdouble(bb/2.0+margin), margin );
            glEnd();
        }
    }

    //    glEnd();

    // 右側面ライン
    //    glBegin(GL_LINE_LOOP);

    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(bb/2.0+margin), GLdouble( hh/2.0+margin), l1-margin );
        glVertex3d(GLdouble(bb/2.0+margin), GLdouble( hh/2.0+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble(bb/2.0+margin), GLdouble(-hh/2.0-margin), l2+margin );
        glVertex3d(GLdouble(bb/2.0+margin), GLdouble(-hh/2.0-margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(bb/2.0+margin), GLdouble( hh/2.0+margin), l1-margin );
            glVertex3d(GLdouble(bb/2.0+margin), GLdouble(-hh/2.0-margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(bb/2.0+margin), GLdouble( hh/2.0+margin), l2+margin );
            glVertex3d(GLdouble(bb/2.0+margin), GLdouble(-hh/2.0-margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
        glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), margin );
        glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
            glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
            glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), margin );
            glEnd();
        }
    }

    //    glEnd();

}

void OutputPersScene::drawPipeSolid( qreal l1, qreal l2, qreal dd, bool isColumn, bool simplified )
{
    int NStep=(simplified) ? 6 : 8 ;
    qreal rr=dd/2.0;

    // 側面
    glBegin(GL_QUAD_STRIP);

    if( isColumn ){
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t=2.0*M_PI*qreal(i)/qreal(NStep);
            glNormal3d( GLdouble(qCos(t)),GLdouble(qSin(t)),0.0 );
            glVertex3d( GLdouble(rr*qCos(t)), GLdouble(rr*qSin(t)), l1 );
            glVertex3d( GLdouble(rr*qCos(t)), GLdouble(rr*qSin(t)), l2 );
        }
    }else{
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t=2.0*M_PI*qreal(i)/qreal(NStep);
            glNormal3d(0.0, GLdouble(qCos(t)),GLdouble(qSin(t)));
            glVertex3d(l1, GLdouble(rr*qCos(t)), GLdouble(rr*qSin(t)-rr/2.0) );
            glVertex3d(l2, GLdouble(rr*qCos(t)), GLdouble(rr*qSin(t)-rr/2.0) );
        }
    }

    glEnd();

    // 下面
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(0.0,0.0,-1.0);
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t=2.0*M_PI*qreal(i)/qreal(NStep);
            glVertex3d(GLdouble(rr*qCos(t)), GLdouble(rr*qSin(t)), l1 );
        }
    }else{
        glNormal3d(0.0,-1.0,0.0);
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t=2.0*M_PI*qreal(i)/qreal(NStep);
            //            glNormal3d(0.0,GLdouble(qCos(t)),GLdouble(qSin(t)));
            glVertex3d(l1, GLdouble(rr*qCos(t)), GLdouble(rr*qSin(t)-rr/2.0) );
        }
    }

    glEnd();

    // 上面
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(0.0,0.0,1.0);
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t=2.0*M_PI*qreal(i)/qreal(NStep);
            glVertex3d(GLdouble(rr*qCos(t)), GLdouble(rr*qSin(t)), l2 );
        }
    }else{
        glNormal3d(0.0,1.0,0.0);
        for (int i = 0; i <= NStep; ++i) {
            GLdouble t=2.0*M_PI*qreal(i)/qreal(NStep);
            //            glNormal3d(0.0,GLdouble(qCos(t)),GLdouble(qSin(t)));
            glVertex3d(l2, GLdouble(rr*qCos(t)), GLdouble(rr*qSin(t)-rr/2.0) );
        }
    }

    glEnd();

}

void OutputPersScene::drawPipeWire( qreal l1, qreal l2, qreal dd, bool isColumn, bool simplified ,bool isStart,bool isEnd)
{
    int NStep=(simplified) ? 6 : 8 ;
    qreal rr=dd/2.0;

    // 側面ライン

    rr=rr+margin/5.0;

    if( isColumn ){
        for (int i = 0; i < NStep; ++i) {
            GLdouble t0=2.0*M_PI*qreal(i)/qreal(NStep);
            GLdouble t1=2.0*M_PI*qreal(i+1)/qreal(NStep);
            glBegin(GL_LINES);
            glVertex3d(GLdouble(rr*qCos(t1)), GLdouble(rr*qSin(t1)), l1-margin );
            glVertex3d(GLdouble(rr*qCos(t1)), GLdouble(rr*qSin(t1)), l2+margin );
            glEnd();
            if(isStart){
                glBegin(GL_LINES);
                glVertex3d(GLdouble(rr*qCos(t0)), GLdouble(rr*qSin(t0)), l1-margin );
                glVertex3d(GLdouble(rr*qCos(t1)), GLdouble(rr*qSin(t1)), l1-margin );
                glEnd();
            }
            if(isEnd){
                glBegin(GL_LINES);
                glVertex3d(GLdouble(rr*qCos(t1)), GLdouble(rr*qSin(t1)), l2+margin );
                glVertex3d(GLdouble(rr*qCos(t0)), GLdouble(rr*qSin(t0)), l2+margin );
                glEnd();
            }
        }
    }else{
        for (int i = 0; i < NStep; ++i) {
            GLdouble t0=2.0*M_PI*qreal(i)/qreal(NStep);
            GLdouble t1=2.0*M_PI*qreal(i+1)/qreal(NStep);
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(rr*qCos(t1)), GLdouble(rr*qSin(t1)-rr/2.0) );
            glVertex3d(l2+margin, GLdouble(rr*qCos(t1)), GLdouble(rr*qSin(t1)-rr/2.0) );
            glEnd();
            if(isStart){
                glBegin(GL_LINES);
                glVertex3d(l1-margin, GLdouble(rr*qCos(t0)), GLdouble(rr*qSin(t0)-rr/2.0) );
                glVertex3d(l1-margin, GLdouble(rr*qCos(t1)), GLdouble(rr*qSin(t1)-rr/2.0) );
                glEnd();
            }
            if(isEnd){
                glBegin(GL_LINES);
                glVertex3d(l2+margin, GLdouble(rr*qCos(t1)), GLdouble(rr*qSin(t1)-rr/2.0) );
                glVertex3d(l2+margin, GLdouble(rr*qCos(t0)), GLdouble(rr*qSin(t0)-rr/2.0) );
                glEnd();
            }
        }
    }
}
void OutputPersScene::drawTSteelSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 上フランジ
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(hh), l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(hh), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(hh), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(hh), l1 );
    }else{
        glNormal3d(0.0,0.0,1.0);
        glVertex3d(l1, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l2, GLdouble( bb/2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb/2.0), 0.0 );
    }

    glEnd();

    // ウェブ
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(1.0,0.0,0.0);
        glVertex3d(0.0, GLdouble( hh), l1 );
        glVertex3d(0.0, GLdouble( hh), l2 );
        glVertex3d(0.0, 0.0, l2 );
        glVertex3d(0.0, 0.0, l1 );
    }else{
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(l1, 0.001, GLdouble(-hh) );
        glVertex3d(l2, 0.001, GLdouble(-hh) );
        glVertex3d(l2, 0.001, 0.0 );
        glVertex3d(l1, 0.001, 0.0 );
    }
    glEnd();

}

void OutputPersScene::drawTSteelWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn,bool isStart,bool isEnd)
{
    // 上フランジライン

    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh+margin), l1-margin );
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh+margin), l2+margin );
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh+margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh+margin), l1-margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh+margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh+margin), l2+margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh+margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), margin );
        glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble( bb/2.0+margin), margin );
        glVertex3d(l1-margin, GLdouble( bb/2.0+margin), margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), margin );
            glVertex3d(l1-margin, GLdouble( bb/2.0+margin), margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), margin );
            glVertex3d(l2+margin, GLdouble( bb/2.0+margin), margin );
            glEnd();
        }
    }

    // ウェブライン
    if(isColumn){
        glBegin(GL_LINES);
        glVertex3d(0.0, GLdouble( hh), l1-margin );
        glVertex3d(0.0, GLdouble( hh), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(0.0, 0.0, l2+margin );
        glVertex3d(0.0, 0.0, l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(0.0, GLdouble( hh), l1-margin );
            glVertex3d(0.0, 0.0, l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(0.0, GLdouble( hh), l2+margin );
            glVertex3d(0.0, 0.0, l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, 0.0, GLdouble(-hh) );
        glVertex3d(l2+margin, 0.0, GLdouble(-hh) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l1-margin, 0.0, 0.0 );
        glVertex3d(l2+margin, 0.0, 0.0 );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, 0.0, GLdouble(-hh) );
            glVertex3d(l1-margin, 0.0, 0.0 );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, 0.0, GLdouble(-hh) );
            glVertex3d(l2+margin, 0.0, 0.0 );
            glEnd();
        }
    }

    // ウェブ破線
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0x0F0F);
    if(isColumn){
        glBegin(GL_LINES);
        glVertex3d(0.0, GLdouble( hh+margin), l1-margin );
        glVertex3d(0.0, GLdouble( hh+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(0.0, GLdouble(0.0-margin), l2+margin );
        glVertex3d(0.0, GLdouble(0.0-margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(0.0, GLdouble( hh+margin), l1-margin );
            glVertex3d(0.0, GLdouble(0.0-margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(0.0, GLdouble( hh+margin), l2+margin );
            glVertex3d(0.0, GLdouble(0.0-margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, 0.0, GLdouble(-hh-margin) );
        glVertex3d(l2+margin, 0.0, GLdouble(-hh-margin) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l1-margin, 0.0, margin );
        glVertex3d(l2+margin, 0.0, margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, 0.0, GLdouble(-hh-margin) );
            glVertex3d(l1-margin, 0.0, margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, 0.0, GLdouble(-hh-margin) );
            glVertex3d(l2+margin, 0.0, margin );
            glEnd();
        }
    }

    glDisable(GL_LINE_STIPPLE);

}
void OutputPersScene::drawCSteelSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{
    // 上フランジ
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(hh/2.0), l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(hh/2.0), l1 );
    }else{
        glNormal3d(0.0,0.0,1.0);
        glVertex3d(l1, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l2, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l2, GLdouble( bb/2.0), 0.0 );
        glVertex3d(l1, GLdouble( bb/2.0), 0.0 );
    }

    glEnd();

    // 下フランジ
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(0.0,-1.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(-hh/2.0), l2 );
        glVertex3d(GLdouble( bb/2.0), GLdouble(-hh/2.0), l1 );
    }else{
        glNormal3d(0.0,0.0,-1.0);
        glVertex3d(l1, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble( bb/2.0), GLdouble(-hh) );
        glVertex3d(l1, GLdouble( bb/2.0), GLdouble(-hh) );
    }

    glEnd();

    // ウェブ
    glBegin(GL_POLYGON);

    if( isColumn ){
        glNormal3d(1.0,0.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0), l1 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0), l2 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l2 );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l1 );
    }else{
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(l1, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l1, GLdouble(-bb/2.0), 0.0 );
    }

    glEnd();


    glEnd();
}

void OutputPersScene::drawCSteelWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn,bool isStart,bool isEnd)
{
    // 上フランジライン
    //LINE_LOOPからLINESに変更してisStart、isEndの値によって書き換え

    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l1-margin );
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l2+margin );
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l1-margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(hh/2.0+margin), l2+margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(hh/2.0+margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), margin );
        glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble( bb/2.0+margin), margin );
        glVertex3d(l1-margin, GLdouble( bb/2.0+margin), margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), margin );
            glVertex3d(l1-margin, GLdouble( bb/2.0+margin), margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), margin );
            glVertex3d(l2+margin, GLdouble( bb/2.0+margin), margin );
            glEnd();
        }
    }

    // 下フランジライン
    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l1-margin );
        glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l2+margin );
        glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l1-margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0-margin), l2+margin );
            glVertex3d(GLdouble( bb/2.0+margin), GLdouble(-hh/2.0-margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
        glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
        glVertex3d(l1-margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
            glVertex3d(l1-margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), GLdouble(-hh-margin) );
            glVertex3d(l2+margin, GLdouble( bb/2.0+margin), GLdouble(-hh-margin) );
            glEnd();
        }
    }


    // ウェブライン
    if(isColumn){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0), l1-margin );
        glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l2+margin );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0), l1-margin );
            glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0), l2+margin );
            glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0), GLdouble(-hh) );
        glVertex3d(l2+margin, GLdouble(-bb/2.0), GLdouble(-hh) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0), 0.0 );
        glVertex3d(l2+margin, GLdouble(-bb/2.0), 0.0 );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0), GLdouble(-hh) );
            glVertex3d(l1-margin, GLdouble(-bb/2.0), 0.0 );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0), GLdouble(-hh) );
            glVertex3d(l2+margin, GLdouble(-bb/2.0), 0.0 );
            glEnd();
        }
    }

    // ウェブ破線
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1,0x0F0F);
    if(isColumn){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0+margin), l1-margin );
        glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0+margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0-margin), l2+margin );
        glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0-margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0+margin), l1-margin );
            glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0-margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0), GLdouble( hh/2.0+margin), l2+margin );
            glVertex3d(GLdouble(-bb/2.0), GLdouble(-hh/2.0-margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0), GLdouble(-hh-margin) );
        glVertex3d(l2+margin, GLdouble(-bb/2.0), GLdouble(-hh-margin) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0), margin );
        glVertex3d(l2+margin, GLdouble(-bb/2.0), margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0), GLdouble(-hh-margin) );
            glVertex3d(l1-margin, GLdouble(-bb/2.0), margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0), GLdouble(-hh-margin) );
            glVertex3d(l2+margin, GLdouble(-bb/2.0), margin );
            glEnd();
        }
    }

    glDisable(GL_LINE_STIPPLE);

}
void OutputPersScene::drawCrossSolid(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn)
{

    // ウェブ1
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(GLdouble(-bb/2.0), 0.0, l1 );
        glVertex3d(GLdouble(-bb/2.0), 0.0, l2 );
        glVertex3d(GLdouble(bb/2.0), 0.0, l2 );
        glVertex3d(GLdouble(bb/2.0), 0.0, l1 );
    }else{
        glNormal3d(0.0,0.0,1.0);
        glVertex3d(l1, GLdouble(-bb/2.0), GLdouble(-hh/2.0) );
        glVertex3d(l2, GLdouble(-bb/2.0), GLdouble(-hh/2.0) );
        glVertex3d(l2, GLdouble(bb/2.0), GLdouble(-hh/2.0) );
        glVertex3d(l1, GLdouble(bb/2.0), GLdouble(-hh/2.0) );
    }
    glEnd();

    // ウェブ2
    glBegin(GL_POLYGON);
    if( isColumn ){
        glNormal3d(1.0,0.0,0.0);
        glVertex3d(0.0,GLdouble(-hh/2.0),  l1 );
        glVertex3d(0.0,GLdouble(-hh/2.0),  l2 );
        glVertex3d(0.0,GLdouble(hh/2.0),  l2 );
        glVertex3d(0.0,GLdouble(hh/2.0),  l1 );
    }else{
        glNormal3d(0.0,1.0,0.0);
        glVertex3d(l1, 0.0, GLdouble(-hh) );
        glVertex3d(l2, 0.0, GLdouble(-hh) );
        glVertex3d(l2, 0.0, GLdouble(0.0) );
        glVertex3d(l1, 0.0, GLdouble(0.0) );
    }
    glEnd();


}

void OutputPersScene::drawCrossWire(qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn,bool isStart,bool isEnd)
{

    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(GLdouble(-bb/2.0-margin), 0.0, l1-margin );
        glVertex3d(GLdouble(-bb/2.0-margin), 0.0, l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(GLdouble( bb/2.0+margin), 0.0, l2+margin );
        glVertex3d(GLdouble( bb/2.0+margin), 0.0, l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), 0.0, l1-margin );
            glVertex3d(GLdouble( bb/2.0+margin), 0.0, l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(GLdouble(-bb/2.0-margin), 0.0, l2+margin );
            glVertex3d(GLdouble( bb/2.0+margin), 0.0, l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0) );
        glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, GLdouble( bb/2.0+margin), GLdouble(-hh/2.0) );
        glVertex3d(l1-margin, GLdouble( bb/2.0+margin), GLdouble(-hh/2.0) );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0) );
            glVertex3d(l1-margin, GLdouble( bb/2.0+margin), GLdouble(-hh/2.0) );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, GLdouble(-bb/2.0-margin), GLdouble(-hh/2.0) );
            glVertex3d(l2+margin, GLdouble( bb/2.0+margin), GLdouble(-hh/2.0) );
            glEnd();
        }
    }


    if( isColumn ){
        glBegin(GL_LINES);
        glVertex3d(0.0, GLdouble(-hh/2.0-margin), l1-margin );
        glVertex3d(0.0, GLdouble(-hh/2.0-margin), l2+margin );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(0.0, GLdouble(hh/2.0+margin), l2+margin );
        glVertex3d(0.0, GLdouble(hh/2.0+margin), l1-margin );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(0.0, GLdouble(-hh/2.0-margin), l1-margin );
            glVertex3d(0.0, GLdouble(hh/2.0+margin), l1-margin );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(0.0, GLdouble(-hh/2.0-margin), l2+margin );
            glVertex3d(0.0, GLdouble(hh/2.0+margin), l2+margin );
            glEnd();
        }
    }else{
        glBegin(GL_LINES);
        glVertex3d(l1-margin, 0.0, GLdouble(0.0+margin) );
        glVertex3d(l2+margin, 0.0, GLdouble(0.0+margin) );
        glEnd();
        glBegin(GL_LINES);
        glVertex3d(l2+margin, 0.0, GLdouble(-hh-margin) );
        glVertex3d(l1-margin, 0.0, GLdouble(-hh-margin) );
        glEnd();
        if(isStart){
            glBegin(GL_LINES);
            glVertex3d(l1-margin, 0.0, GLdouble(0.0+margin) );
            glVertex3d(l1-margin, 0.0, GLdouble(-hh-margin) );
            glEnd();
        }
        if(isEnd){
            glBegin(GL_LINES);
            glVertex3d(l2+margin, 0.0, GLdouble(0.0+margin) );
            glVertex3d(l2+margin, 0.0, GLdouble(-hh-margin) );
            glEnd();
        }
    }



}


QList<Vec> OutputPersScene::divideClosedMember( MemberData* md ) const
{
    //dummy
    qreal dispRatioXY = 1.0;//myViewSettings.getDispRatioXY();
    QStringList loadcase = QStringList()<<u8"RSTDL";
    QList<qreal> loadcoef = QList<qreal>()<<1.0;
    QList<int> loadstep = QList<int>()<<1;
    //dummy



    QList<Vec> tmpPoints;
    Q_FOREACH( JointData* jd, md->getJointList() ){

        XYZ c_xyz=jd->xyz();
        XYZ c_disp=getCombinationDisp(jd,dispRatioXY,loadcase,loadcoef,loadstep);

        c_xyz.x+=c_disp.x;
        c_xyz.y+=c_disp.y;
        c_xyz.z+=c_disp.z;
        tmpPoints.append( Vec(c_xyz.x,c_xyz.y,c_xyz.z) );

        //        tmpPoints.append( Vec( jd->xPos(),jd->yPos(),jd->zPos() ) );
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
//void OutputPersScene::drawMemberLabel(int fontsize)
//{
//    if( filteredMembers.isEmpty() ) return;

//    for( int i=0; i < filteredMembers.count() ; i++ ){
//        MemberData* md=filteredMembers.at(i);
//        if(md->getSection()->memberType==DATATYPE::TPWALL){
//            QList<JointData*> jdlist=md->getJointList();
//            QList<XYZ> xyzlist;
//            foreach(JointData* jd,jdlist){
//                XYZ xyz=jd->xyz();
//                XYZ disp=getCombinationDisp(jd,dispRatioXY,loadcase,loadcoef,loadstep);
//                xyz.x+=disp.x;
//                xyz.y+=disp.y;
//                xyz.z+=disp.z;
//                xyzlist.append(xyz);
//            }

//            qreal minx,maxx,miny,maxy,minz,maxz;
//            for(int i=0;i<xyzlist.count();i++){
//                if(i==0){
//                    minx=xyzlist.at(i).x;
//                    maxx=xyzlist.at(i).x;
//                    miny=xyzlist.at(i).y;
//                    maxy=xyzlist.at(i).y;
//                    minz=xyzlist.at(i).z;
//                    maxz=xyzlist.at(i).z;
//                }else{
//                    minx=qMin(minx,xyzlist.at(i).x);
//                    maxx=qMax(maxx,xyzlist.at(i).x);
//                    miny=qMin(miny,xyzlist.at(i).y);
//                    maxy=qMax(maxy,xyzlist.at(i).y);
//                    minz=qMin(minz,xyzlist.at(i).z);
//                    maxz=qMax(maxz,xyzlist.at(i).z);
//                }
//            }
//            XYZ spoint;
//            XYZ epoint;
//            if(md->option.wallDirection==OptionalMemberData::IDATATYPE::TPWALL){
//                spoint=XYZ((minx+maxx)/2.0,(miny+maxy)/2.0,minz);//i端
//                epoint=XYZ((minx+maxx)/2.0,(miny+maxy)/2.0,maxz);//j端
//            }else if(md->option.wallDirection==OptionalMemberData::HDATATYPE::TPWALL && (maxx-minx)>=(maxy-miny)){//X軸上のH壁
//                spoint=XYZ(minx,(miny+maxy)/2.0,(minz+maxz)/2.0);//i端
//                epoint=XYZ(maxx,(miny+maxy)/2.0,(minz+maxz)/2.0);//j端
//            }else if(md->option.wallDirection==OptionalMemberData::HDATATYPE::TPWALL && (maxx-minx)<(maxy-miny)){//Y軸上のH壁
//                spoint=XYZ((minx+maxx)/2.0,miny,(minz+maxz)/2.0);//i端
//                epoint=XYZ((minx+maxx)/2.0,maxy,(minz+maxz)/2.0);//j端
//            }


//            //colorは0～255での指定なので、0.5はほぼほぼ黒
//            QColor color=QColor(0.5,0.5,0.5);
//            QFont font;
//            font.setFamily(QString::fromLocal8Bit("Serif"));
//            font.setPointSize(fontsize);

//            if(md->option.wallDirection==OptionalMemberData::HDATATYPE::TPWALL){

//                glPushMatrix();
//                setLocalMatrix(spoint.x, spoint.y, spoint.z, epoint.x, epoint.y, epoint.z,
//                               0,0,0,DATATYPE::TPGIRDER,DIRECTION_Z,true);
//                qreal len=qSqrt((maxx-minx)*(maxx-minx) + (maxy-miny)*(maxy-miny));
//                drawText(XYZ(len/2.0,0,0),font,color,md->getSection()->sectionName,TEXTPOS::CENTER);
//                glPopMatrix();

//            }else if(md->option.wallDirection==OptionalMemberData::IDATATYPE::TPWALL){
//                glPushMatrix();
//                qreal angle = atan2(maxy-miny,maxx-minx)*360.0/6.283185;
//                setLocalMatrix(spoint.x, spoint.y, spoint.z, epoint.x, epoint.y, epoint.z,
//                               0,0,angle,DATATYPE::TPCOLUMN,DIRECTION_Z,true);
//                glRotatef(90.0,1.0,0.0,0.0);
//                drawText(XYZ(0,(maxz-minz)*0.5,0),font,color,md->getSection()->sectionName,TEXTPOS::CENTER);
//                glPopMatrix();

//            }

//        }

//        if(md->getSection()->memberType!=DATATYPE::TPWALL){

//            JointData* ijd=md->iJoint();
//            XYZ i_xyz=ijd->xyz();
//            XYZ i_disp=getCombinationDisp(ijd,dispRatioXY,loadcase,loadcoef,loadstep);
//            i_xyz.x+=i_disp.x;
//            i_xyz.y+=i_disp.y;
//            i_xyz.z+=i_disp.z;
//            JointData* jjd=md->jJoint();
//            XYZ j_xyz=jjd->xyz();
//            XYZ j_disp=getCombinationDisp(jjd,dispRatioXY,loadcase,loadcoef,loadstep);
//            j_xyz.x+=j_disp.x;
//            j_xyz.y+=j_disp.y;
//            j_xyz.z+=j_disp.z;

//            SECTIONDIM* sc=md->getSection();

//            glPushMatrix();

//            qreal th=md->getCodeAngle()/180.0*M_PI;
//            qreal hh=sc->sizeB/2.0*qAbs( qSin(th) )+sc->sizeH/2.0*qAbs( qCos(th) );

//            bool isVertical=false;
//            if(qAbs(ijd->xPos()-jjd->xPos())+qAbs(ijd->yPos()-jjd->yPos())<0.0001)isVertical=true;
//            setLocalMatrix(i_xyz.x, i_xyz.y, i_xyz.z, j_xyz.x, j_xyz.y, j_xyz.z,
//                           sc->sizeH/2.0,hh,md->getCodeAngle(),DATATYPE::TPGIRDER,DIRECTION_Z,isVertical);

//            Vec a1(j_xyz.x-i_xyz.x,j_xyz.y-i_xyz.y,j_xyz.z-i_xyz.z);

//            QString secName=sc->sectionName.trimmed();
//            secName=secName.split("-").at(0);
//            QColor color=QColor(0.5,0.5,0.5);
//            QFont font;
//            font.setFamily(QString::fromLocal8Bit("Serif"));
//            font.setPointSize(fontsize);

//            if(md->dataType()==DATATYPE::TPCOLUMN){
//                glPushMatrix();
//                glRotatef(135,1.0,0.0,0.0);
//                drawText(XYZ(a1.norm()/2.0,0.0,0.0),font,color,secName,TEXTPOS::TOP2);
//                glPopMatrix();
//            }else if(md->dataType()!=DATATYPE::TPCOLUMN){
//                glPushMatrix();
//                glRotatef(45,1.0,0.0,0.0);
//                drawText(XYZ(a1.norm()/2.0,0.0,0.0),font,color,secName,TEXTPOS::TOP2);
//                glPopMatrix();
//            }
//            glPopMatrix();
//        }
//    }//部材ループ
//}

//void OutputPersScene::drawNumberLabel(int fontsize)
//{
//    if( filteredMembers.isEmpty() ) return;

//    for( int i=0; i < filteredMembers.count() ; i++ ){
//        MemberData* md=filteredMembers.at(i);

//        if(md->getSection()->memberType==DATATYPE::TPWALL && md->option.isStructuralWall==true){
//            QList<JointData*> jdlist=md->getJointList();
//            QList<XYZ> xyzlist;
//            foreach(JointData* jd,jdlist){
//                XYZ xyz=jd->xyz();
//                XYZ disp=getCombinationDisp(jd,dispRatioXY,loadcase,loadcoef,loadstep);
//                xyz.x+=disp.x;
//                xyz.y+=disp.y;
//                xyz.z+=disp.z;
//                xyzlist.append(xyz);
//            }

//            qreal minx,maxx,miny,maxy,minz,maxz;
//            for(int i=0;i<xyzlist.count();i++){
//                if(i==0){
//                    minx=xyzlist.at(i).x;
//                    maxx=xyzlist.at(i).x;
//                    miny=xyzlist.at(i).y;
//                    maxy=xyzlist.at(i).y;
//                    minz=xyzlist.at(i).z;
//                    maxz=xyzlist.at(i).z;
//                }else{
//                    minx=qMin(minx,xyzlist.at(i).x);
//                    maxx=qMax(maxx,xyzlist.at(i).x);
//                    miny=qMin(miny,xyzlist.at(i).y);
//                    maxy=qMax(maxy,xyzlist.at(i).y);
//                    minz=qMin(minz,xyzlist.at(i).z);
//                    maxz=qMax(maxz,xyzlist.at(i).z);
//                }
//            }
//            XYZ spoint;
//            XYZ epoint;
//            if(md->option.wallDirection==OptionalMemberData::IDATATYPE::TPWALL){
//                spoint=XYZ((minx+maxx)/2.0,(miny+maxy)/2.0,minz);//i端
//                epoint=XYZ((minx+maxx)/2.0,(miny+maxy)/2.0,maxz);//j端
//            }else if(md->option.wallDirection==OptionalMemberData::HDATATYPE::TPWALL && (maxx-minx)>=(maxy-miny)){//X軸上のH壁
//                spoint=XYZ(minx,(miny+maxy)/2.0,(minz+maxz)/2.0);//i端
//                epoint=XYZ(maxx,(miny+maxy)/2.0,(minz+maxz)/2.0);//j端
//            }else if(md->option.wallDirection==OptionalMemberData::HDATATYPE::TPWALL && (maxx-minx)<(maxy-miny)){//Y軸上のH壁
//                spoint=XYZ((minx+maxx)/2.0,miny,(minz+maxz)/2.0);//i端
//                epoint=XYZ((minx+maxx)/2.0,maxy,(minz+maxz)/2.0);//j端
//            }
//            glPushMatrix();
//            if(md->option.wallDirection==OptionalMemberData::HDATATYPE::TPWALL){
//                setLocalMatrix(spoint.x, spoint.y, spoint.z, epoint.x, epoint.y, epoint.z,
//                               0,0,0,DATATYPE::TPGIRDER,DIRECTION_Z,true);
//            }else if(md->option.wallDirection==OptionalMemberData::IDATATYPE::TPWALL){
//                qreal angle = atan2(maxy-miny,maxx-minx)*360.0/6.283185;
//                setLocalMatrix(spoint.x, spoint.y, spoint.z, epoint.x, epoint.y, epoint.z,
//                               0,0,angle,DATATYPE::TPCOLUMN,DIRECTION_Z,true);
//                glRotatef(90.0,1.0,0.0,0.0);
//            }

//            Vec vec(epoint.x-spoint.x,epoint.y-spoint.y,epoint.z-spoint.z);
//            QList<bool> ftypelist=forceType["W"];
//            qDebug()<<ftypelist;
//            if((ftypelist.at(0)||ftypelist.at(1)||ftypelist.at(2))==false){
//                glPopMatrix();
//                continue;//全部falseの場合
//            }
//            qreal ivalue=0,cvalue=0,jvalue=0;
//            MemberForce mf;
//            if(nodeOrFace==0)mf=md->getCombinationNodeMemberForce(loadcoef,loadcase,loadstep);
//            if(nodeOrFace==1)mf=md->getCombinationFaceMemberForce(loadcoef,loadcase,loadstep);
//            for(int ftype=0;ftype<3;ftype++){
//                if(ftypelist.at(ftype)==false)continue;//描画しない応力→スキップ
//                switch(ftype){
//                case 0:
//                    cvalue=mf.N;
//                    break;
//                case 1:
//                    cvalue=-mf.iQy;
//                    break;
//                case 2:
//                    ivalue=-mf.iMz;
//                    jvalue=mf.jMz;
//                    break;
//                default:
//                    break;
//                }
//                //                qDebug()<<ivalue<<cvalue<<jvalue;
//                QColor color=QColor(120,120,120);
//                QFont font;
//                font.setFamily(QString::fromLocal8Bit("Serif"));
//                font.setPointSize(fontsize);
//                if(md->option.wallDirection==OptionalMemberData::IDATATYPE::TPWALL){
//                    if(ftype==0){
//                        QString cstr=QString::number((int)cvalue);
//                        drawText(XYZ(0.0,0.0,0.01),font,color,cstr,TEXTPOS::TOP2);
//                    }else if(ftype==1){
//                        QString cstr=QString::number((int)cvalue);
//                        drawText(XYZ(0.0,vec.norm(),0.01),font,color,cstr,TEXTPOS::BOTTOM2);
//                    }
//                    else{
//                        QString istr=QString::number((int)ivalue);
//                        drawText(XYZ(0.0,0.0,0.01),font,color,istr,TEXTPOS::TOP1);
//                        QString jstr=QString::number((int)jvalue);
//                        drawText(XYZ(0.0,vec.norm(),0.01),font,color,jstr,TEXTPOS::BOTTOM1);
//                    }
//                }else if(md->option.wallDirection==OptionalMemberData::HDATATYPE::TPWALL){
//                    if(ftype==0){
//                        QString cstr=QString::number((int)cvalue);
//                        drawText(XYZ(vec.norm()/2.0,0.0,0.01),font,color,cstr,TEXTPOS::BOTTOM2);
//                    }else if(ftype==1){
//                        QString cstr=QString::number((int)cvalue);
//                        drawText(XYZ(vec.norm()/2.0,0.0,0.01),font,color,cstr,TEXTPOS::TOP2);
//                    }
//                    else{
//                        QString istr=QString::number((int)ivalue);
//                        drawText(XYZ(0.0,0.0,0.01),font,color,istr,TEXTPOS::RIGHT);
//                        QString jstr=QString::number((int)jvalue);
//                        drawText(XYZ(0.0,vec.norm(),0.01),font,color,jstr,TEXTPOS::LEFT);
//                    }

//                }
//            }
//            glPopMatrix();
//        }


//        if(md->getSection()->memberType!=DATATYPE::TPWALL){//線部材のとき

//            JointData* ijd=md->iJoint();
//            XYZ i_xyz=ijd->xyz();
//            XYZ i_disp=getCombinationDisp(ijd,dispRatioXY,loadcase,loadcoef,loadstep);
//            i_xyz.x+=i_disp.x;
//            i_xyz.y+=i_disp.y;
//            i_xyz.z+=i_disp.z;
//            JointData* jjd=md->jJoint();
//            XYZ j_xyz=jjd->xyz();
//            XYZ j_disp=getCombinationDisp(jjd,dispRatioXY,loadcase,loadcoef,loadstep);
//            j_xyz.x+=j_disp.x;
//            j_xyz.y+=j_disp.y;
//            j_xyz.z+=j_disp.z;

//            SECTIONDIM* sc=md->getSection();

//            glPushMatrix();

//            qreal th=md->getCodeAngle()/180.0*M_PI;
//            qreal hh=sc->sizeB/2.0*qAbs( qSin(th) )+sc->sizeH/2.0*qAbs( qCos(th) );
//            bool isVertical=false;
//            if(qAbs(ijd->xPos()-jjd->xPos())+qAbs(ijd->yPos()-jjd->yPos())<0.0001)isVertical=true;
//            setLocalMatrix(i_xyz.x, i_xyz.y, i_xyz.z, j_xyz.x, j_xyz.y, j_xyz.z,
//                           sc->sizeH/2.0,hh,md->getCodeAngle(),md->dataType(),DIRECTION_Z,isVertical);

//            Vec a1(j_xyz.x-i_xyz.x,j_xyz.y-i_xyz.y,j_xyz.z-i_xyz.z);

//            QList<bool> ftypelist;
//            if(md->dataType()==DATATYPE::TPCOLUMN){
//                ftypelist=forceType["C"];
//            }else if(md->dataType()==DATATYPE::TPGIRDER){
//                ftypelist=forceType["G"];
//            }else if(md->dataType()==DATATYPE::TPBRACE){
//                ftypelist=forceType["B"];
//            }
//            if((ftypelist.at(0)||ftypelist.at(1)||ftypelist.at(2)||ftypelist.at(3)||ftypelist.at(4)||ftypelist.at(5))==false){
//                glPopMatrix();
//                continue;
//            }//全部falseの場合

//            qreal lvalue,cvalue,rvalue;
//            MemberForce mf;
//            if(nodeOrFace==0)mf=md->getCombinationNodeMemberForce(loadcoef,loadcase,loadstep);
//            if(nodeOrFace==1)mf=md->getCombinationFaceMemberForce(loadcoef,loadcase,loadstep);
//            QColor color=QColor(120,120,120);
//            QFont font;
//            font.setFamily(QString::fromLocal8Bit("Serif"));
//            font.setPointSize(fontsize);




//            for(int ftype=0;ftype<6;ftype++){
//                if(ftypelist.at(ftype)==false)continue;//描画しない応力→スキップ

//                bool isNewPstn = (globalData::get()->npstn != nullptr);
//                if(isNewPstn){
//                    //faceのとき、iPos=1 or 3、中央でiPos=2,節点の時、iPos=0かつpos=0 or 1？確認
//                    QVector<qreal> itotal(6,0.0);
//                    QVector<qreal> itmp(6,0.0);
//                    QVector<qreal> ctotal(6,0.0);
//                    QVector<qreal> ctmp(6,0.0);
//                    QVector<qreal> jtotal(6,0.0);
//                    QVector<qreal> jtmp(6,0.0);
//                    //                    qDebug()<<md->no()<<md->getTotalLength()<<x<<loadcase.at(i)<<loadstep.at(i);

//                    if(nodeOrFace == 0){//node
//                        for(int j=0;j<loadcase.count();j++){
//                            if(loadcase.at(j).isEmpty())continue;
//                            if(loadstep.at(j)==0)continue;
//                            //        //stepが0、rstがinvalidだったら0、　stepが負や巨大なとき、最終ステップを返すようにしたい(calcMemberForceで対応)
//                            itmp = calcAnyPointStress::calcMemberForce(md->no(),0,0,loadcase.at(j),loadstep.at(j));
//                            ctmp = calcAnyPointStress::calcMemberForce(md->no(),0,0.5*md->getTotalLength(),loadcase.at(j),loadstep.at(j));
//                            jtmp = calcAnyPointStress::calcMemberForce(md->no(),0,md->getTotalLength(),loadcase.at(j),loadstep.at(j));
//                            for(int k=0;k<6;k++){
//                                itotal[k] += itmp[k] *  loadcoef.at(j);
//                                ctotal[k] += ctmp[k] *  loadcoef.at(j);
//                                jtotal[k] += jtmp[k] *  loadcoef.at(j);
//                            }
//                        }

//                    }else if(nodeOrFace == 1){
//                        for(int j=0;j<loadcase.count();j++){
//                            if(loadcase.at(j).isEmpty())continue;
//                            if(loadstep.at(j)==0)continue;
//                            //        //stepが0、rstがinvalidだったら0、　stepが負や巨大なとき、最終ステップを返すようにしたい(calcMemberForceで対応)
//                            //xの値、フェイス基準にする。描画もフェイスに揃える
//                            itmp = calcAnyPointStress::calcMemberForce(md->no(),1,0,loadcase.at(j),loadstep.at(j));
//                            ctmp = calcAnyPointStress::calcMemberForce(md->no(),2,0,loadcase.at(j),loadstep.at(j));
//                            jtmp = calcAnyPointStress::calcMemberForce(md->no(),3,0,loadcase.at(j),loadstep.at(j));
//                            for(int k=0;k<6;k++){
//                                itotal[k] += itmp[k] *  loadcoef.at(j);
//                                ctotal[k] += ctmp[k] *  loadcoef.at(j);
//                                jtotal[k] += jtmp[k] *  loadcoef.at(j);
//                            }
//                        }

//                    }

//                    switch(ftype){
//                    case 0:
//                        cvalue=mf.N + itotal.at(0);
//                        break;
//                    case 1:
//                        lvalue=-mf.getQy(0.0) - itotal.at(1);
//                        cvalue= -mf.getQy(0.5)-ctotal.at(1);
//                        rvalue=-mf.getQy(1.0) - jtotal.at(1);
//                        break;
//                    case 2:
//                        lvalue=-mf.getQz(0.0)- itotal.at(2);
//                        cvalue= -mf.getQz(0.5)-ctotal.at(2);
//                        rvalue=-mf.getQz(1.0)- jtotal.at(2);
//                        break;
//                    case 3:
//                        lvalue=mf.getMx(0.0)+ itotal.at(3);
//                        cvalue=mf.getMx(0.5)-ctotal.at(3);
//                        rvalue=mf.getMx(1.0)+ jtotal.at(3);
//                        break;
//                    case 4:
//                        lvalue= -mf.getMy(0.0)- itotal.at(4);
//                        cvalue= -mf.getMy(0.5)-ctotal.at(4);
//                        rvalue=mf.getMy(1.0)+ jtotal.at(4);
//                        break;
//                    case 5:
//                        lvalue=-mf.getMz(0.0)- itotal.at(5);
//                        cvalue=-mf.getMz(0.5)-ctotal.at(5);
//                        rvalue=mf.getMz(1.0)+ jtotal.at(5);
//                        break;
//                    default:
//                        break;

//                    }
//                }


//                else{


//                    switch(ftype){
//                    case 0:
//                        cvalue=mf.N;
//                        break;
//                    case 1:
//                        lvalue=-mf.getQy(0.0);
//                        cvalue=-mf.getQy(0.5);
//                        rvalue=-mf.getQy(1.0);
//                        break;
//                    case 2:
//                        lvalue=-mf.getQz(0.0);
//                        cvalue=-mf.getQz(0.5);
//                        rvalue=-mf.getQz(1.0);
//                        break;
//                    case 3:
//                        lvalue=mf.getMx(0.0);
//                        cvalue=mf.getMx(0.5);
//                        rvalue=mf.getMx(1.0);
//                        break;
//                    case 4:
//                        lvalue=-mf.getMy(0.0);
//                        cvalue=-mf.getMy(0.5);
//                        rvalue=mf.getMy(1.0);
//                        break;
//                    case 5:
//                        lvalue=-mf.getMz(0.0);
//                        cvalue=-mf.getMz(0.5);
//                        rvalue=mf.getMz(1.0);
//                        break;
//                    default:
//                        break;

//                    }
//                }

//                qDebug()<<i<<"set value";
//                if(ftype==4||ftype==5){//モーメントの場合、端部中央 4...My,5...Mz
//                    QString lstr=QString::number((int)lvalue);
//                    QString cstr=QString::number((int)cvalue);
//                    QString rstr=QString::number((int)rvalue);
//                    if(md->dataType()==DATATYPE::TPCOLUMN && ftype==4){//柱、Y方向荷重時のモーメント
//                        //ローカルx軸周りに90度回す。その後ローカルy軸回りにも
//                        //文字描画座標系＝xが右、yが上、原点がi端になるように
//                        glPushMatrix();
//                        glRotatef(90.0,1.0,0.0,0.0);
//                        glRotatef(90.0,0.0,1.0,0.0);
//                        drawText(XYZ(0.0,0.0,0.0),font,color,lstr,TEXTPOS::TOPRIGHT1);
//                        //                        drawText(XYZ(0.0,a1.norm()/2.0,0.0),font,color,cstr,TEXTPOS::RIGHT);
//                        drawText(XYZ(0.0,a1.norm(),0.0),font,color,rstr,TEXTPOS::BOTTOMLEFT1);
//                        glPopMatrix();
//                    }else if(md->dataType()==DATATYPE::TPCOLUMN && ftype==5){//柱、X方向荷重時のモーメント
//                        //ローカルx軸周りに90度回す
//                        glPushMatrix();
//                        glRotatef(90.0,1.0,0.0,0.0);
//                        drawText(XYZ(0.0,0.0,0.0),font,color,lstr,TEXTPOS::TOPRIGHT1);
//                        //                        drawText(XYZ(0.0,a1.norm()/2.0,0.0),font,color,cstr,TEXTPOS::RIGHT);
//                        drawText(XYZ(0.0,a1.norm(),0.0),font,color,rstr,TEXTPOS::BOTTOMLEFT1);
//                        glPopMatrix();
//                    }else if(md->dataType()!=DATATYPE::TPCOLUMN && ftype==4){//面内曲げの梁
//                        //90度軸回りに回す ok
//                        glPushMatrix();
//                        glRotatef(90.0,1.0,0.0,0.0);
//                        drawText(XYZ(0.0,0.0,0.0),font,color,lstr,TEXTPOS::BOTTOMRIGHT1);
//                        drawText(XYZ(a1.norm()/2.0,0.0,0.0),font,color,cstr,TEXTPOS::BOTTOM1);
//                        drawText(XYZ(a1.norm(),0.0,0.0),font,color,rstr,TEXTPOS::TOPLEFT1);
//                        glPopMatrix();
//                    }else if(md->dataType()!=DATATYPE::TPCOLUMN && ftype==5){//面外曲げの梁
//                        //そのまま ok
//                        drawText(XYZ(0.0,0.0,0.0),font,color,lstr,TEXTPOS::BOTTOMRIGHT1);
//                        drawText(XYZ(a1.norm()/2.0,0.0,0.0),font,color,cstr,TEXTPOS::BOTTOM1);
//                        drawText(XYZ(a1.norm(),0.0,0.0),font,color,rstr,TEXTPOS::TOPLEFT1);
//                    }
//                }else if(ftype==0){//軸力場合、中央のみ 0...N
//                    QString cstr=QString::number((int)cvalue);
//                    if(md->dataType()==DATATYPE::TPCOLUMN){
//                        glPushMatrix();
//                        glRotatef(90.0,1.0,0.0,0.0);
//                        glRotatef(45.0,0.0,1.0,0.0);
//                        drawText(XYZ(0.0,a1.norm()/2.0,0.0),font,color,cstr,TEXTPOS::LEFT);
//                        glPopMatrix();
//                    }else if(md->dataType()!=DATATYPE::TPCOLUMN){
//                        glPushMatrix();
//                        glRotatef(45.0,1.0,0.0,0.0);
//                        drawText(XYZ(a1.norm()/2.0,0.0,0.0),font,color,cstr,TEXTPOS::TOP1);
//                        glPopMatrix();
//                    }
//                }else{//両端　1...Qy,2...Qz,3...Mx
//                    QString lstr=QString::number((int)lvalue);
//                    QString rstr=QString::number((int)rvalue);
//                    QString cstr=QString::number((int)cvalue);
//                    //qDebug()<<lstr<<rstr;
//                    if(md->dataType()==DATATYPE::TPCOLUMN &&  (ftype==1)){//柱のせん断
//                        glPushMatrix();
//                        glRotatef(90.0,1.0,0.0,0.0);
//                        drawText(XYZ(0.0,a1.norm()/2.0,0.0),font,color,cstr,TEXTPOS::RIGHT);
//                        glPopMatrix();
//                    }else if(md->dataType()==DATATYPE::TPCOLUMN && ftype==2){//柱のせん断
//                        glPushMatrix();
//                        glRotatef(90.0,1.0,0.0,0.0);
//                        glRotatef(90.0,0.0,1.0,0.0);
//                        drawText(XYZ(0.0,a1.norm()/2.0,0.0),font,color,cstr,TEXTPOS::RIGHT);
//                        glPopMatrix();
//                    }else if(md->dataType()==DATATYPE::TPCOLUMN && ftype==3){//柱のねじれ
//                        glPushMatrix();
//                        glRotatef(90.0,1.0,0.0,0.0);
//                        glRotatef(45.0,0.0,1.0,0.0);
//                        drawText(XYZ(0.0,a1.norm()/2.0,0.0),font,color,cstr,TEXTPOS::BOTTOMLEFT2);
//                        glPopMatrix();
//                    }else if(md->dataType()!=DATATYPE::TPCOLUMN && (ftype==1)){//梁のせん断
//                        drawText(XYZ(0.0,0.0,0.0),font,color,lstr,TEXTPOS::TOPRIGHT2);
//                        drawText(XYZ(a1.norm(),0.0,0.0),font,color,rstr,TEXTPOS::TOPLEFT2);
//                    }else if(md->dataType()!=DATATYPE::TPCOLUMN && (ftype==2)){//梁のせん断
//                        glPushMatrix();
//                        glRotatef(90.0,1.0,0.0,0.0);
//                        drawText(XYZ(0.0,0.0,0.0),font,color,lstr,TEXTPOS::TOPRIGHT2);
//                        drawText(XYZ(a1.norm(),0.0,0.0),font,color,rstr,TEXTPOS::TOPLEFT2);
//                        glPopMatrix();
//                    }else if(md->dataType()!=DATATYPE::TPCOLUMN && ftype==3){//梁のねじれ
//                        glPushMatrix();
//                        glRotatef(45.0,1.0,0.0,0.0);
//                        drawText(XYZ(a1.norm()/2,0.0,0.0),font,color,cstr,TEXTPOS::BOTTOM2);
//                        glPopMatrix();
//                    }
//                }//数値描画
//                qDebug()<<ftype<<"force done";
//            }//応力タイプループ
//            glPopMatrix();
//            qDebug()<<"all type of force done";
//        }//部材タイプ
//    }//部材ループ
//    qDebug()<<"draw number done";
//    //    DRAWDIRECTION dir;
//    //    if(     ( md->dataType()==DATATYPE::TPCOLUMN && (ftype==0 || ftype==1 || ftype==3 || ftype==5))
//    //             ||(md->dataType()!=DATATYPE::TPCOLUMN && (ftype==1 || ftype==5))){
//    //        dir=DIR_Y;
//    //    }else if((md->dataType()==DATATYPE::TPCOLUMN && (ftype==2 || ftype==4))
//    //             ||(md->dataType()!=DATATYPE::TPCOLUMN && (ftype==0 || ftype==2 || ftype==3 || ftype==4))){
//    //        dir=DIR_Z;
//    //    }

//}


void OutputPersScene::drawGraph(int selectGraphScale,qreal unitforce)
{
qDebug()<<"drawGraph!!";
    //dummy
    qreal dispRatioXY = 1.0;//myViewSettings.getDispRatioXY();
    QStringList loadcase = QStringList()<<u8"RSTDL";
    QList<qreal> loadcoef = QList<qreal>()<<1.0;
    QList<int> loadstep = QList<int>()<<1;
    STRESS st1;
    st1.rst=loadcase.at(0);
    st1.coef = loadcoef.at(0);
    //st1.step=loadstep.at(0);
    QList<STRESS> stress;
    stress<<st1;
    bool drawContour = true;

    //dummy



    //    bool isNewPstn = (globalData::get()->npstn != nullptr);
    //最大値がscalefactorメートルで表示
    enum DRAWDIRECTION{DIR_Y=0,DIR_Z};
    if( filteredMembers.isEmpty() ) return;

    UnifiedAnalysisData* uad=UnifiedFixedData::getInstance()->getAnalysisData();
    Analysis3DViewSettings  myViewSettings=UnifiedSettings::getInstance()->getOutput3DViewSettings();


    for( int i=0; i < filteredMembers.count() ; i++ ){
        MemberData* md=filteredMembers.at(i);
        QUuid uid = md->getUuid();
        if(md->dataType()!=DATATYPE::TPCOLUMN
                && md->dataType()!=DATATYPE::TPGIRDER
                && md->dataType()!=DATATYPE::TPBRACE)continue;
        //        if(md->getSection()->memberType==DATATYPE::TPWALL)continue;

        JointData* ijd=md->iJoint();
        XYZ i_xyz=ijd->xyz();
        XYZ i_disp=getCombinationDisp(ijd,dispRatioXY,loadcase,loadcoef,loadstep);
        i_xyz.x+=i_disp.x;
        i_xyz.y+=i_disp.y;
        i_xyz.z+=i_disp.z;
        JointData* jjd=md->jJoint();
        XYZ j_xyz=jjd->xyz();
        XYZ j_disp=getCombinationDisp(jjd,dispRatioXY,loadcase,loadcoef,loadstep);
        j_xyz.x+=j_disp.x;
        j_xyz.y+=j_disp.y;
        j_xyz.z+=j_disp.z;

        //TODO:部材中央断面
        LinePartsParameter sc =uad->getSectionValues(md->dataType(),md->sectionName(),i_xyz.z).c_param;

        glPushMatrix();

        qreal th=md->codeAngle()/180.0*M_PI;
        qreal hh=sc.sizeB/2.0*qAbs( qSin(th) )+sc.sizeH/2.0*qAbs( qCos(th) );
        bool isVertical=false;
        if(qAbs(ijd->xPos()-jjd->xPos())+qAbs(ijd->yPos()-jjd->yPos())<0.0001)isVertical=true;
        setLocalMatrix(i_xyz.x, i_xyz.y, i_xyz.z, j_xyz.x, j_xyz.y, j_xyz.z,
                       sc.sizeH/2.0,hh,md->codeAngle(),md->dataType(),DIRECTION_Z,isVertical);

        Vec a1(j_xyz.x-i_xyz.x,j_xyz.y-i_xyz.y,j_xyz.z-i_xyz.z);

        const int div=10;
        QList<QColor> colorlist;
        QList<qreal> valuelist;
        QList<qreal> lengthlist;

        //        QList<bool> ftypelist;
        //        if(md->dataType()==DATATYPE::TPCOLUMN){
        //            ftypelist=forceType["C"];
        //        }else if(md->dataType()==DATATYPE::TPGIRDER){
        //            ftypelist=forceType["G"];
        //        }else if(md->dataType()==DATATYPE::TPBRACE){
        //            ftypelist=forceType["B"];
        //        }
        //TODO:MとQとNでグラフを分ける
        QVector<bool> ftypelist(6,false);
        if(widgetState.drawN)ftypelist[0]=true;
        if(widgetState.drawQ)ftypelist[1]=true;
        if(widgetState.drawQ)ftypelist[2]=true;
        if(widgetState.drawM)ftypelist[3]=true;
        if(widgetState.drawM)ftypelist[4]=true;
        if(widgetState.drawM)ftypelist[5]=true;

        for(int ftype=0;ftype<6;ftype++){//ftype=N,Qy,Qz,Mx,My,Mz
            colorlist.clear();
            valuelist.clear();
            lengthlist.clear();
            if(ftypelist.at(ftype)==false)continue;


            DRAWDIRECTION dir;
            if(     ( md->dataType()==DATATYPE::TPCOLUMN && (ftype==0 || ftype==1 || ftype==3 || ftype==5))
                    ||(md->dataType()!=DATATYPE::TPCOLUMN && (ftype==1 || ftype==5))){
                dir=DIR_Y;
            }else if((md->dataType()==DATATYPE::TPCOLUMN && (ftype==2 || ftype==4))
                     ||(md->dataType()!=DATATYPE::TPCOLUMN && (ftype==0 || ftype==2 || ftype==3 || ftype==4))){
                dir=DIR_Z;
            }
            for(int i=0;i<=div;i++){
                qreal x;
                if( myViewSettings.getNodeOrFace() == 0){
                    x=(qreal)i/(qreal)div;
                }else{
                    //TODO:フェイス長さはfigureTemplateとかfigureTypePropertyの営み
                    //                    QVector<qreal> len_f_r=getOutputFaceRigidLength();
                    //                    qreal i_len =len_f_r.at(0)/ md->getTotalLength();
                    //                    qreal c_len = 1.0 - i_len - len_f_r.at(1)/ md->getTotalLength();
                    //                    x=i_len + c_len * (qreal)i/(qreal)div;
                }

                //forceの種類によってスイッチ
                qreal value;

                MemberForce mf;

                if(myViewSettings.getNodeOrFace() == 0)mf=UnifiedFixedData::getInstance()->getPstnData()->getCombinationNodeMemberForce(uid, stress);
                if(myViewSettings.getNodeOrFace() == 1)mf=UnifiedFixedData::getInstance()->getPstnData()->getCombinationFaceMemberForce(uid, stress);


                //


                //faceのとき、iPos=1 or 3、中央でiPos=2,節点の時、iPos=0かつpos=0 or 1？確認
                QVector<qreal> total(6,0.0);
//                QVector<qreal> tmp(6,0.0);
                MemberForceByIL mfil;
                //                    qDebug()<<md->no()<<md->getTotalLength()<<x<<loadcase.at(i)<<loadstep.at(i);
                if(myViewSettings.getNodeOrFace() == 0){//node
                    for(int j=0;j<loadcase.count();j++){
                        if(loadcase.at(j).isEmpty())continue;
                        if(loadstep.at(j)==0)continue;
                        //        //stepが0、rstがinvalidだったら0、　stepが負や巨大なとき、最終ステップを返すようにしたい(calcMemberForceで対応)
//                        tmp = calcAnyPointStress::calcMemberForce(md->no(),0,md->getTotalLength()*x,loadcase.at(j),loadstep.at(j));
                        mfil = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(uid,stress,0,x * md->getTotalLength());
//                        for(int k=0;k<6;k++){
                            total[0] += mfil.getN();
                            total[1] += mfil.getQy();
                            total[2] += mfil.getQz();
                            total[3] += mfil.getMx();
                            total[4] += mfil.getMy();
                            total[5] += mfil.getMz();
//                        }
                    }

                }else if(myViewSettings.getNodeOrFace() == 1){
                    for(int j=0;j<loadcase.count();j++){
                        if(loadcase.at(j).isEmpty())continue;
                        if(loadstep.at(j)==0)continue;
                        //        //stepが0、rstがinvalidだったら0、　stepが負や巨大なとき、最終ステップを返すようにしたい(calcMemberForceで対応)
                        //xの値、フェイス基準にする。描画もフェイスに揃える
//                        tmp = calcAnyPointStress::calcMemberForce(md->no(),0,md->getTotalLength()*x,loadcase.at(j),loadstep.at(j));
//                        mfil = UnifiedFixedData::getInstance()->getPstnData()
//                                ->getAnyPointMemberForce(uid,stress,0,iface + x*(jface-iface));

//                        for(int k=0;k<6;k++){
//                            total[k] += tmp[k] *  loadcoef.at(j);
//                        }
                    }

                }
                switch(ftype){
                case 0:
                    value=mf.N + total[0];
                    break;
                case 1:
                    value=mf.getQy((qreal)i/(qreal)div)+ total[1];
                    break;
                case 2:
                    value=mf.getQz((qreal)i/(qreal)div)+ total[2];
                    break;
                case 3:
                    value=mf.getMx((qreal)i/(qreal)div)+ total[3];
                    break;
                case 4:
                    value=mf.getMy((qreal)i/(qreal)div)+ total[4];
                    qDebug()<<u8"moment My"<<value;
                    break;
                case 5:
                    value=mf.getMz((qreal)i/(qreal)div)+ total[5];
                    qDebug()<<u8"moment Mz"<<value;
                    break;
                default:
                    break;

                }

                //



                QColor color;
                if(drawContour==true){color=colorpicker.getColor(value);}
                else{color=colorpicker.getColor();}
                valuelist<<value;
                colorlist<<color;
                if(selectGraphScale==0){//全部材の最大値=4m
                    const qreal maxlength=4.0;
                    qreal maxvalue;
                    if(ftype==0){maxvalue=qMax(qAbs(minmax.AllMax["N"]),qAbs(minmax.AllMin["N"]));}
                    else if(ftype==1 || ftype==2){maxvalue=qMax(qMax(qAbs(minmax.AllMax["Qy"]),qAbs(minmax.AllMin["Qy"])),qMax(qAbs(minmax.AllMin["Qz"]),qAbs(minmax.AllMin["Qz"])));}
                    else if(ftype==3){maxvalue=qMax(qAbs(minmax.AllMax["Mx"]),qAbs(minmax.AllMin["Mx"]));}
                    else if(ftype==4 || ftype==5){maxvalue=qMax(qMax(qAbs(minmax.AllMax["My"]),qAbs(minmax.AllMin["My"])),qMax(qAbs(minmax.AllMin["Mz"]),qAbs(minmax.AllMin["Mz"])));}
                    lengthlist<<value/maxvalue*maxlength;
                }else if(selectGraphScale==1){//表示部材の最大値=4m
                    const qreal maxlength=4.0;
                    qreal maxvalue;
                    if(ftype==0){maxvalue=qMax(qAbs(minmax.DisplayMax["N"]),qAbs(minmax.DisplayMin["N"]));}
                    else if(ftype==1 || ftype==2){maxvalue=qMax(qMax(qAbs(minmax.DisplayMax["Qy"]),qAbs(minmax.DisplayMin["Qy"])),qMax(qAbs(minmax.DisplayMin["Qz"]),qAbs(minmax.DisplayMin["Qz"])));}
                    else if(ftype==3){maxvalue=qMax(qAbs(minmax.DisplayMax["Mx"]),qAbs(minmax.DisplayMin["Mx"]));}
                    else if(ftype==4 || ftype==5){maxvalue=qMax(qMax(qAbs(minmax.DisplayMax["My"]),qAbs(minmax.DisplayMin["My"])),qMax(qAbs(minmax.DisplayMin["Mz"]),qAbs(minmax.DisplayMin["Mz"])));}
                    lengthlist<<value/maxvalue*maxlength;
                }
                else if(selectGraphScale==2){//unitforceあたり1m
                    lengthlist<<value/unitforce;
                }
            }

            for(int i=0;i<=div;i++){
                qreal x,x1;
                if(myViewSettings.getNodeOrFace() == 0){
                    x=a1.norm()*(qreal)i/(qreal)div;
                    x1=a1.norm()*(qreal)(i+1)/(qreal)div;
                }else{
//                    qreal i_len =calcAnyPointStress::iFaceLen(md->no())/ md->getTotalLength();// npstn->model.dmm1[md->no()][6]  / md->getTotalLength();
//                    qreal c_len = 1.0 - i_len - calcAnyPointStress::jFaceLen(md->no())/ md->getTotalLength();// npstn->model.dmm1[md->no()][7] / md->getTotalLength();
//                    x=a1.norm()*(i_len + c_len * (qreal)i/(qreal)div);
//                    x1=a1.norm()*(i_len + c_len * (qreal)(i+1)/(qreal)div);
                }

                QColor color=colorlist.at(i);
                glColor3f(color.redF(),color.greenF(),color.blueF());
                if(md->dataType()==DATATYPE::TPCOLUMN && dir==DIR_Y){
                    glBegin(GL_LINES);
                    glVertex3d(0.0, 0.0, x );
                    glVertex3d(-lengthlist.at(i), 0.0, x );//i端が上？なのでマイナスとする
                    glEnd();
                }else if(md->dataType()==DATATYPE::TPCOLUMN && dir==DIR_Z){
                    glBegin(GL_LINES);
                    glVertex3d(0.0, 0.0, x  );
                    glVertex3d(0.0, lengthlist.at(i), x );
                    glEnd();
                }else if(md->dataType()!=DATATYPE::TPCOLUMN && dir==DIR_Y){
                    glBegin(GL_LINES);
                    glVertex3d(x,0.0, 0.0  );
                    glVertex3d(x,lengthlist.at(i), 0.0 );
                    glEnd();
                }else if(md->dataType()!=DATATYPE::TPCOLUMN && dir==DIR_Z){
                    glBegin(GL_LINES);
                    glVertex3d(x,0.0, 0.0  );
                    glVertex3d(x,0.0,lengthlist.at(i) );
                    glEnd();
                }else{qDebug()<<"部材種別と描画方向が不正な値";}
            }
            //稜線をつなぐ
            for(int i=0;i<div;i++){
                qreal x,x1;
                if(myViewSettings.getNodeOrFace() == 0){
                    x=a1.norm()*(qreal)i/(qreal)div;
                    x1=a1.norm()*(qreal)(i+1)/(qreal)div;
                }else{
//                    qreal i_len =calcAnyPointStress::iFaceLen(md->no())/ md->getTotalLength();// npstn->model.dmm1[md->no()][6]  / md->getTotalLength();
//                    qreal c_len = 1.0 - i_len - calcAnyPointStress::jFaceLen(md->no())/ md->getTotalLength();// npstn->model.dmm1[md->no()][7] / md->getTotalLength();
//                    x=a1.norm()*(i_len + c_len * (qreal)i/(qreal)div);
//                    x1=a1.norm()*(i_len + c_len * (qreal)(i+1)/(qreal)div);
                }

                QColor color1=colorlist.at(i);
                QColor color2=colorlist.at(i+1);
                if(md->dataType()==DATATYPE::TPCOLUMN && dir==DIR_Y){
                    glBegin(GL_LINES);
                    glColor3f(color1.redF(),color1.greenF(),color1.blueF());
                    glVertex3d(-lengthlist.at(i), 0.0, x );
                    glColor3f(color2.redF(),color2.greenF(),color2.blueF());
                    glVertex3d(-lengthlist.at(i+1), 0.0, x1 );
                    glEnd();
                }else if(md->dataType()==DATATYPE::TPCOLUMN && dir==DIR_Z){
                    glBegin(GL_LINES);
                    glColor3f(color1.redF(),color1.greenF(),color1.blueF());
                    glVertex3d(0.0, lengthlist.at(i), x );
                    glColor3f(color2.redF(),color2.greenF(),color2.blueF());
                    glVertex3d(0.0, lengthlist.at(i+1), x1 );
                    glEnd();
                }else if(md->dataType()!=DATATYPE::TPCOLUMN && dir==DIR_Y){
                    glBegin(GL_LINES);
                    glColor3f(color1.redF(),color1.greenF(),color1.blueF());
                    glVertex3d(x,lengthlist.at(i), 0.0 );
                    glColor3f(color2.redF(),color2.greenF(),color2.blueF());
                    glVertex3d(x1,lengthlist.at(i+1), 0.0 );
                    glEnd();
                }else if(md->dataType()!=DATATYPE::TPCOLUMN && dir==DIR_Z){
                    glBegin(GL_LINES);
                    glColor3f(color1.redF(),color1.greenF(),color1.blueF());
                    glVertex3d(x,0.0,lengthlist.at(i) );
                    glColor3f(color2.redF(),color2.greenF(),color2.blueF());
                    glVertex3d(x1,0.0,lengthlist.at(i+1) );
                    glEnd();
                }
            }
        }
        glPopMatrix();
    }
}

//void OutputPersScene::drawCoordinate(){
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
//    glColor3d(0.4,0.0,0.0);
//    float linewidth;
//    glGetFloatv(GL_LINE_WIDTH , &linewidth);

//    glLineWidth(2);

//    glBegin(GL_LINES);
//    //x-axis
//    glVertex3d(-1.0,-1.0,0.0);
//    glVertex3d(4.0,-1.0,0.0);

//    glVertex3d(4.0,-1.0,0.0);
//    glVertex3d(5.0,0.0,0.0);

//    glVertex3d(5.0,0.0,0.0);
//    glVertex3d(4.0,1.0,0.0);

//    glVertex3d(4.0,1.0,0.0);
//    glVertex3d(-1.0,1.0,0.0);

//    glVertex3d(-1.0,-1.0,0.0);
//    glVertex3d(1.0,-1.0,0.0);

//    //y-axis
//    glVertex3d(1.0,-1.0,0.0);
//    glVertex3d(1.0,4.0,0.0);

//    glVertex3d(1.0,4.0,0.0);
//    glVertex3d(0.0,5.0,0.0);

//    glVertex3d(0.0,5.0,0.0);
//    glVertex3d(-1.0,4.0,0.0);

//    glVertex3d(-1.0,4.0,0.0);
//    glVertex3d(-1.0,-1.0,0.0);

//    //X
//    glVertex3d(3.0,-0.5,0.0);
//    glVertex3d(4.0,0.5,0.0);

//    glVertex3d(3.0,0.5,0.0);
//    glVertex3d(4.0,-0.5,0.0);

//    //Y
//    glVertex3d(-0.5,4.0,0.0);
//    glVertex3d(0.0,3.5,0.0);

//    glVertex3d(0.5,4.0,0.0);
//    glVertex3d(0.0,3.5,0.0);

//    glVertex3d(0.0,3.5,0.0);
//    glVertex3d(0.0,3.0,0.0);
//    glEnd();

////    drawText(XYZ(-1.0,-1.2,0.0),QFont("Arial",10),Qt::black,u8"3DVisual ver4.0.1 developped by (株)日建設計",TEXTPOS::BOTTOMRIGHT1);

//    glEnable(GL_LIGHTING);
//    glEnable(GL_DEPTH_TEST);

//    glLineWidth(linewidth);
//    return;
//}
//void OutputPersScene::drawLogo(){
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
//    glColor3d(0.4,0.0,0.0);
//    float linewidth;
//    glGetFloatv(GL_LINE_WIDTH , &linewidth);

//    glLineWidth(2);
//    drawText(XYZ(-1.0,-1.2,0.0),QFont("Arial",10),Qt::black,u8"3DVisual ©2017- 日建設計",TEXTPOS::BOTTOMRIGHT1);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_DEPTH_TEST);

//    glLineWidth(linewidth);
//    return;

//}

//void OutputPersScene::drawDispNumber(int fontsize)
//{
//    if( filteredMembers.isEmpty() ) return;

//    //表示されている節点の抽出
//    QList<JointData*> jointlist;
//    for( int i=0; i < filteredMembers.count() ; i++ ){
//        MemberData* md=filteredMembers.at(i);
//        foreach(JointData* jd,md->getJointList()){
//            if(!jointlist.contains(jd))jointlist<<jd;
//        }
//    }
//    //変位の描画
//    QColor color(Qt::blue);
//    QFont font;
//    font.setFamily(QString::fromLocal8Bit("Serif"));
//    font.setPointSize(fontsize);

//    foreach(JointData* jd,jointlist){
//        XYZ jd_disp=getCombinationDisp(jd,dispRatioXY,loadcase,loadcoef,loadstep);
//        QString valuestrx=QString::number((int)(1000*jd_disp.x/dispRatioXY));//m -> mm
//        QString valuestry=QString::number((int)(1000*jd_disp.y/dispRatioXY));
//        QString valuestrz=QString::number((int)(1000*jd_disp.z/dispRatioXY));
//        glPushMatrix();
//        glTranslatef(jd_disp.x+jd->xPos(),jd_disp.y+jd->yPos(),jd_disp.z+jd->zPos());
//        glRotatef(90,1,0,0);
//        glRotatef(45,0,1,0);
//        drawText(XYZ(),font,color,valuestrx,TEXTPOS::BOTTOMRIGHT1);//10 0.2,0.6,1.0  //20 0.4,1.2,2.0
//        drawText(XYZ(),font,color,valuestry,TEXTPOS::BOTTOMRIGHT2);
//        drawText(XYZ(),font,color,valuestrz,TEXTPOS::BOTTOMRIGHT3);
//        glPopMatrix();
//    }
//}

//void OutputPersScene::drawReaction(QList<bool>drawarrow,bool drawvalue,qreal scale,int fontsize)
//{
//    scale=0.002*scale;

//    if( filteredMembers.isEmpty() ) return;

//    //表示されている節点の抽出
//    QList<JointData*> jointlist;
//    for( int i=0; i < filteredMembers.count() ; i++ ){
//        MemberData* md=filteredMembers.at(i);
//        foreach(JointData* jd,md->getJointList()){
//            if(!jointlist.contains(jd))jointlist<<jd;
//        }
//    }
//    //反力の描画
//    foreach(JointData* jd,jointlist){
//        const qreal unitValue=1.0;
//        const int mxdof=6;
//        for(int dof=0;dof<mxdof;dof++){
//            if(drawarrow.at(dof)==true){
//                drawArrow(jd,scale/unitValue,drawvalue,dof,fontsize);
//            }
//        }
//    }
//}
//void OutputPersScene::drawArrow(JointData* jd,qreal scale,bool drawValue,int dof,int fontsize){
//    //矢印の始点終点
//    QColor color;
//    const qreal arrowr=0.3;//コーン半径
//    const qreal arrowl=1.0;//コーン長さ
//    const QVector3D unitZ=QVector3D(0.0,0.0,1.0);
//    QList<qreal> react=jd->getCombinationReaction(loadcase,loadcoef,loadstep);

//    //    if(qAbs(react[dof])<0.1)return;
//    if(react[dof]>=0.0){color=Qt::red;}
//    else{color=Qt::blue;}
//    XYZ end_xyz=jd->xyz();
//    XYZ end_disp=getCombinationDisp(jd,dispRatioXY,loadcase,loadcoef,loadstep);
//    end_xyz.x+=end_disp.x;
//    end_xyz.y+=end_disp.y;
//    end_xyz.z+=end_disp.z;
//    QVector3D epoint=QVector3D(end_xyz.x,end_xyz.y,end_xyz.z);
//    QVector3D spoint=epoint;
//    if(dof==0 || dof==3){
//        spoint.setX(spoint.x()-react[dof]*scale);
//    }else if(dof == 1||dof==4){
//        spoint.setY(spoint.y()-react[dof]*scale);
//    }else if(dof == 2||dof==5){
//        spoint.setZ(spoint.z()-react[dof]*scale);
//    }
////    if(dof==0 && react[dof]>=0.0){
////        spoint.setX(spoint.x()-qMax(2.0*arrowl,react[dof]*scale));
////    }else if(dof==0 && react[dof]<0.0){
////        spoint.setX(spoint.x()-qMin(-2.0*arrowl,react[dof]*scale));
////    }else if(dof==1 && react[dof]>=0.0){
////        spoint.setY(spoint.y()-qMax(2.0*arrowl,react[dof]*scale));
////    }else if(dof==1 && react[dof]<0.0){
////        spoint.setY(spoint.y()-qMin(-2.0*arrowl,react[dof]*scale));
////    }else if(dof==2 && react[dof]>=0.0){
////        spoint.setZ(spoint.z()-qMax(2.0*arrowl,react[dof]*scale));
////    }else if(dof==2 && react[dof]<0.0){
////        spoint.setZ(spoint.z()-qMin(-2.0*arrowl,react[dof]*scale));
////    }
//    QVector3D line=epoint-spoint;

//    //z軸との角度と回転軸
//    qreal rot_ang=acos(QVector3D::dotProduct(unitZ,line.normalized()))*180.0/M_PI;
//    QVector3D rot_axis=(QVector3D::crossProduct(unitZ,line)).normalized();
////    if(qAbs(QVector3D::dotProduct(unitZ,line.normalized())-1.0)<0.001){
////        rot_ang=0.0;
////        rot_axis=QVector3D(0.0,0.0,-1.0);
////    }

//    glPushMatrix();

//    //部材座標系への変換
//    glTranslatef(spoint.x(),spoint.y(),spoint.z());
//    if(qAbs(QVector3D::dotProduct(unitZ,line.normalized())-1.0)<0.00001){//Z軸と一致

//    }else if(qAbs(QVector3D::dotProduct(unitZ,line.normalized())+1.0)<0.00001){//Z軸の負方向と一致
//        glRotatef(180.0,0.0,1.0,0.0);
//    }else{
//        glRotatef(rot_ang,rot_axis.x(),rot_axis.y(),rot_axis.z());
//    }
////    if(react[dof]<0)glRotatef(180,0.0,0.0,1.0);

//    glColor3f(color.redF(),color.greenF(),color.blueF());
//    glLineWidth(2);

//    if( qAbs(react[dof])>0.01){

//        glBegin(GL_LINES);
//        glVertex3d(0.0, 0.0, 0.0 );
//        glVertex3d(0.0, 0.0, line.length() );
//        glEnd();

//        glBegin(GL_TRIANGLE_FAN);
//        glVertex3d(0.0, 0.0, line.length());
//        int div=36;
//        for(int i=0;i<=div;i++){//コーン部分
//            glVertex3d(arrowr*cos(2*M_PI*i/div),arrowr*sin(2*M_PI*i/div),line.length()-arrowl);
//        }
//        glEnd();

//        if(dof>=3){//モーメントの場合、ダブルコーン
//            glBegin(GL_TRIANGLE_FAN);
//            glVertex3d(0.0, 0.0, line.length()-arrowl);
//            int div=36;
//            for(int i=0;i<=div;i++){
//                glVertex3d(arrowr*cos(2*M_PI*i/div),arrowr*sin(2*M_PI*i/div),line.length()-arrowl*2.0);
//            }
//            glEnd();
//        }
//    }

//    if(drawValue && qAbs(react[dof])>0.1){//0.1以下は数字書かない
//        QString str=QString::number((int)react[dof]);
//        QFont font;
//        font.setFamily(QString::fromLocal8Bit("Serif"));
//        font.setPointSize(fontsize);
//        glPushMatrix();
//        glLineWidth(0);
//        if(dof<=2 && line.length() < arrowl){
//            glTranslatef(0,0,line.length()-arrowl);
//        }else if(dof>=3 && line.length() < 2.0*arrowl){
//            glTranslatef(0,0,line.length()-2.0*arrowl);
//        }
////        if(react[dof]<0.0){
////            //            glRotatef(180,0.0,1.0,0.0);
////            //            glRotatef(180,1.0,0.0,0.0);
////            //            glRotatef(180,0.0,0.0,1.0);
////            glRotatef(-2.0*rot_ang,rot_axis.x(),rot_axis.y(),rot_axis.z());
////            glRotatef(180.0,0.0,0.0,1.0);
////            if(dof==1 || dof==4){
////                glRotatef(-135.0,1.0,0.0,0.0);
////            }else{
////                glRotatef(-45.0,1.0,0.0,0.0);
////            }
////        }else{
////            if(dof==1 || dof==4){
////                glRotatef(135.0,1.0,0.0,0.0);
////            }else{
//                glRotatef(45.0,1.0,0.0,0.0);
////            }
////        }
//        drawText(XYZ(0.0,0.0,0.0),font,color,str,TEXTPOS::BOTTOMRIGHT1);
//        glPopMatrix();
//    }
//    glPopMatrix();
//}
////void OutputPersScene::drawText(XYZ pos, QFont font, QColor color,QString str,bool centering){
//////    qDebug()<<pos.x<<pos.y<<pos.z<<str;
////    if(str.isEmpty())return;

////    glPushAttrib(GL_ALL_ATTRIB_BITS);
//////    glMatrixMode(GL_PROJECTION);
////    glPushMatrix();

////    //fontの設定
////    int length=str.length();
////    HFONT Hfont;
////    wchar_t* c_family= new wchar_t[font.family().length()];
////    font.family().toWCharArray(c_family);
////    wchar_t* c_str = new wchar_t[length];
////    str.toWCharArray(c_str);//QString -> wchar_t
////    glColor3f(color.redF(),color.greenF(),color.blueF());

////    int list=glGenLists(length);//連続したディスプレイリストの作成（listはディスプレイリスト識別番号先頭のインデックス）
////    HDC Hdc;//デバイスコンテキストのハンドル
////    Hfont = CreateFontW(
////                font.pointSize(),       //フォント高さ
////                0,                      //文字幅
////                0,                      //テキストの角度
////                0,                      //ベースラインとｘ軸との角度
////                FW_REGULAR,             //フォントの太さ
////                FALSE,                  //イタリック体
////                FALSE,                  //アンダーライン
////                FALSE,                  //打ち消し線
////                SHIFTJIS_CHARSET,       //文字セット
////                OUT_DEFAULT_PRECIS,     //出力精度
////                CLIP_DEFAULT_PRECIS,    //クリッピング精度
////                ANTIALIASED_QUALITY,    //出力品質
////                FIXED_PITCH | FF_MODERN,//ピッチとファミリー
////                c_family);         //書体名

////    Hdc = wglGetCurrentDC();
////    SelectObject(Hdc, Hfont);
////    SIZE sz;
////    GetTextExtentPoint32W(Hdc,c_str,length,&sz);//テキストのサイズを取得

////    for(int i=0;i<length;i++){
////        wglUseFontBitmapsW(Hdc,c_str[i],1,list+(DWORD)i);//文字をビットマップのディスプレイリストで作る。DWORDはデータ処理の単位
////    }

////    glRasterPos3f(pos.x,pos.y,pos.z);//位置
////    if(centering==true)glBitmap(0,0,0,0,-sz.cx/2.0,-sz.cy/2.0+1.0,NULL);//テキストの幅・高さの半分平行移動

////    //ディスプレイリストで描画（文字数分反復）
////    for(int i=0;i<length;i++){
////        glCallList(list+i);
////    }
////    glDeleteLists(list,length);//ディスプレイリストの破棄
////    DeleteObject( Hfont );//フォントの破棄
////    delete[] c_str;
////    delete[] c_family;

////    glPopMatrix();
////    glPopAttrib();
////}
//void OutputPersScene::drawText(XYZ pos, QFont font, QColor color,QString str,TEXTPOS pos2){
//    if(str.isEmpty())return;

//    //boolcenteringではコントロールが弱いので、
//    //左上1,2,3、上1,2,3、右上1,2,3、左、中、右、左下1,2,3、下1,2,3、右下1,2,3
//    //    TEXTPOS pos2=BOTTOMLEFT1;

//    //vec
//    //部材のどの位置に書くか、どの角度で描くかは呼び出し元を書き換える
//    //面内・面外？MyとMzを別の角度に書く？
//    //テキストを用いて処理している表現（節点）などは、適さないので元のものを使う

//    glPushAttrib(GL_ALL_ATTRIB_BITS);
//    glPushMatrix();



//    glColor3f(color.redF(),color.greenF(),color.blueF());
//   // qDebug()<<"drawText";

//    glMatrixMode(GL_MODELVIEW);
//    //    QPainterPath path;
//    QPainterPath path = strokeFont::getInstance()->getStringPath(str);
//    qreal fontSize = font.pointSize()/10.0;
//    //    QPainterPath path = strokeFont::getInstance()->getFontPath(str);
//    glDisable(GL_LIGHTING);

//    GLfloat m[16];
//    glGetFloatv(GL_MODELVIEW_MATRIX, m);
////    qDebug()<<"model";
////    qDebug()<<m[0]<<m[1]<<m[2]<<m[3];
////    qDebug()<<m[4]<<m[5]<<m[6]<<m[7];
////    qDebug()<<m[8]<<m[9]<<m[10]<<m[11];
////    qDebug()<<m[12]<<m[13]<<m[14]<<m[15];
//    //画面座標系（右がX軸、上がY軸、手前がZ軸正方向として、
//    //文字列のX方向ベクトルの、画面座標系表示：0,4,8
//    //文字列のY方向ベクトルの、画面座標系表示：1,5,9
//    //文字列のZ方向ベクトルの、画面座標系表示：2,6,10


//        //    path.addText(QPointF(0,0 ), font, str);
//    qreal ww=fontSize*path.boundingRect().width()*0.1f;
//    qreal hh=fontSize*path.boundingRect().height()*0.1f;
//    //原点
//    QPointF modp = TextPosFunctions::getModifiedTextPos(pos.x,pos.y,ww,hh,pos2);
//    if(m[10]<0){//文字が裏向き
//        //原点に移動
//        glTranslatef(modp.x(),modp.x(),0.0);
//        //y軸回りに回転
//        glRotatef(180,0,1,0);
//        //原点を戻す
//        glTranslatef(-modp.x(),-modp.x(),0.0);
//        //文字幅だけシフトする
//        glTranslatef(-ww,0.0,0.0);
//    }


//    QList<QPolygonF> poly = path.toSubpathPolygons();

//    for(QList<QPolygonF>::iterator i= poly.begin();i!=poly.end();i++){
//        glBegin(GL_LINE_STRIP);
//        for(QPolygonF::iterator p=(*i).begin();p!=i->end();p++){
//            glVertex3f(fontSize*p->rx()*0.1f+modp.x(), hh+0.1-fontSize*p->ry()*0.1f+modp.y(),pos.z+0.01);
//        }
//        glEnd();

//    }
//    glEnable(GL_LIGHTING);
//    glMatrixMode(GL_MODELVIEW);

//    glPopMatrix();
//    glPopAttrib();
//}
}
