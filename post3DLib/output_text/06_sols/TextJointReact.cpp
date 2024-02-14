#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextJointReact::write(const FixedData& fixedData) const{
    //PR813

    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, getJapanese()};
}
} // namespace post3dapp
