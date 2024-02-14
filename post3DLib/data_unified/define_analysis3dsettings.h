#ifndef DEFINEOUT3DSETTINGS_H
#define DEFINEOUT3DSETTINGS_H

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


/* 解析　パース　表示設定 */
class POST3D_CLASS_DLLSPEC Analysis3DViewSettings{
public:
    Analysis3DViewSettings(const Analysis3DViewSettings&);
    explicit Analysis3DViewSettings(
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
        bool _useScaledFont,

            QColor _backGroundColor=Qt::lightGray,
            bool _hasShade=false,
            bool _useSmoothing=false,
            bool _isOrthoGraphic=true
            )
        : isJointVisible(_joint),
          jointColor(_jointcol),
          isLineVisible(_line),
          lineColor(_linecol),
          isPlaneVisible(_plane),
          planeColor(_planecol),
          isJnoVisible(_jno),
          jnoColor(_jnocol),
          jnoSize(_jnosize),
          isMemberNameVisible(_name),
          memberNameColor(_namecol),
          memberNameSize(_namesize),
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
          useScaledFont(_useScaledFont),

          backGroundColor(_backGroundColor),
          hasShade(_hasShade),
          useSmoothing(_useSmoothing),
          isOrthoGraphic(_isOrthoGraphic)
    {
    }
    bool getIsJointVisible() const{return isJointVisible;}
    QColor getJointColor() const{return jointColor;}
    bool getIsLineVisible() const{return isLineVisible;}
    QColor getLineColor() const{return lineColor;}
    bool getIsPlaneVisible() const{return isPlaneVisible;}
    QColor getPlaneColor() const{return planeColor;}
    bool getIsJnoVisible() const{return isJnoVisible;}
    QColor getJnoColor() const{return jnoColor;}
    int getJnoSize() const{return jnoSize;}
    bool getIsMemberNameVisible() const{return isMemberNameVisible;}
    QColor getMemberNameColor() const{return memberNameColor;}
    int getMemberNameSize() const{return memberNameSize;}
    bool getIsColorBarVisible() const{return isColorBarVisible;}
    QColor getColorBarTextColor() const{return colorBarTextColor;}
    int getColorBarTextSize() const{return colorBarTextSize;}
    int getValueAlpha() const{return valueAlpha;}
    QColor getSelectColor() const{return selectColor;}
    QColor getScaleColor() const{return scaleColor;}
    bool getIsFrScaleVisible() const{return isFrScaleVisible;}
    QUuid getFrScaleFloor() const{return frScaleFloor;}
    bool getIsFlScaleVisible() const{return isFlScaleVisible;}
    QPointF getFloorScaleXY() const{return floorScaleXY;}

    static Analysis3DViewSettings getDefault();
    static Analysis3DViewSettings readSettings(const QString&);
    void saveSettings(const QString&);
    bool UseScaledFont() const{return useScaledFont;}
    void SetScaledFont(bool _useScaledFont);


    QColor getBackGroundColor()const{return backGroundColor;}
    bool getHasShade()const{return hasShade;}
    bool getUseSmoothing()const{return useSmoothing;}
    bool getIsOrthoGraphic()const{return isOrthoGraphic;}
    int getLineWidth()const{return lineWidth;}
    int getFontSize()const{return fontSize;}
    bool getDrawLogo()const{return drawLogo;}
    bool getDrawCoordinate()const{return drawCoordinate;}
    QVector<bool> getDrawReactionDOF()const{return drawReactionDOF;}
    qreal getReactScale()const{return reactScale;}
    qreal getGraphScale()const{return graphScale;}
    int getGraphScaleType()const{return 2;}//graphScaleType;}
    int getNodeOrFace()const{return nodeOrFace;}

private:
    explicit Analysis3DViewSettings(){
        backGroundColor=Qt::lightGray;
        hasShade=false;
        useSmoothing=false;
        isOrthoGraphic=true;
    }
    bool isJointVisible;
    QColor jointColor;
    bool isLineVisible;
    QColor lineColor;
    bool isPlaneVisible;
    QColor planeColor;
    bool isJnoVisible;
    QColor jnoColor;
    int jnoSize;
    bool isMemberNameVisible;
    QColor memberNameColor;
    int memberNameSize;
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
    bool useScaledFont;

    QColor backGroundColor;
    bool  hasShade;
    bool useSmoothing;
    bool isOrthoGraphic;
    int lineWidth;
    int fontSize;
    bool drawLogo;
    bool drawCoordinate;
    QVector<bool> drawReactionDOF;
    qreal reactScale;
    qreal graphScale;
    int graphScaleType;
    int nodeOrFace;
};

} // namespace post3dapp

#endif
