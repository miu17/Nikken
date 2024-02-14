#include <QMouseEvent>
#include <QMessageBox>
#include <QCoreApplication>
#include <QDebug>

#include "analysisgroup_glview.h"
#include "unified_settings.h"

using namespace qglviewer;
using namespace std;

namespace post3dapp{

AnalysisGroupGLViewer::AnalysisGroupGLViewer( AnalysisGroupGLScene* s,
                                                  QWidget* parent )
  : QGLViewer(parent), myScene(s)
{
    camera()->setViewDirection(Vec(0.0,1.0,0.0));
    camera()->setUpVector(Vec(0.0,0.0,1.0));
    camera()->setType(Camera::ORTHOGRAPHIC);

    camera()->fitBoundingBox(Vec(-5000.0f,-5000.0f,-5000.0f),
                             Vec(15000.0f,15000.0f,15000.0f));

    GLdouble ff=-1000000.0;
    camera()->setZNearCoefficient(ff);
    camera()->setZClippingCoefficient(1000000.);

    myViewSettings=PERSANALYSISSETTINGS();
    myScene->initDisplayList();
}

void AnalysisGroupGLViewer::fitWindow()
{
    Vec v1=myScene->boundingMinVec();
    Vec v2=myScene->boundingMaxVec();
    camera()->fitBoundingBox(v1,v2);
    update();
}

void AnalysisGroupGLViewer::changeViewSettings(const PERSANALYSISSETTINGS &settings)
{
    myViewSettings=settings;
//    setSceneCenter( Vec(myViewSettings.centerX,myViewSettings.centerY,myViewSettings.centerZ) );
    setBackgroundColor( myViewSettings.backColor );
    update();
}

void AnalysisGroupGLViewer::setSelectedJointItems(const QList<JointData *> &joints)
{
    myScene->setSelectedItems(joints, QList<MemberData*>{});
    update();
}

void AnalysisGroupGLViewer::setSelectedMemberItems(const QList<MemberData *> &members)
{
    myScene->setSelectedItems(QList<JointData*>{}, members);
    update();
}

void AnalysisGroupGLViewer::init()
{
    setBackgroundColor( myViewSettings.backColor );

    glDisable(GL_LIGHT0);
    float lightAmbient0[]={1.0f,1.0f,1.0f,1.0f};	//環境光反射
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient0);	//環境光反射設定

    glEnable(GL_LIGHT1);

    float lightPos1[]={  1.0f, -1.0f, 1.0f, 0.0f};	//光源
    float lightAmbient[]={0.2f,0.2f,0.2f,1.0f};	    //環境光反射
    float lightDiffuse[]={0.8f,0.8f,0.8f,0.8f};	    //拡散反射

    glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);	    //光源
    glLightfv(GL_LIGHT1,GL_AMBIENT,lightAmbient);	//環境光反射設定
    glLightfv(GL_LIGHT1,GL_DIFFUSE,lightDiffuse);	//拡散反射設定
/*
    glDisable(GL_LIGHT2);

    float lightPos2[]={  -1.0f, 1.0f, 1.0f, 0.0f};	//光源

    glLightfv(GL_LIGHT2,GL_POSITION,lightPos2);	    //光源
    glLightfv(GL_LIGHT2,GL_AMBIENT,lightAmbient);	//環境光反射設定
    glLightfv(GL_LIGHT2,GL_DIFFUSE,lightDiffuse);	//拡散反射設定
*/
    glEnable(GL_LIGHT3);

    float lightPos3[]={  -1.0f, -1.0f, 1.0f, 0.0f};	//光源

    glLightfv(GL_LIGHT3,GL_POSITION,lightPos3);	    //光源
    glLightfv(GL_LIGHT3,GL_AMBIENT,lightAmbient);	//環境光反射設定
    glLightfv(GL_LIGHT3,GL_DIFFUSE,lightDiffuse);	//拡散反射設定

    glEnable(GL_CLIP_PLANE0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setMouseBinding(Qt::NoModifier, Qt::LeftButton, SELECT);
    setMouseBinding( Qt::ControlModifier, Qt::LeftButton, SELECT );
    setMouseBinding(Qt::ShiftModifier, Qt::LeftButton, SELECT);

    setMouseBinding( Qt::NoModifier, Qt::MiddleButton, CAMERA, TRANSLATE ,false );
    setMouseBinding( Qt::ShiftModifier, Qt::MiddleButton, CAMERA, ROTATE);
    setMouseBinding(Qt::ControlModifier, Qt::MiddleButton, CAMERA, ZOOM, false);

    setShortcut(DRAW_AXIS, 0);
    setShortcut(DRAW_GRID, 0);
    setShortcut(DISPLAY_FPS, 0);
    setShortcut(ENABLE_TEXT, 0);
    setShortcut(STEREO, 0);
    setShortcut(HELP, 0);
    setShortcut(EXIT_VIEWER, 0);
    setShortcut(STEREO, 0);
}

void AnalysisGroupGLViewer::draw()
{
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT3);

    glLineWidth(1.0);

    glColor3f(0.0f, 0.0f, 1.0f);
    drawAxis( myViewSettings.axisLength );

    GLfloat refm[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, refm);

    glEnable(GL_BLEND);
    myScene->drawSolid(false);
    glDisable(GL_BLEND);

    glEnable(GL_LIGHT0);

    if( myViewSettings.isTextOn ){
        myScene->drawMemberText( camera()->rightVector(), camera()->upVector() );
        myScene->drawJointText( camera()->rightVector(), camera()->upVector() );
    }
    if( myViewSettings.isJointOn ) myScene->drawPoints();
    myScene->drawFrameScale( camera()->rightVector(), camera()->upVector() );
    myScene->drawFloorScale( camera()->rightVector(), camera()->upVector() );

    if( selectMode != PERSSELECTMODE::PERS_NOSELECT ) drawSelectionRectangle();

    glDisable(GL_LIGHT0);
}

void AnalysisGroupGLViewer::fastDraw()
{
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT3);

    glColor3f(0.0f, 0.0f, 1.0f);

    if( myViewSettings.isAxisOn ){
        drawAxis( myViewSettings.axisLength );
    }

    glLineWidth(1.0);

    GLfloat refm[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glShadeModel(GL_FLAT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, refm);

    glEnable(GL_BLEND);
    myScene->drawSolid(false);
    glDisable(GL_BLEND);
}

void AnalysisGroupGLViewer::drawWithNames()
{
//   if( isRenderMode ) return;
    myScene->drawSolid(true);
}

void AnalysisGroupGLViewer::postSelection( const QPoint& )
{
/*
    GLuint id=selectedName();
    if( id!=-1 ){
        myScene->appendSelectedMemberID(id);
        updateGL();
//        myScene->sendSelectedItems();
    }
*/
}

void AnalysisGroupGLViewer::mousePressEvent(QMouseEvent* e)
{
     selectRect = QRect(e->pos(), e->pos());

     selectMode=PERSSELECTMODE::PERS_NOSELECT;
     if (e->button() == Qt::LeftButton){
         if (e->modifiers() == Qt::ControlModifier){
             selectMode = PERSSELECTMODE::PERS_ADDSELECT;
         }else if (e->modifiers() == Qt::ShiftModifier){
             selectMode = PERSSELECTMODE::PERS_REMOVESELECT;
         }else if (e->modifiers() == Qt::NoModifier){
             myScene->clearSelectedItems();
             update();
             selectMode = PERSSELECTMODE::PERS_ADDSELECT;
         }
     }

     // TODO:回転中心を本当は原点中心から画面中心にしたい。下のコードはモデル中心。それも違う
     auto center = (myScene->boundingMinVec() + myScene->boundingMaxVec()) / 2.0;
     setSceneCenter(center);

     QGLViewer::mousePressEvent(e);

     if( e->button() == Qt::RightButton && (e->modifiers() == Qt::NoModifier) ){
//         qDebug() << myScene->selectedElementTypes();
//         manageActions->showContextMenu( myScene->selectedElementTypes() );
     }
}

void AnalysisGroupGLViewer::mouseMoveEvent(QMouseEvent* e)
{
  if( selectMode != PERSSELECTMODE::PERS_NOSELECT ){
      selectRect.setBottomRight(e->pos());
      QGLViewer::mouseMoveEvent(e);
      update();
  } else{
      QGLViewer::mouseMoveEvent(e);
  }
}

void AnalysisGroupGLViewer::mouseReleaseEvent(QMouseEvent* e)
{
    if( selectMode!=PERSSELECTMODE::PERS_NOSELECT ){
        myScene->selectPoints(camera(),selectRect,selectMode);
        update();
    }
    selectMode=PERSSELECTMODE::PERS_NOSELECT;

    QGLViewer::mouseReleaseEvent(e);
}

void AnalysisGroupGLViewer::keyPressEvent(QKeyEvent *e)
{
//    const Qt::KeyboardModifiers modifiers = e->modifiers();
    switch( e->key() ){
    case Qt::Key_Escape:
        slotClearSelectedItems();
        break;
    default :
        break;
    }
}

void AnalysisGroupGLViewer::drawSelectionRectangle() const
{
    startScreenCoordinatesSystem();

    glEnable(GL_LINE_STIPPLE);

    glLineStipple(1,0x0F0F);
    glLineWidth(1.0);
    glColor3f(0.3f, 0.3f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2i(selectRect.left(),  selectRect.top());
    glVertex2i(selectRect.right(), selectRect.top());
    glVertex2i(selectRect.right(), selectRect.bottom());
    glVertex2i(selectRect.left(),  selectRect.bottom());
    glEnd();

    glDisable(GL_LINE_STIPPLE);

    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);

    glColor4f(0.0, 0.0, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2i(selectRect.left(),  selectRect.top());
    glVertex2i(selectRect.right(), selectRect.top());
    glVertex2i(selectRect.right(), selectRect.bottom());
    glVertex2i(selectRect.left(),  selectRect.bottom());
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

    stopScreenCoordinatesSystem();
}

void AnalysisGroupGLViewer::zoomIn()
{
    QWheelEvent ev(QPoint(),-8,Qt::NoButton,Qt::NoModifier);
    QCoreApplication::sendEvent(this, &ev);
}

void AnalysisGroupGLViewer::zoomOut()
{
    QWheelEvent ev(QPoint(),8,Qt::NoButton,Qt::NoModifier);
    QCoreApplication::sendEvent(this, &ev);
}

void AnalysisGroupGLViewer::slotClearSelectedItems()
{
    myScene->clearSelectedItems();
    update();
}

void AnalysisGroupGLViewer::slotChangeLimitedFloor(const QString &str)
{
    myScene->setLimitedFloor(str);
    update();
}

void AnalysisGroupGLViewer::slotChangeLimitedFrame(const QString &str)
{
    myScene->setLimitedFrame(str);
    update();
}
} // namespace post3dapp
