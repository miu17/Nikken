#ifndef ELEVATIONCL_VIEW_H
#define ELEVATIONCL_VIEW_H

#include <QGraphicsLineItem>

#include "customdraw_grview.h"

namespace post3dapp{
class ElevationCalcLoadGrScene;
class JointData;
class MemberData;

class ElevationCalcLoadGrView : public CustomDrawGrView
{
    Q_OBJECT

public:

    ElevationCalcLoadGrView( ElevationCalcLoadGrScene *scene, QWidget *parent );

public slots:

    void fitWindow() override;
    void slotWaitingSelect();
    void slotShowTextDialog();

public slots:

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
    QPair<QList<JointData *>, QList<MemberData *> > getSelectedItems() const;
    void sendSelectionChanged();
    ElevationCalcLoadGrScene *elevationGrScene;
    QGraphicsLineItem *dummyItem;

    QMenu *menu;

    bool isViewMode;
    bool rubberBandOn;
};
} // namespace post3dapp
#endif // ELEVATIONCL_VIEW_H
