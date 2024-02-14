#ifndef OUTPUTGRAPHICSITEM_H
#define OUTPUTGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QPainter>

#include "calc_vector3d.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp {
enum class DATATYPE;
enum class STRUCTTYPE;
class FigureTemplate;

enum class OFFSETPOINT {OFFSET_NONE = 0, OFFSET_LEFTTOP, OFFSET_LEFTBOTTOM, OFFSET_LEFTBOTTOM2, OFFSET_RIGHTTOP,
                  OFFSET_RIGHTBOTTOM, OFFSET_RIGHTBOTTOM2, OFFSET_RIGHTBOTTOM3,OFFSET_CENTERTOP,OFFSET_CENTERTOP2, OFFSET_CENTERBOTTOM,OFFSET_CENTERBOTTOM2,
                  OFFSET_LEFTTOP2, OFFSET_RIGHTTOP2, OFFSET_LEFTMIDDLE, OFFSET_RIGHTMIDDLE, OFFSET_RIGHTMIDDLE2, OFFSET_RIGHTMIDDLE3
                 };
enum class ATTENTION_STATUS{
    STATUS_NORMAL=0,STATUS_WARNING,STATUS_FATAL
};

/* 描画位置と文字 */
struct POINT_AND_TEXT {
    explicit POINT_AND_TEXT( const QPointF &_p = QPointF(),
                           qreal _ag = 0.0,
                           const QString &_str = QString(),
                           const QColor &_color = QColor(),
                           OFFSETPOINT _offset = OFFSETPOINT::OFFSET_NONE,
                           qreal max = 999999.0,
                             int _size = 10)
        : pos(_p), angle(_ag), vtext(_str), color(_color), offset(_offset),MaxLength(max), size(_size) { }

    QPointF pos;
    qreal angle;   // rad
    QString vtext;
    QColor color;
    OFFSETPOINT offset;
    qreal MaxLength;
    int size;
};
/* 図形と色 */
struct FIGURE_AND_COLOR {
    explicit FIGURE_AND_COLOR( const QPolygonF &_fig = QPolygonF(),
                               const QPen &_pen = QPen(),
                               const QBrush &_brush = QBrush(),
                               const Qt::FillRule &_rule = Qt::OddEvenFill
            )
        : figure(_fig), pen(_pen),brush(_brush),  rule(_rule){ }

    QPolygonF figure;
    QPen pen;
    QBrush brush;
    Qt::FillRule rule;
};
/* パスとペン設定 */
struct PATH_AND_PEN {
    explicit PATH_AND_PEN( const QPainterPath &_path = QPainterPath(),
                               const QPen &_pen = QPen(),
                               const QBrush &_brush = QBrush(),
                               const Qt::FillRule &_rule = Qt::OddEvenFill
            )
        : path(_path), pen(_pen), brush(_brush), rule(_rule){ }

    QPainterPath path;
    QPen pen;
    QBrush brush;
    Qt::FillRule rule;
};

class POST3D_CLASS_DLLSPEC OutputGraphicsItem : public QGraphicsItem
{

public:

    OutputGraphicsItem(FigureTemplate*, DATATYPE,STRUCTTYPE, const QUuid &);

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setPainter(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void setDrawingStatusDirect() {}

    DATATYPE dataType() const
    {
        return myDataType;
    }
    STRUCTTYPE structureType() const
    {
        return myStructureType;
    }
    QUuid getUuid() const
    {
        return myUuid;
    }
    QColor stringColor(ATTENTION_STATUS);
    virtual QColor itemColor(ATTENTION_STATUS);
    QString getShortTip()const;

//    QScopedPointer* d_ptr;
    static bool isZero(qreal value){const qreal eps = 0.00000001;return (qAbs(value) < eps); }

protected:
    const bool useScaledFont;
    const DATATYPE myDataType;
    const STRUCTTYPE myStructureType;
    const QUuid myUuid;
    ATTENTION_STATUS attentionStatus;

    QRectF boundRect;

    bool isSectionTextVisible;
    bool isLineDirVisible;

    //共通
    qreal textAngle;    // 度
    QPainterPath shapePath;
    QPainterPath drawShapePath;
    QString memberText;
    int memberTextSize;
    int valueTextSize;
    QPointF memberTextPos;

    //固有
    //図形のリストとペン設定
    FigureTemplate *myFigureTemplate;
    QList<POINT_AND_TEXT> OTvaluetext;
    QList<FIGURE_AND_COLOR> OTfigures;
    QList<PATH_AND_PEN> OTpathes;

    QColor normalColor;
    QColor selectColor;
    QBrush selectBrush;

    QColor warningColor;
    QColor fatalColor;
    QColor normalTextColor;
    QColor warningTextColor;
    QColor fatalTextColor;

    VEC3D myPlaneXVector;//紙面X方向
    VEC3D myPlaneVector;//法線ベクトル

    virtual void drawOTShapePath(QPainter *painter);
    virtual void drawOTFigure(QPainter *painter) ;
    virtual void drawOTPath(QPainter *painter) ;
    virtual void drawOTText(QPainter *painter) ;

    qreal FontSize(const QPainter&, int) const;
    qreal DpiScale(const QPainter&) const;


    const qreal M_TAU=2.0*M_PI;
};
} // namespace post3dapp


#endif // OUTPUTGRAPHICSITEM_H
