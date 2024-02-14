#ifndef DEFINECALC3DSETTINGS_H
#define DEFINECALC3DSETTINGS_H

#include <QBrush>
#include <QHash>
#include <QObject>
#include <QUuid>
#include "define_settings.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{

/* 荷重分布 図設定3D */
class POST3D_CLASS_DLLSPEC CalculationLoad3DViewSettings{
public:
    CalculationLoad3DViewSettings(const CalculationLoad3DViewSettings&);
    explicit CalculationLoad3DViewSettings(
        bool _joint,
        const QColor &_jointcol,
        bool _line,
        const QColor &_linecol,
        bool _plane,
        const QColor &_planecol,
        bool _jno,
        const QColor &_jnocol,
        int _jnosize,
        bool _name,
        const QColor &_namecol,
        int _namesize,
        bool _bar,
        const QColor &_barcol,
        int _barsize,
        int _alpha,
        const QColor &_select,
        const QColor &_scale,
        bool _frvisible,
        const QUuid &_fid,
        bool _flvisible,
        const QPointF &_pp,
        int _jloadsize,
        qreal _mloadsize,
        bool _jcheck,
        qreal _jminvalue,
        const QColor &_jmincolor,
        const QColor &_jmaxcolor,
        qreal _jmaxvalue,
        bool _pcheck,
        qreal _pminvalue,
        const QColor &_pmincolor,
        const QColor &_pmaxcolor,
        qreal _pmaxvalue,
        bool _wcheck,
        qreal _wminvalue,
        const QColor &_wmincolor,
        const QColor &_wmaxcolor,
        qreal _wmaxvalue,
        bool _dcheck,
        qreal _dminvalue,
        const QColor &_dmincolor,
        const QColor &_dmaxcolor,
        qreal _dmaxvalue,
        bool _useScaledFont)
        :
          isJointVisible(_joint),
          jointColor(_jointcol),
          isLineVisible(_line),
          lineColor(_linecol),
          isPlaneVisible(_plane),
          planeColor(_planecol),
          isJnoVisible(_jno),
          jnoColor(_jnocol),
          jnoSize(_jnosize),
          isNameVisible(_name),
          nameColor(_namecol),
          nameSize(_namesize),
          isColorBarVisible(_bar),
          colorBarTextColor(_barcol),
          colorBarTextSize(_barsize),
          valueAlpha(_alpha),
          selectColor(_select),
          scaleColor(_scale),
          isFrScaleVisible(_frvisible),
          frScaleFloor(_fid),
          isFlScaleVisible(_flvisible),
          floorScaleXY(_pp),
          jloadSize(_jloadsize),
          mloadSize(_mloadsize),
          isJloadVisible(_jcheck),
          jloadMinValue(_jminvalue),
          jloadMinColor(_jmincolor),
          jloadMaxColor(_jmaxcolor),
          jloadMaxValue(_jmaxvalue),
          isPloadVisible(_pcheck),
          ploadMinValue(_pminvalue),
          ploadMinColor(_pmincolor),
          ploadMaxColor(_pmaxcolor),
          ploadMaxValue(_pmaxvalue),
          isWloadVisible(_wcheck),
          wloadMinValue(_wminvalue),
          wloadMinColor(_wmincolor),
          wloadMaxColor(_wmaxcolor),
          wloadMaxValue(_wmaxvalue),
          isDloadVisible(_dcheck),
          dloadMinValue(_dminvalue),
          dloadMinColor(_dmincolor),
          dloadMaxColor(_dmaxcolor),
          dloadMaxValue(_dmaxvalue),
          useScaledFont(_useScaledFont)
    { }

    static CalculationLoad3DViewSettings getDefault();
    static CalculationLoad3DViewSettings readSettings(const QString&);
    void saveSettings(const QString&) const;


    bool getIsJointVisible() const {return isJointVisible;}
    QColor getJointColor() const {return jointColor;}
    bool getIsLineVisible() const {return isLineVisible;}
    QColor getLineColor() const {return lineColor;}
    bool getIsPlaneVisible() const {return isPlaneVisible;}
    QColor getPlaneColor() const {return planeColor;}
    bool getIsJnoVisible() const{return isJnoVisible;}
    QColor getJnoColor() const{return jnoColor;}
    int getJnoSize() const {return jnoSize;}
    bool getIsNameVisible() const{return isNameVisible;}
    QColor getNameColor() const{return nameColor;}
    int getNameSize() const{return nameSize;}
    bool getIsColorBarVisible() const {return isColorBarVisible;}
    QColor getColorBarTextColor() const{return colorBarTextColor;}
    int getColorBarTextSize() const {return colorBarTextSize;}
    int getValueAlpha() const {return valueAlpha;}
    QColor getSelectColor() const {return selectColor;}
    QColor getScaleColor() const {return scaleColor;}
    bool getIsFrScaleVisible() const{return isFrScaleVisible;}
    QUuid getFrScaleFloor() const{return frScaleFloor;}
    bool getIsFlScaleVisible() const{return isFlScaleVisible;}
    QPointF getFloorScaleXY() const{return floorScaleXY;}
    int getJloadSize() const{return jloadSize;}
    qreal getMloadSize() const{return mloadSize;}
    bool getIsJloadVisible() const{return isJloadVisible;}
    qreal getJloadMinValue() const{return jloadMinValue;}
    QColor getJloadMinColor() const{return jloadMinColor;}
    QColor getJloadMaxColor() const{return jloadMaxColor;}
    qreal getJloadMaxValue() const{return jloadMaxValue;}
    bool getIsPloadVisible() const{return isPloadVisible;}
    qreal getPloadMinValue() const{return ploadMinValue;}
    QColor getPloadMinColor() const{return ploadMinColor;}
    QColor getPloadMaxColor() const{return ploadMaxColor;}
    qreal getPloadMaxValue() const{return ploadMaxValue;}
    bool getIsWloadVisible() const{return isWloadVisible;}
    qreal getWloadMinValue() const{return wloadMinValue;}
    QColor getWloadMinColor() const{return wloadMinColor;}
    QColor getWloadMaxColor() const{return wloadMaxColor;}
    qreal getWloadMaxValue() const{return wloadMaxValue;}
    bool getIsDloadVisible() const{return isDloadVisible;}
    qreal getDloadMinValue() const{return dloadMinValue;}
    QColor getDloadMinColor() const{return dloadMinColor;}
    QColor getDloadMaxColor() const{return dloadMaxColor;}
    qreal getDloadMaxValue() const{return dloadMaxValue;}

    bool UseScaledFont() const{return useScaledFont;}
    void SetUseScaledFont(bool _useScaledFont);

    QColor getElementColor(ELEMENTTYPE etype) const {
        return ( etype == ELSLAB || etype == ELWALL || etype == ELUWALL ) ?
               planeColor : lineColor ;
    }

private:
    CalculationLoad3DViewSettings(){}
    bool isJointVisible;
    QColor jointColor;

    bool isLineVisible;
    QColor lineColor;

    bool isPlaneVisible;
    QColor planeColor;

    bool isJnoVisible;
    QColor jnoColor;
    int jnoSize;

    bool isNameVisible;
    QColor nameColor;
    int nameSize;

    bool isColorBarVisible;
    QColor colorBarTextColor;
    int colorBarTextSize;

    int valueAlpha;
    QColor selectColor;

    QColor scaleColor;
    bool isFrScaleVisible;
    QUuid frScaleFloor;

    bool isFlScaleVisible;
    QPointF floorScaleXY;

    int jloadSize;
    qreal mloadSize;

    bool isJloadVisible;
    qreal jloadMinValue;
    QColor jloadMinColor;
    QColor jloadMaxColor;
    qreal jloadMaxValue;

    bool isPloadVisible;
    qreal ploadMinValue;
    QColor ploadMinColor;
    QColor ploadMaxColor;
    qreal ploadMaxValue;

    bool isWloadVisible;
    qreal wloadMinValue;
    QColor wloadMinColor;
    QColor wloadMaxColor;
    qreal wloadMaxValue;

    bool isDloadVisible;
    qreal dloadMinValue;
    QColor dloadMinColor;
    QColor dloadMaxColor;
    qreal dloadMaxValue;

    bool useScaledFont;
};
} // namespace post3dapp

#endif
