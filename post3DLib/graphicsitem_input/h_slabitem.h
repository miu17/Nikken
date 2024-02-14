#ifndef SLABHPLANEITEM_H
#define SLABHPLANEITEM_H

#include "customdraw_hplaneitem.h"
namespace post3dapp{
class SlabHPlaneItem : public CustomDrawHPlaneItem
{

public:

    SlabHPlaneItem(const Input2DViewSettings& setting, const QList<QPointF> &, const QString &, bool, qreal thickness = 0.0 );

protected:

    void createShapePath() override;

};
} // namespace post3dapp
#endif
