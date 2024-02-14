#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextAxialSpring::write(const FixedData& fixedData) const{
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, getJapanese()};
}
} // namespace post3dapp
