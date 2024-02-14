#include "calcload_background_frame.h"

#include <QtGui>
#include <QObject>

#include "calc_vector2d.h"
#include "fixed_data.h"

namespace post3dapp{
CalcLoadBackGroundFrame::CalcLoadBackGroundFrame(bool _useScaledFont, UnifiedData* data)
    : BaseBackGroundFrame(_useScaledFont, data)
{
}
} // namespace post3dapp
