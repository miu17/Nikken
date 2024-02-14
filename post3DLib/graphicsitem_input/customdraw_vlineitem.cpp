#include "customdraw_vlineitem.h"

#include <QDebug>
#include <QPainter>
#include <qmath.h>

#include "calc_vector2d.h"
#include "unified_data.h"
namespace post3dapp{
CustomDrawVLineItem::CustomDrawVLineItem(bool _isPlan, const Input2DViewSettings& setting,ELEMENTTYPE type, QPointF p0, const VLINESHAPE &shape,
                                          ITEMVIEWTYPE view_type, ELEMENTDIR direct, bool forward)
    : CustomDrawItem(setting, type, shape.sectionName,ITEMTYPE::VLINEITEM),
      myPosition(p0),
      myLineShape(shape),
      myItemView(view_type),
      myElementDir(direct),
      isForward(forward)
{
    if (!_isPlan){
        normalColor = setting.getCrossElementView().lineColor;
        normalBrush = setting.getCrossElementView().paintBrush;
        isNameVisible &= setting.getCrossElementView().isNameVisible;
    }

    if ( setting.getIsPriorIndividual() ) {
        if ( setting.existIndividualSetting(sectionName) ) {
            normalColor = setting.getIndividualSetting(sectionName).lineColor;
            normalBrush = setting.getIndividualSetting(sectionName).lineColor;
        }
    }

}

void CustomDrawVLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *)
{
    if (myElementDir == ELEMENTDIR::ELINPLANE || myElementDir == ELEMENTDIR::ELDOWN)
        return;

    setPainter(painter, option);

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(drawShapePath);

    if ( !isNameVisible ) return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->translate(textPoint.x(), -textPoint.y());

    if ( myElementDir == ELEMENTDIR::ELCROSS) {
        painter->setFont(QFont("Consolas", FontSize(*painter)));
        QRect boundingRect = QFontMetrics(painter->font()).boundingRect(textName);
        painter->drawText(QRect(0.0, 0.0, boundingRect.width(), boundingRect.height()), Qt::AlignLeft | Qt::AlignVCenter,
                          textName);
    }
}

void CustomDrawVLineItem::createErrorShape()
{
    myElementState = ELERROR;
    drawShapePath = QPainterPath();
    drawShapePath.addRect(-100.0, -100.0, 200.0, 200.0);
    drawShapePath.closeSubpath();
    shapePath = drawShapePath;
    boundRect = drawShapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
    textPoint = ( myElementDir == ELEMENTDIR::ELCROSS ) ? QPointF(100.0, -100.0)
                : QPointF(-100.0, -100.0);
    textName = "ERROR(" + sectionName + ")";
}

void CustomDrawVLineItem::createDummyShape()
{
    myElementState = ELDUMMY;
    drawShapePath = QPainterPath();
    if (myElementType == ELBRACE){
        drawShapePath.addEllipse(QPointF(), 100.0, 100.0);
    }else if(myElementType == ELISO){
        drawShapePath.addEllipse(QPointF(), 400.0, 400.0);
    }else{
        drawShapePath.addRect(-100.0, -100.0, 200.0, 200.0);
    }
    drawShapePath.closeSubpath();
    shapePath = drawShapePath;
    boundRect = drawShapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
    textPoint = ( myElementDir == ELEMENTDIR::ELCROSS ) ? QPointF(100.0, -100.0)
                : QPointF(-100.0, -100.0);
    if ( myElementType == ELCOLUMN ) {
        textName = "(dummyC)";
    } else if ( myElementType == ELGIRDER ) {
        textName = "(dummyG)";
    } else if ( myElementType == ELBRACE ) {
        textName = "(dummyV)";
    } else if ( myElementType == ELBEAM ) {
        textName = "(dummyB)";
    } else if (myElementType == ELISO) {
        textName = "(dummyI)";
    } else if (myElementType == ELDAMPER){
        textName = "(dummyD)";
    }
}

} // namespace post3dapp
