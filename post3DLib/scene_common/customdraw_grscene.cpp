#include "customdraw_grscene.h"

#include <QPainter>

#include "base_background.h"
#include "define_draw_figure.h"
#include "draw_figure.h"

namespace post3dapp{

CustomDrawGrScene::CustomDrawGrScene(bool _syncSetting, QObject *parent)
    :QGraphicsScene(parent), syncSetting(_syncSetting)
{
}
void CustomDrawGrScene::exportRenderBackGround(QPainter* painter,const int tate,const int yoko,const FIGURE_SIZE_RESULT& size,const FIGURE_PROPERTY& pos){
    const int dpi = painter->device()->logicalDpiX();
    QRectF targetRect = size.getOuterPixcelRect(pos, dpi);
    painter->setClipping(false);
    painter->setClipRect(targetRect, Qt::IntersectClip);
    qDebug()<<"drawMarker"<<(int)pos.drawMarker;
    if(pos.drawMarker==DRAWMARKER::NotDisplay
            || (pos.drawMarker==DRAWMARKER::IfDivided && pos.divide.tate == 1&& pos.divide.yoko == 1)){
        getBackGround()->drawWholeWithScale(painter,size,pos);
    }else if(pos.drawMarker==DRAWMARKER::Always
             || (pos.drawMarker==DRAWMARKER::IfDivided && pos.divide.tate > 1&& pos.divide.yoko > 1)) {
        getBackGround()->drawPart(painter, tate,yoko,size,pos,true,true);
    }else if(pos.drawMarker==DRAWMARKER::IfDivided && pos.divide.tate == 1 && pos.divide.yoko > 1) {
        getBackGround()->drawPart(painter, tate,yoko,size,pos,false,true);
    }else if(pos.drawMarker==DRAWMARKER::IfDivided && pos.divide.tate > 1 && pos.divide.yoko == 1) {
        getBackGround()->drawPart(painter, tate,yoko,size,pos,true,false);
    }
}

void CustomDrawGrScene::exportRenderItems(QPainter *painter, const int tate, const int yoko, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &pos)
{
    const int dpi = painter->device()->logicalDpiX();
    QRectF targetRect = size.getInnerPixcelRect(pos, dpi);
    painter->setClipping(false);
    QTransform trans(1, 0, 0, 1, 0, 0);
    QTransform save=painter->transform();
    painter->setTransform(trans, false);
    painter->setClipRect(targetRect, Qt::IntersectClip);

    painter->translate(targetRect.left(), targetRect.top());
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->translate(-targetRect.left(), -targetRect.top());
    painter->translate(0.0, -targetRect.height());
    render(painter,targetRect,pos.sourceRect(tate,yoko),Qt::KeepAspectRatio);

    painter->setTransform(save, false);
    return;

}
void CustomDrawGrScene::exportRenderPaperItems(QPainter* painter, const int tate,const int yoko,const FIGURE_SIZE_RESULT& size, const FIGURE_PROPERTY& prop, const FIGURE_TYPE_PROPERTY& type){
    const QFont origfont = painter->font();
    const QPen origpen = painter->pen();

    const QPointF offset(2, 2);
    const int dpi = painter->device()->logicalDpiX();
    const QRectF targetRect = size.getOuterPixcelRect(prop, dpi);
    const QRectF innerTargetRect = size.getInnerPixcelRect(prop, dpi);
    qreal markerSize = dpi * prop.markerSize / DrawFigure::inchToMm;
    qreal headerSize = dpi * prop.headerSize / DrawFigure::inchToMm;
    qreal footerSize = dpi * prop.footerSize / DrawFigure::inchToMm;

    //QTransform save=painter->transform();
    painter->save();
    painter->setFont(QFont("Helvetica [Cronyx]",8));
    QTransform trans(1, 0, 0, 1, 0, 0);
    painter->setTransform(trans, false);

    //凡例

    //追加データ

    //図面枠
    if(prop.drawWaku){
        painter->save();
        QPen pen;
        pen.setColor(Qt::black);
        pen.setWidth(0);
        painter->setPen(pen);
        painter->drawRect(innerTargetRect);
        painter->restore();
    }

    //ヘッダー
    if(prop.drawHeader){
        //〇〇図　何とか通り　縦〇/2 x 横〇/3
        QString header1 = type.title();
        QString header2 =QString();

        bool hasHeader2 = (prop.divide.tate != 1 || prop.divide.yoko != 1);
        if(hasHeader2){
            header2 += u8"　縦: " + QString::number(tate+1) + u8" / "+ QString::number(prop.divide.tate);
            header2 += u8"　横: " + QString::number(yoko+1) + u8" / "+ QString::number(prop.divide.yoko);
        }
        QFontMetrics fm(painter->font());
        qreal sx = (targetRect.width() - 2 * offset.x()) / fm.width(header1 + header2);
        qreal sy = (headerSize - 2 * offset.y()) / fm.height();
        qreal scale = qMin(sx,sy);

//        QRectF newRect = QRectF(targetRect.topLeft() - QPointF(0, fm.height() * scale) + offset,
//                                targetRect.topLeft() + QPointF(fm.width(header1) * scale, 0) - offset);
        QRectF newRect = QRectF(targetRect.topLeft()  + offset,
                                targetRect.topLeft() + QPointF(fm.width(header1) * scale, fm.height() * scale) - offset);
        painter->save();
        painter->translate(newRect.center());
        painter->scale(scale, scale);
        painter->translate(-newRect.center());
        painter->drawText(newRect, Qt::AlignHCenter | Qt::AlignVCenter| Qt::TextDontClip, header1);
        painter->restore();

        if (hasHeader2){
            QRectF newRect2 = QRectF(targetRect.topRight() - QPointF(fm.width(header2)*scale,0.0) + offset,
                                     targetRect.topRight() + QPointF(0.0, fm.height() * scale) - offset);
//            QRectF newRect2 = QRectF(targetRect.topRight() - QPointF(fm.width(header2)*scale, fm.height()*scale) + offset,
//                                     targetRect.topRight() - offset);
            painter->save();
            painter->translate(newRect2.center());
            painter->scale(scale, scale);
            painter->translate(-newRect2.center());
            painter->drawText(newRect2, Qt::AlignHCenter | Qt::AlignVCenter| Qt::TextDontClip, header2);
            painter->restore();
        }
    }

    //フッター
    if(prop.drawFooter){
        //.........pstn
        QString footer = type.filename;
        QFontMetrics fm(painter->font());
        qreal sx = (targetRect.width() - 2 * offset.x()) / fm.width(footer);
        qreal sy = (footerSize - 2 * offset.y()) / fm.height();
        qreal scale = qMin(sx,sy);

        QRectF newRect = QRectF(targetRect.bottomLeft() - QPointF(0.0, fm.height()*scale)+ offset,
                                targetRect.bottomLeft()  + QPointF(fm.width(footer)*scale, 0.0)- offset);
//        QRectF newRect = QRectF(targetRect.bottomLeft() + offset,
//                                targetRect.bottomLeft() + QPointF(fm.width(footer)*scale, fm.height()*scale) - offset);
        painter->save();
        painter->translate(newRect.center());
        painter->scale(scale, scale);
        painter->translate(-newRect.center());
        painter->drawText(newRect, Qt::AlignHCenter | Qt::AlignVCenter| Qt::TextDontClip, footer);
        painter->restore();
    }

    //painter->setTransform(save, false);
    painter->restore();
    return;

}
}
