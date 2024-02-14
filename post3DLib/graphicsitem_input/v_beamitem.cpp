#include "v_beamitem.h"
namespace post3dapp{
BeamVLineItem::BeamVLineItem(bool _isPlan, const Input2DViewSettings& setting,QPointF p0, const VLINESHAPE &shape,
                             ITEMVIEWTYPE view_type, bool forward, qreal angle, qreal fy)
    : GirderVLineItem(_isPlan, setting,ELBEAM, p0, shape, view_type, forward, angle, fy)
{
}

} // namespace post3dapp
