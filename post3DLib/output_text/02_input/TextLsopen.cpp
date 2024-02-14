#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLsopen::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"開口符号  +------ 参照開口符号 ------+  +-------------- 開口位置 --------------+  +---- 開口寸法 ---+   コメント").append("\n");
    second.append(u8"                                         横配置  :  X(mm)    縦配置  :  Y(mm)      幅(mm)  高さ(mm)").append("\n");
    second.append("\n");
    Q_FOREACH(auto sec, fixedData.getOpenList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
