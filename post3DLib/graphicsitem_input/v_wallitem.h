#ifndef WALLVPLANEITEM_H
#define WALLVPLANEITEM_H

#include "customdraw_vplaneitem.h"
namespace post3dapp{
class WallVPlaneItem : public CustomDrawVPlaneItem
{
public:
    WallVPlaneItem( bool _isPlan, const Input2DViewSettings& setting,ELEMENTTYPE, const QList<QPointF> &, const QString &,
                    bool, qreal tickness, qreal shift_z = 0.0 );

protected:
    void createShapePath() override;

};
} // namespace post3dapp
#endif
