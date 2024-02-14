#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLsfwa::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"                               +------------------タテ筋------------------+ +----------ヨコ筋-----------+").append("\n");
    second.append(u8"地下外壁符号  厚さ   仕上      +--- 上部 ---+ +--- 中部 ---+ +--- 下部 ---+ +--- 端部 ---+ +--- 中央 ---+").append("\n");
    second.append(u8"                     重量        D    D         D   D          D   D          D   D          D   D").append("\n");
    Q_FOREACH(auto sec, fixedData.getFoundationWallList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
