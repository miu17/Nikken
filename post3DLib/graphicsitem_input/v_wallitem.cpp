#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <qmath.h>

#include "v_wallitem.h"
#include "calc_vector2d.h"
namespace post3dapp{
WallVPlaneItem::WallVPlaneItem( bool _isPlan, const Input2DViewSettings &setting,ELEMENTTYPE type, const QList<QPointF> &plist, const QString &str,
                                bool onplane,  qreal thickness, qreal shift_z )
    : CustomDrawVPlaneItem(_isPlan, setting, type, plist, str, onplane, thickness, shift_z )
{
    qreal len = 0.0;
    myLenList.append(len);
    for (int i = 1; i < myPointList.count(); i++) {
        len += CalcVector2D::length( myPointList.at(i) - myPointList.at(i - 1) );
        myLenList.append(len);
    }

    createShapePath();
    update(boundRect);
}

void WallVPlaneItem::createShapePath()
{
    qreal drawWidth = 150.0;
    if ( sectionName.isEmpty() ) {
        myElementState = ELDUMMY;
        textName = "(dummyW)";
    } else if ( !isOnPlane || myThickness < 1.0e-3 ) {
        myElementState = ELERROR;
        textName = "ERROR(" + sectionName + ")";
    } else {
        myElementState = ELNORMAL;
        textName = sectionName;
        drawWidth = myThickness;
    }

    setDrawShape(drawShapePath, drawWidth, 0);
    shapePath = drawShapePath;
    boundRect = drawShapePath.boundingRect();
    setTextProperty(drawWidth, 0);
}

} // namespace post3dapp

