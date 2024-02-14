#ifndef CALCLOADPLANELINEITEM_H
#define CALCLOADPLANELINEITEM_H

#include "calcloaditem_line.h"
namespace post3dapp{
class CalcLoadLinePlaneItem : public CalcLoadLineItem
{
public:

    CalcLoadLinePlaneItem(DATATYPE dtype, const QUuid &uid, const CalculationLoad2DViewSettings &setting,
                         const QList<QPointF> &plist );

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void createLinePath();

private:
    QColor planeColor;

};
} // namespace post3dapp
#endif
