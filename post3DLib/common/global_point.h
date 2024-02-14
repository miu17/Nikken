#ifndef GLOBALPOINT_H
#define GLOBALPOINT_H

#include <QtGlobal>
#include <QList>
#include <QPointF>
#include <QUuid>
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
enum class UnifiedDataType;
class POST3D_CLASS_DLLSPEC GlobalPoint
{

public:

    GlobalPoint();
    GlobalPoint( qreal xpos, qreal ypos, qreal zpos = 0.0 );
    GlobalPoint( QPointF pp );
    GlobalPoint( const GlobalPoint &gp );

    qreal x() const;
    qreal y() const;
    qreal z() const;
    qreal elvX() const;
    qreal elvY() const;
    qreal elvZ() const;
    void setX(qreal x);
    void setY(qreal y);
    void setZ(qreal z);
    void setElvX(qreal x);
    void setElvY(qreal y);
    void setElvZ(qreal z);

    qreal length() const;
    qreal xyLength() const;
    qreal xyManhattanLength() const;

    GlobalPoint &operator+=(const GlobalPoint &p);
    GlobalPoint &operator-=(const GlobalPoint &p);
    GlobalPoint &operator*=(qreal c);
    GlobalPoint &operator/=(qreal c);

    friend inline bool operator==(const GlobalPoint &p1, const GlobalPoint &p2);
    friend inline bool operator!=(const GlobalPoint &p1, const GlobalPoint &p2);
    friend inline const GlobalPoint operator+(const GlobalPoint &p1, const GlobalPoint &p2);
    friend inline const GlobalPoint operator-(const GlobalPoint &p1, const GlobalPoint &p2);
    friend inline const GlobalPoint operator*(const GlobalPoint &p, qreal c);
    friend inline const GlobalPoint operator*(qreal c, const GlobalPoint &p);
    friend inline const GlobalPoint operator-(const GlobalPoint &p);
    friend inline const GlobalPoint operator/(const GlobalPoint &p, qreal c);


    QList<QUuid> getAttachedFloorList() const;
    QList<QUuid> getAttachedFrameList() const;

    void appendAttachedFloor(const QUuid &id);
    void appendAttachedFloor(const QList<QUuid> &idlist);
    void appendAttachedFrame(const QUuid &id);
    void appendAttachedFrame(const QList<QUuid> &idlist);
    void removeAttachedFloor(const QUuid &id);
    void removeAttachedFloor(const QList<QUuid> &idlist);
    void removeAttachedFrame(const QUuid &id);
    void removeAttachedFrame(const QList<QUuid> &idlist);
    void clearAttachedFloor();
    void clearAttachedFrame();

    QPointF getXYQPointF() const;
    QPointF getElvQPointF() const;

    bool containsFloor(const QUuid &id) const;
    bool containsFloor(const QList<QUuid> &idlist) const;
    bool containsFrame(const QUuid &id) const;
    bool containsFrame(const QList<QUuid> &idlist) const;

    bool isNearlyPoint(const QPointF &pp, qreal eps = 1.0e-3) const;
    bool isNearlyPoint(const GlobalPoint &pp, qreal eps = 1.0e-3) const;

    void clearNotFoundFloor(UnifiedDataType);
    void clearNotFoundFrame(UnifiedDataType);

private:

    qreal xp, yp, zp;
    qreal elv_x, elv_y, elv_z;

    QList<QUuid> attachedFloor;
    QList<QUuid> attachedFrame;

};


inline GlobalPoint::GlobalPoint()
{
    xp = 0.0;
    yp = 0.0;
    zp = 0.0;
    elv_x = 0.0;
    elv_y = 0.0;
    elv_z = 0.0;
}

inline GlobalPoint::GlobalPoint( qreal xpos, qreal ypos, qreal zpos )
{
    xp = xpos;
    yp = ypos;
    zp = zpos;
    elv_x = 0.0;
    elv_y = 0.0;
    elv_z = 0.0;
}

inline GlobalPoint::GlobalPoint( QPointF pp )
{
    xp = pp.x();
    yp = pp.y();
    zp = 0.0;
    elv_x = 0.0;
    elv_y = 0.0;
    elv_z = 0.0;
}

inline GlobalPoint::GlobalPoint( const GlobalPoint &gp )
{
    xp = gp.x();
    yp = gp.y();
    zp = gp.z();
    attachedFloor = gp.getAttachedFloorList();
    attachedFrame = gp.getAttachedFrameList();
    elv_x = gp.elvX();
    elv_y = gp.elvY();
    elv_z = gp.elvZ();
}

inline qreal GlobalPoint::x() const
{
    return xp;
}

inline qreal GlobalPoint::y() const
{
    return yp;
}

inline qreal GlobalPoint::z() const
{
    return zp;
}

inline qreal GlobalPoint::elvX() const
{
    return elv_x;
}

inline qreal GlobalPoint::elvY() const
{
    return elv_y;
}

inline qreal GlobalPoint::elvZ() const
{
    return elv_z;
}

inline void GlobalPoint::setX(qreal xpos)
{
    xp = xpos;
}

inline void GlobalPoint::setY(qreal ypos)
{
    yp = ypos;
}

inline void GlobalPoint::setZ(qreal zpos)
{
    zp = zpos;
}

inline void GlobalPoint::setElvX(qreal x)
{
    elv_x = x;
}

inline void GlobalPoint::setElvY(qreal y)
{
    elv_y = y;
}

inline void GlobalPoint::setElvZ(qreal z)
{
    elv_z = z;
}

inline GlobalPoint &GlobalPoint::operator+=(const GlobalPoint &p)
{
    xp += p.xp;
    yp += p.yp;
    zp += p.zp;
    return *this;
}

inline GlobalPoint &GlobalPoint::operator-=(const GlobalPoint &p)
{
    xp -= p.xp;
    yp -= p.yp;
    zp -= p.zp;
    return *this;
}

inline GlobalPoint &GlobalPoint::operator*=(qreal c)
{
    xp = xp * c;
    yp = yp * c;
    zp = zp * c;
    return *this;
}

inline GlobalPoint &GlobalPoint::operator/=(qreal c)
{
    xp = xp / c;
    yp = yp / c;
    zp = zp / c;
    return *this;
}

inline bool operator==(const GlobalPoint &p1, const GlobalPoint &p2)
{
    return p1.xp == p2.xp && p1.yp == p2.yp && p1.zp == p2.zp ;
}

inline bool operator!=(const GlobalPoint &p1, const GlobalPoint &p2)
{
    return p1.xp != p2.xp || p1.yp != p2.yp || p1.zp != p2.zp ;
}

inline const GlobalPoint operator+(const GlobalPoint &p1, const GlobalPoint &p2)
{
    return GlobalPoint(p1.xp + p2.xp, p1.yp + p2.yp, p1.zp + p2.zp);
}

inline const GlobalPoint operator-(const GlobalPoint &p1, const GlobalPoint &p2)
{
    return GlobalPoint(p1.xp - p2.xp, p1.yp - p2.yp, p1.zp - p2.zp);
}

inline const GlobalPoint operator*(const GlobalPoint &p, qreal c)
{
    return GlobalPoint(p.xp * c, p.yp * c, p.zp * c);
}

inline const GlobalPoint operator*(qreal c, const GlobalPoint &p)
{
    return GlobalPoint(p.xp * c, p.yp * c, p.zp * c);
}

inline const GlobalPoint operator-(const GlobalPoint &p)
{
    return GlobalPoint(-p.xp, -p.yp, -p.zp);
}

inline const GlobalPoint operator/(const GlobalPoint &p, qreal c)
{
    return GlobalPoint(p.xp / c, p.yp / c, p.zp / c);
}

} // namespace post3dapp
#endif

