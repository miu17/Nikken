#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFloor::write(const FixedData& fixedData) const{
    //PR041
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, getJapanese()};
}
} // namespace post3dapp
