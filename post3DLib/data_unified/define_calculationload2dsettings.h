#ifndef DEFINE_CALCULATIONLOAD2DSETTINGS_H
#define DEFINE_CALCULATIONLOAD2DSETTINGS_H

#include <QObject>
#include <QColor>
#include <QFont>
#include "define_settings.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class POST3D_CLASS_DLLSPEC CalculationLoad2DViewSettings{
public:
    CalculationLoad2DViewSettings(const CalculationLoad2DViewSettings&);
    explicit CalculationLoad2DViewSettings(
            bool _isDirectionVisible,
            const QColor& _jointColor,
            bool _isJointStringVisible,
            int _jointSize,
            const QColor& _lineColor,
            bool _isLineStringVisible,
            int _lineSize,
            const QColor& _planeColor,
            bool _isPlaneStringVisible,
            int _planeSize,
            const QColor& _selectColor,
            const QColor& _scaleColor,
            int _strsize,
            bool _jcheck,
            const QColor &_jcolor,
            int _jsize,
            bool _pcheck,
            const QColor &_pcolor,
            int _psize,
            bool _wcheck,
            const QColor &_wcolor,
            qreal _wsize,
            bool _dcheck,
            const QColor &_dcolor,
            qreal _dsize,
            bool _useScaledFont
            )
        :
          isDirectionVisible(_isDirectionVisible),
          jointColor(_jointColor),
          isJointStringVisible(_isJointStringVisible),
          jointSize(_jointSize),
          lineColor(_lineColor),
          isLineStringVisible(_isLineStringVisible),
          lineSize(_lineSize),
          planeColor(_planeColor),
          isPlaneStringVisible(_isPlaneStringVisible),
          planeSize(_planeSize),
          selectColor(_selectColor),
          scaleColor(_scaleColor),
          stringSize(_strsize),
          isJloadVisible(_jcheck), jloadColor(_jcolor), jloadSize(_jsize),
          isPloadVisible(_pcheck), ploadColor(_pcolor), ploadSize(_psize),
          isWloadVisible(_wcheck), wloadColor(_wcolor), wloadSize(_wsize),
          isDloadVisible(_dcheck), dloadColor(_dcolor), dloadSize(_dsize),
          useScaledFont(_useScaledFont)
    { }

    static CalculationLoad2DViewSettings getDefault();
    static CalculationLoad2DViewSettings readSettings(const QString&);
    void saveSettings(const QString&) const;

    bool getIsDirectionVisible() const {return isDirectionVisible;}
    QColor getJointColor() const {return jointColor;}
    bool getIsJointStringVisible() const {return isJointStringVisible;}
    int getJointSize() const {return jointSize;}
    QColor getLineColor() const {return lineColor;}
    bool getIsLineStringVisible() const {return isLineStringVisible;}
    int getLineSize() const {return lineSize;}
    QColor getPlaneColor() const {return planeColor;}
    bool getIsPlaneStringVisible() const {return isPlaneStringVisible;}
    int getPlaneSize() const {return planeSize;}
    QColor getSelectColor() const {return selectColor;}
    QColor getScaleColor() const {return scaleColor;}
    int getStringSize() const {return stringSize;}
    bool getJLoadVisible() const {return isJloadVisible;}
    QColor getJLoadColor() const {return jloadColor;}
    int getJLoadSize() const {return jloadSize;}
    bool getIsPLoadVisible() const {return isPloadVisible;}
    QColor getPLoadColor() const {return ploadColor;}
    int getPLoadSize() const {return ploadSize;}
    bool getIsWloadVisible() const {return isWloadVisible;}
    QColor getWLoadColor() const {return wloadColor;}
    qreal getWLoadSize() const {return wloadSize;}
    bool getIsDloadVisible() const {return isDloadVisible;}
    QColor getDloadColor() const {return dloadColor;}
    qreal getDloadSize() const {return dloadSize;}

    bool UseScaledFont() const{return useScaledFont;}
    void SetUseScaledFont(bool _useScaledFont);

private:
    CalculationLoad2DViewSettings(){}
    bool isDirectionVisible;

    QColor jointColor;
    bool isJointStringVisible;
    int jointSize;

    QColor lineColor;
    bool isLineStringVisible;
    int lineSize;

    QColor planeColor;
    bool isPlaneStringVisible;
    int planeSize;

    QColor selectColor;
    QColor scaleColor;

    int stringSize;

    bool isJloadVisible;
    QColor jloadColor;
    int jloadSize;

    bool isPloadVisible;
    QColor ploadColor;
    int ploadSize;

    bool isWloadVisible;
    QColor wloadColor;
    qreal wloadSize;

    bool isDloadVisible;
    QColor dloadColor;
    qreal dloadSize;

    bool useScaledFont;
};
} // namespace post3dapp

#endif // DEFINE_CALCULATIONLOAD2DSETTINGS_H
