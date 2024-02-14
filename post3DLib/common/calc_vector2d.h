#ifndef CALCVECTOR2D_H
#define CALCVECTOR2D_H

#include <QLineF>
#include <QList>
#include <QPolygonF>
#include <QRectF>
#include "qmath.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class POST3D_CLASS_DLLSPEC CalcVector2D
{


public:

    CalcVector2D() { }
    ~CalcVector2D() { }

    /* 線分と線分の交差判定をし、真であればplistに交点が追加される */
    static QList<QPointF> intersectCheck(const QLineF &line1, const QLineF &line2, qreal eps = 1.0e-5);

    /* 直線と線分の交差判定をし、真であればplistに交点が追加される */
    static QList<QPointF> intersectSegmentToLine(const QLineF &line1, const QLineF &line2,
                                                 qreal eps = 1.0e-5);

    /* 線分p1-p2に点p3が含まれるか否か */
    static bool between(const QPointF &p1, const QPointF &p2, const QPointF &p3, qreal eps = 1.0e-5);

    /* 線分p1-p2と点p3の距離 */
    static qreal segmentDistance(const QPointF &p1, const QPointF &p2, const QPointF &p3,
                                 qreal eps = 1.0e-5);

    /* 線分p1-p2と点p3の距離（正負付き） 進行方向右側が正、左側が負 */
    static qreal signedSegmentDistance(const QPointF &p1, const QPointF &p2, const QPointF &p3,
                                       qreal eps = 1.0e-5);

    /* 有向線分p1-p2と点p3の関係 進行方向右側であれば+1、左側であれば-1、線上であれば0 */
    static int sideToSegment(const QPointF &p1, const QPointF &p2, const QPointF &p3,
                             qreal eps = 1.0e-5);

    /* 線分p1-p2と点p3の最近傍点　*/
    static QPointF perpPointToSegment(const QPointF &p1, const QPointF &p2, const QPointF &p3,
                                      qreal eps = 1.0e-5);

    /* 直線p1-p2と点p3の最近傍点　*/
    static QPointF perpPointToLine(const QPointF &p1, const QPointF &p2, const QPointF &p3,
                                   qreal eps = 1.0e-5);

    /* 線分p1-p2と線分p3-p4が交差する場合、交差点を算出 */
    static QPointF intersectPoint(const QPointF &p1, const QPointF &p2,
                                  const QPointF &p3, const QPointF &p4);

    /* 点p1->p2を長さdだけ延長した点を返す */
    static QPointF extendPoint(const QPointF &p1, const QPointF &p2, qreal d);

    /* 点p1,p2を内分比rr=m/(m+n)で内分した点を返す */
    static QPointF interiorPoint(const QPointF &p1, const QPointF &p2, qreal rr);

    /* 点ptが多角形polyの内部にあるか否か */
    static bool isInPolygon( const QPolygonF &poly, const QPointF &pt, qreal eps = 1.0e-5 );

    /* 線分を長方形でクリッピングし、線分を返す */
    static QLineF clipSegment(const QLineF &line1, const QRectF &rect);

    /* 直線y=mx+n を長方形でクリッピングし、線分を返す */
    static QLineF clipLine(qreal c_m, qreal c_n, const QRectF &rect);

    /* ベクトルp1とベクトルp2の外積 */
    static qreal cross(const QPointF &p1, const QPointF &p2);

    /* ベクトルp1とベクトルp2の内積 */
    static qreal dot(const QPointF &p1, const QPointF &p2);

    /* p1-p2を結ぶ直線と点p3の距離 */
    static qreal distance(const QPointF &p1, const QPointF &p2, const QPointF &p3);

    /* p1-p2を結ぶ直線と点p3の距離（正負付き） 進行方向右側が正、左側が負 */
    static qreal signedDistance(const QPointF &p1, const QPointF &p2, const QPointF &p3);

    /* ベクトルp1の長さ */
    static qreal length(const QPointF &p1);

    /* 点列の長さ */
    static qreal totalLength(const QList<QPointF> &pointList);

    /* 指定の点列における長さ方向の値xxに対応する点を返す */
    static QPointF lengthToPoint(const QList<QPointF> &pointList, qreal xx );

    /* XY座標系の点を長さ座標系（X軸）の値に変換 */
//    static qreal pointToElvX(const QList<QPointF> &pointList, QPointF xy );

    /* 軸組座標系（X,Z）の値をXY座標系の点に変換 */
    static QPointF elvXZToGlobalXY(const QList<QPointF> &pointList, QPointF ep, bool isOpen = true );

    /* XY座標系の点を軸組座標系（X,Z）の値に変換 */
    static QPointF globalXYToElvXZ(const QList<QPointF> &pointList, QPointF xy, bool isOpen = true );

    /* p1-p2の線分に対し、p2を中心に角度theta（rad,進行方向からの時計回りの角度）、距離xの点を返す */
    static QPointF rotExtendPoint(const QPointF &p1, const QPointF &p2, qreal theta, qreal xx);

    /* ある点（X,Y）の通り点列に対する角度(rad)を返す */
    static qreal globalXYToElvAngle(const QList<QPointF> &pointList, QPointF xy, bool isOpen = true );


    /* ２つの通り点列を長さ方向に結合　内分比rr */
    static QList<QPointF> combinePointList(const QList<QPointF> &, const QList<QPointF> &, qreal );

    /* 点列で定義される多角形の符号付き面積を返す */
    static qreal signedPolygonArea(const QPolygonF &pointList );

    /* 点列で定義される多角形の重心位置を返す */
    static QPointF centerOfPolygon(const QPolygonF &pointList );

    /* 線分と線分の交差判定をし、交差するならtrueを返す 交点に端点を含まない場合はexceptをtrueにする */
    static bool checkSegmentIntersection(const QLineF &line1, const QLineF &line2, bool except = true,
                                         qreal eps = 1.0e-5);

    /* FRAMEPOINTVALUEのpointListを比較して交点を返す*/
    static QList<QPointF> crossPointList(const QList<QPointF> &a_list,
                                         const QList<QPointF> &b_list);
private:

};
} // namespace post3dapp

#endif


