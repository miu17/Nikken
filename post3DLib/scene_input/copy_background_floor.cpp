#include <QtGui>
#include <QObject>

#include "calc_vector2d.h"
#include "copy_background_floor.h"
#include "unified_inputdata.h"
namespace post3dapp{
CopyBackGroundFloor::CopyBackGroundFloor(bool _useScaledFont, UnifiedInputData* input)
    :BaseBackGroundFloor(_useScaledFont, input)
{
}

} // namespace post3dapp
