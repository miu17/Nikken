#include "base_background.h"

#include <QtGui>
#include <QObject>

#include "calc_vector2d.h"
#include "calcload_background_floor.h"
#include "define_draw_figure.h"
#include "fixed_data.h"
#include "unified_data.h"

namespace post3dapp {


BaseBackGround::BaseBackGround(bool _useScaledFont)
    : useScaledFont(_useScaledFont)
{
    scaleTextSize = 3;
    floorTextVisible = true;
    gridTextVisible = true;
    drawingCross = false;
    coloringSpan = false;

}
void BaseBackGround::setColor(const QColor &col1, const QColor &col2)
{
    qDebug()<<"baseBackGroundFloor setColor" << col1<<col2;
    defColor = col1;
    curColor = col2;
}

void BaseBackGround::setScaleTextSize(int size)
{
    scaleTextSize = size;
}

void BaseBackGround::setScaleTextVisible(bool _FloorTextVisible, bool _GridTextVisible)
{
    floorTextVisible = _FloorTextVisible;
    gridTextVisible = _GridTextVisible;
}

void BaseBackGround::setScaleAndUpSideDown(QPainter *painter,const int tate,const int yoko,const FIGURE_SIZE_RESULT& size,const FIGURE_PROPERTY& pos)
{
    const int dpi = painter->device()->logicalDpiX();
    QRectF targetRect = size.getInnerPixcelRect(pos, dpi);
    QRectF sourceRect = pos.sourceRect(tate,yoko);
    //〇がついてきていない
    painter->setClipRect(targetRect, Qt::IntersectClip);
    painter->translate(targetRect.left(), targetRect.top());
    painter->scale(targetRect.width()/sourceRect.width(),targetRect.height()/sourceRect.height());
    painter->translate(-sourceRect.left(), sourceRect.bottom());
}

qreal BaseBackGround::FontSize(const QPainter &painter, int fontSize) const
{
    return useScaledFont ? (fontSize * 4 + 20) / qPow(painter.transform().m11(), 0.75) * DpiScale(painter)
                             : (fontSize * 4) / painter.transform().m11();
}

qreal BaseBackGround::DpiScale(const QPainter &painter) const
{
    return painter.device()->logicalDpiX() / 72.0;
}



BaseBackGroundFloor::BaseBackGroundFloor(bool _useScaledFont, UnifiedData* data)
    : BaseBackGround(_useScaledFont)
{
    qDebug()<<"baseBackGroundFloor init";
    myData = data;
    myFloorFrameID = QUuid();
    defColor = Qt::black;
    curColor = Qt::blue;
}
QRectF BaseBackGroundFloor::boundingRect()const
{
    qreal xmin=9999999.9;
    qreal xmax=-9999999.9;
    qreal ymin=9999999.9;
    qreal ymax=-9999999.9;

    Q_FOREACH (FRAMEPOINTSVALUE fp, drawFrameList) {
        for(int i=0;i<fp.pointList.count();i++){
            QPointF pt = fp.pointList.at(i);
            if(xmin>pt.x())xmin = pt.x();
            if(xmax<pt.x())xmax = pt.x();
            if(ymin>pt.y())ymin = pt.y();
            if(ymax<pt.y())ymax = pt.y();
        }
        if(xmin>fp.namePoint.x())xmin=fp.namePoint.x();
        if(xmax<fp.namePoint.x())xmax=fp.namePoint.x();
        if(ymin>fp.namePoint.y())ymin=fp.namePoint.y();
        if(ymax<fp.namePoint.y())ymax=fp.namePoint.y();
    }
    if(xmax > xmin && ymax>ymin){
        qreal namePointRadius = 1000.0;
        qreal toMm = 1000.0;
        return QRectF(toMm*xmin-namePointRadius,
                      toMm*ymin-namePointRadius,
                      toMm*(xmax-xmin)+2*namePointRadius,
                      toMm*(ymax-ymin)+2*namePointRadius);
    }

    return QRectF();
}
QRectF BaseBackGroundFrame::boundingRect()const
{
    qreal xmin=9999999.9;
    qreal xmax=-9999999.9;
    qreal ymin=9999999.9;
    qreal ymax=-9999999.9;
    Q_FOREACH (QUuid fl_id, drawFloorIDList) {


        qDebug()<<"drawFloorIds"<<drawFloorIDList;
        // 通りの定義点
        QList<GlobalPoint> def_plist = definePointList.value(fl_id);
        for(int i=0;i<def_plist.count();i++){
            QPointF pt = def_plist.at(i).getElvQPointF();
            if(xmin>pt.x())xmin = pt.x();
            if(xmax<pt.x())xmax = pt.x();
            if(ymin>pt.y())ymin = pt.y();
            if(ymax<pt.y())ymax = pt.y();
        }
    }


    //    }
    if(xmax > xmin && ymax > ymin ){
        qreal toMm = 1000.0;
        return QRectF(toMm*xmin,
                      toMm*ymin-8000.0,
                      toMm*(xmax-xmin)+6000.0,
                      toMm*(ymax-ymin)+10000.0);
    }
    return QRectF();
}
void BaseBackGroundFloor::resetFramePoints(const QUuid &fl_id)
{
    clearDrawingData();
    myFloorFrameID = fl_id;
    if ( myFloorFrameID.isNull() ) return;
    drawFrameList = myData->getFrameValuesOfFloor(myFloorFrameID);
    if ( drawingCross ) {
        myData->calcCrossPoints(myFloorFrameID, drawFrameList, definePointList,
                                crossPointList);
    }
}

void BaseBackGroundFloor::resetFramePoints(const QList<FRAMEPOINTSVALUE> &plist)
{
    clearDrawingData();
    drawFrameList = plist;
}

QList<GLOBALLINE> BaseBackGroundFloor::calcBatchColumnLines()
{
    QUuid fl_id = myData->getUpperFloorID(myFloorFrameID);
    if ( fl_id.isNull() ) return QList<GLOBALLINE>();
    QList<FRAMEPOINTSVALUE> plist = myData->getFrameValuesOfFloor(fl_id);
    QList<GlobalPoint> d_pointList;
    QList<GlobalPoint> c_pointList;
    myData->calcCrossPoints(fl_id, plist, d_pointList, c_pointList);

    QList<GLOBALLINE> g_lines;
    Q_FOREACH (GlobalPoint gp, crossPointList) {
        QList<QUuid> fr_list = gp.getAttachedFrameList();
        int icount = 0;
        GlobalPoint gp2;
        Q_FOREACH (GlobalPoint gp1, c_pointList) {
            QList<QUuid> fr1_list = gp1.getAttachedFrameList();
            if ( fr1_list.count() < fr_list.count() ) continue;
            bool error = false;
            Q_FOREACH (QUuid id, fr_list) {
                if ( !fr1_list.contains(id) ) {
                    error = true;
                    break;
                }
            }
            if ( error ) continue;
            icount++;
            gp2 = gp1;
        }
        if ( icount > 1 ) continue;
        g_lines.append(GLOBALLINE(gp, gp2));
    }
    return g_lines;
}


void BaseBackGround::setDrawCrossMode(bool onoff)
{
    drawingCross = onoff;
}

void BaseBackGround::setSpanColorMode(bool onoff)
{
    coloringSpan = onoff;
}

void BaseBackGroundFloor::setCurrentFrame(const QList<QUuid> &idlist)
{
    currentFrames = idlist;
}

void BaseBackGroundFloor::clearCurrentFrame()
{
    currentFrames.clear();
}

void BaseBackGroundFloor::setCurrentGID(const QUuid &gid)
{
    currentGID = gid;
}

void BaseBackGroundFloor::clearCurrentGID()
{
    currentGID = QUuid();
}

void BaseBackGroundFloor::clearDrawingData()
{
    myFloorFrameID = QUuid();
    drawFrameList.clear();
    crossPointList.clear();
    definePointList.clear();
}

void BaseBackGroundFloor::completeGlobalXYZ(GlobalPoint &gpoint)
{
    QPointF pp = gpoint.getElvQPointF();
    gpoint.setX(pp.x());
    gpoint.setY(pp.y());
    gpoint.setZ(myData->getFloorHeight(myFloorFrameID) + gpoint.elvZ());
    gpoint.appendAttachedFloor(myFloorFrameID);

    Q_FOREACH (FRAMEPOINTSVALUE fp, drawFrameList) {
        if ( fp.pointList.isEmpty() ) continue;
        bool online = false;
        for (int i = 0; i < fp.pointList.count() - 1; i++) {
            if ( CalcVector2D::between(fp.pointList.at(i), fp.pointList.at(i + 1), pp) ) {
                online = true;
                break;
            }
        }
        if ( !online && ( fp.frameType == FrameType::FRAMETYPE::CLCIRCLE || fp.frameType == FrameType::FRAMETYPE::CLVARIABLE ) ) {
            online = ( CalcVector2D::between(fp.pointList.last(), fp.pointList.first(), pp) );
        }
        if (online) gpoint.appendAttachedFrame(fp.UUID);
    }
}

QList<GlobalPoint> BaseBackGroundFloor::getCrossPoints() const
{
    return crossPointList;
}

QList<GlobalPoint> BaseBackGroundFloor::getDefinePoints() const
{
    return definePointList;
}

void BaseBackGroundFloor::drawCore(QPainter *painter)
{
    //source(mm)*scale = target(pix) // scale=exportscale*dpi/25.4
    //paperはsource(mm)を描画領域の縦横比に合わせてトリミングしたもの?
    QTransform t_trans = painter->transform(); //スケール
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    QTransform bb_trans = painter->transform(); //逆さ＋スケール
    painter->setTransform(t_trans);

    QPen pen1 = QPen(QBrush(), 50, Qt::SolidLine);
    QPen pen2 = QPen(QBrush(), 20, Qt::DashLine);
    pen1.setWidth(0);
    pen2.setWidth(0);

    // TODO:スカイツリーみたいな複雑な通り構成で通りの切断線を計算するとき、余計な通りがあるのを嫌う場合がある
    QList<QUuid> oklist;
    Q_FOREACH (FRAMEPOINTSVALUE fp, drawFrameList) {
        QList<JointData *> jdsOfFrame = myData->jointListOfFrame(fp.UUID);
        QList<JointData *> jdsOfFloor = myData->jointListOfFloor(myFloorFrameID);
        Q_FOREACH (JointData *jd, jdsOfFrame) {
            if (jdsOfFloor.contains(jd)) {
                if (!oklist.contains(fp.GID))oklist.append(fp.GID);

            }
        }
    }

    Q_FOREACH (FRAMEPOINTSVALUE fp, drawFrameList) {
        /* 節点を定義しないと表示されないので、Input画面が作成できない。 -> 解析画面だと消すことを検討する。
        if (!oklist.contains(fp.GID))
            continue;
        */

        if ( fp.pointList.isEmpty() )
            continue;

        QColor myColor = ( currentFrames.contains(fp.UUID) || currentGID == fp.GID ) ?
                    curColor : defColor ;
        if (!coloringSpan)
            myColor = defColor;
        pen1.setColor(myColor);
        pen2.setColor(myColor);
        pen1.setWidth(0);
        pen2.setWidth(0);

        painter->setTransform(bb_trans);

        painter->setBrush(QBrush(myColor, Qt::SolidPattern));
        QPointF p0 = fp.pointList.first();
        p0 *= 1000.;
        painter->setPen(pen1);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawEllipse(p0, 50.0, 50.0);

        for (int i = 1; i < fp.pointList.count(); i++) {
            QPointF p1 = fp.pointList.at(i - 1) * 1000.;
            QPointF p2 = fp.pointList.at(i) * 1000.;

            /* 通りの方向を示す△ */
            if(i==1){
                qreal tr_ang=qAtan2((p2.y()-p1.y()),(p2.x()-p1.x()));
                QPointF tr_1=p1+QPointF(1000.*qCos(tr_ang),1000.*qSin(tr_ang));
                tr_ang=tr_ang+150./180.*M_PI;
                QPointF tr_2=tr_1+QPointF(300.*qCos(tr_ang),300.*qSin(tr_ang));
                tr_ang=tr_ang+60./180.*M_PI;
                QPointF tr_3=tr_1+QPointF(300.*qCos(tr_ang),300.*qSin(tr_ang));
                painter->setPen(pen1);
                painter->setRenderHint(QPainter::Antialiasing,false);
                painter->drawLine(tr_1,tr_2);
                painter->drawLine(tr_2,tr_3);
                painter->drawLine(tr_3,tr_1);
            }

            painter->setPen(pen2);
            painter->setRenderHint(QPainter::Antialiasing, false);
            painter->drawLine(p1, p2);

            painter->setPen(pen1);
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->drawEllipse(p2, 50.0, 50.0);

        }

        if ( fp.frameType == FrameType::FRAMETYPE::CLCIRCLE ||  fp.frameType == FrameType::FRAMETYPE::CLVARIABLE  ) {
            QPointF p1 = fp.pointList.last() * 1000.;
            QPointF p2 = fp.pointList.first() * 1000.;
            painter->setPen(pen2);
            painter->setRenderHint(QPainter::Antialiasing, false);
            painter->drawLine(p1, p2);
        }

        // 通り名
        if(gridTextVisible){
            int strFontSize = FontSize(*painter, scaleTextSize);
            int circleSize = useScaledFont ? strFontSize : strFontSize * DpiScale(*painter);
            QFont strFont("Helvetica [Cronyx]", strFontSize);
            painter->setBrush(Qt::NoBrush);
            QPointF p3 = fp.namePoint * 1000.;
            painter->setPen(pen1);
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->drawEllipse(p3, circleSize, circleSize);
            painter->setTransform(t_trans);
            p3 = QPointF(p3.x() - circleSize, -p3.y() - circleSize);
            painter->setFont(strFont);
            painter->drawText(QRectF(p3.x(), p3.y(), 2.0 * circleSize, 2.0 * circleSize),
                              Qt::AlignCenter | Qt::TextDontClip, fp.frameName);
        }

        painter->setPen(pen1);

        // 直線通りのスパン（文字＋ライン）
        if ( fp.frameType == FrameType::FRAMETYPE::STRAIGHT ) {
            int numFontSize = FontSize(*painter, scaleTextSize);
            QFont numFont("Helvetica [Cronyx]", numFontSize);
            painter->setTransform(t_trans);
            painter->setFont(numFont);

            QPointF p1 = fp.straight.spoint * 1000.;
            QString tstr = QString("%1").arg( fp.straight.span * 1000., 0, 'f', 0 );
            QRectF trect = QRectF(p1.x(), -p1.y(), fp.straight.span * 1000., 2000.);

            painter->translate(p1.x(), -p1.y());
            painter->rotate(-fp.straight.angle);
            painter->translate(-p1.x(), p1.y());

            if ( painter->fontMetrics().width(tstr) > trect.width() ) {
                qreal ww = trect.width();
                int stretch = int(ww * 100. / qreal(painter->fontMetrics().width(tstr)));
                if ( stretch > 0 ) numFont.setStretch(stretch);
            }
            painter->setFont(numFont);
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->drawText(trect, Qt::AlignHCenter | Qt::AlignBottom, tstr);

            painter->setRenderHint(QPainter::Antialiasing, false);
            painter->drawLine(trect.bottomLeft(), trect.bottomRight());
            painter->drawLine(trect.bottomRight(), trect.topRight());
            if ( fp.straight.isFirst || fp.straight.isLast )
                painter->drawLine(trect.bottomLeft(), trect.topLeft());
        }
    }

    // 交点の描画
    if (drawingCross) {

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setTransform(bb_trans);
        painter->setBrush(Qt::NoBrush);

        Q_FOREACH (GlobalPoint gp, crossPointList) {
            QColor myColor = ( gp.containsFrame(currentFrames) ) ? curColor : defColor ;
            pen1.setColor(myColor);
            painter->setPen(pen1);
            painter->drawEllipse(gp.getXYQPointF() * 1000., 80.0, 80.0);
        }
    }

    painter->setTransform(t_trans);
}

void BaseBackGroundFloor::drawDivide(QPainter *painter, const int tate, const int yoko, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &pos, const bool tatediv, const bool yokodiv)
{
    const qreal exportscale = pos.scale;
    const qreal markerSize = pos.markerSize;//mm
    const qreal tooriradius = 0.5*markerSize/exportscale;//0.5*markerSize / 25.4 * painter->device()->logicalDpiX() / scale;
    const QFont toorifont("Helvetica [Cronyx]", (int)(600 * tooriradius / 1000));
    const QRectF source = pos.sourceRect(tate,yoko);

    const QTransform t_trans = painter->transform(); //スケール
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    const QTransform bb_trans = painter->transform(); //逆さ＋スケール
    painter->setTransform(bb_trans);
    QPointF p1, p2;
    QPen pen1 = QPen(QBrush(), 50, Qt::SolidLine);
    QPen pen2 = QPen(QBrush(), 20, Qt::DashLine);

    //四周を白く塗る。できればクリッピングで実現したい
    qDebug()<<"clip false before"<<painter->clipBoundingRect();
    painter->setClipping(false);
    qDebug()<<"clip false after"<<painter->clipBoundingRect();
    QRectF clip;
    if(tatediv && yokodiv){
        clip=QRectF(source.x()-2.0*tooriradius,
                    source.y()-2.0*tooriradius,
                    source.width()+4.0*tooriradius,
                    source.height()+4.0*tooriradius);
    }else if(tatediv){
        clip=QRectF(source.x(),
                    source.y()-2.0*tooriradius,
                    source.width(),
                    source.height()+4.0*tooriradius);

    }else if(yokodiv){
        clip=QRectF(source.x()-2.0*tooriradius,
                    source.y(),
                    source.width()+4.0*tooriradius,
                    source.height());

    }
    painter->setClipRect(clip, Qt::ReplaceClip);

    painter->setPen(QPen(Qt::darkGray,0,Qt::DotLine));
    painter->drawRect(source);

    QList<QUuid> oklist;
    Q_FOREACH (FRAMEPOINTSVALUE fp, drawFrameList) {
        QList<JointData *> jdsOfFrame = myData->jointListOfFrame(fp.UUID);
        QList<JointData *> jdsOfFloor = myData->jointListOfFloor(myFloorFrameID);
        Q_FOREACH (JointData *jd, jdsOfFrame) {
            if (jdsOfFloor.contains(jd)) {
                if (!oklist.contains(fp.GID))oklist.append(fp.GID);

            }
        }
    }

    Q_FOREACH (FRAMEPOINTSVALUE fp, drawFrameList) {

        /* 節点を定義しないと表示されないので、Input画面が作成できない。 -> 解析画面だと消すことを検討する。
        if (!oklist.contains(fp.GID))
            continue;
        */
        if ( fp.pointList.isEmpty() )
            continue;

        QColor myColor = ( currentFrames.contains(fp.UUID) || currentGID == fp.GID ) ?
                    curColor : defColor ;
        pen1.setColor(myColor);
        pen2.setColor(myColor);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(pen1);


        //p1p2と周囲の線分が交わるかどうか判定
        QPointF pbl(source.x() , source.y());
        QPointF pbr(source.x() + source.width() , source.y() );
        QPointF ptr(source.x() + source.width(),
                    source.y() + source.height() );
        QPointF ptl(source.x() , source.y() + source.height() );

        for (int i = 0; i < fp.pointList.count(); i++) {
            if (i == 0)
                p1 = fp.namePoint * 1000.;
            else
                p1 = fp.pointList.at(i - 1) * 1000.;
            p2 = fp.pointList.at(i) * 1000.;
            QPointF crossPoint;
            if(tatediv){
                if (checkIntersect(p1, p2, pbl, pbr) != QPointF()) {
                    crossPoint = checkIntersect(p1, p2, pbl, pbr);
                    if (p1.x() == p2.x())
                        crossPoint -= QPointF(0, tooriradius);
                    else {
                        qreal tan = (p1.y() - p2.y()) / (p1.x() - p2.x());
                        crossPoint -= QPointF(tooriradius / tan, tooriradius);
                    }
                    painter->setTransform(bb_trans);
                    painter->drawEllipse(crossPoint, (int)tooriradius, (int)tooriradius);
                    painter->setTransform(t_trans);
                    crossPoint = QPointF(crossPoint.x() - tooriradius, -crossPoint.y() - tooriradius);
                    painter->setFont(toorifont);
                    painter->drawText(QRectF(crossPoint.x(), crossPoint.y(), 2 * tooriradius, 2 * tooriradius),
                                      Qt::AlignCenter | Qt::TextDontClip, fp.frameName);

                    qDebug() << "crossingDetected at" << crossPoint;
                }
                if (checkIntersect(p1, p2, ptl, ptr) != QPointF()) {
                    crossPoint = checkIntersect(p1, p2, ptl, ptr);
                    if (p1.x() == p2.x())
                        crossPoint += QPointF(0, tooriradius);
                    else {
                        qreal tan = (p1.y() - p2.y()) / (p1.x() - p2.x());
                        crossPoint += QPointF(tooriradius / tan, tooriradius);
                    }
                    painter->setTransform(bb_trans);
                    painter->drawEllipse(crossPoint, (int)tooriradius, (int)tooriradius);
                    painter->setTransform(t_trans);
                    crossPoint = QPointF(crossPoint.x() - tooriradius, -crossPoint.y() - tooriradius);
                    painter->setFont(toorifont);
                    painter->drawText(QRectF(crossPoint.x(), crossPoint.y(), 2 * tooriradius, 2 * tooriradius),
                                      Qt::AlignCenter | Qt::TextDontClip, fp.frameName);
                    qDebug() << "crossingDetected at" << crossPoint;
                }
            }
            if(yokodiv){
                if (checkIntersect(p1, p2, pbr, ptr) != QPointF()) {
                    crossPoint = checkIntersect(p1, p2, pbr, ptr);
                    if (p1.x() == p2.x())
                        crossPoint += QPointF(tooriradius, 0.0);
                    else {
                        qreal tan = (p1.y() - p2.y()) / (p1.x() - p2.x());
                        crossPoint += QPointF(tooriradius, tooriradius * tan);
                    }
                    painter->setTransform(bb_trans);
                    painter->drawEllipse(crossPoint, (int)tooriradius, (int)tooriradius);
                    painter->setTransform(t_trans);
                    crossPoint = QPointF(crossPoint.x() - tooriradius, -crossPoint.y() - tooriradius);
                    painter->setFont(toorifont);
                    painter->drawText(QRectF(crossPoint.x(), crossPoint.y(), 2 * tooriradius, 2 * tooriradius),
                                      Qt::AlignCenter | Qt::TextDontClip, fp.frameName);
                    qDebug() << "crossingDetected at" << crossPoint;
                }

                if (checkIntersect(p1, p2, ptl, pbl) != QPointF()) {
                    crossPoint = checkIntersect(p1, p2, ptl, pbl);
                    if (p1.x() == p2.x())
                        crossPoint -= QPointF(tooriradius, 0.0);
                    else {
                        qreal tan = (p1.y() - p2.y()) / (p1.x() - p2.x());
                        crossPoint -= QPointF(tooriradius, tooriradius * tan);
                    }
                    painter->setTransform(bb_trans);
                    painter->drawEllipse(crossPoint, (int)tooriradius, (int)tooriradius);
                    painter->setTransform(t_trans);
                    crossPoint = QPointF(crossPoint.x() - tooriradius, -crossPoint.y() - tooriradius);
                    painter->setFont(toorifont);
                    painter->drawText(QRectF(crossPoint.x(), crossPoint.y(), 2 * tooriradius, 2 * tooriradius),
                                      Qt::AlignCenter | Qt::TextDontClip, fp.frameName);
                    qDebug() << "crossingDetected at" << crossPoint;
                }
            }
            painter->setTransform(bb_trans);
        }

    }
}
QPointF BaseBackGroundFloor::checkIntersect(const QPointF p1, const QPointF p2, const QPointF p3,
                                            const QPointF p4)const
{
    //線分p1p2とp3p4が交差すればtrue
    qreal ax = p1.x();
    qreal ay = p1.y();
    qreal bx = p2.x();
    qreal by = p2.y();
    qreal cx = p3.x();
    qreal cy = p3.y();
    qreal dx = p4.x();
    qreal dy = p4.y();
    qreal ta = (cx - dx) * (ay - cy) + (cy - dy) * (cx - ax);
    qreal tb = (cx - dx) * (by - cy) + (cy - dy) * (cx - bx);
    qreal tc = (ax - bx) * (cy - ay) + (ay - by) * (ax - cx);
    qreal td = (ax - bx) * (dy - ay) + (ay - by) * (ax - dx);


    if (tc * td < 0 && ta * tb < 0) {
        qreal S1 = ((dx - cx) * (ay - cy) - (dy - cy) * (ax - cx)) / 2;
        qreal S2 = ((dx - cx) * (cy - by) - (dy - cy) * (cx - bx)) / 2;

        qreal cpx = ax + (bx - ax) * S1 / (S1 + S2);
        qreal cpy = ay + (by - ay) * S1 / (S1 + S2);
        return QPointF(cpx, cpy);
    } else return QPointF();
}

BaseBackGroundFrame::BaseBackGroundFrame(bool _useScaledFont, UnifiedData* data)
    : BaseBackGround(_useScaledFont)
{
    myData = data;
    myFloorFrameID = QUuid();
    //    isAnalysisFigure = true;//floorとafloorのスイッチ
    defColor = Qt::black;
    curColor = Qt::blue;
}


void BaseBackGroundFrame::setCurrentFloors(const QList<QUuid> &idlist)
{
    currentFloors = idlist;
}

void BaseBackGroundFrame::setCurrentFrames(const QList<QUuid> &idlist)
{
    currentFrames = idlist;
    currentFrames.removeOne(myFloorFrameID);
}

void BaseBackGroundFrame::clearCurrentFloors()
{
    currentFloors.clear();
}

void BaseBackGroundFrame::clearCurrentFrames()
{
    currentFrames.clear();
}

void BaseBackGroundFrame::clearDrawingData()
{
    crossPointList.clear();
    definePointList.clear();
    floorIDList.clear();
    drawFloorIDList.clear();
    crossFrameLines.clear();
    crossFrameNames.clear();
    //    bottomFloorIDList.clear();
    bottomFloorPoints.clear();
}

void BaseBackGroundFrame::completeGlobalXYZ(GlobalPoint &gpoint)
{
    if ( floorIDList.isEmpty() ) return;

    gpoint.appendAttachedFrame(myFloorFrameID);
    qreal hh_gp = gpoint.elvY();
    QList<QUuid> idlist = myData->getNearlyFloorOfHeight(floorIDList, hh_gp);

    FrameType::FRAMETYPE f_type = myData->getFrameTypeOfFloor(myFloorFrameID);
    bool isOpen = ( f_type != FrameType::FRAMETYPE::CLCIRCLE && f_type != FrameType::FRAMETYPE::CLVARIABLE );

    QPointF pp;
    if ( idlist.count() == 1 ) {
        QList<QPointF> plist;
        Q_FOREACH (GlobalPoint gp, definePointList.value(idlist.first()))
            plist.append(gp.getXYQPointF());
        pp = CalcVector2D::elvXZToGlobalXY(plist, QPointF(gpoint.elvX(), gpoint.elvZ()), isOpen);
        if ( idlist.first() == floorIDList.first() ) {
            qreal hh = myData->getFloorHeight(floorIDList.first());
            if ( qAbs(hh - hh_gp) <= 1.0e-5 ) gpoint.appendAttachedFloor(idlist.first());
        } else if ( idlist.first() == floorIDList.last() ) {
            qreal hh = myData->getFloorHeight(floorIDList.last());
            if ( qAbs(hh - hh_gp) <= 1.0e-5 ) gpoint.appendAttachedFloor(idlist.first());
        } else {
            gpoint.appendAttachedFloor(idlist.first());
        }

    } else if ( idlist.count() >= 2 ) {
        QList<QPointF> plist1;
        Q_FOREACH (GlobalPoint gp, definePointList.value(idlist.first()))
            plist1.append(gp.getXYQPointF());
        qreal h1 = myData->getFloorHeight(idlist.first());
        QList<QPointF> plist2;
        Q_FOREACH (GlobalPoint gp, definePointList.value(idlist.last()))
            plist2.append(gp.getXYQPointF());
        qreal h2 = myData->getFloorHeight(idlist.last());
        qreal rr = (hh_gp - h1) / (h2 - h1);
        QList<QPointF> c_plist = CalcVector2D::combinePointList(plist1, plist2, rr);
        pp = CalcVector2D::elvXZToGlobalXY(c_plist, QPointF(gpoint.elvX(), gpoint.elvZ()), isOpen);
    }

    /* 交点がチェックされておらず、交差通りが上下階に連続していなかった場合、その交差通りの交点が無視される 計算時間短縮のため */
    Q_FOREACH ( QUuid cr_fr, crossFrameLines.keys() ) {
        QList<QLineF> crossLines = crossFrameLines.values(cr_fr);
        bool online = false;
        Q_FOREACH ( QLineF line, crossLines ) {
            if ( CalcVector2D::between(line.p1(), line.p2(), gpoint.getElvQPointF() * 1000.0, 1.0e-3) ) {
                online = true;
                break;
            }
        }
        if (online) gpoint.appendAttachedFrame(cr_fr);
    }

    gpoint.setX(pp.x());
    gpoint.setY(pp.y());
    gpoint.setZ(hh_gp);
}

void BaseBackGroundFrame::resetFramePoints(const QUuid &fr_id)
{
    qDebug()<<"BaseBackGroundFrame::reset in";
    qreal eps = 0.10; // 10cm以内を同一点と見なす

    clearDrawingData();
    qDebug()<<"BaseBackGroundFrame::reset0";
    myFloorFrameID = fr_id;
    if (myFloorFrameID.isNull()) return;

    qDebug()<<"BaseBackGroundFrame::reset1";
    QList<QUuid> fl_idlist = myData->getFloorListOfFrame(myFloorFrameID);
    qDebug()<<"BaseBackGroundFrame::reset2";

    Q_FOREACH (QUuid fl_id, fl_idlist) {

        qreal fl_height = myData->getFloorHeight(fl_id);

        if ( IsAnalysisFigure() ) {
            FLOORVALUES cur_fv = myData->getFloorValue(fl_id);
            if ( cur_fv.floorType == FLOORTYPE::FL_CHILD ) continue;
            fl_height += cur_fv.adjustedLevel;
        }

        QList<FRAMEPOINTSVALUE> fp_list = myData->getFrameValuesOfFloor(fl_id);

        QList<GlobalPoint> fl_crossList;
        QList<GlobalPoint> fl_defineList;
        QList<QPointF> myPointList;
        QHash< QUuid, QList<QPointF> > crossLineTable;

        Q_FOREACH ( FRAMEPOINTSVALUE fp, fp_list ) {

            QList<QPointF> plist = fp.pointList;

            if ( plist.count() < 2 ) continue;
            if ( fp.frameType == FrameType::FRAMETYPE::CLCIRCLE ||  fp.frameType == FrameType::FRAMETYPE::CLVARIABLE )
                plist.append(plist.first());

            if ( fp.UUID == myFloorFrameID ) {

                qreal sum_len = 0.0;
                for (int i = 0; i < plist.count(); i++) {
                    GlobalPoint gp(plist.at(i));
                    gp.setZ(fl_height);
                    gp.setElvX(sum_len);
                    gp.setElvY(fl_height);
                    gp.appendAttachedFloor(fl_id);
                    gp.appendAttachedFrame(myFloorFrameID);
                    fl_defineList.append(GlobalPoint(gp));
                    if (i < plist.count() - 1) sum_len += CalcVector2D::length(plist.at(i + 1) - plist.at(i));
                }
                myPointList = plist;

            } else {
                crossLineTable.insert(fp.UUID, plist);
            }

        }

        if (myPointList.isEmpty()) continue;

        QList<QUuid> n_idlist = crossLineTable.keys();

        Q_FOREACH (QUuid p_id, n_idlist) {

            QList<QPointF> o_plist = crossLineTable.value(p_id);
            QList<GlobalPoint> c_plist = calcCrossPoints(myPointList, o_plist);
            Q_FOREACH (GlobalPoint c_gp, c_plist) {
                int ii = indexOfGlobalPoint(fl_crossList, c_gp, eps);
                if ( ii >= 0 ) {
                    GlobalPoint gp = fl_crossList.at(ii);
                    gp.appendAttachedFrame(p_id);
                    fl_crossList.replace(ii, gp);
                } else {
                    int jj = indexOfGlobalPoint(fl_defineList, c_gp, eps);
                    if ( jj >= 0 ) {
                        GlobalPoint gp = fl_defineList.at(jj);
                        gp.appendAttachedFrame(p_id);
                        fl_crossList.append(gp);
                    } else {
                        c_gp.setZ(fl_height);
                        c_gp.setElvY(fl_height);
                        c_gp.appendAttachedFloor(fl_id);
                        c_gp.appendAttachedFrame(myFloorFrameID);
                        c_gp.appendAttachedFrame(p_id);
                        fl_crossList.append(c_gp);
                    }
                }
            }
        }

        QList<GlobalPoint> fl_crossList_sort;
        Q_FOREACH (GlobalPoint gp, fl_crossList) {
            int idx = 0;
            for (int i = 0; i < fl_crossList_sort.count(); i++) {
                if ( fl_crossList_sort.at(i).elvX() > gp.elvX() ) break;
                idx++;
            }
            fl_crossList_sort.insert(idx, gp);
        }
        crossPointList.insert(fl_id, fl_crossList_sort);
        definePointList.insert(fl_id, fl_defineList);
    }

    floorIDList = myData->sortFloorOrderOfHeight(definePointList.keys());
    if ( IsAnalysisFigure() ) {
        drawFloorIDList = floorIDList; // crossPointの算出時に補助階は除外済み
    } else {
        QList<QUuid> d_flist;
        Q_FOREACH ( QUuid uid, floorIDList ) {
            if ( !myData->isChildFloor(uid) ) d_flist.append(uid);
        }
        drawFloorIDList = d_flist;
    }

    calcCrossFrameLines();
}

/* ベース通り点列 a_list、他通り点列 b_list */
QList<GlobalPoint> BaseBackGroundFrame::calcCrossPoints( const QList<QPointF> &a_list,
                                                         const QList<QPointF> &b_list )
{
    //    qDebug() << "calcCross";

    QList<GlobalPoint> crosslist;

    qreal sum_len = 0.0;

    for ( int i = 0; i < a_list.count() - 1 ; i++) {

        QLineF aline( a_list.at(i), a_list.at(i + 1) );
        qreal len = 0.0;

        for ( int j = 0; j < b_list.count() - 1 ; j++) {

            QLineF bline( b_list.at(j), b_list.at(j + 1) );
            QList<QPointF> plist = CalcVector2D::intersectCheck(aline, bline);
            Q_FOREACH (QPointF pp, plist) {
                len = CalcVector2D::length(pp - a_list.at(i));
                GlobalPoint gp(pp);
                gp.setElvX(len + sum_len);
                crosslist.append(gp);
            }

            if (i == a_list.count() - 2) {
                if ( CalcVector2D::between(b_list.at(j), b_list.at(j + 1), a_list.last()) ) {
                    len = CalcVector2D::length(a_list.last() - a_list.at(i));
                    GlobalPoint gp(a_list.last());
                    gp.setElvX(len + sum_len);
                    crosslist.append(gp);
                }
            }
        }

        if ( CalcVector2D::between( a_list.at(i), a_list.at(i + 1), b_list.last()) ) {
            len = CalcVector2D::length(b_list.last() - a_list.at(i));
            GlobalPoint gp(b_list.last());
            gp.setElvX(len + sum_len);
            crosslist.append(gp);
        }

        sum_len += CalcVector2D::length(a_list.at(i + 1) - a_list.at(i));
    }

    //    qDebug() << crosslist.count();

    return crosslist;
}

void BaseBackGroundFrame::calcCrossFrameLines()
{
    QHash<QUuid, QList<QPointF> > downFloorPoints;

    QPointF p1, p2, p3;

    Q_FOREACH (QUuid fl_id, drawFloorIDList) {

        QList<GlobalPoint> cross_plist = crossPointList.value(fl_id);
        QHash<QUuid, QList<QPointF> > curFloorPoints;

        for (int i = 0; i < cross_plist.count(); i++) {

            p1 = cross_plist.at(i).getElvQPointF();
            p1 *= 1000.;

            QList<QUuid> frlist = cross_plist.at(i).getAttachedFrameList();
            Q_FOREACH (QUuid id, frlist) {
                //                if( curFloorPoints.contains(id) || id==myFloorFrameID ) continue;
                if ( id == myFloorFrameID ) continue;
                QList<QPointF> c_points = curFloorPoints.value(id);
                curFloorPoints.insert( id, c_points << p1 );
                if ( downFloorPoints.contains(id) ) {
                    QList<QPointF> d_points = downFloorPoints.value(id);
                    p2 = ( d_points.count() >= c_points.count() ) ? d_points.at(c_points.count() - 1) : d_points.last();
                    crossFrameLines.insert(id, QLineF(p1, p2));
                } else {
                    p3 = p1 + QPointF(0, -2000.);
                    QString str = myData->idToName(id, DATATYPE::TPFRAMEP);

                    if (fl_id == drawFloorIDList.first()) {
                        p3 = p3 + QPointF(0, -4500.);

                        int idx = 0;
                        for (int j = 0; j < bottomFloorPoints.count(); j++) {
                            if ( bottomFloorPoints.at(j).x() > p3.x() ) break;
                            idx++;
                        }
                        bottomFloorPoints.insert(idx, p3);
                        //                        bottomFloorIDList.insert(idx, id);
                    }

                    crossFrameNames.insert(id, FRNAMEANDPOINT(str, p3));
                }
            }
        }

        downFloorPoints = curFloorPoints;
    }

}


int BaseBackGroundFrame::indexOfGlobalPoint(const QList<GlobalPoint> &gplist, const GlobalPoint &gp,
                                            qreal eps)
{
    int idx = -1;
    for ( int i = 0; i < gplist.count(); i++ ) {
        if ( qAbs( gplist.at(i).elvX() - gp.elvX() ) < eps ) {
            if ( gplist.at(i).isNearlyPoint(gp.getXYQPointF(), eps) ) {
                idx = i;
                break;
            }
        }
    }
    return idx;
}

QList<GlobalPoint> BaseBackGroundFrame::getCrossPoints() const
{
    QList< QList<GlobalPoint> > gglist = crossPointList.values();

    QList<GlobalPoint> gplist;
    Q_FOREACH (QList<GlobalPoint> glist, gglist)
        gplist.append(glist);

    return gplist;
}

QList<GlobalPoint> BaseBackGroundFrame::getDefinePoints() const
{
    QList< QList<GlobalPoint> > gglist = definePointList.values();

    QList<GlobalPoint> gplist;
    Q_FOREACH (QList<GlobalPoint> glist, gglist)
        gplist.append(glist);

    return gplist;
}

void BaseBackGround::drawWhole(QPainter *painter)
{
    painter->save();
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    drawCore(painter);
    painter->restore();
}

void BaseBackGround::drawWholeWithScale(QPainter *painter, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &pos)
{
    painter->save();
    setScaleAndUpSideDown(painter,0,0,size,pos);
    drawCore(painter);
    painter->restore();
}

void BaseBackGround::drawPart(QPainter *painter, const int tate, const int yoko, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &pos, const bool tatediv, const bool yokodiv)
{
    painter->save();
    setScaleAndUpSideDown(painter, tate, yoko, size, pos);
    drawCore(painter);
    drawDivide(painter, tate, yoko, size, pos, tatediv, yokodiv);
    painter->restore();
}

void BaseBackGroundFrame::drawCore(QPainter *painter)
{
    QTransform t_trans = painter->transform(); //スケール
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    QTransform bb_trans = painter->transform(); //逆さ＋スケール

    QPen pen1 = QPen(QBrush(), 50, Qt::SolidLine);
    QPen pen2 = QPen(QBrush(), 20, Qt::DashLine);
    pen1.setWidth(0);
    pen2.setWidth(0);
    const int strFontSize = FontSize(*painter, scaleTextSize);
    const int circleSize = useScaledFont ? strFontSize : strFontSize * DpiScale(*painter);
    const QFont strFont("Helvetica [Cronyx]", strFontSize);
    qreal max_x = 0.0;

    Q_FOREACH (QUuid fl_id, drawFloorIDList) {
        QColor myColor = ( currentFloors.contains(fl_id) ) ? curColor : defColor ;
        if (!coloringSpan)
            myColor = defColor;
        pen1.setColor(myColor);
        pen2.setColor(myColor);
        painter->setBrush(Qt::NoBrush);

        // 通りの定義点
        QList<GlobalPoint> def_plist = definePointList.value(fl_id);
        QPointF p0 = def_plist.first().getElvQPointF();
        p0 *= 1000.;
        painter->setPen(pen1);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawEllipse(p0, 50.0, 50.0);

        painter->setRenderHint(QPainter::Antialiasing, false);
        for (int i = 1; i < def_plist.count(); i++) {
            QPointF p1 = def_plist.at(i - 1).getElvQPointF();
            QPointF p2 = def_plist.at(i).getElvQPointF();
            p1 *= 1000.;
            p2 *= 1000.;
            if (p2.x() > max_x)
                max_x = p2.x();

            painter->setPen(pen2);
            painter->setRenderHint(QPainter::Antialiasing, false);
            painter->drawLine(p1, p2);
            painter->setPen(pen1);
            painter->setRenderHint(QPainter::Antialiasing, false);
            painter->drawEllipse(p2, 50.0, 50.0);
        }
    }

    // 通りの交点
    painter->setBrush(Qt::NoBrush);
    painter->setRenderHint(QPainter::Antialiasing, false);
    if(drawingCross){
        Q_FOREACH ( GlobalPoint gp, getCrossPoints() ) {
            bool isOk = false;
            Q_FOREACH ( QUuid uid, gp.getAttachedFloorList() ) {
                if ( drawFloorIDList.contains(uid) ) {
                    isOk = true;
                    break;
                }
            }
            if ( !isOk )
                continue;

            QColor myColor = ( gp.containsFrame(currentFrames)
                        || gp.containsFloor(currentFloors) ) ? curColor : defColor ;
            pen1.setColor(myColor);
            painter->setPen(pen1);
            QPointF p1 = gp.getElvQPointF() * 1000.0;
            painter->drawEllipse(p1,80.0,80.0);
        }
    }

    // 交差通りの通り名および軸線
    painter->setPen(pen1);
    painter->setBrush(Qt::NoBrush);
    Q_FOREACH (QUuid fr_id, crossFrameNames.uniqueKeys()) {
        QColor myColor = ( currentFrames.contains(fr_id) ) ? curColor : defColor ;
        pen2.setColor(myColor);
        painter->setPen(pen2);
        painter->setRenderHint(QPainter::Antialiasing, true);
        Q_FOREACH (QLineF line, crossFrameLines.values(fr_id)) {
            painter->drawLine(line);
        }

        pen1.setColor(myColor);
        painter->setPen(pen1);
        painter->setRenderHint(QPainter::Antialiasing, true);
        if(gridTextVisible){
            Q_FOREACH (FRNAMEANDPOINT fname, crossFrameNames.values(fr_id) ) {
                QPointF p3 = fname.nameCenter;
                painter->drawEllipse(p3, circleSize, circleSize);
                painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true); // t_trans?
                p3 = QPointF(p3.x() - circleSize, -p3.y() - circleSize);
                painter->setFont(strFont);
                painter->drawText(QRectF(p3.x(), p3.y(), 2.0 * circleSize, 2.0 * circleSize),
                                  Qt::AlignCenter | Qt::TextDontClip,
                                  fname.frameName);
                painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true); // bb_trans?
            }
        }
    }

    //最下階の交差通りのスパン
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->setFont(strFont);
    pen1.setColor(defColor);
    painter->setPen(pen1);

    QFont numFont(strFont);
    QList<int> drawpointindex; // わざわざ変数をつくらなくてもいいかも
    for (int i = 0; i < bottomFloorPoints.count() - 1; i++) {
        QPointF p1 = bottomFloorPoints.at(i) + QPointF(0, 5500.0);
        qreal span = bottomFloorPoints.at(i + 1).x() - bottomFloorPoints.at(i).x();
        drawpointindex << i;

        QString cstr = QString("%1").arg( span, 0, 'f', 0 );
        const QRectF crect = QRectF(p1.x(), -p1.y(), span, 2500.0);

        if (span < 0.001) {
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->drawLine(crect.bottomRight(), crect.topRight());
            continue;
        }

        // いる？
        if (drawpointindex.count() == 1) {
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->drawLine(crect.bottomLeft(), crect.topLeft());
        }

        if ( painter->fontMetrics().width(cstr) > crect.width() ) {
            qreal ww = crect.width();
            int stretch = int(ww * 100.0 / qreal(painter->fontMetrics().width(cstr)));
            if ( stretch > 0 )
                numFont.setStretch(stretch);
        }
        painter->setFont(numFont);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawText(crect, Qt::AlignHCenter | Qt::AlignBottom, cstr);
        numFont.setStretch(100);

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawLine(crect.bottomLeft(), crect.bottomRight());
        painter->drawLine(crect.bottomRight(), crect.topRight());

        if ( i == 0 )
            painter->drawLine(crect.bottomLeft(), crect.topLeft());

    }

    if ( drawpointindex.count() > 0 ) {
        QPointF p1 = bottomFloorPoints.at(drawpointindex.first()) + QPointF(0, 3000.0);
        qreal tspan = bottomFloorPoints.at(drawpointindex.last() + 1).x() - bottomFloorPoints.at(
                    drawpointindex.first()).x();
        QString tstr = QString("%1").arg( tspan, 0, 'f', 0 );
        QRectF trect = QRectF(p1.x(), -p1.y(), tspan, 1200.0);

        if ( painter->fontMetrics().width(tstr) > trect.width() ) {
            qreal ww = trect.width();
            int stretch = int(ww * 100.0 / qreal(painter->fontMetrics().width(tstr)));
            if ( stretch > 0 ) numFont.setStretch(stretch);
        }

        painter->setFont(numFont);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawText(trect, Qt::AlignHCenter | Qt::AlignBottom, tstr);
        numFont.setStretch(100);

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawLine(trect.bottomLeft(), trect.bottomRight());
        painter->drawLine(trect.bottomRight(), trect.topRight());
        painter->drawLine(trect.bottomLeft(), trect.topLeft());
        painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);  // ここがWholeにはない
    }

    //階スケール
    QPointF p6;
    qreal down_h;
    qreal dx = max_x + 2000.0;
    for (int i = 0; i < drawFloorIDList.count() ; i++) {
        QColor myColor = ( currentFloors.contains(drawFloorIDList.at(i)) ) ? curColor : defColor ;
        painter->setTransform(bb_trans);
        pen1.setColor(myColor);
        painter->setPen(pen1);

        qreal fl_h;
        QString fl_name;

        if ( IsAnalysisFigure() ) {
            FLOORVALUES cur_fv = myData->getFloorValue( drawFloorIDList.at(i) );
            fl_h = cur_fv.floorHeight + cur_fv.adjustedLevel;
            fl_name = cur_fv.analysisName;
        } else {
            fl_h = myData->getFloorHeight(drawFloorIDList.at(i));
            fl_name = myData->idToName(drawFloorIDList.at(i), DATATYPE::TPFLOOR);
        }

        QPointF p1 = QPointF(dx, fl_h * 1000.0 );
        QPointF p2 = p1 + QPointF(4000.0, 0.0);
        painter->drawLine(p1, p2);
        QPointF p3 = p1 + QPointF(1800.0, 0.0);
        QPointF p4 = p3 + QPointF(-300.0, 520.0);
        QPointF p5 = p4 + QPointF( 600.0, 0.0);
        painter->drawLine(p3, p4);
        painter->drawLine(p4, p5);
        painter->drawLine(p5, p3);

        painter->setTransform(t_trans);
        p3 = p1 + QPointF(0.0, 3000.0);
        p4 = p3 + QPointF(3600.0, -2480.0);
        p3.setY(-p3.y());
        p4.setY(-p4.y());
        if(floorTextVisible){
        painter->drawText(QRectF(p3, p4), Qt::AlignHCenter | Qt::AlignBottom | Qt::TextDontClip, fl_name);
        }

        if (i == 0) {
            p6 = p2;
            down_h = fl_h;
            continue;
        }

        pen1.setColor(defColor);
        painter->setPen(pen1);

        painter->setTransform(bb_trans);
        painter->drawLine(p2, p6);

        qreal dh = (fl_h - down_h) * 1000.0;

        QPointF p7 = p6 + QPointF(-2000.0, 0.0);
        QPointF p8 = p7 + QPointF(dh, -2000.0);
        p7.setY(-p7.y());
        p8.setY(-p8.y());

        painter->setTransform(t_trans);
        painter->translate(p7.x(), p7.y());
        painter->rotate(-90);
        painter->translate(-p7.x(), -p7.y());

        QString str = QString("%1").arg(dh, 0, 'f', 0);
        if ( painter->fontMetrics().width(str) > dh ) {
            int stretch = int(dh * 100.0 / qreal(painter->fontMetrics().width(str)));
            if (stretch > 0) numFont.setStretch(stretch);
            painter->setFont(numFont);
        }
        painter->drawText(QRectF(p7, p8),
                          Qt::AlignHCenter | Qt::AlignBottom, str);
        numFont.setStretch(100);
        painter->setFont(numFont);

        p6 = p2;
        down_h = fl_h;
    }

    if ( drawFloorIDList.count() < 2 )
        return;

    qreal minHeight, maxHeight;
    if ( IsAnalysisFigure() ) {
        FLOORVALUES cur_fv = myData->getFloorValue(drawFloorIDList.first());
        minHeight = ( cur_fv.floorHeight + cur_fv.adjustedLevel ) * 1000.0;
        cur_fv = myData->getFloorValue(drawFloorIDList.last());
        maxHeight = ( cur_fv.floorHeight + cur_fv.adjustedLevel ) * 1000.0;
    } else {
        minHeight = myData->getFloorHeight(drawFloorIDList.first()) * 1000.0;
        maxHeight = myData->getFloorHeight(drawFloorIDList.last()) * 1000.0;
    }

    pen1.setColor(defColor);
    painter->setPen(pen1);

    if ( minHeight < 0.0 && maxHeight > 0.0 ) {
        qreal dh = -minHeight;
        painter->setTransform(bb_trans);
        QPointF p1 = QPointF(dx + 4000.0, minHeight);
        QPointF p2 = p1 + QPointF(1500.0, 0.0);
        QPointF p3 = p2 + QPointF(0.0, dh);
        QPointF p4 = p3 + QPointF(-1500.0, 0.0);
        painter->drawLine(p1, p2);
        painter->drawLine(p2, p3);
        painter->drawLine(p3, p4);

        p2 = p1 + QPointF(dh, -1500.0);
        p1.setY(-p1.y());
        p2.setY(-p2.y());
        QString str = QString("%1").arg(dh, 0, 'f', 0);
        painter->setTransform(t_trans);
        painter->translate(p1.x(), p1.y());
        painter->rotate(-90);
        painter->translate(-p1.x(), -p1.y());
        painter->drawText(QRectF(p1, p2),
                          Qt::AlignHCenter | Qt::AlignBottom | Qt::TextDontClip, str);

        dh = maxHeight;
        painter->setTransform(bb_trans);
        p1 = QPointF(dx + 4000.0, 0.0);
        p2 = p1 + QPointF(1500.0, 0.0);
        p3 = p2 + QPointF(0.0, dh);
        p4 = p3 + QPointF(-1500.0, 0.0);
        painter->drawLine(p2, p3);
        painter->drawLine(p3, p4);

        p2 = p1 + QPointF(dh, -1500.0);
        p1.setY(-p1.y());
        p2.setY(-p2.y());
        str = QString("%1").arg(dh, 0, 'f', 0);
        painter->setTransform(t_trans);
        painter->translate(p1.x(), p1.y());
        painter->rotate(-90);
        painter->translate(-p1.x(), -p1.y());
        painter->drawText(QRectF(p1, p2),
                          Qt::AlignHCenter | Qt::AlignBottom | Qt::TextDontClip, str);

    } else {
        qreal dh = maxHeight - minHeight;
        painter->setTransform(bb_trans);
        QPointF p1 = QPointF(dx + 4000.0, minHeight);
        QPointF p2 = p1 + QPointF(1500.0, 0.0);
        QPointF p3 = p2 + QPointF(0.0, dh);
        QPointF p4 = p3 + QPointF(-1500.0, 0.0);
        painter->drawLine(p1, p2);
        painter->drawLine(p2, p3);
        painter->drawLine(p3, p4);

        p2 = p1 + QPointF(dh, -1500.0);
        p1.setY(-p1.y());
        p2.setY(-p2.y());
        QString str = QString("%1").arg(dh, 0, 'f', 0);
        painter->setTransform(t_trans);
        painter->translate(p1.x(), p1.y());
        painter->rotate(-90);
        painter->translate(-p1.x(), -p1.y());
        painter->drawText(QRectF(p1, p2),
                          Qt::AlignHCenter | Qt::AlignBottom | Qt::TextDontClip, str);
    }

    painter->setTransform(t_trans); //refactoring2はt_transをかけた状態スタート
}

void BaseBackGroundFrame::drawDivide(QPainter *painter, const int tate, const int yoko, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &pos, const bool tatediv, const bool yokodiv)
{
    QRectF source = pos.sourceRect(tate,yoko);
    QList<QUuid> fr_idlist = crossFrameNames.uniqueKeys();
    QColor myColor;
    QPointF p1, p2, p3, p4, p5;
    QPen pen1 = QPen(QBrush(), 50, Qt::SolidLine);
    qreal exportscale = pos.scale;
    qreal markerSize = pos.markerSize;
    qreal tooriradius = 0.5*markerSize/exportscale;// / 25.4 * painter->device()->logicalDpiX() / scale;
    QFont toorifont("Helvetica [Cronyx]", (int)(600 * tooriradius / 1000));

    QTransform t_trans = painter->transform();
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    QTransform bb_trans = painter->transform();

    //切断通りマーカー
    painter->setTransform(bb_trans);
    painter->setClipRect(source.x()-2.0*tooriradius,source.y()-2.0*tooriradius,
                         source.width()+4.0*tooriradius,source.height()+4.0*tooriradius,Qt::ReplaceClip);
    painter->setPen(QPen(Qt::darkGray,0,Qt::DotLine));
    painter->drawRect(source);
    if(tatediv){

        Q_FOREACH (QUuid fr_id, fr_idlist) {
            Q_FOREACH (FRNAMEANDPOINT fname, crossFrameNames.values(fr_id) ) {
                p3 = fname.nameCenter;
                //範囲外の通り名を除く
                //            if (p3.x() < xmin - 100 || p3.x() > xmax + 100)continue;
                //
                myColor =  defColor ;

                pen1.setColor(myColor);
                painter->setPen(pen1);

                QPointF p4 = p3;
                p4.setY(source.y() + source.height() + tooriradius);
                QPointF p5 = p3;
                p5.setY(source.y() - tooriradius);

                //            if (p3.y() > p5.y())continue;
                painter->drawEllipse(p4, tooriradius, tooriradius);
                painter->drawEllipse(p5, tooriradius, tooriradius);
                painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);

                p4 = QPointF(p4.x() - tooriradius, -p4.y() - tooriradius);
                p5 = QPointF(p5.x() - tooriradius, -p5.y() - tooriradius);
                QString str = fname.frameName;
                painter->setFont(toorifont);
                painter->drawText(QRectF(p4.x(), p4.y(), 2 * tooriradius, 2 * tooriradius),
                                  Qt::AlignCenter | Qt::TextDontClip, str);
                painter->drawText(QRectF(p5.x(), p5.y(), 2 * tooriradius, 2 * tooriradius),
                                  Qt::AlignCenter | Qt::TextDontClip, str);
                painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
            }
        }
    }
    if(yokodiv){
        //切断線の階スケール
        for (int i = 0; i < drawFloorIDList.count() ; i++) {

            myColor = ( currentFloors.contains(drawFloorIDList.at(i)) ) ? curColor : defColor ;

            pen1.setColor(myColor);
            painter->setPen(pen1);

            qreal fl_h;
            QString fl_name;

            if ( IsAnalysisFigure() ) {
                FLOORVALUES cur_fv = myData->getFloorValue( drawFloorIDList.at(i) );
                fl_h = cur_fv.floorHeight + cur_fv.adjustedLevel;
                fl_name = cur_fv.analysisName;
            } else {
                fl_h = myData->getFloorHeight(drawFloorIDList.at(i));
                fl_name = myData->idToName(drawFloorIDList.at(i), DATATYPE::TPFLOOR);
            }

            for(int sayuu=0;sayuu<2;sayuu++){
                painter->setTransform(bb_trans);
                if(sayuu==0){
                    p1 = QPointF(source.x()+source.width(), fl_h * 1000.0 );
                }else{
                    p1 = QPointF(source.x()-2.0*tooriradius, fl_h * 1000.0 );
                }
                p2 = p1 + QPointF(2.0*tooriradius, 0.0);//レベル水平線の右端
                p3 = p1 + QPointF(1.0*tooriradius, 0.0);//レベル水平線の中央
                p4 = p3 + QPointF(-0.5*tooriradius, 0.7*tooriradius);//▽の左上
                p5 = p3 + QPointF( 0.5*tooriradius, 0.7*tooriradius);//▽の右上
                painter->drawLine(p3, p4);
                painter->drawLine(p4, p5);
                painter->drawLine(p5, p3);
                painter->drawLine(p1, p2);


                painter->setTransform(t_trans);
                p3 = p1 + QPointF(0.0+1.0, 0.7*tooriradius);
                p4 = p3 + QPointF(2.0*tooriradius-2.0, 0.4*tooriradius);
                p3.setY(-p3.y());
                p4.setY(-p4.y());
                //幅に収まるようにする
                painter->setFont(toorifont);
                painter->drawText(QRectF(p3, p4), Qt::AlignHCenter | Qt::AlignBottom | Qt::TextDontClip, fl_name);
            }


        }
    }
}

} // namespace post3dapp
