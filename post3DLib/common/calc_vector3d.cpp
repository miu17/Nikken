#include "calc_vector3d.h"

#include <QDebug>
#include "qmath.h"
namespace post3dapp{
/* ベクトルの長さ（ノルム）を計算する　vec：ベクトル,n:次元数 */
qreal CalcVector3D::norm( const VEC3D &vec )
{
    qreal s = 0.0;
    s += vec.x * vec.x;
    s += vec.y * vec.y;
    s += vec.z * vec.z;
    return qSqrt(s);
}

/* 正規化されたベクトルを返す*/
VEC3D CalcVector3D::normalize( const VEC3D &vec )
{
    qreal nrm = norm(vec);
    return ( qAbs(nrm) < 1.0e-5 ) ? VEC3D() : VEC3D( vec.x / nrm, vec.y / nrm, vec.z / nrm );
}

/* v1-v2 */
VEC3D CalcVector3D::minus( const VEC3D &v1, const VEC3D &v2 )
{
    return VEC3D( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z );
}

/* v1+v2 */
VEC3D CalcVector3D::plus( const VEC3D &v1, const VEC3D &v2 )
{
    return VEC3D( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z );
}

/* ベクトルv1とベクトルv2の外積ベクトル */
VEC3D CalcVector3D::cross(const VEC3D &v1, const VEC3D &v2)
{
    return VEC3D( v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x );
}

/* ベクトルv1とベクトルv2の外積ベクトル（単位ベクトル） */
VEC3D CalcVector3D::crossNormalized(const VEC3D &v1, const VEC3D &v2)
{
    qreal xx = v1.y * v2.z - v1.z * v2.y;
    qreal yy = v1.z * v2.x - v1.x * v2.z;
    qreal zz = v1.x * v2.y - v1.y * v2.x;
    qreal ss = qSqrt( xx * xx + yy * yy + zz * zz );
    return ( qAbs(ss) < 1.0e-5 ) ? VEC3D() : VEC3D( xx / ss, yy / ss, zz / ss );
}

/* ベクトルv1とベクトルv2の内積 */
qreal CalcVector3D::dot(const VEC3D &v1, const VEC3D &v2)
{
    return ( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z );
}

/* 線分v1-v2に点v3が含まれるか否か */
bool CalcVector3D::between(const VEC3D &v1, const VEC3D &v2, const VEC3D &v3, qreal eps)
{
    VEC3D v12(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
    VEC3D v13(v1.x - v3.x, v1.y - v3.y, v1.z - v3.z);
    VEC3D v23(v2.x - v3.x, v2.y - v3.y, v2.z - v3.z);
    return ( qAbs( norm(v12) - norm(v13) - norm(v23) ) < eps ) ;
}

/* 線分v1-v2と点v3の最近傍点　*/
VEC3D CalcVector3D::perpPointToSegment(const VEC3D &v1, const VEC3D &v2, const VEC3D &v3, qreal eps)
{
    qreal up = (v3.x - v1.x) * (v2.x - v1.x) + (v3.y - v1.y) * (v2.y - v1.y) + (v3.z - v1.z) *
               (v2.z - v1.z);
    qreal dn = (v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) *
               (v2.z - v1.z);
    qreal tt = up / dn;

    if ( tt > -eps && tt < 1.0 + eps ) {
        return VEC3D(tt * v2.x + (1.0 - tt) * v1.x, tt * v2.y + (1.0 - tt) * v1.y,
                     tt * v2.z + (1.0 - tt) * v1.z);
    }

    qreal l1 = norm( VEC3D(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z) );
    qreal l2 = norm( VEC3D(v3.x - v2.x, v3.y - v2.y, v3.z - v2.z) );
    return ( l1 < l2 ) ? v1 : v2 ;
}

/* 多点列の法線ベクトル（代表平面） 得られた法線ベクトルの長さの1/2が面積 */
VEC3D CalcVector3D::polygonNorm( const QList<VEC3D> &poly )
{
    qreal x = 0.0, y = 0.0, z = 0.0;
    for ( int i = 0; i < poly.count(); i++ ) {
        int i1 = ( i == poly.count() - 1 ) ? 0 : i + 1 ;
        x += ( poly.at(i).y - poly.at(i1).y ) * ( poly.at(i).z + poly.at(i1).z );
        y += ( poly.at(i).z - poly.at(i1).z ) * ( poly.at(i).x + poly.at(i1).x );
        z += ( poly.at(i).x - poly.at(i1).x ) * ( poly.at(i).y + poly.at(i1).y );
    }
    return VEC3D(x, y, z);
}

/* 全体座標系から局所座標系への変換
   u_v,v_v,w_v:Local座標系のu-v-w単位ベクトル
   op:Local座標系の中心点,gxyz:Global座標系の対象点
*/
VEC3D CalcVector3D::globalToLocal( const VEC3D &u_v, const VEC3D &v_v, const VEC3D &w_v,
                                   const VEC3D &op,  const VEC3D &gxyz )
{
    VEC3D gp( gxyz.x - op.x, gxyz.y - op.y, gxyz.z - op.z );
    return VEC3D( dot(u_v, gp), dot(v_v, gp), dot(w_v, gp) );
}

/* 局所座標系から全体座標系への変換
   u_v,v_v,w_v:Local座標系のu-v-w単位ベクトル
   op:Local座標系の原点,lxyz:Local座標系の対象点
*/
VEC3D CalcVector3D::localToGlobal( const VEC3D &u_v, const VEC3D &v_v, const VEC3D &w_v,
                                   const VEC3D &op,  const VEC3D &lxyz )
{
    return VEC3D( lxyz.x * u_v.x + lxyz.y * v_v.x + lxyz.z * w_v.x + op.x,
                  lxyz.x * u_v.y + lxyz.y * v_v.y + lxyz.z * w_v.y + op.y,
                  lxyz.x * u_v.z + lxyz.y * v_v.z + lxyz.z * w_v.z + op.z );
}

/* 点から面への距離（法線ベクトルと同方向を正）を返す
   p_v:面の法線ベクトル（単位ベクトル）,p_p:面を通る点,
   o_p:対象点,xyz:面への投影点
*/
qreal CalcVector3D::pointToPlane( const VEC3D &p_v, const VEC3D &p_p, VEC3D &o_p )
{
    VEC3D q_p( o_p.x - p_p.x, o_p.y - p_p.y, o_p.z - p_p.z );
    qreal dist = dot( p_v, q_p );
    o_p = VEC3D( o_p.x - dist * p_v.x, o_p.y - dist * p_v.y, o_p.z - dist * p_v.z );
    return dist;
}

/* 任意軸回りの回転変換（右ねじ） vlist:元点列->変換後座標,theta:角度(rad),vr:回転軸ベクトル(単位ベクトル)  */
void CalcVector3D::rotateToVR( QList<VEC3D> &vlist, qreal theta, const VEC3D &vr )
{
    qreal ss = qSin(theta);
    qreal cc = qCos(theta);
    qreal c1 = 1.0 - cc;

    qreal m11 = vr.x * vr.x * c1 + cc;
    qreal m12 = vr.x * vr.y * c1 - vr.z * ss;
    qreal m13 = vr.x * vr.z * c1 + vr.y * ss;
    qreal m21 = vr.y * vr.x * c1 + vr.z * ss;
    qreal m22 = vr.y * vr.y * c1 + cc;
    qreal m23 = vr.y * vr.z * c1 - vr.x * ss;
    qreal m31 = vr.x * vr.z * c1 - vr.y * ss;
    qreal m32 = vr.y * vr.z * c1 + vr.x * ss;
    qreal m33 = vr.z * vr.z * c1 + cc;

    for ( int i = 0; i < vlist.count(); i++ ) {
        VEC3D vv = vlist.at(i);
        qreal xx = m11 * vv.x + m12 * vv.y + m13 * vv.z;
        qreal yy = m21 * vv.x + m22 * vv.y + m23 * vv.z;
        qreal zz = m31 * vv.x + m32 * vv.y + m33 * vv.z;
        vlist.replace(i, VEC3D(xx, yy, zz));
    }
}

} // namespace post3dapp

