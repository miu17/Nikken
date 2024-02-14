#include "abstracttext.h"

#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLdf::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"   符号         床用    架構用1   架構用2   架構用3   架構用4   地震用    コメント").append("\n");
    second.append(u8"                N/m2      N/m2      N/m2      N/m2      N/m2      N/m2").append("\n");
    second.append("\n");
    Q_FOREACH(auto load, fixedData.getSlabLoadList()){
        second.append(load).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}

} // namespace post3dapp
