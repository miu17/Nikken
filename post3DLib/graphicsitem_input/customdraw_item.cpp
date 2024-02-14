#include "customdraw_item.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "unified_settings.h"
namespace post3dapp{
CustomDrawItem::CustomDrawItem(const Input2DViewSettings& setting, ELEMENTTYPE etype, const QString &name,ITEMTYPE ltype )
    : useScaledFont(setting.UseScaledFont()), myElementType(etype), sectionName(name), myItemType(ltype)
{
    setCacheMode(NoCache);
    setAcceptHoverEvents(true);

    myElementState = ELNORMAL;
    isInputState = false;

    normalColor = setting.getElementView(myElementType).lineColor;
    dummyColor = setting.getElementStateView(ELDUMMY).lineColor;
    errorColor = setting.getElementStateView(ELERROR).lineColor;
    selectColor = setting.getElementStateView(ELSELECT).lineColor;
    inputColor = setting.getElementStateView(ELINPUT).lineColor;

    normalBrush = setting.getElementView(myElementType).paintBrush;
    dummyBrush = setting.getElementStateView(ELDUMMY).paintBrush;
    errorBrush = setting.getElementStateView(ELERROR).paintBrush;
    selectBrush = setting.getElementStateView(ELSELECT).paintBrush;
    inputBrush = setting.getElementStateView(ELINPUT).paintBrush;

    isNameVisible = setting.getElementView(myElementType).isNameVisible;
    nameSize = setting.getElementNameSize(myElementType);
    isLineDirVisible = setting.getIsLineDirVisible();

    if ( myElementType == ELJOINT || sectionName.isEmpty() ) return;
    if ( setting.getIsPriorIndividual() ) {
        if ( setting.existIndividualSetting(sectionName) ) {
            normalColor = setting.getIndividualSetting(sectionName).lineColor;
            normalBrush = setting.getIndividualSetting(sectionName).paintBrush;
        }
    }
}

QRectF CustomDrawItem::boundingRect() const
{
    return boundRect;
}

QPainterPath CustomDrawItem::shape() const
{
    return shapePath;
}

void CustomDrawItem::setPainter(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    QColor orgColor = normalColor;
    QBrush orgBrush = normalBrush;

    switch ( myElementState ) {
    case ELDUMMY:
        orgColor = dummyColor;
        orgBrush = dummyBrush;
        break;
    case ELERROR:
        orgColor = errorColor;
        orgBrush = errorBrush;
        break;
    default:
        break;
    }

    if ( isInputState ) {
        orgColor = inputColor;
        orgBrush = inputBrush;
    }

    if ( !isInputState && option->state & QStyle::State_Selected ) {
        orgColor = selectColor;
        orgBrush = selectBrush;
        if ( option->state & QStyle::State_MouseOver ) orgColor = selectColor.lighter(150);
    }

    QPen pen(orgColor);
    pen.setWidth(0);
    ( myElementState == ELDUMMY ) ? pen.setStyle(Qt::DashLine) : pen.setStyle(Qt::SolidLine) ;
    painter->setPen(pen);
    painter->setBrush(orgBrush);
}

qreal CustomDrawItem::FontSize(const QPainter &painter) const
{
    return useScaledFont ? (nameSize * 4 + 20) / qPow(painter.transform().m11(), 0.75) * DpiScale(painter)
                             : nameSize * 4 / painter.transform().m11();
}

qreal CustomDrawItem::DpiScale(const QPainter &painter) const
{
    return painter.device()->logicalDpiX() / 72.0;
}

} // namespace post3dapp

