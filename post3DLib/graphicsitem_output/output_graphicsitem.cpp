#include "output_graphicsitem.h"

#include <QtMath>
#include <QDebug>
#include <QPainter>
#include <QPaintEngine>
#include <QStyleOption>

#include "calc_vector2d.h"
#include "figure_template.h"
#include "unified_data.h"
#include "unified_settings.h"

namespace post3dapp{


OutputGraphicsItem::OutputGraphicsItem(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE st, const QUuid &uid)
    :  useScaledFont(fig->useScaledFont()), myDataType(dtype),myStructureType(st), myUuid(uid),myFigureTemplate(fig)
{
    // setCacheMode(NoCache);
    // setAcceptHoverEvents(true);
    qDebug()<<"OutputGraphicsItem";

    normalColor = fig->figureColor(myDataType,myStructureType,ATTENTION_STATUS::STATUS_NORMAL);
    warningColor = fig->figureColor(myDataType,myStructureType,ATTENTION_STATUS::STATUS_WARNING);
    fatalColor = fig->figureColor(myDataType,myStructureType,ATTENTION_STATUS::STATUS_FATAL);
    isSectionTextVisible = fig->nameVisible(myDataType);
    memberTextSize = fig->nameTextSize(myDataType);
    valueTextSize = fig->valueTextSize(myDataType);

    isLineDirVisible = fig->isLineDirVisible();
    selectColor = fig->selectColor(myDataType);
    selectBrush = QBrush(selectColor);

    normalTextColor=fig->valueTextColor(myDataType,ATTENTION_STATUS::STATUS_NORMAL);
    warningTextColor=fig->valueTextColor(myDataType,ATTENTION_STATUS::STATUS_WARNING);
    fatalTextColor=fig->valueTextColor(myDataType,ATTENTION_STATUS::STATUS_FATAL);

    attentionStatus = ATTENTION_STATUS::STATUS_NORMAL;
    //ステータスはITEMを作った段階ではなく、数値計算してから判断したい。
    //選択状態はITEM作成後に確定する
    qDebug()<<"OutputGraphicsItem constructed";
}

QColor OutputGraphicsItem::stringColor(ATTENTION_STATUS st){
    if(st == ATTENTION_STATUS::STATUS_NORMAL){
        return normalTextColor;
    }else if(st == ATTENTION_STATUS::STATUS_WARNING){
        return warningTextColor;
    }else if(st == ATTENTION_STATUS::STATUS_FATAL){
        return fatalTextColor;
    }
    return normalTextColor;

}
QColor OutputGraphicsItem::itemColor(ATTENTION_STATUS st){
//    return Qt::red;
    if(this->isSelected())return selectColor;
    if(st == ATTENTION_STATUS::STATUS_NORMAL){
        return normalColor;
    }else if(st == ATTENTION_STATUS::STATUS_WARNING){
        return warningColor;
    }else if(st == ATTENTION_STATUS::STATUS_FATAL){
        return fatalColor;
    }
    return normalColor;
}

QString OutputGraphicsItem::getShortTip() const
{
    return UnifiedData::dataTypeToJapanese(myDataType);
}


void OutputGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    setPainter(painter,option);
    drawOTShapePath(painter);
    drawOTFigure(painter);
    drawOTPath(painter);
    drawOTText(painter);
}
void OutputGraphicsItem::drawOTShapePath(QPainter *painter){
    //選択状態で色を変えるものはこちら
    QPen tmppen=painter->pen();
    QBrush tmpbrush=painter->brush();

    painter->setPen(QPen(itemColor(attentionStatus),0));
    painter->setBrush(QBrush(itemColor(attentionStatus)));
    painter->drawPath(drawShapePath);

    painter->setPen(tmppen);
    painter->setBrush(tmpbrush);
};

void OutputGraphicsItem::drawOTFigure(QPainter* painter)
{
    QPen tmppen=painter->pen();
    QBrush tmpbrush=painter->brush();

    Q_FOREACH(FIGURE_AND_COLOR fig,OTfigures){

        if(isSelected()){
            painter->setPen(QPen(itemColor(attentionStatus),fig.pen.width()));
            if(fig.brush==Qt::NoBrush){
                painter->setBrush(Qt::NoBrush);
            }else{
                painter->setBrush(QBrush(itemColor(attentionStatus)));
            }
        }else{
        painter->setPen(fig.pen);
        painter->setBrush(fig.brush);
        }
        painter->drawPolygon(fig.figure,fig.rule);
        painter->setPen(tmppen);
        painter->setBrush(tmpbrush);

    }
}
void OutputGraphicsItem::drawOTPath(QPainter* painter)
{
    QPen tmppen=painter->pen();
    QBrush tmpbrush=painter->brush();

    Q_FOREACH(PATH_AND_PEN path,OTpathes){
        if(isSelected()){
            painter->setPen(QPen(itemColor(attentionStatus),path.pen.width()));
            if(path.brush==Qt::NoBrush){
                painter->setBrush(Qt::NoBrush);
            }else{
                painter->setBrush(QBrush(itemColor(attentionStatus)));
            }
        }else{
            painter->setPen(path.pen);
            painter->setBrush(path.brush);
        }
        painter->drawPath(path.path);
        painter->setPen(tmppen);
        painter->setBrush(tmpbrush);

    }

}
void OutputGraphicsItem::drawOTText(QPainter* painter)
{
    //scalePowerが拡縮へのフォントサイズの感度、scaledFontのpst.sizeにかかっている数がフォントサイズへの感度、それに足されている数が最小フォントサイズ

    /*
    qreal scalePower = myFigureTemplate->useScaledFont() ? 0.6 : 1.0;
    // scale = 0.01でスケールフォントサイズと絶対フォントサイズがそろうように調整
    qreal syuuseikeisuu = qPow(0.01,scalePower) / qPow(0.01,0.6);


    qreal dpiScale = 72.0 / painter->device()->logicalDpiX();
    qreal scale = painter->transform().m11();
    //実際の拡大率よりテキストのスケーリングをマイルドにしている。0.0乗でスケールに比例、1.0乗でスケールしない
//    qreal factor = 1.0 / qPow(scale, 0.75);
    qreal factor = syuuseikeisuu / qPow(scale, scalePower);
    */
    QFont orgfont = painter->font();
    QPen orgpen = painter->pen();

    //drawText
    QTransform orgtrans = painter->transform();


//reactfigのとき、最大値を計算
//    qreal maxTextWidth = 0.0;
//    Q_FOREACH ( POINT_AND_TEXT pst, OTvaluetext ) {
//        qreal textwidth = painter->fontMetrics().width(pst.vtext);
//        if (maxTextWidth < textwidth)maxTextWidth = textwidth;

//    }
    Q_FOREACH ( POINT_AND_TEXT pst, OTvaluetext ) {
//        painter->setPen(Qt::blue);
        painter->setPen(pst.color);
        painter->setTransform(orgtrans);
        painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);

        //計算書などは変な換算せず直接フォントサイズ指定したい。形状入力→モデル座標系のフォントサイズ。任意項目の出力→画面座標系のフォント。2mm固定？
        QFont scaledfont = QFont("Consolas", FontSize(*painter, pst.size));
        painter->setFont(scaledfont);

        //reactFigのとき
//        qreal textwidth = maxTextWidth;
        QRect boundingRect = QFontMetrics(painter->font()).boundingRect(pst.vtext);
        qreal xmargin = boundingRect.width() / 4.0; // 交点からの距離
        qreal ymargin = boundingRect.height() / 8.0; // 交点からの距離
        qreal ypadding = 10 * DpiScale(*painter); //高さ方向数字同士の空き
//        qreal ypadding = 10;
        painter->translate(pst.pos.x(), pst.pos.y());
        painter->rotate(-pst.angle * 180.0 / M_PI);

        if(pst.vtext == "-0")pst.vtext = "0";

        switch (pst.offset) {
        case (OFFSETPOINT::OFFSET_LEFTBOTTOM):
            painter->drawText(QRectF(-boundingRect.width() -  xmargin, ymargin, boundingRect.width(), boundingRect.height()),
                              Qt::AlignRight | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_LEFTBOTTOM2):
            painter->drawText(QRectF(-boundingRect.width() -  xmargin, ymargin +ypadding+ boundingRect.height(), boundingRect.width(), boundingRect.height()),
                              Qt::AlignRight | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_LEFTTOP):
            painter->drawText(QRectF(-boundingRect.width() -  xmargin, -ymargin -boundingRect.width(), boundingRect.width(), boundingRect.height()),
                              Qt::AlignRight | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_LEFTTOP2):
            painter->drawText(QRectF(-boundingRect.width() -  xmargin, -ymargin -ypadding-2 * boundingRect.height() , boundingRect.width() ,boundingRect.height()),
                              Qt::AlignRight | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_RIGHTBOTTOM):
            painter->drawText(QRectF(0.0 + xmargin,ymargin, boundingRect.width(), boundingRect.height()),
                              Qt::AlignLeft | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_RIGHTBOTTOM2):
            painter->drawText(QRectF(0.0 + xmargin, ymargin + ypadding+boundingRect.height(), boundingRect.width(), boundingRect.height()),
                              Qt::AlignLeft | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_RIGHTBOTTOM3):
            painter->drawText(QRectF(0.0 + xmargin, ymargin + 2.0*ypadding+ 2*boundingRect.height(), boundingRect.width(), boundingRect.height()),
                              Qt::AlignLeft | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_RIGHTTOP):
            painter->drawText(QRectF(0.0 + xmargin,-ymargin -boundingRect.height(), boundingRect.width(), boundingRect.height()),
                              Qt::AlignLeft | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_RIGHTTOP2):
            painter->drawText(QRectF(0.0 + xmargin, -ymargin -ypadding -2 * boundingRect.height(), boundingRect.width(), boundingRect.height()),
                              Qt::AlignLeft | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_CENTERTOP):
            painter->drawText(QRectF(-0.5 * boundingRect.width(), -ymargin - boundingRect.height(), boundingRect.width(), boundingRect.height()),
                              Qt::AlignHCenter | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_CENTERTOP2):
            painter->drawText(QRectF(-0.5 * boundingRect.width() , -ymargin-ypadding-2 * boundingRect.height() , boundingRect.width(), boundingRect.height()),
                              Qt::AlignHCenter | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_CENTERBOTTOM):
            painter->drawText(QRectF(-0.5 * boundingRect.width() , ymargin, boundingRect.width(), boundingRect.height()),
                              Qt::AlignHCenter | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_CENTERBOTTOM2):
            painter->drawText(QRectF(-0.5 * boundingRect.width(), ymargin + ypadding+boundingRect.height() , boundingRect.width(), boundingRect.height()),
                              Qt::AlignHCenter | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_RIGHTMIDDLE):
            painter->drawText(QRectF(0.0 + xmargin, -0.5 * boundingRect.height(), boundingRect.width() , boundingRect.height()),
                              Qt::AlignLeft | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_RIGHTMIDDLE2)://i
            painter->drawText(QRectF(0.0 + xmargin, ypadding+0.5 * boundingRect.height(), boundingRect.width(), boundingRect.height()),
                              Qt::AlignLeft | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_RIGHTMIDDLE3)://j
            painter->drawText(QRectF(0.0 + xmargin, -ypadding-1.5 * boundingRect.height(), boundingRect.width(), boundingRect.height()),
                              Qt::AlignLeft | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSETPOINT::OFFSET_LEFTMIDDLE):
            painter->drawText(QRectF(-boundingRect.width() -  xmargin, -0.5 * boundingRect.height(), boundingRect.width(),  boundingRect.height()),
                              Qt::AlignRight | Qt::AlignVCenter, pst.vtext);
            break;
        default:
            painter->drawText(QRectF(-0.5 * boundingRect.width(), -0.5 * boundingRect.height(), boundingRect.width(), boundingRect.height()),
                              Qt::AlignHCenter | Qt::AlignVCenter, pst.vtext);
            break;
        }

    }
    painter->setPen(orgpen);
    painter->setFont(orgfont);
    painter->setTransform(orgtrans);

}

QRectF OutputGraphicsItem::boundingRect() const
{
    return boundRect;
}

QPainterPath OutputGraphicsItem::shape() const
{
    return shapePath;
}

void OutputGraphicsItem::setPainter(QPainter *painter, const QStyleOptionGraphicsItem *option)
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

qreal OutputGraphicsItem::FontSize(const QPainter &painter, int fontSize) const
{
    return useScaledFont ? (fontSize * 4 + 20) / qPow(painter.transform().m11(), 0.75) * DpiScale(painter)
                             : fontSize * 4 / painter.transform().m11();
}

qreal OutputGraphicsItem::DpiScale(const QPainter &painter) const
{
    return painter.device()->logicalDpiX() / 72.0;
}

} // namespace post3dapp
