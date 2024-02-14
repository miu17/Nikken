#ifndef UNIFIEDSETTINGS_H
#define UNIFIEDSETTINGS_H

#include <QUuid>
#include "define_calculationload2dsettings.h"
#include "define_calculationload3dsettings.h"
#include "define_input2dsettings.h"
#include "define_input3dsettings.h"
#include "define_analysis2dsettings.h"
#include "define_analysis3dsettings.h"
#include "define_unifieddata.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
class QGLWidget;
namespace post3dapp{
enum class STRUCTTYPE;

class POST3D_CLASS_DLLSPEC UnifiedSettings : public QObject
{
    Q_OBJECT

public:
    static UnifiedSettings *getInstance()
    {
        if ( myInstance == NULL ) myInstance = new UnifiedSettings;
        return myInstance;
    }

    static void DeleteInstance()
    {
        if ( myInstance != NULL ) delete myInstance;
        myInstance = NULL;
    }

    bool read(const QString&);

    // 表示設定
    void uploadViewSettings( const Input2DViewSettings & );
    void uploadViewSettings( const Input3DViewSettings & );
    void uploadViewSettings( const CalculationLoad2DViewSettings &);
    void uploadViewSettings( const CalculationLoad3DViewSettings &);
    void uploadViewSettings( const Analysis2DViewSettings & );
    void uploadViewSettings( const Analysis3DViewSettings & );

    void signalViewLimitedChanged()
    {
        emit persLimitedChanged();
    }
    void signalViewSettingChanged(MODELVIEWTYPE);

    Input2DViewSettings getInput2DViewSettings() const
    {
        return input2D_ViewSettings;
    }
    Input3DViewSettings getInput3DViewSettings() const
    {
        return input3D_ViewSettings;
    }

    CalculationLoad2DViewSettings getCalc2DViewSettings() const
    {
        return calculationLoad2D_ViewSettings;
    }
    CalculationLoad3DViewSettings getCalc3DViewSettings() const
    {
        return calculationLoad3D_ViewSettings;
    }

    Analysis2DViewSettings getOutput2DViewSettings() const
    {
        return analysis2D_ViewSettings;
    }
    Analysis3DViewSettings getOutput3DViewSettings() const
    {
        return analysis3D_ViewSettings;
    }

signals:

    void in2D_SettingChanged();
    void in3D_SettingChanged();
    void calc2D_SettingChanged();
    void calc3D_SettingChanged();
    void out2D_SettingChanged();
    void out3D_SettingChanged();

    void persLimitedChanged();

private:

    static UnifiedSettings *myInstance;

    UnifiedSettings();
    UnifiedSettings(const UnifiedSettings &);
    UnifiedSettings &operator=(const UnifiedSettings &);

    /* 形状　伏図・軸図　表示設定 */
    Input2DViewSettings input2D_ViewSettings;

    /* 形状　パース　表示設定 */
    Input3DViewSettings input3D_ViewSettings;

    /* 荷重拾い　表示設定 */
    CalculationLoad2DViewSettings calculationLoad2D_ViewSettings;
    CalculationLoad3DViewSettings calculationLoad3D_ViewSettings;

    /* 解析　伏図・軸図　表示設定 */
    Analysis2DViewSettings analysis2D_ViewSettings;

    /* 解析　パース　表示設定 */
    Analysis3DViewSettings analysis3D_ViewSettings;


};
} // namespace post3dapp
#endif
