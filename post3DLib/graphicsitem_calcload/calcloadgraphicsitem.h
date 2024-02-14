#ifndef CALCLOADGRAPHICSITEM_H
#define CALCLOADGRAPHICSITEM_H

#include <QBrush>
#include <QGraphicsItem>

#include "define_calculationload2dsettings.h"
#include "define_draw_figure.h"//OUTFIG_TYPE_OPTIONの定義
#include "define_unifieddata.h"//DATATYPEの定義
#include "output_graphicsitem.h"

class QGraphicsView;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

namespace post3dapp{
class CalcLoadGraphicsItem : public QGraphicsItem
{

public:
    CalcLoadGraphicsItem( DATATYPE, const QUuid &, const CalculationLoad2DViewSettings&);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void setPainter(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    DATATYPE dataType() const
    {
        return myDataType;
    }
    QUuid getUuid() const
    {
        return myUuid;
    }

protected:
    const bool useScaledFont;
    const DATATYPE myDataType;
    const QUuid myUuid;
    const CalculationLoad2DViewSettings myViewSettings;

    qreal FontSize(const QPainter&, int) const;
    qreal DpiScale(const QPainter&) const;

    QRectF boundRect;
    QPainterPath shapePath;
    QPainterPath drawShapePath;

    bool isSectionTextVisible;
    bool isLineDirVisible;
    qreal textAngle;    // 度

    QString memberText;
    int memberTextSize;
    QPointF memberTextPos;

    QList<POINT_AND_TEXT> valueTextPos;

    QColor normalColor;
    QColor selectColor;
    QBrush selectBrush;

};


} // namespace post3dapp
#endif
