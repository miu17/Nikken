#ifndef DEFINE_TEXTPOS_H
#define DEFINE_TEXTPOS_H
#include <QPointF>

enum TEXTPOS{TOPLEFT1,TOPLEFT2,TOPLEFT3,TOP1,TOP2,TOP3,TOPRIGHT1,TOPRIGHT2,TOPRIGHT3,LEFT,CENTER,RIGHT,
BOTTOMLEFT1,BOTTOMLEFT2,BOTTOMLEFT3,BOTTOM1,BOTTOM2,BOTTOM3,BOTTOMRIGHT1,BOTTOMRIGHT2,BOTTOMRIGHT3};

class TextPosFunctions{
public:
    static QPointF getModifiedTextPos(qreal x,qreal y,qreal ww,qreal hh,TEXTPOS pos){
        qreal modx=0;
        qreal mody=0;
        qreal margin=0.1;
        switch(pos){
        case TOPLEFT1:
            modx=-ww -margin;
            mody=margin;
            break;
        case TOPLEFT2:
            modx=-ww-margin;
            mody=hh+2.0*margin;
            break;
        case TOPLEFT3:
            modx=-ww-margin;
            mody=2.0*hh+3.0*margin;
            break;
        case TOP1:
            modx=-0.5*ww;
            mody=margin;
            break;
        case TOP2:
            modx=-0.5*ww;
            mody=hh+2.0*margin;
            break;
        case TOP3:
            modx=-0.5*ww;
            mody=2.0*hh+3.0*margin;
            break;
        case TOPRIGHT1:
            modx=margin;
            mody=margin;
            break;
        case TOPRIGHT2:
            modx=margin;
            mody=hh+2.0*margin;
            break;
        case TOPRIGHT3:
            modx=margin;
            mody=2.0*hh+3.0*margin;
            break;
        case LEFT:
            modx=-ww-margin;
            mody=-0.5*hh;
            break;
        case CENTER:
            modx=-0.5*ww;
            mody=-0.5*hh;
            break;
        case RIGHT:
            modx=margin;
            mody=-0.5*hh;
            break;
        case BOTTOMLEFT1:
            modx=-ww-margin;
            mody=-hh-margin;
            break;
        case BOTTOMLEFT2:
            modx=-ww-margin;
            mody=-2.0*hh-2.0*margin;
            break;
        case BOTTOMLEFT3:
            modx=-ww-margin;
            mody=-3.0*hh-3.0*margin;
            break;
        case BOTTOM1:
            modx=-0.5*ww;
            mody=-hh-margin;
            break;
        case BOTTOM2:
            modx=-0.5*ww;
            mody=-2.0*hh-2.0*margin;
            break;
        case BOTTOM3:
            modx=-0.5*ww;
            mody=-3.0*hh-3.0*margin;
            break;
        case BOTTOMRIGHT1:
            modx=margin;
            mody=-hh-margin;
            break;
        case BOTTOMRIGHT2:
            modx=margin;
            mody=-2.0*hh-2.0*margin;
            break;
        case BOTTOMRIGHT3:
            modx=margin;
            mody=-3.0*hh-3.0*margin;
            break;
        }
        return QPointF(x+modx,y+mody);
    }

};


#endif //DEFINE_TEXTPOS_H
