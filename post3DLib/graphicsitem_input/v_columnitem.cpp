#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <qmath.h>

#include "v_columnitem.h"
#include "unified_data.h"
#include "calc_vector2d.h"
namespace post3dapp{
ColumnVLineItem::ColumnVLineItem( bool _isPlan, const Input2DViewSettings& setting,QPointF p0, const VLINESHAPE &shape,
                                  ITEMVIEWTYPE view_type, ELEMENTDIR direct, bool forward, qreal fr_angle, qreal shx )
    : CustomDrawVLineItem(_isPlan, setting,ELCOLUMN, p0, shape, view_type, direct, forward),
      frameAngle(fr_angle), shiftX(shx)
{
    if (_isPlan && direct == ELEMENTDIR::ELDOWN){
        normalColor = setting.getDownColorBrushSetting().lineColor;
        normalBrush = setting.getDownColorBrushSetting().paintBrush;
        isNameVisible &= setting.getDownColorBrushSetting().isNameVisible;
        if ( setting.getIsPriorIndividual() ) {
            if ( setting.existIndividualSetting(sectionName) ) {
                normalColor = setting.getIndividualSetting(sectionName).lineColor;
                normalBrush = setting.getIndividualSetting(sectionName).lineColor;
            }
        }
    }

    createShapePath();
    update(boundRect);
}

void ColumnVLineItem::createShapePath()
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

void ColumnVLineItem::createPlanShapePath()
{
    drawShapePath = QPainterPath();
    myLineShape.drawColumnPlanLineShape(drawShapePath, isForward);
    drawShapePath.closeSubpath();

    basePlatePath = QPainterPath();
    baseTextName = myLineShape.baseName;
    if ( !baseTextName.isEmpty() ) {
        myLineShape.drawColumnPlanBaseShape(basePlatePath, isForward);
    }
    basePlatePath.closeSubpath();

    shapePath = QPainterPath();
    myLineShape.drawColumnPlanBoundingShape(shapePath, isForward);
    shapePath.closeSubpath();
    boundRect = shapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
    textPoint = ( myElementDir == ELEMENTDIR::ELCROSS )
            ? QPointF(myLineShape.sizeB / 2.0, -myLineShape.sizeH / 2.0)
            : QPointF(-myLineShape.sizeB / 2.0, -myLineShape.sizeH / 2.0);
    textPoint = textPoint + QPointF(myLineShape.shiftY, myLineShape.shiftZ);
    textName = sectionName;

    if ( !baseTextName.isEmpty() ) {
        baseTextPoint = QPointF(myLineShape.baseSizeB / 2.0, myLineShape.baseSizeD / 2.0);
        baseTextPoint = baseTextPoint + QPointF(myLineShape.shiftY, myLineShape.shiftZ);
    }
}

void ColumnVLineItem::createElevationShapePath()
{
    drawShapePath = QPainterPath();
    myLineShape.drawColumnElevationLineShape(drawShapePath, isForward, frameAngle, shiftX);
    drawShapePath.closeSubpath();

    basePlatePath = QPainterPath();
    baseTextName = myLineShape.baseName;
    if ( !baseTextName.isEmpty() ) {
        myLineShape.drawColumnElevationBaseShape(shapePath, isForward, frameAngle, shiftX);
    }
    basePlatePath.closeSubpath();

    shapePath = QPainterPath();
    myLineShape.drawColumnElevationBoundingShape(shapePath, isForward, frameAngle, shiftX);
    shapePath.closeSubpath();
    boundRect = shapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
    textPoint = ( myElementDir == ELEMENTDIR::ELCROSS ) ? QPointF(myLineShape.sizeB / 2.0, -myLineShape.sizeH / 2.0)
                : QPointF(-myLineShape.sizeB / 2.0, -myLineShape.sizeH / 2.0);
    textPoint = textPoint + QPointF(shiftX, 0.0);
    textName = sectionName;

    if ( !baseTextName.isEmpty() ) {
        baseTextPoint = QPointF(myLineShape.baseSizeB / 2.0, myLineShape.baseSizeD / 2.0);
        baseTextPoint = baseTextPoint + QPointF(shiftX, 0.0);
    }
}

void ColumnVLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    setPainter(painter, option);

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(drawShapePath);

    if ( !baseTextName.isEmpty() ) {
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(basePlatePath);
    }

    if (isNameVisible){
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
        painter->translate(textPoint.x(), -textPoint.y());

        if ( myElementDir == ELEMENTDIR::ELCROSS) {
            painter->setPen(QPen(QBrush(normalColor),5));
            painter->setFont(QFont("Consolas", FontSize(*painter)));
            QRect boundingRect = QFontMetrics(painter->font()).boundingRect(textName);
            painter->drawText(QRect(0.0, 0.0, boundingRect.width(), boundingRect.height()), Qt::AlignLeft | Qt::AlignVCenter,
                              textName);
        } else if (myElementDir == ELEMENTDIR::ELDOWN){
            painter->setPen(QPen(QBrush(normalColor),5));
            painter->setFont(QFont("Consolas", FontSize(*painter) * 0.7));
            QRect boundingRect = QFontMetrics(painter->font()).boundingRect(textName);
            painter->drawText(QRect(-boundingRect.width(), 0.0, boundingRect.width(), boundingRect.height()),
                              Qt::AlignRight | Qt::AlignVCenter, textName);
        }
    }

    if ( baseTextName.isEmpty() || myElementDir != ELEMENTDIR::ELCROSS)
        return;

    painter->translate(-textPoint.x(), textPoint.y());
    painter->translate(baseTextPoint.x(), -baseTextPoint.y());
    painter->setFont(QFont("Consolas", FontSize(*painter) * 0.7));
    QRect boundingRect = QFontMetrics(painter->font()).boundingRect(baseTextName);
    painter->drawText(QRect(0.0, -boundingRect.height(), boundingRect.width(), boundingRect.height()),
                      Qt::AlignLeft | Qt::AlignVCenter, baseTextName);
}

} // namespace post3dapp
