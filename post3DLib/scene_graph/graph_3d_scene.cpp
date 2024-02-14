#include "graph_3d_scene.h"

#include <QDebug>
#include <QVector3D>
#include "qmath.h"

#include "fixed_data.h"




using namespace std;
using namespace qglviewer;
namespace post3dapp{


void Graph3dScene::drawSolid()
{
    qDebug()<<"Graph3dScene::drawSolid()";
//    if( allLines.isEmpty() ) return;

    for( int i=0; i < allLines.count() ; i++ ){
        Graph3DLine md=allLines.at(i);
        Graph3DPoint ijd=md.getIPoint();
        XYZ i_xyz=ijd.xyz();
        Graph3DPoint jjd=md.getJPoint();
        XYZ j_xyz=jjd.xyz();


        LineParameter sc=md.getLineParameter();

            glPushMatrix();//モデルの全体座標系をいったん保存

            //部材座標系に変換
            setLocalMatrix(i_xyz.x, i_xyz.y, i_xyz.z, j_xyz.x, j_xyz.y, j_xyz.z);

            //部材座標系で描画
            Vec a1(xScale*(j_xyz.x-i_xyz.x),yScale*(j_xyz.y-i_xyz.y),zScale*(j_xyz.z-i_xyz.z));
            createLineElement(sc.lineColor,sc.paintColor,sc.lineWidth,sc.lineStyle,0.0,a1.norm());

            glPopMatrix();//全体座標系に復帰
    }
    for( int i=0; i < allPlanes.count() ; i++ ){
        Graph3DPlane md=allPlanes.at(i);
        PlaneParameter sc=md.getPlaneParameter();
        //三角形に分割
        md.divideIntoTriangles();

        //面の外周をベクトルのリストで表現
        QList<Vec> vlist;
        Q_FOREACH( Graph3DPoint jd, md.getPoints() ){
            XYZ c_xyz=jd.xyz();
            vlist.append( Vec(c_xyz.x,c_xyz.y,c_xyz.z) );
        }
        createPlaneElement(sc.lineColor,sc.paintColor,
                           vlist,md.getTriangles());
    }
    for( int i=0; i < allPoints.count() ; i++ ){
//        if(isPlotPoint){}
        if(allPoints.at(i).parameter().type==PointParameter::PLOTWITHLINE){
            createPlotWithLineElement(allPoints.at(i).x(),allPoints.at(i).y(),allPoints.at(i).z(),
                        allPoints.at(i).parameter().name,Qt::red,15);
        }

    }





}


void Graph3dScene::setLocalMatrix(qreal x1,  qreal y1,  qreal z1,
                               qreal x2,  qreal y2,  qreal z2)
{
    //全体座標->部材座標。beta角削ったせいで冗長な表現
    qreal xx=xScale*(x2-x1);
    qreal yy=yScale*(y2-y1);
    qreal zz=zScale*(z2-z1);

    Vec a1(1.0,0.0,0.0);
    Vec a2(xx,yy,0.0);
    Vec a3(xx,yy,zz);
    a3.normalize();

    qreal cos1,cos2;
    GLdouble theta1,theta2;
    Vec v1,v2;

    if( a2.norm() > 0.001 ){//部材が鉛直でない場合
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
    }else{//部材が鉛直の場合
        theta1=0.0;
        v1=Vec(0.0,0.0,0.0);
        theta2=90.0;
        v2=Vec(0.0,-1.0,0.0);
    }

    glTranslatef(xScale*x1,yScale*y1,zScale*z1);
    if( v2.norm() > 0.001 ) glRotatef(theta2,v2.x,v2.y,v2.z);
    if( v1.norm() > 0.001 ) glRotatef(theta1,v1.x,v1.y,v1.z);

}
void Graph3dScene::createPlotWithLineElement(qreal _x,qreal _y,qreal _z,
                                             const QString& str,const QColor& color,int size)
{
        qreal x = _x*xScale;
        qreal y = _y*yScale;
        qreal z = _z*zScale;
//        int size = 15;
        QColor col1 = Qt::darkRed;
        float linewidth;
        glGetFloatv(GL_LINE_WIDTH , &linewidth);//現在の線の太さを保持
        glLineWidth(1.0);
        glColor4d(col1.redF(), col1.greenF(), col1.blueF(), col1.alphaF());

        glBegin(GL_QUADS);
        glVertex3d(x-size, y-size, z+size );
        glVertex3d(x-size, y-size, z-size );
        glVertex3d(x-size, y+size, z-size );
        glVertex3d(x-size, y+size, z+size );

        glVertex3d(x+size, y-size, z+size );
        glVertex3d(x+size, y-size, z-size );
        glVertex3d(x+size, y+size, z-size );
        glVertex3d(x+size, y+size, z+size );

        glVertex3d(x-size, y-size, z+size );
        glVertex3d(x-size, y-size, z-size );
        glVertex3d(x+size, y-size, z-size );
        glVertex3d(x+size, y-size, z+size );

        glVertex3d(x-size, y+size, z+size );
        glVertex3d(x-size, y+size, z-size );
        glVertex3d(x+size, y+size, z-size );
        glVertex3d(x+size, y+size, z+size );

        glVertex3d(x-size, y+size, z-size );
        glVertex3d(x-size, y-size, z-size );
        glVertex3d(x+size, y-size, z-size );
        glVertex3d(x+size, y+size, z-size );

        glVertex3d(x-size, y+size, z+size );
        glVertex3d(x-size, y-size, z+size );
        glVertex3d(x+size, y-size, z+size );
        glVertex3d(x+size, y+size, z+size );
        glEnd();

//        glBegin(GL_LINES);
//        glVertex3d(x, y, z );
//        glVertex3d(x+300, y+300, z+300 );
//        glVertex3d(x+300, y+300, z+300 );
//        glVertex3d(x+600, y+600, z+300 );
//        glEnd();

//        drawText(str,XYZ((x+600)/xScale,(y+600)/yScale,(z+300)/zScale));
        drawText(str,XYZ((_x+10),(_y+10),(_z+10)));
        glLineWidth(linewidth);//線幅を戻す

}
void Graph3dScene::createLineElement(const QColor& col1, const QColor& col2,int weight,int style,
                                 qreal l1, qreal l2)
{
    Q_UNUSED(col2)
    if( l2-l1 < 1.0e-3 ) return;


    float linewidth;
    glGetFloatv(GL_LINE_WIDTH , &linewidth);//現在の線の太さを保持
    glLineWidth(weight);
    glColor4d(col1.redF(), col1.greenF(), col1.blueF(), col1.alphaF());

    if(style == 2){
        glEnable(GL_LINE_STIPPLE);// GLenum cap
        glLineStipple(
            8,      // GLint factor
            0x5555);// GLushort pattern
    }
    glBegin(GL_LINES);
    glVertex3d(l1, 0.0, 0.0 );
    glVertex3d(l2, 0.0, 0.0 );
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glLineWidth(linewidth);//線幅を戻す


    return;

}


void Graph3dScene::createPlaneElement(const QColor& col1, const QColor& col2,
                                   const QList<Vec> &plist, const QList<Vec> &tplist )
//plist...外周節点のリスト
//tplist...三角形に分割されたリスト
{
//    qDebug()<<"外周線";
    glColor4d(col1.redF(), col1.greenF(), col1.blueF(), col1.alphaF());

    glBegin(GL_LINE_LOOP);
    for( int i=0; i<plist.count(); i++ ){
        Vec vv=plist.at(i);
        glVertex3d( xScale*vv.x,yScale*vv.y, zScale*vv.z );
    }
    glEnd();

//    qDebug()<<"分割された三角形";
    glDisable(GL_DEPTH_TEST);//半透明のものを描くときは深度テストを切る（＝奥のものも描く）

    glColor4d(col2.redF(), col2.greenF(), col2.blueF(), 0.3);
    glBegin(GL_TRIANGLES);
    Q_FOREACH( Vec vv, tplist ) glVertex3d( xScale*vv.x, yScale*vv.y, zScale*vv.z );
    glEnd();
    glEnable(GL_DEPTH_TEST);

    return;
}


void Graph3DPlane::divideIntoTriangles()
{
    this->planeElementTriangles.clear();
    QList<Vec> tmpPoints;
    Q_FOREACH( Graph3DPoint jd, this->getPoints() ){

        XYZ c_org=jd.xyz();
        tmpPoints.append( Vec(c_org.x,c_org.y,c_org.z) );

    }
    QList<Vec> dividedPoints;

    int cur_idx = 0,pre_idx,next_idx;
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

        while( true ){

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

    this->planeElementTriangles = dividedPoints;
}


void Graph3dScene::drawAxis(){
    //最大最小にしたがっていい感じにやる。グリッド空間と、交線が欲しい？makeClippingPlaneとか。
    //値と座標は別。
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3d(0.2,0.2,0.2);
    float linewidth;
    glGetFloatv(GL_LINE_WIDTH , &linewidth);

    glLineWidth(1);

    glBegin(GL_LINES);
    glVertex3d(xAxisValues.first()*xScale,0.0,0.0);
    glVertex3d(xAxisValues.last()*xScale,0.0,0.0);
    glVertex3d(xAxisValues.last()*xScale,0.0,0.0);
    glVertex3d(xAxisValues.last()*xScale-50,50.0,0.0);
    glVertex3d(xAxisValues.last()*xScale,0.0,0.0);
    glVertex3d(xAxisValues.last()*xScale-50,-50.0,0.0);

    glVertex3d(0.0,yAxisValues.first()*yScale,0.0);
    glVertex3d(0.0,yAxisValues.last()*yScale,0.0);
    glVertex3d(0.0,yAxisValues.last()*yScale,0.0);
    glVertex3d(50.0,yAxisValues.last()*yScale-50,0.0);
    glVertex3d(0.0,yAxisValues.last()*yScale,0.0);
    glVertex3d(-50.0,yAxisValues.last()*yScale-50,0.0);

    glVertex3d(0.0,0.0,zAxisValues.first()*zScale);
    glVertex3d(0.0,0.0,zAxisValues.last()*zScale);
    glVertex3d(0.0,0.0,zAxisValues.last()*zScale);
    glVertex3d(50.0,0.0,zAxisValues.last()*zScale-50);
    glVertex3d(0.0,0.0,zAxisValues.last()*zScale);
    glVertex3d(-50.0,0.0,zAxisValues.last()*zScale-50);

    for(int i=0;i<xAxisValues.count()-1;i++){
        qreal val = xAxisValues.at(i);
        glVertex3d(val*xScale,30.0,0.0);
        glVertex3d(val*xScale,-30.0,0.0);
    }
    for(int i=0;i<yAxisValues.count()-1;i++){
        qreal val = yAxisValues.at(i);
        glVertex3d(30.0,val*yScale,0.0);
        glVertex3d(-30.0,val*yScale,0.0);
    }
    for(int i=0;i<zAxisValues.count()-1;i++){
        qreal val = zAxisValues.at(i);
        glVertex3d(30,0,val*zScale);
        glVertex3d(-30,0,val*zScale);
    }

    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glLineWidth(linewidth);

    //軸の数字
    int xmid,ymid,zmid;
    if(xAxisValues.count()%2 == 1){
        xmid = (xAxisValues.count()-1)/2;
    }else{
        xmid = xAxisValues.count()/2;
    }
    if(yAxisValues.count()%2 == 1){
        ymid = (yAxisValues.count()-1)/2;
    }else{
        ymid = yAxisValues.count()/2;
    }
    if(zAxisValues.count()%2 == 1){
        zmid = (zAxisValues.count()-1)/2;
    }else{
        zmid = zAxisValues.count()/2;
    }
        for(int i=5;i<xmid;i+=5){
            qreal val1 = xAxisValues.at(xmid + i);
            drawText(QString::number(val1),XYZ(val1,0.0,0.0));
            qreal val2 = xAxisValues.at(xmid - i);
            drawText(QString::number(val2),XYZ(val2,0.0,0.0));
        }
        for(int i=5;i<ymid;i+=5){
            qreal val1 = yAxisValues.at(ymid + i);
            drawText(QString::number(val1),XYZ(0.0,val1,0.0));
            qreal val2 = yAxisValues.at(ymid - i);
            drawText(QString::number(val2),XYZ(0.0,val2,0.0));
        }
        for(int i=5;i<(zAxisValues.count()-1)/2;i+=5){
            qreal val1 = zAxisValues.at(zmid + i);
            drawText(QString::number(val1),XYZ(0.0,0.0,val1));
            qreal val2 = zAxisValues.at(zmid - i);
            drawText(QString::number(val2),XYZ(0.0,0.0,val2));
        }

    //軸ラベル
    drawText(jikuStrX,XYZ(xAxisValues.last()+50.0,0.0,0.0));
    drawText(jikuStrY,XYZ(0.0,yAxisValues.last()+50.0,0.0));
    drawText(jikuStrZ,XYZ(0.0,0.0,zAxisValues.last()+50.0));

    return;
}

void Graph3dScene::drawXZText(const QString &str, qreal swidth, qreal shx, qreal shy)
{
    QList<RXYLINE> linelist = ascii_table.stringToXY(str, swidth, swidth * 1.25, shx, shy);

    glBegin(GL_LINES);
    Q_FOREACH (RXYLINE line, linelist) {
        glVertex3d(line.x1,   0.0, line.y1 );
        glVertex3d(line.x2,   0.0, line.y2 );
    }
    glEnd();
}
void Graph3dScene::drawText( const QString& str,const XYZ pos)//,const qglviewer::Vec &v_camera_right, const qglviewer::Vec &v_camera_up )
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    QColor color = Qt::black;
    glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
//    int isize = 1;
    qreal swidth = 50;//100.0 + qreal(isize) * 30.0;

    // カメラベクトルと正対するような座標変換
    GLdouble theta1, theta2;
    qglviewer::Vec v1, v2;
    qglviewer::Vec a1(1.0, 0.0, 0.0);
    qglviewer::Quaternion q1(a1, camera_right);
    theta1 = q1.angle() * 180.0 / M_PI;
    v1 = q1.axis();
    qreal v1_norm = v1.norm();
    qglviewer::Vec a2(0.0, 0.0, 1.0);
    qglviewer::Vec a3 = q1.rotate(a2);
    qglviewer::Quaternion q2(a3, camera_up);
    theta2 = q2.angle() * 180. / M_PI;
    v2 = q2.axis();
    qreal v2_norm = v2.norm();


        qreal shiftX = 0.0;
        qreal shiftY = 0.0;

        shiftX = -swidth * str.count() / 2.0;

        glPushMatrix();
        glTranslatef(pos.x * xScale, pos.y * yScale, pos.z * zScale);

        if ( v2_norm > 0.001 ) glRotatef(theta2, v2.x, v2.y, v2.z);
        if ( v1_norm > 0.001 ) glRotatef(theta1, v1.x, v1.y, v1.z);

        drawXZText(str, swidth, shiftX, shiftY);

        glPopMatrix();


    glEnable(GL_LIGHTING);
}
}
