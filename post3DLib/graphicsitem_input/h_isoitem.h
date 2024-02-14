#ifndef ISOHLINEITEM_H
#define ISOHLINEITEM_H

#include "customdraw_hlineitem.h"
namespace post3dapp{
class IsolatorHLineItem : public CustomDrawHLineItem
{

public:

    IsolatorHLineItem( const Input2DViewSettings& setting,const QList<QPointF> &plist, const QList<qreal> &lenlist,
                       const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                       const MEMBERPARTITION &partition );

protected:

    void createShapePath() override;

};
} // namespace post3dapp
#endif
