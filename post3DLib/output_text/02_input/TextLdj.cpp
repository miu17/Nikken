#include "abstracttext.h"

#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLdj::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"   符号   タイプ   Fx     Fy     Fz     Mx     My     Mz    コメント").append("\n");
    second.append(u8"                kN      kN      kN      kN*m      kN*m      kN*m").append("\n");
    second.append("\n");
    Q_FOREACH(auto load, fixedData.getJointLoadList()){
        second.append(load).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
