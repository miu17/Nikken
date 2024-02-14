#ifndef GIRDERHLINEITEM_H
#define GIRDERHLINEITEM_H

#include "customdraw_hlineitem.h"
namespace post3dapp{
class GirderHLineItem : public CustomDrawHLineItem
{

public:

    GirderHLineItem( const Input2DViewSettings &setting, ELEMENTTYPE type,
                     const QList<QPointF> &plist, const QList<qreal> &lenlist,
                     const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                     const MEMBERPARTITION &partition );

protected:

    void createShapePath() override;
    void createDummyShape();
    void createErrorShape();

private:
    void createPlanShapePath();
    void createElevationShapePath();

};
} // namespace post3dapp
#endif
