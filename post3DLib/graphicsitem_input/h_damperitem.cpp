#include "h_damperitem.h"
#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "calc_vector2d.h"
#include "unified_data.h"
namespace post3dapp{
DamperHLineItem::DamperHLineItem( const Input2DViewSettings &setting, const QList<QPointF> &plist, const QList<qreal> &lenlist,
                                  const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                                  const MEMBERPARTITION &partition )
    : CustomDrawHLineItem(setting,ELDAMPER, plist, lenlist, shape, view_type, partition)
{
    createShapePath();
    update(boundRect);
}

void DamperHLineItem::createShapePath()
{
    myElementState = ( !sectionName.isEmpty() ) ? ELNORMAL : ELDUMMY;
    qreal dummyWidth = ( myElementState == ELDUMMY ) ? 200.0 : myLineShape.leftSizeB ;
    qreal shapeWidth = ( myElementState == ELDUMMY ) ? 300.0 : myLineShape.leftSizeB + 100.0 ;
    drawShapePath = QPainterPath();
    setPolygonPath(drawShapePath, dummyWidth);
    drawShapePath.closeSubpath();

    shapePath = QPainterPath();
    setPolygonPath(shapePath, shapeWidth);
    shapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
    QString text = ( myElementState == ELDUMMY ) ? "(dummyD)" : sectionName;
    setErrorOrDummyTextProperty(text, dummyWidth);
}
} // namespace post3dapp


