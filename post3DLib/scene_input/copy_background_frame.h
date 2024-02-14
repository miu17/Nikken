#ifndef COPYBACKGROUNDFRAME_H
#define COPYBACKGROUNDFRAME_H

#include "base_background.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QPainter;
class QColor;
class QLineF;
namespace post3dapp{
class UnifiedInputData;
class POST3D_CLASS_DLLSPEC CopyBackGroundFrame : public BaseBackGroundFrame
{
public:
    CopyBackGroundFrame(bool, UnifiedInputData*);
};
} // namespace post3dapp

#endif // COPYBACKGROUNDFRAME_H
