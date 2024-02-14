#ifndef BRACEHLINEITEM_H
#define BRACEHLINEITEM_H

#include "h_girderitem.h"
namespace post3dapp{
class BraceHLineItem : public GirderHLineItem
{
public:
    BraceHLineItem( const Input2DViewSettings &setting, const QList<QPointF> &plist, const QList<qreal> &lenlist,
                    const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                    const MEMBERPARTITION &partition );
};
} // namespace post3dapp
#endif
