#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLsbra::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"ブレース符号  仕上                    +---------- 鉄骨寸法 ---------+ +-コンクリート断面-+ +--主筋--+ +---フープ---+").append("\n");
    second.append(u8"              重量  タイプ  鉄骨材種        A  x   B  x  t1  x  t2       b      D           n  D       n  D").append("\n");
    second.append("\n");
    Q_FOREACH(auto sec, fixedData.getBraceList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
