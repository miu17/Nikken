#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLsswa::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"鋼板壁符号  板厚  仕上  +------ タテ ------+  +------ ヨコ ------+");
    second.append(u8"                  重量   ピッチ スチフナー材   ピッチ スチフナー材");
    Q_FOREACH(auto sec, fixedData.getSteelWallList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
