#include <QtGui>
#include <QObject>

#include "background_frame.h"
#include "calc_vector2d.h"
#include "copy_background_frame.h"
#include "unified_inputdata.h"
namespace post3dapp{
CopyBackGroundFrame::CopyBackGroundFrame(bool _useScaledFont, UnifiedInputData* input)
    :BaseBackGroundFrame(_useScaledFont, input)
{
}
} // namespace post3dapp
