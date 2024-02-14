#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLdl::write(const FixedData&) const{
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, QString{}};
}
} // namespace post3dapp
