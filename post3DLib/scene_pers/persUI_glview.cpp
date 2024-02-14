#include "persUI_glview.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMouseEvent>

#include "persUI_scene.h"
#include "unified_settings.h"


namespace post3dapp{
PersInterfaceGLViewer::PersInterfaceGLViewer( PersInterfaceScene *s, QWidget *parent,
                                              const QGLWidget *shareWidget)
    : QGLViewer(parent, shareWidget), myScene(s)
{
    camera()->setViewDirection(qglviewer::Vec(0.0, 1.0, 0.0));
    camera()->setUpVector(qglviewer::Vec(0.0, 0.0, 1.0));
    camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);

    camera()->fitBoundingBox( qglviewer::Vec(-5000.0f, -5000.0f, -5000.0f),
                              qglviewer::Vec(15000.0f, 15000.0f, 15000.0f) );

    GLdouble ff = -1000000.0;
    camera()->setZNearCoefficient(ff);
    camera()->setZClippingCoefficient(1000000.);

    myViewSettings = PERSSETTINGVALUES();
    isRenderMode = false;
    drawingStyle = DRAWINGSTYLE::SOLIDWIRE;
    connect(myScene, &PersInterfaceScene::dataUpdated, this, QOverload<void>::of(&PersInterfaceGLViewer::update));
    connect(myScene, &PersInterfaceScene::selectedItemChanged, this, &PersInterfaceGLViewer::signalSendSelectedItems);
}

void PersInterfaceGLViewer::fitWindow()
{
    qglviewer::Vec v1 = myScene->boundingMinVec();
    qglviewer::Vec v2 = myScene->boundingMaxVec();
    camera()->fitBoundingBox(v1, v2);
    update();
}

void PersInterfaceGLViewer::resetViewDir(int index)
{

    VIEWDIR dir = static_cast<VIEWDIR>(index);
    switch ( dir ) {
    case VIEWDIR::Y_PLUS:
        camera()->setViewDirection(qglviewer::Vec(0.0, 1.0, 0.0));
        camera()->setUpVector(qglviewer::Vec(0.0, 0.0, 1.0));
        break;
    case VIEWDIR::Y_MINUS:
        camera()->setViewDirection(qglviewer::Vec(0.0, -1.0, 0.0));
        camera()->setUpVector(qglviewer::Vec(0.0, 0.0, 1.0));
        break;
    case VIEWDIR::X_PLUS:
        camera()->setViewDirection(qglviewer::Vec(1.0, 0.0, 0.0));
        camera()->setUpVector(qglviewer::Vec(0.0, 0.0, 1.0));
        break;
    case VIEWDIR::X_MINUS:
        camera()->setViewDirection(qglviewer::Vec(-1.0, 0.0, 0.0));
        camera()->setUpVector(qglviewer::Vec(0.0, 0.0, 1.0));
        break;
    case VIEWDIR::Z_PLUS:
        camera()->setViewDirection(qglviewer::Vec(0.0, 0.0, 1.0));
        camera()->setUpVector(qglviewer::Vec(0.0, 1.0, 0.0));
        break;
    case VIEWDIR::Z_MINUS:
        camera()->setViewDirection(qglviewer::Vec(0.0, 0.0, -1.0));
        camera()->setUpVector(qglviewer::Vec(0.0, 1.0, 0.0));
        break;
    default:
        break;
    }
    fitWindow();
}

void PersInterfaceGLViewer::changeViewSettings(const PERSSETTINGVALUES &settings)
{
    myViewSettings = settings;
    drawingStyle = static_cast<DRAWINGSTYLE>(myViewSettings.drawingStyle);
    setSceneCenter( qglviewer::Vec(myViewSettings.centerX, myViewSettings.centerY, myViewSettings.centerZ) );
    setBackgroundColor( myViewSettings.backColor );
    update();
}

void PersInterfaceGLViewer::slotChangeDrawingStyle(int style)
{
    drawingStyle = static_cast<DRAWINGSTYLE>(style);
    update();
}

void PersInterfaceGLViewer::init()
{
    setBackgroundColor( myViewSettings.backColor );

    glDisable(GL_LIGHT0);
    float lightAmbient0[] = {1.0f, 1.0f, 1.0f, 1.0f}; //環境光反射
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0); //環境光反射設定

    glEnable(GL_LIGHT1);

    float lightPos1[] = {  1.0f, -1.0f, 1.0f, 0.0f}; //光源
    float lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f}; //環境光反射
    float lightDiffuse[] = {0.8f, 0.8f, 0.8f, 0.8f}; //拡散反射

    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);   //光源
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient); //環境光反射設定
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse); //拡散反射設定

    glDisable(GL_LIGHT2);

    float lightPos2[] = { -1.0f, 1.0f, 1.0f, 0.0f}; //光源

    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);   //光源
    glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient); //環境光反射設定
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse); //拡散反射設定

    glDisable(GL_LIGHT3);

    float lightPos3[] = { -1.0f, -1.0f, 1.0f, 0.0f}; //光源

    glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);   //光源
    glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmbient); //環境光反射設定
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDiffuse); //拡散反射設定

//    setManipulatedFrame(new ManipulatedFrame());
    glEnable(GL_CLIP_PLANE0);

//    glBlendFunc(GL_ONE, GL_ONE);

    //  アルファブレンドを有効化する
//    glEnable(GL_BLEND);
    //  アルファブレンドの係数を設定する
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glShadeModel(GL_FLAT);

    setMouseBinding(Qt::NoModifier, Qt::LeftButton, SELECT);
    setMouseBinding( Qt::ControlModifier, Qt::LeftButton, SELECT );
    setMouseBinding(Qt::ShiftModifier, Qt::LeftButton, SELECT);

    setMouseBinding( Qt::NoModifier, Qt::MiddleButton, CAMERA, TRANSLATE ,false );
    setMouseBinding( Qt::ShiftModifier, Qt::MiddleButton, CAMERA, ROTATE);
    setMouseBinding(Qt::ControlModifier, Qt::MiddleButton, CAMERA, ZOOM, false);

//    clearShortcuts();

    setShortcut(DRAW_AXIS, 0);
    setShortcut(DRAW_GRID, 0);
    setShortcut(DISPLAY_FPS, 0);
    setShortcut(ENABLE_TEXT, 0);
    setShortcut(STEREO, 0);
    setShortcut(HELP, 0);
    setShortcut(EXIT_VIEWER, 0);
    setShortcut(STEREO, 0);



}

void PersInterfaceGLViewer::draw()
{
    if ( myViewSettings.isLight1On ) {
        float lightPos[] = { myViewSettings.light1X, myViewSettings.light1Y, myViewSettings.light1Z, 0.0f };
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
        glEnable(GL_LIGHT1);
    } else {
        glDisable(GL_LIGHT1);
    }

    if ( myViewSettings.isLight2On ) {
        float lightPos[] = { myViewSettings.light2X, myViewSettings.light2Y, myViewSettings.light2Z, 0.0f };
        glLightfv(GL_LIGHT2, GL_POSITION, lightPos);
        glEnable(GL_LIGHT2);
    } else {
        glDisable(GL_LIGHT2);
    }

    if ( myViewSettings.isLight3On ) {
        float lightPos[] = { myViewSettings.light3X, myViewSettings.light3Y, myViewSettings.light3Z, 0.0f };
        glLightfv(GL_LIGHT3, GL_POSITION, lightPos);
        glEnable(GL_LIGHT3);
    } else {
        glDisable(GL_LIGHT3);
    }

    glLineWidth(1.0);

    if ( myViewSettings.isClippingOn ) {
        glEnable(GL_CLIP_PLANE0);
        glPushMatrix();
        GLdouble equation[] = { GLdouble(myViewSettings.clippingCorA),
                                GLdouble(myViewSettings.clippingCorB),
                                GLdouble(myViewSettings.clippingCorC),
                                GLdouble(myViewSettings.clippingCorD)
                              };
        glClipPlane(GL_CLIP_PLANE0, equation);
        glPopMatrix();
    } else {
        glDisable(GL_CLIP_PLANE0);
    }

    glColor3f(0.0f, 0.0f, 1.0f);

    if ( myViewSettings.isAxisOn ) {
        drawAxis( myViewSettings.axisLength );
    }

    glColor3f(0.3f, 0.3f, 0.3f);
    if ( myViewSettings.isGridOn ) {
        myScene->drawGrid( myViewSettings.gridX, myViewSettings.gridY, myViewSettings.gridZ,
                           myViewSettings.gridPitch, myViewSettings.gridCountX, myViewSettings.gridCountY);
    }

    GLfloat refm[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, refm);

    glEnable(GL_BLEND);
//    myScene->drawSolid(false, false, drawingStyle, false);
    myScene->drawSolid(false, !isRenderMode, drawingStyle, false);
    glDisable(GL_BLEND);

    glEnable(GL_LIGHT0);

    if ( myViewSettings.isTextOn ) {
        myScene->drawMemberText( camera()->rightVector(), camera()->upVector() );
        myScene->drawJointText( camera()->rightVector(), camera()->upVector() );
    }
    if ( myViewSettings.isJointOn ) myScene->drawPoints();
    myScene->drawFrameScale( camera()->rightVector(), camera()->upVector() );
    myScene->drawFloorScale( camera()->rightVector(), camera()->upVector() );

    if ( selectMode != PERSSELECTMODE::PERS_NOSELECT ) drawSelectionRectangle();

    glDisable(GL_LIGHT0);
}

void PersInterfaceGLViewer::fastDraw()
{
    if ( isRenderMode ) return;

    if ( myViewSettings.isLight1On ) {
        float lightPos[] = { myViewSettings.light1X, myViewSettings.light1Y, myViewSettings.light1Z, 0.0f };
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
        glEnable(GL_LIGHT1);
    } else {
        glDisable(GL_LIGHT1);
    }
    if ( myViewSettings.isLight2On ) {
        float lightPos[] = { myViewSettings.light2X, myViewSettings.light2Y, myViewSettings.light2Z, 0.0f };
        glLightfv(GL_LIGHT2, GL_POSITION, lightPos);
        glEnable(GL_LIGHT2);
    } else {
        glDisable(GL_LIGHT2);
    }
    if ( myViewSettings.isLight3On ) {
        float lightPos[] = { myViewSettings.light3X, myViewSettings.light3Y, myViewSettings.light3Z, 0.0f };
        glLightfv(GL_LIGHT3, GL_POSITION, lightPos);
        glEnable(GL_LIGHT3);
    } else {
        glDisable(GL_LIGHT3);
    }

    if ( myViewSettings.isClippingOn ) {
        glEnable(GL_CLIP_PLANE0);
        glPushMatrix();
        GLdouble equation[] = { GLdouble(myViewSettings.clippingCorA),
                                GLdouble(myViewSettings.clippingCorB),
                                GLdouble(myViewSettings.clippingCorC),
                                GLdouble(myViewSettings.clippingCorD)
                              };
        glClipPlane(GL_CLIP_PLANE0, equation);
        glPopMatrix();
    } else {
        glDisable(GL_CLIP_PLANE0);
    }

    glColor3f(0.0f, 0.0f, 1.0f);

    if ( myViewSettings.isAxisOn ) {
        drawAxis( myViewSettings.axisLength );
    }

    glLineWidth(1.0);

    GLfloat refm[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glShadeModel(GL_FLAT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, refm);

    if ( myViewSettings.isClippingOn ) {
        glPushMatrix();
        GLdouble equation[] = { GLdouble(myViewSettings.clippingCorA),
                                GLdouble(myViewSettings.clippingCorB),
                                GLdouble(myViewSettings.clippingCorC),
                                GLdouble(myViewSettings.clippingCorD)
                              };
        glClipPlane(GL_CLIP_PLANE0, equation);
        glPopMatrix();
    }

    glEnable(GL_BLEND);
    myScene->drawSolid(false, true, drawingStyle, true);
    glDisable(GL_BLEND);
}

void PersInterfaceGLViewer::drawWithNames()
{
    if ( isRenderMode ) return;
    myScene->drawSolid(true, true, DRAWINGSTYLE::SOLIDONLY, false);
}

void PersInterfaceGLViewer::postSelection( const QPoint & )
{
    GLuint id = selectedName();
    if ( id != -1 ) {
        emit signalAppendSelectedMemberID(id);
        emit signalSendSelectedItems();
    }
}

void PersInterfaceGLViewer::mousePressEvent(QMouseEvent *e)
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

    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier)) {
        myScene->removeSelectedMemberID( selectedName() );
        emit signalSendSelectedItems();
    }
    if ( e->button() == Qt::RightButton && (e->modifiers() == Qt::NoModifier) ) {
        emit signalSetActionStatus(myScene->selectedDataTypes());
    }
}


void PersInterfaceGLViewer::mouseMoveEvent(QMouseEvent *e)
{
    if ( selectMode != PERSSELECTMODE::PERS_NOSELECT ) {
        selectRect.setBottomRight(e->pos());
        QGLViewer::mouseMoveEvent(e);
        update();
    } else {
        QGLViewer::mouseMoveEvent(e);
    }
}

void PersInterfaceGLViewer::mouseReleaseEvent(QMouseEvent *e)
{
    if ( selectMode != PERSSELECTMODE::PERS_NOSELECT ) {
        emit signalSelectPoints(camera(), selectRect, selectMode);
        update();
    }
    selectMode = PERSSELECTMODE::PERS_NOSELECT;

    QGLViewer::mouseReleaseEvent(e);
}



void PersInterfaceGLViewer::keyPressEvent(QKeyEvent *e)
{
    if ( e->key() == Qt::Key_Escape ) {
        slotClearSelectedItems();
    } else if ( e->key() == Qt::Key_Delete ) {
        emit signalDeleteItems();
    } else if ( e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_F ) {
        emit signalFindItems();
    }
}

void PersInterfaceGLViewer::drawSelectionRectangle() const
{
    startScreenCoordinatesSystem();

    glEnable(GL_LINE_STIPPLE);

    glLineStipple(1, 0x0F0F);
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

void PersInterfaceGLViewer::zoomIn()
{
    QWheelEvent ev(QPoint(), -8, Qt::NoButton, Qt::NoModifier);
    QCoreApplication::sendEvent(this, &ev);
}

void PersInterfaceGLViewer::zoomOut()
{
    QWheelEvent ev(QPoint(), 8, Qt::NoButton, Qt::NoModifier);
    QCoreApplication::sendEvent(this, &ev);
}

void PersInterfaceGLViewer::slotClearSelectedItems()
{
    myScene->clearSelectedItems();
}

} // namespace post3dapp
