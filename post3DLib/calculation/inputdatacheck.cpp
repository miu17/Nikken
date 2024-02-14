#include "inputdatacheck.h"

#include <QProgressDialog>
namespace post3dapp{

InputDataCheck::InputDataCheck(QObject *parent) : QObject(parent)
{

}

QString InputDataCheck::executeCheck(QProgressDialog *dialog){
Q_UNUSED(dialog);
    QString err_msg;

    // データチェック
    // ねじれている

    //　壁と壁の交差

    // 小梁上の雑壁
    // 小梁に接している　上下つながっているか、片持ちかどうか、ぶら下がっている。　柱に接続した小梁で柱にも接している

    // 架構内雑壁かどうか
    // 架構内雑壁だったら全ての片が解析部材につながれていないといけない
    // フロアまたぎ

    // パラペットの壁
    // 大梁に接している　片持ち壁は剛性考慮なのではじかない




    return err_msg;
}

QString InputDataCheck::checkWall(){
    QString err_msg;
    return err_msg;
}

} // namespace post3dapp
