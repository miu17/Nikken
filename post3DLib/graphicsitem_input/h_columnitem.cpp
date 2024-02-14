#include "h_columnitem.h"
#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <qmath.h>

#include "calc_vector2d.h"
#include "unified_data.h"
namespace post3dapp{
ColumnHLineItem::ColumnHLineItem( const Input2DViewSettings& setting,const QList<QPointF> &plist, const QList<qreal> &lenlist,
                                  const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                                  const MEMBERPARTITION &partition, qreal fr_angle, qreal shx )
    : CustomDrawHLineItem(setting,ELCOLUMN, plist, lenlist, shape, view_type, partition)
{
    frameAngle = fr_angle;
    shiftX = shx;
    createShapePath();
    update(boundRect);
}

void ColumnHLineItem::createShapePath()
{
    baseTextName = QString();
    if ( sectionName.isEmpty() ) {
        createDummyShape();
        return;
    }
    if ( (myLineShape.leftSizeB < 1.0e-3 && myLineShape.leftSizeH < 1.0e-3)
         || (myPartition.lenFi > myPartition.lenFj)) {
        createErrorShape();
        return;
    }
    ( myItemView == ITEMVIEWTYPE::VIEW_PLAN ) ? createPlanShapePath() : createElevationShapePath() ;
}

void ColumnHLineItem::createPlanShapePath()
{
    frameAngle = qAtan2(myPointList.at(1).y() - myPointList.at(0).y(),
                        myPointList.at(1).x() - myPointList.at(0).x()) ;
    qreal theta = M_PI - frameAngle + myLineShape.angle;

    bool inRange = ( theta > -M_PI - eps && theta < M_PI + eps );
    while ( !inRange ) {
        if ( theta < -M_PI ) theta = theta + 2.0 * M_PI;
        if ( theta > M_PI ) theta = theta - 2.0 * M_PI;
        inRange = ( theta > -M_PI - eps && theta < M_PI + eps );
    }

    const QPointF shiftP(myLineShape.shiftY, myLineShape.shiftZ);
    createCommonShapePath(theta, M_PI, shiftP);

    ROTATEDSECTION left = getRotatedSection(myLineShape.renderType, myLineShape.leftSizeB, myLineShape.leftSizeH, theta);
    ROTATEDSECTION right = getRotatedSection(myLineShape.renderType, myLineShape.rightSizeB, myLineShape.rightSizeH, theta);
    // 符号描画の諸元（パスに含まず）
    setTextProperty(left.rotW, right.rotW, shiftP);

    // 柱脚
    setBaseProperty(theta, shiftP);
}

void ColumnHLineItem::setTextProperty(qreal leftSizeW, qreal rightSizeW, const QPointF& shiftP)
{
    const qreal len_mid = ( myLenList.first() + myLenList.last() ) / 2.0;
    int idx_mid = 0;
    for (int i = 0; i < myLenList.count() - 1; i++) {
        if ( myLenList.at(i) - eps < len_mid && len_mid < myLenList.at(i + 1) ) {
            idx_mid = i;
            break;
        }
    }
    textAngle = qAtan2(myPointList.at(idx_mid + 1).y() - myPointList.at(idx_mid).y(),
                       myPointList.at(idx_mid + 1).x() - myPointList.at(idx_mid).x()) * 180.0 / M_PI;
    qreal rr = (len_mid - myLenList.at(idx_mid)) / (myLenList.at(idx_mid + 1) - myLenList.at(idx_mid));
    textPoint = CalcVector2D::interiorPoint(myPointList.at(idx_mid), myPointList.at(idx_mid + 1), rr);
    textPoint = textPoint + shiftP;
    textShift = ( leftSizeW > rightSizeW ) ? leftSizeW / 2.0 : rightSizeW / 2.0 ;
    textName = sectionName;
    if ( textAngle < 92.0 && textAngle > -88.0 ) {
        if (isLineDirVisible) textName += " >";
    } else {
        textAngle = textAngle + 180.0;
        if (isLineDirVisible) textName += " <";
    }
}

void ColumnHLineItem::setBaseProperty(qreal theta, const QPointF& shift)
{
    basePlatePath = QPainterPath();
    baseTextName = myLineShape.baseName;
    if ( !baseTextName.isEmpty() && myLenList.first() < 1.0e-3 ) {

        qreal base_w = myLineShape.baseSizeB;
        qreal base_t = myLineShape.baseSizeT;

        if ( qAbs(theta) > eps ) {
            ROTATEDSECTION brs = getRotatedSection(RENDERSHAPE::SHAPE_BOX, base_w, myLineShape.baseSizeD, theta);
            base_w = brs.rotW;
        }

        base_w = qMax(base_w, 10.0);
        base_t = qMax(base_t, 10.0);

        QPointF p1 = myPointList.at(0);
        QPointF p2 = myPointList.at(1);
        qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());
        p2 = p1 + QPointF(base_t *qCos(ag), base_t *qSin(ag));

        QPointF q1 = p1 + QPointF(base_w / 2.0 * qCos(ag + M_PI / 2.0), base_w / 2.0 * qSin(ag + M_PI / 2.0));
        QPointF q2 = p2 + QPointF(base_w / 2.0 * qCos(ag + M_PI / 2.0), base_w / 2.0 * qSin(ag + M_PI / 2.0));
        QPointF q3 = p2 + QPointF(base_w / 2.0 * qCos(ag - M_PI / 2.0), base_w / 2.0 * qSin(ag - M_PI / 2.0));
        QPointF q4 = p1 + QPointF(base_w / 2.0 * qCos(ag - M_PI / 2.0), base_w / 2.0 * qSin(ag - M_PI / 2.0));

        QPolygonF polygon;
        polygon << q1 << q2 << q3 << q4 << q1;
        basePlatePath.addPolygon(polygon);
        baseTextPoint = q3 + shift;
        baseTextAngle = ag * 180.0 / M_PI - 90.0;
    }
}

void ColumnHLineItem::createCommonShapePath(qreal theta, qreal localRotate, const QPointF& shiftP){
    ROTATEDSECTION left = getRotatedSection(myLineShape.renderType, myLineShape.leftSizeB,
                                            myLineShape.leftSizeH, theta);
    qreal leftSizeW = left.rotW;
    qreal leftSizeL = left.rotL;
    ROTATEDSECTION center = getRotatedSection(myLineShape.renderType, myLineShape.centerSizeB,
                                             myLineShape.centerSizeH, theta);
    qreal centerSizeW = center.rotW;
    qreal centerSizeL = center.rotL;
    ROTATEDSECTION right = getRotatedSection(myLineShape.renderType, myLineShape.rightSizeB,
                                             myLineShape.rightSizeH, theta);
    qreal rightSizeW = right.rotW;
    qreal rightSizeL = right.rotL;

    const qreal len_i = myPartition.lenI;
    const qreal len_j = myPartition.lenJ;
    const qreal len_fi = myPartition.lenFi;
    const qreal len_fj = myPartition.lenFj;

    qreal cur_B, cur_L;
    if ( myLenList.first() < len_fi - eps ) {
        cur_B = 0.0;
        cur_L = 0.0;
    } else if ( myLenList.first() < len_i + eps ) {
        cur_B = leftSizeW;
        cur_L = ( leftSizeL > 0 ) ? leftSizeL : 0.0;
    } else if ( myLenList.first() < len_j - eps ) {
        cur_B = centerSizeW;
        cur_L = ( centerSizeL > 0 ) ? centerSizeL : 0.0;
    } else if ( myLenList.first() < len_fj + eps ) {
        cur_B = rightSizeW;
        cur_L = ( rightSizeL > 0 ) ? rightSizeL : 0.0;
    } else {
        cur_B = 0.0;
        cur_L = 0.0;
    }
    qreal cur_x = 0.0;
    QPointF cur_p1, cur_p2, cur_p3;
    qreal b1, b2, b3;
    QPointF q1, q2, q3, q4;
    drawShapePath = QPainterPath();
    shapePath = QPainterPath();
    qreal shapeWidth = ( centerSizeW > eps ) ? centerSizeW + 100.0 : 200.0 ;

    for ( int i = 0 ; i < myPointList.count() - 1 ; i++ ) {
        QVector<QPointF> p1_list, p2_list, p3_list;
        const QPointF p1 = myPointList.at(i);
        const QPointF p2 = myPointList.at(i + 1);
        const qreal l1 = myLenList.at(i);
        const qreal l2 = myLenList.at(i + 1);
        const qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());

        b1 = cur_B / 2.0;
        b2 = -cur_B / 2.0;
        b3 = b1 - cur_L;
        cur_p1 = p1 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
        cur_p2 = p1 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
        cur_p3 = p1 + shiftP + QPointF(b3 * qCos(ag + localRotate / 2.0), b3 * qSin(ag + localRotate / 2.0));
        cur_x = l1;

        if ( l1 - eps < len_fi && len_fi < l2 + eps ) {
            QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_fi - l1) / (l2 - l1));
            cur_B = leftSizeW;
            cur_L = ( leftSizeL > 0 ) ? leftSizeL : 0.0;
            if ( cur_B < eps ) cur_B = 10.0;
            b1 = cur_B / 2.0;
            b2 = -cur_B / 2.0;
            b3 = b1 - cur_L;
            cur_p1 = p0 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
            cur_p2 = p0 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
            cur_p3 = p0 + shiftP + QPointF(b3 * qCos(ag + localRotate / 2.0), b3 * qSin(ag + localRotate / 2.0));
            cur_x = len_fi;
        }

        if ( l1 - eps < len_i && len_i < l2 + eps ) {

            if ( len_i - cur_x < eps ) {
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_i - l1) / (l2 - l1));
                cur_B = centerSizeW;
                cur_L = ( centerSizeL > 0 ) ? centerSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = cur_B / 2.0;
                b2 = -cur_B / 2.0;
                b3 = b1 - cur_L;
                cur_p1 = p0 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
                cur_p2 = p0 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
                cur_p3 = p0 + shiftP + QPointF(b3 * qCos(ag + localRotate / 2.0), b3 * qSin(ag + localRotate / 2.0));
            } else {
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_i - l1) / (l2 - l1));
                b1 = cur_B / 2.0;
                b2 = -cur_B / 2.0;
                b3 = b1 - cur_L;
                cur_p1 = p0 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
                cur_p2 = p0 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
                cur_p3 = p0 + shiftP + QPointF(b3 * qCos(ag + localRotate / 2.0), b3 * qSin(ag + localRotate / 2.0));
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                cur_B = centerSizeW;
                cur_L = ( centerSizeL > 0 ) ? centerSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = cur_B / 2.0;
                b2 = -cur_B / 2.0;
                b3 = b1 - cur_L;
                cur_p1 = p0 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
                cur_p2 = p0 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
                cur_p3 = p0 + shiftP + QPointF(b3 * qCos(ag + localRotate / 2.0), b3 * qSin(ag + localRotate / 2.0));
            }
            cur_x = len_i;
        }

        if ( l1 - eps < len_j && len_j < l2 + eps ) {

            if ( len_j - cur_x < eps ) {
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_j - l1) / (l2 - l1));
                cur_B = rightSizeW;
                cur_L = ( rightSizeL > 0 ) ? rightSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = cur_B / 2.0;
                b2 = -cur_B / 2.0;
                b3 = b1 - cur_L;
                cur_p1 = p0 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
                cur_p2 = p0 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
                cur_p3 = p0 + shiftP + QPointF(b3 * qCos(ag + localRotate / 2.0), b3 * qSin(ag + localRotate / 2.0));
            } else {
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_j - l1) / (l2 - l1));
                b1 = cur_B / 2.0;
                b2 = -cur_B / 2.0;
                b3 = b1 - cur_L;
                cur_p1 = p0 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
                cur_p2 = p0 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
                cur_p3 = p0 + shiftP + QPointF(b3 * qCos(ag + localRotate / 2.0), b3 * qSin(ag + localRotate / 2.0));
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                cur_B = rightSizeW;
                cur_L = ( rightSizeL > 0 ) ? rightSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = cur_B / 2.0;
                b2 = -cur_B / 2.0;
                b3 = b1 - cur_L;
                cur_p1 = p0 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
                cur_p2 = p0 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
                cur_p3 = p0 + shiftP + QPointF(b3 * qCos(ag + localRotate / 2.0), b3 * qSin(ag + localRotate / 2.0));
            }
            cur_x = len_j;
        }

        if ( l1 - eps < len_fj && len_fj < l2 + eps ) {

            if ( len_fj - cur_x > eps ) {
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_fj - l1) / (l2 - l1));
                b1 = cur_B / 2.0;
                b2 = -cur_B / 2.0;
                b3 = b1 - cur_L;
                cur_p1 = p0 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
                cur_p2 = p0 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
                cur_p3 = p0 + shiftP + QPointF(b3 * qCos(ag + localRotate / 2.0), b3 * qSin(ag + localRotate / 2.0));
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                cur_B = 0.0;
                cur_L = 0.0;
                cur_p1 = p0;
                cur_p2 = p0;
                cur_p3 = p0;
            }
            cur_x = len_fj;
        }

        if ( cur_x < l2 + eps && cur_B > eps ) {
            p1_list.append(cur_p1);
            p2_list.prepend(cur_p2);
            p3_list.append(cur_p3);
            b1 = cur_B / 2.0;
            b2 = -cur_B / 2.0;
            b3 = b1 - cur_L;
            cur_p1 = p2 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
            cur_p2 = p2 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
            cur_p3 = p2 + shiftP + QPointF(b3 * qCos(ag + localRotate / 2.0), b3 * qSin(ag + localRotate / 2.0));
            cur_x = l2;
            p1_list.append(cur_p1);
            p2_list.prepend(cur_p2);
            p3_list.append(cur_p3);
        }

        if ( !p1_list.isEmpty() ) {
            QPolygonF polygon( QVector<QPointF>(p1_list + p2_list) );
            polygon << p1_list.first();
            drawShapePath.addPolygon(polygon);
            if ( leftSizeL > 0.0 || rightSizeL > 0.0 ) {
                for (int j = 1; j < p3_list.count(); j++) {
                    drawShapePath.moveTo(p3_list.at(j - 1));
                    drawShapePath.lineTo(p3_list.at(j));
                }
            }
        }

        b1 = shapeWidth / 2.0;
        b2 = -shapeWidth / 2.0;
        q1 = p1 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
        q2 = p2 + shiftP + QPointF(b1 * qCos(ag + localRotate / 2.0), b1 * qSin(ag + localRotate / 2.0));
        q3 = p2 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));
        q4 = p1 + shiftP + QPointF(b2 * qCos(ag + localRotate / 2.0), b2 * qSin(ag + localRotate / 2.0));

        QPolygonF s_polygon;
        s_polygon << q1 << q2 << q3 << q4;
        shapePath.addPolygon(s_polygon);
    }

    drawShapePath.closeSubpath();
    shapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();
}

void ColumnHLineItem::createElevationShapePath()
{
    qreal theta = ( myPointList.at(1).y() - myPointList.at(0).y() > -1.0e-5 ) ?
                  -myLineShape.angle + frameAngle - M_PI / 2.0 : -myLineShape.angle + frameAngle + M_PI / 2.0 ;

    bool inRange = ( theta > -M_PI - eps && theta < M_PI + eps );
    while ( !inRange ) {
        if ( theta < -M_PI ) theta = theta + 2.0 * M_PI;
        if ( theta > M_PI ) theta = theta - 2.0 * M_PI;
        inRange = ( theta > -M_PI - eps && theta < M_PI + eps );
    }

    const QPointF shiftP(shiftX, 0.0);
    createCommonShapePath(theta, -M_PI, shiftP);

    ROTATEDSECTION left = getRotatedSection(myLineShape.renderType, myLineShape.leftSizeB, myLineShape.leftSizeH, theta);
    ROTATEDSECTION right = getRotatedSection(myLineShape.renderType, myLineShape.rightSizeB, myLineShape.rightSizeH, theta);
    // 符号描画の諸元（パスに含まず）
    setTextProperty(left.rotW, right.rotW, shiftP);

    // 柱脚
    setBaseProperty(theta, shiftP);
}

void ColumnHLineItem::createErrorShape()
{
    myElementState = ELERROR;
    const qreal errorWidth = 100.0;
    drawShapePath = QPainterPath();
    setPolygonPath(drawShapePath, errorWidth);
    drawShapePath.closeSubpath();

    const qreal shapeWidth = 200.0;
    shapePath = QPainterPath();
    setPolygonPath(shapePath, shapeWidth);
    shapePath.closeSubpath();

    boundRect = drawShapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
    setErrorOrDummyTextProperty("ERROR(" + sectionName + ")", errorWidth);
}

void ColumnHLineItem::createDummyShape()
{
    myElementState = ELDUMMY;
    const qreal dummyWidth = 100.0;
    drawShapePath = QPainterPath();
    setPolygonPath(drawShapePath, 100.0);
    drawShapePath.closeSubpath();

    const qreal shapeWidth = 200.0;
    shapePath = QPainterPath();
    setPolygonPath(shapePath, shapeWidth);
    shapePath.closeSubpath();

    boundRect = drawShapePath.boundingRect();

    // 符号描画の諸元（パスに含まず）
    setErrorOrDummyTextProperty("(dummyC)", dummyWidth);
}

void ColumnHLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    setPainter(painter, option);

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(drawShapePath);

    if ( !baseTextName.isEmpty() ) {
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(basePlatePath);
    }

    if ( !isNameVisible ) return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->translate(textPoint.x(), -textPoint.y());

    painter->setFont(QFont("Consolas", FontSize(*painter)));
    QRect boundingRect = QFontMetrics(painter->font()).boundingRect(textName);
    painter->rotate(-textAngle);
    painter->drawText(QRect(-boundingRect.width() / 2.0, -textShift - boundingRect.height(), boundingRect.width(), boundingRect.height()),
                      Qt::AlignCenter, textName);

    if ( baseTextName.isEmpty() ) return;

    painter->rotate(textAngle);
    painter->translate(-textPoint.x(), textPoint.y());

    painter->translate(baseTextPoint.x(), -baseTextPoint.y());
    painter->setFont(QFont("Consolas", FontSize(*painter) * 0.7));
    boundingRect = QFontMetrics(painter->font()).boundingRect(baseTextName);

    painter->rotate(-baseTextAngle);
    painter->drawText(QRect(textShift, - boundingRect.height(), boundingRect.width(), boundingRect.height()),
                      Qt::AlignLeft, baseTextName);

}
} // namespace post3dapp
