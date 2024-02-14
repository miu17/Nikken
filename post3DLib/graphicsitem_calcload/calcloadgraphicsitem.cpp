#include "calcloadgraphicsitem.h"

#include <QDebug>
#include <QPaintEngine>
#include <QPainter>
#include <QStyleOption>

#include "unified_settings.h"
namespace post3dapp{
CalcLoadGraphicsItem::CalcLoadGraphicsItem(DATATYPE dtype, const QUuid &uid, const CalculationLoad2DViewSettings &setting)
    : useScaledFont(setting.UseScaledFont()), myDataType(dtype), myUuid(uid), myViewSettings(setting)
{
    setCacheMode(NoCache);
    setAcceptHoverEvents(true);

    if ( myDataType == DATATYPE::TPJOINT ) {
        normalColor = myViewSettings.getJointColor();
        isSectionTextVisible = myViewSettings.getIsJointStringVisible();
        memberTextSize = myViewSettings.getJointSize();
    } else if ( myDataType == DATATYPE::TPSLAB || myDataType == DATATYPE::TPWALL || myDataType == DATATYPE::TPUWALL ) {
        normalColor = myViewSettings.getPlaneColor();
        isSectionTextVisible = myViewSettings.getIsPlaneStringVisible();
        memberTextSize = myViewSettings.getPlaneSize();
    } else {
        normalColor = myViewSettings.getLineColor();
        isSectionTextVisible = myViewSettings.getIsLineStringVisible();
        memberTextSize = myViewSettings.getLineSize();
    }

    isLineDirVisible = myViewSettings.getIsDirectionVisible();
    selectColor = myViewSettings.getSelectColor();
    selectBrush = QBrush(selectColor);
}

QRectF CalcLoadGraphicsItem::boundingRect() const
{
    return boundRect;
}

QPainterPath CalcLoadGraphicsItem::shape() const
{
    return shapePath;
}

void CalcLoadGraphicsItem::setPainter(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    QColor orgColor = normalColor;
    QBrush orgBrush = QBrush(orgColor);

    if ( option->state & QStyle::State_Selected ) {
        orgColor = selectColor;
        orgBrush = selectBrush;
        if ( option->state & QStyle::State_MouseOver ) orgColor = selectColor.lighter(150);
    }

    //svgのときとそうでないときでペンを変える
    if (painter->paintEngine()->type() == QPaintEngine::SVG) {
        const QPen pen(orgColor, 0, Qt::SolidLine);
        painter->setPen(pen);
        painter->setBrush(orgBrush);

    } else {
        QPen pen(orgColor);
        pen.setWidth(0);
        painter->setPen(pen);
        painter->setBrush(orgBrush);
    }
}

qreal CalcLoadGraphicsItem::FontSize(const QPainter &painter, int fontSize) const
{
    return useScaledFont ? (fontSize * 4 + 20) / qPow(painter.transform().m11(), 0.75) * DpiScale(painter)
                             : fontSize * 4 / painter.transform().m11();
}

qreal CalcLoadGraphicsItem::DpiScale(const QPainter &painter) const
{
    return painter.device()->logicalDpiX() / 72.0;
}

} // namespace post3dapp
