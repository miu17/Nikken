#include "h_wallitem.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <qmath.h>

#include "calc_vector2d.h"

#include "unified_inputdata.h"
#include "define_section_struct.h"

namespace post3dapp{
WallHPlaneItem::WallHPlaneItem( const Input2DViewSettings &setting, ELEMENTTYPE type, const QList<QPointF> &plist,
                                const QString &str, bool onplane, qreal thickness, const QString &wpname)
    : CustomDrawHPlaneItem(setting,type, plist, str, onplane, thickness), wallOpenName(wpname)
{
    createShapePath();
    update(boundRect);
}

void WallHPlaneItem::createShapePath()
{
    if ( myPointList.count() < 3 ) return;

    /* 自己交差チェック */
    bool isError = ( myThickness < 1.0e-3 ) ;
    for ( int i = 0; i < myPointList.count() - 1; i++ ) {
        if ( isError ) break;
        QLineF l1 = QLineF(myPointList.at(i), myPointList.at(i + 1));
        for ( int j = i + 1; j < myPointList.count(); j++ ) {
            QLineF l2 = ( j == myPointList.count() - 1 ) ? QLineF(myPointList.at(j), myPointList.first())
                        : QLineF(myPointList.at(j), myPointList.at(j + 1)) ;
            if ( CalcVector2D::checkSegmentIntersection(l1, l2) ) {
                isError = true;
                break;
            }
        }
    }

    /* 時計回り・反時計回り、図心位置の算出 */
    qreal area = 0.0, cx = 0.0, cy = 0.0;
    for ( int i = 0; i < myPointList.count(); i++ ) {
        QPointF p1 = myPointList.at(i);
        QPointF p2 = ( i == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(i + 1) ;
        area += p1.x() * p2.y() - p2.x() * p1.y();
        cx += (p1.x() + p2.x()) * (p1.x() * p2.y() - p2.x() * p1.y());
        cy += (p1.y() + p2.y()) * (p1.x() * p2.y() - p2.x() * p1.y());
    }
    area = area / 2.0;

    if ( isError || qAbs(area) < 1.0e-5 ) {
        cx = 0.0;
        cy = 0.0;
        Q_FOREACH ( QPointF pp, myPointList ) {
            cx += pp.x();
            cy += pp.y();
        }
        cx = cx / qreal( myPointList.count() );
        cy = cy / qreal( myPointList.count() );
    } else {
        cx = cx / 6.0 / area;
        cy = cy / 6.0 / area;
    }

    /* 描画パスの作成 */
    drawShapePath = QPainterPath();
    drawShapePath.addPolygon( QPolygonF( myPointList.toVector() ) );
    drawShapePath.closeSubpath();

    qreal clen = 800.0;
    for ( int i = 0; i < myPointList.count(); i++ ) {

        QPointF p1 = ( i == 0 ) ? myPointList.last() : myPointList.at(i - 1) ;
        QPointF p2 = myPointList.at(i) ;
        QPointF p3 = ( i == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(i + 1) ;

        qreal th1 = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());
        qreal th2 = qAtan2(p3.y() - p2.y(), p3.x() - p2.x());
        qreal a1 = (th1 + th2 - M_PI) / 2.0;
        qreal a2 = a1 + M_PI;

        QPointF q1 = p2 + QPointF(clen * qCos(a1), clen * qSin(a1));

        int side = CalcVector2D::sideToSegment(p1, p2, q1);
        if ( area < 0.0 && side < 0 ) {
            q1 = p2 + QPointF(clen * qCos(a2), clen * qSin(a2));
        } else if ( area > 0.0 && side > 0 ) {
            q1 = p2 + QPointF(clen * qCos(a2), clen * qSin(a2));
        }

        drawShapePath.moveTo(p2);
        drawShapePath.lineTo(q1);
    }

    shapePath = QPainterPath();
    shapePath.addPolygon( QPolygonF( myPointList.toVector() ) );
    shapePath.closeSubpath();
    boundRect = shapePath.boundingRect();

    textPoint = QPointF(cx, cy);
    if ( sectionName.isEmpty() ) {
        myElementState = ELDUMMY;
        textName = "(dummyW)";
    } else if ( !isOnPlane || isError ) {
        myElementState = ELERROR;
        textName = "ERROR(" + sectionName + ")";
    } else {
        myElementState = ELNORMAL;
        textName = sectionName;
    }

    // 開口無しの場合 return
    if ( wallOpenName.isEmpty() ) return;

    int id_lb = 0, id_rb = 0, id_lt = 0, id_rt = 0;
    qreal r1max = -1.0e+5, r1min = 1.0e+5, r2max = -1.0e+5, r2min = 1.0e+5;
    for ( int i = 0; i < myPointList.count(); i++ ) {
        QPointF pp = myPointList.at(i) ;
        qreal r1 = pp.x() + pp.y();
        if ( r1 > r1max ) {
            id_rt = i;
            r1max = r1;
        }
        if ( r1 < r1min ) {
            id_lb = i;
            r1min = r1;
        }
        qreal r2 = pp.x() - pp.y();
        if ( r2 > r2max ) {
            id_rb = i;
            r2max = r2;
        }
        if ( r2 < r2min ) {
            id_lt = i;
            r2min = r2;
        }
    }

    QList<WallOpenParameter> o_values = UnifiedInputData::getInInstance()->getWallOpenValues(wallOpenName);
    QPainterPath op_path;

    Q_FOREACH ( WallOpenParameter wp, o_values ) {
        if ( wp.openType != WallOpenType::Type::NORMAL ) continue;

        QPointF o_p, v_x, v_y;
        if (!wp.isHorizontalFace && !wp.isVerticalFace){
            if ( wp.hPosition == WallOpenType::HPosition::LEFT && wp.vPosition == WallOpenType::VPosition::BOTTOM ) {
                o_p = myPointList.at(id_lb);
                if ( area > 0.0 ) {
                    QPointF pp = ( id_lb == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(id_lb + 1);
                    v_x = pp - o_p;
                } else {
                    QPointF pp = ( id_lb == 0 ) ? myPointList.last() : myPointList.at(id_lb - 1);
                    v_x = pp - o_p;
                }
                v_x /= CalcVector2D::length(v_x);
                v_y = QPointF(-v_x.y(), v_x.x());
            } else if ( wp.hPosition == WallOpenType::HPosition::LEFT && wp.vPosition != WallOpenType::VPosition::BOTTOM ) {
                o_p = myPointList.at(id_lt);
                if ( area > 0.0 ) {
                    QPointF pp = ( id_lt == 0 ) ? myPointList.last() : myPointList.at(id_lt - 1);
                    v_x = pp - o_p;
                } else {
                    QPointF pp = ( id_lt == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(id_lt + 1);
                    v_x = pp - o_p;
                }
                v_x /= CalcVector2D::length(v_x);
                v_y = QPointF(v_x.y(), -v_x.x());
            } else if ( wp.hPosition != WallOpenType::HPosition::LEFT && wp.vPosition == WallOpenType::VPosition::BOTTOM ) {
                o_p = myPointList.at(id_rb);
                if ( area > 0.0 ) {
                    QPointF pp = ( id_rb == 0 ) ? myPointList.last() : myPointList.at(id_rb - 1);
                    v_x = pp - o_p;
                } else {
                    QPointF pp = ( id_rb == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(id_rb + 1);
                    v_x = pp - o_p;
                }
                v_x /= CalcVector2D::length(v_x);
                v_y = QPointF(v_x.y(), -v_x.x());
            } else if (wp.hPosition != WallOpenType::HPosition::LEFT && wp.vPosition == WallOpenType::VPosition::BOTTOM){
                o_p = myPointList.at(id_rt);
                if ( area > 0.0 ) {
                    QPointF pp = ( id_rt == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(id_rt + 1);
                    v_x = pp - o_p;
                } else {
                    QPointF pp = ( id_rt == 0 ) ? myPointList.last() : myPointList.at(id_rt - 1);
                    v_x = pp - o_p;
                }
                v_x /= CalcVector2D::length(v_x);
                v_y = QPointF(-v_x.y(), v_x.x());
            }
        }


        QPointF p1 = myPointList.at(id_lb);
        QPointF p2 = myPointList.at(id_rb);
        QPointF p3 = myPointList.at(id_rt);
        QPointF p4 = myPointList.at(id_lt);

        if ( wp.width < 1.0e-5 && wp.height > 1.0e-5 ) {
            QLineF l1( o_p + v_y * wp.oy, o_p + v_y * wp.oy + v_x ); // 直線
            QLineF l2( o_p + v_y * (wp.oy + wp.height), o_p + v_y * (wp.oy + wp.height) + v_x ); // 直線
            qreal x1max = -1.0e+5, x1min = 1.0e+5, x2max = -1.0e+5, x2min = 1.0e+5;
            for ( int i = 0; i < myPointList.count(); i++ ) {
                QLineF ll = ( i == myPointList.count() - 1 ) ? QLineF(myPointList.at(i), myPointList.first())
                            : QLineF(myPointList.at(i), myPointList.at(i + 1));
                QList<QPointF> plist1 = CalcVector2D::intersectSegmentToLine(l1, ll);
                QList<QPointF> plist2 = CalcVector2D::intersectSegmentToLine(l2, ll);
                Q_FOREACH ( QPointF pp, plist1 ) {
                    if ( pp.x() > x1max ) {
                        x1max = pp.x();
                        p2 = pp;
                    }
                    if ( pp.x() < x1min ) {
                        x1min = pp.x();
                        p1 = pp;
                    }
                }
                Q_FOREACH ( QPointF pp, plist2 ) {
                    if ( pp.x() > x2max ) {
                        x2max = pp.x();
                        p3 = pp;
                    }
                    if ( pp.x() < x2min ) {
                        x2min = pp.x();
                        p4 = pp;
                    }
                }
            }
        } else if ( wp.width > 1.0e-5 && wp.height < 1.0e-5 ) {
            QLineF l1( o_p + v_x * wp.ox, o_p + v_x * wp.ox + v_y ); // 直線
            QLineF l2( o_p + v_x * (wp.ox + wp.width), o_p + v_x * (wp.ox + wp.width) + v_y ); // 直線
            qreal y1max = -1.0e+5, y1min = 1.0e+5, y2max = -1.0e+5, y2min = 1.0e+5;
            for ( int i = 0; i < myPointList.count(); i++ ) {
                QLineF ll = ( i == myPointList.count() - 1 ) ? QLineF(myPointList.at(i), myPointList.first())
                            : QLineF(myPointList.at(i), myPointList.at(i + 1));
                QList<QPointF> plist1 = CalcVector2D::intersectSegmentToLine(l1, ll);
                QList<QPointF> plist2 = CalcVector2D::intersectSegmentToLine(l2, ll);
                Q_FOREACH ( QPointF pp, plist1 ) {
                    if ( pp.y() > y1max ) {
                        y1max = pp.y();
                        p4 = pp;
                    }
                    if ( pp.y() < y1min ) {
                        y1min = pp.y();
                        p1 = pp;
                    }
                }
                Q_FOREACH ( QPointF pp, plist2 ) {
                    if ( pp.y() > y2max ) {
                        y2max = pp.y();
                        p3 = pp;
                    }
                    if ( pp.y() < y2min ) {
                        y2min = pp.y();
                        p2 = pp;
                    }
                }
            }
        } else if ( wp.width > 1.0e-5 && wp.height > 1.0e-5 ) {
            p1 = o_p + v_x * wp.ox + v_y * wp.oy;
            p2 = p1 + v_x * wp.width;
            p3 = p2 + v_y * wp.height;
            p4 = p1 + v_y * wp.height;
        }

        op_path.moveTo(p1);
        op_path.lineTo(p2);
        op_path.lineTo(p3);
        op_path.lineTo(p4);
        op_path.lineTo(p1);
        op_path.lineTo(p3);
        op_path.moveTo(p2);
        op_path.lineTo(p4);
    }

    drawShapePath.addPath(op_path);


    // スリット
    qreal s_l = 0.0, s_r = 0.0, s_b = 0.0, s_t = 0.0;
    Q_FOREACH ( WallOpenParameter wp, o_values ) {
        if ( wp.openType != WallOpenType::Type::SLIT ) continue;
        s_l = qMax(s_l, wp.slitL);
        s_r = qMax(s_r, wp.slitR);
        s_b = qMax(s_b, wp.slitB);
        s_t = qMax(s_t, wp.slitT);
    }

    qreal xmax = myPointList.first().x();
    qreal xmin = myPointList.first().x();
    qreal ymax = myPointList.first().y();
    qreal ymin = myPointList.first().y();
    Q_FOREACH ( QPointF pp, myPointList ) {
        xmax = qMax(xmax, pp.x());
        xmin = qMin(xmin, pp.x());
        ymax = qMax(ymax, pp.y());
        ymin = qMin(ymin, pp.y());
    }

    s_l = xmin + s_l;
    s_r = xmax - s_r;
    s_b = ymin + s_b;
    s_t = ymax - s_t;
//    QRectF crect( QPointF(s_l-0.01,s_b-0.01),QPointF(s_r+0.01,s_t+0.01) );

    for ( int i = 0; i < myPointList.count(); i++ ) {

        QPointF p1 = myPointList.at(i);
        QPointF p2 = ( i == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(i + 1);
//        if( crect.contains(p1) || crect.contains(p2) ) continue;

        qreal x1 = p1.x();
        qreal x2 = p2.x();
        qreal y1 = p1.y();
        qreal y2 = p2.y();

        bool isOk = false;
        if ( x1 < s_l - 0.01 && x2 < s_l - 0.01 ) isOk = true;
        if ( x1 > s_r + 0.01 && x2 > s_r + 0.01 ) isOk = true;
        if ( y1 < s_b - 0.01 && y2 < s_b - 0.01 ) isOk = true;
        if ( y1 > s_t + 0.01 && y2 > s_t + 0.01 ) isOk = true;
        if ( !isOk ) continue;

        QPointF p3 = (p1 + p2) / 2.0;
        p3 = ( area > 0.0 ) ? CalcVector2D::rotExtendPoint(p1, p3, -1.0 / 2.0 * M_PI, 20.0)
             : CalcVector2D::rotExtendPoint(p1, p3, 1.0 / 2.0 * M_PI, 20.0);
        QPointF p4 = ( area > 0.0 ) ? CalcVector2D::rotExtendPoint(p2, p1, 1.0 / 2.0 * M_PI, 20.0)
                     : CalcVector2D::rotExtendPoint(p2, p1, -1.0 / 2.0 * M_PI, 20.0);
        QPointF p5 = ( area > 0.0 ) ? CalcVector2D::rotExtendPoint(p1, p2, -1.0 / 2.0 * M_PI, 20.0)
                     : CalcVector2D::rotExtendPoint(p1, p2, 1.0 / 2.0 * M_PI, 20.0);
        QPointF p6 = ( area > 0.0 ) ? CalcVector2D::rotExtendPoint(p4, p3, -2.0 / 3.0 * M_PI, 250.0)
                     : CalcVector2D::rotExtendPoint(p4, p3, 2.0 / 3.0 * M_PI, 250.0);
        QPointF p7 = ( area > 0.0 ) ? CalcVector2D::rotExtendPoint(p4, p3, -1.0 / 3.0 * M_PI, 250.0)
                     : CalcVector2D::rotExtendPoint(p4, p3, 1.0 / 3.0 * M_PI, 250.0);
        wallSlitPath.moveTo(p4);
        wallSlitPath.lineTo(p5);
        wallSlitPath.moveTo(p3);
        wallSlitPath.lineTo(p6);
        wallSlitPath.lineTo(p7);
        wallSlitPath.lineTo(p3);
    }

//    textName+=" +"+wallOpenName;
}

void WallHPlaneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    setPainter(painter, option);
//    QPen pen = painter->pen();
//    pen.setWidth(pen.width()*1000+2);
//    painter->setPen(pen);

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(drawShapePath);

    QColor pcol = painter->pen().color();
    painter->setBrush(QBrush(pcol));
    painter->drawPath(wallSlitPath);

    setPainter(painter, option);

    if ( !isNameVisible ) return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->translate(textPoint.x(), -textPoint.y());

    painter->setFont(QFont("Consolas", FontSize(*painter)));
    QRect boundingRect = QFontMetrics(painter->font()).boundingRect(textName);
    painter->drawText(QRect(-boundingRect.width() / 2.0, -boundingRect.height(), boundingRect.width(), boundingRect.height()),
                      Qt::AlignCenter, textName);
    if ( !wallOpenName.isEmpty() ) {
        boundingRect = QFontMetrics(painter->font()).boundingRect("(" + wallOpenName + ")");
        painter->drawText(QRect(-boundingRect.width() / 2.0,   0, boundingRect.width(), boundingRect.height()),
                          Qt::AlignCenter, "(" + wallOpenName + ")");
    }
}
} // namespace post3dapp
