#ifndef CUSTOMDRAWGRVIEW_H
#define CUSTOMDRAWGRVIEW_H

#include <QGraphicsView>

namespace post3dapp{
enum class UnifiedDataType;

class  CustomDrawGrView : public QGraphicsView
{
    Q_OBJECT

public:

    CustomDrawGrView( UnifiedDataType isout,QWidget *parent );

    void clearSelectedItems();
//    void setSelectItems(const QList<QGraphicsItem*> &);
    UnifiedDataType askIsOutput(){return isOutput;}

public slots:

    virtual void fitWindow() { }

signals:

    void scaleChanged(qreal);

protected:

    virtual void drawForeground(QPainter *painter, const QRectF &rect) override;
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void changeViewScale(qreal scale);
    virtual void mouseDoubleClickEvent (QMouseEvent *) override { }
    virtual void mousePressEvent (QMouseEvent *event) override;
    virtual void mouseReleaseEvent (QMouseEvent *event) override;
    virtual void mouseMoveEvent (QMouseEvent *event) override ;

    bool mousePressed;
    QPointF pressedPoint;
    qreal scaleFactor;

    QPoint rubberStartPoint;
    QList<QGraphicsItem *> mySelectedItems;
    QList<QGraphicsItem *> moveSelectedItems;

    const UnifiedDataType isOutput;

};
} // namespace post3dapp
#endif
