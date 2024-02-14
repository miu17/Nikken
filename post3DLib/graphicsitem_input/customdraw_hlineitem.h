#ifndef CUSTOMDRAWHLINEITEM_H
#define CUSTOMDRAWHLINEITEM_H

#include "customdraw_item.h"
#include "define_unifieddata.h"
#include "lineshape.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{


/* 回転された断面の幅とコーナー線の位置  */
struct ROTATEDSECTION {
    explicit ROTATEDSECTION(
        qreal ww = 0.0,
        qreal xx = -1.0 )
        : rotW(ww), rotL(xx) { }

    qreal rotW;
    qreal rotL;
};



class POST3D_CLASS_DLLSPEC CustomDrawHLineItem : public CustomDrawItem
{
public:

    CustomDrawHLineItem( const Input2DViewSettings& setting,ELEMENTTYPE type,
                         const QList<QPointF> &plist, const QList<qreal> &lenlist,
                         const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                         const MEMBERPARTITION &partition );

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setIncompleteFlag(bool onoff)
    {
        isIncomplete = onoff;
    }
    bool isIncompleteItem() const
    {
        return isIncomplete;
    }

    QRectF approximatelyRect() const;
    bool approximatelyContains(QPointF pp) const;

    QList<QPointF> divideEquallyPoints(int) const;
    QList<QPointF> divideFromIJPoints(qreal, qreal) const;
    bool isOnLinePoint(QPointF pp, bool exceptPoint = false) const;

    QPointF divideRatioPoint(qreal, qreal) const;
    QList<QPointF> divideFromIPoint(qreal) const;
    QList<QPointF> divideFromJPoint(qreal) const;

    qreal distanceToPoint(QPointF pp) const;
    QList<QLineF> getItemLines() const;
    QLineF getIJLine() const;

protected:
    static constexpr qreal eps = 1.0e-6;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event)}
    void setPolygonPath(QPainterPath&, qreal);
    void setErrorOrDummyTextProperty(const QString&, qreal);

    QRectF appRect;
    bool isIncomplete;

    const ITEMVIEWTYPE myItemView;

    QList<QPointF> myPointList;
    const QList<qreal> myLenList;
    const HLINESHAPE myLineShape;
    const MEMBERPARTITION myPartition;

    virtual void createShapePath() = 0 ;

    ROTATEDSECTION getRotatedSection(RENDERSHAPE type, qreal bb, qreal hh, qreal angle) const;

};
} // namespace post3dapp
#endif
