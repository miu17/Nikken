#ifndef DEFINE_INPUT3DSETTINGS_H
#define DEFINE_INPUT3DSETTINGS_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

#include "define_settings.h"

namespace post3dapp{

/* 形状　パース　表示設定 */
class POST3D_CLASS_DLLSPEC Input3DViewSettings{
public:
    Input3DViewSettings(const Input3DViewSettings&);
    explicit Input3DViewSettings(
        bool _prior,
        ELEMENTLIMITED _flstate,
        const QList<QUuid> &_flids,
        ELEMENTLIMITED _frstate,
        const QList<QUuid> &_frids,
        const QColor &_scolor,
        bool _frcheck,
        const QUuid &_fid,
        bool _flcheck,
        const QPointF &_pp,
        int _jsize,
        const QColor &_jcolor,
        int _msize,
        const QColor &_mcolor,
        const QHash<ELEMENTSTATE, COLORBRUSHSETTING> &_stateViewSetting,
        const QHash<ELEMENTTYPE, ELEMENTVIEW> &_typeViewSetting,
        const QMap<QString, COLORBRUSHSETTING> &_nameToViewSetting,
        bool _useScaledFont)
        : isPriorIndivisual(_prior),
          floorLimitedState(_flstate),
          limitedFloors(_flids),
          frameLimitedState(_frstate),
          limitedFrames(_frids),
          scaleColor(_scolor),
          frameScaleIsChecked(_frcheck),
          frameScaleFloorID(_fid),
          floorScaleIsChecked(_flcheck),
          floorScaleXY(_pp),
          jointNameSize(_jsize),
          jointNameColor(_jcolor),
          memberNameSize(_msize),
          memberNameColor(_mcolor),
          StateViewSetting(_stateViewSetting),
          TypeViewSetting(_typeViewSetting),
          NameToViewSetting(_nameToViewSetting),
          useScaledFont(_useScaledFont){ }

    static Input3DViewSettings getDefault();
    static Input3DViewSettings readSettings(const QString&);
    void saveSettings(const QString&) const;

    bool getIsPriorIndividual() const {return isPriorIndivisual;}
    ELEMENTLIMITED getFloorLimitedState() const {return floorLimitedState;}
    QList<QUuid> getLimitedFloors() const {return limitedFloors;}
    ELEMENTLIMITED getFrameLimitedState() const {return frameLimitedState;}
    QList<QUuid> getLimitedFrames() const {return limitedFrames;}
    QColor getScaleColor() const{return scaleColor;}
    bool getFrameScaleIsChecked() const {return frameScaleIsChecked;}
    QUuid getFrameScaleFloorID() const {return frameScaleFloorID;}
    bool getFloorScaleIsChecked() const {return floorScaleIsChecked;}
    QPointF getFloorScaleXY() const {return floorScaleXY;}
    int getJointNameSize() const {return jointNameSize;}
    QColor getJointNameColor() const {return jointNameColor;}
    int getMemberNameSize() const {return memberNameSize;}
    QColor getMemberNameColor() const {return memberNameColor;}

    COLORBRUSHSETTING getElementStateView(ELEMENTSTATE state) const;
    bool isVisibleElement(ELEMENTTYPE etype) const;
    ELEMENTVIEW getElementView(ELEMENTTYPE etype) const;

    QList<QString> getNameToViewSettingsKeys() const{return NameToViewSetting.keys();}

    COLORBRUSHSETTING getIndividualSetting( const QString &name) const;

    bool existIndividualSetting(const QString &name) const;
    int getElementNameSize( ELEMENTTYPE) const;
    QColor getElementNameColor( ELEMENTTYPE) const;

    bool UseScaledFont() const{return useScaledFont;}
    void SetUseScaledFont(bool);

private:
    explicit Input3DViewSettings(){}

    bool isPriorIndivisual;
    ELEMENTLIMITED floorLimitedState;
    QList<QUuid> limitedFloors;
    ELEMENTLIMITED frameLimitedState;
    QList<QUuid> limitedFrames;
    QColor scaleColor;
    bool frameScaleIsChecked;
    QUuid frameScaleFloorID;
    bool floorScaleIsChecked;
    QPointF floorScaleXY;
    int jointNameSize;
    QColor jointNameColor;
    int memberNameSize;
    QColor memberNameColor;

    QHash<ELEMENTSTATE, COLORBRUSHSETTING> StateViewSetting; // 入力補助要素の表示の有無に違いがある。　2D：あり　3D：なし
    QHash<ELEMENTTYPE, ELEMENTVIEW> TypeViewSetting;
    QMap<QString, COLORBRUSHSETTING> NameToViewSetting;

    bool useScaledFont;

};

} // namespace post3dapp

#endif // DEFINE_INPUT3DSETTINGS_H
