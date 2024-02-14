#ifndef PAINTFRAME_H
#define PAINTFRAME_H

#include <QColor>
#include "base_background.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QPainter;
namespace post3dapp{
class UnifiedData;


class POST3D_CLASS_DLLSPEC BackGroundFrame : public BaseBackGroundFrame
{

public:
    BackGroundFrame(bool _useScaledFont, UnifiedData* data);
};
} // namespace post3dapp
#endif
