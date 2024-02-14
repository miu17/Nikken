#include "customdraw_hlineitem.h"

#include <QDebug>
#include <QPainter>
#include <qmath.h>

#include "calc_vector2d.h"
#include "unified_data.h"
namespace post3dapp{
CustomDrawHLineItem::CustomDrawHLineItem(const Input2DViewSettings& setting,ELEMENTTYPE type, const QList<QPointF> &plist,
                                         const QList<qreal> &lenlist,
                                         const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                                         const MEMBERPARTITION &partition)
    : CustomDrawItem(setting, type, shape.sectionName,ITEMTYPE::HLINEITEM), myLineShape(shape), myItemView(view_type), myLenList(lenlist), myPartition(partition)
{
    isIncomplete = false;

    QPointF p0 = plist.first();
    myPointList.clear();
    qreal xmin = p0.x();
    qreal xmax = p0.x();
    qreal ymin = p0.y();
    qreal ymax = p0.y();
    Q_FOREACH (QPointF p1, plist) {
        if ( p1.x() < xmin ) xmin = p1.x();
        if ( p1.x() > xmax ) xmax = p1.x();
        if ( p1.y() < ymin ) ymin = p1.y();
        if ( p1.y() > ymax ) ymax = p1.y();
        myPointList.append(p1 - p0);
    }
    if ( (xmax - xmin) < 5000.0 ) {
        xmin = (xmax + xmin) / 2.0 - 2500.0;
        xmax = xmin + 5000.0;
    }
    if ( (ymax - ymin) < 5000.0 ) {
        ymin = (ymax + ymin) / 2.0 - 2500.0;
        ymax = ymin + 5000.0;
    }
    appRect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
}

QRectF CustomDrawHLineItem::approximatelyRect() const
{
    return appRect;
}

bool CustomDrawHLineItem::approximatelyContains(QPointF pp) const
{
    return appRect.contains(pp);
}

void CustomDrawHLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *)
{
    setPainter(painter, option);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(drawShapePath);

    if ( !isNameVisible ) return;
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->translate(textPoint.x(), -textPoint.y());

    painter->setFont(QFont("Consolas", FontSize(*painter)));
    QRect boundingRect = QFontMetrics(painter->font()).boundingRect(textName);
    QRectF rect = QRectF(-boundingRect.width() / 2.0, -textShift - boundingRect.height(), boundingRect.width(), boundingRect.height());

    painter->rotate(-textAngle); // rotateは最後
    painter->drawText(rect, Qt::AlignHCenter | Qt::AlignBottom, textName);
}


QList<QPointF> CustomDrawHLineItem::divideEquallyPoints(int ii) const
{
    if ( ii < 2 ) return QList<QPointF>();

    QList<qreal> div_lenlist;
    qreal div_len = myLenList.last() / qreal(ii);
    for (int i = 1; i < ii ; i++)
        div_lenlist.append( div_len * qreal(i) );

    QList<QPointF> div_plist;
    for (int i = 0; i < myLenList.count() - 1 ; i++) {
        qreal l1 = myLenList.at(i);
        qreal l2 = myLenList.at(i + 1);
        Q_FOREACH (qreal len, div_lenlist) {
            if ( l1 - 1.0e-3 < len && len < l2 + 1.0e-3 ) {
                QPointF p0 = CalcVector2D::interiorPoint(myPointList.at(i), myPointList.at(i + 1),
                                                         (len - l1) / (l2 - l1));
                div_plist.append(p0 + this->pos());
                div_lenlist.removeFirst();
            }
        }
    }

    return div_plist;
}

QList<QPointF> CustomDrawHLineItem::divideFromIJPoints(qreal fromI, qreal fromJ) const
{
    QList<QPointF> div_plist;
    fromJ = myLenList.last() - fromJ;
    for (int i = 0; i < myLenList.count() - 1 ; i++) {
        qreal l1 = myLenList.at(i);
        qreal l2 = myLenList.at(i + 1);
        if ( l1 < fromI && fromI < l2 + 1.0e-3 ) {
            QPointF p0 = CalcVector2D::interiorPoint(myPointList.at(i), myPointList.at(i + 1),
                                                     (fromI - l1) / (l2 - l1));
            div_plist.append(p0 + this->pos());
        }
        if ( l1 < fromJ && fromJ < l2 + 1.0e-3 ) {
            QPointF p0 = CalcVector2D::interiorPoint(myPointList.at(i), myPointList.at(i + 1),
                                                     (fromJ - l1) / (l2 - l1));
            div_plist.append(p0 + this->pos());
        }
    }

    return div_plist;
}

QPointF CustomDrawHLineItem::divideRatioPoint(qreal ir, qreal jr) const
{
    QPointF div_point;
    qreal div_len = myLenList.last() * ir / (ir + jr);

    for (int i = 0; i < myLenList.count() - 1 ; i++) {
        qreal l1 = myLenList.at(i);
        qreal l2 = myLenList.at(i + 1);
        if ( l1 < div_len && div_len < l2 + 1.0e-3 ) {
            div_point = CalcVector2D::interiorPoint(myPointList.at(i), myPointList.at(i + 1),
                                                    (div_len - l1) / (l2 - l1));
            break;
        }
    }

    return this->pos() + div_point;
}

QList<QPointF> CustomDrawHLineItem::divideFromIPoint(qreal fromI) const
{
    QList<QPointF> div_point;

    for (int i = 0; i < myLenList.count() - 1 ; i++) {
        qreal l1 = myLenList.at(i);
        qreal l2 = myLenList.at(i + 1);
        if ( l1 < fromI && fromI < l2 + 1.0e-3 ) {
            QPointF pp = CalcVector2D::interiorPoint(myPointList.at(i),
                                                     myPointList.at(i + 1),
                                                     (fromI - l1) / (l2 - l1));
            div_point.append(this->pos() + pp);
            break;
        }
    }

    return div_point;
}

QList<QPointF> CustomDrawHLineItem::divideFromJPoint(qreal fromJ) const
{
    QList<QPointF> div_point;
    fromJ = myLenList.last() - fromJ;

    for (int i = 0; i < myLenList.count() - 1 ; i++) {
        qreal l1 = myLenList.at(i);
        qreal l2 = myLenList.at(i + 1);
        if ( l1 < fromJ && fromJ < l2 + 1.0e-3 ) {
            QPointF pp = CalcVector2D::interiorPoint(myPointList.at(i),
                                                     myPointList.at(i + 1),
                                                     (fromJ - l1) / (l2 - l1));
            div_point.append(this->pos() + pp);
            break;
        }
    }

    return div_point;
}

bool CustomDrawHLineItem::isOnLinePoint(QPointF pp, bool exceptPoint) const
{
    if ( exceptPoint
            && ( CalcVector2D::length( myPointList.first() + this->pos() - pp ) < 1.0e-3 ) ) return false;
    for (int i = 0; i < myPointList.count() - 1 ; i++) {
        QPointF p1 = myPointList.at(i) + this->pos();
        QPointF p2 = myPointList.at(i + 1) + this->pos();
        if ( exceptPoint && ( CalcVector2D::length( p2 - pp ) < 1.0e-3 ) ) return false;
        if ( CalcVector2D::between(p1, p2, pp, 1.0e-3) ) return true;
    }
    return false;
}

qreal CustomDrawHLineItem::distanceToPoint(QPointF pp) const
{
    QPointF p1 = myPointList.at(0) + this->pos();
    QPointF p2 = myPointList.at(1) + this->pos();
    qreal min_len = CalcVector2D::segmentDistance(p1, p2, pp);

    for (int i = 1; i < myPointList.count() - 1 ; i++) {
        p1 = myPointList.at(i) + this->pos();
        p2 = myPointList.at(i + 1) + this->pos();
        qreal len = CalcVector2D::segmentDistance(p1, p2, pp);
        min_len = qMin( min_len, len );
    }
    return min_len;
}

QList<QLineF> CustomDrawHLineItem::getItemLines() const
{
    QList<QLineF> myLines;
    for (int i = 0; i < myPointList.count() - 1 ; i++) {
        QPointF p1 = myPointList.at(i) + this->pos();
        QPointF p2 = myPointList.at(i + 1) + this->pos();
        myLines.append(QLineF(p1, p2));
    }
    return myLines;
}

QLineF CustomDrawHLineItem::getIJLine() const
{
    return QLineF( myPointList.first() + this->pos(), myPointList.last() + this->pos() );
}



// angle -> 時計回りの回転角（rad）
ROTATEDSECTION CustomDrawHLineItem::getRotatedSection(RENDERSHAPE type, qreal b, qreal h,
                                                      qreal angle) const
{
    if (qAbs(angle) <= 1.0e-6){
        switch (type) {
        case RENDERSHAPE::SHAPE_H:
            return ROTATEDSECTION(b);
        default:
            return ROTATEDSECTION(h);
        }
    }

    qreal AG = angle;
    switch(type){
    case RENDERSHAPE::SHAPE_T1:
    case RENDERSHAPE::SHAPE_T2:
    case RENDERSHAPE::SHAPE_C1:
    case RENDERSHAPE::SHAPE_C2:
    case RENDERSHAPE::SHAPE_H:
        AG -= M_PI / 2.0;
        break;
    }

    switch (type) {
    case RENDERSHAPE::SHAPE_PIPE:
    {
        QPointF p1(-b / 2.0 * qCos(AG) + h / 2.0 * qSin(AG), b / 2.0 * qSin(AG) + h / 2.0 * qCos(AG));
        QPointF p2( b / 2.0 * qCos(AG) + h / 2.0 * qSin(AG), -b / 2.0 * qSin(AG) + h / 2.0 * qCos(AG));
        QPointF p3( b / 2.0 * qCos(AG) - h / 2.0 * qSin(AG), -b / 2.0 * qSin(AG) - h / 2.0 * qCos(AG));
        QPointF p4(-b / 2.0 * qCos(AG) - h / 2.0 * qSin(AG), b / 2.0 * qSin(AG) - h / 2.0 * qCos(AG));

        if ( qAbs(p1.y() - p2.y()) < 1.0e-3 ) {
            return ROTATEDSECTION(h);
        } else if ( qAbs(p1.x() - p2.x()) < 1.0e-3 ) {
            return ROTATEDSECTION(h, b);
        }

        if ( p1.y() > p2.y() ) {
            if ( p1.x() < p2.x() ) {
                return ROTATEDSECTION(qAbs(p1.y() - p3.y()), qAbs(p1.y() - p3.y()));
            } else {
                return ROTATEDSECTION(qAbs(p4.y() - p2.y()), qAbs(p4.y() - p1.y()));
            }
        } else {
            if ( p1.x() < p2.x() ) {
                return ROTATEDSECTION(qAbs(p2.y() - p4.y()), qAbs(p2.y() - p3.y()));
            } else {
                return ROTATEDSECTION(qAbs(p3.y() - p1.y()), qAbs(p3.y() - p4.y()));
            }
        }
    }break;
    case RENDERSHAPE::SHAPE_BOX:
    case RENDERSHAPE::SHAPE_H:
    case RENDERSHAPE::SHAPE_I:
    case RENDERSHAPE::SHAPE_C1:
    case RENDERSHAPE::SHAPE_C2:
    case RENDERSHAPE::SHAPE_C3:
    case RENDERSHAPE::SHAPE_C4:
    {
        QPointF p1(-b / 2.0 * qCos(AG) + h / 2.0 * qSin(AG), b / 2.0 * qSin(AG) + h / 2.0 * qCos(AG));
        QPointF p2( b / 2.0 * qCos(AG) + h / 2.0 * qSin(AG), -b / 2.0 * qSin(AG) + h / 2.0 * qCos(AG));
        QPointF p3(-h / 2.0 * qSin(AG), -h / 2.0 * qCos(AG));

        if ( qAbs(p1.y() - p2.y()) < 1.0e-3 ) {
            return ROTATEDSECTION(h);
        } else if ( qAbs(p1.x() - p2.x()) < 1.0e-3 ) {
            ROTATEDSECTION rs(b);
            if ( p1.y() < p2.y() ) rs.rotL = b / 2.0;
            return rs;
        }

        if ( p1.y() > p2.y() ) {
            if ( p1.x() < p2.x() ) {
                if ( p2.y() > p3.y() ) {
                    return ROTATEDSECTION(qAbs(p1.y() - p3.y()), qAbs(p1.y() - p2.y()));
                } else {
                    return ROTATEDSECTION(qAbs(p1.y() - p2.y()));
                }
            } else {
                if ( p3.y() > p1.y() ) {
                    return ROTATEDSECTION(qAbs(p3.y() - p2.y()), qAbs(p3.y() - p1.y()));
                } else {
                    return ROTATEDSECTION(qAbs(p1.y() - p2.y()));
                }
            }
        } else {
            if ( p1.x() < p2.x() ) {
                if ( p3.y() > p1.y() ) {
                    return ROTATEDSECTION(qAbs(p2.y() - p1.y()), qAbs(p2.y() - p3.y()));
                } else {
                    return ROTATEDSECTION(qAbs(p2.y() - p3.y()));
                }
            } else {
                if ( p2.y() > p3.y() ) {
                    return ROTATEDSECTION(qAbs(p2.y() - p1.y()), qAbs(p2.y() - p3.y()));
                } else {
                    return ROTATEDSECTION(qAbs(p3.y() - p1.y()));
                }
            }
        }
    }break;
    case RENDERSHAPE::SHAPE_T1:
    case RENDERSHAPE::SHAPE_T2:
    case RENDERSHAPE::SHAPE_T3:
    case RENDERSHAPE::SHAPE_T4:
    {
        QPointF p1(-b / 2.0 * qCos(AG) + h / 2.0 * qSin(AG), b / 2.0 * qSin(AG) + h / 2.0 * qCos(AG));
        QPointF p2( b / 2.0 * qCos(AG) + h / 2.0 * qSin(AG), -b / 2.0 * qSin(AG) + h / 2.0 * qCos(AG));
        QPointF p3(-h / 2.0 * qSin(AG), -h / 2.0 * qCos(AG));

        if ( qAbs(p1.y() - p2.y()) < 1.0e-3 ) {
            return ROTATEDSECTION(h);
        } else if ( qAbs(p1.x() - p2.x()) < 1.0e-3 ) {
            ROTATEDSECTION rs(b);
            if ( p1.y() < p2.y() ) rs.rotL = b / 2.0;
            return rs;
        }

        if ( p1.y() > p2.y() ) {
            if ( p1.x() < p2.x() ) {
                if ( p2.y() > p3.y() ) {
                    return ROTATEDSECTION(qAbs(p1.y() - p3.y()), qAbs(p1.y() - p2.y()));
                } else {
                    return ROTATEDSECTION(qAbs(p1.y() - p2.y()));
                }
            } else {
                if ( p3.y() > p1.y() ) {
                    return ROTATEDSECTION(qAbs(p3.y() - p2.y()), qAbs(p3.y() - p1.y()));
                } else {
                    return ROTATEDSECTION(qAbs(p1.y() - p2.y()));
                }
            }
        } else {
            if ( p1.x() < p2.x() ) {
                if ( p3.y() > p1.y() ) {
                    return ROTATEDSECTION(qAbs(p2.y() - p1.y()), qAbs(p2.y() - p3.y()));
                } else {
                    return ROTATEDSECTION(qAbs(p2.y() - p3.y()));
                }
            } else {
                if ( p2.y() > p3.y() ) {
                    return ROTATEDSECTION(qAbs(p2.y() - p1.y()), qAbs(p2.y() - p3.y()));
                } else {
                    return ROTATEDSECTION(qAbs(p3.y() - p1.y()));
                }
            }
        }
    }break;
    case RENDERSHAPE::SHAPE_L:
    {
        QPointF p1(-b / 2.0 * qCos(AG) + h / 2.0 * qSin(AG), b / 2.0 * qSin(AG) + h / 2.0 * qCos(AG));
        QPointF p2( b / 2.0 * qCos(AG) - h / 2.0 * qSin(AG), -b / 2.0 * qSin(AG) - h / 2.0 * qCos(AG));
        QPointF p3(-b / 2.0 * qCos(AG) - h / 2.0 * qSin(AG), b / 2.0 * qSin(AG) - h / 2.0 * qCos(AG));

        if ( qAbs(p2.y() - p3.y()) < 1.0e-3 ) {
            return ROTATEDSECTION(h);
        } else if ( qAbs(p2.x() - p3.x()) < 1.0e-3 ) {
            return ROTATEDSECTION(b);
        }

        if ( p2.y() > p3.y() ) {
            if ( p2.x() < p3.x() ) {
                return ROTATEDSECTION(qAbs(p2.y() - p1.y()), qAbs(p2.y() - p3.y()));
            } else {
                return ROTATEDSECTION(qAbs(p1.y() - p3.y()), qAbs(p1.y() - p2.y()));
            }
        } else {
            if ( p2.x() < p3.x() ) {
                return ROTATEDSECTION(qAbs(p3.y() - p1.y()));
            } else {
                return ROTATEDSECTION(qAbs(p1.y() - p2.y()));
            }
        }
    }break;
    case RENDERSHAPE::SHAPE_CROSS:
    case RENDERSHAPE::SHAPE_CROSSH:
        return ROTATEDSECTION(h);
    default:
        return ROTATEDSECTION();
    }

}

void CustomDrawHLineItem::setPolygonPath(QPainterPath& path, qreal width){
    for ( int i = 0; i < myPointList.count() - 1 ; i++ ) {
        const QPointF p1 = myPointList.at(i);
        const QPointF p2 = myPointList.at(i + 1);
        const qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());

        const QPointF q1 = p1 + QPointF(width / 2.0 * qCos(ag + M_PI / 2.0),
                                        width / 2.0 * qSin(ag + M_PI / 2.0));
        const QPointF q2 = p2 + QPointF(width / 2.0 * qCos(ag + M_PI / 2.0),
                                        width / 2.0 * qSin(ag + M_PI / 2.0));
        const QPointF q3 = p2 + QPointF(width / 2.0 * qCos(ag - M_PI / 2.0),
                                        width / 2.0 * qSin(ag - M_PI / 2.0));
        const QPointF q4 = p1 + QPointF(width / 2.0 * qCos(ag - M_PI / 2.0),
                                        width / 2.0 * qSin(ag - M_PI / 2.0));

        path.addPolygon(QPolygonF{QVector<QPointF>{q1, q2, q3, q4, q1}});
    }
}

void CustomDrawHLineItem::setErrorOrDummyTextProperty(const QString& name, qreal width){
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
    textShift = width / 2.0;
    textName = name;
    if ( textAngle >= 92.0 || textAngle <= -88.0 ) textAngle += 180.0;
}

} // namespace post3dapp
