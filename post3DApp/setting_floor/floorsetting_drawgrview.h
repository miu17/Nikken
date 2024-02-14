#ifndef FLOORSETTINGDRAWGRVIEW_H
#define FLOORSETTINGDRAWGRVIEW_H

#include "customdraw_grview.h"

namespace post3dapp{
struct FLOORVALUES;
class FloorSettingDrawGrView: public CustomDrawGrView
{
    Q_OBJECT

public:

    explicit FloorSettingDrawGrView(QWidget *parent = 0);

    void setDrawingData(const QList<FLOORVALUES> &list);

public slots:

    void fitWindow() override;

protected:

    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent (QMouseEvent *event) override;

private:

    QList<FLOORVALUES> allFloorList;
    qreal minHeight;
    qreal maxHeight;
};
} // namespace post3dapp

#endif
