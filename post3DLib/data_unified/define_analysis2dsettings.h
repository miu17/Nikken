#ifndef DEFINE_OUTPUT2DSETTINGS_H
#define DEFINE_OUTPUT2DSETTINGS_H

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QObject>
#include "define_settings.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QSettings;

namespace post3dapp{
enum class OUTFIGTYPE;

/* 共通設定部 */
/* 部材長図 */
/* フェイス長図 */
/* 節点座標図 */
/* 剛域長図 */
/* 横座屈長さ図 */
/* 圧縮座屈長さ図 */
/* 節点重量図 */
/* 節点外力図 */
/* 節点外力M図 */
/* 剛比・曲げ剛性増大率図 */
/* 軸力マップ */
/* せん断力マップ */
/* 鉛直荷重マップ */
/* 層間変形マップ */
/* 層剛性マップ */
/* 変位図 */
/* 塑性率図 */
/* 降伏ステップ図 */
/* 降伏荷重倍率図 */

class POST3D_CLASS_DLLSPEC AnalysisBaseViewSettings
{
public :
    virtual void SetValueTextSize(qreal){}
    virtual void saveSettings(QSettings&) const = 0;
protected:
    explicit AnalysisBaseViewSettings(){};
    virtual ~AnalysisBaseViewSettings(){}
};

class POST3D_CLASS_DLLSPEC AnalysisCommon2DViewSetting :public AnalysisBaseViewSettings  {
public:
    AnalysisCommon2DViewSetting(const AnalysisCommon2DViewSetting&);
    explicit AnalysisCommon2DViewSetting(
            QColor _scaleColor,
            int _scaleSize,
            bool _gridStringVisible,
            bool _floorStringVisible,
            QColor _jointColor,
            int _jointSize,
            QColor _lineRcColor,
            QColor _lineSrcColor,
            QColor _lineSColor,
            QColor _lineCftColor,
            QColor _lineDamperColor,
            QColor _lineIsoColor,
            QColor _planeRcColor,
            QColor _planeSColor,
            QColor _planeDamperColor,
            QColor _supportColor,
            QColor _valueColor,
            int _valueTextSize,
            QColor _selectedColor,
            QColor _noteColor,
            int _noteTextSize,
            QColor _plasticSymbolColor,
            int _plasticSymbolSize):
        AnalysisBaseViewSettings(),
        scaleColor(_scaleColor),
        scaleSize(_scaleSize),
        gridStringVisible(_gridStringVisible),
        floorStringVisible(_floorStringVisible),
        jointColor(_jointColor),
        jointSize(_jointSize),
        lineRcColor(_lineRcColor),
        lineSrcColor(_lineSrcColor),
        lineSColor(_lineSColor),
        lineCftColor(_lineCftColor),
        lineDamperColor(_lineDamperColor),
        lineIsoColor(_lineIsoColor),
        planeRcColor(_planeRcColor),
        planeSColor(_planeSColor),
        planeDamperColor(_planeDamperColor),
        supportColor(_supportColor),
        valueColor(_valueColor),
        valueTextSize(_valueTextSize),
        selectedColor(_selectedColor),
        noteColor(_noteColor),
        noteTextSize(_noteTextSize),
        plasticSymbolColor(_plasticSymbolColor),
        plasticSymbolSize(_plasticSymbolSize){}

    virtual ~AnalysisCommon2DViewSetting(){}
    void SetValueTextSize(qreal _size) final{valueTextSize = _size;}
    static AnalysisCommon2DViewSetting* getDefault();
    static AnalysisCommon2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;

    QColor getScaleColor() const{return scaleColor;}
    int getScaleSize() const{return scaleSize;}
    bool getGridStringVisible() const{return gridStringVisible;}
    bool getFloorStringVisible() const{return floorStringVisible;}
    QColor getJointColor() const{return jointColor;}
    int getJointSize() const{return jointSize;}
    QColor getLineRcColor() const{return lineRcColor;}
    QColor getLineSrcColor() const{return lineSrcColor;}
    QColor getLineSColor() const{return lineSColor;}
    QColor getLineCftColor() const{return lineCftColor;}
    QColor getLineDamperColor() const{return lineDamperColor;}
    QColor getLineIsoColor() const{return lineIsoColor;}
    QColor getPlaneRcColor() const{return planeRcColor;}
    QColor getPlaneSColor() const{return planeSColor;}
    QColor getPlaneDamperColor() const{return planeDamperColor;}
    QColor getSupportColor() const{return supportColor;}
    QColor getValueColor() const{return valueColor;}
    int getValueTextSize() const{return valueTextSize;}
    QColor getSelectedColor() const{return selectedColor;}
    QColor getNoteColor() const{return noteColor;}
    int getNoteTextSize() const{return noteTextSize;}
    QColor getPlasticSymbolColor() const{return plasticSymbolColor;}
    int getPlasticSymbolSize() const{return plasticSymbolSize;}

protected:
    AnalysisCommon2DViewSetting(){}
    QColor scaleColor;
    int scaleSize;
    bool gridStringVisible;
    bool floorStringVisible;
    QColor jointColor;
    int jointSize;
    QColor lineRcColor;
    QColor lineSrcColor;
    QColor lineSColor;
    QColor lineCftColor;
    QColor lineDamperColor;
    QColor lineIsoColor;
    QColor planeRcColor;
    QColor planeSColor;
    QColor planeDamperColor;
    QColor supportColor;

    QColor valueColor;
    int valueTextSize;
    QColor selectedColor;
    QColor noteColor;
    int noteTextSize;

    QColor plasticSymbolColor;
    int plasticSymbolSize;
};


/* 反力マップ */
/* 組合せ鉛直反力マップ */

class POST3D_CLASS_DLLSPEC AnalysisReactionForce2DViewSetting : public AnalysisCommon2DViewSetting {
public:
    AnalysisReactionForce2DViewSetting(const AnalysisReactionForce2DViewSetting&);
    explicit AnalysisReactionForce2DViewSetting(
            QColor _scaleColor,
            int _scaleSize,
            bool _gridStringVisible,
            bool _floorStringVisible,
            QColor _jointColor,
            int _jointSize,
            QColor _lineRcColor,
            QColor _lineSrcColor,
            QColor _lineSColor,
            QColor _lineCftColor,
            QColor _lineDamperColor,
            QColor _lineIsoColor,
            QColor _planeRcColor,
            QColor _planeSColor,
            QColor _planeDamperColor,
            QColor _supportColor,
            QColor _valueColor,
            int _valueTextSize,
            QColor _selectedColor,
            QColor _noteColor,
            int _noteTextSize,
            QColor _plasticSymbolColor,
            int _plasticSymbolSize,
            QColor _minusColor,
            QColor _plusColor):
        AnalysisCommon2DViewSetting(_scaleColor,
                                  _scaleSize,
                                  _gridStringVisible,
                                  _floorStringVisible,
                                  _jointColor,
                                  _jointSize,
                                  _lineRcColor,
                                  _lineSrcColor,
                                  _lineSColor,
                                  _lineCftColor,
                                  _lineDamperColor,
                                  _lineIsoColor,
                                  _planeRcColor,
                                  _planeSColor,
                                  _planeDamperColor,
                                  _supportColor,
                                  _valueColor,
                                  _valueTextSize,
                                  _selectedColor,
                                  _noteColor,
                                  _noteTextSize,
                                  _plasticSymbolColor,
                                  _plasticSymbolSize),
        minusColor(_minusColor),
        plusColor(_plusColor){}
    static AnalysisReactionForce2DViewSetting* getDefault();
    static AnalysisReactionForce2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;

    QColor getMinusColor() const {return minusColor;}
    QColor getPlusColor() const {return plusColor;}
protected:
    AnalysisReactionForce2DViewSetting(){}
    QColor minusColor;
    QColor plusColor;
};


/* 軸力図 */
class POST3D_CLASS_DLLSPEC AnalysisAxialForce2DViewSetting : public AnalysisCommon2DViewSetting {
public:
    AnalysisAxialForce2DViewSetting(const AnalysisAxialForce2DViewSetting&);
    explicit AnalysisAxialForce2DViewSetting(
            QColor _scaleColor,
            int _scaleSize,
            bool _gridStringVisible,
            bool _floorStringVisible,
            QColor _jointColor,
            int _jointSize,
            QColor _lineRcColor,
            QColor _lineSrcColor,
            QColor _lineSColor,
            QColor _lineCftColor,
            QColor _lineDamperColor,
            QColor _lineIsoColor,
            QColor _planeRcColor,
            QColor _planeSColor,
            QColor _planeDamperColor,
            QColor _supportColor,
            QColor _valueColor,
            int _valueTextSize,
            QColor _selectedColor,
            QColor _noteColor,
            int _noteTextSize,
            QColor _plasticSymbolColor,
            int _plasticSymbolSize,
            QColor _axialMinusColor,
            QColor _axialPlusColor,
            double _stressScale
            ):
        AnalysisCommon2DViewSetting(_scaleColor,
                                  _scaleSize,
                                  _gridStringVisible,
                                  _floorStringVisible,
                                  _jointColor,
                                  _jointSize,
                                  _lineRcColor,
                                  _lineSrcColor,
                                  _lineSColor,
                                  _lineCftColor,
                                  _lineDamperColor,
                                  _lineIsoColor,
                                  _planeRcColor,
                                  _planeSColor,
                                  _planeDamperColor,
                                  _supportColor,
                                  _valueColor,
                                  _valueTextSize,
                                  _selectedColor,
                                  _noteColor,
                                  _noteTextSize,
                                  _plasticSymbolColor,
                                  _plasticSymbolSize),
        axialMinusColor(_axialMinusColor),
        axialPlusColor(_axialPlusColor),
        axialScale(_stressScale){}

    static AnalysisAxialForce2DViewSetting* getDefault();
    static AnalysisAxialForce2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;

    QColor getAxialMinusColor() const{return axialMinusColor;}
    QColor getAxialPlusColor() const{return axialPlusColor;}
    double getAxialScale() const{return axialScale;}
protected:
    AnalysisAxialForce2DViewSetting(){}
    QColor axialMinusColor;
    QColor axialPlusColor;
    double axialScale;
};

/* 変形図 */
class POST3D_CLASS_DLLSPEC AnalysisDeformation2DViewSetting : public AnalysisCommon2DViewSetting {
public:
    AnalysisDeformation2DViewSetting(const AnalysisDeformation2DViewSetting&);
    explicit AnalysisDeformation2DViewSetting(
            QColor _scaleColor,
            int _scaleSize,
            bool _gridStringVisible,
            bool _floorStringVisible,
            QColor _jointColor,
            int _jointSize,
            QColor _lineRcColor,
            QColor _lineSrcColor,
            QColor _lineSColor,
            QColor _lineCftColor,
            QColor _lineDamperColor,
            QColor _lineIsoColor,
            QColor _planeRcColor,
            QColor _planeSColor,
            QColor _planeDamperColor,
            QColor _supportColor,
            QColor _valueColor,
            int _valueTextSize,
            QColor _selectedColor,
            QColor _noteColor,
            int _noteTextSize,
            QColor _plasticSymbolColor,
            int _plasticSymbolSize,
            QColor _beforeColor,
            QColor _afterColor,
            double _deformScale):
        AnalysisCommon2DViewSetting(_scaleColor,
                                  _scaleSize,
                                  _gridStringVisible,
                                  _floorStringVisible,
                                  _jointColor,
                                  _jointSize,
                                  _lineRcColor,
                                  _lineSrcColor,
                                  _lineSColor,
                                  _lineCftColor,
                                  _lineDamperColor,
                                  _lineIsoColor,
                                  _planeRcColor,
                                  _planeSColor,
                                  _planeDamperColor,
                                  _supportColor,
                                  _valueColor,
                                  _valueTextSize,
                                  _selectedColor,
                                  _noteColor,
                                  _noteTextSize,
                                  _plasticSymbolColor,
                                  _plasticSymbolSize),
        beforeColor(_beforeColor),
        afterColor(_afterColor),
        deformScale(_deformScale){}
    static AnalysisDeformation2DViewSetting* getDefault();
    static AnalysisDeformation2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;

    QColor getBeforeColor() const{return beforeColor;}
    QColor getAfterColor() const{return afterColor;}
    double getDeformScale() const{return deformScale;}
protected:
    AnalysisDeformation2DViewSetting(){}
    QColor beforeColor;
    QColor afterColor;
    double deformScale;
};



/* 断面検討検定比図 */
/* せん断余裕率図E */
/* せん断余裕率図U */
class POST3D_CLASS_DLLSPEC AnalysisCheck2DViewSetting : public AnalysisCommon2DViewSetting {
public:
    AnalysisCheck2DViewSetting(const AnalysisCheck2DViewSetting&);
    explicit AnalysisCheck2DViewSetting(
            QColor _scaleColor,
            int _scaleSize,
            bool _gridStringVisible,
            bool _floorStringVisible,
            QColor _jointColor,
            int _jointSize,
            QColor _lineRcColor,
            QColor _lineSrcColor,
            QColor _lineSColor,
            QColor _lineCftColor,
            QColor _lineDamperColor,
            QColor _lineIsoColor,
            QColor _planeRcColor,
            QColor _planeSColor,
            QColor _planeDamperColor,
            QColor _supportColor,
            QColor _valueColor,
            int _valueTextSize,
            QColor _selectedColor,
            QColor _noteColor,
            int _noteTextSize,
            QColor _plasticSymbolColor,
            int _plasticSymbolSize,
            QColor _ngColor,
            bool _useGradiationColor):
        AnalysisCommon2DViewSetting(_scaleColor,
                                  _scaleSize,
                                  _gridStringVisible,
                                  _floorStringVisible,
                                  _jointColor,
                                  _jointSize,
                                  _lineRcColor,
                                  _lineSrcColor,
                                  _lineSColor,
                                  _lineCftColor,
                                  _lineDamperColor,
                                  _lineIsoColor,
                                  _planeRcColor,
                                  _planeSColor,
                                  _planeDamperColor,
                                  _supportColor,
                                  _valueColor,
                                  _valueTextSize,
                                  _selectedColor,
                                  _noteColor,
                                  _noteTextSize,
                                  _plasticSymbolColor,
                                  _plasticSymbolSize),
        ngColor(_ngColor),
        useGradiationColor(_useGradiationColor){}
    static AnalysisCheck2DViewSetting* getDefault();
    static AnalysisCheck2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;
    QColor getNgColor() const{return ngColor;}
    bool getUseGradiationColor() const{return useGradiationColor;}

protected:
    AnalysisCheck2DViewSetting(){}
    QColor ngColor;
    bool useGradiationColor;
};

/* CMQ図 */
/* 節点応力図 */
/* フェイス応力図 */
/* ねじれモーメント図 */
/* 許容応力図 */
/* ランク図 */
class POST3D_CLASS_DLLSPEC AnalysisStress2DViewSetting : public AnalysisCommon2DViewSetting {
public:
    AnalysisStress2DViewSetting(const AnalysisStress2DViewSetting&);
    explicit AnalysisStress2DViewSetting(
            QColor _scaleColor,
            int _scaleSize,
            bool _gridStringVisible,
            bool _floorStringVisible,
            QColor _jointColor,
            int _jointSize,
            QColor _lineRcColor,
            QColor _lineSrcColor,
            QColor _lineSColor,
            QColor _lineCftColor,
            QColor _lineDamperColor,
            QColor _lineIsoColor,
            QColor _planeRcColor,
            QColor _planeSColor,
            QColor _planeDamperColor,
            QColor _supportColor,
            QColor _valueColor,
            int _valueTextSize,
            QColor _selectedColor,
            QColor _noteColor,
            int _noteTextSize,
            QColor _plasticSymbolColor,
            int _plasticSymbolSize,
            QColor _stressColor,
            double _stressScale,
            bool _valueNVisible,
            bool _valueMVisible,
            bool _valueQVisible,
            bool _columnStressVisible,
            QColor _columnStressValueColor,
            bool _girderStressVisible,
            QColor _girderStressValueColor,
            bool _braceStressVisible,
            QColor _braceStressValueColor,
            bool _wallStressVisible,
            QColor _wallStressValueColor,
            bool _slabStressVisible,
            QColor _slabStressValueColor,
            bool _panelStressVisible,
            QColor _panelStressValueColor,
            bool _springStressVisible,
            QColor _springStressValueColor,
            bool _damperStressVisible,
            QColor _damperStressValueColor,
            bool _isoStressVisible,
            QColor _isoStressValueColor):
        AnalysisCommon2DViewSetting(_scaleColor,
                                  _scaleSize,
                                  _gridStringVisible,
                                  _floorStringVisible,
                                  _jointColor,
                                  _jointSize,
                                  _lineRcColor,
                                  _lineSrcColor,
                                  _lineSColor,
                                  _lineCftColor,
                                  _lineDamperColor,
                                  _lineIsoColor,
                                  _planeRcColor,
                                  _planeSColor,
                                  _planeDamperColor,
                                  _supportColor,
                                  _valueColor,
                                  _valueTextSize,
                                  _selectedColor,
                                  _noteColor,
                                  _noteTextSize,
                                  _plasticSymbolColor,
                                  _plasticSymbolSize),
        stressColor(_stressColor),
        stressScale(_stressScale),
        valueNVisible(_valueNVisible),
        valueMVisible(_valueMVisible),
        valueQVisible(_valueQVisible),
        columnStressVisible(_columnStressVisible),
        columnStressValueColor(_columnStressValueColor),
        girderStressVisible(_girderStressVisible),
        girderStressValueColor(_girderStressValueColor),
        braceStressVisible(_braceStressVisible),
        braceStressValueColor(_braceStressValueColor),
        wallStressVisible(_wallStressVisible),
        wallStressValueColor(_wallStressValueColor),
        slabStressVisible(_slabStressVisible),
        slabStressValueColor(_slabStressValueColor),
        panelStressVisible(_panelStressVisible),
        panelStressValueColor(_panelStressValueColor),
        springStressVisible(_springStressVisible),
        springStressValueColor(_springStressValueColor),
        damperStressVisible(_damperStressVisible),
        damperStressValueColor(_damperStressValueColor),
        isoStressVisible(_isoStressVisible),
        isoStressValueColor(_isoStressValueColor){}

    static AnalysisStress2DViewSetting* getDefault();
    static AnalysisStress2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;
    QColor getStressColor() const{return stressColor;}
    double getStressScale() const{return stressScale;}
    bool getValueNVisible() const{return valueNVisible;}
    bool getValueMVisible() const{return valueMVisible;}
    bool getValueQVisible() const{return valueQVisible;}

    bool getColumnStressVisible() const{return columnStressVisible;}
    QColor getColumnStressValueColor() const{return columnStressValueColor;}
    bool getGirderStressVisible() const{return girderStressVisible;}
    QColor getGirderStressValueColor() const{return girderStressValueColor;}
    bool getBraceStressVisible() const{return braceStressVisible;}
    QColor getBraceStressValueColor() const{return braceStressValueColor;}
    bool getWallStressVisible() const{return wallStressVisible;}
    QColor getWallStressValueColor() const{return wallStressValueColor;}
    bool getSlabStressVisible() const{return slabStressVisible;}
    QColor getSlabStressValueColor() const{return slabStressValueColor;}
    bool getPanelStressVisible() const{return panelStressVisible;}
    QColor getPanelStressValueColor() const{return panelStressValueColor;}
    bool getSpringStressVisible() const{return springStressVisible;}
    QColor getSpringStressValueColor() const{return springStressValueColor;}
    bool geteDamperStressVisible() const{return damperStressVisible;}
    QColor getDamperStressValueColor() const{return damperStressValueColor;}
    bool getIsoStressVisible() const{return isoStressVisible;}
    QColor getIsoStressValueColor() const{return isoStressValueColor;}

protected:
    AnalysisStress2DViewSetting(){}
    QColor stressColor;
    double stressScale;
    bool valueNVisible;
    bool valueMVisible;
    bool valueQVisible;

    bool columnStressVisible;
    QColor columnStressValueColor;
    bool girderStressVisible;
    QColor girderStressValueColor;
    bool braceStressVisible;
    QColor braceStressValueColor;
    bool wallStressVisible;
    QColor wallStressValueColor;
    bool slabStressVisible;
    QColor slabStressValueColor;
    bool panelStressVisible;
    QColor panelStressValueColor;
    bool springStressVisible;
    QColor springStressValueColor;
    bool damperStressVisible;
    QColor damperStressValueColor;
    bool isoStressVisible;
    QColor isoStressValueColor;

};

/* 層せん断力中心・剛心伏図 */
class POST3D_CLASS_DLLSPEC AnalysisCenterOfRigidity2DViewSetting : public AnalysisCommon2DViewSetting {
public:
    AnalysisCenterOfRigidity2DViewSetting(const AnalysisCenterOfRigidity2DViewSetting&);
    explicit AnalysisCenterOfRigidity2DViewSetting(
            QColor _scaleColor,
            int _scaleSize,
            bool _gridStringVisible,
            bool _floorStringVisible,
            QColor _jointColor,
            int _jointSize,
            QColor _lineRcColor,
            QColor _lineSrcColor,
            QColor _lineSColor,
            QColor _lineCftColor,
            QColor _lineDamperColor,
            QColor _lineIsoColor,
            QColor _planeRcColor,
            QColor _planeSColor,
            QColor _planeDamperColor,
            QColor _supportColor,
            QColor _valueColor,
            int _valueTextSize,
            QColor _selectedColor,
            QColor _noteColor,
            int _noteTextSize,
            QColor _plasticSymbolColor,
            int _plasticSymbolSize,
            QColor _shearForceColor,
            QColor _rigidityColor,
            int _rigiditySize):
        AnalysisCommon2DViewSetting(_scaleColor,
                                  _scaleSize,
                                  _gridStringVisible,
                                  _floorStringVisible,
                                  _jointColor,
                                  _jointSize,
                                  _lineRcColor,
                                  _lineSrcColor,
                                  _lineSColor,
                                  _lineCftColor,
                                  _lineDamperColor,
                                  _lineIsoColor,
                                  _planeRcColor,
                                  _planeSColor,
                                  _planeDamperColor,
                                  _supportColor,
                                  _valueColor,
                                  _valueTextSize,
                                  _selectedColor,
                                  _noteColor,
                                  _noteTextSize,
                                  _plasticSymbolColor,
                                  _plasticSymbolSize
                                  ),
        shearForceColor(_shearForceColor),
        rigidityColor(_rigidityColor),
        rigiditySize(_rigiditySize){}
    static AnalysisCenterOfRigidity2DViewSetting* getDefault();
    static AnalysisCenterOfRigidity2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;

    QColor getShearForceColor() const{return shearForceColor;}
    QColor getRigidityColor() const{return rigidityColor;}
    int getRigiditySize() const{return rigiditySize;}

protected:
    AnalysisCenterOfRigidity2DViewSetting(){}
    QColor shearForceColor;
    QColor rigidityColor;
    int rigiditySize;
};

/* モデル図 */
class POST3D_CLASS_DLLSPEC AnalysisModel2DViewSetting : public AnalysisBaseViewSettings {
public :
    AnalysisModel2DViewSetting(const AnalysisModel2DViewSetting&);
    explicit AnalysisModel2DViewSetting(
            QColor _scaleColor,
            int _scaleSize,
            bool _gridStringVisible,
            bool _floorStringVisible,
            QColor _jointColor,
            int _jointSize,
            QColor _lineRcColor,
            QColor _lineSrcColor,
            QColor _lineSColor,
            QColor _lineCftColor,
            QColor _lineDamperColor,
            QColor _lineIsoColor,
            QColor _planeRcColor,
            QColor _planeSColor,
            QColor _planeDamperColor,
            QColor _supportColor,
            QColor _valueColor,
            int _valueTextSize,
            QColor _selectedColor,
            QColor _noteColor,
            int _noteTextSize,
            bool _columnMarkVisible,
            bool _girderMarkVisible,
            bool _braceMarkVisible,
            bool _wallMarkVisible,
            bool _steelWallMarkVisible,
            bool _foundationWallMarkVisible,
            bool _slabMarkVisible,
            bool _damperMarkVisible,
            bool _isoMarkVisible,
            int _markSize,
            bool _jointNumberVisible,
            QColor _jointNumberColor,
            int _jointNumberSize,
            bool _boundaryConditionVisible,
            QColor _boundaryConditionColor,
            int _boundaryConditionSize,
            bool _directionVisible,
            QColor _zwallColor,
            QColor _rigidRangeColor,
            QColor _panelZoneColor)
        : AnalysisBaseViewSettings(),
          scaleColor(_scaleColor),
          scaleSize(_scaleSize),
          gridStringVisible(_gridStringVisible),
          floorStringVisible(_floorStringVisible),
          jointColor(_jointColor),
          jointSize(_jointSize),
          lineRcColor(_lineRcColor),
          lineSrcColor(_lineSrcColor),
          lineSColor(_lineSColor),
          lineCftColor(_lineCftColor),
          lineDamperColor(_lineDamperColor),
          lineIsoColor(_lineIsoColor),
          planeRcColor(_planeRcColor),
          planeSColor(_planeSColor),
          planeDamperColor(_planeDamperColor),
          supportColor(_supportColor),
          valueColor(_valueColor),
          valueTextSize(_valueTextSize),
          selectedColor(_selectedColor),
          noteColor(_noteColor),
          noteTextSize(_noteTextSize),
          columnMarkVisible(_columnMarkVisible),
          girderMarkVisible(_girderMarkVisible),
          braceMarkVisible(_braceMarkVisible),
          wallMarkVisible(_wallMarkVisible),
          steelWallMarkVisible(_steelWallMarkVisible),
          foundationWallMarkVisible(_foundationWallMarkVisible),
          slabMarkVisible(_slabMarkVisible),
          damperMarkVisible(_damperMarkVisible),
          isoMarkVisible(_isoMarkVisible),
          markSize(_markSize),
          jointNumberVisible(_jointNumberVisible),
          jointNumberColor(_jointNumberColor),
          jointNumberSize(_jointNumberSize),
          boundaryConditionVisible(_boundaryConditionVisible),
          boundaryConditionColor(_boundaryConditionColor),
          boundaryConditionSize(_boundaryConditionSize),
          directionVisible(_directionVisible),
          zwallColor(_zwallColor),
          rigidRangeColor(_rigidRangeColor),
          panelZoneColor(_panelZoneColor){}
    static AnalysisModel2DViewSetting* getDefault();
    static AnalysisModel2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;
    void SetValueTextSize(qreal _size) override {valueTextSize = _size;}

    QColor getScaleColor() const{return scaleColor;}
    int getScaleSize() const {return scaleSize;}
    bool getGridStringVisible() const{return gridStringVisible;}
    bool getFloorStringVisible() const{return floorStringVisible;}
    QColor getJointColor() const{return jointColor;}
    int getJointSize() const{return jointSize;}
    QColor getLineRcColor() const{return lineRcColor;}
    QColor getLineSrcColor() const{return lineSrcColor;}
    QColor getLineSColor() const{return lineSColor;}
    QColor getLineCftColor() const{return lineCftColor;}
    QColor getLineDamperColor() const{return lineDamperColor;}
    QColor getLineIsoColor() const{return lineIsoColor;}
    QColor getPlaneRcColor() const{return planeRcColor;}
    QColor getPlaneSColor() const{return planeSColor;}
    QColor getPlaneDamperColor() const{return planeDamperColor;}
    QColor getSupportColor() const{return supportColor;}
    QColor getValueColor() const{return valueColor;}
    int getValueTextSize() const{return valueTextSize;}
    QColor getSelectedColor() const{return selectedColor;}
    QColor getNoteColor() const{return noteColor;}
    int getNoteTextSize() const{return noteTextSize;}
    bool getColumnMarkVisible() const{return columnMarkVisible;}
    bool getGirderMarkVisible() const{return girderMarkVisible;}
    bool getBraceMarkVisible() const{return braceMarkVisible;}
    bool getWallMarkVisible() const{return wallMarkVisible;}
    bool getSteelWallMarkVisible() const{return steelWallMarkVisible;}
    bool getFoundationWallMarkVisible() const{return foundationWallMarkVisible;}
    bool getSlabMarkVisible() const{return slabMarkVisible;}
    bool getDamperMarkVisible() const{return damperMarkVisible;}
    bool getIsoMarkVisible() const{return isoMarkVisible;}
    int getMarkSize() const{return markSize;}
    bool getJointNumberVisible() const{return jointNumberVisible;}
    QColor getJointNumberColor() const{return jointNumberColor;}
    int getJointNumberSize() const{return jointNumberSize;}
    bool getBoundaryConditionVisible() const{return boundaryConditionVisible;}
    QColor getBoundaryConditionColor() const{return boundaryConditionColor;}
    int getBoundaryConditionSize() const{return boundaryConditionSize;}
    bool getDirectionVisible() const{return directionVisible;}
    QColor getZwallColor() const{return zwallColor;}
    QColor getRigidRangeColor() const{return rigidRangeColor;}
    QColor getPanelZoneColor() const{return panelZoneColor;}

protected:
    AnalysisModel2DViewSetting(){}
    QColor scaleColor;
    int scaleSize;
    bool gridStringVisible;
    bool floorStringVisible;
    QColor jointColor;
    int jointSize;
    QColor lineRcColor;
    QColor lineSrcColor;
    QColor lineSColor;
    QColor lineCftColor;
    QColor lineDamperColor;
    QColor lineIsoColor;
    QColor planeRcColor;
    QColor planeSColor;
    QColor planeDamperColor;
    QColor supportColor;

    QColor valueColor;
    int valueTextSize;
    QColor selectedColor;
    QColor noteColor;
    int noteTextSize;
    bool columnMarkVisible;
    bool girderMarkVisible;
    bool braceMarkVisible;
    bool wallMarkVisible;
    bool steelWallMarkVisible;
    bool foundationWallMarkVisible;
    bool slabMarkVisible;
    bool damperMarkVisible;
    bool isoMarkVisible;
    int markSize;

    bool jointNumberVisible;
    QColor jointNumberColor;
    int jointNumberSize;

    bool boundaryConditionVisible;
    QColor boundaryConditionColor;
    int boundaryConditionSize;
    bool directionVisible;
    QColor zwallColor;
    QColor rigidRangeColor;
    QColor panelZoneColor;
};

/* グラフ　Qデルタ */
/* グラフ　Qデルタ架構毎 */
/* グラフ　Qデルタフロア毎 */
/* グラフ　Qシータ */
/* グラフ　Qシータ架構毎 */
/* グラフ　Qシータフロア毎 */

class POST3D_CLASS_DLLSPEC AnalysisGraphBaseViewSettings : public AnalysisBaseViewSettings
{
protected:
    explicit AnalysisGraphBaseViewSettings():AnalysisBaseViewSettings(){}
    virtual ~AnalysisGraphBaseViewSettings(){}


private:
};

class POST3D_CLASS_DLLSPEC AnalysisGraphQDelta2DViewSetting :public AnalysisGraphBaseViewSettings {
public:
    AnalysisGraphQDelta2DViewSetting(const AnalysisGraphQDelta2DViewSetting&);
    explicit AnalysisGraphQDelta2DViewSetting(
            bool _titleVisible,
            QColor _plasticSymbolColor,
            QColor _plasticStepColor,
            int _axisValueSize,
            int _axisTextSize,
            int _titleTextSize,
            int _symbolSize,
            int _symbolLabelSize,

            bool _minmax_auto_q,
            double _minmax_min_q,
            double _minmax_max_q,

            bool _minmax_auto_d,
            double _minmax_min_d,
            double _minmax_max_d,

            bool _minmax_auto_rad,
            double _minmax_min_rad,
            double _minmax_max_rad,

            bool _ruledLineVisible,
            QColor _ruledLineColor,
            Qt::PenStyle _ruledLineStyle
            ):
        AnalysisGraphBaseViewSettings(),
        titleVisible(_titleVisible),
        plasticSymbolColor(_plasticSymbolColor),
        plasticStepColor(_plasticStepColor),
        axisLabelSize(_axisValueSize),
        axisScaleTextSize(_axisTextSize),
        titleTextSize(_titleTextSize),
        symbolSize(_symbolSize),
        symbolLabelSize(_symbolLabelSize),
        minmax_auto_q(_minmax_auto_q),
        minmax_min_q(_minmax_min_q),
        minmax_max_q(_minmax_max_q),
        minmax_auto_d(_minmax_auto_d),
        minmax_min_d(_minmax_min_d),
        minmax_max_d(_minmax_max_d),
        minmax_auto_rad(_minmax_auto_rad),
        minmax_min_rad(_minmax_min_rad),
        minmax_max_rad(_minmax_max_rad),
        ruledLineVisible(_ruledLineVisible),
        ruledLineColor(_ruledLineColor),
        ruledLineStyle(_ruledLineStyle){}
    static AnalysisGraphQDelta2DViewSetting* getDefault();
    static AnalysisGraphQDelta2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;

    bool getTitleVisible() const{return titleVisible;}
    QColor getPlasticSymbolColor() const{return plasticSymbolColor;}
    QColor getPlasticStepColor() const{return plasticStepColor;}
    int getAxisLabelSize() const{return axisLabelSize;}
    int getAxisScaleTextSize() const{return axisScaleTextSize;}
    int getTitleTextSize() const{return titleTextSize;}
    int getSymbolSize() const{return symbolSize;}
    int getSymbolLabelSize() const{return symbolLabelSize;}

    bool getMinMaxAutoQ() const{return minmax_auto_q;}
    double getMinMaxMinQ() const{return minmax_min_q;}
    double getMinMaxMaxQ() const{return minmax_max_q;}
    bool getMinMaxAutoD() const{return minmax_auto_d;}
    double getMinMaxMinD() const{return minmax_min_d;}
    double getMinMaxMaxD() const{return minmax_max_d;}
    bool getMinMaxAutoRad() const{return minmax_auto_rad;}
    double getMinMaxMinRad() const{return minmax_min_rad;}
    double getMinMaxMaxRad() const{return minmax_max_rad;}
    bool getRuledLineVisible() const{return ruledLineVisible;}
    QColor getRuledLineColor() const{return ruledLineColor;}
    Qt::PenStyle getRuledLineStyle() const{return ruledLineStyle;}
protected:
    AnalysisGraphQDelta2DViewSetting(){}
    bool titleVisible;
    QColor plasticSymbolColor;
    QColor plasticStepColor;

    int axisLabelSize;
    int axisScaleTextSize;
    int titleTextSize;
    int symbolSize;
    int symbolLabelSize;

    bool minmax_auto_q;
    double minmax_min_q;
    double minmax_max_q;

    bool minmax_auto_d;
    double minmax_min_d;
    double minmax_max_d;

    bool minmax_auto_rad;
    double minmax_min_rad;
    double minmax_max_rad;

    bool ruledLineVisible;
    QColor ruledLineColor;
    Qt::PenStyle ruledLineStyle;

};

/* グラフ　必要保有水平耐力図 */
class POST3D_CLASS_DLLSPEC AnalysisGraphHorizontalCapacity2DViewSetting :public AnalysisGraphBaseViewSettings {
public:
    AnalysisGraphHorizontalCapacity2DViewSetting(const AnalysisGraphHorizontalCapacity2DViewSetting&);
    explicit AnalysisGraphHorizontalCapacity2DViewSetting(
            bool _titleVisible,
            int _axisLabelSize,
            int _axisValueSize,
            int _titleLabelSize,
            bool _minmax_auto_q,
            double _minmax_min_q,
            double _minmax_max_q,
            bool _style_visible_u,
            QColor _style_color_u,
            Qt::PenStyle _style_style_u,
            bool _style_visible_un,
            QColor _style_color_un,
            Qt::PenStyle _style_style_un,
            bool _style_visible_ruler,
            QColor _style_color_ruler,
            Qt::PenStyle _style_style_ruler
            ):
        AnalysisGraphBaseViewSettings(),
        titleVisible(_titleVisible),
        axisLabelSize(_axisLabelSize),
        axisScaleTextSize(_axisValueSize),
        titleLabelSize(_titleLabelSize),
        minmax_auto_q(_minmax_auto_q),
        minmax_min_q(_minmax_min_q),
        minmax_max_q(_minmax_max_q),
        visible_u(_style_visible_u),
        color_u(_style_color_u),
        style_u(_style_style_u),
        visible_un(_style_visible_un),
        color_un(_style_color_un),
        style_un(_style_style_un),
        visible_ruler(_style_visible_ruler),
        color_ruler(_style_color_ruler),
        style_ruler(_style_style_ruler)
    {}
    static AnalysisGraphHorizontalCapacity2DViewSetting* getDefault();
    static AnalysisGraphHorizontalCapacity2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;
    bool getTitleVisible() const{return titleVisible;}
    int getAxisLabelSize() const{return axisLabelSize;}
    int getAxisScaleTextSize() const{return axisScaleTextSize;}
    int getTitleLabelSize() const{return titleLabelSize;}
    bool getMinMaxAutoQ() const{return minmax_auto_q;}
    double getMinMaxMinQ() const{return minmax_min_q;}
    double getMinMaxMaxQ() const{return minmax_max_q;}
    bool getVisibleU() const{return visible_u;}
    QColor getColorU() const{return color_u;}
    Qt::PenStyle getStyleU() const{return style_u;}
    bool getVisibleUn() const{return visible_un;}
    QColor getColorUn() const{return color_un;}
    Qt::PenStyle getStyleUn() const{return style_un;}
    bool getVisibleRuler()const{return visible_ruler;}
    QColor getColorRuler() const{return color_ruler;}
    Qt::PenStyle getStyleRuler() const{return style_ruler;}
protected:
    AnalysisGraphHorizontalCapacity2DViewSetting(){}
    bool titleVisible;
    int axisLabelSize;
    int axisScaleTextSize;
    int titleLabelSize;

    bool minmax_auto_q;
    double minmax_min_q;
    double minmax_max_q;

    bool visible_u;
    QColor color_u;
    Qt::PenStyle style_u;
    bool visible_un;
    QColor color_un;
    Qt::PenStyle style_un;
    bool visible_ruler;
    QColor color_ruler;
    Qt::PenStyle style_ruler;
};

/* グラフ　NMINT */
class POST3D_CLASS_DLLSPEC AnalysisGraphNmint2DViewSetting :public AnalysisGraphBaseViewSettings {
public:
    AnalysisGraphNmint2DViewSetting(const AnalysisGraphNmint2DViewSetting&);
    explicit AnalysisGraphNmint2DViewSetting(
            bool _titleVisible,
            QColor _axisColor,
            QColor _axisTextColor,
            QColor _symbolColor,
            int _axisLabelSize,
            int _axisScaleTextSize,
            int _titleTextSize,
            int _symbolSize,
            int _symbolLabelSize,

            bool _isAssignedNValue,
            double _minNValue,
            double _maxNValue,
            bool _isAssignedMyValue,
            double _minMyValue,
            double _maxMyValue,
            bool _isAssingedMzValue,
            double _minMzValue,
            double _maxMzValue,

            bool _outputLVisible,
            QColor _outputLColor,
            Qt::PenStyle _outputLStyle,
            bool _outputSVisible,
            QColor _outputSColor,
            Qt::PenStyle _outputSStyle,
            bool _outputUVisible,
            QColor _outputUColor,
            Qt::PenStyle _outputUStyle,
            bool _koteiVisible,
            QColor _koteiColor,
            Qt::PenStyle _koteiStyle,
            bool _hendouVisible,
            QColor _hendouColor,
            Qt::PenStyle _hendouStyle,
            bool _rulerVisible,
            QColor _rulerColor,
            Qt::PenStyle _rulerStyle
            )
        :
          AnalysisGraphBaseViewSettings(),
          titleVisible(_titleVisible),
          axisColor(_axisColor),
          axisTextColor(_axisTextColor),
          symbolColor(_symbolColor),

          axisLabelSize(_axisLabelSize),
          axisScaleTextSize(_axisScaleTextSize),
          titleTextSize(_titleTextSize),
          symbolSize(_symbolSize),
          symbolLabelSize(_symbolLabelSize),

          minmaxAuto_N(_isAssignedNValue),
          minNValue(_minNValue),
          maxNValue(_maxNValue),
          minmaxAuto_My(_isAssignedMyValue),
          minMyValue(_minMyValue),
          maxMyValue(_maxMyValue),
          minmaxAuto_Mz(_isAssingedMzValue),
          minMzValue(_minMzValue),
          maxMzValue(_maxMzValue),

          outputLVisible(_outputLVisible),
          outputLColor(_outputLColor),
          outputLStyle(_outputLStyle),
          outputSVisible(_outputSVisible),
          outputSColor(_outputSColor),
          outputSStyle(_outputSStyle),
          outputUVisible(_outputUVisible),
          outputUColor(_outputUColor),
          outputUStyle(_outputUStyle),
          koteiVisible(_koteiVisible),
          koteiColor(_koteiColor),
          koteiStyle(_koteiStyle),
          hendouVisible(_hendouVisible),
          hendouColor(_hendouColor),
          hendouStyle(_hendouStyle),
          rulerVisible(_rulerVisible),
          rulerColor(_rulerColor),
          rulerStyle(_rulerStyle)
    {}
    static AnalysisGraphNmint2DViewSetting* getDefault();
    static AnalysisGraphNmint2DViewSetting* readSettings(QSettings&);
    void saveSettings(QSettings&) const override;
    bool getTitleVisible() const{return titleVisible;}
    QColor getAxisColor() const{return axisColor;}
    QColor getAxisTextColor() const{return axisTextColor;}
    QColor getSymbolColor() const{return symbolColor;}
    int getAxisLabelSize() const{return axisLabelSize;}
    int getAxisScaleTextSize() const{return axisScaleTextSize;}
    int getTitleTextSize() const{return titleTextSize;}
    int getSymbolSize() const{return symbolSize;}
    int getSymbolLabelSize() const{return symbolLabelSize;}
    bool getMinMaxAutoN() const{return minmaxAuto_N;}
    double getMinNValue() const{return minNValue;}
    double getMaxNValue() const{return maxNValue;}
    bool getMinMaxAutoMy() const{return minmaxAuto_My;}
    double getMinMyValue() const{return minMyValue;}
    double getMaxMyValue() const{return maxMyValue;}
    bool getMinMaxAutoMz() const{return minmaxAuto_Mz;}
    double getMinMzValue() const{return minMzValue;}
    double getMaxMzValue() const{return maxMzValue;}
    bool getOutputLVisible() const{return outputLVisible;}
    QColor getOutputLColor() const{return outputLColor;}
    Qt::PenStyle getOutputLStyle() const{return outputLStyle;}
    bool getOutputSVisible() const{return outputSVisible;}
    QColor getOutputSColor() const{return outputSColor;}
    Qt::PenStyle getOutputSStyle() const{return outputSStyle;}
    bool getOutputUVisible() const{return outputUVisible;}
    QColor getOutputUColor() const{return outputUColor;}
    Qt::PenStyle getOutputUStyle() const{return outputUStyle;}
    bool getKoteiVisible() const{return koteiVisible;}
    QColor getKoteiColor() const{return koteiColor;}
    Qt::PenStyle getKoteiStyle() const{return koteiStyle;}
    bool getHendouVisible() const{return hendouVisible;}
    QColor getHendoColor() const{return hendouColor;}
    Qt::PenStyle getHendouStyle() const{return hendouStyle;}
    bool getRulerVisible() const{return rulerVisible;}
    QColor getRulerColor() const{return rulerColor;}
    Qt::PenStyle getRulerStyle() const{return rulerStyle;}
protected:
    AnalysisGraphNmint2DViewSetting(){}
    bool titleVisible;
    QColor axisColor;
    QColor axisTextColor;
    QColor symbolColor;

    int axisLabelSize;
    int axisScaleTextSize;
    int titleTextSize;
    int symbolSize;
    int symbolLabelSize;

    bool minmaxAuto_N;
    double minNValue;
    double maxNValue;

    bool minmaxAuto_My;
    double minMyValue;
    double maxMyValue;

    bool minmaxAuto_Mz;
    double minMzValue;
    double maxMzValue;

    bool outputLVisible;
    QColor outputLColor;
    Qt::PenStyle outputLStyle;
    bool outputSVisible;
    QColor outputSColor;
    Qt::PenStyle outputSStyle;
    bool outputUVisible;
    QColor outputUColor;
    Qt::PenStyle outputUStyle;

    bool koteiVisible;
    QColor koteiColor;
    Qt::PenStyle koteiStyle;
    bool hendouVisible;
    QColor hendouColor;
    Qt::PenStyle hendouStyle;

    bool rulerVisible;
    QColor rulerColor;
    Qt::PenStyle rulerStyle;
};

/* 解析　伏・軸　表示設定 */

class POST3D_CLASS_DLLSPEC Analysis2DViewSettings{
public:
    Analysis2DViewSettings(const Analysis2DViewSettings&);
    static Analysis2DViewSettings getDefault();
    static Analysis2DViewSettings readSettings(const QString&);
    void saveSettings(const QString&);
    AnalysisBaseViewSettings* getSetting(OUTFIGTYPE type) const;
    void setSetting(OUTFIGTYPE type, AnalysisBaseViewSettings* viewSetting);
    bool UseScaledFont() const {return useScaledFont;}
    void SetUseScaledFont(bool _scaledFont);
private:
    Analysis2DViewSettings(){}
    QHash<int, AnalysisBaseViewSettings*> settings;
    bool useScaledFont;
};




} // namespace post3dapp

#endif // DEFINE_OUTPUT2DSETTINGS_H
