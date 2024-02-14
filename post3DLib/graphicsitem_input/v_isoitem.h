#ifndef ISOVLINEITEM_H
#define ISOVLINEITEM_H

#include "customdraw_vlineitem.h"
namespace post3dapp{
class IsolatorVLineItem : public CustomDrawVLineItem
{
public:
    IsolatorVLineItem( bool _isPlan, const Input2DViewSettings &setting, QPointF p0, const VLINESHAPE &shape,
                       ITEMVIEWTYPE view_type, bool forward = true );

protected:
    void createShapePath() override;

};
} // namespace post3dapp
#endif
