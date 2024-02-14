#include "global_point.h"

#include <QDebug>
#include "qmath.h"

#include "factory_unifieddata.h"
#include "unified_data.h"
namespace post3dapp{
qreal GlobalPoint::length() const
{
    return qSqrt( qPow(x(), 2) + qPow(y(), 2) + qPow(z(), 2) );
}

qreal GlobalPoint::xyLength() const
{
    return qSqrt( qPow(x(), 2) + qPow(y(), 2) );
}

qreal GlobalPoint::xyManhattanLength() const
{
    return qAbs(x()) + qAbs(y());
}

QList<QUuid> GlobalPoint::getAttachedFloorList() const
{
    return attachedFloor;
}

QList<QUuid> GlobalPoint::getAttachedFrameList() const
{
    return attachedFrame;
}

void GlobalPoint::appendAttachedFloor(const QUuid &id)
{
    if (!attachedFloor.contains(id)) attachedFloor.append(id);
}

void GlobalPoint::appendAttachedFloor(const QList<QUuid> &idlist)
{
    Q_FOREACH (QUuid id, idlist) {
        if (!attachedFloor.contains(id)) attachedFloor.append(id);
    }
}

void GlobalPoint::removeAttachedFloor(const QUuid &id)
{
    if (attachedFloor.contains(id)) attachedFloor.removeOne(id);
}

void GlobalPoint::removeAttachedFloor(const QList<QUuid> &idlist)
{
    Q_FOREACH (QUuid id, idlist) {
        if (attachedFloor.contains(id)) attachedFloor.removeOne(id);
    }
}

void GlobalPoint::appendAttachedFrame(const QUuid &id)
{
    if (!attachedFrame.contains(id)) attachedFrame.append(id);
}

void GlobalPoint::appendAttachedFrame(const QList<QUuid> &idlist)
{
    Q_FOREACH (QUuid id, idlist) {
        if (!attachedFrame.contains(id)) attachedFrame.append(id);
    }
}

void GlobalPoint::removeAttachedFrame(const QUuid &id)
{
    if (attachedFrame.contains(id)) attachedFrame.removeOne(id);
}

void GlobalPoint::removeAttachedFrame(const QList<QUuid> &idlist)
{
    Q_FOREACH (QUuid id, idlist) {
        if (attachedFrame.contains(id)) attachedFrame.removeOne(id);
    }
}

void GlobalPoint::clearAttachedFloor()
{
    attachedFloor.clear();
}

void GlobalPoint::clearAttachedFrame()
{
    attachedFrame.clear();
}

QPointF GlobalPoint::getXYQPointF() const
{
    return QPointF(x(), y());
}

QPointF GlobalPoint::getElvQPointF() const
{
    return QPointF(elvX(), elvY());
}

bool GlobalPoint::containsFloor(const QUuid &id) const
{
    return attachedFloor.contains(id);
}

bool GlobalPoint::containsFloor(const QList<QUuid> &idlist) const
{
    bool ok = false;
    Q_FOREACH (QUuid id, idlist) {
        if (attachedFloor.contains(id)) {
            ok = true;
            break;
        }
    }
    return ok;
}

bool GlobalPoint::containsFrame(const QUuid &id)const
{
    return attachedFrame.contains(id);
}

bool GlobalPoint::containsFrame(const QList<QUuid> &idlist)const
{
    bool ok = false;
    Q_FOREACH (QUuid id, idlist) {
        if (attachedFrame.contains(id)) {
            ok = true;
            break;
        }
    }
    return ok;
}

bool GlobalPoint::isNearlyPoint(const QPointF &pp, qreal eps) const
{
    return ( qPow((xp - pp.x()), 2.0) + qPow((yp - pp.y()), 2.0) < eps * eps );
}

bool GlobalPoint::isNearlyPoint(const GlobalPoint &pp, qreal eps) const
{
    return ( qPow((xp - pp.x()), 2.0) + qPow((yp - pp.y()), 2.0) + qPow((zp - pp.z()),
                                                                        2.0) < eps * eps );
}

void GlobalPoint::clearNotFoundFloor(UnifiedDataType type)
{
    for (int i = attachedFloor.count() - 1; i >= 0 ; i--) {
            if (!FactoryUnifiedData::getInstance(type)->isExistedID(attachedFloor.at(i), DATATYPE::TPFLOOR))
                attachedFloor.removeAt(i);
    }
}

void GlobalPoint::clearNotFoundFrame(UnifiedDataType type)
{
    for (int i = attachedFrame.count() - 1; i >= 0 ; i--) {
            if (!FactoryUnifiedData::getInstance(type)->isExistedID(attachedFrame.at(i), DATATYPE::TPFRAMEP))
                attachedFrame.removeAt(i);
    }
}
} // namespace post3dapp
