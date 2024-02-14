#ifndef SLABLOADHPLANEITEM_H
#define SLABLOADHPLANEITEM_H

#include "customdraw_hplaneitem.h"
namespace post3dapp{
class SlabLoadHPlaneItem : public CustomDrawHPlaneItem
{

public:

    SlabLoadHPlaneItem(const Input2DViewSettings& setting, const QList<QPointF> &, const QString &, bool);

protected:

    void createShapePath() override;

};
} // namespace post3dapp
#endif
