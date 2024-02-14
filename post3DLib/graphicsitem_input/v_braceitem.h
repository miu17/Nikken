#ifndef BRACEVLINEITEM_H
#define BRACEVLINEITEM_H

#include "v_girderitem.h"
namespace post3dapp{
class BraceVLineItem : public GirderVLineItem
{
public:
    BraceVLineItem( bool _isPlan, const Input2DViewSettings &setting, QPointF p0, const VLINESHAPE &shape,
                    ITEMVIEWTYPE view_type, bool forward = true, qreal angle = 0.0, qreal fy = 0.0 );
};
} // namespace post3dapp
#endif
