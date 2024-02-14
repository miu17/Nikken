#ifndef PLANOUTDRAWGRVIEW_H
#define PLANOUTDRAWGRVIEW_H

#include "customdraw_grview.h"

namespace post3dapp{
class PlanOutputGrScene;

class PlanOutputGrView : public CustomDrawGrView
{
    Q_OBJECT

public:

    PlanOutputGrView( PlanOutputGrScene *scene, QWidget *parent );

public slots:

    void fitWindow() override;
    void slotWaitingSelect();
    void slotShowTextDialog();
    void slotShowNmintDialog();
    void slotSelectOne(QAction*);
    void slotSelectHighLight (QAction* act);
    void slotSelectionChanged();

protected:

    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent (QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;
    void mousePressEvent (QMouseEvent *event) override;


    void closeEvent ( QCloseEvent *event ) override
    {
        delete dummyItem;
        CustomDrawGrView::closeEvent( event );
    }

private:

    PlanOutputGrScene *planGrScene;
    QGraphicsLineItem *dummyItem;

    QMenu *menu;

    bool isViewMode;
    bool rubberBandOn;

    QList<QGraphicsItem*> tmpItems;
    int tmpItemIndex;

};
} // namespace post3dapp
#endif
