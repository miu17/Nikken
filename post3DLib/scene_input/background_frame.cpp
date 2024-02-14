#include <QtGui>
#include <QObject>

#include "background_frame.h"
#include "calc_vector2d.h"
#include "unified_inputdata.h"
namespace post3dapp{
BackGroundFrame::BackGroundFrame(bool _useScaledFont, UnifiedData *data)
    : BaseBackGroundFrame(_useScaledFont, data)
{
}

} // namespace post3dapp
