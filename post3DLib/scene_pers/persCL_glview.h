#ifndef PERSCLGLVIEWER_H
#define PERSCLGLVIEWER_H

#include "define_glsetting.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QMenu;
namespace post3dapp{
class PersCalcLoadScene;

class POST3D_CLASS_DLLSPEC PersCalcLoadGLViewer : public QGLViewer
{

    Q_OBJECT

public:

    PersCalcLoadGLViewer( PersCalcLoadScene *s, QWidget *parent,
                        const QGLWidget *shareWidget = nullptr);

    void clearFloorAttachment();
    bool appendFloorAttachment(const QString &);
    void clearFrameAttachment();
    bool appendFrameAttachment(const QString &);

    void changeViewSettings(const PERSSETTINGVALUES &settings);

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

    void init() override;
    void draw() override;
    void fastDraw() override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:

    PersCalcLoadScene *myScene;
    QRect selectRect;

    QMenu *menu;

    PERSSETTINGVALUES myViewSettings;
    PERSSELECTMODE selectMode;

    void drawSelectionRectangle() const;
    void drawOutputScale();

};
} // namespace post3dapp
#endif

