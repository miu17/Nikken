#ifndef PLANCL_GRVIEW_H
#define PLANCL_GRVIEW_H

#include "customdraw_grview.h"

class QMenu;

namespace post3dapp{
class JointData;
class MemberData;
class PlanCalcLoadGrScene;

class PlanCalcLoadGrView : public CustomDrawGrView
{
    Q_OBJECT

public:

    PlanCalcLoadGrView( PlanCalcLoadGrScene *scene, QWidget *parent );

public slots:

    void fitWindow() override;
    void slotWaitingSelect();
    void slotShowTextDialog();

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
    QPair<QList<JointData*>, QList<MemberData*>> getSelectedItems() const;
    void sendSelectionChanged();
    PlanCalcLoadGrScene *planGrScene;
    QGraphicsLineItem *dummyItem;

    QMenu *menu;

    bool isViewMode;
    bool rubberBandOn;
};
} // namespace post3dapp
#endif // PLANCL_GRVIEW_H
