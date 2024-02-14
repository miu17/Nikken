#ifndef PERSOUTGLVIEWER_H
#define PERSOUTGLVIEWER_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

#include <QGLViewer/qglviewer.h>
#include "define_glsetting.h"

class QMenu;
namespace post3dapp{
class PersOutputScene;

class POST3D_CLASS_DLLSPEC PersOutputGLViewer : public QGLViewer
{

    Q_OBJECT

public:

    PersOutputGLViewer( PersOutputScene *s, QWidget *parent,
                        const QGLWidget *shareWidget = NULL);

    void clearFloorAttachment();
    bool appendFloorAttachment(const QString &);
    void clearFrameAttachment();
    bool appendFrameAttachment(const QString &);

    void changeViewSettings(const PERSSETTINGVALUES &settings);
    PersOutputScene* getScene(){return myScene;}

public slots:

    void zoomIn();
    void zoomOut();
    void fitWindow();
    void slotClearSelectedItems();

    void slotChangeLimitedFloor(const QString &);
    void slotChangeLimitedFrame(const QString &);

signals:
    void signalShowTextDialog();

protected :

    virtual void init();
    virtual void draw();
    virtual void fastDraw();
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent *e);

private:

    PersOutputScene *myScene;
    QRect selectRect;

    QMenu *menu;

    PERSSETTINGVALUES myViewSettings;
    PERSSELECTMODE selectMode;

    void drawSelectionRectangle() const;

};
} // namespace post3dapp
#endif

