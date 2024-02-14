#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextPnlmdl::write(const FixedData& fixedData) const{
    //OPEN + PR3DOUT

    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, getJapanese()};
}
} // namespace post3dapp
