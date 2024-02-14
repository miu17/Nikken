#include "unified_settings.h"

#include <QStringList>
#include <QDebug>

#include "define_unifieddata.h"
#include "persUI_scene.h"
namespace post3dapp{
UnifiedSettings *UnifiedSettings::myInstance = 0;

UnifiedSettings::UnifiedSettings()
    : input2D_ViewSettings(Input2DViewSettings::getDefault()),
      input3D_ViewSettings(Input3DViewSettings::getDefault()),
      calculationLoad2D_ViewSettings(CalculationLoad2DViewSettings::getDefault()),
      calculationLoad3D_ViewSettings(CalculationLoad3DViewSettings::getDefault()),
      analysis2D_ViewSettings(Analysis2DViewSettings::getDefault()),
      analysis3D_ViewSettings(Analysis3DViewSettings::getDefault())
{
}

// 　表示設定

void UnifiedSettings::uploadViewSettings( const Input2DViewSettings &settings )
{
    input2D_ViewSettings = settings;
    emit in2D_SettingChanged();
}

void UnifiedSettings::uploadViewSettings( const Input3DViewSettings &settings )
{
    input3D_ViewSettings = settings;
    emit in3D_SettingChanged();
}

void UnifiedSettings::uploadViewSettings( const Analysis2DViewSettings &settings )
{
    analysis2D_ViewSettings = settings;
    emit out2D_SettingChanged();
}
void UnifiedSettings::uploadViewSettings( const Analysis3DViewSettings &settings )
{
    analysis3D_ViewSettings = settings;
    emit out3D_SettingChanged();
}

void UnifiedSettings::uploadViewSettings( const CalculationLoad2DViewSettings &settings )
{
    calculationLoad2D_ViewSettings = settings;
    emit calc2D_SettingChanged();
}
void UnifiedSettings::uploadViewSettings( const CalculationLoad3DViewSettings &settings )
{
    calculationLoad3D_ViewSettings = settings;
    emit calc3D_SettingChanged();
}

void UnifiedSettings::signalViewSettingChanged(MODELVIEWTYPE mtype)
{
    if ( mtype == INPUT2DMODEL || mtype == INPUT2DMODELCOPY) {
        emit in2D_SettingChanged();
    } else if ( mtype == INPUT3DMODEL ) {
        emit in3D_SettingChanged();
    } else if ( mtype == CALC2DMODEL ) {
        emit calc2D_SettingChanged();
    } else if ( mtype == CALC3DMODEL ) {
        emit calc3D_SettingChanged();
    } else if ( mtype == OUTPUT2DMODEL ) {
        emit out2D_SettingChanged();
    } else if ( mtype == OUTPUT3DMODEL ) {
        emit out3D_SettingChanged();
    }
}

bool post3dapp::UnifiedSettings::read(const QString &filepath)
{
    if (!QFileInfo::exists(filepath))
        return false;

    uploadViewSettings(Input2DViewSettings::readSettings(filepath));
    uploadViewSettings(Input3DViewSettings::readSettings(filepath));
    uploadViewSettings(CalculationLoad2DViewSettings::readSettings(filepath));
    uploadViewSettings(CalculationLoad3DViewSettings::readSettings(filepath));
    uploadViewSettings(Analysis2DViewSettings::readSettings(filepath));
    uploadViewSettings(Analysis3DViewSettings::readSettings(filepath));
    return true;
}

} // namespace post3dapp
