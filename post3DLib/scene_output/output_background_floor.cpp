#include "output_background_floor.h"

#include <QtGui>
#include <QObject>

#include "unified_data.h"

namespace post3dapp{
OutputBackGroundFloor::OutputBackGroundFloor(bool _useScaledFont, UnifiedData* data)
    : BaseBackGroundFloor(_useScaledFont, data)
{
}
} // namespace post3dapp
