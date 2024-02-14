#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLsbs::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"                                             /----------- ベースプレート -----------/ /--- アンカーボルト ----/").append("\n");
    second.append(u8"        NAME     MAKER      規格                 B      D     T    hy    hz  C材種     ny  nz  径     F     lb").append("\n");
    second.append(u8"                                               (MM)   (MM)  (MM)  (MM)  (MM)                   (N/mm2)   (mm)").append("\n");
    second.append(u8"   MNO.      I      J LIST             FL     FR     GR1    GR2    IorJ  BASE").append("\n");
    second.append(u8"                                         横配置  :  X(mm)    縦配置  :  Y(mm)      幅(mm)  高さ(mm)").append("\n");
    Q_FOREACH(auto sec, fixedData.getBaseSList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
