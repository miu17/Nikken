#include "calc_vector2d.h"

#include <QDebug>
namespace post3dapp{
/* 線分と線分の交差判定し、真であればplistに交点が追加される */
QList<QPointF> CalcVector2D::intersectCheck(const QLineF &l1, const QLineF &l2, qreal eps)
{
    QList<QPointF> plist;

    // X座標によるラフチェック
    if ( l1.x1() >= l1.x2() ) {
        if ( (l1.x1() < l2.x1() && l1.x1() < l2.x2()) ||
                (l1.x2() > l2.x1() && l1.x2() > l2.x2()) ) return plist;
    } else {
        if ( (l1.x2() < l2.x1() && l1.x2() < l2.x2()) ||
                (l1.x1() > l2.x1() && l1.x1() > l2.x2()) ) return plist;
    }

    // Y座標によるラフチェック
    if ( l1.y1() >= l1.y2() ) {
        if ( (l1.y1() < l2.y1() && l1.y1() < l2.y2()) ||
                (l1.y2() > l2.y1() && l1.y2() > l2.y2()) ) return plist;
    } else {
        if ( (l1.y2() < l2.y1() && l1.y2() < l2.y2()) ||
                (l1.y1() > l2.y1() && l1.y1() > l2.y2()) ) return plist;
    }

    // 線分上に点が存在する場合（線分同士が重なる状態を含む）
    // i端側のみチェック　×between(p1,p2,p4) ×between(p3,p4,p2)
    if ( between(l1.p1(), l1.p2(), l2.p1()) ) plist.append(l2.p1());
    if ( between(l2.p1(), l2.p2(), l1.p1()) && length(l1.p1() - l2.p1()) > eps ) plist.append(l1.p1());
    if ( plist.count() > 0 ) return plist;

    // 交差判定し、真であれば交点を追加する
    if ( (cross(l1.p2() - l1.p1(), l2.p1() - l1.p1())*cross(l1.p2() - l1.p1(),
                                                            l2.p2() - l1.p1()) < -eps) &&
            (cross(l2.p2() - l2.p1(), l1.p1() - l2.p1())*cross(l2.p2() - l2.p1(), l1.p2() - l2.p1()) < -eps) ) {
        QPointF pp = intersectPoint(l1.p1(), l1.p2(), l2.p1(), l2.p2());
        plist.append(pp);
    }
    return plist;
}


/* 直線と線分の交差判定をし、真であればplistに交点が追加される  l1:直線　l2:線分 */
QList<QPointF> CalcVector2D::intersectSegmentToLine(const QLineF &l1, const QLineF &l2, qreal eps)
{
    QList<QPointF> plist;

    // 交点の有り無しチェック（直線に対する側判定）
    int i_side = sideToSegment(l1.p1(), l1.p2(), l2.p1(), eps);
    int j_side = sideToSegment(l1.p1(), l1.p2(), l2.p2(), eps);
    if ( i_side * j_side > 0 ) return plist;
    if ( i_side * j_side == 0 ) {
        if ( i_side == 0 ) plist.append(l2.p1());
        if ( j_side == 0 ) plist.append(l2.p2());
        return plist;
    }

    qreal dev = (l1.y2() - l1.y1()) * (l2.x2() - l2.x1()) - (l1.x2() - l1.x1()) * (l2.y2() - l2.y1());
    if ( qAbs(dev) < eps ) return plist;
    qreal d1 = (l2.y1() * l2.x2() - l2.x1() * l2.y2());
    qreal d2 = (l1.y1() * l1.x2() - l1.x1() * l1.y2());

    plist.append(QPointF( ( d1 * (l1.x2() - l1.x1()) - d2 * (l2.x2() - l2.x1()) ) / dev,
                          ( d1 * (l1.y2() - l1.y1()) - d2 * (l2.y2() - l2.y1()) ) / dev ));
    return plist;
}


/* 線分p1-p2に点p3が含まれるか否か */
bool CalcVector2D::between(const QPointF &p1, const QPointF &p2, const QPointF &p3, qreal eps)
{
    if ( distance(p1, p2, p3) > eps ) return false;
    if ( qAbs( p1.x() - p2.x() ) > eps  )
        return ( ( ( p1.x() - p3.x() <  eps ) && ( p3.x() - p2.x() <  eps ) ) ||
                 ( ( p1.x() - p3.x() > -eps ) && ( p3.x() - p2.x() > -eps ) ) );
    else return ( ( ( p1.y() - p3.y() <  eps ) && ( p3.y() - p2.y() <  eps ) ) ||
                      ( ( p1.y() - p3.y() > -eps ) && ( p3.y() - p2.y() > -eps ) ) );
}

/* 線分p1-p2と点p3の距離 */
qreal CalcVector2D::segmentDistance(const QPointF &p1, const QPointF &p2, const QPointF &p3,
                                    qreal eps)
{
    if ( dot(p2 - p1, p3 - p1) < eps ) return length(p3 - p1);
    if ( dot(p1 - p2, p3 - p2) < eps ) return length(p3 - p2);
    return distance(p1, p2, p3) ;
}

/* 線分p1-p2と点p3の距離（正負付き） 進行方向右側が正、左側が負 */
qreal CalcVector2D::signedSegmentDistance(const QPointF &p1, const QPointF &p2, const QPointF &p3,
                                          qreal eps)
{
    if ( dot(p2 - p1, p3 - p1) < eps ) {
        return ( cross(p2 - p1, p3 - p1) > eps ) ? -length(p3 - p1) : length(p3 - p1) ;
    }
    if ( dot(p1 - p2, p3 - p2) < eps ) {
        return ( cross(p1 - p2, p3 - p2) > eps ) ? length(p3 - p2) : -length(p3 - p2) ;
    }
    return signedDistance(p1, p2, p3) ;
}

/* 有向線分p1-p2と点p3の関係 進行方向右側であれば+1、左側であれば-1、線上であれば0 */
int CalcVector2D::sideToSegment(const QPointF &p1, const QPointF &p2, const QPointF &pp, qreal eps)
{
    qreal dd = pp.x() * (p1.y() - p2.y()) + p1.x() * (p2.y() - pp.y()) + p2.x() * (pp.y() - p1.y());
    if ( dd > eps ) {
        return -1;    // 左
    } else if ( dd < -eps ) {
        return 1;    // 右
    } else {
        return 0;    // 線上
    }
}

/* 線分p1-p2と点p3の最近傍点　*/
QPointF CalcVector2D::perpPointToSegment(const QPointF &p1, const QPointF &p2, const QPointF &p3,
                                         qreal eps)
{
    if ( dot(p2 - p1, p3 - p1) < eps ) return p1;
    if ( dot(p1 - p2, p3 - p2) < eps ) return p2;

    return perpPointToLine(p1, p2, p3);
}

/* 直線p1-p2と点p3の最近傍点　*/
QPointF CalcVector2D::perpPointToLine(const QPointF &p1, const QPointF &p2, const QPointF &p3,
                                      qreal eps)
{
    if ( qAbs( p1.x() - p2.x() ) < eps ) return QPointF(p1.x(), p3.y());
    if ( qAbs( p1.y() - p2.y() ) < eps ) return QPointF(p3.x(), p1.y());

    qreal m = (p1.y() - p2.y()) / (p1.x() - p2.x());
    return QPointF( ( m * p1.x() + p3.x() / m - p1.y() + p3.y() ) / ( m + 1. / m ),
                    ( m * p3.y() + p1.y() / m - p1.x() + p3.x() ) / ( m + 1. / m ) ) ;
}

/* 線分p1-p2と線分p3-p4が交差する場合、交差点を算出 */
QPointF CalcVector2D::intersectPoint(const QPointF &p1, const QPointF &p2,
                                     const QPointF &p3, const QPointF &p4)
{
    qreal d1 = qAbs(cross(p4 - p3, p1 - p3));
    qreal d2 = qAbs(cross(p4 - p3, p2 - p3));
    qreal t = d1 / (d1 + d2);

    return ( p1 + (p2 - p1) * t ) ;
}

/* 点p1->p2を長さdだけ延長した点を返す */
QPointF CalcVector2D::extendPoint(const QPointF &p1, const QPointF &p2, qreal d)
{
    qreal d0 = length(p1 - p2);
    if (qAbs(d0) < 1.0e-5) return p2;
    qreal xx = ( -d * p1.x() + (d0 + d) * p2.x() ) / d0;
    qreal yy = ( -d * p1.y() + (d0 + d) * p2.y() ) / d0;
    return QPointF(xx, yy);
}

/* 点p1,p2を内分比rr=m/(m+n)で内分した点を返す */
QPointF CalcVector2D::interiorPoint(const QPointF &p1, const QPointF &p2, qreal rr)
{
    qreal xx = (1.0 - rr) * p1.x() + rr * p2.x();
    qreal yy = (1.0 - rr) * p1.y() + rr * p2.y();
    return QPointF(xx, yy);
}

/* 点ptが多角形polyの内部にあるか否か */
bool CalcVector2D::isInPolygon( const QPolygonF &poly, const QPointF &pt, qreal eps )
{
    QPointF p0 = poly.first();
    qreal sum_t = 0.0;

    for ( int i = 0; i < poly.count(); i++ ) {
        QPointF p1 = poly.at(i);
        if ( (p1 - pt).manhattanLength() < eps ) return true; // 頂点と一致
        if ( i == poly.count() - 1 ) {
            if ( (p1 - p0).manhattanLength() < eps ) break;
        }
        QPointF p2 = ( i == poly.count() - 1 ) ? p0 : poly.at(i + 1);
        if ( between(p1, p2, pt, eps) ) return true; // 多角形の辺上

        qreal dt = dot( p1 - pt, p2 - pt );
        qreal cr = cross( p1 - pt, p2 - pt );
        sum_t += qAtan2(cr, dt);
    }
    return ( qAbs(sum_t) > 0.1 ); // 外部であれば0度、内部であれば+-360度
}


/* 線分を長方形でクリッピングし、線分を返す */
QLineF CalcVector2D::clipSegment(const QLineF &line1, const QRectF &rect)
{
    QPointF p1 = line1.p1();
    QPointF p2 = line1.p2();

    bool isContainP1 = rect.contains(p1);
    bool isContainP2 = rect.contains(p2);

    if ( isContainP1 && isContainP2 ) return line1;

    qreal eps = 1.0e-5;
    QPointF q1, q2;

    if ( qAbs(p1.x() - p2.x()) < eps ) {
        qreal ry1 = rect.y();
        qreal ry2 = ry1 + rect.height();
        if ( isContainP1 ) {
            q1 = p1;
            q2 = ( p1.y() > p2.y() ) ? QPointF(p2.x(), ry1) : QPointF(p2.x(), ry2);
        } else {
            q2 = p2;
            q1 = ( p1.y() > p2.y() ) ? QPointF(p1.x(), ry2) : QPointF(p1.x(), ry1);
        }
        return QLineF(q1, q2);
    }

    qreal c_m = (p1.y() - p2.y()) / (p1.x() - p2.x());
    qreal c_n = p1.y() - p1.x() * c_m;

    QLineF line2 = clipLine(c_m, c_n, rect);

    if ( line2.isNull() ) return QLineF();

    QPointF p3 = ( line1.p1().x() < line1.p2().x() ) ? line1.p1() : line1.p2() ;
    QPointF p4 = ( line1.p1().x() < line1.p2().x() ) ? line1.p2() : line1.p1() ;

    if ( p1.x() < p2.x() ) {
        if ( p1.x() >= p4.x() || p2.x() <= p3.x() ) return QLineF();
        q1 = ( p1.x() < p3.x() ) ? p3 : p1 ;
        q2 = ( p2.x() < p4.x() ) ? p2 : p4 ;
    } else {
        if ( p2.x() >= p4.x() || p1.x() <= p3.x() ) return QLineF();
        q1 = ( p1.x() < p4.x() ) ? p1 : p4 ;
        q2 = ( p2.x() < p3.x() ) ? p3 : p2 ;
    }
    return QLineF(q1, q2);
}

/* 直線y=mx+n を長方形でクリッピングし、線分を返す */
QLineF CalcVector2D::clipLine(qreal c_m, qreal c_n, const QRectF &rect)
{
    qreal rx1 = rect.x();
    qreal ry1 = rect.y();
    qreal rx2 = rx1 + rect.width();
    qreal ry2 = ry1 + rect.height();

    QPointF p1, p2;

    if ( qAbs(c_m) < 1.0e-5 ) {
        if ( c_n < ry1 || c_n > ry2 ) return QLineF();
        p1 = QPointF( rx1, c_n );
        p2 = QPointF( rx2, c_n );
        return QLineF(p1, p2);
    }

    qreal y1 = c_m * rx1 + c_n;
    qreal y2 = c_m * rx2 + c_n;
    if ( y1 > ry2 ) {
        if ( y2 > ry2 ) return QLineF();
        p1 = QPointF( (ry2 - c_n) / c_m, ry2 );
        if ( y2 <= ry2 && y2 >= ry1 ) {
            p2 = QPointF( rx2, y2 );
        } else if ( y2 < ry1 ) {
            p2 = QPointF( (ry1 - c_n) / c_m, ry1 );
        }
    } else if ( y1 < ry1 ) {
        if ( y2 < ry1 ) return QLineF();
        p1 = QPointF( (ry1 - c_n) / c_m, ry1 );
        if ( y2 <= ry2 && y2 >= ry1 ) {
            p2 = QPointF( rx2, y2 );
        } else if ( y2 > ry2 ) {
            p2 = QPointF( (ry2 - c_n) / c_m, ry2 );
        }
    } else {
        p1 = QPointF( rx1, y1 );
        if ( y2 > ry2 ) {
            p2 = QPointF( (ry2 - c_n) / c_m, ry2 );
        }
        if ( y2 <= ry2 && y2 >= ry1 ) {
            p2 = QPointF( rx2, y2 );
        } else if ( y2 < ry1 ) {
            p2 = QPointF( (ry1 - c_n) / c_m, ry1 );
        }
    }
    return QLineF(p1, p2);
}

/* ベクトルp1とベクトルp2の外積 */
qreal CalcVector2D::cross(const QPointF &p1, const QPointF &p2)
{
    return ( p1.x() * p2.y() - p1.y() * p2.x() );
}

/* ベクトルp1とベクトルp2の内積 */
qreal CalcVector2D::dot(const QPointF &p1, const QPointF &p2)
{
    return ( p1.x() * p2.x() + p1.y() * p2.y() );
}

/* p1-p2を結ぶ直線と点p3の距離 */
qreal CalcVector2D::distance(const QPointF &p1, const QPointF &p2, const QPointF &p3)
{
    return qAbs( cross(p2 - p1, p3 - p1) ) / length(p2 - p1);
}

/* p1-p2を結ぶ直線と点p3の距離（正負付き） 進行方向右側が正、左側が負 */
qreal CalcVector2D::signedDistance(const QPointF &p1, const QPointF &p2, const QPointF &p3)
{
    return ( -cross(p2 - p1, p3 - p1) ) / length(p2 - p1);
}

/* ベクトルp1の長さ */
qreal CalcVector2D::length(const QPointF &p1)
{
    return qSqrt( qPow(p1.x(), 2.0) + qPow(p1.y(), 2.0) );
}

/* 点列の長さ */
qreal CalcVector2D::totalLength(const QList<QPointF> &pointList)
{
    qreal sum_len = 0.0;
    for (int i = 0; i < pointList.count() - 1; i++)
        sum_len += length( pointList.at(i + 1) - pointList.at(i) );
    return sum_len;
}

/* 指定の点列における長さ方向の値xxに対応する点を返す */
QPointF CalcVector2D::lengthToPoint(const QList<QPointF> &pointList, qreal xx )
{
    if ( pointList.isEmpty() ) return QPointF();
    if ( pointList.count() == 1 ) return pointList.first();

    QList<qreal> lenlist;
    qreal len = 0.0;
    lenlist.append(len);
    int idx = -1;
    if ( xx >= len ) idx = 0;
    for (int i = 1; i < pointList.count(); i++) {
        len += length(pointList.at(i) - pointList.at(i - 1));
        lenlist.append(len);
        if ( xx >= len ) idx = i;
    }

    if (idx < 0) {
        QPointF p1 = pointList.first();
        QPointF p2 = pointList.at(1);
        return extendPoint(p2, p1, qAbs(xx));
    } else if (idx == pointList.count() - 1) {
        QPointF p1 = pointList.at(pointList.count() - 2);
        QPointF p2 = pointList.last();
        return extendPoint(p1, p2, qAbs(xx - lenlist.last()));
    } else {
        qreal len1 = lenlist.at(idx);
        qreal len2 = lenlist.at(idx + 1);
        qreal rr = (xx - len1) / (len2 - len1);
        return interiorPoint(pointList.at(idx), pointList.at(idx + 1), rr);
    }

    return QPointF();
}

/* XY座標系の点を長さ座標系（X軸）の値に変換  ※閉じた通りの場合は考慮していない　*/
/*
qreal CalcVector2D::pointToElvX(const QList<QPointF> &pointList, QPointF xy )
{
    if( pointList.isEmpty() ) return 0.0;
    if( pointList.count()==1 ) return length( pointList.first()-xy );

    QList<qreal> lenlist;
    qreal len=0.0;
    lenlist.append(len);

    int idx=1;
    qreal min_dis=1.0e+10;

    // 線分上にあれば距離をそのまま返す、なければ線分との距離を保持
    for(int i=1; i<pointList.count(); i++){
        if( between(pointList.at(i),pointList.at(i-1),xy) ){
            return len+length(xy-pointList.at(i-1)) ;
        }
        len+=length(pointList.at(i)-pointList.at(i-1));
        lenlist.append(len);

        qreal dis=segmentDistance(pointList.at(i-1),pointList.at(i),xy);
        if( dis < min_dis ){ idx=i; min_dis=dis; }
    }

    // 開始線分あるいは終了線分と最も近かった場合、線分範囲内に入るか否かの判断
    if( idx==1 ){
        qreal dis=distance(pointList.first(),pointList.at(1),xy);
        if( dis < min_dis ) idx=-1;
    }else if( idx==pointList.count()-1 ){
        qreal dis=distance(pointList.at(pointList.count()-2),pointList.last(),xy);
        if( dis < min_dis ) idx=-2;
    }

    qreal xx;
    if( idx > 0){
        QPointF cp=perpPointToSegment(pointList.at(idx-1),pointList.at(idx),xy);
        xx=lenlist.at(idx-1)+length(cp-pointList.at(idx-1));
    }else if( pointList.count()==2 ){
        QPointF cp=perpPointToLine(pointList.first(),pointList.last(),xy);
        qreal x1=length(cp-pointList.first());
        qreal x2=length(cp-pointList.last());
        if( x1 < x2 ){
            xx=lenlist.first()-x1;
        }else{
            xx=lenlist.last()+x2;
        }
    }else if( idx==-1 ){
        QPointF cp=perpPointToLine(pointList.first(),pointList.at(1),xy);
        qreal x1=length(cp-pointList.first());
        qreal x2=length(cp-pointList.at(1));
        if( x1 < x2 ){
            xx=lenlist.first()-x1;
        }else{
            xx=lenlist.at(1);
        }
    }else if( idx==-2 ){
        QPointF cp=perpPointToLine(pointList.at(pointList.count()-2),pointList.last(),xy);
        qreal x1=length(cp-pointList.last());
        qreal x2=length(cp-pointList.at(pointList.count()-2));
        if( x1 < x2 ){
            xx=lenlist.last()+x1;
        }else{
            xx=lenlist.at(lenlist.count()-2);
        }
    }
    return xx;
}
*/

/* 軸組座標系（X,z）の値をXY座標系の点に変換 */
QPointF CalcVector2D::elvXZToGlobalXY(const QList<QPointF> &pointList, QPointF ep, bool isOpen )
{
    if ( pointList.isEmpty() ) return QPointF();
    if ( pointList.count() == 1 ) return pointList.first();

    QList<QPointF> framePoints = pointList;
    if ( !isOpen ) framePoints.prepend(pointList.at(pointList.count() - 2));

    qreal eps = 1.0e-3;
    QList<qreal> lenlist;
    qreal len = ( isOpen ) ? 0.0 : -length(framePoints.at(1) - framePoints.at(0)) ;
    lenlist.append(len);
    int idx = -1;
    if ( ep.x() > len - eps ) idx = 0;
    for (int i = 1; i < framePoints.count(); i++) {
        len += length(framePoints.at(i) - framePoints.at(i - 1));
        lenlist.append(len);
        if ( ep.x() > len - eps ) idx = i;
    }

    if ( !isOpen && ( idx < 1 || idx == framePoints.count() - 1 ) ) return framePoints.at(1);

    if (idx < 0) {
        QPointF p1 = framePoints.first();
        QPointF p2 = framePoints.at(1);
        if ( qAbs(ep.y()) < eps ) return extendPoint(p2, p1, qAbs(ep.x()));
        QPointF p3 = extendPoint(p2, p1, qAbs(ep.x()));
        if ( (p2 - p3).manhattanLength() > eps ) return rotExtendPoint(p2, p3, -M_PI / 2.0, ep.y());
        return rotExtendPoint(p1, p3, -M_PI / 2.0, ep.y());
    } else if (idx == framePoints.count() - 1) {
        QPointF p1 = framePoints.at(framePoints.count() - 2);
        QPointF p2 = framePoints.last();
        if ( qAbs(ep.y()) < eps ) return extendPoint(p1, p2, qAbs(ep.x() - lenlist.last()));
        QPointF p3 = extendPoint(p1, p2, qAbs(ep.x() - lenlist.last()));
        if ( (p3 - p2).manhattanLength() > eps ) return rotExtendPoint(p2, p3, M_PI / 2.0, ep.y());
        return rotExtendPoint(p1, p3, M_PI / 2.0, ep.y());
    } else {
        qreal len1 = lenlist.at(idx);
        qreal len2 = lenlist.at(idx + 1);
        qreal rr = (ep.x() - len1) / (len2 - len1);
        if ( qAbs(ep.y()) < eps ) return interiorPoint(framePoints.at(idx), framePoints.at(idx + 1), rr);
        QPointF p1 = framePoints.at(idx);
        QPointF p2 = framePoints.at(idx + 1);
        QPointF p3 = interiorPoint(p1, p2, rr);
        if ( (p3 - p1).manhattanLength() < eps ) {
            if (idx == 0) return rotExtendPoint(p2, p3, -M_PI / 2.0, ep.y());
            qreal p0_ang = qAtan2(p1.y() - framePoints.at(idx - 1).y(), p1.x() - framePoints.at(idx - 1).x());
            qreal p1_ang = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());
            qreal th1 = (p0_ang + p1_ang - M_PI) / 2.0;
            QPointF p4 = p3 + QPointF(qCos(th1), qSin(th1));
            if ( sideToSegment(framePoints.at(idx - 1), p3, p4) < 0 ) th1 += M_PI;
            return p3 + QPointF(ep.y() * qCos(th1), ep.y() * qSin(th1));
        } else if ( (p3 - p2).manhattanLength() < eps ) {
            if (idx == framePoints.count() - 2) return rotExtendPoint(p1, p3, M_PI / 2.0, ep.y());
            qreal p1_ang = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());
            qreal p2_ang = qAtan2(framePoints.at(idx + 2).y() - p2.y(), framePoints.at(idx + 2).x() - p2.x());
            qreal th1 = (p1_ang + p2_ang - M_PI) / 2.0;
            QPointF p4 = p3 + QPointF(qCos(th1), qSin(th1));
            if ( sideToSegment(p3, framePoints.at(idx + 2), p4) < 0 ) th1 += M_PI;
            return p3 + QPointF(ep.y() * qCos(th1), ep.y() * qSin(th1));
        } else {
            return rotExtendPoint(p1, p3, M_PI / 2.0, ep.y());
        }
    }

    return QPointF();
}

/* XY座標系の点を軸組座標系（X,Z）の値に変換 */
QPointF CalcVector2D::globalXYToElvXZ(const QList<QPointF> &pointList, QPointF xy, bool isOpen)
{
//    qDebug()<<"init globalXYToElvXZ";
    if ( pointList.isEmpty() ) return QPointF();
    if ( pointList.count() == 1 ) return QPointF( 0.0, length( pointList.first() - xy ));

    QList<qreal> lenlist;
    qreal len = 0.0;
    lenlist.append(len);

    int idx = 1;
    qreal min_dis = 1.0e+10;

    // 線分上にあれば距離をそのまま返す、なければ線分との距離を保持
//    qDebug()<<"calc line distance";
    for (int i = 1; i < pointList.count(); i++) {
        if ( between(pointList.at(i), pointList.at(i - 1), xy) ) {
            return QPointF(len + length(xy - pointList.at(i - 1)), 0.0) ;
        }
        len += length(pointList.at(i) - pointList.at(i - 1));
        lenlist.append(len);

        qreal dis = signedSegmentDistance(pointList.at(i - 1), pointList.at(i), xy);
        if ( qAbs(dis) < qAbs(min_dis) ) {
            idx = i;
            min_dis = dis;
        }
    }

    // 開始線分あるいは終了線分と最も近かった場合、線分範囲内に入るか否かの判断
//    qDebug()<<"judge inter segment";
    if ( idx == 1 ) {
        qreal dis = distance(pointList.first(), pointList.at(1), xy);
        if ( dis < qAbs(min_dis) ) idx = -1;
    } else if ( idx == pointList.count() - 1 ) {
        qreal dis = distance(pointList.at(pointList.count() - 2), pointList.last(), xy);
        if ( dis < qAbs(min_dis) ) idx = -2;
    }

    qreal xx, zz;
    if ( idx > 0) {
        QPointF cp = perpPointToSegment(pointList.at(idx - 1), pointList.at(idx), xy);
        xx = lenlist.at(idx - 1) + length(cp - pointList.at(idx - 1));
        zz = min_dis;
    } else if ( pointList.count() == 2 ) {
        QPointF cp = perpPointToLine(pointList.first(), pointList.last(), xy);
        qreal x1 = length(cp - pointList.first());
        qreal x2 = length(cp - pointList.last());
        if ( x1 < x2 ) {
            xx = lenlist.first() - x1;
        } else {
            xx = lenlist.last() + x2;
        }
        zz = signedDistance(pointList.first(), pointList.last(), xy);
    } else if ( !isOpen && idx == -1 ) {
        QPointF cp = perpPointToLine(pointList.first(), pointList.at(1), xy);
        qreal x1 = length(cp - pointList.first());
        qreal x2 = length(cp - pointList.at(1));
        if ( x1 < x2 ) {
            xx = 0.0;
            zz = length(xy - pointList.first());
            if ( sideToSegment(pointList.first(), pointList.at(1), xy) < 0 ) zz = -zz;
        } else {
            xx = lenlist.at(1);
            zz = min_dis;
        }
    } else if ( !isOpen && idx == -2 ) {
        QPointF cp = perpPointToLine(pointList.at(pointList.count() - 2), pointList.last(), xy);
        qreal x1 = length(cp - pointList.last());
        qreal x2 = length(cp - pointList.at(pointList.count() - 2));
        if ( x1 < x2 ) {
            xx = 0.0;
            zz = length(xy - pointList.first());
            if ( sideToSegment(pointList.first(), pointList.at(1), xy) < 0 ) zz = -zz;
        } else {
            xx = lenlist.at(lenlist.count() - 2);
            zz = min_dis;
        }
    } else if ( idx == -1 ) {
        QPointF cp = perpPointToLine(pointList.first(), pointList.at(1), xy);
        qreal x1 = length(cp - pointList.first());
        qreal x2 = length(cp - pointList.at(1));
        if ( x1 < x2 ) {
            xx = lenlist.first() - x1;
            zz = signedDistance(pointList.first(), pointList.last(), xy);
        } else {
            xx = lenlist.at(1);
            zz = min_dis;
        }
    } else if ( idx == -2 ) {
        QPointF cp = perpPointToLine(pointList.at(pointList.count() - 2), pointList.last(), xy);
        qreal x1 = length(cp - pointList.last());
        qreal x2 = length(cp - pointList.at(pointList.count() - 2));
        if ( x1 < x2 ) {
            xx = lenlist.last() + x1;
            zz = signedDistance(pointList.at(pointList.count() - 2), pointList.last(), xy);
        } else {
            xx = lenlist.at(lenlist.count() - 2);
            zz = min_dis;
        }
    }
    return QPointF(xx, zz);
}

/* ある点（X,Y）の通り点列に対する角度を返す */
qreal CalcVector2D::globalXYToElvAngle(const QList<QPointF> &pointList, QPointF xy, bool isOpen)
{
    if ( pointList.count() < 2 ) return 0.0;

    // 点列上にある場合、始点終点であれば通りの角度を、中間点であれば通りの角度平均を返す
    for (int i = 0; i < pointList.count(); i++) {
        if ( (xy - pointList.at(i)).manhattanLength() < 1.0e-5 ) {
            if ( !isOpen && ( i == 0 || i == pointList.count() - 1 ) ) {
                QPointF p1 = pointList.first() - pointList.at(pointList.count() - 2);
                QPointF p2 = pointList.at(1) - pointList.first();
                return qAtan2( (p1.y() + p2.y()) / 2.0, (p1.x() + p2.x()) / 2.0 );
            } else if ( i == 0 ) {
                QPointF pp = pointList.at(i + 1) - pointList.at(i);
                return qAtan2( pp.y(), pp.x() );
            } else if ( i == pointList.count() - 1 ) {
                QPointF pp = pointList.at(i) - pointList.at(i - 1);
                return qAtan2( pp.y(), pp.x() );
            } else {
                QPointF p1 = pointList.at(i) - pointList.at(i - 1);
                QPointF p2 = pointList.at(i + 1) - pointList.at(i);
                return qAtan2( (p1.y() + p2.y()) / 2.0, (p1.x() + p2.x()) / 2.0 );
            }
        }
    }

    QList<qreal> lenlist;
    qreal len = 0.0;
    lenlist.append(len);

    int idx = 1;
    qreal min_dis = 1.0e+10;

    // 線分上にあればその通りの角度を返す、なければ線分との距離を保持
    for (int i = 1; i < pointList.count(); i++) {
        if ( between(pointList.at(i), pointList.at(i - 1), xy) ) {
            QPointF pp = pointList.at(i) - pointList.at(i - 1);
            return qAtan2( pp.y(), pp.x() );
        }
        len += length(pointList.at(i) - pointList.at(i - 1));
        lenlist.append(len);

        qreal dis = signedSegmentDistance(pointList.at(i - 1), pointList.at(i), xy);
        if ( qAbs(dis) < qAbs(min_dis) ) {
            idx = i;
            min_dis = dis;
        }
    }

    // 線分との最近傍点を調べ、点上にあれば前後の通りの平均角度を、線上にあればその通りの角度を返す
    QPointF cp = perpPointToSegment(pointList.at(idx - 1), pointList.at(idx), xy);
    bool isOnI = ( (cp - pointList.at(idx - 1)).manhattanLength() < 1.0e-5 );
    bool isOnJ = ( (cp - pointList.at(idx)).manhattanLength() < 1.0e-5 );
    if ( isOnI && idx - 1 > 0 ) {
        QPointF p1 = pointList.at(idx - 1) - pointList.at(idx - 2);
        QPointF p2 = pointList.at(idx) - pointList.at(idx - 1);
        return qAtan2( (p1.y() + p2.y()) / 2.0, (p1.x() + p2.x()) / 2.0 );
    } else if ( isOnJ && idx < pointList.count() - 1 ) {
        QPointF p1 = pointList.at(idx) - pointList.at(idx - 1);
        QPointF p2 = pointList.at(idx + 1) - pointList.at(idx);
        return qAtan2( (p1.y() + p2.y()) / 2.0, (p1.x() + p2.x()) / 2.0 );
    }

    if ( !isOpen && ( isOnI || isOnJ ) ) {
        QPointF p1 = pointList.first() - pointList.at(pointList.count() - 2);
        QPointF p2 = pointList.at(1) - pointList.first();
        return qAtan2( (p1.y() + p2.y()) / 2.0, (p1.x() + p2.x()) / 2.0 );
    }

    QPointF pp = pointList.at(idx) - pointList.at(idx - 1);
    return qAtan2( pp.y(), pp.x() );
}

/* p1-p2の線分に対し、p2を中心に角度theta（rad,進行方向からの時計回りの角度）、距離xの点を返す */
QPointF CalcVector2D::rotExtendPoint(const QPointF &p1, const QPointF &p2, qreal theta, qreal xx)
{
    qreal p_ang = qAtan2((p2.y() - p1.y()), (p2.x() - p1.x()));
    qreal t_ang = p_ang - theta;
    return p2 + QPointF(xx * qCos(t_ang), xx * qSin(t_ang));
}

/* ２つの通り点列を長さ方向に結合　内分比rr */
QList<QPointF> CalcVector2D::combinePointList(const QList<QPointF> &p1_list,
                                              const QList<QPointF> &p2_list, qreal rr)
{
    QList<QPointF> plist_com;
    plist_com.append(interiorPoint(p1_list.first(), p2_list.first(), rr));

    qreal rtmp;
    QPointF ptmp;

    qreal len1 = 0.0;
    qreal len2 = 0.0;
    qreal b_len1, b_len2;

    int j = 1;
    b_len2 = len2;
    len2 += length(p2_list.at(j) - p2_list.at(j - 1));

    for (int i = 1; i < p1_list.count(); i++) {

        b_len1 = len1;
        len1 += length(p1_list.at(i) - p1_list.at(i - 1));

        while ( j < p2_list.count() ) {
            if ( len2 > len1 - 1.0e-5 ) break;

            rtmp = (len2 - b_len1) / (len1 - b_len1);
            ptmp = interiorPoint(p1_list.at(i - 1), p1_list.at(i), rtmp);
            plist_com.append(interiorPoint(ptmp, p2_list.at(j), rr));

            j++;
            if ( j >= p2_list.count() ) break;
            b_len2 = len2;
            len2 += length(p2_list.at(j) - p2_list.at(j - 1));
        }

        if (j >= p2_list.count()) {
            ptmp = extendPoint(p2_list.at(p2_list.count() - 2), p2_list.last(), (len1 - len2));
            plist_com.append(interiorPoint(p1_list.at(i), ptmp, rr));
        } else if ( len2 <= len1 + 1.0e-5 && len2 >= len1 - 1.0e-5 ) {
            plist_com.append(interiorPoint(p1_list.at(i), p2_list.at(j), rr));
            j++;
            if ( j >= p2_list.count() ) continue;
            b_len2 = len2;
            len2 += length(p2_list.at(j) - p2_list.at(j - 1));
        } else {
            rtmp = (len1 - b_len2) / (len2 - b_len2);
            ptmp = interiorPoint(p2_list.at(j - 1), p2_list.at(j), rtmp);
            plist_com.append(interiorPoint(p1_list.at(i), ptmp, rr));
        }

    }

    while ( j < p2_list.count() ) {
        ptmp = extendPoint(p1_list.at(p1_list.count() - 2), p1_list.last(), (len2 - len1));
        plist_com.append(interiorPoint(ptmp, p2_list.at(j), rr));

        j++;
        if ( j >= p2_list.count() ) break;
        b_len2 = len2;
        len2 += length(p2_list.at(j) - p2_list.at(j - 1));
    }

    return plist_com;
}

/* 点列で定義される多角形の符号付き面積を返す */
qreal CalcVector2D::signedPolygonArea( const QPolygonF &pointList )
{
    if ( pointList.count() < 3 ) return 0.0;
    qreal area = 0.0;
    for ( int i = 0; i < pointList.count(); i++ ) {
        QPointF p1 = pointList.at(i);
        QPointF p2 = ( i == pointList.count() - 1 ) ? pointList.first() : pointList.at(i + 1) ;
        area += p1.x() * p2.y() - p2.x() * p1.y();
    }
    return area / 2.0;
}

/* 点列で定義される多角形の重心位置を返す */
QPointF CalcVector2D::centerOfPolygon( const QPolygonF &pointList )
{
    if ( pointList.count() < 3 ) return QPointF();
    qreal area = 0.0, cx = 0.0, cy = 0.0;
    for ( int i = 0; i < pointList.count(); i++ ) {
        QPointF p1 = pointList.at(i);
        QPointF p2 = ( i == pointList.count() - 1 ) ? pointList.first() : pointList.at(i + 1) ;
        area += p1.x() * p2.y() - p2.x() * p1.y();
        cx += (p1.x() + p2.x()) * (p1.x() * p2.y() - p2.x() * p1.y());
        cy += (p1.y() + p2.y()) * (p1.x() * p2.y() - p2.x() * p1.y());
    }
    area = area / 2.0;
    if ( qAbs(area) < 1.0e-5 ) return QPointF();
    return QPointF(cx / 6.0 / area, cy / 6.0 / area);
}


/* 線分と線分の交差判定をし、交差するならtrueを返す 交点に端点を含まない場合はexceptをtrueにする */
bool CalcVector2D::checkSegmentIntersection(const QLineF &l1, const QLineF &l2, bool except,
                                            qreal eps)
{
    // X座標によるラフチェック
    if ( l1.x1() >= l1.x2() ) {
        if ( (l1.x1() < l2.x1() && l1.x1() < l2.x2()) ||
                (l1.x2() > l2.x1() && l1.x2() > l2.x2()) ) return false;
    } else {
        if ( (l1.x2() < l2.x1() && l1.x2() < l2.x2()) ||
                (l1.x1() > l2.x1() && l1.x1() > l2.x2()) ) return false;
    }

    // Y座標によるラフチェック
    if ( l1.y1() >= l1.y2() ) {
        if ( (l1.y1() < l2.y1() && l1.y1() < l2.y2()) ||
                (l1.y2() > l2.y1() && l1.y2() > l2.y2()) ) return false;
    } else {
        if ( (l1.y2() < l2.y1() && l1.y2() < l2.y2()) ||
                (l1.y1() > l2.y1() && l1.y1() > l2.y2()) ) return false;
    }

    // 線分上に点が存在する場合（線分同士が重なる状態を含む）
    if ( !except ) {
        // i端側のみチェック　×between(p1,p2,p4) ×between(p3,p4,p2)
        if ( between(l1.p1(), l1.p2(), l2.p1()) ) return true;
        if ( between(l2.p1(), l2.p2(), l1.p1()) ) return true;
    }

    // 交差判定し、真であれば交点を追加する
    return ( (cross(l1.p2() - l1.p1(), l2.p1() - l1.p1()) * cross(l1.p2() - l1.p1(),
                                                                  l2.p2() - l1.p1()) < -eps) &&
             (cross(l2.p2() - l2.p1(), l1.p1() - l2.p1()) * cross(l2.p2() - l2.p1(),
                                                                  l1.p2() - l2.p1()) < -eps) ) ;
}

QList<QPointF> CalcVector2D::crossPointList(const QList<QPointF> &a_list, const QList<QPointF> &b_list){
    QList<QPointF> crosslist;

    for ( int i = 0; i < a_list.count() - 1 ; i++) {

        QLineF aline( a_list.at(i), a_list.at(i + 1) );

        for ( int j = 0; j < b_list.count() - 1 ; j++) {

            QLineF bline( b_list.at(j), b_list.at(j + 1) );
            QList<QPointF> plist = CalcVector2D::intersectCheck(aline, bline);
            crosslist.append(plist);

            if (i == a_list.count() - 2) {
                if ( CalcVector2D::between(b_list.at(j), b_list.at(j + 1), a_list.last()) )
                    crosslist.append(a_list.last());
            }
        }

        if ( CalcVector2D::between( a_list.at(i), a_list.at(i + 1), b_list.last()) )
            crosslist.append(b_list.last());
    }

    return crosslist;
}
} // namespace post3dapp
