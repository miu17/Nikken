#ifndef CHECK_ELEVATION_TEMPLATE_H
#define CHECK_ELEVATION_TEMPLATE_H
#include "numeric_elevation_template.h"
namespace post3dapp{

class CheckElevationTemplate :public NumericElevationTemplate
{
    Q_OBJECT
public:
    CheckElevationTemplate(const Analysis2DViewSettings& _set, const FIGURE_TYPE_PROPERTY& prop, QUuid uuid)
        :NumericElevationTemplate(_set, prop,uuid) {}
    bool useGradation() const{return getViewSettings()->getUseGradiationColor();}
    QColor okColor() const{return getViewSettings()->getValueColor();}
    QColor ngColor() const{return getViewSettings()->getNgColor();}
    AnalysisCheck2DViewSetting* getViewSettings() const override {return static_cast<AnalysisCheck2DViewSetting *>(setting.getSetting(option.type));}
private:

};
} // namespace post3dapp
#endif // CHECK_ELEVATION_TEMPLATE_H
