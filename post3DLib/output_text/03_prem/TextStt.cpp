#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextStt::write(const FixedData& fixedData) const{
    //PR022A
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, getJapanese()};
}
} // namespace post3dapp
