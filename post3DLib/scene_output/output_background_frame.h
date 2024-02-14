#ifndef PAINTOUTFRAME_H
#define PAINTOUTFRAME_H

#include "base_background.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QPainter;

namespace post3dapp{
class UnifiedData;


class POST3D_CLASS_DLLSPEC OutputBackGroundFrame : public BaseBackGroundFrame
{

public:

    OutputBackGroundFrame(bool _useScaledFont, UnifiedData* data);
//軸伏せの関数が一致　→　親クラスに移植
//不一致　→　一致度によって親クラスで共通部分を記述できるか、親クラスを純粋仮想にするか判断

protected:
    bool IsAnalysisFigure() override{return true;}

private:

};
} // namespace post3dapp
#endif
