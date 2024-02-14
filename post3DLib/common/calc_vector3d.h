#ifndef CALCVECTOR3D_H
#define CALCVECTOR3D_H

#include <QList>
#include "qmath.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
/* ３次元ベクトル */
struct POST3D_CLASS_DLLSPEC VEC3D {
    explicit VEC3D( qreal _x = 0.0, qreal _y = 0.0, qreal _z = 0.0 )
        : x(_x), y(_y), z(_z) { }

    qreal x;
    qreal y;
    qreal z;
};


class POST3D_CLASS_DLLSPEC CalcVector3D
{

public:

    CalcVector3D() { }
    ~CalcVector3D() { }

    /* ベクトルの長さ（ノルム）を計算する　*/
    static qreal norm( const VEC3D & );

    /* 正規化されたベクトルを返す */
    static VEC3D normalize( const VEC3D & );

    /* v1-v2 */
    static VEC3D minus( const VEC3D &v1, const VEC3D &v2 );

    /* v1+v2 */
    static VEC3D plus( const VEC3D &v1, const VEC3D &v2 );

    /* 線分v1-v2に点v3が含まれるか否か */
    static bool between(const VEC3D &v1, const VEC3D &v2, const VEC3D &v3, qreal eps = 1.0e-5);

    /* 線分v1-v2と点v3の最近傍点　*/
    static VEC3D perpPointToSegment(const VEC3D &v1, const VEC3D &v2, const VEC3D &v3,
                                    qreal eps = 1.0e-5);

    /* ベクトルv1とベクトルv2の外積 */
    static VEC3D cross(const VEC3D &v1, const VEC3D &v2);

    /* ベクトルv1とベクトルv2の外積（単位ベクトル） */
    static VEC3D crossNormalized(const VEC3D &v1, const VEC3D &v2);

    /* ベクトルv1とベクトルv2の内積 */
    static qreal dot(const VEC3D &v1, const VEC3D &v2);

    /* 多点列の法線ベクトル（代表平面） 得られた法線ベクトルの1/2が面積 */
    static VEC3D polygonNorm( const QList<VEC3D> &poly );

    /* 全体座標系から局所座標系への変換
       u_v,v_v,w_v:Local座標系のu-v-w単位ベクトル
       op:Local座標系の原点,gxyz:Global座標系の対象点
    */
    static VEC3D globalToLocal( const VEC3D &u_v, const VEC3D &v_v, const VEC3D &w_v,
                                const VEC3D &op,  const VEC3D &gxyz );

    /* 局所座標系から全体座標系への変換
       u_v,v_v,w_v:Local座標系のu-v-w単位ベクトル
       op:Local座標系の原点,lxyz:Local座標系の対象点
    */
    static VEC3D localToGlobal( const VEC3D &u_v, const VEC3D &v_v, const VEC3D &w_v,
                                const VEC3D &op,  const VEC3D &lxyz );

    /* 点から面への距離（法線ベクトルと同方向を正）を返す
       p_v:面の法線ベクトル（単位ベクトル）,p_p:面を通る点,o_p:投影したい点->面への投影点
    */
    static qreal pointToPlane( const VEC3D &p_v, const VEC3D &p_p, VEC3D &o_p );


    /* 任意軸回りの回転変換（右ねじ） vlist:元点列->変換後座標,theta:角度(rad),vr:回転軸ベクトル(単位ベクトル)  */
    static void rotateToVR( QList<VEC3D> &vlist, qreal theta, const VEC3D &vr );

private:

};

} // namespace post3dapp
#endif


