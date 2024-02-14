#ifndef GIRDERVLINEITEM_H
#define GIRDERVLINEITEM_H

#include "customdraw_vlineitem.h"
namespace post3dapp{
class GirderVLineItem : public CustomDrawVLineItem
{
public:
    GirderVLineItem( bool _isPlan, const Input2DViewSettings& setting,ELEMENTTYPE type,
                     QPointF p0, const VLINESHAPE &shape,
                     ITEMVIEWTYPE view_type, bool forward = true, qreal angle = 0.0, qreal fy = 0.0 );

protected:
    void createShapePath() override;

private:
    const qreal globalAngle;
    const qreal faceShift;

    void createPlanShapePath();
    void createElevationShapePath();

};
} // namespace post3dapp
#endif
