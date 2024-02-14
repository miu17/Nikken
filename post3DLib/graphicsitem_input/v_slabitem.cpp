#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <qmath.h>

#include "v_slabitem.h"
#include "calc_vector2d.h"
namespace post3dapp{
SlabVPlaneItem::SlabVPlaneItem( bool _isPlan, const Input2DViewSettings& setting,const QList<QPointF> &plist, const QString &str,
                                bool onplane, ITEMVIEWTYPE view_type,qreal thickness, qreal shift_z )
    : CustomDrawVPlaneItem(_isPlan, setting, ELSLAB, plist, str, onplane, thickness, shift_z), myItemView(view_type)
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

void SlabVPlaneItem::createShapePath()
{
    qreal drawWidth = 150.0;
    if ( sectionName.isEmpty() ) {
        myElementState = ELDUMMY;
        textName = "(dummyS)";
    } else if ( !isOnPlane || myThickness < 1.0e-3 ) {
        myElementState = ELERROR;
        textName = "ERROR(" + sectionName + ")";
    } else {
        myElementState = ELNORMAL;
        textName = sectionName;
        drawWidth = myThickness;
    }
    ( myItemView == ITEMVIEWTYPE::VIEW_PLAN )
            ? createSlabShapePath(drawWidth, 0)
            : createSlabShapePath(drawWidth, drawWidth / 2.0) ;
}

void SlabVPlaneItem::createSlabShapePath(qreal drawWidth, qreal center)
{
    setDrawShape(drawShapePath, drawWidth, center);
    shapePath = drawShapePath;
    boundRect = drawShapePath.boundingRect();
    setTextProperty(drawWidth, 0);
}

} // namespace post3dapp


