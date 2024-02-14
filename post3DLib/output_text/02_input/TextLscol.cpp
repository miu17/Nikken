#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLscol::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8" 柱符号  +-- 階 --+  仕上 構造 コンクリート 配筋    主筋   +-- フープ --+  鉄骨    +---- 鉄骨断面(y) -----+  +---- 鉄骨断面(z) -----+").append("\n");
    second.append(u8"                     重量 形式    Dy   Dz  タイプ ny nz D  ny nz  D        材種         A  x  B  x  t1 x t2       A  x  B  x  t1 x t2").append("\n");
    second.append("\n");
    Q_FOREACH(auto sec, fixedData.getColumnRcList()){
        second.append(sec).append("\n");
    }
    Q_FOREACH(auto sec, fixedData.getColumnSrcList()){
        second.append(sec).append("\n");
    }
    Q_FOREACH(auto sec, fixedData.getColumnSList()){
        second.append(sec).append("\n");
    }
    Q_FOREACH(auto sec, fixedData.getColumnCftList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}

} // namespace post3dapp
