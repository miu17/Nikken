#include "calcload_background_floor.h"

#include <QtGui>
#include <QObject>

#include "calc_vector2d.h"
#include "fixed_data.h"

namespace post3dapp{
CalcLoadBackGroundFloor::CalcLoadBackGroundFloor(bool _useScaledFont, UnifiedData* data)
    : BaseBackGroundFloor(_useScaledFont, data)
{
}
} // namespace post3dapp
