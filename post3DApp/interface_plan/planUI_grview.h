#ifndef PLANUIDRAWGRVIEW_H
#define PLANUIDRAWGRVIEW_H

#include "customdraw_grview.h"

namespace post3dapp{
class AbstractEventOperation;
class EditAllEventOperation;
class LineEventOperation;
class PlaneEventOperation;
class PlanInterfaceGrScene;
class PointEventOperation;
class WallEventOperation;

class PlanInterfaceGrView : public CustomDrawGrView
{
    Q_OBJECT

public:

    PlanInterfaceGrView( PlanInterfaceGrScene *scene, QWidget *parent );

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

    PlanInterfaceGrScene *planGrScene;
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
