#ifndef WALLHPLANEITEM_H
#define WALLHPLANEITEM_H

#include "customdraw_hplaneitem.h"
namespace post3dapp{
class WallHPlaneItem : public CustomDrawHPlaneItem
{

public:

    WallHPlaneItem( const Input2DViewSettings& setting, ELEMENTTYPE, const QList<QPointF> &, const QString &, bool,
                    qreal thickness = 0.0, const QString &wpname = QString());


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void createShapePath() override;

private:
    QString wallOpenName;
    QPainterPath wallSlitPath;



};
} // namespace post3dapp
#endif
