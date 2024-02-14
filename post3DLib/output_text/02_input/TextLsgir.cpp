#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLsgir::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8" 大梁符号 +-- 階 --+   仕上  構造   コンクリート断面  配筋 +-上端主筋-+ +-下端主筋-+ +スターラップ+ 鉄骨   +------- 鉄骨断面 -----+").append("\n");
    second.append(u8"                       重量  形式    b     D  ハンチ タイプ  n  n  n D    n  n  n D    n D          材種        A  x  B  x t1 x t2").append("\n");
    second.append("\n");
    Q_FOREACH(auto sec, fixedData.getGirderRcList()){
        second.append(sec).append("\n");
    }
    Q_FOREACH(auto sec, fixedData.getGirderSrcList()){
        second.append(sec).append("\n");
    }
    Q_FOREACH(auto sec, fixedData.getGirderSList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}

} // namespace post3dapp
