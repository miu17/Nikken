#include "h_memberloaditem.h"

#include <QDebug>
#include <QPainter>

#include "calc_vector2d.h"
namespace post3dapp{
MemberLoadHLineItem::MemberLoadHLineItem( const Input2DViewSettings& setting,const QList<QPointF> &plist, const QList<qreal> &lenlist,
                                          const QString &name )
    : CustomDrawHLineItem(setting,ELMEMBERLOAD, plist, lenlist, HLINESHAPE(name), ITEMVIEWTYPE::VIEW_PLAN, MEMBERPARTITION())
{
    createShapePath();
    update(boundRect);
}

void MemberLoadHLineItem::createShapePath()
{
    /* 描画パスの作成 */
    qreal drawWidth = 50.0;
    qreal shapeWidth = 150.0;
    drawShapePath = QPainterPath();
    setPolygonPath(drawShapePath, drawWidth);
    drawShapePath.closeSubpath();

    shapePath = QPainterPath();
    setPolygonPath(shapePath, shapeWidth);
    shapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();


    /* 符号描画の諸元 */
//    qreal len_mid=2.0*( myLenList.first()+myLenList.last() ) / 3.0;

    qreal len_lef = ( myLenList.first() + myLenList.last() ) / 4.0;
    qreal len_rgt = 3.0 * ( myLenList.first() + myLenList.last() ) / 4.0;
    int idx_lef = 0;
    int idx_rgt = 0;
    for (int i = 0; i < myLenList.count() - 1; i++) {
        if ( myLenList.at(i) - 1.0e-3 < len_lef && len_lef < myLenList.at(i + 1) ) {
            idx_lef = i;
            break;
        }
        if ( myLenList.at(i) - 1.0e-3 < len_rgt && len_rgt < myLenList.at(i + 1) ) {
            idx_rgt = i;
            break;
        }
    }
    textAngle = qAtan2(myPointList.at(idx_rgt + 1).y() - myPointList.at(idx_rgt).y(),
                       myPointList.at(idx_rgt + 1).x() - myPointList.at(idx_rgt).x()) * 180.0 / M_PI;
    if ( textAngle >= 92.0 || textAngle <= -88.0 ) {
        textAngle += 180.0;
        qreal rr = (len_lef - myLenList.at(idx_lef)) / (myLenList.at(idx_lef + 1) - myLenList.at(idx_lef));
        textPoint = CalcVector2D::interiorPoint(myPointList.at(idx_lef), myPointList.at(idx_lef + 1), rr);
    } else {
        qreal rr = (len_rgt - myLenList.at(idx_rgt)) / (myLenList.at(idx_rgt + 1) - myLenList.at(idx_rgt));
        textPoint = CalcVector2D::interiorPoint(myPointList.at(idx_rgt), myPointList.at(idx_rgt + 1), rr);
    }

    textShift = -drawWidth / 2.0;
    if ( sectionName.isEmpty() ) {
        myElementState = ELDUMMY;
        textName = "(dummyM)";
    } else {
        myElementState = ELNORMAL;
        textName = sectionName;
    }
}

void MemberLoadHLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
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

    painter->rotate(-textAngle);
    painter->drawText(QRect(-boundingRect.width() / 2.0, -textShift * DpiScale(*painter), boundingRect.width(), boundingRect.height()),
                      Qt::AlignCenter, textName);
}
} // namespace post3dapp
