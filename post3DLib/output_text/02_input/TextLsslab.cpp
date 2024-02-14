#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLsslab::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"                                   +---------------短辺方向配筋---------------+ +---------------長辺方向配筋---------------+").append("\n");
    second.append(u8" スラブ符号  厚さ デッキ  仕上     +--Ａ部端部--+ +--Ａ部中央--+ +--Ｂ部端中--+ +--Ａ部端部--+ +--Ａ部中央--+ +--Ｂ部端中--+").append("\n");
    second.append(u8"                          重量      D   D          D   D          D   D          D   D          D   D          D   D").append("\n");
    second.append("\n");
    Q_FOREACH(auto sec, fixedData.getSlabList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}

} // namespace post3dapp
