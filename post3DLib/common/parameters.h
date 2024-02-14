#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <QtCore>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
struct POST3D_CLASS_DLLSPEC PARAMETERS {
    const static qreal scene_x_origin;
    const static qreal scene_y_origin;
    const static qreal scene_width;
    const static qreal scene_height;
    const static qreal floor_setting_scene_x_origin;
    const static qreal floor_setting_scene_y_origin;
    const static qreal floor_setting_scene_width;
    const static qreal floor_setting_scene_height;
private:
    PARAMETERS() {}
};
} // namespace post3dapp
#endif
