#include "v_braceitem.h"
namespace post3dapp{
BraceVLineItem::BraceVLineItem(bool _isPlan, const Input2DViewSettings& setting,QPointF p0, const VLINESHAPE &shape,
                               ITEMVIEWTYPE view_type, bool forward, qreal angle, qreal fy)
    : GirderVLineItem(_isPlan, setting,ELBRACE, p0, shape, view_type, forward, angle, fy)
{
}

} // namespace post3dapp
