#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <qmath.h>

#include "v_girderitem.h"
#include "unified_data.h"
#include "calc_vector2d.h"
namespace post3dapp{
GirderVLineItem::GirderVLineItem(bool _isPlan, const Input2DViewSettings &setting, ELEMENTTYPE type,
                                 QPointF p0, const VLINESHAPE &shape,
                                 ITEMVIEWTYPE view_type, bool forward, qreal angle, qreal fy)
    : CustomDrawVLineItem(_isPlan, setting,type, p0, shape, view_type, ELEMENTDIR::ELCROSS, forward), globalAngle(angle), faceShift(fy)
{
    createShapePath();
    update(boundRect);
}

void GirderVLineItem::createShapePath()
{
    if ( sectionName.isEmpty() ) {
        createDummyShape();
        return;
    }
    if ( myLineShape.sizeB < 1.0e-3 && myLineShape.sizeH < 1.0e-3 ) {
        createErrorShape();
        return;
    }
    ( myItemView == ITEMVIEWTYPE::VIEW_PLAN ) ? createPlanShapePath() : createElevationShapePath() ;
}

void GirderVLineItem::createPlanShapePath()
{
    qreal sh = qAbs(myLineShape.sizeH * qCos(myLineShape.angle))
            + qAbs(myLineShape.sizeB * qSin(myLineShape.angle));
    if ( myLineShape.renderType == RENDERSHAPE::SHAPE_PIPE ) sh = qAbs(myLineShape.sizeH);
    qreal sx = 0.0, sy = 0.0;

    if ( isForward ) {
        sx = (faceShift + sh / 2.0) * qCos(globalAngle)
             - myLineShape.shiftY * qSin(globalAngle) - myLineShape.shiftZ * qCos(globalAngle);
        sy = (faceShift + sh / 2.0) * qSin(globalAngle)
             + myLineShape.shiftY * qCos(globalAngle) - myLineShape.shiftZ * qSin(globalAngle);
    } else {
        sx = (faceShift + sh / 2.0) * qCos(globalAngle)
             + myLineShape.shiftY * qSin(globalAngle) - myLineShape.shiftZ * qCos(globalAngle);
        sy = (faceShift + sh / 2.0) * qSin(globalAngle)
             - myLineShape.shiftY * qCos(globalAngle) - myLineShape.shiftZ * qSin(globalAngle);
    }

    drawShapePath = QPainterPath();
    myLineShape.drawGirderPlanLineShape(drawShapePath, isForward, globalAngle, sx, sy);
    drawShapePath.closeSubpath();

    shapePath = QPainterPath();
    myLineShape.drawGirderPlanBoundingShape(shapePath, isForward, globalAngle, sx, sy);
    shapePath.closeSubpath();
    boundRect = shapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
    textPoint = QPointF( myLineShape.sizeB / 2.0, -myLineShape.sizeH / 2.0);
    textPoint = textPoint + QPointF(sx, sy);
    textName = sectionName;
}

void GirderVLineItem::createElevationShapePath()
{
    qreal sx = ( isForward ) ? myLineShape.shiftY : -myLineShape.shiftY;
    qreal sy = myLineShape.getGirderElevationOffset(isForward);

    drawShapePath = QPainterPath();
    myLineShape.drawGirderElevationLineShape(drawShapePath, isForward, sx, sy);
    drawShapePath.closeSubpath();

    shapePath = QPainterPath();
    myLineShape.drawGirderElevationBoundingShape(shapePath, isForward, sx, sy);
    shapePath.closeSubpath();
    boundRect = shapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
    textPoint = QPointF(myLineShape.sizeB / 2.0, -myLineShape.sizeH / 2.0);
    textPoint = textPoint + QPointF(sx, sy);
    textName = sectionName;
}
} // namespace post3dapp
