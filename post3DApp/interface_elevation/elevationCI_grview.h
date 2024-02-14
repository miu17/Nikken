#ifndef ELEVATIONCI_GRVIEW_H
#define ELEVATIONCI_GRVIEW_H

#include "customdraw_grview.h"

namespace post3dapp{

class AbstractEventOperation;
class EditAllEventOperation;
class ElevationCopyInterfaceGrScene;
class LineEventOperation;
class PlaneEventOperation;
class PointEventOperation;
class WallEventOperation;

class ElevationCopyInterfaceGrView : public CustomDrawGrView
{
    Q_OBJECT

public:

    ElevationCopyInterfaceGrView( ElevationCopyInterfaceGrScene *scene, QWidget *parent );

    void initializeEventOperation();

public slots:

    void fitWindow() override;
    void sendSelectedItems();

public slots:

    void importCurrentStatus();

protected:

    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent (QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;
    void mousePressEvent (QMouseEvent *event) override;
    void keyPressEvent (QKeyEvent *event) override;
    void mouseDoubleClickEvent (QMouseEvent *event) override;

    void closeEvent ( QCloseEvent *event ) override
    {
        delete dummyItem;
        CustomDrawGrView::closeEvent( event );
    }

private:

    ElevationCopyInterfaceGrScene *elevationGrScene;
    QGraphicsLineItem *dummyItem;

    AbstractEventOperation *currentEventOperation;
    PointEventOperation *pointEventOperation;
    LineEventOperation *lineEventOperation;
    PlaneEventOperation *planeEventOperation;
    WallEventOperation *wallEventOperation;
    EditAllEventOperation *editAllEventOperation;
    AbstractEventOperation *noneEventOperation;
};
} // namespace post3dapp

#endif // ELEVATIONCI_GRVIEW_H
