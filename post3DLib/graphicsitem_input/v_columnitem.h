#ifndef COLUMNVLINEITEM_H
#define COLUMNVLINEITEM_H

#include "customdraw_vlineitem.h"
namespace post3dapp{
class ColumnVLineItem : public CustomDrawVLineItem
{
public:
    ColumnVLineItem( bool _isPlan, const Input2DViewSettings &setting, QPointF p0, const VLINESHAPE &shape,
                     ITEMVIEWTYPE view_type, ELEMENTDIR direct, bool forward = true, qreal fr_angle = 0.0,
                     qreal shx = 0.0 );
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void createShapePath() override;

private:
    const qreal frameAngle;
    const qreal shiftX;

    QPainterPath basePlatePath;
    QString baseTextName;
    QPointF baseTextPoint;

    void createPlanShapePath();
    void createElevationShapePath();
};
} // namespace post3dapp
#endif
