#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextJointDisp::write(const FixedData& fixedData) const{
    //PR811

    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, getJapanese()};
}
} // namespace post3dapp
