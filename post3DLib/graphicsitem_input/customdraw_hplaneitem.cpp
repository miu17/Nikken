#include "customdraw_hplaneitem.h"

#include <QDebug>
#include <QPainter>

#include "calc_vector2d.h"
namespace post3dapp{
CustomDrawHPlaneItem::CustomDrawHPlaneItem(const Input2DViewSettings &setting,ELEMENTTYPE type, const QList<QPointF> &plist,
                                           const QString &str, bool onplane, qreal thickness)
    : CustomDrawItem(setting, type, str,ITEMTYPE::HPLANEITEM), myThickness(thickness)
{
    isOnPlane = onplane;
    QPointF p0 = plist.first();
    myPointList.clear();
    qreal xmin = p0.x();
    qreal xmax = p0.x();
    qreal ymin = p0.y();
    qreal ymax = p0.y();
    Q_FOREACH (QPointF p1, plist) {
        if ( p1.x() < xmin ) xmin = p1.x();
        if ( p1.x() > xmax ) xmax = p1.x();
        if ( p1.y() < ymin ) ymin = p1.y();
        if ( p1.y() > ymax ) ymax = p1.y();
        myPointList.append(p1 - p0);
    }
    appRect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
}

bool CustomDrawHPlaneItem::approximatelyContains(QPointF pp) const
{
    return appRect.contains(pp);
}

void CustomDrawHPlaneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                 QWidget *)
{
    setPainter(painter, option);

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(drawShapePath);

    if ( !isNameVisible ) return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->translate(textPoint.x(), -textPoint.y());

    painter->setFont(QFont("Consolas", FontSize(*painter)));
    QRect boundingRect = QFontMetrics(painter->font()).boundingRect(textName);
    QRectF rect;
    if ( myElementType != ELSLABLOAD ) {
        rect = QRectF(-boundingRect.width() / 2.0, -boundingRect.height(), boundingRect.width(), boundingRect.height());
    } else {
        rect = QRectF(-boundingRect.width() / 2.0, 0, boundingRect.width(), boundingRect.height());
    }
    painter->drawText(rect, Qt::AlignCenter, textName);
}

bool CustomDrawHPlaneItem::isOnLinePoint(QPointF pp, bool exceptPoint) const
{
    if ( exceptPoint
            && ( CalcVector2D::length( myPointList.first() + this->pos() - pp ) < 1.0e-3 ) ) return false;
    for (int i = 0; i < myPointList.count() ; i++) {
        QPointF p1 = myPointList.at(i) + this->pos();
        QPointF p2 = ( i == myPointList.count() - 1 ) ?  myPointList.first() + this->pos() :
                     myPointList.at(i + 1) + this->pos() ;
        if ( exceptPoint && ( CalcVector2D::length( p2 - pp ) < 1.0e-3 ) ) return false;
        if ( CalcVector2D::between(p1, p2, pp, 1.0e-3) ) return true;
    }
    return false;
}
} // namespace post3dapp
