#ifndef OUTPUTLINEITEMSTRESS_H
#define OUTPUTLINEITEMSTRESS_H

#include "output_lineitem.h"

namespace post3dapp{
class MemberData;
struct MRESULT;
struct STRESS;

struct VALUEPLOTMAXLENGTH
{
    qreal maxLength = 999999.0;
    qreal iLeftMaxLength = 999999.0;
    qreal jLeftMaxLength = 999999.0;
    qreal cLeftMaxLength = 999999.0;
    qreal iRightMaxLength = 999999.0;
    qreal jRightMaxLength = 999999.0;
    qreal cRightMaxLength = 999999.0;
};

class OutputLineItemStress : public OutputLineItem
{
public:
    OutputLineItemStress(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid,
                         const QString &, const QList<QPointF> &plist, qreal tlen,
                         bool zin, bool isCW, MRESULT *mr, VEC3D mypvec, bool isaroundplane);

    void setDrawingStatusDirect() override;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *) override{};
    qreal totalLength;
    MRESULT *myResult;
    bool isZin; // 部材Z軸が面内であるか否か
    bool isClockWize; //ijに対して右が正か左が正か
    bool isAroundPlane;

    QList<MemberData *> iRelatedMember;
    QList<MemberData *> jRelatedMember;



    const int div = 20;

    void drawCurve(QList<qreal> vallist, qreal start, qreal end);
    VALUEPLOTMAXLENGTH calcMaxLength();
    void plotValues(QVector<qreal> values);

    void nodeDiagram(const QUuid &uid, const QList<STRESS> &stress, bool isNormalDirection = false );
    void faceDiagram(const QUuid &uid, const QList<STRESS> &stress, bool isNormalDirection = false );
    void rigidDiagram(const QUuid &uid, const QList<STRESS> &stress, bool isNormalDirection = false );
    void cmqDiagram(const QUuid &uid, const QList<STRESS> &stress, bool isNormalDirection = false );
    void rankDiagram(const QUuid &uid, bool isNormalDirection = false );

    QList<STRESS> getQuDsStressList(const QString& fileName)const;
private:
    void appendRank();
};
} // namespace post3dapp

#endif // OUTPUTLINEITEMSTRESS_H
