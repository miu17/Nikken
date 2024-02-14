#ifndef COLUMNHLINEITEM_H
#define COLUMNHLINEITEM_H

#include "customdraw_hlineitem.h"
namespace post3dapp{
class ColumnHLineItem : public CustomDrawHLineItem
{

public:

    ColumnHLineItem( const Input2DViewSettings &setting, const QList<QPointF> &plist, const QList<qreal> &lenlist,
                     const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                     const MEMBERPARTITION &partition, qreal fr_angle = 0.0, qreal shx = 0.0 );

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

    void createShapePath() override;
    void createDummyShape();
    void createErrorShape();

private:
    qreal frameAngle;
    qreal shiftX;

    QPainterPath basePlatePath;
    QString baseTextName;
    QPointF baseTextPoint;
    qreal baseTextAngle;

    void createPlanShapePath();
    void createElevationShapePath();
    void createCommonShapePath(qreal, qreal, const QPointF&);
    void setTextProperty(qreal, qreal, const QPointF&);
    void setBaseProperty(qreal, const QPointF&);
};
} // namespace post3dapp
#endif
