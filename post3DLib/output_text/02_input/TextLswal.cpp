#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLswal::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"                             +-------- 壁配筋 --------+ +-------- 開口補強配筋 --------+ +-------- 開口補強配筋 --------+").append("\n");
    second.append(u8" 壁符号  厚さ  仕上   配筋   +--タテ筋--+  +--ヨコ筋--+ +-タテ筋-+ +-ヨコ筋-+ +--斜筋--+ +-タテ筋-+ +-ヨコ筋-+ +--斜筋--+").append("\n");
    second.append(u8"               重量  タイプ   D   D         D   D          n  D       n  D       n  D       n  D       n  D       n  D").append("\n");
    second.append(u8"").append("\n");
    Q_FOREACH(auto sec, fixedData.getWallList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}

} // namespace post3dapp
