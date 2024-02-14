#ifndef PERSINTERFACEVIEWER_H
#define PERSINTERFACEVIEWER_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

#include "define_glsetting.h"

namespace post3dapp{
enum ELEMENTTYPE;
enum class DATATYPE;
class JointData;
class ManagePersActions;
class MemberData;
class PersInterfaceScene;

class POST3D_CLASS_DLLSPEC PersInterfaceGLViewer : public QGLViewer
{

    Q_OBJECT

public:

    PersInterfaceGLViewer( PersInterfaceScene *s, QWidget *parent,
                           const QGLWidget *shareWidget = NULL);

    void setRenderMode(bool on)
    {
        isRenderMode = on;
    }
    void changeViewSettings(const PERSSETTINGVALUES &settings);

    PersInterfaceScene* getScene(){return myScene;}

public slots:

    void zoomIn();
    void zoomOut();
    void fitWindow();
    void resetViewDir(int);
    void slotChangeDrawingStyle(int style);

    void slotClearSelectedItems();


signals:
    void signalShowAutoAttachDialog();
    void signalShowChangeAngleDialog();
    void signalShowChangeFaceDialog();
    void signalShowChangeJointDialog();
    void signalChangeWallOpen();
    void signalChangeSectionDialog();
    void signalShowChangeShiftDialog();
    void signalFindItems();
    void signalDeleteItems();
    void signalDeleteIndependentJoints();
    void signalReverseMemberJoint();
    void signalDeleteWallOpen();

    void signalSetActionStatus(const QList<DATATYPE>&);
    void signalSendSelectedItems();
    void signalSelectPoints(qglviewer::Camera *camera, const QRect &rect, PERSSELECTMODE mode);
    void signalAppendSelectedMemberID(GLuint);
//    void selectionChanged();

protected :

    void init() override;
    void draw() override;
    void fastDraw() override;
    void drawWithNames() override;
    void postSelection(const QPoint &point) override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:

    PersInterfaceScene *myScene;
    QRect selectRect;

    bool isRenderMode;

    PERSSETTINGVALUES myViewSettings;

    DRAWINGSTYLE drawingStyle;

    PERSSELECTMODE selectMode;

    void drawSelectionRectangle() const;



};
} // namespace post3dapp
#endif

