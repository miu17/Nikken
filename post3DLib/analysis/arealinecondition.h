#ifndef AREALINECONDITION_H
#define AREALINECONDITION_H

#include "linecondition.h"

namespace post3dapp{
class JointData;
struct FRAMEPOINTSVALUE;
struct FLOORVALUES;

class AbstratAreaLineCondition : public LineCondition
{
public:
    AbstratAreaLineCondition(
            const QString&,
            ANALYSISSTEP _myStep = ANALYSISSTEP::STEP_NONE,
            const QList<LINESTATUSTYPE>& = QList<LINESTATUSTYPE>{},
            const QList<MEMBERCATEGORY>& = QList<MEMBERCATEGORY>{});
    QString convert(const QString&, QSet<QString>* , QSet<QString>*, QString*) const override;
protected:
    FRAMEPOINTSVALUE getFrame(const QString&, QString*) const;
    QList<FRAMEPOINTSVALUE> getFrames(const QString&, const QString&, QString*) const;
    FLOORVALUES getFloor(const QString&, QString*) const;
    QList<FLOORVALUES> getFloors(const QString&, const QString&, QString*) const;
    virtual QList<QSet<JointData*>> getJointList(const QString&, QString*) const = 0;
    QSet<QString> getJoints(const QString&, QString*) const;
    QSet<QString> getMembers(const QString&, QString*) const;
    QList<MEMBERCATEGORY> checkMemberCategory(const QString&, QString*) const;
    QString sign;
    QList<MEMBERCATEGORY> memberCategory;
};

class BulkAreaLineCondition : public AbstratAreaLineCondition
{
public:
    BulkAreaLineCondition(
            const QString&,
            ANALYSISSTEP,
            const QList<LINESTATUSTYPE>&,
            const QList<MEMBERCATEGORY>&);
protected:
    QList<QSet<JointData*>> getJointList(const QString&, QString*) const override;

};

class GridAreaLineCondition : public AbstratAreaLineCondition
{
public:
    GridAreaLineCondition(
            const QString&,
            ANALYSISSTEP,
            const QList<LINESTATUSTYPE>&,
            const QList<MEMBERCATEGORY>&);
protected:
    QList<QSet<JointData*>> getJointList(const QString&, QString*) const override;

};

class FloorAreaLineCondition : public AbstratAreaLineCondition
{
public:
    FloorAreaLineCondition(
            const QString& sign,
            ANALYSISSTEP step,
            const QList<LINESTATUSTYPE>& type,
            const QList<MEMBERCATEGORY>& category);
protected:
    QList<QSet<JointData*>> getJointList(const QString&, QString*) const override;
};

class NodeAreaCondition : public AbstratAreaLineCondition
{
public:
    NodeAreaCondition(
            const QString& sign,
            ANALYSISSTEP step,
            const QList<LINESTATUSTYPE>& type,
            const QList<MEMBERCATEGORY>& category);
protected:
    QList<QSet<JointData*>> getJointList(const QString&, QString*) const override;
};


class CloseAreaCondition : public AbstratAreaLineCondition
{
public:
    CloseAreaCondition(
            const QString& sign,
            ANALYSISSTEP step,
            const QList<LINESTATUSTYPE>& type,
            const QList<MEMBERCATEGORY>& category);
protected:
    QList<QSet<JointData*>> getJointList(const QString&, QString*) const override;
};

class CoordinateAreaCondition : public AbstratAreaLineCondition
{
public:
    CoordinateAreaCondition(
            const QString& sign,
            ANALYSISSTEP step,
            const QList<LINESTATUSTYPE>& type,
            const QList<MEMBERCATEGORY>& category);
protected:
    QList<QSet<JointData*>> getJointList(const QString&, QString*) const override;
};

} // namespace post3dapp


#endif // AREALINECONDITION_H
