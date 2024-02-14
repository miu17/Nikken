#include "abstracttext.h"
#include "fixed_data.h"
 namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLdm::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"   符号   方向    wa        wb        コメント").append("\n");
    second.append(u8"                kN/m      kN/m").append("\n");
    second.append("\n");
    Q_FOREACH(auto load, fixedData.getMemberLoadList()){
        second.append(load).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};}
} // namespace post3dapp
