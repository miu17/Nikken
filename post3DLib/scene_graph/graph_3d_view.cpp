#include "graph_3d_view.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QMouseEvent>


using namespace qglviewer;
using namespace std;
namespace post3dapp{
Graph3dView::Graph3dView( Graph3dScene* s, QWidget* parent, const QGLWidget* shareWidget)
    : QGLViewer(parent, shareWidget), myScene(s)
{
    camera()->setUpVector(Vec(-1.,1.,1.));
    camera()->setViewDirection(Vec(-1.,1.,-1.));
    camera()->fitBoundingBox(Vec(-1000.0,-1000.0,-5000.0),Vec(1000.0,1000.0,5000.0));
    camera()->setType(Camera::ORTHOGRAPHIC);

    qDebug()<<"myGraph3dView created";
}

//void Graph3dView::loadViewSettings( const MODELSETTINGS &settings )
//{
//    myViewSettings=settings;
//    setBackgroundColor( myViewSettings.backColor );
//    myScene->setFileNo(myViewSettings.fileNo);

//    qDebug()<<"setting done";
//}

void Graph3dView::init()
{

    setBackgroundColor( Qt::white );

    //  アルファブレンドの係数を設定する。半透明のものを重ねたときの混色の方法
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);//物が重なったとき、奥のものを描写しない
    glShadeModel(GL_FLAT);//フラットな物体

    glClearStencil(0);//開口などの「物を描かない範囲」のリセット
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//バッファのクリア
    glDisable(GL_POINT_SMOOTH);//アンチエイリアスなし
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);//アンチエイリアスありの場合、品質
    glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
//    if(myViewSettings.renderType==0){
        camera()->setType(Camera::ORTHOGRAPHIC);
        camera()->setZNearCoefficient(-10000.);//意味なし
        camera()->setZClippingCoefficient(10000.);
//    }
//    if(myViewSettings.renderType==1){
//        camera()->setType(Camera::PERSPECTIVE);
//        camera()->setZNearCoefficient(0.001);//default=0.005
//        camera()->setZClippingCoefficient(100.);//default=root3
//    }
}

void Graph3dView::draw()
{

    qDebug()<<"Graph3dView::draw()";
    qreal pw=1;//qreal(myViewSettings.lineWidth);
    glLineWidth( pw );

    GLfloat refm[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, refm);

    glEnable(GL_BLEND);

    myScene->setCameraVec(camera()->rightVector(),camera()->upVector());
    myScene->drawSolid();
    myScene->drawAxis();
 //   myScene->drawText(camera()->rightVector(),camera()->upVector());
    glDisable(GL_BLEND);
}

//描画範囲のフィッティング
void Graph3dView::fitBoundingBox(XYZ min,XYZ max)
{
    camera()->fitBoundingBox(Vec(min.x,min.y,min.z),Vec(max.x,max.y,max.z));
}

//画面のコピー
void Graph3dView::keyPressEvent(QKeyEvent* event){

    QGLViewer::keyPressEvent(event);
    if(event->type() == QKeyEvent::KeyPress) {
        if(event->matches(QKeySequence::Copy)) {

            QImage image=this->grabFramebuffer();
            QClipboard * clipboad = QApplication::clipboard();
            clipboad->setImage( image );
            //            clipboad->setPixmap( pixmap );
        }
    }
}

void Graph3dView::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter;
    painter.begin(this);

    glDisable(GL_LIGHTING);
    glPushAttrib(GL_ALL_ATTRIB_BITS);//属性をスタックにプッシュ
//    glMatrixMode(GL_PROJECTION);//投影変換モード
//    glPushMatrix();//投影マトリックスをスタックにプッシュ

    glMatrixMode(GL_MODELVIEW);//モデルビューモード
    glPushMatrix();//投影マトリックスをスタックにプッシュ


    init();
    preDraw();
    draw();
    postDraw();
//    glMatrixMode(GL_MODELVIEW);
//    glPopMatrix();//投影マトリクスの復元
//    glMatrixMode(GL_PROJECTION);
    glPopMatrix();//投影マトリクスの復元
    glPopAttrib();//属性の復元

    painter.end();

}
QString Graph3dView::moldDoubleToText(qreal value,int length,int precision){
    QString moldedValue;
    QString str=QString::number(value,'f',precision);
    bool isMinus=str.startsWith("-");
    if(isMinus){
        moldedValue="-";
        str.remove("-");
    }else{
        moldedValue="+";
    }
    if(str.length()>length){
        QString estr=QString::number(value,'e',qMax(1,length-4-2));
        if(value>=0.0)estr="+"+estr;
        return estr;
    }
    str=str.rightJustified(length,' ',false);
    moldedValue=moldedValue+str;
    return moldedValue;
}
}
