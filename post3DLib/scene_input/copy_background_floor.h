#ifndef COPYBACKGROUNDFLOOR_H
#define COPYBACKGROUNDFLOOR_H

#include "base_background.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QPainter;
namespace post3dapp{
class UnifiedInputData;
class POST3D_CLASS_DLLSPEC CopyBackGroundFloor : public BaseBackGroundFloor
{

public:
    CopyBackGroundFloor(bool _useScaledFont, UnifiedInputData*);
};
} // namespace post3dapp
#endif // COPYBACKGROUNDFLOOR_H
