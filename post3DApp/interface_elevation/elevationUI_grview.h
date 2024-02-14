#ifndef ELEVATIONUIDRAWGRVIEW_H
#define ELEVATIONUIDRAWGRVIEW_H

#include "customdraw_grview.h"

namespace post3dapp{
class AbstractEventOperation;
class EditAllEventOperation;
class ElevationInterfaceGrScene;
class LineEventOperation;
class PlaneEventOperation;
class PointEventOperation;
class WallEventOperation;

class ElevationInterfaceGrView : public CustomDrawGrView
{
    Q_OBJECT

public:

    ElevationInterfaceGrView( ElevationInterfaceGrScene *scene, QWidget *parent );

    void initializeEventOperation();

public slots:

    void fitWindow() override;

public slots:

    void importCurrentStatus();
    void sendSelectedItems();

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

    ElevationInterfaceGrScene *elevationGrScene;
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
#endif
