#ifndef CALCSIMPLE_H
#define CALCSIMPLE_H

#include <QRectF>
#include <QPointF>
#include "qmath.h"

namespace post3dapp{
class CalcSimple
{

public:

    CalcSimple() { }
    ~CalcSimple() { }

    /* tmax,tminの間の適当な目盛り値を返す */
    static qreal tic(qreal tmax, qreal tmin)
    {

        qreal tmp = tmax - tmin;
        if ( tmp < 0.0 ) return -tmp;
        qreal unit = 1.0;

        while (tmp >= 10.0) {
            tmp /= 10.0;
            unit *= 10.0;
        }
        while (tmp <= 1.0) {
            tmp *= 10.0;
            unit /= 10.0;
        }

        if (tmp > 8.0) return 2.0 * unit;
        else if (tmp > 4.0) return unit;
        else if (tmp > 2.0) return 0.5 * unit;
        else return 0.25 * unit;

    }

private:

};
} // namespace post3dapp

#endif

