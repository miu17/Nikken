#include "h_girderitem.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "calc_vector2d.h"
#include "unified_data.h"
namespace post3dapp{
GirderHLineItem::GirderHLineItem( const Input2DViewSettings& setting,ELEMENTTYPE type,
                                  const QList<QPointF> &plist, const QList<qreal> &lenlist,
                                  const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                                  const MEMBERPARTITION &partition )
    : CustomDrawHLineItem(setting,type, plist, lenlist, shape, view_type, partition)
{
    createShapePath();
    update(boundRect);
}

void GirderHLineItem::createShapePath()
{
    if ( sectionName.isEmpty() ) {
        createDummyShape();
        return;
    }
    if ( myLineShape.leftSizeB < 1.0e-3 && myLineShape.leftSizeH < 1.0e-3 ) {
        createErrorShape();
        return;
    }
    ( myItemView == ITEMVIEWTYPE::VIEW_PLAN ) ? createPlanShapePath() : createElevationShapePath() ;
}

void GirderHLineItem::createPlanShapePath()
{
    qreal len_i = myPartition.lenI;
    qreal len_j = myPartition.lenJ;
    qreal len_fi = myPartition.lenFi;
    qreal len_fj = myPartition.lenFj;
    qreal len_mid = ( myLenList.first() + myLenList.last() ) / 2.0;

    if ( len_fi > len_fj ) {
        createErrorShape();
        return;
    }

    bool isRotS = ( myLineShape.renderType == RENDERSHAPE::SHAPE_I || myLineShape.renderType == RENDERSHAPE::SHAPE_T2 );
    qreal leftSizeW = ( isRotS ) ? myLineShape.leftSizeH : myLineShape.leftSizeB;
    qreal leftSizeL = -1.0;
    qreal centerSizeW = ( isRotS ) ? myLineShape.centerSizeH : myLineShape.centerSizeB;
    qreal centerSizeL = -1.0;
    qreal rightSizeW = ( isRotS ) ? myLineShape.rightSizeH : myLineShape.rightSizeB;
    qreal rightSizeL = -1.0;
    /*
        qreal leftSizeW=myLineShape.leftSizeB;
        qreal leftSizeL=-1.0;
        qreal centerSizeW=myLineShape.centerSizeB;
        qreal centerSizeL=-1.0;
        qreal rightSizeW=myLineShape.rightSizeB;
        qreal rightSizeL=-1.0;
    */
    qreal shiftY = myLineShape.shiftY;

    if ( qAbs(myLineShape.angle) > eps ) {

        ROTATEDSECTION rs = getRotatedSection(myLineShape.renderType, myLineShape.leftSizeB,
                                              myLineShape.leftSizeH, myLineShape.angle + M_PI / 2.0);
        leftSizeW = rs.rotW;
        leftSizeL = rs.rotL;

        rs = getRotatedSection(myLineShape.renderType, myLineShape.centerSizeB,
                               myLineShape.centerSizeH, myLineShape.angle + M_PI / 2.0);
        centerSizeW = rs.rotW;
        centerSizeL = rs.rotL;

        rs = getRotatedSection(myLineShape.renderType, myLineShape.rightSizeB,
                               myLineShape.rightSizeH, myLineShape.angle + M_PI / 2.0);
        rightSizeW = rs.rotW;
        rightSizeL = rs.rotL;
    }

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
        QPointF p1 = myPointList.at(i);
        QPointF p2 = myPointList.at(i + 1);
        qreal l1 = myLenList.at(i);
        qreal l2 = myLenList.at(i + 1);
        qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());

        b1 = cur_B / 2.0 + shiftY;
        b2 = -cur_B / 2.0 + shiftY;
        b3 = b1 - cur_L;
        cur_p1 = p1 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
        cur_p2 = p1 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
        cur_p3 = p1 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//        cur_p1=p1+QPointF(cur_B/2.0*qCos(ag+M_PI/2.0),cur_B/2.0*qSin(ag+M_PI/2.0));
//        cur_p2=p1+QPointF(cur_B/2.0*qCos(ag-M_PI/2.0),cur_B/2.0*qSin(ag-M_PI/2.0));
        cur_x = l1;

        if ( l1 - eps < len_fi && len_fi < l2 + eps ) {
            QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_fi - l1) / (l2 - l1));
            cur_B = leftSizeW;
            cur_L = ( leftSizeL > 0 ) ? leftSizeL : 0.0;
            if ( cur_B < eps ) cur_B = 10.0;
            b1 = cur_B / 2.0 + shiftY;
            b2 = -cur_B / 2.0 + shiftY;
            b3 = b1 - cur_L;
            cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
            cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
            cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//            cur_p1=p0+QPointF(cur_B/2.0*qCos(ag+M_PI/2.0),cur_B/2.0*qSin(ag+M_PI/2.0));
//            cur_p2=p0+QPointF(cur_B/2.0*qCos(ag-M_PI/2.0),cur_B/2.0*qSin(ag-M_PI/2.0));
            cur_x = len_fi;
        }

        if ( l1 - eps < len_i && len_i < l2 + eps ) {

            if ( len_i - cur_x < eps ) {
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_i - l1) / (l2 - l1));
                cur_B = centerSizeW;
                cur_L = ( centerSizeL > 0 ) ? centerSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = cur_B / 2.0 + shiftY;
                b2 = -cur_B / 2.0 + shiftY;
                b3 = b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p1=p0+QPointF(cur_B/2.0*qCos(ag+M_PI/2.0),cur_B/2.0*qSin(ag+M_PI/2.0));
//                cur_p2=p0+QPointF(cur_B/2.0*qCos(ag-M_PI/2.0),cur_B/2.0*qSin(ag-M_PI/2.0));
            } else {
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_i - l1) / (l2 - l1));
                b1 = cur_B / 2.0 + shiftY;
                b2 = -cur_B / 2.0 + shiftY;
                b3 = b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p1=p0+QPointF(cur_B/2.0*qCos(ag+M_PI/2.0),cur_B/2.0*qSin(ag+M_PI/2.0));
//                cur_p2=p0+QPointF(cur_B/2.0*qCos(ag-M_PI/2.0),cur_B/2.0*qSin(ag-M_PI/2.0));
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                cur_B = centerSizeW;
                cur_L = ( centerSizeL > 0 ) ? centerSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = cur_B / 2.0 + shiftY;
                b2 = -cur_B / 2.0 + shiftY;
                b3 = b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p1=p0+QPointF(cur_B/2.0*qCos(ag+M_PI/2.0),cur_B/2.0*qSin(ag+M_PI/2.0));
//                cur_p2=p0+QPointF(cur_B/2.0*qCos(ag-M_PI/2.0),cur_B/2.0*qSin(ag-M_PI/2.0));
            }
            cur_x = len_i;
        }

        if ( l1 - eps < len_j && len_j < l2 + eps ) {

            if ( len_j - cur_x < eps ) {
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_j - l1) / (l2 - l1));
                cur_B = rightSizeW;
                cur_L = ( rightSizeL > 0 ) ? rightSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = cur_B / 2.0 + shiftY;
                b2 = -cur_B / 2.0 + shiftY;
                b3 = b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p1=p0+QPointF(cur_B/2.0*qCos(ag+M_PI/2.0),cur_B/2.0*qSin(ag+M_PI/2.0));
//                cur_p2=p0+QPointF(cur_B/2.0*qCos(ag-M_PI/2.0),cur_B/2.0*qSin(ag-M_PI/2.0));
            } else {
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_j - l1) / (l2 - l1));
                b1 = cur_B / 2.0 + shiftY;
                b2 = -cur_B / 2.0 + shiftY;
                b3 = b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p1=p0+QPointF(cur_B/2.0*qCos(ag+M_PI/2.0),cur_B/2.0*qSin(ag+M_PI/2.0));
//                cur_p2=p0+QPointF(cur_B/2.0*qCos(ag-M_PI/2.0),cur_B/2.0*qSin(ag-M_PI/2.0));
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                cur_B = rightSizeW;
                cur_L = ( rightSizeL > 0 ) ? rightSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = cur_B / 2.0 + shiftY;
                b2 = -cur_B / 2.0 + shiftY;
                b3 = b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p1=p0+QPointF(cur_B/2.0*qCos(ag+M_PI/2.0),cur_B/2.0*qSin(ag+M_PI/2.0));
//                cur_p2=p0+QPointF(cur_B/2.0*qCos(ag-M_PI/2.0),cur_B/2.0*qSin(ag-M_PI/2.0));
            }
            cur_x = len_j;
        }

        if ( l1 - eps < len_fj && len_fj < l2 + eps ) {

            if ( len_fj - cur_x > eps ) {
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_fj - l1) / (l2 - l1));
                b1 = cur_B / 2.0 + shiftY;
                b2 = -cur_B / 2.0 + shiftY;
                b3 = b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p1=p0+QPointF(cur_B/2.0*qCos(ag+M_PI/2.0),cur_B/2.0*qSin(ag+M_PI/2.0));
//                cur_p2=p0+QPointF(cur_B/2.0*qCos(ag-M_PI/2.0),cur_B/2.0*qSin(ag-M_PI/2.0));
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
            b1 = cur_B / 2.0 + shiftY;
            b2 = -cur_B / 2.0 + shiftY;
            b3 = b1 - cur_L;
            cur_p1 = p2 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
            cur_p2 = p2 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
            cur_p3 = p2 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//            cur_p1=p2+QPointF(cur_B/2.0*qCos(ag+M_PI/2.0),cur_B/2.0*qSin(ag+M_PI/2.0));
//            cur_p2=p2+QPointF(cur_B/2.0*qCos(ag-M_PI/2.0),cur_B/2.0*qSin(ag-M_PI/2.0));
            cur_x = l2;
            p1_list.append(cur_p1);
            p2_list.prepend(cur_p2);
            p3_list.append(cur_p3);
        }

        if ( !p1_list.isEmpty() ) {
            QPolygonF polygon( QVector<QPointF>(p1_list + p2_list) );
            polygon << p1_list.first();
            drawShapePath.addPolygon(polygon);
            if ( leftSizeL > 0.0 || centerSizeL > 0.0 || rightSizeL > 0.0 ) {
                for (int j = 1; j < p3_list.count(); j++) {
                    drawShapePath.moveTo(p3_list.at(j - 1));
                    drawShapePath.lineTo(p3_list.at(j));
                }
            }
        }

        b1 = shapeWidth / 2.0 + shiftY;
        b2 = -shapeWidth / 2.0 + shiftY;
        q1 = p1 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
        q2 = p2 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
        q3 = p2 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
        q4 = p1 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
//        q1=p1+QPointF(shapeWidth/2.0*qCos(ag+M_PI/2.0),shapeWidth/2.0*qSin(ag+M_PI/2.0));
//        q2=p2+QPointF(shapeWidth/2.0*qCos(ag+M_PI/2.0),shapeWidth/2.0*qSin(ag+M_PI/2.0));
//        q3=p2+QPointF(shapeWidth/2.0*qCos(ag-M_PI/2.0),shapeWidth/2.0*qSin(ag-M_PI/2.0));
//        q4=p1+QPointF(shapeWidth/2.0*qCos(ag-M_PI/2.0),shapeWidth/2.0*qSin(ag-M_PI/2.0));

        QPolygonF s_polygon;
        s_polygon << q1 << q2 << q3 << q4;
        shapePath.addPolygon(s_polygon);
    }

    drawShapePath.closeSubpath();
    shapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();


    // 符号描画の諸元（パスに含まず）
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
    textShift = centerSizeW / 2.0;
    if (len_mid < len_i) textShift = leftSizeW / 2.0;
    if (len_mid > len_j) textShift = rightSizeW / 2.0;
    textName = sectionName;
    if ( textAngle < 92.0 && textAngle > -88.0 ) {
        if (isLineDirVisible) textName += " >";
        textShift += shiftY;
    } else {
        textAngle = textAngle + 180.0;
        if (isLineDirVisible) textName += " <";
        textShift -= shiftY;
    }
}

void GirderHLineItem::createElevationShapePath()
{
    qreal len_i = myPartition.lenI;
    qreal len_j = myPartition.lenJ;
    qreal len_fi = myPartition.lenFi;
    qreal len_fj = myPartition.lenFj;
    qreal len_mid = ( myLenList.first() + myLenList.last() ) / 2.0;

    bool isRotS = ( myLineShape.renderType == RENDERSHAPE::SHAPE_I || myLineShape.renderType == RENDERSHAPE::SHAPE_T2 );
    qreal leftSizeW = ( isRotS ) ? myLineShape.leftSizeB : myLineShape.leftSizeH;
    qreal leftSizeL = -1.0;
    qreal centerSizeW = ( isRotS ) ? myLineShape.centerSizeB : myLineShape.centerSizeH;
    qreal centerSizeL = -1.0;
    qreal rightSizeW = ( isRotS ) ? myLineShape.rightSizeB : myLineShape.rightSizeH;
    qreal rightSizeL = -1.0;
    /*
        qreal leftSizeW=myLineShape.leftSizeH;
        qreal leftSizeL=-1.0;
        qreal centerSizeW=myLineShape.centerSizeH;
        qreal centerSizeL=-1.0;
        qreal rightSizeW=myLineShape.rightSizeH;
        qreal rightSizeL=-1.0;
    */
    qreal shiftY = myLineShape.shiftZ;

//
//    myLineShape.angle=10.0*M_PI/180.0;
//    shiftY=100.0;
//

    if ( qAbs(myLineShape.angle) > eps ) {

        ROTATEDSECTION rs = getRotatedSection(myLineShape.renderType, myLineShape.leftSizeB,
                                              myLineShape.leftSizeH, myLineShape.angle);
        leftSizeW = rs.rotW;
        leftSizeL = rs.rotL;

        rs = getRotatedSection(myLineShape.renderType, myLineShape.centerSizeB,
                               myLineShape.centerSizeH, myLineShape.angle);
        centerSizeW = rs.rotW;
        centerSizeL = rs.rotL;

        rs = getRotatedSection(myLineShape.renderType, myLineShape.rightSizeB,
                               myLineShape.rightSizeH, myLineShape.angle);
        rightSizeW = rs.rotW;
        rightSizeL = rs.rotL;
    }

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
    qreal shapeHeight = ( centerSizeW > eps ) ? centerSizeW + 100.0 : 200.0 ;

    for ( int i = 0 ; i < myPointList.count() - 1 ; i++ ) {

        QVector<QPointF> p1_list, p2_list, p3_list;
        QPointF p1 = myPointList.at(i);
        QPointF p2 = myPointList.at(i + 1);
        qreal l1 = myLenList.at(i);
        qreal l2 = myLenList.at(i + 1);
        qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());

        bool direct_ji = ( ag * 180.0 / M_PI >= 92.0 || ag * 180.0 / M_PI <= -88.0 );
        if ( direct_ji ) ag += M_PI;

        b1 = shiftY;
        b2 = -cur_B + shiftY;
        b3 = ( direct_ji ) ? b2 + cur_L : b1 - cur_L;
        cur_p1 = p1 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
        cur_p2 = p1 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
        cur_p3 = p1 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//        cur_p1=p1;
//        cur_p2=p1+QPointF(cur_B*qCos(ag-M_PI/2.0),cur_B*qSin(ag-M_PI/2.0));
        cur_x = l1;

        if ( l1 - eps < len_fi && len_fi < l2 + eps ) {
            QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_fi - l1) / (l2 - l1));
            cur_B = leftSizeW;
            cur_L = ( leftSizeL > 0 ) ? leftSizeL : 0.0;
            if ( cur_B < eps ) cur_B = 10.0;
            b1 = shiftY;
            b2 = -cur_B + shiftY;
            b3 = ( direct_ji ) ? b2 + cur_L : b1 - cur_L;
            cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
            cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
            cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//            cur_p1=p0;
//            cur_p2=p0+QPointF(cur_B*qCos(ag-M_PI/2.0),cur_B*qSin(ag-M_PI/2.0));
            cur_x = len_fi;
        }

        if ( l1 - eps < len_i && len_i < l2 + eps ) {

            if ( len_i - cur_x < eps ) {
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_i - l1) / (l2 - l1));
                cur_B = centerSizeW;
                cur_L = ( centerSizeL > 0 ) ? centerSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = shiftY;
                b2 = -cur_B + shiftY;
                b3 = ( direct_ji ) ? b2 + cur_L : b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p2=p0+QPointF(cur_B*qCos(ag-M_PI/2.0),cur_B*qSin(ag-M_PI/2.0));
            } else {
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_i - l1) / (l2 - l1));
                b1 = shiftY;
                b2 = -cur_B + shiftY;
                b3 = ( direct_ji ) ? b2 + cur_L : b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p2=p0+QPointF(cur_B*qCos(ag-M_PI/2.0),cur_B*qSin(ag-M_PI/2.0));
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                cur_B = centerSizeW;
                cur_L = ( centerSizeL > 0 ) ? centerSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = shiftY;
                b2 = -cur_B + shiftY;
                b3 = ( direct_ji ) ? b2 + cur_L : b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p2=p0+QPointF(cur_B*qCos(ag-M_PI/2.0),cur_B*qSin(ag-M_PI/2.0));
            }
            cur_x = len_i;
        }

        if ( l1 - eps < len_j && len_j < l2 + eps ) {

            if ( len_j - cur_x < eps ) {
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_j - l1) / (l2 - l1));
                cur_B = rightSizeW;
                cur_L = ( rightSizeL > 0 ) ? rightSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = shiftY;
                b2 = -cur_B + shiftY;
                b3 = ( direct_ji ) ? b2 + cur_L : b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p2=p0+QPointF(cur_B*qCos(ag-M_PI/2.0),cur_B*qSin(ag-M_PI/2.0));
            } else {
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_j - l1) / (l2 - l1));
                b1 = shiftY;
                b2 = -cur_B + shiftY;
                b3 = ( direct_ji ) ? b2 + cur_L : b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p2=p0+QPointF(cur_B*qCos(ag-M_PI/2.0),cur_B*qSin(ag-M_PI/2.0));
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                cur_B = rightSizeW;
                cur_L = ( rightSizeL > 0 ) ? rightSizeL : 0.0;
                if ( cur_B < eps ) cur_B = 10.0;
                b1 = shiftY;
                b2 = -cur_B + shiftY;
                b3 = ( direct_ji ) ? b2 + cur_L : b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p2=p0+QPointF(cur_B*qCos(ag-M_PI/2.0),cur_B*qSin(ag-M_PI/2.0));
            }
            cur_x = len_j;
        }

        if ( l1 - eps < len_fj && len_fj < l2 + eps ) {

            if ( len_fj - cur_x > eps ) {
                p1_list.append(cur_p1);
                p2_list.prepend(cur_p2);
                p3_list.append(cur_p3);
                QPointF p0 = CalcVector2D::interiorPoint(p1, p2, (len_fj - l1) / (l2 - l1));
                b1 = shiftY;
                b2 = -cur_B + shiftY;
                b3 = ( direct_ji ) ? b2 + cur_L : b1 - cur_L;
                cur_p1 = p0 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
                cur_p2 = p0 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
                cur_p3 = p0 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//                cur_p1=p0;
//                cur_p2=p0+QPointF(cur_B*qCos(ag-M_PI/2.0),cur_B*qSin(ag-M_PI/2.0));
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
//        if( l2-cur_x > eps && cur_B > eps ){
            p1_list.append(cur_p1);
            p2_list.prepend(cur_p2);
            p3_list.append(cur_p3);
            b1 = shiftY;
            b2 = -cur_B + shiftY;
            b3 = ( direct_ji ) ? b2 + cur_L : b1 - cur_L;
            cur_p1 = p2 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
            cur_p2 = p2 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
            cur_p3 = p2 + QPointF(b3 * qCos(ag + M_PI / 2.0), b3 * qSin(ag + M_PI / 2.0));
//            cur_p1=p2;
//            cur_p2=p2+QPointF(cur_B*qCos(ag-M_PI/2.0),cur_B*qSin(ag-M_PI/2.0));
            cur_x = l2;
            p1_list.append(cur_p1);
            p2_list.prepend(cur_p2);
            p3_list.append(cur_p3);
        }

        if ( !p1_list.isEmpty() ) {
            QPolygonF polygon( QVector<QPointF>(p1_list + p2_list) );
            polygon << p1_list.first();
            drawShapePath.addPolygon(polygon);
            if ( leftSizeL > 0.0 || centerSizeL > 0.0 || rightSizeL > 0.0 ) {
                for (int j = 1; j < p3_list.count(); j++) {
                    drawShapePath.moveTo(p3_list.at(j - 1));
                    drawShapePath.lineTo(p3_list.at(j));
                }
            }
        }

        b1 = shiftY;
        b2 = -shapeHeight + shiftY;
        q1 = p1 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
        q2 = p2 + QPointF(b1 * qCos(ag + M_PI / 2.0), b1 * qSin(ag + M_PI / 2.0));
        q3 = p2 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
        q4 = p1 + QPointF(b2 * qCos(ag + M_PI / 2.0), b2 * qSin(ag + M_PI / 2.0));
//        q1=p1;
//        q2=p2;
//        q3=p2+QPointF(shapeHeight*qCos(ag-M_PI/2.0),shapeHeight*qSin(ag-M_PI/2.0));
//        q4=p1+QPointF(shapeHeight*qCos(ag-M_PI/2.0),shapeHeight*qSin(ag-M_PI/2.0));

        QPolygonF s_polygon;
        s_polygon << q1 << q2 << q3 << q4;
        shapePath.addPolygon(s_polygon);
    }

    drawShapePath.closeSubpath();
    shapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();


    // 符号描画の諸元（パスに含まず）
    int idx_mid = 0;
    for (int i = 0; i < myLenList.count() - 1; i++) {
        if ( myLenList.at(i) - eps < len_mid && len_mid < myLenList.at(i + 1) ) {
            idx_mid = i;
            break;
        }
    }
    textAngle = qAtan2(myPointList.at(idx_mid + 1).y() - myPointList.at(idx_mid).y(),
                       myPointList.at(idx_mid + 1).x() - myPointList.at(idx_mid).x()) * 180.0 / M_PI;;
    qreal rr = (len_mid - myLenList.at(idx_mid)) / (myLenList.at(idx_mid + 1) - myLenList.at(idx_mid));
    textPoint = CalcVector2D::interiorPoint(myPointList.at(idx_mid), myPointList.at(idx_mid + 1), rr);
    textName = sectionName;
    textShift = 0.0;
    if ( textAngle < 92.0 && textAngle > -88.0 ) {
        if (isLineDirVisible) textName += " >";
        textShift += shiftY;
    } else {
        textAngle = textAngle + 180.0;
        if (isLineDirVisible) textName += " <";
        textShift -= shiftY;
    }
}


void GirderHLineItem::createErrorShape()
{
    myElementState = ELERROR;
    qreal errorWidth = 100.0;
    qreal shapeWidth = 200.0;

    drawShapePath = QPainterPath();
    setPolygonPath(drawShapePath, errorWidth);
    drawShapePath.closeSubpath();

    shapePath = QPainterPath();
    setPolygonPath(shapePath, shapeWidth);
    shapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();

    setErrorOrDummyTextProperty("ERROR(" + sectionName + ")", errorWidth);
}

void GirderHLineItem::createDummyShape()
{
    myElementState = ELDUMMY;

    qreal dummyWidth = 100.0;
    qreal shapeWidth = 200.0;
    drawShapePath = QPainterPath();
    setPolygonPath(drawShapePath, dummyWidth);
    drawShapePath.closeSubpath();

    shapePath = QPainterPath();
    setPolygonPath(shapePath, shapeWidth);
    shapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();

    QString text;
    if ( myElementType == ELGIRDER ) {
        text = "(dummyG)";
    } else if ( myElementType == ELBRACE ) {
        text = "(dummyV)";
    } else if ( myElementType == ELBEAM ) {
        text = "(dummyB)";
    } else if ( myElementType == ELMEMBERLOAD ) {
        text = "(dummyM)";
    }

    setErrorOrDummyTextProperty(text, dummyWidth);
}

} // namespace post3dapp
