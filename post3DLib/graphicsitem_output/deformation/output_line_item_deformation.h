#ifndef OUTPUTLINEITEMDeformation_H
#define OUTPUTLINEITEMDeformation_H

#include "output_lineitem.h"

namespace post3dapp{

class OutputLineItemDeformation : public OutputLineItem
{
public:
    OutputLineItemDeformation(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, bool zin, bool isCW, VEC3D mypvec, VEC3D mypxvec, bool shape_on = true);
    void setDrawingStatusDirect() override;
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override{Q_UNUSED(event)};
    void createDeformationShapePathWithShear();
//    virtual void drawOTShapePath(QPainter*)override;
//    virtual QColor itemColor(ATTENTION_STATUS)override;

    bool isZin;
    bool isClockWize;
private:
    void figplt(qreal XA0, qreal YA0, qreal XB0, qreal YB0, qreal UAI, qreal UBI, qreal VAI, qreal VBI, qreal TAI, qreal TBI, qreal RAL1, qreal RAL2, qreal FAL1, qreal FAL2, int PINA, int PINB, qreal EI, qreal GA, qreal D, QVector<qreal> xpos, QVector<qreal> qq, QVector<qreal> mm, int idv, qreal UBAI);
    void calcBendingCurvePP(QVector<qreal> &XA_bend, QVector<qreal> &YA_bend, qreal XAR, qreal YAR, qreal XBR, qreal YBR, qreal RL, qreal RR, qreal FL, qreal FR, qreal TAI, qreal TBI, qreal TT0, qreal XCC_origin, qreal XSS_origin, int idv, qreal UBAI);
    void calcBendingCurvePF(QVector<qreal> &XA_bend, QVector<qreal> &YA_bend, qreal XAR, qreal YAR, qreal XBR, qreal YBR, qreal RL, qreal RR, qreal FL, qreal FR, qreal TAI, qreal TBI, qreal TT0, qreal XCC_origin, qreal XSS_origin, int idv, qreal UBAI);
    void calcBendingCurveFP(QVector<qreal> &XA_bend, QVector<qreal> &YA_bend, qreal XAR, qreal YAR, qreal XBR, qreal YBR, qreal RL, qreal RR, qreal FL, qreal FR, qreal TAI, qreal TBI, qreal TT0, qreal XCC_origin, qreal XSS_origin, int idv, qreal UBAI);
    void calcBendingCurveFF(QVector<qreal> &XA_bend, QVector<qreal> &YA_bend, qreal XAR, qreal YAR, qreal XBR, qreal YBR, qreal RL, qreal RR, qreal FL, qreal FR, qreal TAI, qreal TBI, qreal TT0, int idv, qreal UBAI);

};
} // namespace post3dapp

#endif // OUTPUTLINEITEMDeformation_H
