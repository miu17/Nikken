﻿#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <qmath.h>

#include "v_isoitem.h"
#include "unified_data.h"
#include "calc_vector2d.h"
namespace post3dapp{
IsolatorVLineItem::IsolatorVLineItem(bool _isPlan, const Input2DViewSettings& setting,QPointF p0, const VLINESHAPE &shape,
                                     ITEMVIEWTYPE view_type, bool forward)
    : CustomDrawVLineItem(_isPlan, setting,ELISO, p0, shape, view_type, ELEMENTDIR::ELCROSS, forward)
{
    createShapePath();
    update(boundRect);
}

void IsolatorVLineItem::createShapePath()
{
    if ( sectionName.isEmpty() ) {
        createDummyShape();
        return;
    }
    if ( myLineShape.sizeB < 1.0e-3 && myLineShape.sizeH < 1.0e-3 ) {
        createErrorShape();
        return;
    }

    drawShapePath = QPainterPath();
    myLineShape.drawIsoLineShape(drawShapePath);
    drawShapePath.closeSubpath();
    shapePath = drawShapePath;
    boundRect = drawShapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
    textPoint = ( myElementDir == ELEMENTDIR::ELCROSS ) ? QPointF(100.0, -100.0)
                : QPointF(-100.0, -100.0);
    textName = sectionName ;
}
} // namespace post3dapp