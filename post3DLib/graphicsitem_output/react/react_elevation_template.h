#ifndef REACTELEVATIONTEMPLATE_H
#define REACTELEVATIONTEMPLATE_H

#include "figure_template_elevation.h"
namespace post3dapp{

class OutputGrScene;
class ReactElevationTemplate :public FigureTemplateElevation
{
    Q_OBJECT

public:
    ReactElevationTemplate(OUTFIGTYPE, QUuid,bool);
    void createJointItem(OutputGrScene *scene,OUTFIG_TYPE_OPTION option,JointData *jd, int ino, const QPointF &pp)override;
    void updateViewSettings(OUTPUT2DVIEWSETTINGS settings)override{
            viewSetting = &settings.reactionForce;
    }
};
} // namespace post3dapp

#endif // REACTELEVATIONTEMPLATE_H
