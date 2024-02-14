#ifndef TORSIONELEVATIONTEMPLATE_H
#define TORSIONELEVATIONTEMPLATE_H


#include "numeric_elevation_template.h"
namespace post3dapp{

class TorsionElevationTemplate :public NumericElevationTemplate
{
    Q_OBJECT
public:
    TorsionElevationTemplate(const Analysis2DViewSettings& set, const FIGURE_TYPE_PROPERTY& prop, QUuid uuid)
        :NumericElevationTemplate(set, prop, uuid){}
protected:
//    OutputStress2DViewSetting* getViewSettings() override {return &viewSetting;}
private:

};
} // namespace post3dapp


#endif // TORSIONELEVATIONTEMPLATE_H
