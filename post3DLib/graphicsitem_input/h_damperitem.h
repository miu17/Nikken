#ifndef DAMPERHLINEITEM_H
#define DAMPERHLINEITEM_H

#include "customdraw_hlineitem.h"
namespace post3dapp{
class DamperHLineItem : public CustomDrawHLineItem
{
public:
    DamperHLineItem( const Input2DViewSettings &setting, const QList<QPointF> &plist, const QList<qreal> &lenlist,
                     const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                     const MEMBERPARTITION &partition );

protected:
    void createShapePath() override;
};
} // namespace post3dapp
#endif
