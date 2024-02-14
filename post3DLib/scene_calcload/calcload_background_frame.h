#ifndef PAINTCALCFRAME_H
#define PAINTCALCFRAME_H

#include "background_frame.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QPainter;
class QColor;
class QLineF;


namespace post3dapp{

class POST3D_CLASS_DLLSPEC CalcLoadBackGroundFrame : public BaseBackGroundFrame
{

public:
    CalcLoadBackGroundFrame(bool, UnifiedData* data);

};
} // namespace post3dapp
#endif
