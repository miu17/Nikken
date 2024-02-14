#ifndef DEFINE_INPUT2DSETTINGS_H
#define DEFINE_INPUT2DSETTINGS_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

#include "define_settings.h"

namespace post3dapp{

/* 形状　伏・軸　表示設定 */
class POST3D_CLASS_DLLSPEC Input2DViewSettings{

public:
    Input2DViewSettings(const Input2DViewSettings&);
    explicit Input2DViewSettings(
        bool _prior,
        bool _line,
        const QColor &_normal,
        const QColor &_focus,
        int _jsize,
        int _msize,
        int _lsize,
        const QHash<ELEMENTSTATE, COLORBRUSHSETTING> &_stateViewSetting,
        const QHash<ELEMENTTYPE, ELEMENTVIEW> &_typeViewSetting,
        const ELEMENTVIEW& _crossElementView,
        const DOWNSETTING& _downColorBrushSetting,
        const QMap<QString, COLORBRUSHSETTING> &_nameToViewSetting,
        bool _useScaledFont,
        int _scaleSize)
        :
          isPriorIndivisual(_prior),
          isLineDirVisible(_line),
          scaleColor(_normal),
          scaleFocusColor(_focus),
          jointNameSize(_jsize),
          memberNameSize(_msize),
          loadNameSize(_lsize),
          StateViewSetting(_stateViewSetting),
          TypeViewSetting(_typeViewSetting),
          crossElementView(_crossElementView),
          downColorBrushSetting(_downColorBrushSetting),
          NameToViewSetting(_nameToViewSetting),
          useScaledFont(_useScaledFont),
          scaleSize(_scaleSize){ }
    static Input2DViewSettings getDefault();
    static Input2DViewSettings makeOutputColorSetting();
    static Input2DViewSettings makeSymbolSetting(const Input2DViewSettings&);
    static Input2DViewSettings makeLoadSetting(const Input2DViewSettings&);
    static Input2DViewSettings readSettings(const QString&);
    void saveSettings(const QString&) const;

    bool getIsPriorIndividual() const {return isPriorIndivisual;}
    bool getIsLineDirVisible() const {return isLineDirVisible;}
    QColor getScaleColor() const{return scaleColor;}
    QColor getScaleFocusColor() const {return scaleFocusColor;}
    int getJointNameSize() const {return jointNameSize;}
    int getMemberNameSize() const {return memberNameSize;}
    int getLoadNameSize() const {return loadNameSize;}
    QList<QString> getNameKeys() const {return NameToViewSetting.keys();}
    COLORBRUSHSETTING getElementStateView(ELEMENTSTATE state) const;
    bool isVisibleElement(ELEMENTTYPE etype) const;
    ELEMENTVIEW getElementView(ELEMENTTYPE etype) const;

    COLORBRUSHSETTING getIndividualSetting( const QString &name) const;
    bool existIndividualSetting(const QString &name) const;

    ELEMENTVIEW getCrossElementView() const {return crossElementView;}
    DOWNSETTING getDownColorBrushSetting() const {return downColorBrushSetting;}

    bool UseScaledFont() const{return useScaledFont;}
    void SetUseScaledFont(bool _useScaledFont);
    int getElementNameSize( ELEMENTTYPE) const;

private:   
    explicit Input2DViewSettings(){}
    void overwriteElementType(ELEMENTTYPE, bool, bool);

    bool isPriorIndivisual;
    bool isLineDirVisible;
    QColor scaleColor;
    QColor scaleFocusColor;
    int jointNameSize;
    int memberNameSize;
    int loadNameSize;
    QHash<ELEMENTSTATE, COLORBRUSHSETTING> StateViewSetting;
    QHash<ELEMENTTYPE, ELEMENTVIEW> TypeViewSetting;
    ELEMENTVIEW crossElementView;
    DOWNSETTING downColorBrushSetting;
    QMap<QString, COLORBRUSHSETTING> NameToViewSetting;

    bool useScaledFont;
    int scaleSize; // 軸・通りサイズ
};

} // namespace post3dapp

#endif // DEFINE_INPUT2DSETTINGS_H
