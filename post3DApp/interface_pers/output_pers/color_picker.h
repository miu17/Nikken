#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H
#include <QObject>
#include <QColor>
#include <QDebug>
namespace post3dapp {

class ColorPicker : public QObject{
    Q_OBJECT
public:
    ColorPicker(qreal _min=0.0,qreal _max=0.0):colormin(_min),colormax(_max){
        colormap.clear();
        const int numcolor=256;
        for(int i=0;i<numcolor;i++){
            colormap.append(QColor(0,i,255));
        }
        for(int i=0;i<numcolor;i++){
            colormap.append(QColor(0,255,255-i));
        }
        for(int i=0;i<numcolor;i++){
            colormap.append(QColor(i,255,0));
        }
        for(int i=0;i<numcolor;i++){
            colormap.append(QColor(255,255-i,0));
        }
        //256*4色
    }
    QColor getColor()const{return QColor(0,255,0);}//デフォルト色
    QColor getColor(const qreal value)const{
        if(qAbs(colormin-colormax)<0.0001){return QColor(0,255,0);}//zero除算回避
        int colorindex=(int)((value-colormin)/(colormax-colormin)*(256*4));
        if(colorindex>=256*4){return QColor(255,0,0);}//colormax以上は赤
        else if(colorindex<0){return QColor(0,0,255);}//colormin以下は青
        else{return colormap[colorindex];}//間の時は配列を参照
    }
    void setMinMax(const qreal _min,const qreal _max){colormin=_min;colormax=_max;qDebug()<<"colorpicker min max="<<_min<<_max;}
    void setMask(const qreal _mask){colormask=_mask;}
    qreal getMin()const{return colormin;}
    qreal getMax()const{return colormax;}
    qreal getMask()const{return colormask;}
private:
    QList<QColor> colormap;
    qreal colormin;
    qreal colormax;
    qreal colormask;
};
}
#endif // COLOR_PICKER_H
