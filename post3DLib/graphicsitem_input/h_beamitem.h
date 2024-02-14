#ifndef BEAMHLINEITEM_H
#define BEAMHLINEITEM_H

#include "h_girderitem.h"
namespace post3dapp{
class BeamHLineItem : public GirderHLineItem
{
public:
    BeamHLineItem( const Input2DViewSettings &setting, const QList<QPointF> &plist, const QList<qreal> &lenlist,
                   const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                   const MEMBERPARTITION &partition );

};
} // namespace post3dapp
#endif
