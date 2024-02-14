#ifndef SLABVPLANEITEM_H
#define SLABVPLANEITEM_H

#include "customdraw_vplaneitem.h"
namespace post3dapp{
class SlabVPlaneItem : public CustomDrawVPlaneItem
{
public:
    SlabVPlaneItem( bool _isPlan, const Input2DViewSettings& setting, const QList<QPointF> &, const QString &,
                    bool, ITEMVIEWTYPE, qreal tickness, qreal shift_z = 0.0 );

protected:
    void createShapePath() override;

private:
    const ITEMVIEWTYPE myItemView;

    void createPlanShapePath();
    void createElevationShapePath();
    void createSlabShapePath(qreal, qreal);
};
} // namespace post3dapp
#endif
