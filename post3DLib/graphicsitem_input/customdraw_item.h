#ifndef CUSTOMDRAWITEM_H
#define CUSTOMDRAWITEM_H

#include <QBrush>
#include <QGraphicsItem>

#include "define_input2dsettings.h"
#include "define_settings.h"

namespace post3dapp{
enum class ITEMVIEWTYPE { VIEW_PLAN = 0, VIEW_ELEVATION };            // 線材描画　伏図、軸組図
enum class ITEMTYPE { HLINEITEM = 0, VLINEITEM, POINTITEM, HPLANEITEM, VPLANEITEM };

class CustomDrawItem : public QGraphicsItem
{

public:

    CustomDrawItem(const Input2DViewSettings& setting,ELEMENTTYPE etype, const QString &name, ITEMTYPE ltype = ITEMTYPE::HLINEITEM );

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;

    virtual void setPainter(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    ELEMENTTYPE elementType() const
    {
        return myElementType;
    }
    ITEMTYPE itemType() const
    {
        return myItemType;
    }
    void setInputState(bool onoff)
    {
        isInputState = onoff;
        update();
    }

protected:
    const bool useScaledFont;
    ELEMENTSTATE myElementState;

    qreal FontSize(const QPainter&) const;
    qreal DpiScale(const QPainter&) const;

    const ELEMENTTYPE myElementType;
    const ITEMTYPE myItemType;
    bool isInputState;

    QRectF boundRect;
    QPainterPath shapePath;
    QPainterPath drawShapePath;

    QString textName;
    qreal textAngle;
    QPointF textPoint;
    qreal textShift;

    QColor normalColor;
    QColor dummyColor;
    QColor errorColor;
    QColor selectColor;
    QColor inputColor;

    QBrush normalBrush;
    QBrush dummyBrush;
    QBrush errorBrush;
    QBrush selectBrush;
    QBrush inputBrush;

    const QString sectionName;
    bool isNameVisible;
    bool isLineDirVisible;
    int nameSize;
};
} // namespace post3dapp
#endif
