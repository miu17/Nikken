#include "h_beamitem.h"
namespace post3dapp{
BeamHLineItem::BeamHLineItem( const Input2DViewSettings& setting, const QList<QPointF> &plist, const QList<qreal> &lenlist,
                              const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                              const MEMBERPARTITION &partition )
    : GirderHLineItem(setting,ELBEAM, plist, lenlist, shape, view_type, partition)
{
}
} // namespace post3dapp
