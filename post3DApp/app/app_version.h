#ifndef APP_VERSION_H
#define APP_VERSION_H

#include "version.h"

///
/// MajorVersion: 大規模な機能追加。
/// MinorVersion: Widgetの形がかたって、p3dの形がかわったら？UnifiedDataの形式がかわっても？　ファイルの互換性。
/// MicroVersion: Libがかわっても。Solverが変わっても。Appだけの場合も。CalculationMakerがかわっても
/// MajorとMinorはAppとLibでそろっていてほしい（ユーザーは期待している)
/// Minorバージョンがかわったらプログラムで吸収してほしい。Majorバージョンはexeを変更するか
/// Post3D.exe 1.1 1.2 1.3
/// ユーザーが.p3dの形式を知りたい
/// 古いバージョンでも読みたい(昔に作ったデータを読み込みたい。実行は今)：
/// 古いバージョンで実行したい(古いのを再現したい)：Post3d -v:1.1　→　コマンド実行はいらない。サーバーに保管
/// 古いバージョンでセーブしたい　→　ない　Writeでは分岐しない
///
namespace post3dapp{
class AppVersion : public Version
{
public:
    explicit AppVersion(const QString& ver):Version(ver)
    {
        version = QVersionNumber::fromString(ver);
    }
    static AppVersion Current(){return AppVersion("1.0.0-App");}
    QString text() const override {return version.toString() + "-App";}
};
} // namespace post3dapp
#endif // APP_VERSION_H
