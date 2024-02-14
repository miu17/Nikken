#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLsbsrc::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"                 /-------------------------------- ベースプレート ------------------------------/ /------- アンカーボルト ---------/").append("\n");
    second.append(u8"        NAME     形状          B      D     T   hy1   hy2   hz1   hz2   sml    B0    D0  C材種    配置    ny1 nz1 ny2 nz2 径      F").append("\n");
    second.append(u8"                             (MM)   (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)                                     (N/mm2)").append("\n");
    second.append(u8"   MNO.      I      J LIST             FL     FR     GR1    GR2    BASE").append("\n");
    Q_FOREACH(auto sec, fixedData.getBaseSrcList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
