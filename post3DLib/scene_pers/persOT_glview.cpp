#include "persOT_glview.h"

#include <QtGui>

#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "manage_springdata.h"
#include "persOT_scene.h"
#include "unified_settings.h"

namespace post3dapp{
PersOutputGLViewer::PersOutputGLViewer( PersOutputScene *s, QWidget *parent,
                                        const QGLWidget *shareWidget)
    : QGLViewer(parent, shareWidget), myScene(s)
{
    camera()->setViewDirection(qglviewer::Vec(0.0, 1.0, 0.0));
    camera()->setUpVector(qglviewer::Vec(0.0, 0.0, 1.0));
    camera()->setType(qglviewer::Camera::ORTHOGRAPHIC);

    camera()->fitBoundingBox(qglviewer::Vec(-5000.0f, -5000.0f, -5000.0f),
                             qglviewer::Vec(15000.0f, 15000.0f, 15000.0f));

    GLdouble ff = -1000000.0;
    camera()->setZNearCoefficient(ff);
    camera()->setZClippingCoefficient(1000000.);

    myViewSettings = PERSSETTINGVALUES();

    connect(UnifiedSettings::getInstance(), &UnifiedSettings::out3D_SettingChanged,
            this, QOverload<void>::of(&PersOutputGLViewer::update));

    QAction *waitingAction = new QAction(u8"選択解除", this);
    waitingAction->setShortcut(tr("Esc"));
    waitingAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( waitingAction, &QAction::triggered, this, &PersOutputGLViewer::slotClearSelectedItems );

    QAction *showTextAction = new QAction(u8"テキスト表示", this);
    showTextAction->setShortcut(tr("Ctrl+d"));
    showTextAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( showTextAction, &QAction::triggered, this, &PersOutputGLViewer::signalShowTextDialog );

    QList<QAction *> actionList;
    actionList << waitingAction << showTextAction;

    menu = new QMenu(this);
    menu->addActions(actionList);
    menu->insertSeparator(showTextAction);
    this->addActions(actionList);
}

void PersOutputGLViewer::fitWindow()
{
    qglviewer::Vec v1 = myScene->boundingMinVec();
    qglviewer::Vec v2 = myScene->boundingMaxVec();
    camera()->fitBoundingBox(v1, v2);
    update();
}

void PersOutputGLViewer::changeViewSettings(const PERSSETTINGVALUES &settings)
{
    myViewSettings = settings;
    setSceneCenter( qglviewer::Vec(myViewSettings.centerX, myViewSettings.centerY, myViewSettings.centerZ) );
    setBackgroundColor( myViewSettings.backColor );
    update();
}

void PersOutputGLViewer::init()
{
    setBackgroundColor( myViewSettings.backColor );

    glDisable(GL_LIGHT0);
    float lightAmbient0[] = {1.0f, 1.0f, 1.0f, 1.0f}; //環境光反射
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0); //環境光反射設定
    /*
        glEnable(GL_LIGHT1);

        float lightPos1[]={  1.0f, -1.0f, 1.0f, 0.0f};  //光源
        float lightAmbient[]={0.2f,0.2f,0.2f,1.0f};     //環境光反射
        float lightDiffuse[]={0.8f,0.8f,0.8f,0.8f};     //拡散反射

        glLightfv(GL_LIGHT1,GL_POSITION,lightPos1);     //光源
        glLightfv(GL_LIGHT1,GL_AMBIENT,lightAmbient);   //環境光反射設定
        glLightfv(GL_LIGHT1,GL_DIFFUSE,lightDiffuse);   //拡散反射設定
    */
    /*
        glDisable(GL_LIGHT2);

        float lightPos2[]={  -1.0f, 1.0f, 1.0f, 0.0f};  //光源

        glLightfv(GL_LIGHT2,GL_POSITION,lightPos2);     //光源
        glLightfv(GL_LIGHT2,GL_AMBIENT,lightAmbient);   //環境光反射設定
        glLightfv(GL_LIGHT2,GL_DIFFUSE,lightDiffuse);   //拡散反射設定
    */
    /*
        glEnable(GL_LIGHT3);

        float lightPos3[]={  -1.0f, -1.0f, 1.0f, 0.0f}; //光源

        glLightfv(GL_LIGHT3,GL_POSITION,lightPos3);     //光源
        glLightfv(GL_LIGHT3,GL_AMBIENT,lightAmbient);   //環境光反射設定
        glLightfv(GL_LIGHT3,GL_DIFFUSE,lightDiffuse);   //拡散反射設定
    */
    glEnable(GL_CLIP_PLANE0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setMouseBinding( Qt::ControlModifier, Qt::LeftButton, SELECT );
    setMouseBinding( Qt::AltModifier,     Qt::LeftButton, SELECT );

    setMouseBinding( Qt::ShiftModifier, Qt::LeftButton,   CAMERA, TRANSLATE, false );
    setMouseBinding( Qt::NoModifier,    Qt::MiddleButton, CAMERA, TRANSLATE, false );

    setShortcut(DRAW_AXIS, 0);
    setShortcut(DRAW_GRID, 0);
    setShortcut(DISPLAY_FPS, 0);
    setShortcut(ENABLE_TEXT, 0);
    setShortcut(STEREO, 0);
    setShortcut(HELP, 0);
    setShortcut(EXIT_VIEWER, 0);
    setShortcut(STEREO, 0);
}

void PersOutputGLViewer::draw()
{
//    glEnable(GL_LIGHT1);
//    glEnable(GL_LIGHT3);

    glLineWidth(1.0);

    glColor3f(0.0f, 0.0f, 1.0f);

    if ( myViewSettings.isAxisOn ) {
        glEnable(GL_LIGHT0);
        drawAxis( myViewSettings.axisLength );
        glDisable(GL_LIGHT0);
    }

    GLfloat refm[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, refm);

    glEnable(GL_BLEND);
    myScene->drawSolid();
    glDisable(GL_BLEND);

    glEnable(GL_LIGHT0);

    if ( myViewSettings.isTextOn ) {
        myScene->drawMemberText( camera()->rightVector(), camera()->upVector() );
        myScene->drawJointText( camera()->rightVector(), camera()->upVector() );
    }
    if ( myViewSettings.isJointOn ) myScene->drawPoints();

//    myScene->drawMemberText( camera()->rightVector(), camera()->upVector() );
//    myScene->drawJointText( camera()->rightVector(), camera()->upVector() );
//    myScene->drawPoints();

    myScene->drawFrameScale( camera()->rightVector(), camera()->upVector() );
    myScene->drawFloorScale( camera()->rightVector(), camera()->upVector() );

    if ( selectMode != PERSSELECTMODE::PERS_NOSELECT ) drawSelectionRectangle();

    myScene->drawOutputScale();
}

void PersOutputGLViewer::fastDraw()
{
//    glEnable(GL_LIGHT1);
//    glEnable(GL_LIGHT3);

    glColor3f(0.0f, 0.0f, 1.0f);

    if ( myViewSettings.isAxisOn ) {
        glEnable(GL_LIGHT0);
        drawAxis( myViewSettings.axisLength );
        glDisable(GL_LIGHT0);
    }

    glLineWidth(1.0);

    GLfloat refm[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glShadeModel(GL_FLAT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, refm);

    glEnable(GL_BLEND);
    myScene->drawSolid();
    glDisable(GL_BLEND);

    if ( myViewSettings.isJointOn ) myScene->drawPoints();
    myScene->drawOutputScale();
}

void PersOutputGLViewer::mousePressEvent(QMouseEvent *e)
{
    selectRect = QRect(e->pos(), e->pos());

    selectMode = PERSSELECTMODE::PERS_NOSELECT;
    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::ControlModifier))
        selectMode = PERSSELECTMODE::PERS_ADDSELECT;
    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier))
        selectMode = PERSSELECTMODE::PERS_REMOVESELECT;

    if ((e->button() == Qt::RightButton) && (e->modifiers() == Qt::AltModifier)) {
        myScene->clearSelectedItems();
        update();
    }

    QGLViewer::mousePressEvent(e);

//     if((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::AltModifier)){
//         myScene->removeSelectedMemberID( selectedName() );
//         updateGL();
//         myScene->sendSelectedItems();
//     }
    if ( e->button() == Qt::RightButton && (e->modifiers() == Qt::NoModifier) ) {
        bool isSelected = myScene->existSelectedItems();
        Q_FOREACH ( QAction *ac, menu->actions() ) ac->setEnabled( isSelected );
        menu->exec(QCursor::pos());
    }
}

void PersOutputGLViewer::mouseMoveEvent(QMouseEvent *e)
{
    if ( selectMode != PERSSELECTMODE::PERS_NOSELECT ) {
        selectRect.setBottomRight(e->pos());
        QGLViewer::mouseMoveEvent(e);
        update();
    } else {
        QGLViewer::mouseMoveEvent(e);
    }
}

void PersOutputGLViewer::mouseReleaseEvent(QMouseEvent *e)
{
    if ( selectMode != PERSSELECTMODE::PERS_NOSELECT ) {
        myScene->selectPoints(camera(), selectRect, selectMode);
        update();
    }
    selectMode = PERSSELECTMODE::PERS_NOSELECT;

    QGLViewer::mouseReleaseEvent(e);
}

void PersOutputGLViewer::keyPressEvent(QKeyEvent *e)
{
//    const Qt::KeyboardModifiers modifiers = e->modifiers();
    switch ( e->key() ) {
    case Qt::Key_Escape:
        slotClearSelectedItems();
        break;
    default :
        break;
    }
}

void PersOutputGLViewer::drawSelectionRectangle() const
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

void PersOutputGLViewer::zoomIn()
{
    QWheelEvent ev(QPoint(), -8, Qt::NoButton, Qt::NoModifier);
    QCoreApplication::sendEvent(this, &ev);
}

void PersOutputGLViewer::zoomOut()
{
    QWheelEvent ev(QPoint(), 8, Qt::NoButton, Qt::NoModifier);
    QCoreApplication::sendEvent(this, &ev);
}

void PersOutputGLViewer::slotClearSelectedItems()
{
    myScene->clearSelectedItems();
    update();
}

void PersOutputGLViewer::slotChangeLimitedFloor(const QString &str)
{
    myScene->setLimitedFloor(str);
    update();
}

void PersOutputGLViewer::slotChangeLimitedFrame(const QString &str)
{
    myScene->setLimitedFrame(str);
    update();
}

} // namespace post3dapp
