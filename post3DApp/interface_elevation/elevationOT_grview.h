#ifndef ELEVATIONOUTDRAWGRVIEW_H
#define ELEVATIONOUTDRAWGRVIEW_H


#include "customdraw_grview.h"

class QGraphicsLineItem;

namespace post3dapp{
class ElevationOutputGrScene;

class ElevationOutputGrView : public CustomDrawGrView
{
    Q_OBJECT

public:

    ElevationOutputGrView( ElevationOutputGrScene *scene, QWidget *parent );

public slots:

    void fitWindow() override;
//    virtual void fitPaper(QSize size);
    void slotWaitingSelect();
    void slotShowTextDialog();
    void slotShowNmintDialog();
//    void changeViewScale(qreal scale){CustomDrawGrView::changeViewScale(scale);}
//    virtual void drawForeGround(QPainter* paniter,const QRectF &rect);
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

    ElevationOutputGrScene *elevationGrScene;
    QGraphicsLineItem *dummyItem;

    QMenu *menu;

    bool isViewMode;
    bool rubberBandOn;

    QList<QGraphicsItem*> tmpItems;
    int tmpItemIndex;
};
} // namespace post3dapp
#endif
