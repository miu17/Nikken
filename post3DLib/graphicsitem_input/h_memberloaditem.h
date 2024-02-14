#ifndef MEMBERLOADHLINEITEM_H
#define MEMBERLOADHLINEITEM_H

#include "customdraw_hlineitem.h"
namespace post3dapp{
class MemberLoadHLineItem : public CustomDrawHLineItem
{
public:
    MemberLoadHLineItem( const Input2DViewSettings &setting, const QList<QPointF> &plist, const QList<qreal> &lenlist,
                         const QString &name );
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void createShapePath() override;

};
} // namespace post3dapp
#endif
