#ifndef PAINTOUTFLOOR_H
#define PAINTOUTFLOOR_H

#include "base_background.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QPainter;
namespace post3dapp{
class UnifiedData;

class POST3D_CLASS_DLLSPEC OutputBackGroundFloor : public BaseBackGroundFloor
{

public:

    OutputBackGroundFloor(bool _useScaledFont, UnifiedData* data);


};
} // namespace post3dapp
#endif
