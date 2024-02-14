#include "lineshape.h"

#include <QtMath>
namespace post3dapp{
void VLINESHAPE::drawDamperLineShape(QPainterPath& path) const
{
     path.addEllipse(QPointF(), sizeB / 2.0, sizeH / 2.0);
}

void VLINESHAPE::drawIsoLineShape(QPainterPath& path) const
{
     path.addEllipse(QPointF(), sizeB / 2.0, sizeH / 2.0);
}

void VLINESHAPE::drawLineShape(QPainterPath& path, qreal localAngle, qreal sx, qreal sy) const
{
    switch(renderType){
    case RENDERSHAPE::SHAPE_BOX:
        addRectangleShape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_H:
        addHShape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_I:
        addIShape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_PIPE:
        addPipeShape(path, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_T1:
        addT1Shape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_T2:
        addT2Shape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_T3:
        addT3Shape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_T4:
        addT4Shape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_C1:
        addC1Shape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_C2:
        addC2Shape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_C3:
        addC3Shape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_C4:
        addC4Shape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_L:
        addLShape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_CROSS:
        addCrossPlateShape(path, localAngle, sx, sy);
        break;
    case RENDERSHAPE::SHAPE_CROSSH:
        addCrossHShape(path, localAngle, sx, sy);
        break;
    }
}

void VLINESHAPE::drawGirderPlanLineShape(QPainterPath& path, bool isForward, qreal globalAngle, qreal sx, qreal sy) const
{
    qreal localAngle = (isForward) ? angle + M_PI / 2.0 + globalAngle: -angle + M_PI / 2.0 + globalAngle;
    drawLineShape(path, localAngle, sx, sy);
}

void VLINESHAPE::drawGirderElevationLineShape(QPainterPath& path, bool isForward, qreal sx, qreal sy) const
{
    qreal localAngle = (isForward) ? angle : -angle;
    drawLineShape(path, localAngle, sx, sy);
}

void VLINESHAPE::drawGirderPlanBoundingShape(QPainterPath& path, bool isForward, qreal globalAngle, qreal sx, qreal sy) const
{
    qreal localAngle = (isForward) ? angle + M_PI / 2.0 + globalAngle: -angle + M_PI / 2.0 + globalAngle;
    addRectangleShape(path, localAngle, sx, sy);
}

void VLINESHAPE::drawGirderElevationBoundingShape(QPainterPath& path, bool isForward, qreal sx, qreal sy) const
{
    qreal localAngle = (isForward) ? angle : -angle;
    addRectangleShape(path, localAngle, sx, sy);
}


void VLINESHAPE::drawColumnPlanLineShape(QPainterPath& path, bool isForward) const{
    qreal localAngle = (isForward) ? angle : -angle;
    drawLineShape(path, localAngle, shiftY, shiftZ);
}

void VLINESHAPE::drawColumnElevationLineShape(QPainterPath& path, bool isForward, qreal frameAngle, qreal shiftX) const
{
    qreal localAngle = (isForward) ? angle - frameAngle : -angle + frameAngle;
    drawLineShape(path, localAngle, shiftX, 0.0);
}


void VLINESHAPE::drawBaseShape(QPainterPath& path, qreal localAngle, qreal sx, qreal sy) const
{
    const qreal sin = qSin(localAngle);
    const qreal cos = qCos(localAngle);
    const QTransform trans(cos, sin, -sin, cos, sx, sy);

    const QPointF p1 = trans.map(QPointF(-baseSizeB / 2.0, baseSizeD / 2.0));
    path.moveTo(p1);
    path.lineTo(trans.map(QPointF(baseSizeB / 2.0, baseSizeD / 2.0)));
    path.lineTo(trans.map(QPointF(baseSizeB / 2.0, -baseSizeD / 2.0)));
    path.lineTo(trans.map(QPointF(-baseSizeB / 2.0, -baseSizeD / 2.0)));
    path.lineTo(p1);
}

void VLINESHAPE::drawColumnPlanBaseShape(QPainterPath &path, bool isForward) const
{
    qreal localAngle = (isForward) ? angle : -angle;
    drawBaseShape(path, localAngle, shiftY, shiftZ);
}

void VLINESHAPE::drawColumnElevationBaseShape(QPainterPath& path, bool isForward, qreal frameAngle, qreal shiftX) const
{
    qreal localAngle = (isForward) ? angle - frameAngle : -angle + frameAngle;
    drawBaseShape(path, localAngle, shiftX, 0.0);
}

void VLINESHAPE::drawColumnPlanBoundingShape(QPainterPath &path, bool isForward) const
{
    qreal localAngle = (isForward) ? angle : -angle;
    addRectangleShape(path, localAngle, shiftY, shiftZ);
}

void VLINESHAPE::drawColumnElevationBoundingShape(QPainterPath& path, bool isForward, qreal frameAngle, qreal shiftX) const
{
    qreal localAngle = (isForward) ? angle - frameAngle : -angle + frameAngle;
    addRectangleShape(path, localAngle, shiftX, 0.0);
}

void VLINESHAPE::addRectangleShape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy) const
{
    const qreal sin = qSin(localAngle);
    const qreal cos = qCos(localAngle);
    const QTransform trans(cos, sin, -sin, cos, sx, sy);

    const QPointF p1 = trans.map(QPointF(-sizeB / 2.0, sizeH / 2.0));
    path.moveTo(p1);
    path.lineTo(trans.map(QPointF(sizeB / 2.0, sizeH / 2.0)));
    path.lineTo(trans.map(QPointF(sizeB / 2.0, -sizeH / 2.0)));
    path.lineTo(trans.map(QPointF(-sizeB / 2.0, -sizeH / 2.0)));
    path.lineTo(p1);
}


void VLINESHAPE::addIShape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy, qreal rotate) const
{
    const qreal sin = qSin(localAngle + rotate);
    const qreal cos = qCos(localAngle + rotate);
    const QTransform trans(cos, sin, -sin, cos, sx, sy);

    const qreal tf = sizeH / 10.0;
    const qreal tw = sizeB / 10.0;
    const QPointF p1 = trans.map(QPointF(-sizeB / 2.0, sizeH / 2.0));
    path.moveTo(p1);
    path.lineTo(trans.map(QPointF( sizeB / 2.0, sizeH / 2.0)));
    path.lineTo(trans.map(QPointF( sizeB / 2.0, sizeH / 2.0 - tf)));
    path.lineTo(trans.map(QPointF( tw / 2.0, sizeH / 2.0 - tf)));
    path.lineTo(trans.map(QPointF( tw / 2.0, -sizeH / 2.0 + tf)));
    path.lineTo(trans.map(QPointF( sizeB / 2.0, -sizeH / 2.0 + tf)));
    path.lineTo(trans.map(QPointF( sizeB / 2.0, -sizeH / 2.0)));
    path.lineTo(trans.map(QPointF(-sizeB / 2.0, -sizeH / 2.0)));
    path.lineTo(trans.map(QPointF(-sizeB / 2.0, -sizeH / 2.0 + tf)));
    path.lineTo(trans.map(QPointF(-tw / 2.0, -sizeH / 2.0 + tf)));
    path.lineTo(trans.map(QPointF(-tw / 2.0, sizeH / 2.0 - tf)));
    path.lineTo(trans.map(QPointF(-sizeB / 2.0, sizeH / 2.0 - tf)));
    path.lineTo(p1);
}

void VLINESHAPE::addHShape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy) const
{
    return addIShape(path, localAngle, sx, sy, -M_PI / 2.0);
}

void VLINESHAPE::addPipeShape(QPainterPath &path, qreal sx, qreal sy) const
{
    path.addEllipse(QPointF(sx, sy), sizeB / 2.0, sizeB / 2.0);
}

void VLINESHAPE::addTShape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy, qreal rotate) const
{
    const qreal sin = qSin(localAngle + rotate);
    const qreal cos = qCos(localAngle + rotate);
    const QTransform trans(cos, sin, -sin, cos, sx, sy);

    qreal tf = sizeH / 10.0;
    qreal tw = sizeB / 10.0;
    QPointF p1 = trans.map(QPointF(-sizeB / 2.0, sizeH / 2.0));
    path.moveTo(p1);
    path.lineTo(trans.map(QPointF( sizeB / 2.0, sizeH / 2.0)));
    path.lineTo(trans.map(QPointF( sizeB / 2.0, sizeH / 2.0 - tf)));
    path.lineTo(trans.map(QPointF( tw / 2.0, sizeH / 2.0 - tf)));
    path.lineTo(trans.map(QPointF( tw / 2.0, -sizeH / 2.0)));
    path.lineTo(trans.map(QPointF(-tw / 2.0, -sizeH / 2.0)));
    path.lineTo(trans.map(QPointF(-tw / 2.0, sizeH / 2.0 - tf)));
    path.lineTo(trans.map(QPointF(-sizeB / 2.0, sizeH / 2.0 - tf)));
    path.lineTo(p1);
}

void VLINESHAPE::addCShape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy, qreal rotate) const
{
    const qreal sin = qSin(localAngle + rotate);
    const qreal cos = qCos(localAngle + rotate);
    const QTransform trans(cos, sin, -sin, cos, sx, sy);

    qreal tf = sizeH / 10.0;
    qreal tw = sizeB / 10.0;
    QPointF p1 = trans.map(QPointF(-sizeB / 2.0, sizeH / 2.0));
    path.moveTo(p1);
    path.lineTo(trans.map(QPointF( sizeB / 2.0, sizeH / 2.0)));
    path.lineTo(trans.map(QPointF( sizeB / 2.0, sizeH / 2.0 - tf)));
    path.lineTo(trans.map(QPointF(-sizeB / 2.0 + tw, sizeH / 2.0 - tf)));
    path.lineTo(trans.map(QPointF(-sizeB / 2.0 + tw, -sizeH / 2.0 + tf)));
    path.lineTo(trans.map(QPointF( sizeB / 2.0, -sizeH / 2.0 + tf)));
    path.lineTo(trans.map(QPointF( sizeB / 2.0, -sizeH / 2.0)));
    path.lineTo(trans.map(QPointF(-sizeB / 2.0, -sizeH / 2.0)));
    path.lineTo(p1);
}

void VLINESHAPE::addLShape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy, qreal rotate) const
{
    const qreal sin = qSin(localAngle + rotate);
    const qreal cos = qCos(localAngle + rotate);
    const QTransform trans(cos, sin, -sin, cos, sx, sy);

    qreal tf = sizeH / 10.0;
    qreal tw = sizeB / 10.0;
    QPointF p1 = trans.map(QPointF(-sizeB / 2.0, sizeH / 2.0));
    path.moveTo(p1);
    path.lineTo(trans.map(QPointF(-sizeB / 2.0 + tw, sizeH / 2.0)));
    path.lineTo(trans.map(QPointF(-sizeB / 2.0 + tw, -sizeH / 2.0 + tf)));
    path.lineTo(trans.map(QPointF( sizeB / 2.0, -sizeH / 2.0 + tf)));
    path.lineTo(trans.map(QPointF( sizeB / 2.0, -sizeH / 2.0)));
    path.lineTo(trans.map(QPointF(-sizeB / 2.0, -sizeH / 2.0)));
    path.lineTo(p1);
}

void VLINESHAPE::addCrossPlateShape(QPainterPath& path, qreal localAngle, qreal sx, qreal sy) const
{
    const qreal sin = qSin(localAngle);
    const qreal cos = qCos(localAngle);
    const QTransform trans(cos, sin, -sin, cos, sx, sy);

    qreal t1 = sizeH / 10.0;
    qreal t2 = sizeB / 10.0;
    QPointF p1 = trans.map(QPointF(-t1 / 2.0, sizeH / 2.0));
    path.moveTo(p1);
    path.lineTo(trans.map(QPointF( t1 / 2.0, sizeH / 2.0)));
    path.lineTo(trans.map(QPointF( t1 / 2.0, -sizeH / 2.0)));
    path.lineTo(trans.map(QPointF(-t1 / 2.0, -sizeH / 2.0)));
    path.lineTo(p1);
    QPointF p5 = trans.map(QPointF(-sizeB/2.0,-t2 / 2.0));
    path.moveTo(p5);
    path.lineTo(trans.map(QPointF(-sizeB/2.0, t2 / 2.0)));
    path.lineTo(trans.map(QPointF(sizeB/2.0, t2 / 2.0)));
    path.lineTo(trans.map(QPointF(sizeB/2.0,-t2 / 2.0)));
    path.lineTo(p5);
}

void VLINESHAPE::addCrossHShape(QPainterPath& path, qreal localAngle, qreal sx, qreal sy) const
{
    const qreal sin = qSin(localAngle);
    const qreal cos = qCos(localAngle);
    const QTransform trans(cos, sin, -sin, cos, sx, sy);

    qreal tf_T = sizeH_T / 10.0;
    qreal tw_T = sizeB_T / 10.0;
    QPointF p1 = trans.map(QPointF(-sizeB_T / 2.0, sizeH_T / 2.0));
    path.moveTo(p1);
    path.lineTo(trans.map(QPointF( sizeB_T / 2.0, sizeH_T / 2.0)));
    path.lineTo(trans.map(QPointF( sizeB_T / 2.0, sizeH_T / 2.0 - tf_T)));
    path.lineTo(trans.map(QPointF( tw_T / 2.0, sizeH_T / 2.0 - tf_T)));
    path.lineTo(trans.map(QPointF( tw_T / 2.0, -sizeH_T / 2.0 + tf_T)));
    path.lineTo(trans.map(QPointF( sizeB_T / 2.0, -sizeH_T / 2.0 + tf_T)));
    path.lineTo(trans.map(QPointF( sizeB_T / 2.0, -sizeH_T / 2.0)));
    path.lineTo(trans.map(QPointF(-sizeB_T / 2.0, -sizeH_T / 2.0)));
    path.lineTo(trans.map(QPointF(-sizeB_T / 2.0, -sizeH_T / 2.0 + tf_T)));
    path.lineTo(trans.map(QPointF(-tw_T / 2.0, -sizeH_T / 2.0 + tf_T)));
    path.lineTo(trans.map(QPointF(-tw_T / 2.0, sizeH_T / 2.0 - tf_T)));
    path.lineTo(trans.map(QPointF(-sizeB_T / 2.0, sizeH_T / 2.0 - tf_T)));
    path.lineTo(p1);

    qreal tf = sizeH / 10.0;
    qreal tw = sizeB / 10.0;
    QPointF p1_T = trans.map(QPointF( sizeH / 2.0 , -sizeB / 2.0));
    path.moveTo(p1_T);
    path.lineTo(trans.map(QPointF( sizeH / 2.0 , sizeB / 2.0)));
    path.lineTo(trans.map(QPointF(  sizeH / 2.0 - tf,sizeB / 2.0)));
    path.lineTo(trans.map(QPointF(  sizeH / 2.0 - tf,tw / 2.0)));
    path.lineTo(trans.map(QPointF(  -sizeH / 2.0 + tf,tw / 2.0)));
    path.lineTo(trans.map(QPointF( -sizeH / 2.0 + tf ,sizeB / 2.0)));
    path.lineTo(trans.map(QPointF( -sizeH / 2.0 ,sizeB / 2.0     )));
    path.lineTo(trans.map(QPointF( -sizeH / 2.0 ,-sizeB / 2.0     )));
    path.lineTo(trans.map(QPointF( -sizeH / 2.0 + tf , -sizeB / 2.0 )));
    path.lineTo(trans.map(QPointF(-sizeH / 2.0 + tf,-tw / 2.0)));
    path.lineTo(trans.map(QPointF( sizeH / 2.0 - tf,-tw / 2.0)));
    path.lineTo(trans.map(QPointF( sizeH / 2.0 - tf,-sizeB / 2.0)));
    path.lineTo(p1_T);
}

void VLINESHAPE::addT1Shape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy) const
{
    addTShape(path, localAngle, sx, sy, -M_PI / 2.0);
}
void VLINESHAPE::addT2Shape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy) const
{
    addTShape(path, localAngle, sx, sy, M_PI / 2.0);
}
void VLINESHAPE::addT3Shape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy) const
{
    addTShape(path, localAngle, sx, sy, M_PI);
}
void VLINESHAPE::addT4Shape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy) const
{
    addTShape(path, localAngle, sx, sy);
}

void VLINESHAPE::addC1Shape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy) const
{
    addCShape(path, localAngle, sx, sy, -M_PI / 2.0);
}
void VLINESHAPE::addC2Shape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy) const
{
    addCShape(path, localAngle, sx, sy, M_PI / 2.0);
}
void VLINESHAPE::addC3Shape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy) const
{
    addCShape(path, localAngle, sx, sy, M_PI);
}
void VLINESHAPE::addC4Shape(QPainterPath &path, qreal localAngle, qreal sx, qreal sy) const
{
    addCShape(path, localAngle, sx, sy);
}

qreal VLINESHAPE::getGirderElevationOffset(bool isForward) const
{
    qreal localAngle = ( isForward ) ? angle : -angle;
    qreal rotate = 0.0;
    switch(renderType){
    case RENDERSHAPE::SHAPE_T1:
    case RENDERSHAPE::SHAPE_C1:
    case RENDERSHAPE::SHAPE_H:
        rotate = M_PI / 2.0;
        break;
    case RENDERSHAPE::SHAPE_C2:
    case RENDERSHAPE::SHAPE_T2:
        rotate = -M_PI / 2.0;
    }

    localAngle -= rotate;

    qreal sh = qAbs(sizeH * qCos(localAngle)) + qAbs(sizeB * qSin(localAngle));
    if ( renderType == RENDERSHAPE::SHAPE_PIPE )
        sh = qAbs(sizeH);

    return shiftZ - sh / 2.0;
}


} // namespace post3dapp
