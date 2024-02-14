#include "h_isoitem.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "calc_vector2d.h"
#include "unified_data.h"
namespace post3dapp{
IsolatorHLineItem::IsolatorHLineItem( const Input2DViewSettings &setting, const QList<QPointF> &plist, const QList<qreal> &lenlist,
                                      const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                                      const MEMBERPARTITION &partition )
    : CustomDrawHLineItem(setting,ELISO, plist, lenlist, shape, view_type, partition)
{
    createShapePath();
    update(boundRect);
}

void IsolatorHLineItem::createShapePath()
{
    myElementState = ( !sectionName.isEmpty() ) ? ELNORMAL : ELDUMMY;

    qreal dummyWidth = ( myElementState == ELDUMMY ) ? 400.0 : myLineShape.leftSizeB ;
    qreal shapeWidth = ( myElementState == ELDUMMY ) ? 500.0 : myLineShape.leftSizeB + 100.0 ;
    drawShapePath = QPainterPath();
    setPolygonPath(drawShapePath, dummyWidth);
    drawShapePath.closeSubpath();

    shapePath = QPainterPath();
    setPolygonPath(shapePath, shapeWidth);
    shapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();

    QString text = ( myElementState == ELDUMMY ) ? "(dummyI)" : sectionName;
    setErrorOrDummyTextProperty(text, dummyWidth);
}

} // namespace post3dapp
