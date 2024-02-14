#include "output_background_frame.h"

#include <QtGui>
#include <QObject>

#include "unified_data.h"
namespace post3dapp{
OutputBackGroundFrame::OutputBackGroundFrame(bool _useScaledFont, UnifiedData* data)
    : BaseBackGroundFrame(_useScaledFont, data)
{
}

} // namespace post3dapp
